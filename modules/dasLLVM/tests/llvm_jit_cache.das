options gen2
options no_aot

require dastest/testing_boost public
require daslib/module_path
require daslib/fio
require daslib/ast_boost
require daslib/rtti
require daslib/strings_boost
require llvm/daslib/llvm_jit_plan
require llvm/daslib/llvm_cpu_class
require llvm/daslib/llvm_jit_sweep
require _llvm_cache_common

[test(description = "the JIT's compile-only escape emits no partition and reads no cached object, and the program still runs interpreted to the right answer")]
def test_jit_compile_only(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    var got = -1
    compile_for_jit("{get_this_module_dir()}/_llvm_compile_only_client.das", false) $(var program) {
        simulate(program) $(sok, context, _serrors) {
            if (!sok) {
                return
            }
            var cfg : JitCliOptions
            cfg.compile_only = some(true)
            let r = run_jit(get_ptr(program), get_ptr(context), cfg)
            t |> success(r.ok, "the jit backend ran to its escape")
            t |> success(r.compile_only, "the compile-only path was taken")
            t |> equal(r.partitions, 0, "compile-only emits no partition")
            t |> equal(r.cached, 0, "compile-only reads no cached object")
            unsafe {
                invoke_in_context(context, "answer", addr(got))
            }
        }
    }
    t |> equal(got, 402, "interpreted execution still produces the right answer")
}

def private check_round(t : T?; round, client : string; cached, acc : int) : SplitRound {
    let r = split_round(client)
    t |> success(r.ok, "{round}: the backend ran")
    t |> equal(r.partitions, 3, "{round}: one partition per das module")
    t |> equal(r.cached, cached, "{round}: cached objects")
    t |> equal(r.acc, acc, "{round}: the cross-partition answer")
    return r
}

[test(description = "a split JIT build after an edit to one module re-emits every partition and links the two unchanged ones from the object cache", timeout = 1200)]
def test_jit_obj_cache(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        t |> skip("needs -jit and a DLL daslang build")
        return
    }
    let client = stage_split_client(t, "das_objcache")
    if (empty(client)) return
    let r1 = check_round(t, "round 1 cold", client, 0, 340)
    t |> success(copy_fixture(get_this_module_dir(), dir_name(client), "_llvm_obj_cache_client.das", "var a = 0", "var a = 1000"),
        "round 2 edit client")
    let r2 = check_round(t, "round 2 tail edit", client, 2, 1340)
    var err : string
    rmdir_rec(dir_name(r1.output_path), err)
    rmdir_rec(dir_name(r2.output_path), err)
    rmdir_rec(dir_name(client), err)
}

def private check_answer(t : T?; round : string; r : SplitRound) {
    t |> success(r.ok, "{round}: the backend ran")
    t |> equal(r.acc, 340, "{round}: cross-partition direct calls")
    t |> equal(r.lo, 19, "{round}: the cmres cross-call's low half")
    t |> equal(r.hi, 36, "{round}: the cmres cross-call's high half")
    t |> equal(r.hit, 33, "{round}: the shared table-method glob")
    t |> equal(r.miss, -1, "{round}: the table-method glob's miss answer")
}

[test(description = "a split JIT build emits one partition per das module whose cross-partition calls answer right, and a second run hits the same content-addressed DLL without re-emitting", timeout = 1200)]
def test_jit_split_modules(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        t |> skip("needs -jit and a DLL daslang build")
        return
    }
    let client = stage_split_client(t, "das_split")
    if (empty(client)) return
    let r1 = split_round(client)
    check_answer(t, "round 1 split", r1)
    t |> equal(r1.partitions, 3, "round 1 split: one partition per das module")
    t |> equal(r1.cached, 0, "round 1 split: nothing was cached to link")
    let r2 = split_round(client)
    check_answer(t, "round 2 dll cache hit", r2)
    t |> equal(r2.partitions, 0, "round 2 dll cache hit: no partition re-emitted")
    t |> strictEqual(r2.output_path, r1.output_path, "round 2 dll cache hit: the same content-addressed DLL")
    var err : string
    rmdir_rec(dir_name(r1.output_path), err)
    rmdir_rec(dir_name(client), err)
}

