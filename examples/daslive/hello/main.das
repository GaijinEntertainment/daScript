options gen2

require live_host

var frame_count : int = 0

[export]
def init() {
    print("hello: init() called (is_reload={is_reload()})\n")
    if (!is_reload()) {
        frame_count = 0
    }
}

[export]
def update() {
    frame_count++
    if (frame_count % 60 == 0) {
        print("hello: frame {frame_count}, dt={get_dt()}, uptime={get_uptime()}, fps={get_fps()}\n")
    }
    if (frame_count >= 300) {
        print("hello: 300 frames done, requesting exit\n")
        request_exit()
    }
}

[export]
def shutdown() {
    print("hello: shutdown() called (total frames={frame_count})\n")
}

// Dual-mode: also works with regular daslang.exe
[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
