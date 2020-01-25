import os
import pathlib
import shutil
from os.path import expanduser

extpath = os.path.join(expanduser("~"),
                       ".local/share/flatpak/extension/org.freedesktop.Platform.GL.host/arm/1.4")

def has_extension():
    return os.path.exists(extpath)

def remove_extension():
    if os.path.exists(extpath):
        shutils.rmtree(extpath)

def sync_extension():
    script = os.path.join( pathlib.Path(__file__).parent.parent.absolute(),
                           "scripts", "flatpak-extension-hybris")
    print(script)
    os.system(script)
