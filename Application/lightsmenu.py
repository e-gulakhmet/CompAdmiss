from PyQt5.QtWidgets import QVBoxLayout, QHBoxLayout, QComboBox, QLabel, QTextEdit, QPushButton
from PyQt5.QtWidgets import QWidget, QSizePolicy, QSlider
from PyQt5.QtCore import Qt, pyqtSignal



class LightsMenu(QWidget) :
    S_light_info = pyqtSignal(str, str, str, str, name='LightInfo')
    S_light_extra_info = pyqtSignal(str, str, str, name='LightExtraInfo')

    def __init__(self, mainmode, mode, brightness, speed, max_cpu_temp, max_gpu_temp, color):
        super().__init__()
        lights_modes = ["TempAdapt", "RandomColor", "Pulse", "Propeller", "Rainbow", "Flicker", "RandomRed",
                        "Radiation", "WhiteTemps", "QuaqBright", "Sparkle"]
        self.info = {"main_mode": int(mainmode), "mode": int(mode), "bright": int(brightness), "speed": int(speed)}
        self.extra_info = {"max_cpu_temp": int(max_cpu_temp), "max_gpu_temp": int(max_gpu_temp), "color": int(color)}

        
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        modeLabel = QLabel("Mode:")
        modeLabel.setObjectName("LightsMenuLabel")

        # ОСНОВНЫЕ НАСТРОЙКИ ПОДСВЕТКИ
        modeComboBox = QComboBox() # Выпадающий список
        modeComboBox.setObjectName("LightsMenuBox")
        modeComboBox.addItems(lights_modes)
        modeComboBox.setCurrentIndex(self.info["mode"])
        modeComboBox.activated.connect(self.set_mode)
        modeComboBox.activated.connect(self.extra_menu_ctrl)

        # Откючение или включение подсветки
        mainModeLabelOff = QLabel("OFF")
        mainModeLabelOff.setObjectName("LightsMenuMainModeLabel")

        mainModeLabelOn = QLabel("ON")
        mainModeLabelOn.setObjectName("LightsMenuMainModeLabel")

        mainModeSlider = QSlider(Qt.Horizontal)
        mainModeSlider.setObjectName("LightsMenuMainModeSlider")
        mainModeSlider.setRange(0, 1)
        mainModeSlider.setFixedSize(50, 25)
        mainModeSlider.setValue(self.info["main_mode"])
        mainModeSlider.valueChanged.connect(self.set_main_mode)

        # Настройка яркости подсветки
        brightnessLabel = QLabel("Brightness:")
        brightnessLabel.setObjectName("LightsMenuLabel")

        brightSlider = QSlider(Qt.Horizontal)
        brightSlider.setObjectName("LightsMenuSlider")
        brightSlider.setRange(0, 255)
        brightSlider.setFixedSize(200, 25)
        brightSlider.setValue(self.info["bright"])
        brightSlider.valueChanged.connect(self.set_bright)

        self.brightInfoLabel = QLabel(str(self.info["bright"]))
        self.brightInfoLabel.setObjectName("LightsMenuSliderLabel")

        # Настройка скорости подсветки
        speedLabel = QLabel("Speed:")
        speedLabel.setObjectName("LightsMenuLabel")

        speedSlider = QSlider(Qt.Horizontal)
        speedSlider.setObjectName("LightsMenuSlider")
        speedSlider.setRange(0, 200)
        speedSlider.setFixedSize(200, 25)
        speedSlider.setValue(self.info["speed"])
        speedSlider.valueChanged.connect(self.set_speed)

        self.speedInfoLabel = QLabel(str(self.info["speed"]))
        self.speedInfoLabel.setObjectName("LightsMenuSliderLabel")


        # Настройка максимальной температуры cpu для подсветки
        maxCpuTempLabel = QLabel("Max CPU Temperature:")

        self.maxCpuTempEditText = QTextEdit(str(self.extra_info["max_cpu_temp"]))
        self.maxCpuTempEditText.setFixedSize(40, 28)

        # Кнопки для увеличения или уменьшения темпераратуры
        minCpuTempButt = QPushButton('-')
        minCpuTempButt.clicked.connect(self.min_cpu_temp)
        minCpuTempButt.setFixedWidth(25)

        plusCpuTempButt = QPushButton('+')
        plusCpuTempButt.clicked.connect(self.plus_cpu_temp)
        plusCpuTempButt.setFixedWidth(25)


        # Настройка максимальной температуры gpu для подсветки
        maxGpuTempLabel = QLabel("Max Gpu Temperature:")

        self.maxGpuTempEditText = QTextEdit(str(self.extra_info["max_gpu_temp"]))
        self.maxGpuTempEditText.setFixedSize(40, 28)

        # Кнопки для увеличения или уменьшения темпераратуры
        minGpuTempButt = QPushButton('-')
        minGpuTempButt.clicked.connect(self.min_gpu_temp)
        minGpuTempButt.setFixedWidth(25)

        plusGpuTempButt = QPushButton('+')
        plusGpuTempButt.clicked.connect(self.plus_gpu_temp)
        plusGpuTempButt.setFixedWidth(25)


        # Настройка цвета режима
        colorLabel = QLabel("Color:")

        colorSlider = QSlider(Qt.Horizontal)
        colorSlider.setRange(0, 255)
        colorSlider.setFixedSize(200, 25)
        colorSlider.setValue(self.extra_info["color"])
        colorSlider.valueChanged.connect(self.set_color)

        self.colorInfoLabel = QLabel(str(self.extra_info["color"]))



        # Пространство для выбора режима и главного режима
        hModeLay = QHBoxLayout()
        hModeLay.addWidget(modeLabel)
        hModeLay.addWidget(modeComboBox)
        hModeLay.addSpacing(20)
        hModeLay.addWidget(mainModeLabelOff)
        hModeLay.addWidget(mainModeSlider)
        hModeLay.addWidget(mainModeLabelOn)
        hModeLay.addStretch(1)

        # Пространство для настройки яркости
        hBrightLay = QHBoxLayout()
        hBrightLay.addWidget(brightnessLabel)
        hBrightLay.addWidget(brightSlider)
        hBrightLay.addWidget(self.brightInfoLabel)
        hBrightLay.addStretch(1)

        # Пространство для настройки скорости
        hSpeedLay = QHBoxLayout()
        hSpeedLay.addWidget(speedLabel)
        hSpeedLay.addWidget(speedSlider)
        hSpeedLay.addWidget(self.speedInfoLabel)
        hSpeedLay.addStretch(1)

        # Пространство для настройки цвета
        hColorLay = QHBoxLayout()
        hColorLay.addWidget(colorLabel)
        hColorLay.addWidget(colorSlider)
        hColorLay.addWidget(self.colorInfoLabel)
        hColorLay.addStretch(1)
        
        # Пространство для настройки максимальной температуры cpu
        hMaxCpuTempLay = QHBoxLayout()
        hMaxCpuTempLay.addWidget(maxCpuTempLabel)
        hMaxCpuTempLay.addWidget(minCpuTempButt)
        hMaxCpuTempLay.addWidget(self.maxCpuTempEditText)
        hMaxCpuTempLay.addWidget(plusCpuTempButt)
        hMaxCpuTempLay.addStretch(1)
        # Пространство для настройки максимальной температуры gpu
        hMaxGpuTempLay = QHBoxLayout()
        hMaxGpuTempLay.addWidget(maxGpuTempLabel)
        hMaxGpuTempLay.addWidget(minGpuTempButt)
        hMaxGpuTempLay.addWidget(self.maxGpuTempEditText)
        hMaxGpuTempLay.addWidget(plusGpuTempButt)
        hMaxGpuTempLay.addStretch(1)
        # Создаем виджеты для настроек определенных эффектов
        # Мы используем виджеты, так как layout не может становиться невидимым
        self.cpuLayWidget = QWidget()
        self.cpuLayWidget.setLayout(hMaxCpuTempLay)
        self.gpuLayWidget = QWidget()
        self.gpuLayWidget.setLayout(hMaxGpuTempLay)
        if (modeComboBox.currentIndex() != 0):
            self.gpuLayWidget.hide()
            self.cpuLayWidget.hide()


        # Основоное пространство
        self.vBoxLay = QVBoxLayout()
        self.vBoxLay.setAlignment(Qt.AlignTop | Qt.AlignLeft)     
        self.vBoxLay.addLayout(hModeLay)
        self.vBoxLay.addLayout(hBrightLay)
        self.vBoxLay.addLayout(hSpeedLay)
        self.vBoxLay.addLayout(hColorLay)
        self.vBoxLay.addLayout
        self.vBoxLay.addStretch()
        self.vBoxLay.addWidget(self.cpuLayWidget)
        self.vBoxLay.addWidget(self.gpuLayWidget)

        self.setLayout(self.vBoxLay)
        self.hide()


    def extra_menu_ctrl(self, index):
        if (index == 0):
            self.gpuLayWidget.show()
            self.cpuLayWidget.show()
        else:
            self.gpuLayWidget.hide()
            self.cpuLayWidget.hide()
        

    
    def set_bright(self, value): # Изменяем яркость подсветки
        self.info["bright"] = value
        self.brightInfoLabel.setText(str(value))
        print("l_b:", value)

    def set_speed(self, speed):
        self.info["speed"] = speed
        self.speedInfoLabel.setText(str(self.info["speed"]))
        print("l_s", speed)

    def set_mode(self, index):
        self.info["mode"] = index
        print("l_m:", index)
    
    def set_main_mode(self, index):
        self.info["main_mode"] = index
        print("l_m_m", index)

    def min_cpu_temp(self):
        self.extra_info["max_cpu_temp"] -= 5
        self.maxCpuTempEditText.setText(str(self.extra_info["max_cpu_temp"]))
    
    def plus_cpu_temp(self):
        self.extra_info["max_cpu_temp"] += 5
        self.maxCpuTempEditText.setText(str(self.extra_info["max_cpu_temp"]))

    def min_gpu_temp(self):
        self.extra_info["max_gpu_temp"] -= 5
        self.maxCpuTempEditText.setText(str(self.extra_info["max_gpu_temp"]))
    
    def plus_gpu_temp(self):
        self.extra_info["max_gpu_temp"] += 5
        self.maxCpuTempEditText.setText(str(self.extra_info["max_gpu_temp"]))

    def set_color(self, index):
        self.extra_info["color"] = index
        self.colorInfoLabel.setText(str(index))
    
    def save_info(self):
        self.S_light_info.emit(str(self.info["main_mode"]), str(self.info["mode"]), str(self.info["bright"]), str(self.info["speed"]))
        
    def save_extra_info(self):
        self.S_light_extra_info.emit(str(self.extra_info["max_cpu_temp"]), str(self.extra_info["max_gpu_temp"]), str(self.extra_info["color"]))

    def get_data(self):
        data = str(self.info["main_mode"]) + ";"
        data += str(self.info["mode"]) + ";"
        data += str(self.info["bright"]) + ";"
        data += str(self.info['speed']) + ";"
        data += str(self.extra_info["max_cpu_temp"]) + ";"
        data += str(self.extra_info["max_gpu_temp"]) + ";"
        data += str(self.extra_info["color"]) + ";"
        return data
