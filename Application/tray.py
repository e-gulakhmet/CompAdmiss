from PyQt5.QtWidgets import QSystemTrayIcon, QMenu, QAction
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSignal



class TrayCofiguration(QSystemTrayIcon):

    S_widget_state = pyqtSignal(bool, name='WidgetState')

    def __init__(self, file):
        super().__init__()
        
        self.file_ = file

        # Иконка для треея
        self.setIcon(QIcon(self.file_))
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
