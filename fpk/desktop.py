import configparser
import copy

class Desktop:
    def __init__(self, fname):
        self.config = configparser.ConfigParser()
        self.config.optionxform = str
        self.config.read(fname)

    @property
    def app(self):
        return self.config['Desktop Entry']['X-Flatpak']
    
    @property
    def icon(self):
        return self.config['Desktop Entry'].get('Icon', None)

    @property
    def is_app(self):
        return 'Desktop Entry' in self.config and 'X-Flatpak' in self.config['Desktop Entry']

    def save_processed(self, fname):
        nc = copy.deepcopy(self.config)
        nexe = "flatpak-runner " + self.app
        nc['Desktop Entry']['Exec'] = nexe
        nc['Desktop Entry']['Icon'] = 'flatpak-runner'
        nc['Desktop Entry']['X-Nemo-Single-Instance']='no'
        nc['Desktop Entry']['X-Nemo-Application-Type']='no-invoker'
        with open(fname, 'w') as f:
            nc.write(f)
