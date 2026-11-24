options gen2
options _dasllama_internal = true

// A pod fleet for the test runner: one RunPod pod a GPU home on the
// models volume, each with its own copy of the volume's tree and its
// own folder of links to the volume's models (an image a cell mints
// lands on the pod's disk, and the runner reaps it after the file).
// A run copies the models its tests open onto the pod's disk in place
// of their links, in the order its children open them, while the
// model-free suite and the earlier children run: a child starts once
// its own models are copies. Ahead of the suites each pod warms the
// CUDA home's kernels into its tree's cubin cache (cuda_prewarm.das).
// A home's caches live on the volume between sessions
// (/workspace/cache/<home>/): its run copies them onto the pod first
// and back once its runs end, the files not modified for 14 days
// pruned; a home's first run starts cold.
// The first pod runs the whole run.das command on the Vulkan home,
// on a two-pod fleet less the CUDA-only files (`--exclude`
// CUDA_ONLY_EXCLUDE), the second the same on the CUDA home, the command kept to the CUDA
// pass's files (`--only` CUDA_PASS_ONLY), each thirty files a child
// unless the caller's `--args` name a `--batch`. The model-free suite runs
// once a fleet: on the CUDA home's pod, or on a one-pod fleet's lone
// pod. No pod runs a part of a
// home's run: a home's run that outgrows its pod cuts tests, never
// adds a pod. The homes' logs land on the volume
// (/workspace/logs/fleet/<run>/), so any pod reads them all. The
// fleet file holds the pods this tool made and nothing else: `down`
// deletes those alone.
//
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- up
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run  # per PR
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run --full               # an arc's final run: no size tier for files no category holds
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run --args "--suite model-free" --env "DASLLAMA_GPU=1"
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- status
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- times                    # where the last run's test time went: the files and the cells
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- down

require daslib/cmp
require daslib/clargs
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require strings
require dasllama/dasllama_env
require ../tests/_model_categories.das   // CUDA_PASS_ONLY, CUDA_ONLY_EXCLUDE: what each home's run keeps
require ../tests/_daslang_argv.das   // disable_module_flags: the prewarm's flags for the modules a home leaves out
require ../../../utils/common/text_lines.das   // non_empty_lines: a remote read's own line
require math   // min: the times report's top cut

[CommandLineArgs]
struct FleetArgs {
    @clarg_positional
    @clarg_doc = "up | sync | run | status | times | down"
    verb : string
    @clarg_doc = "up: pods to make, 1 or 2 - the first runs the Vulkan home, the second CUDA"
    pods : int = 2
    @clarg_doc = "up: the GPU, as `runpodctl gpu list` names it"
    gpu : string = "NVIDIA RTX PRO 4500 Blackwell"
    @clarg_doc = "up: the network volume mounted at /workspace - the tree at /workspace/daScript, the models at /workspace/models"
    volume : string = "0xba7n8wvn"
    @clarg_doc = "up: the volume's data center"
    datacenter : string = "EU-RO-1"
    @clarg_doc = "up: the container image"
    image : string = "nvidia/cuda:12.8.1-devel-ubuntu24.04"
    @clarg_doc = "up: the pods' container disk in GB - the tree's copy, the children's AST records and the copies of the models a run opens live there"
    disk : int = 200
    @clarg_doc = "up: the fewest cores a pod's cgroup quota may give it, else remade; 0 = off"
    min_cores : float = 20.0
    @clarg_doc = "up: the slowest a pod may hand a process fresh memory, in GB a second, else remade; 0 = off"
    min_gbs : float = 1.0
    @clarg_doc = "the ssh private key the pods admit; its .pub is handed to every pod"
    key : string = "~/.ssh/runpod"
    @clarg_doc = "run: the children's environment - KEY=VALUE tokens separated by spaces; the models directory is the pod's own folder of links to the volume's models, each model the run opens copied in place of its link, the whisper packs sit in that folder and the ASR clips in its asr_corpus subfolder, the children's scratch is on the pod's own disk (the run's TMPDIR is the volume's, which reads a fresh file back slower than the pod's own disk), the image rail is off (DASLLAMA_IMAGE=0), allocations take no huge-page hint (DAS_NO_HUGEPAGE_HINT=1: a pod's host stalls on them) and so is the large tier off; an image suite's run passes its own --env with the rail on"
    env : string = "DASLLAMA_MODELS_DIR=/root/models DASLLAMA_WHISPER_DIR=/root/models DASLLAMA_CORPUS_DIR=/root/models/asr_corpus DASLLAMA_SCRATCH_DIR=/root/scratch DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DAS_NO_HUGEPAGE_HINT=1 DASLLAMA_ALLOW_UNTUNED=1"
    @clarg_doc = "run: the full suite - the children also take DASLLAMA_PARITY_FULL=1, which lifts the size tier for the files no category holds and every test wall; a model past the tier is a category's and runs only under run.das --category; an arc's final run"
    full : bool
    @clarg_doc = "run: the run.das arguments, no --only; the CUDA pod adds the CUDA pass's, both pods `--batch 30` where they name no --batch"
    args : string = "--changed --base origin/master"
    @clarg_doc = "status, times: the run to read (default: the fleet file's last run)"
    run : string
    @clarg_doc = "the fleet file - the pods this tool made, and its last run"
    fleet : string = "~/.dasllama/fleet.json"
    @clarg_doc = "the runpodctl binary"
    runpodctl : string = "runpodctl"
    @clarg_doc = "show this help and exit"
    help : bool
}

struct FleetPod {
    id : string
    name : string
    ip : string
    port : int
    index : int
}

struct Fleet {
    pods : array<FleetPod>
    last_run : string
}

//! the index `up` gives its next pod: past the largest live one, since a partial `down` leaves the set sparse. Non-private:
//! the pod-fleet tests (tests/test_pod_fleet.das) hold it
def next_pod_index(fleet : Fleet) : int {
    var top = 0
    for (pod in fleet.pods) {
        if (pod.index > top) {
            top = pod.index
        }
    }
    return top + 1
}

//! the GPU homes a fleet runs, a pod each in this order
def fleet_homes() : array<string> => <- ["vulkan", "cuda"]

