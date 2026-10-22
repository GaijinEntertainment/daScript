// The arm64 CPU classes: the ladder descends through nested feature sets (a higher class carries
// every feature of the classes below it, so adopting a lower profile never assumes a feature the
// box lacks), and an i8mm box that also reports SVE resolves to arm-sve - the Neoverse class -
// instead of the Apple arm-i8mm one. The x86 ladder is not nested by spelling: x86-vnni512 has no
// avxvnni, and the vnni256 gate reads avx512vnni + avx512vl as the same seat.
options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings
require llvm/daslib/llvm_cpu_class
require _spawn_common

[test]
def test_arm_ladder_nests(t : T?) {
    let ladder <- [for (c in split(jit_cpu_class_ladder("arm64"), ";")); c; where !empty(c)]
    t |> equal(join(ladder, ";"), "arm-sve;arm-i8mm;arm-neon", "the arm64 ladder, most capable first")
    for (i in range(length(ladder))) {
        let row = jit_cpu_class_row(ladder[i])
        t |> equal(row.name, ladder[i], "the ladder's {ladder[i]} is a row of the class table")
        t |> equal(row.arch, "arm64", "{ladder[i]} belongs to arm64")
        continue if (i + 1 >= length(ladder))
        for (f in split(jit_cpu_class_row(ladder[i + 1]).features, ";")) {
            t |> success(jit_class_has(ladder[i], f), "{ladder[i]} carries {f}, which the lower {ladder[i + 1]} has")
        }
    }
    t |> success(jit_class_has("arm-sve", "sve") && !jit_class_has("arm-i8mm", "sve"), "sve is what splits arm-sve from arm-i8mm")
}

def private probe_class(t : T?; forced : string) : string {
    let dirRes = create_temp_directory_result("llvm_cpu_class_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return ""
    }
    let work = dirRes as value
    let script = path_join(work, "probe.das")
    fopen(script, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\nrequire llvm/daslib/llvm_tune\n")
            fprint(f, "[export] def main() \{ print(\"CLASS=[\{tune_cpu_class()}]\\n\"); \}\n")
        }
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    var lines : array<string>
    let env = (get_platform_name() == "windows" ?
        "set DAS_JIT_BASELINE=&& set DAS_JIT_ARM64_FORCE_FEATURES={forced}&& " :
        "DAS_JIT_BASELINE= DAS_JIT_ARM64_FORCE_FEATURES={forced} ")
    let rc = spawn_child("{env}\"{bin}\" -no-module-cache \"{script}\"", lines)
    t |> equal(rc, 0, "the probe runs under '{forced}': {join(lines, "\n")}")
    remove(script)
    rmdir(work)
    for (ln in lines) {
        let at = find(ln, "CLASS=[")
        continue if (at < 0)
        let tail = slice(ln, at + 7)
        let close = find(tail, "]")
        return close >= 0 ? slice(tail, 0, close) : tail
    }
    t |> failure("the probe under '{forced}' printed no class: {join(lines, "\n")}")
    return ""
}

[test]
def test_sve_box_is_arm_sve(t : T?) {
    if (get_architecture_name() != "arm64") {
        t |> success(true)
        return
    }
    t |> equal(probe_class(t, "i8mm,sve"), "arm-sve", "an i8mm box reporting SVE is arm-sve")
}

    //! The forced features append to the detected ones, so this leg needs a box reporting no SVE
    //! of its own - the Apple class; a Neoverse box skips it.
[test]
def test_i8mm_box_without_sve_is_arm_i8mm(t : T?) {
    if (get_architecture_name() != "arm64") {
        t |> success(true)
        return
    }
    if (cpu_supports("sve")) {
        t |> skip("this box reports SVE itself; forcing i8mm cannot take it away")
        return
    }
    t |> equal(probe_class(t, "i8mm"), "arm-i8mm", "an i8mm box with no SVE stays arm-i8mm")
}
