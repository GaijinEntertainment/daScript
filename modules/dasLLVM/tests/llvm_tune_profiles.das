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
// the old full race. The fixture set is COPIED into a per-process temp directory and every
// spawn runs from the copy: [tune_scope(defaults=...)] resolves against the declaring file
// and the per-app sidecar resolves beside the root script, so the defaults dir, the profile
// files and the sidecar all land under the copy and the tracked fixtures beside this file
// are never written or removed. A profile's name carries this box's tune_cpu_class(), and
// its features field is this box's fingerprint (or empty, to fake a less-capable minting
// box).

def private cleared_tune_env_prefix() : string {
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

def private count_hits(lines : array<string>; needle : string) : int {
    var n = 0
    for (ln in lines) {
        if (find(ln, needle) >= 0) {
            n++
        }
    }
    return n
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

// The working copy's defaults dir is named apart from the dir the scope fixture declares, so
// no path this test writes is ever spelled like the tracked defaults dir beside it.
let private WORK_DEFAULTS = "tune_defaults"
let private SCOPE_FIXTURE = "llvm_tune_profiles_lib.das"
let private DECLARED_DEFAULTS = "defaults = \"./"

def private copy_fixture(t : T?; name, dstdir : string) {
    let text = fread("{get_this_module_dir()}/{name}")
    t |> success(!empty(text), "fixture {name} is unreadable")
    t |> success(fwrite(path_join(dstdir, name), text), "could not copy {name} into {dstdir}")
}

def private redirect_defaults(src : string) : string {
    let at = find(src, DECLARED_DEFAULTS)
    return src if (at < 0)
    let from = at + length(DECLARED_DEFAULTS)
    let close = find(src, "\"", from)
    return src if (close < 0)
    return "{slice(src, 0, at)}{DECLARED_DEFAULTS}{WORK_DEFAULTS}{slice(src, close)}"
}

def private copy_scope_fixture(t : T?; dstdir : string) {
    let src = fread("{get_this_module_dir()}/{SCOPE_FIXTURE}")
    let text = redirect_defaults(src)
    t |> success(text != src, "{SCOPE_FIXTURE} declares no relative {DECLARED_DEFAULTS}...\" for the copy to redirect")
    t |> success(fwrite(path_join(dstdir, SCOPE_FIXTURE), text), "could not copy {SCOPE_FIXTURE} into {dstdir}")
}

[test]
def test_llvm_tune_profiles(t : T?) { // nolint:STYLE038 - one flat spawn-and-assert rail per scenario; a split would hide the sequence the scenarios build on
    if (!jit_enabled()) {
        // no generator tier without the jit, so no stamped-tier fingerprint to read
        t |> success(true)
        return
    }
    let args <- get_command_line_arguments()  // aliases a process-global Array header - do not delete
    let bin = args[0]
    let dirRes = create_temp_directory_result("llvm_tune_profiles_")
    if (dirRes is error) {
        t |> failure("could not create a temp dir: {dirRes as error}")
        return
    }
    let work = dirRes as value
    copy_fixture(t, "llvm_tune_profiles_client.das", work)
    copy_fixture(t, "llvm_tune_profiles_tuner.das", work)
    copy_scope_fixture(t, work)
    let client = path_join(work, "llvm_tune_profiles_client.das")
    let sidecar = path_join(work, "llvm_tune_profiles_client.tune.json")
    let defdir = path_join(work, WORK_DEFAULTS)
    let klass = tune_cpu_class()
    let profile = path_join(defdir, "{klass}.tune-defaults.json")

    // 1. no profile file: the plain auto flow - full race (empty filter), the tuner's k2 runs
    remove(sidecar)
    remove(profile)
    var noProfile : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\"", noProfile), 0)
    t |> success(noProfile |> has("FAKE TUNER"))
    t |> success(noProfile |> has("ONLY=''"))
    t |> success(!(noProfile |> has("defaults profile")))
    t |> success(noProfile |> has("RESULT 7"))
    delete noProfile

    // 2. full-cover profile (this box's own fingerprint): adopted, no tuner, its winner stamps
    remove(sidecar)
    write_profile(defdir, klass, "k0", tune_feature_fingerprint())
    var fullCover : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\"", fullCover), 0)
    t |> success(fullCover |> has("adopted the '{klass}' defaults profile"))
    t |> success(!(fullCover |> has("FAKE TUNER")))
    t |> success(fullCover |> has("RESULT 5"))
    t |> success(fullCover |> has("STATUS prof_add|llvm_tune_profiles_probe|k0|manifest"))
    let adoptedSidecar = fread(sidecar)
    t |> success(find(adoptedSidecar, "\"adopted_profile\"") >= 0)
    delete fullCover

    // 3. a profile minted with no features leaves the k3 requires= seat unlocked here: adopt AND race just that family - the tuner's k2 outranks the adopted k0
    remove(sidecar)
    write_profile(defdir, klass, "k0", "")
    var unlockedSeat : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\"", unlockedSeat), 0)
    t |> success(unlockedSeat |> has("racing 1 unlocked/new families"))
    t |> success(unlockedSeat |> has("ONLY='prof_add'"))
    t |> success(unlockedSeat |> has("RESULT 7"))
    t |> success(unlockedSeat |> has("STATUS prof_add|llvm_tune_profiles_probe|k2|manifest"))
    delete unlockedSeat

    // 4. --tune outranks the profile: full re-race, no adoption
    var forcedTune : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\" -- --tune", forcedTune), 0)
    t |> success(!(forcedTune |> has("defaults profile")))
    t |> success(forcedTune |> has("FAKE TUNER"))
    delete forcedTune

    // 5. --tune-only converges: the relaunch strips both spellings, so exactly one tuner runs and
    //    the child does not re-tune forever (the strip arms are what keep it from looping)
    var onlyOne : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\" -- --tune-only prof_add", onlyOne), 0)
    t |> equal(count_hits(onlyOne, "FAKE TUNER"), 1)
    t |> success(onlyOne |> has("ONLY='prof_add'"))
    t |> success(onlyOne |> has("RESULT 7"))
    delete onlyOne
    var onlyEq : array<string>
    t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\" -- --tune-only=prof_add", onlyEq), 0)
    t |> equal(count_hits(onlyEq, "FAKE TUNER"), 1)
    delete onlyEq

    // 6. the class chain: only a LOWER class's profile on disk, and the box adopts it (the
    //    residue race then covers whatever its own class unlocks)
    var chain <- tune_class_chain()
    t |> equal(chain[0], tune_cpu_class(), "the box's own class heads its ladder - a class outside the chain could never adopt its own profile")
    if (length(chain) > 1) {
        let lower = chain[length(chain) - 1]
        remove(sidecar)
        remove(profile)
        write_profile(defdir, lower, "k0", tune_feature_fingerprint())
        var lowerAdopt : array<string>
        t |> equal(spawn_child("{cleared_tune_env_prefix()}\"{bin}\" -jit \"{client}\"", lowerAdopt), 0)
        t |> success(lowerAdopt |> has("adopted the '{lower}' defaults profile"))
        t |> success(!(lowerAdopt |> has("FAKE TUNER")))
        delete lowerAdopt
        remove(path_join(defdir, "{lower}.tune-defaults.json"))
    }
    delete chain

    // 7. DAS_TUNE_POLICY=reference serves the original bodies and announces itself even on a
    //    fully tuned box (the sidecar from case 5 is complete)
    var refRun : array<string>
    let refEnv = (get_platform_name() == "windows" ?
        "set DAS_TUNE_MODE=&& set DAS_TUNE_MANIFEST=&& set DAS_TUNE_VERBOSITY=verbose&& set DAS_TUNE_POLICY=reference&& set DAS_TUNE_ONLY=&& " :
        "DAS_TUNE_MODE= DAS_TUNE_MANIFEST= DAS_TUNE_VERBOSITY=verbose DAS_TUNE_POLICY=reference DAS_TUNE_ONLY= ")
    t |> equal(spawn_child("{refEnv}\"{bin}\" -jit \"{client}\"", refRun), 0)
    t |> success(refRun |> has("policy 'reference' set by DAS_TUNE_POLICY"))
    t |> success(refRun |> has("STATUS prof_add|llvm_tune_profiles_probe||reference"))
    t |> success(refRun |> has("RESULT 5"))
    delete refRun

    rmdir_rec(work)
}

[export]
def main {
    return
}
