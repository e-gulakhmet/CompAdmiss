import sys
if sys.platform == "win32":
    import wmi
import psutil
import PyQt5.QtCore


class ConfData() :
    def __init__(self, d_type, path, value) :
        self.d_type = d_type
        self.path = path
        self.value = value


class ConfigReader() :
    # Класс нужен для того, чтобы была возможность
    # переключаться между Linux и Windows
    def __init__(self) :
        self.conf_list = list()

    def read(self) :
        return self.conf_list



class WMIConfReader(ConfigReader) :
    # Класс измеряющий температуру в Windows

    def __init__(self, namespace) :
        super().__init__()
        self.namespace = namespace

    def read(self) :
        # Проходимся по каждому из сенсоров
        # Добавляем в список информацию по сенсорам
        self.conf_list.clear()
        for s in wmi.WMI(namespace=self.namespace).Sensor():
            self.conf_list.append(ConfData(s.SensorType, s.Name, s.Value))
        return self.conf_list



class psUtilConfReader(ConfigReader):
    def __init__(self):
        super().__init__()
    
    def read(self):
        temps = psutil.sensors_temperatures()
        for name, info in temps.items():
            for value in info:
                # Получаем данные о температуре процессора
                if name == "coretemp" and value.label == "Core 0":
                    self.conf_list.append(ConfData(u"Temperature", u"CPU Core", value.current))
                # Получаем данные о температуре видеокарты
                if name == "acpitz":
                    self.conf_list.append(ConfData(u"Temperature", u"GPU Core", value.current))
        return self.conf_list


class SensorList() :
    # Класс в котором содержатся все созданные сенсоры
    # и измеряется температрура, для всех добавленных сенсоров.
    # В этом смысл этого класса, чтобы проверять сразу все датчики
    # Функция добавления сенсора(создается список содержащий в себе сенсоры)
    # Функция обновления данных каждого имеющегося сенсора
    def __init__(self, config_reader) :
        self.config_reader = config_reader  # Указываем используемую конфигурацию
        self.sens_by_types = dict()  # sens_type: [sens1, sens2, ...]


    def add(self, sensor) :
        # Если в списке нет данного сенсора
        # Добавляем его
        if sensor.sens_type not in self.sens_by_types:
            self.sens_by_types[sensor.sens_type] = list()

        self.sens_by_types[sensor.sens_type].append(sensor) 


    def update(self) :
        # Измеряем температуру и сохраняем ее
        for d in self.config_reader.read():
            # Заходим в раздел температур
            if d.d_type in self.sens_by_types:
                # Проходимся по каждому типу сенсоров
                for s in self.sens_by_types[d.d_type] :
                    if d.path == s.dev_path:
                        # Обновляем данные каждого сенсора
                        s.update(d)


class Sensor(PyQt5.QtCore.QObject) :
    # Класс создания сенсора, который затем передаем в SensorList
    # Функия update обновляет данные сенсора(одного)
    
    value_changed = PyQt5.QtCore.pyqtSignal(str, int, name='valueChanged')

    def __init__(self, name, sens_type, dev_path) :
        super().__init__()
        self.name = name
        self.sens_type = sens_type
        self.dev_path = dev_path
        self.value = 0

    def update(self, conf_data) :
        """
        Checks sensor data and update if it changed
        """
        # Обновляем данные сенсора
        # Посылаем сигнал, что данные поменялись
        # Если температура изменилась
        if conf_data.value != self.value :
            # Изменяем данные
            self.value = conf_data.value
            # Выбрасываем их
            self.value_changed.emit(str(self.name), int(self.value))