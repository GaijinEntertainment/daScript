options gen2

require daslib/fio
require strings

let private MAX_CAPTURE_BYTES = 8 * 1024 * 1024

def private option_value(prefix : string) : string {
    for (argument in get_command_line_arguments()) {
        if (starts_with(argument, prefix)) return slice(argument, length(prefix))
    }
    return ""
}

// Streams a git command's stdout to a file byte-exactly. The hosted task
// terminal carries only these diagnostics, so ConPTY column wrapping can
// never reach the captured payload (long diff lines were wrapped at the
// 80-column task grid and parsed as phantom context lines).
[export]
def main() : int {
    let output_path = option_value("--output=")
    let expected_exit = to_int(option_value("--expect-exit="))
    var command_argv : array<string>
    var collecting = false
    for (argument in get_command_line_arguments()) {
        if (collecting) {
            command_argv |> push(argument)
        } elif (argument == "--argv") {
            collecting = true
        }
    }
    if (empty(output_path) || empty(command_argv)) {
        print("output and an --argv command are required\n")
        return 2
    }
    var exit_code = -1
    var byte_count = 0
    var output_opened = false
    var write_failed = false
    var capture_too_large = false
    fopen(output_path, "wb") $(output) {
        if (output != null) {
            output_opened = true
            exit_code = unsafe(popen_argv_pipe(command_argv, $(_stdin_w, input) {
                if (input == null) return
                var chunk : array<uint8>
                chunk |> resize(65536)
                while (true) {
                    let read_count = fread(input, chunk)
                    if (read_count <= 0) break
                    if (capture_too_large || write_failed) continue
                    if (byte_count + read_count > MAX_CAPTURE_BYTES) {
                        capture_too_large = true
                        continue
                    }
                    if (read_count < length(chunk)) chunk |> resize(read_count)
                    let written = fwrite(output, chunk)
                    if (written != read_count) {
                        write_failed = true
                        if (read_count < 65536) chunk |> resize(65536)
                        continue
                    }
                    byte_count += written
                    if (read_count < 65536) chunk |> resize(65536)
                }
                delete chunk
            }))
        }
    }
    if (!output_opened) {
        print("could not open the capture output file\n")
        return 3
    }
    if (capture_too_large) {
        let _removed = remove(output_path)
        print("captured output exceeds the 8 MiB limit\n")
        return 4
    }
    if (write_failed) {
        let _removed = remove(output_path)
        print("could not write the captured output\n")
        return 3
    }
    if (exit_code != expected_exit) {
        let _removed = remove(output_path)
        print("git exited with {exit_code} (expected {expected_exit})\n")
        return 5
    }
    print("captured {byte_count} bytes\n")
    return 0
}