def private parse_round(client, blob : string; var res : SplitRound) : bool {
    var wrote = false
    compile_for_jit(client, false) $(var program) {
        var ser = create_ast_serializer()
        if (serialize_program(ser, program)) {
            ast_serializer_get_data(ser) $(data) {
                fopen(blob, "wb") $(bf) {
                    if (bf != null) {
                        bf |> fwrite(data)
                        wrote = true
                    }
                }
            }
        }
        delete_ast_serializer(ser)
        res = split_round(program)
    }
    return wrote
}

def private deser_round(blob : string; var res : SplitRound) : bool {
    var read_back = false
    ast_gc_guard() {
        var data : array<uint8>
        fopen(blob, "rb") $(f) {
            if (f != null) {
                data |> resize(int64(stat(blob).size))
                fread(f, data)
            }
        }
        if (empty(data)) {
            return
        }
        var ser = create_ast_deserializer(data)
        var inscope access <- make_file_access("")
        access |> add_extra_module("llvm_jit_finalizer",
            "{get_das_root()}/modules/dasLLVM/daslib/llvm_jit_finalizer.das")
        using() $(var mg : ModuleGroup) {
            deserialize_program(ser, access, unsafe(addr(mg))) $(ok, program, _error) {
                if (!ok || program == null) {
                    return
                }
                read_back = true
                res = split_round(program)
            }
        }
        delete_ast_serializer(ser)
    }
    return read_back
}

[test(description = "a program deserialized from its AST blob hits the DLL its parsed twin wrote, so every function hash survives the serialization round trip", timeout = 1200)]
def test_jit_ast_roundtrip(t : T?) {
    if (!jit_enabled() || !das_is_dll_build()) {
        t |> skip("needs -jit and a DLL daslang build")
        return
    }
    let client = stage_split_client(t, "das_astrt")
    if (empty(client)) return
    let blob = "{dir_name(client)}/ast.bin"
    var parsed : SplitRound
    t |> success(parse_round(client, blob, parsed), "round 1 parsed: the program serialized")
    t |> success(parsed.ok, "round 1 parsed: the backend ran")
    t |> equal(parsed.partitions, 3, "round 1 parsed: one partition per das module")
    t |> equal(parsed.cached, 0, "round 1 parsed: nothing was cached to link")
    t |> equal(parsed.acc, 340, "round 1 parsed: the cross-partition answer")
    var deser : SplitRound
    t |> success(deser_round(blob, deser), "round 2 deserialized: the blob read back")
    t |> success(deser.ok, "round 2 deserialized: the backend ran")
    t |> equal(deser.partitions, 0, "round 2 deserialized: nothing re-emitted")
    t |> strictEqual(deser.output_path, parsed.output_path,
        "round 2 deserialized: every function hash survived the round trip")
    t |> equal(deser.acc, 340, "round 2 deserialized: the cross-partition answer")
    var err : string
    rmdir_rec(dir_name(parsed.output_path), err)
    rmdir_rec(dir_name(client), err)
}

let private SLOT_NAMES_FIXTURE = "modules/dasLLVM/tests/_slot_names_fixture.das"
let private SLOT_NAMES_FOLD_CHILD = "modules/dasLLVM/tests/_slot_names_fold_child.das"

struct private FixtureKeys {
    compiled : bool
    var_aot : uint64
    let_aot : uint64
    var_key : string
    let_key : string
    var_fold : uint64
    var_fold_selected : uint64
    other_fold : uint64
}

def private named(mod : Module?; name : string) : array<FunctionPtr> {
    var found : array<FunctionPtr>
    for_each_function(mod, name) $(fn) {
        found |> push(fn)
    }
    return <- found
}

def private dll_key(prog : Program?; funcs : array<FunctionPtr>) : string =>
    jit_dll_basename(prog, funcs, 3, 0, false, false, "", CpuClass.none, false, false,
        JitSanitize())

