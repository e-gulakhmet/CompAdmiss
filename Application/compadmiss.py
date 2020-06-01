import sys
import random
import os
import argparse
from PyQt5.QtWidgets import QApplication, QVBoxLayout, QWidget, QMenuBar, QAction
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QIcon

import presenter
import lightsmenu
import fanmenu
import sender
import settingmenu
import configurator
import tray



# TODO: Закончить описание проекта



class MainWindow(QWidget) :
    # S_fan_mode = pyqtSignal(int, name='fanMode')
    # S_fan_cpu_step_temp = pyqtSignal(int, name='fanCpuStepTemp')
    # S_fan_gpu_step_temp = pyqtSignal(int, name='fanGpuStepTemp')
    # S_lights_mode = pyqtSignal(int, name='lightsMode')
    # S_lights_bright = pyqtSignal(int, name='lightsBright')
    # S_tray_visible = pyqtSignal(bool, name='trayVisible')

    def __init__(self) :
        super().__init__()

        # Задаем имя главному меню, чтобы потом использовать его в css файле
        self.setObjectName("MainWindow")

        args = self.create_parser()

        # Устанавливаем иконку приложения
        self.setWindowIcon(QIcon(os.path.join(os.getcwd(), args.path, "images/icon3.png")))
        # Устанавливаем размеры и расположение окна приложения
        self.setGeometry(200, 200, 400, 300)
        # Создание строки которое пишется в рамке приложения
        self.setWindowTitle("CompAdmiss")


        # Добавляем трей
        self.trayConf = tray.TrayCofiguration(os.path.join(os.getcwd(), args.path, "images/icon3.png"))
        # Подключаемся к управлению главным виджетом
        self.trayConf.S_widget_state.connect(self.change_widget_state)


        # Инициализируем класс сохраняющий конфигурцию приложения
        self.config = configurator.Configurator(os.path.join(os.getcwd(), args.path, "config.ini"))


        # Инициализируем класса отвечающий за выdод информации о температурах
        self.dp = presenter.DataPresenter()


        # Инициализируем класс отвечающий за настройку подсветки
        self.lm = lightsmenu.LightsMenu(self.config.config_data["LIGHT"]["main_mode"],
                                        self.config.config_data["LIGHT"]["mode"],
                                        self.config.config_data["LIGHT"]["brightness"],
                                        self.config.config_data["LIGHT"]["speed"],
                                        self.config.config_data["LIGHT"]["max_cpu_temp"],
                                        self.config.config_data["LIGHT"]["max_gpu_temp"],
                                        self.config.config_data["LIGHT"]["color"])
        # self.S_lights_mode.connect(self.lm.update_mode)
        # self.S_lights_bright.connect(self.lm.update_bright)


        # Инициализируем класс отвечающий за настройку вентилятора
        self.fm = fanmenu.FanMenu(self.config.config_data['FAN']['mode'], self.config.config_data['FAN']['step_cpu_temp'], self.config.config_data['FAN']['step_gpu_temp'])
        # self.S_fan_mode.connect(self.fm.update_mode)
        # self.S_fan_cpu_step_temp.connect(self.fm.set_cpu_step_temp)
        # self.S_fan_gpu_step_temp.connect(self.fm.set_gpu_step_temp)


        self.ds = sender.DataSender(self.config.config_data['SETTING']['port'], self.config.config_data['SETTING']['port_speed'])

        # Инициализируем класс настроек
        self.sm = settingmenu.SettingMenu(self.config.config_data['SETTING']['port'], self.config.config_data['SETTING']['port_speed'])
        self.sm.S_port_info.connect(self.ds.connect)
        self.sm.S_update_ports.connect(self.ds.get_ports)
        self.ds.S_ports.connect(self.sm.update_ports)


        # Создаем вертикальное пространство
        vBoxLay = QVBoxLayout()
        vBoxLay.setContentsMargins(0, 0, 0, 0)
        # Прикрепляем  к левому верхнему краю
        # vBoxLay.setAlignment(Qt.AlignTop | Qt.AlignLeft)

        # Добавляем в пространсво виджеты
        vBoxLay.addSpacing(20)
        vBoxLay.addWidget(self.dp)
        vBoxLay.addWidget(self.lm)
        vBoxLay.addWidget(self.fm)
        vBoxLay.addWidget(self.sm)

        self.setLayout(vBoxLay)


        # Cоздаем действия
        openHardInfo = QAction("Hardware", self)
        openHardInfo.triggered.connect(self.fm.hide)
        openHardInfo.triggered.connect(self.lm.hide)
        openHardInfo.triggered.connect(self.sm.hide)
        openHardInfo.triggered.connect(self.dp.show)

        openFanMenu = QAction("Fan", self)
        openFanMenu.triggered.connect(self.dp.hide)
        openFanMenu.triggered.connect(self.lm.hide)
        openFanMenu.triggered.connect(self.sm.hide)
        openFanMenu.triggered.connect(self.fm.show)

        openLightMenu = QAction("Light", self)
        openLightMenu.triggered.connect(self.dp.hide)
        openLightMenu.triggered.connect(self.sm.hide)
        openLightMenu.triggered.connect(self.fm.hide)
        openLightMenu.triggered.connect(self.lm.show)

        openSettingMenu = QAction("Settings", self)
        openSettingMenu.triggered.connect(self.lm.hide)
        openSettingMenu.triggered.connect(self.dp.hide)
        openSettingMenu.triggered.connect(self.fm.hide)
        openSettingMenu.triggered.connect(self.sm.show)
        openSettingMenu.triggered.connect(self.ds.get_ports)


        saveAct = QAction(QIcon(os.path.join(os.getcwd(), args.path, "images/save.png")), "Save", self)
        # Cохраняем данные о подсвектке
        saveAct.triggered.connect(self.lm.save_info)
        saveAct.triggered.connect(self.lm.save_extra_info)
        self.lm.S_light_info.connect(self.config.save_light_info)
        self.lm.S_light_extra_info.connect(self.config.save_light_extra_info)
        # Сохраняем данные которые были изменены в классе вентилятора
        saveAct.triggered.connect(self.fm.save_info)
        self.fm.S_fan_info.connect(self.config.save_fan_info)
        # Сохраняем данные о порте и настройках
        self.sm.S_port_info.connect(self.config.save_port)
        # Сохраняем файл настроек
        saveAct.triggered.connect(self.config.save)


        # Создаем меню
        menuBar = QMenuBar(self)
        menuBar.setObjectName("MenuBar")
        # Добавляем действия к меню
        menuBar.addAction(openHardInfo)
        menuBar.addAction(openFanMenu)
        menuBar.addAction(openLightMenu)
        menuBar.addAction(openSettingMenu)
        menuBar.addAction(saveAct)


        timer = QTimer(self)
        timer.setInterval(1000)
        timer.setSingleShot(False)
        if (self.ds.is_connect()):
            timer.timeout.connect(self.send_info)
        timer.start(1000)

        self.show()

    def create_parser(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("-p", "--path", default="")

        args = parser.parse_args()

        return args


    def change_widget_state(self, state):
        if(state and self.isVisible() is False):
            self.setVisible(True)
            self.setWindowState(Qt.WindowActive)
        elif(state is False and self.isVisible() is True):
            self.setVisible(False)
            self.close()

    def hideEvent(self, event) :
        self.hide()
        self.config.save()

    def send_info(self) : # Отравка данных в COM порт
        # # Проверить есть ли данные от контроллера
        # # Если данные есть, разобрать их и через сигналы отправить их в lights и fan
        # # Если данных нет, то отправить текущие данные
        # port_data = self.ds.read()
        # # Проверить наличие данных в порту
        # if(port_data != ""): # Если строка не пустая
        #     for e in port_data.split(';'): # Разделяем полученные данные по символу ';'
        #         vals = e[2:].split(',') # Разделяем разделенные данные по символу ',',
        #                                 # начиная со второго символа
        #         if len(e) == 0:
        #             continue
        #         if e[0:2] == "F:": # Если первые два символа равны "F:"
        #             self.S_fan_mode.emit(self._get_int(vals[0], 0, 2)) # Отправляем считанные значения
        #             self.S_fan_cpu_step_temp.emit(self._get_int(vals[1], 0, 100))
        #             self.S_fan_gpu_step_temp.emit(self._get_int(vals[2], 0, 100))
        #         elif e[0:2] == "L:":
        #             self.S_lights_mode.emit(self._get_int(vals[0], 0, 10))
        #             self.S_lights_bright.emit(self._get_int(vals[1], 0, 255, 100))
        #         else:
        #             print("invalid data from controller [", e, ']')

        # Получаем данные от подсветки и вентилятора
        data = str(random.randint(0, 255))
        data += self.dp.get_data()
        data += self.fm.get_data()
        data += self.lm.get_data()
        self.ds.send(data)


    # def _get_int(self, s, min, max, default=None): # Проверяем данные
    #     if default is None:
    #         default = min
    #     if min >= max:
    #         raise ValueError("_get_int max is not greater than min" , min, max)
    #     try:
    #         v = int(s)
    #         if v < min:
    #             v = min
    #         if v > max:
    #             v = max
    #     except ValueError:
    #         return default
    #     return v



def main() :
    # Инициализируем основной класс приложения
    qApp = QApplication(sys.argv)
    # Инициализируем основной класс окна меню
    mw = MainWindow()
    
    # Открываем и установливаем файл css для приложения
    with open("/home/whoman/wrk/development/arduino/CompAdmiss/Application/style.css") as f :
        qApp.setStyleSheet(f.read())

    sys.exit(qApp.exec_())


if __name__ == '__main__':
    main()