//! a fleet of `have` pods taking `add` more: the refusal, or "" when
//! it stands - a pod a home, two at most. Non-private: the pod-fleet
//! tests (tests/test_pod_fleet.das) hold
//! it
def fleet_refusal(have, add : int) : string {
    let most = length(fleet_homes())
    return "pod_fleet: --pods takes 1 or {most}, not {add}" if (add < 1 || add > most)
    return "" if (have + add <= most)
    return ("pod_fleet: a fleet holds one pod a GPU home ({join(fleet_homes(), ", ")}),"
        + " {most} at most, not {have + add}: a home's run that outgrows its pod cuts tests"
        + " and never adds a pod")
}

//! the refusal of `run` arguments that carry their own `--only`, or
//! "": each home runs its whole per-PR set, the CUDA home its pass's
//! files. Non-private: the pod-fleet tests
//! (tests/test_pod_fleet.das) hold it
def args_refusal(args : string) : string {
    for (word in split(args, " ")) {
        if (word == "--only" || word |> starts_with("--only=")) {
            return ("pod_fleet: --args carries --only; the fleet runs each home's whole per-PR set"
                + " - the Vulkan home every file, the CUDA home its pass's ({CUDA_PASS_ONLY})")
        }
    }
    return ""
}

//! each home's pod: its seat in the fleet's order, whatever the pods'
//! indices (a partial `down` leaves them sparse), -1 for a home with
//! no pod, and whether its run holds the fleet's one model-free suite
//! - the last seated home's: the CUDA home where it has a pod, else
//! the lone pod's. Non-private: the pod-fleet tests
//! (tests/test_pod_fleet.das) hold it
def home_plan(fleet : Fleet) : array<tuple<home : string; seat : int; model_free : bool>> {
    let homes <- fleet_homes()
    let n = length(fleet.pods)
    let last = min(n, length(homes)) - 1
    return <- [for (i, home in iter_range(homes), homes); (home = home, seat = i < n ? i : -1, model_free = i == last)]
}

//! the shape of a `runpodctl pod create --wait -o json` record this tool reads; every other key is ignored
struct PodSsh {
    ip : string
    port : int
}

struct PodRecord {
    id : string
    name : string
    ssh : PodSsh
}

//! the pods `up` makes at one index before a reading under a
//! floor fails it
let POD_TRIES = 3

//! a pod's CPU allotment as its cgroup's `cpu.max` reads: the
//! cores its quota buys a period, unlimited (the quota `max`),
//! or why the text is no `cpu.max`
variant CpuMax {
    cores : float
    unlimited : bool
    failure : string
}

//! the last line of a remote read that is not empty: the ssh call's own warnings come back ahead of the line the
//! command printed
def private last_line(text : string) : string {
    let lines <- non_empty_lines(text)
    return empty(lines) ? "" : lines[length(lines) - 1]
}

//! the remote command that reads the pod's cgroup CPU quota.
//! Non-private: the pod-fleet tests read it
def cpu_max_command() : string => "cat /sys/fs/cgroup/cpu.max"

//! a `cpu.max` text as the pod's allotment: its last line that
//! is not empty is `<quota> <period>`, since the ssh call's own
//! warnings come back ahead of the file's line. Non-private:
//! the pod-fleet tests read it
def cpu_max_cores(text : string) : CpuMax {
    let line = last_line(text)
    let words <- split(line, " ")
    return CpuMax(failure = "cpu.max is not `<quota> <period>`: '{line}'") if (length(words) != 2)
    let period = try_to_int64(words[1])
    return CpuMax(failure = "cpu.max period '{words[1]}' is not a number") if (period is err)
    return CpuMax(failure = "cpu.max period is {words[1]}: no rate") if (unwrap(period) <= 0l)
    return CpuMax(unlimited = true) if (words[0] == "max")
    let quota = try_to_int64(words[0])
    return CpuMax(failure = "cpu.max quota '{words[0]}' is not a number") if (quota is err)
    return CpuMax(cores = float(unwrap(quota)) / float(unwrap(period)))
}

//! whether `value` meets `floor`: every value where the floor is off (0 or under)
def private meets_floor(floor, value : float) : bool => floor <= 0.0 || value >= floor

//! whether `up` keeps a new pod: every pod with the check off
//! (`min_cores` 0), an unlimited one, one at the floor or over
//! it; an unread allotment is replaced. Non-private: the
//! pod-fleet tests read it
def keeps_pod(min_cores : float; cpu : CpuMax) : bool {
    return true if (min_cores <= 0.0 || cpu is unlimited)
    return cpu is cores && meets_floor(min_cores, cpu as cores)
}

//! how fast a pod hands a process fresh memory, in GB a second, or why the read gave no rate
variant MemoryRate {
    gbs : float
    failure : string
}

//! the remote command that reads how fast the pod hands a process fresh memory: one 2 GB buffer of `dd`'s,
//! filled once, its summary in the C locale's words. Non-private: the pod-fleet tests read it
def memory_rate_command() : string => "LC_ALL=C dd if=/dev/zero of=/dev/null bs=2000M count=1 2>&1 | tail -1"

//! the rate out of `dd`'s summary line (`<bytes> bytes ... copied, <seconds> s...`), the last line of `text` that
//! is not empty. Non-private: the pod-fleet tests read it
def memory_rate(text : string) : MemoryRate {
    let line = last_line(text)
    let words <- split(line, " ")
    let copied = find_index(words, "copied,")
    let unread = "dd's summary is not `<bytes> bytes ... copied, <seconds> s`: '{line}'"
    return MemoryRate(failure = unread) if (copied < 0 || copied + 1 >= length(words))
    let bytes = try_to_int64(words[0])
    let seconds = try_to_float(words[copied + 1])
    return MemoryRate(failure = unread) if (bytes is err || seconds is err || unwrap(seconds) <= 0.0)
    return MemoryRate(gbs = float(double(unwrap(bytes)) / 1000000000.0lf) / unwrap(seconds))
}

//! whether `up` keeps a new pod by its memory: every pod with the check off (`min_gbs` 0), one at the
//! floor or over it; an unread rate is replaced. Non-private: the pod-fleet tests read it
def keeps_memory(min_gbs : float; rate : MemoryRate) : bool =>
    min_gbs <= 0.0 || rate is gbs && meets_floor(min_gbs, rate as gbs)

//! the rate as `up`'s log line states it
def memory_reading(rate : MemoryRate) : string {
    return "{fmt(":.1f", rate as gbs)} GB/s of fresh memory" if (rate is gbs)
    return "no memory rate read ({rate as failure})"
}

