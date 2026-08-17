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

[export]
def main() : int {
    let worktree = option_value("--worktree=")
    let revision = option_value("--revision=")
    let file_path = option_value("--file=")
    let output_path = option_value("--output=")
    if (empty(worktree) || empty(revision) || empty(file_path)
            || empty(output_path)) {
        print("worktree, revision, file, and output are required\n")
        return 2
    }
    let object_name = "{revision}:{file_path}"
    var exit_code = -1
    var byte_count = 0
    var output_opened = false
    var write_failed = false
    var capture_too_large = false
    fopen(output_path, "wb") $(output) {
        if (output != null) {
            output_opened = true
            exit_code = unsafe(popen_argv_pipe([
                "git", "--literal-pathspecs", "-C", worktree,
                "--no-pager", "show", "--format=", object_name], $(_stdin_w, input) {
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
        print("could not open captured Git blob\n")
        return 3
    }
    if (capture_too_large) {
        let _removed = remove(output_path)
        print("Git blob exceeds the 8 MiB preview limit\n")
        return 4
    }
    if (write_failed) {
        let _removed = remove(output_path)
        print("could not write captured Git blob\n")
        return 3
    }
    if (exit_code != 0) {
        let _removed = remove(output_path)
        print("git blob capture failed ({exit_code})\n")
        return exit_code
    }
    print("captured {byte_count} bytes\n")
    return 0
}
