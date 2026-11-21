options gen2
options _dasllama_internal = true

// A pod fleet for the test runner: one RunPod pod a GPU home on the
// models volume, each with its own copy of the volume's tree and its
// own folder of links to the volume's models (an image a cell mints
// lands on the pod's disk, and the runner reaps it after the file).
// The first pod runs the whole run.das command on the Vulkan home,
// the second the same command on the CUDA home, kept to the CUDA
// pass's files (`--only` CUDA_PASS_ONLY). No pod runs a part of a
// home's run: a home's run that outgrows its pod cuts tests, never
// adds a pod. The homes' logs land on the volume
// (/workspace/logs/fleet/<run>/), so any pod reads them all. The
// fleet file holds the pods this tool made and nothing else: `down`
// deletes those alone.
//
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- up
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run  # per PR
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run --full               # an arc's final run: the large tier too
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
require ../tests/_model_categories.das   // CUDA_PASS_ONLY: the CUDA home's pass
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
    @clarg_doc = "up: the pods' container disk in GB - the tree's copy and the children's AST records live there"
    disk : int = 60
    @clarg_doc = "up: the fewest cores a pod's cgroup quota may give it, else remade; 0 = off"
    min_cores : float = 20.0
    @clarg_doc = "the ssh private key the pods admit; its .pub is handed to every pod"
    key : string = "~/.ssh/runpod"
    @clarg_doc = "run: the children's environment - KEY=VALUE tokens separated by spaces; the models directory is the pod's own folder of links to the volume's models, the image rail is off (DASLLAMA_IMAGE=0) and so is the large tier; an image suite's run passes its own --env with the rail on"
    env : string = "DASLLAMA_MODELS_DIR=/root/models DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1"
    @clarg_doc = "run: the full suite - the children also take DASLLAMA_PARITY_FULL=1, so the models past the size tier run and no test wall stands; an arc's final run"
    full : bool
    @clarg_doc = "run: the run.das arguments, no --only; the CUDA pod adds the CUDA pass's"
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
//! the pod-fleet tests (tests/uncategorized_persistent_heap_stack_524288.das) hold it
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
//! tests (tests/uncategorized_persistent_heap_stack_524288.das) hold
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
//! (tests/uncategorized_persistent_heap_stack_524288.das) hold it
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
//! no pod. Non-private: the pod-fleet tests
//! (tests/uncategorized_persistent_heap_stack_524288.das) hold it
def home_plan(fleet : Fleet) : array<tuple<home : string; seat : int>> {
    let homes <- fleet_homes()
    let n = length(fleet.pods)
    return <- [for (i, home in iter_range(homes), homes); (home = home, seat = i < n ? i : -1)]
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

//! the pods `up` makes at one index before an allotment under
//! the floor fails it
let CPU_TRIES = 3

//! a pod's CPU allotment as its cgroup's `cpu.max` reads: the
//! cores its quota buys a period, unlimited (the quota `max`),
//! or why the text is no `cpu.max`
variant CpuMax {
    cores : float
    unlimited : bool
    failure : string
}

//! the remote command that reads the pod's cgroup CPU quota.
//! Non-private: the pod-fleet tests read it
def cpu_max_command() : string => "cat /sys/fs/cgroup/cpu.max"

//! a `cpu.max` text (`<quota> <period>`, a trailing newline
//! allowed) as the pod's allotment. Non-private: the pod-fleet
//! tests read it
def cpu_max_cores(text : string) : CpuMax {
    let line = strip(text)
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

//! whether `up` keeps a new pod: every pod with the check off
//! (`min_cores` 0), an unlimited one, one at the floor or over
//! it; an unread allotment is replaced. Non-private: the
//! pod-fleet tests read it
def keeps_pod(min_cores : float; cpu : CpuMax) : bool {
    return true if (min_cores <= 0.0 || cpu is unlimited)
    return cpu is cores && (cpu as cores) >= min_cores
}

//! the allotment as `up`'s log line states it
def cpu_reading(cpu : CpuMax) : string {
    return "{fmt(":.1f", cpu as cores)} cores" if (cpu is cores)
    return "unlimited cores" if (cpu is unlimited)
    return "no allotment read ({cpu as failure})"
}

//! the failure of `up` when every try at pod `index` came in
//! under the floor: the floor and each try's cores.
//! Non-private: the pod-fleet tests read it
def cores_refusal(index : int; min_cores : float; readings : array<string>) : string {
    let floor = fmt(":.1f", min_cores)
    return ("pod_fleet: POD {index}: all {length(readings)} tries came in under the {floor}-core"
        + " floor ({join(readings, ", ")}); `--min-cores 0` keeps such a pod")
}

let POD_TREE = "/root/daScript"
let VOLUME_TREE = "/workspace/daScript"
let FLEET_LOGS = "/workspace/logs/fleet"
let VOLUME_MODELS = "/workspace/models"
let POD_MODELS = "/root/models"   //! the pod's own models directory: a link a model file of the volume, so what a run writes beside a model stays on the pod's disk

//! a leading `~` as the user's home - USERPROFILE on Windows, else HOME, off the environment registry
def home_path(path : string) : string {
    return path if (!(path |> starts_with("~")))
    let home = !empty(g_env_core.user_profile) ? g_env_core.user_profile : g_env_core.home
    return "{home}{slice(path, 1)}"
}

//! the `runpodctl pod create` argument list for the fleet's pod `index`; `pub` is the public key line. Non-private:
//! the pod-fleet tests (tests/uncategorized_persistent_heap_stack_524288.das) read it
def create_argv(cfg : FleetArgs; index : int; pub : string) : array<string> {
    //! the container's command: sshd admitting the key, rsync for the tree's copy, git for --changed, the Vulkan
    //! loader for the Vulkan home with the GL and EGL loaders the NVIDIA ICD (`libGLX_nvidia`, which the container
    //! runtime brings) opens, and the libraries the tree's LLVM shared library links
    let boot = ("bash -c \"apt-get update && apt-get install -y --no-install-recommends openssh-server rsync git libvulkan1 libgl1 libegl1 libz3-4 libxml2"
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
//! `.cache/` are per tree, so two pods never share one - makes the fleet's log root, and makes the pod's models
//! directory afresh: the volume's folders made again and a link to every file of the volume's but its prepared images
//! and save temp files, so a pod reads and writes no image another pod does, in a subfolder as at the top
def prep_command() : string =>
    ("mkdir -p {FLEET_LOGS} && rsync -a --delete --exclude .cache --exclude build {VOLUME_TREE}/ {POD_TREE}/"
        + " && rm -rf {POD_MODELS} && mkdir -p {POD_MODELS} && cd {VOLUME_MODELS}"
        + " && find . -type d -exec mkdir -p {POD_MODELS}/\{\} \\;"
        + " && find . ! -type d ! -name \"*.dlim\" ! -name \"*.tmp\" -exec ln -s {VOLUME_MODELS}/\{\} {POD_MODELS}/\{\} \\;"
        + " && echo PREP-OK")

//! the remote command that starts the run on `home`, the CUDA home
//! under DASLLAMA_CUDA=1 and kept to its pass's files (`--only`
//! CUDA_PASS_ONLY): the children's logs in the home's folder of
//! the run on the volume (TMPDIR), the runner's own log `<home>.log`
//! beside it, RUN-EXIT=<rc> as its last line; the runner reaps the
//! images its children mint where the environment names the pod's own
//! models directory (`--reap-images`: under any other directory the
//! mints are another pod's too, and stay), both homes name the run's
//! one stop file (`--stop-file`: a test past its wall on one pod
//! starts no later child on any), and the memory tracker beside it
//! keeps the largest child's peak in the folder's `mem_peak.txt`.
//! LD_LIBRARY_PATH puts the pod's copy of the runtime libraries ahead
//! of the binary's RUNPATH, which names the volume tree's - a rebuild
//! there mid-run would otherwise kill every child started after it.
//! Non-private: the pod-fleet tests
//! (tests/uncategorized_persistent_heap_stack_524288.das) read it
def run_command(cfg : FleetArgs; run, home : string) : string {
    let log = "{FLEET_LOGS}/{run}/{home}.log"
    let tmp = "{FLEET_LOGS}/{run}/{home}"
    let reap = find(" {cfg.env} ", " DASLLAMA_MODELS_DIR={POD_MODELS} ") >= 0 ? " --reap-images" : ""
    let cuda = home == "cuda" ? " DASLLAMA_CUDA=1" : ""
    let only = home == "cuda" ? " --only {CUDA_PASS_ONLY}" : ""
    let full = cfg.full ? " DASLLAMA_PARITY_FULL=1" : ""
    let inner = ("cd {POD_TREE} && export LD_LIBRARY_PATH={POD_TREE}/lib TMPDIR={tmp}"
        + " {cfg.env}{cuda}{full}"
        + " && (sh modules/dasLLAMA/harness/pod_mem_track.sh {tmp}/mem_peak.txt & echo $! > {tmp}/mem_track.pid)"
        + " && ./bin/daslang -jit -module-cache .cache/daslang/module_cache/dastest.dascache modules/dasLLAMA/tests/run.das --"
        + " {cfg.args}{only}{reap} --stop-file {FLEET_LOGS}/{run}/STOP;"
        + " rc=$?; kill $(cat {tmp}/mem_track.pid); echo RUN-EXIT=$rc")
    //! `;` not `&&` ahead of the detached command: `a && b &` backgrounds a subshell that keeps the ssh session's
    //! output open, and the ssh call then lasts as long as the run
    let detached = "setsid nohup sh -c '{inner}' > {log} 2>&1 < /dev/null &"
    return "mkdir -p {tmp}; {detached} echo STARTED {home}"
}

//! the remote command that reads each home's log of `run` off the
//! volume, under the home's name: its DONE count, REPORT and NEXT
//! lines, MISSING lines (a model file the home's cells ask that the
//! volume lacks), RUN-EXIT line and every red DONE line; then each
//! home's peak - the largest child's resident kB and the file it ran.
//! A home no pod ran has no log and prints nothing
def status_command(run : string) : string {
    let homes = join(fleet_homes(), " ")
    let at = "{FLEET_LOGS}/{run}"
    return ("for h in {homes}; do f={at}/$h.log; [ -f $f ] || continue;"
        + " echo \"== $h: $(grep -cE 'DONE' $f) done\";"
        + " grep -E '^\\[I\\] (REPORT|NEXT)|^\\[W\\] MISSING|RUN-EXIT|WALL |LOST |STOPPED' $f;"
        + " grep -E 'DONE' $f | grep -vE ' 0 failed, 0 errors'; done;"
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

def private file_text(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private load_fleet(path : string) : Fleet {
    var fleet : Fleet
    let text = file_text(path)
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

//! pod `index` of `up`, made again while its cgroup allotment
//! comes in under `--min-cores`, CPU_TRIES pods in all: 0 when
//! one is kept, 1 when none is made (the pods before it stand),
//! 2 when every try came in short or a short pod outlived its
//! delete (the fleet file keeps it for `down`)
def private make_pod(cfg : FleetArgs; var fleet : Fleet; index, want : int; pub : string) : int {
    var readings : array<string>
    readings |> reserve(CPU_TRIES)
    fleet.pods |> reserve(length(fleet.pods) + 1)
    for (attempt in range(CPU_TRIES)) {
        let tried = "POD {index}/{want} (try {attempt + 1}/{CPU_TRIES})"
        to_log(LOG_INFO, "{tried}: creating on {cfg.gpu} in {cfg.datacenter}\n")
        let made = run_capture(create_argv(cfg, index, pub), 900.0)
        var rec : PodRecord
        let at = find(made.out, "\{")
        let read_rec = at >= 0 && sscan_json(slice(made.out, at), rec)
        if (made.rc != 0 || !read_rec || empty(rec.id) || empty(rec.ssh.ip)) {
            to_log(LOG_ERROR, "{tried}: not made (exit {made.rc}):\n{made.out}\n")
            return 1
        }
        let pod = FleetPod(id = rec.id, name = rec.name, ip = rec.ssh.ip, port = rec.ssh.port,
            index = index)
        fleet.pods |> push(pod)
        save_fleet(home_path(cfg.fleet), fleet)
        to_log(LOG_INFO, "{tried}: {rec.id} at {rec.ssh.ip}:{rec.ssh.port}\n")
        let read = run_capture(ssh_argv(cfg, pod, cpu_max_command()), 60.0)
        let cpu = read.rc == 0 ? cpu_max_cores(read.out) : CpuMax(failure = "ssh exit {read.rc}")
        if (keeps_pod(cfg.min_cores, cpu)) {
            to_log(LOG_INFO, "POD {index} {rec.id}: {cpu_reading(cpu)}\n")
            return 0
        }
        readings |> push(cpu_reading(cpu))
        let short = "POD {index} {rec.id}: {cpu_reading(cpu)}"
        to_log(LOG_INFO, "{short}, under the floor - replacing it\n")
        let gone = delete_pod(cfg, rec.id)
        if (gone.rc != 0) {
            to_log(LOG_ERROR, "POD {index} {rec.id}: not deleted (exit {gone.rc}):\n{gone.out}\n")
            return 2
        }
        fleet.pods |> pop()
        save_fleet(home_path(cfg.fleet), fleet)
    }
    to_log(LOG_ERROR, "{cores_refusal(index, cfg.min_cores, readings)}\n")
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
    let pub = strip(file_text("{home_path(cfg.key)}.pub"))
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
        let started = run_capture(ssh_argv(cfg, pod, run_command(cfg, run, seat.home)), 60.0)
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
    var kept : array<FleetPod>
    for (pod in fleet.pods) {
        let gone = delete_pod(cfg, pod.id)
        if (gone.rc != 0) {
            to_log(LOG_ERROR, "DOWN {pod.index}: {pod.id} not deleted (exit {gone.rc}):\n{gone.out}\n")
            kept |> push(pod)
        } else {
            to_log(LOG_INFO, "DOWN {pod.index}: {pod.id} deleted\n")
        }
    }
    fleet.pods <- kept
    save_fleet(home_path(cfg.fleet), fleet)
    return empty(fleet.pods) ? 0 : 1
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