//! the allotment as `up`'s log line states it
def cpu_reading(cpu : CpuMax) : string {
    return "{fmt(":.1f", cpu as cores)} cores" if (cpu is cores)
    return "unlimited cores" if (cpu is unlimited)
    return "no allotment read ({cpu as failure})"
}

//! what `up` read off a new pod - the CPU allotment `cpu`, and the memory read's exit code `rc` and output `out`
//! where the memory floor is on - as its log line states it, and whether the pod stays: both floors met. A failed
//! memory read is an unread rate. Non-private: the pod-fleet tests read it
def pod_reading(min_cores, min_gbs : float; cpu : CpuMax; rc : int; out : string) : tuple<kept : bool; text : string> {
    return (kept = keeps_pod(min_cores, cpu), text = cpu_reading(cpu)) if (min_gbs <= 0.0)
    let rate = rc == 0 ? memory_rate(out) : MemoryRate(failure = "ssh exit {rc}")
    return (kept = keeps_pod(min_cores, cpu) && keeps_memory(min_gbs, rate), text = "{cpu_reading(cpu)}, {memory_reading(rate)}")
}

//! the failure of `up` when every try at pod `index` came in
//! under a floor: each floor that is on and each try's readings.
//! Non-private: the pod-fleet tests read it
def floor_refusal(index : int; min_cores, min_gbs : float; readings : array<string>) : string {
    var floors, offs : array<string>
    if (min_cores > 0.0) {
        floors |> push("the {fmt(":.1f", min_cores)}-core floor")
        offs |> push("`--min-cores 0`")
    }
    if (min_gbs > 0.0) {
        floors |> push("the {fmt(":.1f", min_gbs)} GB/s memory floor")
        offs |> push("`--min-gbs 0`")
    }
    return ("pod_fleet: POD {index}: all {length(readings)} tries came in under {join(floors, " or ")}"
        + " ({join(readings, "; ")}); {join(offs, " and ")} keep{length(offs) == 1 ? "s" : ""} such a pod")
}

let POD_TREE = "/root/daScript"
let VOLUME_TREE = "/workspace/daScript"
let FLEET_LOGS = "/workspace/logs/fleet"
let VOLUME_MODELS = "/workspace/models"
let RUNNER = "modules/dasLLAMA/tests/run.das --"   //! the list call names no -module-cache: that flag prints a cache line on stdout, which is the list
let PREWARM = "modules/dasLLAMA/harness/cuda_prewarm.das"   //! the CUDA home's kernel prewarm, a step ahead of the suites
let POD_MODELS = "/root/models"   //! the pod's own models directory: a link a model file of the volume, or a copy where a run copied it, so what a run writes beside a model stays on the pod's disk
let VOLUME_CACHE = "/workspace/cache"   //! the caches a home's runs keep between sessions, one folder a GPU home (`<home>/daslang`, `<home>/nvidia`): the two pods never write one folder, and a home's environment keys its own records
let POD_NVIDIA_CACHE = "/root/.cache/nvidia"   //! the NVIDIA driver's disk cache: the Vulkan pipelines the driver builds at their first use
let CACHE_DAYS = 14   //! a volume cache file not modified in this many days goes when a run copies its caches back

//! a leading `~` as the user's home - USERPROFILE on Windows, else HOME, off the environment registry
def home_path(path : string) : string {
    return path if (!(path |> starts_with("~")))
    let home = !empty(g_env_core.user_profile) ? g_env_core.user_profile : g_env_core.home
    return "{home}{slice(path, 1)}"
}

//! the `runpodctl pod create` argument list for the fleet's pod `index`; `pub` is the public key line. Non-private:
//! the pod-fleet tests (tests/test_pod_fleet.das) read it
def create_argv(cfg : FleetArgs; index : int; pub : string) : array<string> {
    //! the container's command: sshd admitting the key, rsync for the tree's copy, git for --changed, the Vulkan
    //! loader for the Vulkan home with the GL and EGL loaders the NVIDIA ICD (`libGLX_nvidia`, which the container
    //! runtime brings) opens, and the libraries the tree's LLVM shared library links
    let boot = ("bash -c \"apt-get update && apt-get install -y --no-install-recommends openssh-server rsync git curl ca-certificates libvulkan1 libgl1 libegl1 libz3-4 libxml2"
        + " && mkdir -p /run/sshd /root/.ssh && echo \\\"$PUBLIC_KEY\\\" > /root/.ssh/authorized_keys"
        + " && chmod 700 /root/.ssh && chmod 600 /root/.ssh/authorized_keys && exec /usr/sbin/sshd -D\"")
    return <- [cfg.runpodctl, "pod", "create", "--name", "dasllama-fleet-{index}", "--image", cfg.image,
                "--gpu-id", cfg.gpu, "--network-volume-id", cfg.volume, "--data-center-ids", cfg.datacenter,
                "--container-disk-in-gb", "{cfg.disk}", "--ports", "22/tcp",
                "--env", "\{\"PUBLIC_KEY\":\"{pub}\",\"NVIDIA_DRIVER_CAPABILITIES\":\"all\"\}",
                "--docker-args", boot, "--wait", "-o", "json"]
}

//! the ssh argument list running `command` on `pod`
def ssh_argv(cfg : FleetArgs; pod : FleetPod; command : string) : array<string> {
    return <- ["ssh", "-i", home_path(cfg.key), "-p", "{pod.port}", "-o", "StrictHostKeyChecking=accept-new",
                "-o", "BatchMode=yes", "root@{pod.ip}", command]
}

//! the remote command that copies the volume's tree onto the pod's own disk - the JIT and AST caches under
//! `.cache/` are per tree, so two pods never share one - makes the fleet's log root, and refreshes the pod's models
//! directory: its links and the images and save temp files a run left are removed, the copies a run made stay, the
//! volume's folders are made again, and every file of the volume's but its prepared images and save temp files gets a
//! link where no copy stands, so a pod reads and writes no image another pod does, in a subfolder as at the top
def prep_command() : string =>
    ("mkdir -p {FLEET_LOGS} && rsync -a --delete --exclude .cache --exclude build {VOLUME_TREE}/ {POD_TREE}/"
        + " && mkdir -p {POD_MODELS} && find {POD_MODELS} -type l -delete"
        + " && find {POD_MODELS} -type f \\( -name \"*.dlim\" -o -name \"*.tmp\" \\) -delete && cd {VOLUME_MODELS}"
        + " && find . -type d -exec mkdir -p {POD_MODELS}/\{\} \\;"
        + " && find . ! -type d ! -name \"*.dlim\" ! -name \"*.tmp\""
        + " -exec sh -c '[ -e \"$1/$0\" ] || ln -s \"$2/$0\" \"$1/$0\"' \{\} {POD_MODELS} {VOLUME_MODELS} \\;"
        + " && echo PREP-OK")

