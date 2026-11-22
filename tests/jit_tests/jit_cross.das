options gen2
//! no_aot: every test here drives the JIT or a JIT-built artifact
options no_aot

require daslib/ast
require daslib/fio
require strings
require dastest/testing_boost
require daslib/strings_boost
require daslib/clargs
require math
require _jit_children

// A return, break or continue that leaves through a `finally`
// dispatches back to its continuation through a switch on an index;
// the wasm64 cross compile of that shape emits its object (the
// indirectbr form left IR the wasm backend crashed on), and a native
// run reaches every continuation.


let WASM_FINALLY_FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_finally_fixture.das"


[test(description = "a native run takes every exit through a finally once, and the wasm64 cross compile of that shape emits its object", timeout = 1200)]
def test_wasm_finally_return(tt : T?) {
    if (!jit_enabled()) {
        tt |> success(true)
        return
    }
    let bin = get_host_binary()
    tt |> run("a native run takes every exit through its finally once") @(t : T?) {
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -jit \"{WASM_FINALLY_FIXTURE}\"", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        // pick: 2 finally runs; loop_exits(10): i = 0..4 enter the block, 4
        // breaks out -> 5 runs
        t |> success(find(output, "values 7 9 3 5 finally 7") >= 0, output)
    }
    tt |> run("the wasm64 cross compile emits its object") @(t : T?) {
        let obj = path_join(test_temp_dir(), "wasm_finally.o")
        var lines : array<string>
        let rc = spawn_child("\"{bin}\" -exe -output \"{obj}\" \"{WASM_FINALLY_FIXTURE}\" " +
            "-- --jit-target=wasm64-unknown-emscripten --jit-emit-object", lines)
        let output = join(lines, "\n")
        t |> equal(rc, 0, output)
        let st = stat(obj)
        t |> success(st.is_reg && st.size > 0ul, "{obj} written:\n{output}")
    }
}

// The signed idot family on the wasm64 cross target lowers to the
// ISA's exact int8 dot halves - the i16x8 extmul products summed by
// i32x4.extadd_pairwise_i16x8_s and folded even/odd - and never to
// the generic widen-multiply chain that runs a fifth as fast there;
// the relaxed 7-bit dot is refused by design (it wraps at -128, and
// the feature string carries no +relaxed-simd - WebKit runs none of
// it), so the dump carries no relaxed intrinsic either. A host
// compile of the same fixture emits no wasm intrinsic. Compile-only
// keeps both compiles on this host and never touches the artifact
// cache, so --jit-dump prints every time.


let WASM_IDOT_FIXTURE = "{get_das_root()}/tests/jit_tests/_wasm_idot_fixture.das"
let PAIRWISE_ADD = "llvm.wasm.extadd.pairwise.signed"
let RELAXED_DOT = "llvm.wasm.relaxed.dot"
let QUAD_LANE_FOLD = "<4 x i32> <i32 0, i32 2, i32 4, i32 6>"

