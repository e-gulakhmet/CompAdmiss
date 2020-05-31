import sys
from PyQt5.QtWidgets import QLabel, QWidget, QSizePolicy
from PyQt5.QtWidgets import QGridLayout
from PyQt5.QtCore import QTimer, Qt

import sensors



class MyLabel(QLabel) :
    # Класс добавляющий функцию к родительскому классу QLabel
    def __init__(self):
        super().__init__()
    
    # Переводим int в str
    def update_from_sensor(self, temp) :
        self.setText(str(temp))



class DataPresenter(QWidget) :
    # Класс предсталяющий данные по температуре
    def __init__(self) :
        super().__init__()  # Даем команду, чтобы все подключаемые библиотеки прогрузились
        self.sensor_info = {'CPU_Temperature': 0, 'GPU_Temperature': 0} # Содержит значения температур
        self.changed = True

        # Позволяем виджету изменять свои размеры при изменении размеров окна
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        # Cоздаем сенсоры
        self.cpuTemp = sensors.Sensor("CPU_Temperature", u"Temperature", u"CPU Core")
        self.gpuTemp = sensors.Sensor("GPU_Temperature", u"Temperature", u"GPU Core")

        # Создаем список сенсоров
        if sys.platform == "win32":
            self.sensList = sensors.SensorList(sensors.WMIConfReader("root\OpenHardwareMonitor"))
        elif sys.platform == "linux":
            self.sensList = sensors.SensorList(sensors.psUtilConfReader())
        else:
            raise OSError("Unsupported platform")

        # Добавляем сенсоры в список
        self.sensList.add(self.cpuTemp)
        self.sensList.add(self.gpuTemp)

        # Создаем строки с информацией о температуре
        cpuNameLabel = QLabel("CPU Temperature:")
        cpuNameLabel.setObjectName("PresenterLabel")

        self.cpuTempLabel = MyLabel()
        self.cpuTempLabel.setObjectName("MenuTempLabel")

        gpuNameLabel = QLabel("GPU Temperature:")
        gpuNameLabel.setObjectName("PresenterLabel")

        self.gpuTempLabel = MyLabel()
        self.gpuTempLabel.setObjectName("MenuTempLabel")                    


        # Создаем пространство в виде коробки с коробками
        gridLayout = QGridLayout()
        gridLayout.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        
        # Добавляем пустрое растяжимое пространство перед слайдером и после него
        gridLayout.setColumnStretch(1, 1)
        # Добавляем вертикальные пробелы

        # Добавляем виджеты в определенные координаты
        gridLayout.addWidget(cpuNameLabel, 0, 0)
        gridLayout.addWidget(self.cpuTempLabel, 0, 1)
        gridLayout.addWidget(gpuNameLabel, 1, 0)
        gridLayout.addWidget(self.gpuTempLabel, 1, 1)

        self.setLayout(gridLayout)
        
        # Подключаемся к сигнам о температурах
        self.cpuTemp.value_changed.connect(self.updateInfo)
        self.gpuTemp.value_changed.connect(self.updateInfo)
        
        # Создаем таймер, для того чтобы температура 
        # измерялась только раз в секунду
        timer = QTimer(self)
        timer.setInterval(5000)
        timer.setSingleShot(False)
        timer.timeout.connect(self.sensList.update)  # Обновляем данные по температуре
        timer.start(5000)


    def updateInfo(self, sensor_type, value) :
        # Обновляем значения
        self.changed = (self.sensor_info[sensor_type] != value)
        self.sensor_info[sensor_type] = value
        self.cpuTempLabel.setText(str(self.sensor_info[self.cpuTemp.name]))
        self.gpuTempLabel.setText(str(self.sensor_info[self.gpuTemp.name]))

    
    def get_data(self):
        data = str(self.sensor_info[self.cpuTemp.name]) + ";"
        data += str(self.sensor_info[self.gpuTemp.name]) + ";"
        return data    