//! the memory tracker, whose `drop <file>...` mode is the one spelling of a page-cache drop
let private MEM_TRACK = "modules/dasLLAMA/harness/pod_mem_track.sh"

//! the copier the run of `home` starts ahead of its runs: it walks the run's model list (`run.das --list-models`, in
//! the order the run's children open them) and replaces each listed file's link under the pod's models directory with
//! a copy off the volume - the link removed first and rsync's own temp file renamed into place, so nothing is written
//! through a link into the volume and a regular file at the path is a whole one. A copy of the volume file's size
//! and time stays: rsync keeps the time, so a file replaced on the volume at the same size is copied again. A copy
//! is synced and its pages, and the volume file's, dropped from the page cache: the pod's memory limit
//! counts the cache, and a cache at the limit stalls the loads on reclaim. The state folder `state` takes `done` when
//! the list is walked and `failed` naming the file a copy failed on. Non-private: the pod-fleet tests read it
def copy_command(state : string) : string =>
    ("while IFS= read -r p; do case \"$p\" in \"#\"*|\"\") continue;; esac;"
        + " d={POD_MODELS}/$p; s={VOLUME_MODELS}/$p;"
        + " if [ -f \"$d\" ] && [ ! -L \"$d\" ] && [ \"$(stat -c %s.%Y \"$d\")\" = \"$(stat -c %s.%Y \"$s\")\" ]; then continue; fi;"
        + " mkdir -p \"$(dirname \"$d\")\" && rm -f \"$d\" && rsync -a --whole-file \"$s\" \"$d\""
        + " && sync \"$d\" && sh {MEM_TRACK} drop \"$d\" \"$s\" && echo \"COPIED $p\""
        + " || \{ echo \"$p\" > {state}/failed; exit 1; \};"
        + " done < {state}/models.txt; echo ok > {state}/done")

//! whether the run.das arguments `args` are the model-free suite's run already, which the home's run then runs once
def runs_model_free(args : string) : bool {
    let words <- split(args, " ")
    for (word, at in words, count()) {
        return true if (word == "--suite=model-free" || (word == "--suite" || word == "-s") && at + 1 < length(words) && words[at + 1] == "model-free")
    }
    return false
}

//! files a child on a home's run: the pod's memory holds a child of thirty files, and a child's compile is paid once a batch
let FLEET_BATCH = 30

//! the run.das arguments `args` with `--batch` FLEET_BATCH, where they carry no `--batch` of their own (the caller's
//! wins) and are not the model-free suite's run, which keeps its own whole-suite batch. Non-private: the pod-fleet
//! tests read it
def batched_args(args : string) : string {
    return args if (runs_model_free(args))
    for (word in split(args, " ")) {
        return args if (word == "--batch" || word |> starts_with("--batch="))
    }
    return "{args} --batch {FLEET_BATCH}"
}

//! the run.das arguments `args` with the `--exclude` tokens `tokens` added: one `--exclude`, the tokens comma-joined to
//! the caller's where `args` carry one. Non-private: the pod-fleet tests read it
def excluded_args(args, tokens : string) : string {
    var inscope words <- split(args, " ")
    for (word, at in words, count()) {
        if (word == "--exclude" && at + 1 < length(words)) {
            words[at + 1] = "{words[at + 1]},{tokens}"
            return join(words, " ")
        } elif (word |> starts_with("--exclude=")) {
            words[at] = "{word},{tokens}"
            return join(words, " ")
        }
    }
    return "{args} --exclude {tokens}"
}

//! the runner's `-module-cache` record under the tree's cache: an explicit path, keyed by nothing
let UNKEYED_RECORD = "module_cache/dastest.dascache"

//! the remote command that copies the volume cache of `home` onto the pod ahead of its run - `daslang` into the tree's
//! `.cache/daslang`, `nvidia` into the driver's disk cache - by `rsync -a`, which keeps the mtimes and sizes the
//! caches' keys read; one `CACHE-IN` line: the size read in, `none` where the home has no cache yet (its run starts
//! cold), or the failed copy's exit, which stops no run. Non-private: the pod-fleet tests read it
def cache_in_command(home : string) : string {
    let at = "{VOLUME_CACHE}/{home}"
    return ("if [ -d {at} ]; then mkdir -p {POD_TREE}/.cache/daslang {POD_NVIDIA_CACHE}"
        + " && \{ [ ! -d {at}/daslang ] || rsync -a {at}/daslang/ {POD_TREE}/.cache/daslang/; \}"
        + " && \{ [ ! -d {at}/nvidia ] || rsync -a {at}/nvidia/ {POD_NVIDIA_CACHE}/; \}"
        + " && echo \"CACHE-IN $(du -sh {at} | cut -f1)\" || echo \"CACHE-IN failed: exit $?\";"
        + " else echo \"CACHE-IN none\"; fi")
}

//! the remote command that copies the pod's two caches back into the volume cache of `home` once its runs end, by
//! `rsync -a` and no `--delete` (a merge: a record this run did not read stays), then removes the volume cache's files
//! not modified for CACHE_DAYS days; one `CACHE-OUT` line: the size kept, or the failed step's exit, which reddens no
//! run. The runner's own module record (UNKEYED_RECORD) stays on the pod: its path is given, so nothing keys it, and a
//! later session's rebuilt binary would be served it. Non-private: the pod-fleet tests read it
def cache_out_command(home : string) : string {
    let at = "{VOLUME_CACHE}/{home}"
    return ("mkdir -p {at}/daslang {at}/nvidia"
        + " && \{ [ ! -d {POD_TREE}/.cache/daslang ] || rsync -a --exclude {UNKEYED_RECORD} {POD_TREE}/.cache/daslang/ {at}/daslang/; \}"
        + " && \{ [ ! -d {POD_NVIDIA_CACHE} ] || rsync -a {POD_NVIDIA_CACHE}/ {at}/nvidia/; \}"
        + " && find {at} -type f -mmin +{CACHE_DAYS * 24 * 60} -delete && find {at} -mindepth 1 -type d -empty -delete"
        + " && echo \"CACHE-OUT $(du -sh {at} | cut -f1)\" || echo \"CACHE-OUT failed: exit $?\"")
}