[test(description = "the wasm64 cross compile lowers idot4 to the pairwise int8 dot with simd128 and no relaxed-simd, the scalar idot to the same dot with no quad-lane fold, and a host compile emits no wasm intrinsic", timeout = 1200)]
def test_wasm_idot_lowering(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let bin = get_host_binary()
    var terr : string
    mkdir_rec(OUTPUT_DIR)
    let dump = path_join(OUTPUT_DIR, "wasm_idot_{ref_time_ticks()}")
    t |> run("the wasm64 cross compile lowers idot4 to the exact pairwise int8 dot") @(t : T?) {
        var out : array<string>
        spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{WASM_IDOT_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten "
            + "--jit-compile-only --jit-dump=\"{dump}.wasm\"", out)
        let ln <- split(fread("{dump}.wasm"), "\n")
        let pairs = count_with(ln, PAIRWISE_ADD)
        t |> success(pairs >= 3, "the declaration and the two half calls of {PAIRWISE_ADD} are in the dump ({pairs} lines, {length(ln)} lines total)")
        t |> success(count_with(ln, RELAXED_DOT) == 0, "the relaxed 7-bit dot is never emitted")
        t |> success(count_with(ln, "idot_prod") == 0, "no generic widen-multiply chain remains")
        // the unsigned-by-signed form takes the same path, widening its first
        // operand with ZExt: drop that arm and these two halves become the
        // 16-lane chain the line above bans
        let zexts = count_with(ln, "zext <8 x i8>")
        t |> success(zexts >= 2,
                     "both halves of the unsigned operand widen with ZExt ({zexts} lines)")
        let folds = count_with(ln, QUAD_LANE_FOLD)
        t |> success(folds == 2,
            "each idot4 call folds its pair sums into quad lanes ({folds} fold shuffles)")
        t |> success(count_with(ln, " = add <4 x i32> %idot_pairs_lo") == 1,
            "the scalar idot adds its two pair-sum vectors unfolded")
    }
    t |> run("a host compile emits no wasm intrinsic") @(t : T?) {
        var out : array<string>
        spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{WASM_IDOT_FIXTURE}\" -- --jit-compile-only "
            + "--jit-dump=\"{dump}.host\"", out)
        let ln <- split(fread("{dump}.host"), "\n")
        t |> success(length(ln) > 20, "the host dump printed ({length(ln)} lines)")
        t |> success(count_with(ln, "llvm.wasm.") == 0, "no wasm intrinsic on the host")
    }
    t |> run("the wasm object's target features carry simd128 and no relaxed-simd") @(t : T?) {
        let dir_r = create_temp_directory_result("wasm_idot_features")
        if (!(dir_r is value)) {
            t |> failure("temp dir failed")
            return
        }
        let dir = unsafe(dir_r.value)
        let obj = "{dir}/idot.o"
        var ln : array<string>
        spawn_child("\"{bin}\" -no-module-cache -exe -output \"{obj}\" " +
            "\"{WASM_IDOT_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten --jit-emit-object", ln)
        let features = object_target_features(obj)
        t |> success(find(features, "simd128") >= 0,
                     "the object's target_features section names simd128: {features}")
        t |> success(find(features, "relaxed-simd") < 0,
                     "no relaxed opcode can be in a module WebKit must run: {features}")
        rmdir_rec_result(dir)
    }
    remove("{dump}.wasm", terr)
    remove("{dump}.host", terr)
}

// the object's `target_features` custom section as one text line -
// each feature name is ASCII inside a binary file, so the bytes are
// read into an array and every non-printable byte becomes a space
// before the search
def private object_target_features(path : string) : string {
    var bytes : array<uint8>
    fopen(path, "rb") $(f) {
        if (f == null) {
            return
        }
        bytes |> resize(int64(stat(path).size))
        fread(f, bytes)
    }
    let text = build_string() $(w) {
        for (b in bytes) {
            let c = int(b)
            w |> write(c >= 0x20 && c < 0x7f ? to_char(c) : " ")
        }
    }
    let at = find(text, "target_features")
    return at < 0 ? "" : slice(text, at, min(at + 256, length(text)))
}

let WASM_INLINED_MATH_CALLS = fixed_array("@llvm.sin.v4f32", "@llvm.cos.v4f32",
    "@llvm.exp.v4f32", "@llvm.exp2.v4f32", "@llvm.tanh.v4f32")

[test(description = "a wasm64 compile rounds through trunc, widens halves with no half type, both vectorized, takes float4 sin, cos, exp, exp2 and tanh as inline polynomials, and carries the user's hints", timeout = 1200)]
def test_wasm_lowerings(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let bin = get_host_binary()
    var terr : string
    mkdir_rec(OUTPUT_DIR)
    let dump = path_join(OUTPUT_DIR, "wasm_lowering_{ref_time_ticks()}")
    t |> run("the wasm64 cross compile") @(t : T?) {
        var out : array<string>
        spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{WASM_LOWERING_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten "
            + "--jit-compile-only --jit-dump=\"{dump}.wasm\"", out)
        let ln <- split(fread("{dump}.wasm"), "\n")
        t |> success(count_with(ln, "@llvm.round.") == 0,
            "no llvm.round, which wasm turns into a roundf call ({length(ln)} lines)")
        let truncs = count_with(ln, "@llvm.trunc.v4f32")
        t |> success(truncs >= 2,
            "the rounding loop vectorizes over the truncation ({truncs} lines)")
        t |> success(count_with(ln, "x half>") == 0 && count_with(ln, "fpext") == 0,
            "no half type reaches the IR")
        let scales = count_with(ln, "fmul ", "splat (float {TWO_POW_112})")
        t |> success(scales >= 1,
            "the widening loop vectorizes over the 2^112 scale ({scales} lines)")
        for (call in WASM_INLINED_MATH_CALLS) {
            t |> success(count_with(ln, call) == 0,
                "no {call}, which wasm turns into four scalar calls")
        }
        t |> success(count_with(ln, " noinline ") >= 1, "a function hint reaches its attributes")
        t |> success(count_with(ln, "llvm.loop.vectorize.enable", "i1 false") >= 1,
            "a loop hint reaches the loop's metadata")
    }
    t |> run("a host compile keeps llvm.round and no 2^112 scale") @(t : T?) {
        var out : array<string>
        spawn_in_temp("\"{bin}\" {NO_DLL_CACHE_JIT} \"{WASM_LOWERING_FIXTURE}\" --"
            + " --jit-compile-only "
            + "--jit-dump=\"{dump}.host\"", out)
        let ln <- split(fread("{dump}.host"), "\n")
        t |> success(count_with(ln, "@llvm.round.") >= 1, "the host rounds through llvm.round")
        t |> success(count_with(ln, "@llvm.sin.v4f32") == 0, "the host takes sin inline too")
        t |> success(count_with(ln, "@llvm.exp.v4f32") == 0, "the host takes exp inline too")
    }
    remove("{dump}.wasm", terr)
    remove("{dump}.host", terr)
}