//! the fixture's keys and folds, read from one compiled and simulated program
def private fixture_keys() : FixtureKeys {
    var res : FixtureKeys
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            let fixture = "{get_das_root()}/{SLOT_NAMES_FIXTURE}"
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(compiled; var program; _issues) {
                return if (!compiled)
                simulate(program) $(simulated; _context; _errors) {
                    return if (!simulated)
                    let prog = get_ptr(program)
                    let mod = get_this_module(program)
                    let by_var <- named(mod, "by_var")
                    let by_let <- named(mod, "by_let")
                    let other <- named(mod, "other_slots")
                    return if (length(by_var) != 1 || length(by_let) != 1 || length(other) != 1)
                    res.compiled = true
                    res.var_aot = get_function_aot_hash(prog, by_var[0])
                    res.let_aot = get_function_aot_hash(prog, by_let[0])
                    res.var_key = dll_key(prog, by_var)
                    res.let_key = dll_key(prog, by_let)
                    res.other_fold = fold_extern_slot_names(prog, other, 0ul)
                    res.var_fold = fold_extern_slot_names(prog, by_var, 0ul)
                    var everything : array<FunctionPtr>
                    program_for_each_module(program) $(m) {
                        everything |> push_from(named(m, ""))
                    }
                    mark_jit_selection(prog, everything)
                    res.var_fold_selected = fold_extern_slot_names(prog, by_var, 0ul)
                    clear_jit_selection(prog)
                }
            }
        }
    }
    return res
}

[test(description = "a local turning let moves the JIT DLL key but not the AOT hash, the extern-slot fold ignores the jit selection, and another process folds the same slot names", timeout = 1200)]
def test_slot_names_key(t : T?) {
    let keys = fixture_keys()
    if (!keys.compiled) {
        t |> failure("the fixture {SLOT_NAMES_FIXTURE} did not compile and simulate")
        return
    }
    t |> run("a local that turns let keeps the AOT hash and moves the DLL key") @(t : T?) {
        t |> equal(keys.var_aot, keys.let_aot, "the AOT hash does not see the const on the at slot")
        t |> success(keys.var_key != keys.let_key, "by_var and by_let key the same DLL")
    }
    t |> run("the fold does not read the jit selection, so every plan of one program keys the same") @(t : T?) {
        t |> success(keys.var_fold != 0ul, "the walk folded by_var's slots")
        t |> equal(keys.var_fold, keys.var_fold_selected)
    }
    t |> run("another process folds the same names") @(t : T?) {
        let args <- get_command_line_arguments()
        var output : string
        let rc = run_and_capture([args[0], "-dasroot", get_das_root(),
            "{get_das_root()}/{SLOT_NAMES_FOLD_CHILD}"], output, 120.0)
        t |> equal(rc, 0, output)
        t |> success(output |> contains("fold={keys.other_fold}"), "this process folded {keys.other_fold}, the child printed: {output}")
    }
}

def private salt(layouts : uint64) : uint64 =>
    jit_env_salt(3, 0, false, false, "", CpuClass.none, true, true, JitSanitize(), layouts)

[test(description = "a handled layout term changes when the field's offset, the field, the type or the module changes, and a type's size is a term apart from a field's offset")]
def test_layout_term(t : T?) {
    let base = handled_layout_term("ast", "ExprBlock", "list", 64ul)
    t |> run("a field that moved is another term") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "list", 72ul) != base)
    }
    t |> run("the field, the type and the module each name the term") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "finalList", 64ul) != base)
        t |> success(handled_layout_term("ast", "ExprLet", "list", 64ul) != base)
        t |> success(handled_layout_term("rtti", "ExprBlock", "list", 64ul) != base)
    }
    t |> run("a type's size is not the offset of a field of that value") @(t : T?) {
        t |> success(handled_layout_term("ast", "ExprBlock", "", 64ul) != base)
    }
}

def private layouts_of(script : string) : uint64 {
    var layouts = 0ul
    compile_for_jit("{get_this_module_dir()}/{script}", false) $(var program) {
        layouts = handled_layout_hash(get_ptr(program))
    }
    return layouts
}

