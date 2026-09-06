options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require strings

//! Linux tray over a private session bus: this daslang spawns `dbus-daemon`, then a child daslang
//! running the tray fixture with DBUS_SESSION_BUS_ADDRESS pointing at it, and drives the child's
//! StatusNotifierItem and dbusmenu objects with `dbus-send`. Registration with a real watcher is
//! the daslinux VM's job (the panel shows the icon); everything else is asserted here.
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : get_full_file_name(args[0])
}

def fixture_path() : string {
    return path_join(get_das_root(), "tests/stddlg/_fixture_tray_child.das")
}

def dbus_tools_present() : bool {
    return fexist("/usr/bin/dbus-daemon") && fexist("/usr/bin/dbus-send")
}

struct TrayRig {
    bus : process
    child : process
    address : string
    dest : string
    lines : array<string>
}

def drain_child(var rig : TrayRig) {
    unsafe(process_drain(rig.child) $(line) {
        rig.lines |> push(line)
    })
}

def wait_for_line(t : T?; var rig : TrayRig; prefix : string; seconds : float) : string {
    let started = ref_time_ticks()
    var seen = 0
    while (float(get_time_usec(started)) < seconds * 1000000.0) {
        drain_child(rig)
        while (seen < length(rig.lines)) {
            if (starts_with(rig.lines[seen], prefix)) {
                return rig.lines[seen]
            }
            seen++
        }
        sleep(5u)
    }
    t |> failure("child never printed a line starting with '{prefix}'; got {rig.lines}")
    return ""
}

def start_bus(t : T?; var rig : TrayRig) : bool {
    let argv <- ["/usr/bin/dbus-daemon", "--session", "--print-address", "--nofork", "--nopidfile"]
    let no_env : array<string>
    rig.bus = unsafe(spawn_process(argv, "", no_env))
    let started = ref_time_ticks()
    while (empty(rig.address) && get_time_usec(started) < 15000000) {
        unsafe(process_drain(rig.bus) $(line) {
            if (empty(rig.address) && starts_with(line, "unix:")) {
                rig.address = clone_string(line)
            }
        })
        sleep(5u)
    }
    if (empty(rig.address)) {
        t |> failure("dbus-daemon printed no address")
        return false
    }
    return true
}

def start_child(t : T?; var rig : TrayRig) : bool {
    let argv <- [das_exe(), fixture_path()]
    let env <- ["DBUS_SESSION_BUS_ADDRESS={rig.address}"]
    rig.child = unsafe(spawn_process(argv, "", env))
    let first = wait_for_line(t, rig, "@", 30.0)
    if (first == "@unavailable") {
        t |> skip("this daslang has no Linux tray backend on this box (built without libdbus-1-dev, or no libdbus-1.so.3), so the D-Bus arms have nothing to drive")
        return false
    }
    if (first == "@create_failed") {
        t |> failure("the child reached the bus but could not create the item")
        return false
    }
    if (empty(first) || empty(wait_for_line(t, rig, "@ready", 30.0))) {
        return false
    }
    rig.dest = "org.kde.StatusNotifierItem-{unsafe(process_pid(rig.child))}-1"
    return true
}

def stop_rig(var rig : TrayRig) {
    unsafe {
        if (rig.child != null) {
            close_process(rig.child)
            rig.child = null
        }
        if (rig.bus != null) {
            close_process(rig.bus)
            rig.bus = null
        }
    }
}

//! `dbus-send` against the child; `reply` asks for --print-reply and returns the dump, otherwise the
//! call is fire-and-forget and the result is empty.
def dbus_send(rig : TrayRig; reply : bool; path : string; method : string; args : array<string>) : string {
    var argv <- ["/usr/bin/dbus-send", "--bus={rig.address}"]
    if (reply) {
        argv |> push("--print-reply")
    } else {
        argv |> push("--type=method_call")
    }
    argv |> push("--dest={rig.dest}")
    argv |> push(path)
    argv |> push(method)
    argv |> push_from(args)
    let no_env : array<string>
    return build_string() $(writer) {
        unsafe {
            var p = spawn_process(argv, "", no_env)
            let started = ref_time_ticks()
            while (process_poll(p) == process_running && get_time_usec(started) < 10000000) {
                process_drain(p) $(line) {
                    writer |> write(line) |> write("\n")
                }
                sleep(5u)
            }
            process_drain(p) $(line) {
                writer |> write(line) |> write("\n")
            }
            close_process(p)
        }
    }
}

