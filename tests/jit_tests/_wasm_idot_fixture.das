// The fixture wasm_idot_lowering.das compiles for the wasm64 cross target and for the host, dumping
// the IR: one signed idot4 over byte16 operands the emitter cannot fold, since the bytes arrive
// through the command line's length.
options gen2

def taint(n : int) : byte16 {
    var b : byte16
    unsafe {
        var p = addr<int8?>(b)
        for (i in range(16)) {
            p[i] = int8(i * 3 - n)
        }
    }
    return b
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let w = taint(length(args))
    let x = taint(length(args) + 5)
    let d = idot4(w, x)
    print("IDOT4=[{d.x} {d.y} {d.z} {d.w}]\n")
}
