options gen2

require llvm_tune_fat_lib

// App-face fixture for llvm_tune_fat.das (no [test] on purpose — the suite just compiles it):
// built as a fat exe, it prints the k-fingerprint of the clone the box dispatched to (1+3+k),
// the companion's k, and one STATUS row per class clone with whether this box runs it. The
// fallback policy keeps the suite's own compile of this file from trying to tune. The first
// operand is the argv length - a value LLVM cannot fold, or IPSCCP evaluates the pure clones
// at their constant call site and the exe carries no clone at all.
[export, tune_policy(missing = "fallback")]
def main : int {
    let n = length(get_command_line_arguments())   // aliases a process-global Array header - do not delete
    var acc = 0
    fat_void(acc, n)
    print("RESULT {fat_add(n, 3)} KV {fat_kv()} VOID {acc}\n")
    var st <- tune_status()
    for (s in st) {
        let active = (tune_fat_mask() & tune_fat_class_bit(s.klass)) != 0
        print("STATUS {s.fname}|{s.klass}|{s.suffix}|{s.source}|{active ? "active" : "idle"}\n")
    }
    delete st
    log_tune_status("llvm_tune_fat")
    return 0
}
