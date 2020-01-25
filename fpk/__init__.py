from .apps import refresh_apps as refresh_apps_impl
from .extension import has_extension as has_extension_impl
from .extension import remove_extension as remove_extension_impl
from .extension import sync_extension as sync_extension_impl

# externally available functions

def has_extension():
    return has_extension_impl()

def refresh_apps(just_delete = False):
    return refresh_apps_impl(just_delete=just_delete)

def remove_extension():
    remove_extension_impl()

def sync_extension():
    sync_extension_impl()
