options gen2

require dastest/testing_boost
require daslib/module_path
require daslib/strings_boost
require daslib/fio
require strings
require llvm/daslib/llvm_tune

// The shipped-defaults-profile rail ([tune_scope(defaults = ...)]) through child daslang
// processes: an untuned start adopts the class profile instead of racing (source=manifest,
// no tuner spawn); a profile whose minting box lacked a feature this box has races ONLY the
// unlocked family (DAS_TUNE_ONLY armed for the tuner, cleared after); no profile file =
// the old full race. The profile files are written HERE — their name carries this box's
// tune_cpu_class(), and their features field is this box's fingerprint (or empty, to fake
// a less-capable minting box).

def private tune_env() : string {
    return (get_platform_name() == "windows" ?
        "set DAS_TUNE_MODE=&& set DAS_TUNE_MANIFEST=&& set DAS_TUNE_VERBOSITY=verbose&& set DAS_TUNE_POLICY=&& set DAS_TUNE_ONLY=&& " :
        "DAS_TUNE_MODE= DAS_TUNE_MANIFEST= DAS_TUNE_VERBOSITY=verbose DAS_TUNE_POLICY= DAS_TUNE_ONLY= ")
}

def private spawn_child(cmd : string; var lines : array<string>) : int {
    var rc : int
    unsafe {
        rc = popen_timeout("{cmd} 2>&1", 300.0) $(f) {
            if (f == null) {
                return
            }
            while (!feof(f)) {
                let ln = strip(fgets(f))
                if (!empty(ln)) {
                    lines |> push("{ln}")
                }
            }
        }
    }
    return rc
}

def private has(lines : array<string>; needle : string) : bool {
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            return true
        }
    }
    return false
}

def private write_profile(dir, klass, winner, features : string) {
    mkdir(dir)
    let body = "\{ \"kernels\" : \{ \"prof_add\" : \"{winner}\" \}, \"provenance\" : \{ \"features\" : \"{features}\" \} \}"
    fwrite(path_join(dir, "{klass}.tune-defaults.json"), body)
}

[test]
def test_llvm_tune_profiles(t : T?) {
    if (!jit_enabled()) {
        //! no generator tier without the jit, so no stamped-tier fingerprint to read
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  //! locked process-global view - never delete it
    let bin = args[0]
    let client = "{get_this_module_dir()}/llvm_tune_profiles_client.das"
    let sidecar = "{get_this_module_dir()}/llvm_tune_profiles_client.tune.json"
    let defdir = "{get_this_module_dir()}/llvm_tune_profiles_defaults"
    let klass = tune_cpu_class()
    let profile = path_join(defdir, "{klass}.tune-defaults.json")

    //! 1. no profile file: the plain auto flow - full race (empty filter), the tuner's k2 runs
    remove(sidecar)
    remove(profile)
    var l1 : array<string>
    t |> equal(spawn_child("{tune_env()}\"{bin}\" -jit \"{client}\"", l1), 0)
    t |> success(l1 |> has("FAKE TUNER"))
    t |> success(l1 |> has("ONLY=''"))
    t |> success(!(l1 |> has("defaults profile")))
    t |> success(l1 |> has("RESULT 7"))
    delete l1

    //! 2. full-cover profile (this box's own fingerprint): adopted, no tuner, its winner stamps
    remove(sidecar)
    write_profile(defdir, klass, "k0", tune_feature_fingerprint())
    var l2 : array<string>
    t |> equal(spawn_child("{tune_env()}\"{bin}\" -jit \"{client}\"", l2), 0)
    t |> success(l2 |> has("adopted the '{klass}' defaults profile"))
    t |> success(!(l2 |> has("FAKE TUNER")))
    t |> success(l2 |> has("RESULT 5"))
    t |> success(l2 |> has("STATUS prof_add|llvm_tune_profiles_probe|k0|manifest"))
    let side2 = fread(sidecar)
    t |> success(find(side2, "\"adopted_profile\"") >= 0)
    delete l2

    //! 3. a profile minted with no features leaves the k3 requires= seat unlocked here: adopt AND race just that family - the tuner's k2 outranks the adopted k0
    remove(sidecar)
    write_profile(defdir, klass, "k0", "")
    var l3 : array<string>
    t |> equal(spawn_child("{tune_env()}\"{bin}\" -jit \"{client}\"", l3), 0)
    t |> success(l3 |> has("racing 1 unlocked/new families"))
    t |> success(l3 |> has("ONLY='prof_add'"))
    t |> success(l3 |> has("RESULT 7"))
    t |> success(l3 |> has("STATUS prof_add|llvm_tune_profiles_probe|k2|manifest"))
    delete l3

    //! 4. --tune outranks the profile: full re-race, no adoption
    var l4 : array<string>
    t |> equal(spawn_child("{tune_env()}\"{bin}\" -jit \"{client}\" -- --tune", l4), 0)
    t |> success(!(l4 |> has("defaults profile")))
    t |> success(l4 |> has("FAKE TUNER"))
    delete l4

    remove(sidecar)
    remove(profile)
}

[export]
def main {
    return
}