//! the dynamic modules `home`'s run leaves out (DASLLAMA_DISABLE_MODULES): the value the caller's `env` sets, else on a
//! two-pod fleet (`paired`) the other home's module - dasVulkan on the CUDA home, dasCUDA on the Vulkan home - and none
//! on a lone pod. Non-private: the pod-fleet tests read it
def home_disabled_modules(env, home : string; paired : bool) : string {
    for (word in split(env, " ")) {
        let key = "DASLLAMA_DISABLE_MODULES="
        return slice(word, length(key)) if (word |> starts_with(key))
    }
    return paired ? (home == "cuda" ? "dasVulkan" : "dasCUDA") : ""
}

//! the remote command that starts the run on `home`, the CUDA home's
//! run of the given command under DASLLAMA_CUDA=1 and kept to its
//! pass's files (`--only` CUDA_PASS_ONLY), a two-pod fleet's Vulkan
//! home's leaving out the CUDA-only files (`--exclude`
//! CUDA_ONLY_EXCLUDE, its model-free slice too), on both homes FLEET_BATCH
//! files a child (`batched_args`): the children's logs in the
//! home's folder of the run on the volume (TMPDIR), the runner's own
//! log `<home>.log` beside it, RUN-EXIT=<rc> as its last line. First
//! the home's volume cache comes onto the pod (`cache_in_command`), so
//! the model list, the first daslang start, finds it. Ahead
//! of the runs the runner lists the model files the given command
//! opens (`--list-models`, with the run's own arguments and
//! environment) and the copier (`copy_command`) starts on that list in
//! the background, its log and markers in the home's `copy/` folder;
//! then the CUDA kernel prewarm (`cuda_prewarm.das`, its PREWARM line
//! in the log) fills the tree's cubin cache, which the children read,
//! so no test pays a fresh cache's compile inside its wall - on every
//! home whose run keeps the CUDA module (`PREWARM none` where it does
//! not); where `model_free` holds (`home_plan`'s one home a fleet) the
//! model-free suite runs next while the copy runs - on a two-pod
//! fleet's Vulkan home its Vulkan files (`--only` VULKAN_FREE_ONLY),
//! which the CUDA home runs without their module -, then the given
//! command, both under `--await-models`: a child starts once its
//! models are copies on the pod's disk. The model-free suite runs
//! without the CUDA ask - it is the per-PR gate a bare box runs, and
//! under the ask its device files would run the Vulkan home's tests on
//! the CUDA home - and without the stop file, whose stop is the given
//! command's wall. On a two-pod fleet each home leaves out the other
//! home's dynamic module (`home_disabled_modules`, the environment's
//! DASLLAMA_DISABLE_MODULES, which the caller's `--env` overrides): its
//! children and its prewarm compile only their own GPU module. RUN-EXIT is red where either run
//! is, the copy failed or the prewarm did (PREWARM-EXIT, which stops
//! no suite: a kernel left out compiles at its load). The runner reaps the images its children
//! mint where the environment names the pod's own models directory
//! (`--reap-images`: under any other directory the mints are another
//! pod's too, and stay), both homes name the run's one stop file
//! (`--stop-file`: a test past its wall on one pod starts no later
//! child on any), and the memory tracker beside it keeps the largest
//! child's peak in the folder's `mem_peak.txt` and the model copies'
//! unmapped pages off the pod's memory limit. LD_LIBRARY_PATH puts
//! the pod's copy of the runtime libraries ahead of the binary's
//! RUNPATH, which names the volume tree's - a rebuild there mid-run
//! would otherwise kill every child started after it. Once the runs
//! end, whatever their verdicts, the pod's caches go back to the
//! home's volume cache (`cache_out_command`) ahead of RUN-EXIT, which
//! a failed copy does not redden. Non-private: the pod-fleet tests
//! read it
def run_command(cfg : FleetArgs; run, home : string; model_free : bool) : string {
    let paired = home == "cuda" || !model_free
    let disabled = home_disabled_modules(cfg.env, home, paired)
    let disable_env = empty(disabled) ? "" : " DASLLAMA_DISABLE_MODULES={disabled}"
    let prewarm_flags = disable_module_flags(disabled)
    let log = "{FLEET_LOGS}/{run}/{home}.log"
    let tmp = "{FLEET_LOGS}/{run}/{home}"
    let state = "{tmp}/copy"
    let reap = find(" {cfg.env} ", " DASLLAMA_MODELS_DIR={POD_MODELS} ") >= 0 ? " --reap-images" : ""
    let cuda = home == "cuda" ? "DASLLAMA_CUDA=1 " : ""
    let only = home == "cuda" ? " --only {CUDA_PASS_ONLY}" : ""
    let full = cfg.full ? " DASLLAMA_PARITY_FULL=1" : ""
    let runner = "./bin/daslang -jit -module-cache .cache/daslang/module_cache/dastest.dascache {RUNNER}"
    let paired_vulkan = paired && home == "vulkan"   //! leaves out the CUDA-only files, whose module it leaves out
    let batched = batched_args("{cfg.args}{only}")
    let given = paired_vulkan ? excluded_args(batched, CUDA_ONLY_EXCLUDE) : batched
    let given_is_it = runs_model_free(cfg.args)
    let skip = (split(cfg.args, " ") |> has_value("--rerun")) ? " --rerun" : ""   //! the caller's `--rerun` is the fleet's model-free suite's too
    let paired_vulkan_only = (model_free ? ""   //! the paired Vulkan home's slice
        : " --only {VULKAN_FREE_ONLY} --exclude {CUDA_ONLY_EXCLUDE}")
    let mf_run = (given_is_it ? " mf=0;"
        : " {runner} --suite model-free{paired_vulkan_only}{skip} --await-models {state}; mf=$?;")
    let mf_exit = "echo MODEL-FREE-EXIT=$\{mf:-none\}"
    let inner = ("cd {POD_TREE} && export LD_LIBRARY_PATH={POD_TREE}/lib TMPDIR={tmp}"
        + "{disable_env} {cfg.env}{full}"
        + " && \{ {cache_in_command(home)}; \}"
        + " && (sh {MEM_TRACK} {tmp}/mem_peak.txt {POD_MODELS} & echo $! > {tmp}/mem_track.pid)"
        + " && mkdir -p {state}"
        + " && \{ {cuda}./bin/daslang -jit {RUNNER} {given} --list-models > {state}/models.txt || echo \"the model list: exit $?\" > {state}/failed; \}"
        + " && \{ [ -f {state}/failed ] || ({copy_command(state)}) > {state}/copy.log 2>&1 & cp=$!; \}"
        + " && \{ ./bin/daslang{prewarm_flags} -jit {PREWARM}; pw=$?; \}"
        + " && \{{mf_run}"
        + " {cuda}{runner} {given}{reap} --await-models {state} --stop-file {FLEET_LOGS}/{run}/STOP; rc=$?; \};"
        + " rc=$\{rc:-$?\}; kill $cp 2>/dev/null; kill $(cat {tmp}/mem_track.pid);"
        + " {cache_out_command(home)};"
        + " [ -f {state}/failed ] && echo \"COPY-FAILED $(cat {state}/failed)\" && [ $rc = 0 ] && rc=1;"
        + " [ $rc = 0 ] && rc=$\{pw:-1\}; echo PREWARM-EXIT=$\{pw:-none\};"
        + " [ $rc = 0 ] && rc=$\{mf:-1\}; {mf_exit}; echo RUN-EXIT=$rc")
    //! `;` not `&&` ahead of the detached command: `a && b &` backgrounds a subshell that keeps the ssh session's
    //! output open, and the ssh call then lasts as long as the run
    let detached = "setsid nohup sh -c '{inner}' > {log} 2>&1 < /dev/null &"
    return "mkdir -p {tmp}; {detached} echo STARTED {home}"
}