[test(description = "a program's handled layout hash is the same on every walk, covers handled types that bind no structure, folds a module the program requires, and does not move when another program of the process loads a module")]
def test_layout_hash(t : T?) {
    t |> run("a program's layouts hash the same on every walk") @(t : T?) {
        compile_for_jit("{get_this_module_dir()}/_llvm_compile_only_client.das",
                        false) $(var program) {
            let first = handled_layout_hash(get_ptr(program))
            t |> success(first != 0ul, "the walk found handled types")
            t |> equal(handled_layout_hash(get_ptr(program)), first)
        }
    }
    t |> run("a handled type that binds no structure is a term too") @(t : T?) {
        compile_for_jit("{get_this_module_dir()}/_llvm_compile_only_client.das",
                        false) $(var program) {
            var structures = 0ul
            var others = 0
            get_ptr(program) |> for_each_module() $(m) {
                let mod = string(m.name)
                module_for_each_annotation(m) $(value) {
                    return if (!value.isTypeAnnotation)
                    if (!value.isBasicStructureAnnotation) {
                        others++
                        return
                    }
                    let typ = string(value.name)
                    structures += handled_layout_term(mod, typ, "",
                                                      uint64(
                                                             get_handled_type_size(
                        unsafe(reinterpret<TypeAnnotation?>(value)))))
                    for_each_field(*unsafe(reinterpret<BasicStructureAnnotation?>(value))) $(name, _cppName, _xtype, offset) {
                        structures += handled_layout_term(mod, typ, name, uint64(offset))
                    }
                }
            }
            t |> success(others > 0,
                "the program's library holds handled types that bind no structure ({others})")
            t |> success(handled_layout_hash(get_ptr(program)) != structures,
                         "the hash is not the structures' terms alone")
        }
    }
    t |> run("a required module's layouts fold, another program's do not") @(t : T?) {
        let plain = layouts_of("_llvm_compile_only_client.das")
        let with_module = layouts_of("_llvm_layouts_module_client.das")
        t |> success(with_module != plain,
                     "the program that requires the module folds its handled types")
        t |> equal(layouts_of("_llvm_compile_only_client.das"), plain,
                   "the module the other program loaded is no input of this one")
    }
}

[test(description = "the salt both JIT keys take is the env salt over the program's handled layout hash, and the env salt folds the layouts it is given and the typed aliasing switch")]
def test_salt_folds_layouts(t : T?) {
    let layouts = layouts_of("_llvm_compile_only_client.das")
    t |> run("the salt a compile takes is the salt over its program's layouts") @(t : T?) {
        compile_for_jit("{get_this_module_dir()}/_llvm_compile_only_client.das",
                        false) $(var program) {
            t |> equal(
                       jit_program_salt(get_ptr(program), 3, 0, false, false, "", CpuClass.none,
                                        true, true, JitSanitize()),
                salt(handled_layout_hash(get_ptr(program))))
        }
    }
    t |> run("other layouts are another salt") @(t : T?) {
        t |> success(salt(layouts + 1ul) != salt(layouts))
    }
    t |> run("the typed aliasing switch is another salt") @(t : T?) {
        let on = salt(layouts)
        g_jit_strict_aliasing = false
        let off = salt(layouts)
        g_jit_strict_aliasing = true
        t |> success(on != off)
        t |> equal(on, salt(layouts))
    }
}

def private folder_entries(folder : string) : array<string> {
    var entries : array<string>
    dir(folder) $(name) {
        if (name != "." && name != "..") {
            entries |> push(name)
        }
    }
    return <- entries
}

