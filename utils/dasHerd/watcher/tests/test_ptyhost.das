options gen2
options rtti

require dastest/testing_boost public
require dashv/dashv_boost
require daslib/fio
require daslib/base64
require daslib/json_boost
require strings
require math

let TEST_PORT = 9873

var private g_hello_seen = false
var private g_attached_seen = false
var private g_output_text = ""
var private g_last_stat = ""

class private ProbeClient : HvWebSocketClient {
    token : string
    def override onOpen {
        send("\{\"op\":\"hello\",\"token\":{sprint_json(token, false)},\"protocol\":1\}")
    }
    def override onClose {}
    def override onMessage(msg : string#) {
        if (find(msg, "\"type\":\"stat\"") >= 0) {
            g_last_stat = string(msg)
            if (!g_hello_seen) {
                g_hello_seen = true
                send("\{\"op\":\"attach\",\"from_byte\":0\}")
            }
        } elif (find(msg, "\"type\":\"attached\"") >= 0) {
            g_attached_seen = true
        }
    }
    def override onMessageFrame(msg : string#; byte_count : int; opcode : ws_opcode) {
        if (opcode == ws_opcode.WS_OPCODE_BINARY) {
            g_output_text += string(msg)
        } else {
            onMessage(msg)
        }
    }
    def send_input(text : string) {
        var inscope bytes : array<uint8>
        bytes |> reserve(length(text))
        peek_data(text) $(view) {
            for (byte in view) {
                bytes |> push(byte)
            }
        }
        let payload = base64_encode(bytes)
        send("\{\"op\":\"input\",\"base64\":\"{payload}\"\}")
    }
}

def private pump_until(var client : ProbeClient?; timeout_ms : int64;
                       blk : block<() : bool>) : bool {
    let started = ref_time_ticks()
    while (get_time_nsec(started) / 1000000l < timeout_ms) {
        client->process_event_que()
        if (invoke(blk)) return true
        sleep(5u)
    }
    return false
}

[test]
def test_ptyhost_lifecycle(t : T?) {
    if (get_platform_name() != "windows") return
    t |> run("host survives its spawner, replays, accepts input, terminates") <| @(t : T?) {
        let root = get_das_root()
        var compiler = path_join(root, "bin/Release/daslang.exe")
        if (!fexist(compiler)) {
            let args <- get_command_line_arguments()
            compiler = empty(args) ? "daslang.exe" : args[0]
        }
        let temp = create_temp_directory_result("dasherd_ptyhost_")
        if (!(temp is value)) {
            t |> failure("could not create temporary test directory")
            return
        }
        let journal_dir = temp as value

        // Stage 1: an intermediary launcher spawns the host DETACHED and
        // exits - the popen child's death is the spawner's death.
        var launcher_output = ""
        let launcher_argv <- [compiler,
            path_join(root, "utils/dasHerd/ptyhost/tests/host_launcher.das"), "--",
            "--token", "t1token",
            "--child-env", "PTYHOST_TEST=from-test-env",
            "--host-args", compiler,
            path_join(root, "utils/dasHerd/ptyhost/main.das"), "--",
            "--session", "t1", "--port", "{TEST_PORT}",
            "--journal", journal_dir, "--cwd", journal_dir,
            "--child", "cmd.exe", "--child", "/q"]
        let launcher_exit = unsafe(popen_argv(launcher_argv, 60.0, $(file) {
            if (file != null) {
                launcher_output = unsafe(fread_to_eof(file))
            }
        }))
        t |> equal(launcher_exit, 0, launcher_output)
        t |> success(find(launcher_output, "launched pid=") >= 0
                && find(launcher_output, "launched pid=0 ") < 0,
            "launcher reported a live host pid: {launcher_output}")

        // Stage 2: the stamp appears while the spawner is already gone.
        let stamp_path = path_join(journal_dir, "t1.host.json")
        var stamp_seen = false
        let stamp_started = ref_time_ticks()
        while (get_time_nsec(stamp_started) / 1000000l < 30000l) {
            if (fexist(stamp_path)) {
                stamp_seen = true
                break
            }
            sleep(20u)
        }
        t |> success(stamp_seen, "host stamp appeared after spawner death")

        // Stage 3: connect, authenticate, attach, drive the child.
        g_hello_seen = false
        g_attached_seen = false
        g_output_text = ""
        g_last_stat = ""
        var client = new ProbeClient(token = "t1token")
        t |> equal(client->init("ws://127.0.0.1:{TEST_PORT}/"), 0)
        t |> success(pump_until(client, 15000l) $() => g_attached_seen,
            "hello + attach handshake completed")
        client->send_input("echo PTYHOST-OK:%PTYHOST_TEST%\r\n")
        t |> success(pump_until(client, 15000l)
                $() => find(g_output_text, "PTYHOST-OK:from-test-env") >= 0,
            "child echoed input with the forwarded env var: {slice(g_output_text, max(0, length(g_output_text) - 400))}")

        // Stage 4: terminate the child; the host stamps the exit.
        client->send("\{\"op\":\"terminate\",\"exit_code\":0\}")
        t |> success(pump_until(client, 15000l) $() {
                client->send("\{\"op\":\"stat\"\}")
                return find(g_last_stat, "\"exited\":true") >= 0
            }, "host observed child exit: {g_last_stat}")

        // Stage 5: shutdown; the host writes the final stamp and exits.
        client->send("\{\"op\":\"shutdown\"\}")
        let shutdown_started = ref_time_ticks()
        var final_stamp = ""
        while (get_time_nsec(shutdown_started) / 1000000l < 15000l) {
            client->process_event_que()
            final_stamp = fread(stamp_path)
            if (find(final_stamp, "\"exited\":true") >= 0
                    && find(final_stamp, "\"drained\":true") >= 0) break
            sleep(20u)
        }
        t |> success(find(final_stamp, "\"exited\":true") >= 0
                && find(final_stamp, "\"drained\":true") >= 0,
            "final stamp records exited + drained: {final_stamp}")
        client->close()
        client->cleanup()
        unsafe {
            delete client
        }
    }
}