//! the remote command that reads each home's log of `run` off the
//! volume, under the home's name: its DONE count, REPORT and NEXT
//! lines (the model-free suite's and the given command's, green or
//! red), MISSING lines (a model file the home's cells ask that the
//! volume lacks), WAIT lines (a child waiting on the model copy), the
//! prewarm's PREWARM and PREWARM-EXIT lines, the caches' CACHE-IN
//! and CACHE-OUT lines, MODEL-FREE-EXIT,
//! COPY-FAILED and RUN-EXIT lines and every red DONE
//! line; the copy's state - failed on a file, done with its count of
//! copies, or running; then each home's peak - the largest child's
//! resident kB and the file it ran. A home no pod ran has no log and
//! prints nothing
def status_command(run : string) : string {
    let homes = join(fleet_homes(), " ")
    let at = "{FLEET_LOGS}/{run}"
    return ("for h in {homes}; do f={at}/$h.log; [ -f $f ] || continue;"
        + " echo \"== $h: $(grep -cE 'DONE' $f) done\";"
        + " grep -E '^(\\[[IWE]\\] )?(REPORT|NEXT|WAIT) |^\\[W\\] MISSING|^PREWARM|^CACHE-(IN|OUT) |MODEL-FREE-EXIT|COPY-FAILED|RUN-EXIT|WALL |LOST |STOPPED' $f;"
        + " grep -E 'DONE' $f | grep -vE ' 0 failed, 0 errors';"
        + " c={at}/$h/copy; if [ -f $c/failed ]; then echo \"COPY $h: failed on $(cat $c/failed)\";"
        + " elif [ -f $c/done ]; then echo \"COPY $h: done, $(grep -c '^COPIED ' $c/copy.log) file(s) copied\";"
        + " else echo \"COPY $h: running\"; fi; done;"
        + " for h in {homes}; do p={at}/$h/mem_peak.txt;"
        + " [ -f $p ] && echo \"PEAK-KB $h: $(cat $p)\"; done; true")
}

//! the remote command that reads every child log of `run` off the volume into one line a verdict: `F <seconds> <file>` for a
//! test file's PASS / FAIL line and `T <seconds> <cell>` for a `[test]` function's, the colour codes stripped first
def times_command(run : string) : string =>
    ("cat {FLEET_LOGS}/{run}/*/dasllama_run_*.log | sed -E 's/\\x1b\\[[0-9;]*m//g'"
        + " | sed -nE \"s/^(PASS|FAIL) (modules\\/[^ ]+\\.das) \\(([0-9.]+)s\\).*/F \\3 \\2/p; s/^--- (PASS|FAIL) '([^']*)' \\(([0-9.]+)s\\).*/T \\3 \\2/p\"; true")

//! the run's test time by file (every file's seconds summed over the children that ran it) and its longest cells (a cell's
//! longest reading where a batch and its retry both ran it): the top `top` of each with the file's share of the whole
def times_report(lines : string; top : int) : string {
    var files, cells : table<string; float>
    for (ln in split(lines, "\n")) {
        let kind = slice(ln, 0, 1)
        continue if (kind != "F" && kind != "T")
        let cut = find(ln, " ", 2)
        continue if (cut < 0)
        let secs = to_float(slice(ln, 2, cut))
        let name = strip(slice(ln, cut + 1))
        if (kind == "F") {
            files[base_name(name)] += secs
        } elif (secs > (cells?[name] ?? 0.0)) {
            cells[name] = secs
        }
    }
    var total = 0.0
    for (v in values(files)) {
        total += v
    }
    var by_file <- [for (k, v in keys(files), values(files)); (name = k, secs = v)]
    var by_cell <- [for (k, v in keys(cells), values(cells)); (name = k, secs = v)]
    sort(by_file) <| cmp_by(-_.secs)
    sort(by_cell) <| cmp_by(-_.secs)
    return build_string() $(w) {
        w |> write("test time {int(total)} s over {length(by_file)} file(s)\n")
        w |> write("FILES\n")
        for (i in range(min(top, length(by_file)))) {
            w |> write("  {int(by_file[i].secs)} s  {total > 0.0 ? int(100.0 * by_file[i].secs / total) : 0}%  {by_file[i].name}\n")
        }
        w |> write("CELLS\n")
        for (i in range(min(top, length(by_cell)))) {
            w |> write("  {int(by_cell[i].secs)} s  {by_cell[i].name}\n")
        }
    }
}

