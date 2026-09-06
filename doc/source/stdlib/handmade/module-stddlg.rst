The stddlg module binds the operating system's standard dialogs and the system
tray (menu bar on macOS, StatusNotifierItem on Linux desktops): message boxes,
open and save file pickers, and a single tray icon with a tooltip, a context menu,
click events and notifications.

The tray is pumped, not threaded: the host calls ``tray_poll`` from its own loop
and receives every event through the block it passes. Icons are RGBA8 pixels
plus a size, so one image serves all platforms:

.. code-block:: das

    require stddlg
    require daslib/fio

    [export]
    def main() {
        if (!tray_create("my tool")) {
            return
        }
        tray_menu_add(1, "Open status page", true, false)
        tray_menu_add_separator()
        tray_menu_add(2, "Quit", true, false)
        tray_menu_commit()
        var quit = false
        while (!quit) {
            tray_poll() $(ev : TrayEvent) {
                if (ev.kind == TrayEventKind.menu && ev.id == 2) {
                    quit = true
                }
            }
            sleep(50u)
        }
        tray_destroy()
    }

``tray_available`` reports whether this process can show an icon at all: a Linux
box without a session bus, a macOS process off the main thread or without a window
server session, and a Windows service session all report false, and ``tray_create``
then returns false.

There is one tray per process, owned by the thread that called ``tray_create``;
every other ``tray_*`` call, including ``tray_poll``, belongs to that thread. On
macOS ``tray_create`` gives an unbundled process an accessory activation policy and
``tray_poll`` pumps the application's whole event queue.
