options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require strings

//! What a test needs to reach a tray icon from outside its process, shared by the stddlg tray
//! test and the watchdog's tray arms: a private session bus and `dbus-send` on Linux, System
//! Events through `osascript` on macOS.
def dbus_tools_present() : bool {
    return fexist("/usr/bin/dbus-daemon") && fexist("/usr/bin/dbus-send")
}

//! A `dbus-daemon` on a private bus and its address; the address is empty when the daemon printed
//! none within 15 s. The caller owns the daemon and closes it. The bus gets its own config with
//! no service directories: the stock session config would activate the desktop's notification
//! daemon on demand, and an arm that asserts "no daemon here" would then find one.
def spawn_session_bus() : tuple<bus : process; address : string> {
    let config_path = "/tmp/das-tray-bus-{ref_time_ticks()}.conf"
    fwrite(config_path, "<!DOCTYPE busconfig PUBLIC \"-//freedesktop//DTD D-Bus Bus Configuration 1.0//EN\" \"http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd\">\n"
        + "<busconfig><type>session</type><listen>unix:tmpdir=/tmp</listen>"
        + "<policy context=\"default\"><allow send_destination=\"*\" eavesdrop=\"true\"/><allow eavesdrop=\"true\"/><allow own=\"*\"/></policy>"
        + "</busconfig>\n")
    let argv <- ["/usr/bin/dbus-daemon", "--config-file={config_path}", "--print-address", "--nofork", "--nopidfile"]
    let no_env : array<string>
    var bus = unsafe(spawn_process(argv, "", no_env))
    var address = ""
    let started = ref_time_ticks()
    while (empty(address) && get_time_usec(started) < 15000000) {
        unsafe(process_drain(bus) $(line) {
            if (empty(address) && starts_with(line, "unix:")) {
                address = clone_string(line)
            }
        })
        sleep(5u)
    }
    remove(config_path)
    return (bus = bus, address = address)
}

//! `dbus-send` on `address` against `dest`; `reply` asks for --print-reply and returns the dump,
//! otherwise the call is fire-and-forget and the result is empty.
def dbus_send_to(address, dest : string; reply : bool; path, method : string; args : array<string>) : string {
    var argv <- ["/usr/bin/dbus-send", "--bus={address}"]
    if (reply) {
        argv |> push("--print-reply")
    } else {
        argv |> push("--type=method_call")
    }
    argv |> push("--dest={dest}")
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

//! osascript; the dump is the script's stdout and stderr, so a denied accessibility permission
//! reads back as text, not a crash.
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

def dump_has(t : T?; haystack : string; needle : string; what : string) {
    t |> success(find(haystack, needle) >= 0, "{what}: expected '{needle}' in\n{haystack}")
}