def private verb_times(cfg : FleetArgs; fleet : Fleet) : int {
    let run = empty(cfg.run) ? fleet.last_run : cfg.run
    if (empty(run)) {
        to_log(LOG_ERROR, "pod_fleet: no run to read - `run` first, or name one with --run\n")
        return 2
    }
    if (empty(fleet.pods)) {
        to_log(LOG_ERROR, "pod_fleet: no fleet pod to read the volume through - `up` first\n")
        return 2
    }
    let read = run_capture(ssh_argv(cfg, fleet.pods[0], times_command(run)), 300.0)
    if (read.rc != 0) {
        to_log(LOG_ERROR, "TIMES {run} (exit {read.rc}):\n{read.out}\n")
        return 1
    }
    to_log(LOG_INFO, "TIMES {run}:\n{times_report(read.out, 15)}")
    return 0
}

def private load_fleet(path : string) : Fleet {
    var fleet : Fleet
    let text = fread(path)
    if (!empty(text) && !sscan_json(text, fleet)) {
        panic("pod_fleet: {path} is not a fleet file")
    }
    return <- fleet
}

def private save_fleet(path : string; fleet : Fleet) {
    mkdir_rec(dir_name(path))
    fopen(path, "wb") $(f) {
        panic("pod_fleet: cannot write {path}") if (f == null)
        f |> fwrite(sprint_json(fleet, true))
    }
}

def private run_capture(argv : array<string>; timeout_sec : float) : tuple<rc : int; out : string> {
    var out : string
    let rc = run_and_capture(argv, out, timeout_sec)
    return (rc = rc, out = out)
}

def private delete_pod(cfg : FleetArgs; id : string) : tuple<rc : int; out : string> =>
    run_capture([cfg.runpodctl, "pod", "delete", id], 120.0)

//! the pods `ids` deleted and taken out of the fleet file, each
//! one's line under `tag`; false when one outlived its delete (the
//! fleet file keeps it for `down`)
def private drop_pods(cfg : FleetArgs; var fleet : Fleet; ids : array<string>; tag : string) : bool {
    var all_gone = true
    for (id in ids) {
        let at = find_index_if(fleet.pods) $(p) => p.id == id
        let index = at >= 0 ? fleet.pods[at].index : -1
        let gone = delete_pod(cfg, id)
        if (gone.rc != 0) {
            to_log(LOG_ERROR, "{tag} {index}: {id} not deleted (exit {gone.rc}):\n{gone.out}\n")
            all_gone = false
            continue
        }
        to_log(LOG_INFO, "{tag} {index}: {id} deleted\n")
        if (at >= 0) {
            fleet.pods |> erase(at)
        }
    }
    save_fleet(home_path(cfg.fleet), fleet)
    return all_gone
}

//! pod `index` of `up`, made again while its cgroup allotment
//! comes in under `--min-cores` or its fresh memory under
//! `--min-gbs`, POD_TRIES pods in all. A short
//! pod stands until the tries end, so the next try is made
//! beside it and not in the slot it would free. 0 when one is
//! kept, 1 when none is made (the pods before it stand), 2 when
//! every try came in short or a short pod outlived its delete
def private make_pod(cfg : FleetArgs; var fleet : Fleet; index, want : int; pub : string) : int {
    var readings, short_ids : array<string>
    readings |> reserve(POD_TRIES)
    short_ids |> reserve(POD_TRIES)
    fleet.pods |> reserve(length(fleet.pods) + 1)
    for (attempt in range(POD_TRIES)) {
        let tried = "POD {index}/{want} (try {attempt + 1}/{POD_TRIES})"
        to_log(LOG_INFO, "{tried}: creating on {cfg.gpu} in {cfg.datacenter}\n")
        let made = run_capture(create_argv(cfg, index, pub), 900.0)
        var rec : PodRecord
        let at = find(made.out, "\{")
        let read_rec = at >= 0 && sscan_json(slice(made.out, at), rec)
        if (made.rc != 0 || !read_rec || empty(rec.id) || empty(rec.ssh.ip)) {
            to_log(LOG_ERROR, "{tried}: not made (exit {made.rc}):\n{made.out}\n")
            return drop_pods(cfg, fleet, short_ids, "POD") ? 1 : 2
        }
        let pod = FleetPod(id = rec.id, name = rec.name, ip = rec.ssh.ip, port = rec.ssh.port,
            index = index)
        fleet.pods |> push(pod)
        save_fleet(home_path(cfg.fleet), fleet)
        to_log(LOG_INFO, "{tried}: {rec.id} at {rec.ssh.ip}:{rec.ssh.port}\n")
        let read = run_capture(ssh_argv(cfg, pod, cpu_max_command()), 60.0)
        let cpu = read.rc == 0 ? cpu_max_cores(read.out) : CpuMax(failure = "ssh exit {read.rc}")
        var timed = (rc = 0, out = "")
        if (cfg.min_gbs > 0.0) {
            timed = run_capture(ssh_argv(cfg, pod, memory_rate_command()), 300.0)
        }
        let seen = pod_reading(cfg.min_cores, cfg.min_gbs, cpu, timed.rc, timed.out)
        if (seen.kept) {
            to_log(LOG_INFO, "POD {index} {rec.id}: {seen.text}\n")
            return drop_pods(cfg, fleet, short_ids, "POD") ? 0 : 2
        }
        readings |> push(seen.text)
        short_ids |> push(rec.id)
        to_log(LOG_INFO, "POD {index} {rec.id}: {seen.text}, under a floor - replaced once the tries end\n")
    }
    drop_pods(cfg, fleet, short_ids, "POD")
    to_log(LOG_ERROR, "{floor_refusal(index, cfg.min_cores, cfg.min_gbs, readings)}\n")
    return 2
}

