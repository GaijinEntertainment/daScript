options gen2

require daslib/fio
require strings

// Prints how many of this process's mappings carry the kernel's
// huge-page advice (the `hg` flag of /proc/self/smaps) once it holds
// one 8 MB array.
[export]
def main() {
    var held : array<uint8>
    held |> resize(8 * 1024 * 1024)
    var advised = 0
    fopen("/proc/self/smaps", "rb") $(f) {
        return if (f == null)
        while (!feof(f)) {
            let line = fgets(f)
            if (line |> starts_with("VmFlags:") && find("{strip(line)} ", " hg ") >= 0) {
                advised++
            }
        }
    }
    print("ADVISED={advised} of {length(held)} bytes held\n")
}