[test(description = "under DAS_JIT_MANIFEST a JIT run records the cached DLL it writes and the next run records the one it loads, a written DLL lists objects that are on disk beside it, a run with the variable empty records nowhere, and a manifest that cannot be written is a warning", timeout = 1200)]
def test_jit_manifest_records(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    let client = stage_split_client(t, "jit_manifest")
    if (empty(client)) return
    let work = dir_name(client)
    let manifest = path_join(work, "manifest")
    let exe = get_command_line_arguments()[0]
    var output : string
    let write_exit = run_and_capture([exe, "-jit", client], work, ["DAS_JIT_MANIFEST={manifest}"],
                                     output, 300.0)
    t |> equal(write_exit, 0, output)
    let entries <- folder_entries(manifest)
    t |> equal(length(entries), 1, "the run that wrote the DLL recorded it: {join(entries, " ")}")
    if (length(entries) == 1) {
        let dot = find(entries[0], ".")
        let folder = path_join(work, ".cache/daslang/{slice(entries[0], 0, dot)}")
        let key = slice(entries[0], dot + 1)
        let objs <- split(fread(path_join(folder, "{key}.files")), "\n")
        t |> success(length(objs) > 1,
                     "the split DLL lists its partition objects: {join(objs, " ")}")
        for (obj in objs) {
            t |> success(stat(path_join(folder, obj)).is_valid, "{obj} is on disk")
        }
        remove(path_join(manifest, entries[0]))
        let load_exit = run_and_capture([exe, "-jit", client], work,
                                        ["DAS_JIT_MANIFEST={manifest}"], output, 300.0)
        t |> equal(load_exit, 0, output)
        let again <- folder_entries(manifest)
        t |> equal(join(again, " "), entries[0],
                   "the run that loaded the cached DLL recorded the same key")
        remove(path_join(manifest, entries[0]))
        let before <- folder_entries(work)
        let unset_exit = run_and_capture([exe, "-jit", client], work, ["DAS_JIT_MANIFEST="], output,
                                         300.0)
        t |> equal(unset_exit, 0, output)
        t |> equal(length(folder_entries(manifest)), 0,
                   "a run with the variable empty records nothing in the manifest")
        t |> equal(join(folder_entries(work), " "), join(before, " "),
                   "and leaves no record anywhere else")
        let blocked_exit = run_and_capture([exe, "-jit", client], work,
                                           ["DAS_JIT_MANIFEST={client}/manifest"], output, 300.0)
        t |> equal(blocked_exit, 0, output)
        t |> success(find(output, "cannot record") >= 0,
                     "a manifest under a file cannot be written, and the run says so: {output}")
    }
    rmdir_rec(work)
}

