# Tray icon as a dasStdDlg primitive

The watchdog is the first consumer: a status icon people click to see the supervised
program's state and open its web page. The primitive lives in `stddlg` (module
`modules/dasStdDlg`) because that is where the platform UI bindings already are, and it
links into the static `bin/watchdog` through the module's static twin.

## Decisions

1. **Pumped, not threaded.** `tray_poll` drains the platform's tray events on the calling
   thread and hands each one to a block. Every backend can be pumped this way; macOS
   additionally requires the caller to be the main thread. No platform thread ever calls
   into daslang.
2. **Icons are RGBA8 pixels plus a size.** The caller decodes a PNG (dasStbImage) or paints
   pixels; each backend builds its native icon (HICON, NSImage, ARGB32 pixmap over D-Bus).
   No per-platform icon files.
3. **Linux is raw StatusNotifierItem plus dbusmenu over libdbus-1, dlopened at run time.**
   No GTK, no appindicator: the static watchdog keeps its zero-dependency deploy, and a box
   without a session bus or without libdbus reports no tray instead of failing to start.
   Headers come from `libdbus-1-dev` at build time (pkg-config); without them the Linux
   backend is the NotImplemented stub, exactly like the dialogs without GTK.
4. **Menus are part of the first cut.** XFCE's tray host delivers no right-click without a
   dbusmenu object, and the point of the icon is a status menu. Same API on all three.
5. **Notifications ride the tray.** `tray_notify` is a Windows balloon, a
   `org.freedesktop.Notifications` call on Linux, and osascript on macOS (an unbundled
   binary has no notification center access). The watchdog's spawn-based notifications stay
   until the tray owns them.

## API (module `stddlg`)

```
tray_available() : bool                  // a backend exists and its host is reachable
tray_create(tooltip : string) : bool     // one tray per process
tray_set_icon(pixels : array<uint8>; width : int; height : int)   // RGBA8, row-major
tray_set_tooltip(text : string)
tray_menu_clear(); tray_menu_add(id : int; label : string; enabled : bool; checked : bool)
tray_menu_add_separator(); tray_menu_commit()
tray_poll() <| $(ev : TrayEvent) { ... } // ev.kind : TrayEventKind (click, double_click, right_click, menu, notification), ev.id, ev.x, ev.y
tray_notify(title : string; body : string) : bool
tray_destroy()
```

## Backends

| | host object | click | menu | notify | pump |
|---|---|---|---|---|---|
| Linux | `org.kde.StatusNotifierItem-<pid>-1` at `/StatusNotifierItem`, registered with `org.kde.StatusNotifierWatcher` (re-registered on watcher restart) | `Activate`/`SecondaryActivate`/`ContextMenu` methods | `com.canonical.dbusmenu` at `/MenuBar` | `org.freedesktop.Notifications.Notify` | `dbus_connection_read_write_dispatch(conn, 0)` |
| Windows | `Shell_NotifyIcon` on a hidden message window | `WM_APP` callback messages | `TrackPopupMenu` on right click | `NIF_INFO` balloon | `PeekMessage` loop |
| macOS | `NSStatusItem` on an accessory-policy `NSApplication` | button action | `NSMenu` | osascript spawn | `nextEventMatchingMask` loop |

## Tests

- Linux, headless: `tests/stddlg/` spawns a private `dbus-daemon --session`, then a child
  daslang running the tray fixture with `DBUS_SESSION_BUS_ADDRESS` set (fio `spawn_process`
  env), and drives it with `dbus-send`: properties, `Activate` -> `click` event, `GetLayout`
  -> menu labels, dbusmenu `Event clicked` -> `menu` event, notify without a daemon -> false.
  Registration with a real watcher is proven on the daslinux VM (the panel shows the icon).
- macOS: the process reads its own status item back through NSStatusItem and a
  `screencapture -R` of the menu bar on the Mac.
- Windows: the fixture arm proves the availability gate (a service or ssh session reports
  unavailable and skips; an interactive desktop reaches ready). Nothing scriptable clicks the
  notification area, so click, menu and balloon are proven by a person at zen2.

## Follow-ups (not this arc)

- dbusmenu icons and submenus.
- Windows dark-mode aware icon variants.
- Watchdog: replace the spawn-based notifications with `tray_notify` once the tray is wired.
