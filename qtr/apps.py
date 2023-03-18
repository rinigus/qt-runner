import glob, os
from pyotherside import send
from os.path import expanduser

from .desktop import Desktop

home = expanduser("~")
src_folders = [ os.path.join(home, ".local/share/flatpak/exports/share") ]

def get_icon(icon, extension):
    sz = 0
    f = None
    for D in src_folders:
        for i in glob.glob(os.path.join(D, "icons") + "/**/" + icon + "." + extension, recursive=True):
            n = os.path.getsize(i)
            if n > sz:
                f, sz = i, n
    return f

def refresh_apps(just_delete = False):
    desktop_prefix = "flatpak-runner-autogen-"
    target_folder = os.path.join(home, ".local/share/applications")
    gen_icons_folder = os.path.join(home, ".local/share/icons/flatpak-runner-autogen")
    
    # remove all autogenerated entries first
    for i in glob.glob(os.path.join(target_folder, desktop_prefix + "*")):
        os.remove(i)
    for i in glob.glob(os.path.join(gen_icons_folder, desktop_prefix + "*")):
        os.remove(i)

    if just_delete:
        return []

    apps = []
    
    #    # scan provided desktop files
    #    for D in src_folders:
    #        for i in glob.glob(os.path.join(D, "applications", "*.desktop")):
    #            d = Desktop(i)
    #            if not d.is_app: continue
    #            fpk = d.app
    #            tfile = os.path.join(target_folder, desktop_prefix + fpk + ".desktop")
    #            if os.path.exists(tfile): continue
    #            icon = get_icon(d.icon, 'png')
    #            if icon is not None:
    #                d.set_icon_path(icon)
    #            else:
    #                icon = get_icon(d.icon, 'svg')
    #                if icon:
    #                    os.makedirs(gen_icons_folder, exist_ok=True)
    #                    n = os.path.join(gen_icons_folder, d.icon + ".png")
    #                    send("saveAsPng", icon, n)
    #                    d.set_icon_path(n)
    #            d.save_processed(tfile)
    #            apps.append( dict(flatpak = fpk,
    #                              name = d.name,
    #                              icon = d.icon_path) )
    return apps