//! `up`: `--pods` more pods join the fleet, their indices after the
//! pods already in it, each copying the tree once made; a fleet past
//! one pod a home is refused
def private verb_up(cfg : FleetArgs; var fleet : Fleet) : int {
    let refused = fleet_refusal(length(fleet.pods), cfg.pods)
    if (!empty(refused)) {
        to_log(LOG_ERROR, "{refused}\n")
        return 2
    }
    let pub = strip(fread("{home_path(cfg.key)}.pub"))
    if (empty(pub)) {
        to_log(LOG_ERROR, "pod_fleet: no public key at {home_path(cfg.key)}.pub\n")
        return 2
    }
    let first = length(fleet.pods)
    let first_index = next_pod_index(fleet)
    let want = first_index + cfg.pods - 1
    for (index in range(first_index, want + 1)) {
        let got = make_pod(cfg, fleet, index, want, pub)
        return 1 if (got == 2)
        break if (got == 1)
    }
    if (length(fleet.pods) == first) {
        to_log(LOG_ERROR, "pod_fleet: no pod made\n")
        return 1
    }
    for (pod in fleet.pods) {
        continue if (pod.index < first_index)
        to_log(LOG_INFO, "PREP {pod.index}: copying the tree\n")
        let prepped = run_capture(ssh_argv(cfg, pod, prep_command()), 1800.0)
        if (prepped.rc != 0 || find(prepped.out, "PREP-OK") < 0) {
            to_log(LOG_ERROR, "PREP {pod.index}: failed (exit {prepped.rc}):\n{prepped.out}\n")
            return 1
        }
    }
    to_log(LOG_INFO, "FLEET {length(fleet.pods)} pod(s) ready; `run` runs a home on each\n")
    return 0
}

//! `sync`: every pod copies the volume's tree again - after a push to the volume, before the next `run`
def private verb_sync(cfg : FleetArgs; fleet : Fleet) : int {
    if (empty(fleet.pods)) {
        to_log(LOG_ERROR, "pod_fleet: no fleet - `up` first\n")
        return 2
    }
    for (pod in fleet.pods) {
        let synced = run_capture(ssh_argv(cfg, pod, prep_command()), 1800.0)
        if (synced.rc != 0 || find(synced.out, "PREP-OK") < 0) {
            to_log(LOG_ERROR, "SYNC {pod.index}: failed (exit {synced.rc}):\n{synced.out}\n")
            return 1
        }
        to_log(LOG_INFO, "SYNC {pod.index}: the tree copied\n")
    }
    return 0
}

def private verb_run(cfg : FleetArgs; var fleet : Fleet) : int {
    if (empty(fleet.pods)) {
        to_log(LOG_ERROR, "pod_fleet: no fleet - `up` first\n")
        return 2
    }
    let refused = fleet_refusal(0, length(fleet.pods))
    if (!empty(refused)) {
        to_log(LOG_ERROR, "{refused}; `down` the fleet and `up` again\n")
        return 2
    }
    let narrowed = args_refusal(cfg.args)
    if (!empty(narrowed)) {
        to_log(LOG_ERROR, "{narrowed}\n")
        return 2
    }
    let stamp = run_capture(ssh_argv(cfg, fleet.pods[0], "date -u +%Y%m%d-%H%M%S"), 60.0)
    let run = strip(stamp.out)
    if (stamp.rc != 0 || empty(run)) {
        to_log(LOG_ERROR, "pod_fleet: pod {fleet.pods[0].index} is not reachable (exit {stamp.rc}):\n{stamp.out}\n")
        return 1
    }
    fleet.last_run = run   //! saved first: `status` reads a run whose CUDA start failed
    save_fleet(home_path(cfg.fleet), fleet)
    for (seat in home_plan(fleet)) {
        if (seat.seat < 0) {
            let held = length(fleet.pods)
            to_log(LOG_INFO, "RUN {run} {seat.home} home: no pod - the fleet holds {held}\n")
            continue
        }
        let pod = fleet.pods[seat.seat]
        let started = run_capture(ssh_argv(cfg, pod, run_command(cfg, run, seat.home, seat.model_free)), 60.0)
        if (started.rc != 0 || find(started.out, "STARTED") < 0) {
            let why = "(exit {started.rc}):\n{started.out}"
            to_log(LOG_ERROR, "RUN {run} {seat.home} home: not started on pod {pod.index} {why}\n")
            return 1
        }
        let on = "pod {pod.index} ({pod.id})"
        to_log(LOG_INFO, "RUN {run} {seat.home} home started on {on}: run.das -- {cfg.args}\n")
    }
    to_log(LOG_INFO, "RUN {run}: logs under {FLEET_LOGS}/{run}/ on the volume; `status` reads them\n")
    return 0
}

def private verb_status(cfg : FleetArgs; fleet : Fleet) : int {
    let run = empty(cfg.run) ? fleet.last_run : cfg.run
    if (empty(run)) {
        to_log(LOG_ERROR, "pod_fleet: no run to read - `run` first, or name one with --run\n")
        return 2
    }
    if (empty(fleet.pods)) {
        to_log(LOG_ERROR, "pod_fleet: no fleet pod to read the volume through - `up` first\n")
        return 2
    }
    let read = run_capture(ssh_argv(cfg, fleet.pods[0], status_command(run)), 120.0)
    to_log(read.rc == 0 ? LOG_INFO : LOG_ERROR, "STATUS {run} (exit {read.rc}):\n{read.out}\n")
    return read.rc == 0 ? 0 : 1
}

def private verb_down(cfg : FleetArgs; var fleet : Fleet) : int {
    if (empty(fleet.pods)) {
        to_log(LOG_INFO, "pod_fleet: no pod to delete\n")
        return 0
    }
    return drop_pods(cfg, fleet, [for (pod in fleet.pods); pod.id], "DOWN") ? 0 : 1
}

[export]
def main() : int {
    if (empty(g_env_core.log_level)) {   //! the tool's lines are its output: the to_log floor is info unless the environment set one
        set_env_variable("DAS_LOG_LEVEL", "info")
    }
    var r <- parse_args(type<FleetArgs>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<FleetArgs>), "modules/dasLLAMA/harness/pod_fleet.das")
        return 2
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.verb)) {
        print_help(get_command_info(type<FleetArgs>), "modules/dasLLAMA/harness/pod_fleet.das")
        return cfg.help ? 0 : 2
    }
    var fleet <- load_fleet(home_path(cfg.fleet))
    if (cfg.verb == "up") {
        return verb_up(cfg, fleet)
    } elif (cfg.verb == "sync") {
        return verb_sync(cfg, fleet)
    } elif (cfg.verb == "run") {
        return verb_run(cfg, fleet)
    } elif (cfg.verb == "status") {
        return verb_status(cfg, fleet)
    } elif (cfg.verb == "times") {
        return verb_times(cfg, fleet)
    } elif (cfg.verb == "down") {
        return verb_down(cfg, fleet)
    }
    to_log(LOG_ERROR, "pod_fleet: unknown verb '{cfg.verb}' (up | sync | run | status | times | down)\n")
    return 2
}