// The target-side folds: get_target_triple /
// get_target_architecture_name answer for the artifact a compile
// EMITS (the --jit-target triple), the host's values with no triple,
// and a static_if over the triple selects the cross arm. Compile-only
// keeps the cross run on this host: the
// folds are decided from argv before the first module parses, and
// main runs interpreted. The children run with the default module
// cache on purpose: a --jit-target after the separator keys the
// cache, or a cached native compile serves the cross one with the
// host's folds.


let CROSS_TARGET_FIXTURE = "{get_das_root()}/tests/jit_tests/_cross_target_fixture.das"

[test(description = "the target triple and architecture name fold to the cross target's values for a --jit-target compile and to the host's otherwise", timeout = 1200)]
def test_cross_target_folds(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let bin = get_host_binary()
    t |> run("a native compile answers the host and keeps the framework live") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\"", ln)
        t |> success(ln |> has("TRIPLE=[]"), "no triple on a native compile: {join(ln, " | ")}")
        t |> success(ln |> has("TARGET_ARCH=[{get_architecture_name()}]"),
                     "the target arch is the host's")
        t |> success(ln |> has("CROSS_PLATFORM=[]"), "no cross platform")
        t |> success(ln |> has("CROSS=[no]"), "a native compile takes the native arm")
        t |> success(ln |> has("LET_WASM=[false]"),
                     "a module-level let over the target folds false natively")
    }
    t |> run("a wasm64 cross target answers wasm64 on any host") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=wasm64-unknown-emscripten --jit-compile-only", ln)
        t |> success(ln |> has("TRIPLE=[wasm64-unknown-emscripten]"),
                     "the triple folds: {join(ln, " | ")}")
        t |> success(ln |> has("TARGET_ARCH=[wasm64]"), "the target arch is the triple's")
        t |> success(ln |> has("HOST_ARCH=[{get_architecture_name()}]"),
                     "the host arch stays the host's")
        t |> success(ln |> has("CROSS_PLATFORM=[emscripten]"), "the cross platform is emscripten")
        t |> success(ln |> has("CROSS=[yes]"), "a cross compile takes the cross arm")
        t |> success(ln |> has("LET_WASM=[true]"),
                     "a module-level let over the target folds true for wasm64")
    }
    t |> run("the last --jit-target on the command line wins, as every other argv reader takes it") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=x86_64-unknown-linux-gnu --jit-target=wasm64-unknown-emscripten " +
                "--jit-compile-only", ln)
        t |> success(ln |> has("TRIPLE=[wasm64-unknown-emscripten]"),
                     "the last triple folds: {join(ln, " | ")}")
    }
    t |> run("an x86_64 linux cross target answers x86_64") @(t : T?) {
        var ln : array<string>
        spawn_child("\"{bin}\" -jit \"{CROSS_TARGET_FIXTURE}\" -- " +
            "--jit-target=x86_64-unknown-linux-gnu --jit-compile-only", ln)
        t |> success(ln |> has("TARGET_ARCH=[x86_64]"),
                     "the target arch is the triple's: {join(ln, " | ")}")
        t |> success(ln |> has("CROSS_PLATFORM=[]"), "linux is not a cross platform name")
        t |> success(ln |> has("CROSS=[yes]"), "any cross target takes the cross arm")
    }
}
