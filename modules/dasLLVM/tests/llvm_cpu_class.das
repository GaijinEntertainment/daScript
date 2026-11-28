// The arm64 CPU classes: the ladder descends through nested feature
// sets (a higher class carries every feature of the classes below
// it, so adopting a lower profile never assumes a feature the box
// lacks), and an i8mm box that also reports SVE resolves to arm-sve -
// the Neoverse class - instead of the Apple arm-i8mm one. The x86
// ladder is not nested by spelling: x86-vnni512 has no avxvnni, and
// the vnni256 gate reads avx512vnni + avx512vl as the same seat.
options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require daslib/strings_boost
require strings
require llvm/daslib/llvm_cpu_class
require _spawn_common

[test(description = "the arm64 class ladder runs arm-sve, arm-i8mm, arm-neon, each class carrying every feature of the class below it, with sve splitting the top two")]
def test_arm_ladder_nests(t : T?) {
    let ladder <- [for (c in jit_cpu_class_ladder(CpuClass.arm_generic)); cpu_class_name(c)]
    t |> equal(join(ladder, ";"), "arm-sve;arm-i8mm;arm-neon",
        "the arm64 ladder, most capable first")
    for (i in range(length(ladder))) {
        let row = jit_cpu_class_row(cpu_class_of(ladder[i]))
        t |> equal(cpu_class_name(row.klass), ladder[i],
            "the ladder's {ladder[i]} is a row of the class table")
        t |> success(cpu_class_arch(row.klass) == CpuClass.arm_generic,
            "{ladder[i]} belongs to arm64")
        continue if (i + 1 >= length(ladder))
        let lower = jit_cpu_class_row(cpu_class_of(ladder[i + 1])).features
        t |> success((row.features & lower) == lower,
            "{ladder[i]} carries every feature the lower {ladder[i + 1]} has")
    }
    t |> success((jit_cpu_class_row(CpuClass.arm_sve).features.sve &&
            !jit_cpu_class_row(CpuClass.arm_i8mm).features.sve),
        "sve is what splits arm-sve from arm-i8mm")
}

//! what a child prints for `expr` (a string expression over
//! llvm_tune) under the `baseline` pin and the `forced` features,
//! `prelude`'s statements run first
def private probe_target(t : T?; baseline, forced, expr : string; prelude : string = "") : string {
    let dirRes = create_temp_directory_result("llvm_cpu_class_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return ""
    }
    let work = dirRes as value
    let script = path_join(work, "probe.das")
    fopen(script, "w") $(f) {
        if (f != null) {
            fprint(f, "options gen2\nrequire llvm/daslib/llvm_tune\nrequire strings\n")
            fprint(f, ("[export] def main() \{ {prelude}" +
                "print(\"CLASS=[\{{expr}}]\\n\"); \}\n"))
        }
    }
    let args <- get_command_line_arguments()  // locked view — do not delete
    let bin = args[0]
    var lines : array<string>
    let env = child_env([("DAS_JIT_BASELINE", baseline), ("DAS_JIT_ARM64_FORCE_FEATURES", forced)])
    let rc = spawn_child("{env}\"{bin}\" -no-module-cache \"{script}\"", lines)
    t |> equal(rc, 0, "the probe runs under '{baseline}' '{forced}': {join(lines, "\n")}")
    remove(script)
    rmdir(work)
    for (ln in lines) {
        let at = find(ln, "CLASS=[")
        continue if (at < 0)
        let tail = slice(ln, at + 7)
        let close = find(tail, "]")
        return close >= 0 ? slice(tail, 0, close) : tail
    }
    t |> failure("the probe under '{baseline}' '{forced}' printed nothing: {join(lines, "\n")}")
    return ""
}

def private probe_class(t : T?; forced : string) : string =>
    probe_target(t, "", forced, "cpu_class_name(tune_process_target().klass)")

[test(description = "a process pinned to a class mints for that class - its name and its feature set - and for the box where no class is named", timeout = 1200)]
def test_mint_target_follows_the_pin(t : T?) {
    let x86 = get_architecture_name() == "x86_64"
    if (!x86 && get_architecture_name() != "arm64") {
        t |> success(true)
        return
    }
    let pin = x86 ? "x86-avx2" : "arm-neon"
    let mint = ("cpu_class_name(tune_mint_target().klass) + \"|\" + " +
        "cpu_features_text(tune_mint_target().features, \";\")")
    t |> equal(probe_target(t, pin, "", mint),
        "{pin}|{cpu_features_text(jit_cpu_class_row(cpu_class_of(pin)).features, ";")}",
        "under the pin a mint stamps the class and the class's features")
    let box = ("cpu_class_name(tune_native_target().klass) + \"|\" + " +
        "cpu_features_text(tune_native_target().features, \";\")")
    let native = probe_target(t, "", "", box)
    t |> equal(probe_target(t, "", "", mint), native, "with no pin a mint stamps this box")
    let feats = cpu_features_text(jit_cpu_class_row(cpu_class_of(pin)).features, ";")
    let save = ("set_tune_manifest_in_memory(); tune_manifest_set(\"probe\", \"reference\"); " +
        "let stamped = find(tune_manifest_text(), \"\\\"{feats}\\\"\") >= 0; ")
    t |> equal(probe_target(t, pin, "", "stamped", save), "true",
        "under the pin a saved sidecar's feature stamp is the class's")
    //! a box whose own features are the class's stamps the same text
    if (!ends_with(native, "|{feats}")) {
        t |> equal(probe_target(t, "", "", "stamped", save), "false",
            "control: with no pin the stamp is the box's, not the class's")
    }
}

[test(description = "an arm64 box reporting i8mm and SVE resolves to the arm-sve class", timeout = 1200)]
def test_sve_box_is_arm_sve(t : T?) {
    if (get_architecture_name() != "arm64") {
        t |> success(true)
        return
    }
    t |> equal(probe_class(t, "i8mm,sve"), "arm-sve", "an i8mm box reporting SVE is arm-sve")
}

//! The forced features append to the detected ones, so this leg
//! needs a box reporting no SVE of its own - the Apple class; a
//! Neoverse box skips it.
[test(description = "an arm64 box reporting i8mm without SVE resolves to the arm-i8mm class", timeout = 1200)]
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