def with_rig(t : T?; blk : block<(var rig : TrayRig) : void>) {
    var rig : TrayRig
    if (!start_bus(t, rig)) {
        stop_rig(rig)
        return
    }
    if (!start_child(t, rig)) {
        stop_rig(rig)
        return
    }
    invoke(blk, rig)
    stop_rig(rig)
}

def has(t : T?; haystack : string; needle : string; what : string) {
    t |> success(find(haystack, needle) >= 0, "{what}: expected '{needle}' in\n{haystack}")
}

//! osascript against the child's status item through System Events; the dump is the script's
//! stdout and stderr, so a denied accessibility permission reads back as text, not a crash.
def apple_script(script : string) : string {
    let argv <- ["/usr/bin/osascript", "-e", script]
    let no_env : array<string>
    return build_string() $(writer) {
        unsafe {
            var p = spawn_process(argv, "", no_env)
            process_wait(p, 10.0)
            process_drain(p) $(line) {
                writer |> write(line) |> write("\n")
            }
            close_process(p)
        }
    }
}

//! Windows has no scriptable route to a real click on the notification area, so this arm only
//! proves the availability gate and the create path: a service or ssh session (no visible window
//! station) reports `@unavailable`, an interactive desktop reaches `@ready`.
[test]
def test_tray_windows_smoke(t : T?) {
    if (!fexist("C:\\Windows\\explorer.exe")) {
        t |> skip("not Windows")
        return
    }
    t |> run("the fixture reaches ready on an interactive desktop, or reports unavailable") @(t : T?) {
        var rig : TrayRig
        let argv <- [das_exe(), fixture_path()]
        let no_env : array<string>
        rig.child = unsafe(spawn_process(argv, "", no_env))
        let first = wait_for_line(t, rig, "@", 30.0)
        if (first == "@unavailable") {
            t |> skip("no interactive desktop for this process (session 0 or no taskbar): the icon needs a person at the box")
        } else {
            t |> equal(first, "@notify true")
            t |> equal(wait_for_line(t, rig, "@ready", 30.0), "@ready")
        }
        stop_rig(rig)
    }
}

[test]
def test_tray_macos_status_item(t : T?) {
    if (!fexist("/usr/bin/osascript")) {
        t |> skip("no /usr/bin/osascript here: this arm drives the macOS status item through System Events")
        return
    }
    t |> run("a System Events click on the status item arrives as a click event") @(t : T?) {
        var rig : TrayRig
        let argv <- [das_exe(), fixture_path()]
        let no_env : array<string>
        rig.child = unsafe(spawn_process(argv, "", no_env))
        let started = ref_time_ticks()
        var first = ""
        while (empty(first) && get_time_usec(started) < 30000000) {
            drain_child(rig)
            for (line in rig.lines) {
                if (starts_with(line, "@")) {
                    first = line
                    break
                }
            }
            sleep(5u)
        }
        if (first == "@unavailable") {
            t |> skip("no window server session for this process, so no status bar to put an item on")
            stop_rig(rig)
            return
        }
        if (empty(wait_for_line(t, rig, "@ready", 30.0))) {
            stop_rig(rig)
            return
        }
        let pid = unsafe(process_pid(rig.child))
        let clicked = apple_script("tell application \"System Events\" to tell (first process whose unix id is {pid}) to click menu bar item 1 of menu bar 1")
        if (find(clicked, "assistive access") >= 0 || find(clicked, "-1719") >= 0 || find(clicked, "-25211") >= 0) {
            t |> skip("System Events cannot reach the status item here (accessibility permission): {clicked}")
            stop_rig(rig)
            return
        }
        t |> equal(wait_for_line(t, rig, "@event click", 10.0), "@event click 0 0 0")
        stop_rig(rig)
    }
}

