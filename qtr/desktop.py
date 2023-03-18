import configparser
import copy

class Desktop:
    def __init__(self, fname):
        self.config = configparser.ConfigParser()
        self.config.optionxform = str
        self.config.read(fname, encoding='utf8')
        self.icon_path = 'flatpak-runner'

    @property
    def app(self):
        return self.config['Desktop Entry']['X-Flatpak']
    
    @property
    def icon(self):
        return self.config['Desktop Entry'].get('Icon', None)

    @property
    def is_app(self):
        return 'Desktop Entry' in self.config and 'X-Flatpak' in self.config['Desktop Entry']

    @property
    def name(self):
        return self.config['Desktop Entry']['Name']

    def save_processed(self, fname):
        nc = copy.deepcopy(self.config)
        nexe = "flatpak-runner " + self.app
        nc['Desktop Entry']['Exec'] = nexe
        nc['Desktop Entry']['Icon'] = self.icon_path
        nc['Desktop Entry']['X-Nemo-Single-Instance']='no'
        nc['Desktop Entry']['X-Nemo-Application-Type']='no-invoker'
        with open(fname, 'w', encoding='utf8') as f:
            nc.write(f)

    def set_icon_path(self, path):
        self.icon_path = path
