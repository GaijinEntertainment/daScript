options gen2
options no_aot

require stddlg
require daslib/fio

//! The tray under test for test_tray.das: one icon, a four-entry menu, then a poll loop that
//! reports every event as an `@event` line and returns 0 once the Quit entry (id 30) fires.
def say(line : string) {
    print("{line}\n")
    fflush(fstdout())
}

[export]
def main() : int {
    if (!tray_available()) {
        say("@unavailable")
        return 2
    }
    if (!tray_create("daslang tray fixture")) {
        say("@create_failed")
        return 3
    }
    let rgba <- [for (v in [255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 128]); uint8(v)]
    tray_set_icon(rgba, 2, 2)
    tray_menu_clear()
    tray_menu_add(10, "Open status page", true, false)
    tray_menu_add_separator()
    tray_menu_add(20, "Paused", false, true)
    tray_menu_add(30, "Quit", true, false)
    tray_menu_commit()
    say("@notify {tray_notify("daslang tray fixture", "hello")}")
    say("@ready")
    var quit = false
    let started = ref_time_ticks()
    while (!quit && get_time_usec(started) < 60000000) {
        tray_poll() $(ev : TrayEvent) {
            say("@event {ev.kind} {ev.id} {ev.x} {ev.y}")
            if (ev.kind == TrayEventKind.menu && ev.id == 30) {
                quit = true
            }
        }
        sleep(10u)
    }
    tray_destroy()
    return quit ? 0 : 4
}
