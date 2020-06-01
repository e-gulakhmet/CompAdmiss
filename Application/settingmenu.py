from PyQt5.QtWidgets import QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QFrame, QSizePolicy, QComboBox
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QIcon


class SettingMenu(QFrame):

    S_port_info = pyqtSignal(str, str, name='PortInfo')
    S_save_butt_clicked = pyqtSignal(bool, name='SaveButtClicked')
    S_update_ports = pyqtSignal(bool, name="UpdateButtClicked")
    

    def __init__(self, port, port_speed):
        super().__init__()
        self.setting_info_ = {'Port': port, 'Speed': port_speed}
        self.ports_ = {}
        speeds_ = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"}
        

        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

        portEditLabel = QLabel("Port Number:")
        portEditLabel.setObjectName("SettMenuLabel")

        self.portBox = QComboBox()
        self.portBox.addItems(self.ports_)
        self.portBox.setCurrentText(self.setting_info_["Port"])

        speedEditLabel = QLabel("Port Speed: ")
        speedEditLabel.setObjectName("SettMenuLabel")

        self.speedBox = QComboBox()
        self.speedBox.addItems(speeds_)
        self.speedBox.setCurrentText(self.setting_info_["Speed"])

        configSaveButt = QPushButton("SAVE")
        configSaveButt.setObjectName("SettMenuSaveButt")
        configSaveButt.clicked.connect(self.set_port_info)

        updateButt = QPushButton("Update")   
        updateButt.clicked.connect(self.act_update_ports)    


        hPortBoxLay = QHBoxLayout()

        hPortBoxLay.addWidget(portEditLabel)
        hPortBoxLay.addWidget(self.portBox)
        hPortBoxLay.addSpacing(20)
        hPortBoxLay.addWidget(speedEditLabel)
        hPortBoxLay.addWidget(self.speedBox)
        hPortBoxLay.addStretch()

        saveButtHBoxLay = QHBoxLayout()
        saveButtHBoxLay.addWidget(configSaveButt)

        
        updateButtHBoxLay = QHBoxLayout()
        updateButtHBoxLay.addWidget(updateButt)

        # Создаем вертикальное пространство
        vBoxLay = QVBoxLayout()
        # Прикрепляем пространство к левому верхнему краю
        vBoxLay.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        vBoxLay.addLayout(hPortBoxLay)
        vBoxLay.addStretch(1)
        vBoxLay.addLayout(saveButtHBoxLay)
        vBoxLay.addLayout(updateButtHBoxLay)

        self.setLayout(vBoxLay)

        self.hide()



    def set_port_info(self):
        self.setting_info_['Port'] = self.portBox.currentText()
        self.setting_info_['Speed'] = self.speedBox.currentText()
        self.S_port_info.emit(self.setting_info_["Port"], self.setting_info_['Speed'])
        self.S_save_butt_clicked.emit(True)

    def update_ports(self, ports):
        print("ports updated")
        self.ports_ = ports
        self.portBox.clear()
        self.portBox.addItems(self.ports_)
    
    def act_update_ports(self):
        self.S_update_ports.emit(True)