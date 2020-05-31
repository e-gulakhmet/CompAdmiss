import configparser


class Configurator() :

    def __init__(self, file) :
        self.file_ = file

        self.config_data = configparser.ConfigParser()
        self.config_data.read(self.file_)

    def save(self):
        with open("config.ini", 'w') as configfile:
            self.config_data.write(configfile)
        print("saved")

    def save_port(self, port, speed):
        self.config_data["SETTING"]["port"] = port
        self.config_data["SETTING"]["port_speed"] = speed      

    def save_fan_info(self, mode, cpu_temp, gpu_temp):
        self.config_data["FAN"]["mode"] = mode
        self.config_data["FAN"]["step_cpu_temp"] = cpu_temp
        self.config_data["FAN"]["step_gpu_temp"] = gpu_temp        
        
    def save_light_info(self, main_mode, mode, bright, speed):
        self.config_data["LIGHT"]["main_mode"] = main_mode
        self.config_data["LIGHT"]["mode"] = mode
        self.config_data["LIGHT"]["brightness"] = bright
        self.config_data["LIGHT"]["speed"] = speed

    def save_light_extra_info(self, cpu_temp, gpu_temp, color):
        self.config_data["LIGHT"]["max_cpu_temp"] = cpu_temp
        self.config_data["LIGHT"]["max_gpu_temp"] = gpu_temp 
        self.config_data["LIGHT"]["color"] = color 