[test]
def test_tray_over_private_bus(t : T?) {
    if (!dbus_tools_present()) {
        t |> skip("no /usr/bin/dbus-daemon or dbus-send here: the Linux tray rides D-Bus, other platforms have their own arms")
        return
    }
    t |> run("the item exposes its StatusNotifierItem properties") @(t : T?) {
        with_rig(t) $(var rig : TrayRig) {
            let title = dbus_send(rig, true, "/StatusNotifierItem", "org.freedesktop.DBus.Properties.Get", ["string:org.kde.StatusNotifierItem", "string:Title"])
            has(t, title, "string \"daslang tray fixture\"", "Title")
            let all = dbus_send(rig, true, "/StatusNotifierItem", "org.freedesktop.DBus.Properties.GetAll", ["string:org.kde.StatusNotifierItem"])
            has(t, all, "string \"ApplicationStatus\"", "Category")
            has(t, all, "string \"Active\"", "Status")
            has(t, all, "object path \"/MenuBar\"", "Menu")
            has(t, all, "string \"IconPixmap\"", "IconPixmap key")
            let pixmap = dbus_send(rig, true, "/StatusNotifierItem", "org.freedesktop.DBus.Properties.Get", ["string:org.kde.StatusNotifierItem", "string:IconPixmap"])
            has(t, pixmap, "int32 2", "pixmap width")
            has(t, pixmap, "ff ff 00 00", "first pixel as ARGB")
            let no_args : array<string>
            let xml = dbus_send(rig, true, "/StatusNotifierItem", "org.freedesktop.DBus.Introspectable.Introspect", no_args)
            has(t, xml, "org.kde.StatusNotifierItem", "introspection")
        }
    }
    t |> run("Activate and ContextMenu arrive as click and right_click events") @(t : T?) {
        with_rig(t) $(var rig : TrayRig) {
            dbus_send(rig, false, "/StatusNotifierItem", "org.kde.StatusNotifierItem.Activate", ["int32:7", "int32:9"])
            t |> equal(wait_for_line(t, rig, "@event click", 10.0), "@event click 0 7 9")
            dbus_send(rig, false, "/StatusNotifierItem", "org.kde.StatusNotifierItem.ContextMenu", ["int32:1", "int32:2"])
            t |> equal(wait_for_line(t, rig, "@event right_click", 10.0), "@event right_click 0 1 2")
        }
    }
    t |> run("the menu layout carries labels, the separator and the checked disabled entry") @(t : T?) {
        with_rig(t) $(var rig : TrayRig) {
            let layout = dbus_send(rig, true, "/MenuBar", "com.canonical.dbusmenu.GetLayout", ["int32:0", "int32:-1", "array:string:"])
            has(t, layout, "string \"Open status page\"", "first label")
            has(t, layout, "string \"separator\"", "separator")
            has(t, layout, "string \"Paused\"", "disabled label")
            has(t, layout, "string \"toggle-state\"", "checkmark")
            has(t, layout, "string \"Quit\"", "last label")
            let version = dbus_send(rig, true, "/MenuBar", "org.freedesktop.DBus.Properties.Get", ["string:com.canonical.dbusmenu", "string:Version"])
            has(t, version, "uint32 3", "dbusmenu version")
        }
    }
    t |> run("a clicked menu entry reports its id, a disabled one reports nothing, Quit ends the child") @(t : T?) {
        with_rig(t) $(var rig : TrayRig) {
            dbus_send(rig, false, "/MenuBar", "com.canonical.dbusmenu.Event", ["int32:3", "string:clicked", "variant:int32:0", "uint32:0"])
            dbus_send(rig, false, "/MenuBar", "com.canonical.dbusmenu.Event", ["int32:1", "string:clicked", "variant:int32:0", "uint32:0"])
            t |> equal(wait_for_line(t, rig, "@event menu", 10.0), "@event menu 10 0 0")
            dbus_send(rig, false, "/MenuBar", "com.canonical.dbusmenu.Event", ["int32:4", "string:clicked", "variant:int32:0", "uint32:0"])
            t |> equal(wait_for_line(t, rig, "@event menu 30", 10.0), "@event menu 30 0 0")
            t |> equal(unsafe(process_wait(rig.child, 10.0)), 0, "child returned 0 after Quit")
        }
    }
    t |> run("EventGroup, the batched form panels send, reaches the same menu event") @(t : T?) {
        if (!fexist("/usr/bin/busctl")) {
            t |> skip("no /usr/bin/busctl here, and dbus-send cannot encode a(isvu)")
            return
        }
        with_rig(t) $(var rig : TrayRig) {
            let argv <- ["/usr/bin/busctl", "--address={rig.address}", "call", rig.dest, "/MenuBar", "com.canonical.dbusmenu", "EventGroup", "a(isvu)", "1", "4", "clicked", "i", "0", "0"]
            let no_env : array<string>
            unsafe {
                var p = spawn_process(argv, "", no_env)
                process_wait(p, 10.0)
                close_process(p)
            }
            t |> equal(wait_for_line(t, rig, "@event menu", 10.0), "@event menu 30 0 0")
        }
    }
    t |> run("notify without a notification daemon reports false") @(t : T?) {
        with_rig(t) $(var rig : TrayRig) {
            t |> equal(wait_for_line(t, rig, "@notify", 5.0), "@notify false")
        }
    }
}