[test(description = "the emitter's own cache hit records the DLL it loads under DAS_JIT_MANIFEST, and a DLL with no content key is not recorded", timeout = 1200)]
def test_jit_manifest_records_emitter_hit(t : T?) {
    if (!jit_enabled()) {
        t |> success(true)
        return
    }
    var terr : string
    let work = create_temp_directory("jit_manifest_emitter", terr)
    if (empty(work)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    for (
         f in ["_llvm_manifest_emitter_root.das", "_llvm_cache_common.das",
               "_llvm_compile_only_client.das"]) {
        t |> success(copy_fixture(get_this_module_dir(), work, f), "stage {f}")
    }
    let manifest = path_join(work, "manifest")
    var output : string
    let exit_code = run_and_capture(
                                    [get_command_line_arguments()[0],
                                     path_join(work, "_llvm_manifest_emitter_root.das")],
        work, ["DAS_JIT_MANIFEST={manifest}"], output, 600.0)
    t |> equal(exit_code, 0, output)
    t |> success(find(output, "EMPTIED 1") >= 0,
        "the run that wrote the DLL recorded it, and the root emptied the manifest: {output}")
    t |> success(find(output, "ROUND 1 ok=true") >= 0,
                 "the second run reached the emitter: {output}")
    t |> success(find(output, "KEYLESS DONE") >= 0,
                 "the root asked for a record of a DLL with no key: {output}")
    t |> equal(length(folder_entries(manifest)), 1,
               "the emitter's cache hit recorded its DLL, and the keyless one left no record")
    rmdir_rec(work)
}

def private sweep_fixture(t : T?; tag : string) : string {
    var terr : string
    let root = create_temp_directory("jit_sweep_{tag}", terr)
    if (empty(root)) {
        t |> failure("create_temp_directory: {terr}")
        return ""
    }
    for (folder in ["cache/_anon_1", "cache/_anon_2", "cache/other", "manifest"]) {
        mkdir_rec(path_join(root, folder))
    }
    let files = [
        (name = "cache/_anon_1/0xa.dll", text = "dll"),
        (name = "cache/_anon_1/0xa.files", text = "0x1.m_main.o\n0x2.m_fio.o"),
        (name = "cache/_anon_1/0x1.m_main.o", text = "obj1"),
        (name = "cache/_anon_1/0x2.m_fio.o", text = "obj2"),
        (name = "cache/_anon_1/0xb.dll", text = "older"),
        (name = "cache/_anon_1/0xb.files", text = "0x3.m_main.o"),
        (name = "cache/_anon_1/0x3.m_main.o", text = "older obj"),
        (name = "cache/_anon_2/0xc.dll", text = "unused"),
        (name = "cache/_anon_file", text = "a file, not a cache folder"),
        (name = "cache/other/keep.txt", text = "not a cache folder"),
        (name = "manifest/_anon_1.0xa", text = ""),
        (name = "manifest/_anon_9.0xz", text = "")
    ]
    for (f in files) {
        t |> success(fwrite(path_join(root, f.name), f.text), "wrote {f.name}")
    }
    return root
}

def private files_left(root : string) : string {
    var left : array<string>
    for (folder in ["cache/_anon_1", "cache/_anon_2", "cache/other"]) {
        left |> push_from(
                          [for (name in folder_entries(path_join(root, folder)));
                           "{folder}/{name}"])
    }
    sort(left)
    return join(left, " ")
}

let private RECORDED_SET = ("cache/_anon_1/0x1.m_main.o cache/_anon_1/0x2.m_fio.o " +
    "cache/_anon_1/0xa.dll cache/_anon_1/0xa.files cache/other/keep.txt")

let private FIXTURE_ALL_FILES = ("cache/_anon_1/0x1.m_main.o cache/_anon_1/0x2.m_fio.o " +
    "cache/_anon_1/0x3.m_main.o cache/_anon_1/0xa.dll cache/_anon_1/0xa.files " +
    "cache/_anon_1/0xb.dll cache/_anon_1/0xb.files cache/_anon_2/0xc.dll cache/other/keep.txt")

[test(description = "a JIT cache sweep keeps each recorded DLL with the objects its link list names, and deletes the older sets and the folders no run recorded; a dry run counts the same and deletes nothing; a missing or empty manifest sweeps nothing and says which it was; a file it cannot delete is counted")]
def test_jit_sweep(t : T?) {
    t |> run("a dry run counts and deletes nothing") @(t : T?) {
        let root = sweep_fixture(t, "dry")
        if (empty(root)) return
        let res = jit_sweep(path_join(root, "cache"), path_join(root, "manifest"), true)
        t |> equal(res.error, "")
        t |> equal(res.folders_kept, 1)
        t |> equal(res.folders_removed, 1)
        t |> equal(res.files_kept, 4)
        t |> equal(res.files_removed, 4)
        t |> equal(res.bytes_removed, 32ul)
        t |> equal(files_left(root), FIXTURE_ALL_FILES)
        rmdir_rec(root)
    }
    t |> run("a sweep keeps the recorded set and what is not a cache folder") @(t : T?) {
        let root = sweep_fixture(t, "real")
        if (empty(root)) return
        let res = jit_sweep(path_join(root, "cache"), path_join(root, "manifest"), false)
        t |> equal(res.error, "")
        t |> equal(res.folders_removed, 1)
        t |> equal(res.files_removed, 4)
        t |> equal(res.failed_deletes, 0)
        t |> equal(files_left(root), RECORDED_SET)
        t |> success(!stat(path_join(root, "cache/_anon_2")).is_valid,
                     "the folder no run recorded is gone")
        t |> success(stat(path_join(root, "cache/_anon_file")).is_valid,
                     "a file named like a cache folder stays")
        rmdir_rec(root)
    }
    t |> run("an empty manifest sweeps nothing") @(t : T?) {
        let root = sweep_fixture(t, "empty")
        if (empty(root)) return
        remove(path_join(root, "manifest/_anon_1.0xa"))
        remove(path_join(root, "manifest/_anon_9.0xz"))
        let res = jit_sweep(path_join(root, "cache"), path_join(root, "manifest"), false)
        t |> success(find(res.error, "names no DLL") >= 0,
                     "the sweep says the manifest was empty: {res.error}")
        t |> equal(res.files_removed, 0)
        t |> equal(files_left(root), FIXTURE_ALL_FILES)
        rmdir_rec(root)
    }
    t |> run("a missing manifest directory sweeps nothing") @(t : T?) {
        let root = sweep_fixture(t, "missing")
        if (empty(root)) return
        let res = jit_sweep(path_join(root, "cache"), path_join(root, "no_such_dir"), false)
        t |> success(find(res.error, "no manifest directory") >= 0,
                     "the sweep says the directory was missing: {res.error}")
        t |> equal(files_left(root), FIXTURE_ALL_FILES)
        rmdir_rec(root)
    }
    t |> run("a file and a folder it cannot delete are counted and left") @(t : T?) {
        if (get_platform_name() != "windows") {
            t |> success(true, "an open file blocks its own delete on Windows alone")
            return
        }
        let root = sweep_fixture(t, "held")
        if (empty(root)) return
        let older = fopen(path_join(root, "cache/_anon_1/0xb.dll"), "rb")
        let unused = fopen(path_join(root, "cache/_anon_2/0xc.dll"), "rb")
        let res = jit_sweep(path_join(root, "cache"), path_join(root, "manifest"), false)
        fclose(older)
        fclose(unused)
        t |> equal(res.failed_deletes, 2, "the held file and the folder holding the other")
        t |> success(stat(path_join(root, "cache/_anon_1/0xb.dll")).is_valid,
                     "the held file is still there")
        t |> success(stat(path_join(root, "cache/_anon_2/0xc.dll")).is_valid,
                     "so is the one in the unrecorded folder")
        rmdir_rec(root)
    }
}

[test(description = "the jit tool's sweep front answers 1 with no manifest named or none to read and 0 after a sweep, and its summary names a dry run and the deletes that failed")]
def test_jit_sweep_cli(t : T?) {
    t |> run("no manifest named, or none to read, is exit 1 and deletes nothing") @(t : T?) {
        let root = sweep_fixture(t, "cli_none")
        if (empty(root)) return
        t |> equal(jit_sweep_cli(path_join(root, "cache"), "", false), 1)
        t |> equal(jit_sweep_cli(path_join(root, "cache"), path_join(root, "no_such_dir"), false),
                   1)
        t |> equal(files_left(root), FIXTURE_ALL_FILES)
        rmdir_rec(root)
    }
    t |> run("a sweep is exit 0, dry or not") @(t : T?) {
        let root = sweep_fixture(t, "cli_ok")
        if (empty(root)) return
        t |> equal(jit_sweep_cli(path_join(root, "cache"), path_join(root, "manifest"), true), 0)
        t |> equal(files_left(root), FIXTURE_ALL_FILES)
        t |> equal(jit_sweep_cli(path_join(root, "cache"), path_join(root, "manifest"), false), 0)
        t |> success(!stat(path_join(root, "cache/_anon_2")).is_valid, "the sweep ran")
        rmdir_rec(root)
    }
    t |> run("the summary names a dry run and the failed deletes") @(t : T?) {
        var res = JitSweepResult(files_removed = 4, folders_removed = 1)
        t |> success(jit_sweep_summary(res, true) |> starts_with("would delete 4 files"))
        t |> success(jit_sweep_summary(res, false) |> starts_with("deleted 4 files"))
        t |> success(find(jit_sweep_summary(res, false), "could not be deleted") < 0,
                     "no failure, no failure clause")
        res.failed_deletes = 2
        t |> success(find(jit_sweep_summary(res, false), "; 2 could not be deleted") >= 0)
    }
}
