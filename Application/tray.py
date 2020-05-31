from PyQt5.QtWidgets import QSystemTrayIcon, QMenu, QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSignal



class TrayCofiguration(QSystemTrayIcon):

    S_widget_state = pyqtSignal(bool, name='WidgetState')

    def __init__(self):
        super().__init__()

        # Иконка для треея
        self.setIcon(QIcon("/home/whoman/wrk/development/arduino/CompAdmiss/Application/images/icon3.png"))
        self.setToolTip("CompAsmiss")
        self.activated.connect(self.open_widget)

        # Дейсвия которые вызываеются в tray
        openTrayAction = QAction("Show", self)
        openTrayAction.triggered.connect(self.open_widget)
  

        # Меню, которое появляется при нажатии на trayIcon
        trayMenu = QMenu()
        trayMenu.addAction(openTrayAction)

        self.setContextMenu(trayMenu)

        self.show()


    def open_widget(self):
        self.S_widget_state.emit(True)
