import serial
import sys

from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import pyqtSignal



class DataSender(QWidget) :

    S_ports = pyqtSignal(list, name='Ports') 

    # Класс отправки данных Ардуинке
    def __init__(self, port="/dev/ttyUSB0", speed=9600):
        super().__init__()
        self.port_ = port
        self.speed_ = speed
        self.is_connect_ = False
        self.connect(self.port_, self.speed_)


    def serial_ports(self):
        if sys.platform.startswith("win"):
            ports = ["COM%s" % (i + 1) for i in range(10)]
        elif sys.platform.startswith("linux"):
            ports = ['/dev/ttyUSB%s' % (i) for i in range(10)]
        else:
            raise EnvironmentError("Unsupported platform")
        print(ports)
        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result


    def connect(self, port, speed):
        try:
            self.ser = serial.Serial(port=port, baudrate=speed, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=0)
            self.is_connect_ = True
            print("connected")
        except serial.serialutil.SerialException:
            self.is_connect_ = False
            print("port is not avaliable")



    def send(self, data): # Функция отправки сообщения в порт
        self.ser.write(str.encode(data))

        # Говорим, что закончили отправку
        self.ser.write(b'E')

    def read(self):
        # Если данные есть, считать их и вернуть
        # Если нет, вернуть пустую строку
        if self.ser.in_waiting == 0:
            return ""
        return self.ser.readline().decode('utf-8')

    def is_connect(self):
        return self.is_connect_


    def get_ports(self):
        self.S_ports.emit(self.serial_ports())
