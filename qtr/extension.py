import os
import pathlib
import shutil
from os.path import expanduser

def extpath(arch):
    if arch == "aarch64":
        base = ".local/share/flatpak/extension/org.freedesktop.Platform.GL.host/aarch64/1.4"
    else:
        base = ".local/share/flatpak/extension/org.freedesktop.Platform.GL.host/arm/1.4"
    return os.path.join(expanduser("~"), base)

def has_extension(arch):
    return os.path.exists(extpath(arch))

def remove_extension(arch):
    if os.path.exists(extpath(arch)):
        shutil.rmtree(extpath(arch))

def sync_extension(arch):
    if arch == "aarch64":
        script = "flatpak-extension-hybris-aarch64"
    else:
        script = "flatpak-extension-hybris"
    script = os.path.join( pathlib.Path(__file__).parent.parent.absolute(),
                           "scripts", script)
    os.system(script)
