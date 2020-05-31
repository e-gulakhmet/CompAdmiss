from PyQt5.QtWidgets import QFrame, QSizePolicy, QHBoxLayout, QVBoxLayout, QSlider, QLabel, QTextEdit, QPushButton
from PyQt5.QtCore import Qt, pyqtSignal


class FanMenu(QFrame) :

    S_fan_info = pyqtSignal(str, str, str, name="FanInfo")

    def __init__(self, mode, step_cpu_temp, step_gpu_temp):
        super().__init__()
        self.info = {"mode": int(mode), "step_cpu_temp": int(step_cpu_temp), "step_gpu_temp": int(step_gpu_temp)} # Данные меню вентилятораG
        self.mode_name = ["OFF", "ON", "AUTO"]

        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        # Настрока режима вентилятора
        switchLabel = QLabel("Fan Mode:")
        switchLabel.setObjectName("FanMenuLabel")

        self.switchSliderLabel = QLabel(self.mode_name[self.info["mode"]])
        self.switchSliderLabel.setObjectName("FanMenuSliderLabel")     

        self.switchSlider = QSlider(Qt.Horizontal)
        self.switchSlider.setObjectName("FanMenuSlider")
        self.switchSlider.setFixedSize(50, 40)
        self.switchSlider.setTickPosition(QSlider.TicksBothSides)
        self.switchSlider.setRange(0, 2)
        self.switchSlider.setValue(self.info['mode'])
        self.switchSlider.valueChanged.connect(self.set_mode)


        # Настройка температуры при которой включается вентилятор
        stepCpuTempLabel = QLabel("Step CPU Temperature:")
        stepCpuTempLabel.setObjectName("FanMenuLabel")

        self.stepCpuTempEditText = QTextEdit(str(self.info["step_cpu_temp"]))
        self.stepCpuTempEditText.setObjectName("FanMenuEditText")
        self.stepCpuTempEditText.setFixedSize(40, 28)

        # Кнопки для увеличения или уменьшения темпераратуры
        minCpuTempButt = QPushButton('-')
        minCpuTempButt.setObjectName("FanMinTempButt")
        minCpuTempButt.clicked.connect(self.min_cpu_temp)
        minCpuTempButt.setFixedWidth(25)

        plusCpuTempButt = QPushButton('+')
        plusCpuTempButt.setObjectName("FanPlusTempButt")
        plusCpuTempButt.clicked.connect(self.plus_cpu_temp)
        plusCpuTempButt.setFixedWidth(25)


        # Настройка температуры видеокарты при которой включается вентилятор
        stepGpuTempLabel = QLabel("Step GPU Temperature:")
        stepGpuTempLabel.setObjectName("FanMenuLabel")

        self.stepGpuTempEditText = QTextEdit(str(self.info["step_gpu_temp"]))
        self.stepGpuTempEditText.setObjectName("FanMenuEditText")
        self.stepGpuTempEditText.setFixedSize(40, 28)

        # Кнопки для увеличения или уменьшения темпераратуры
        minGpuTempButt = QPushButton('-')
        minGpuTempButt.setObjectName("FanMinTempButt")
        minGpuTempButt.clicked.connect(self.min_gpu_temp)
        minGpuTempButt.setFixedWidth(25)

        plusGpuTempButt = QPushButton('+')
        plusGpuTempButt.setObjectName("FanPlusTempButt")
        plusGpuTempButt.clicked.connect(self.plus_gpu_temp)
        plusGpuTempButt.setFixedWidth(25)


        hSwitchLay = QHBoxLayout()
        hSwitchLay.addWidget(switchLabel)
        hSwitchLay.addWidget(self.switchSlider)
        hSwitchLay.addWidget(self.switchSliderLabel)
        hSwitchLay.addStretch()

        hCpuTempLay = QHBoxLayout()
        hCpuTempLay.addWidget(stepCpuTempLabel)
        hCpuTempLay.addWidget(minCpuTempButt)
        hCpuTempLay.addWidget(self.stepCpuTempEditText)
        hCpuTempLay.addWidget(plusCpuTempButt)
        hCpuTempLay.addStretch()

        hGpuTempLay = QHBoxLayout()
        hGpuTempLay.addWidget(stepGpuTempLabel)
        hGpuTempLay.addWidget(minGpuTempButt)
        hGpuTempLay.addWidget(self.stepGpuTempEditText)
        hGpuTempLay.addWidget(plusGpuTempButt)
        hGpuTempLay.addStretch()

        vBoxLay = QVBoxLayout()
        vBoxLay.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        vBoxLay.addLayout(hSwitchLay)
        vBoxLay.addLayout(hCpuTempLay)
        vBoxLay.addLayout(hGpuTempLay)
        vBoxLay.addStretch()

        self.setLayout(vBoxLay)
        self.hide()


    def min_cpu_temp(self): # Уменьшить температуру смены режима вентилятора
        if(self.info["step_cpu_temp"]) > 0:
            self.info["step_cpu_temp"] -= 5
        else:
            self.info["step_cpu_temp"] = 0
        self.stepCpuTempEditText.setText(str(self.info["step_cpu_temp"]))
        print(self.info["step_cpu_temp"])

    def plus_cpu_temp(self): # Увеличить температуру смены режима вентилятора
        if(self.info["step_cpu_temp"]) < 100:
            self.info["step_cpu_temp"] += 5
        else:
            self.info["step_cpu_temp"] = 100
        self.stepCpuTempEditText.setText(str(self.info["step_cpu_temp"]))
        print(self.info["step_cpu_temp"])

    def min_gpu_temp(self): # Уменьшить температуру смены режима вентилятора
        if(self.info["step_gpu_temp"]) > 0:
            self.info["step_gpu_temp"] -= 5
        else:
            self.info["step_gpu_temp"] = 0
        self.stepGpuTempEditText.setText(str(self.info["step_gpu_temp"]))
        print(self.info["step_gpu_temp"])

    def plus_gpu_temp(self): # Увеличить температуру смены режима вентилятора
        if(self.info["step_gpu_temp"]) < 100:
            self.info["step_gpu_temp"] += 5
        else:
            self.info["step_gpu_temp"] = 100
        self.stepGpuTempEditText.setText(str(self.info["step_gpu_temp"]))
        print(self.info["step_gpu_temp"])

    def set_cpu_step_temp(self, value):
        self.info["step_cpu_temp"] = value
        self.stepCpuTempEditText.setText(str(self.info["step_cpu_temp"]))
        print("f_c_s:", value)

    def set_gpu_step_temp(self, value):
        self.info["step_gpu_temp"] = value
        self.stepGpuTempEditText.setText(str(self.info["step_gpu_temp"]))
        print("f_g_s:", value)

    def set_mode(self, value): # Изменяем режим работы вентилятора
        # Изменияем цвет кнопок и устанавливаем значение режима
        self.switchSliderLabel.setText(self.mode_name[value])
        self.switchSlider.setValue(value)

        self.info['mode'] = value
        print("f_m:", value)

    def save_info(self):
        self.S_fan_info.emit(str(self.info["mode"]), str(self.info["step_cpu_temp"]), str(self.info["step_gpu_temp"]))
    
    
    def get_data(self):
        data = str(self.info['mode']) + ";"
        data += str(self.info["step_cpu_temp"]) + ";"
        data += str(self.info["step_gpu_temp"]) + ";"
        return data
