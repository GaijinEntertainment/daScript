options gen2
options _dasllama_internal = true

// A pod fleet for the test runner: n RunPod pods on the models volume, each holding its own copy of the
// volume's tree, each running one shard of the same run.das command. The shards' logs land on the volume
// (/workspace/logs/fleet/<run>/), so any pod of the fleet reads them all. The fleet file holds the pods this
// tool made and nothing else: `down` deletes those alone.
//
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- up --pods 3
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run                      # the per-PR run, sharded over the fleet
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- run --args "--suite model-free" --env "DASLLAMA_GPU=1"
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- status
//   daslang modules/dasLLAMA/harness/pod_fleet.das -- down

require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings
require dasllama/dasllama_env

[CommandLineArgs]
struct FleetArgs {
    @clarg_positional
    @clarg_doc = "up | sync | run | status | down"
    verb : string
    @clarg_doc = "up: pods to make"
    pods : int = 2
    @clarg_doc = "up: the GPU, as `runpodctl gpu list` names it"
    gpu : string = "NVIDIA RTX PRO 4500 Blackwell"
    @clarg_doc = "up: the network volume mounted at /workspace - the tree at /workspace/daScript, the models at /workspace/models"
    volume : string = "xl56y905w1"
    @clarg_doc = "up: the volume's data center"
    datacenter : string = "EU-RO-1"
    @clarg_doc = "up: the container image"
    image : string = "nvidia/cuda:12.8.1-devel-ubuntu24.04"
    @clarg_doc = "up: the pods' container disk in GB - the tree's copy and the children's AST records live there"
    disk : int = 60
    @clarg_doc = "the ssh private key the pods admit; its .pub is handed to every pod"
    key : string = "~/.ssh/runpod"
    @clarg_doc = "run: the children's environment - KEY=VALUE tokens separated by spaces; the image rail stays off (DASLLAMA_IMAGE=0) because the pods share one models directory, so the image suite runs on one box"
    env : string = "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1"
    @clarg_doc = "run: the run.das arguments; the tool appends each pod's --shard i/n"
    args : string = "--changed --base origin/master"
    @clarg_doc = "status: the run to read (default: the fleet file's last run)"
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
//! test_pod_fleet.das holds it
def next_pod_index(fleet : Fleet) : int {
    var top = 0
    for (pod in fleet.pods) {
        if (pod.index > top) {
            top = pod.index
        }
    }
    return top + 1
}

//! the shard each live pod runs, 1..n in the fleet's order, whatever the pods' indices (a partial `down` leaves them sparse).
//! Non-private: test_pod_fleet.das holds it
def shard_plan(fleet : Fleet) : array<tuple<pod : int; shard : int>> => <- [for (i, pod in iter_range(fleet.pods), fleet.pods); (pod = pod.index, shard = i + 1)]

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

let POD_TREE = "/root/daScript"
let VOLUME_TREE = "/workspace/daScript"
let FLEET_LOGS = "/workspace/logs/fleet"

//! a leading `~` as the user's home - USERPROFILE on Windows, else HOME, off the environment registry
def home_path(path : string) : string {
    return path if (!(path |> starts_with("~")))
    let home = !empty(g_env_core.user_profile) ? g_env_core.user_profile : g_env_core.home
    return "{home}{slice(path, 1)}"
}

//! the `runpodctl pod create` argument list for the fleet's pod `index`; `pub` is the public key line. Non-private:
//! tests/test_pod_fleet.das reads it
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
//! `.cache/` are per tree, so two pods never share one - and makes the fleet's log root
def prep_command() : string =>
    "mkdir -p {FLEET_LOGS} && rsync -a --delete --exclude .cache --exclude build {VOLUME_TREE}/ {POD_TREE}/ && echo PREP-OK"

//! the remote command that starts shard `index`/`n` of the run: the children's logs under the run's folder on
//! the volume (TMPDIR), the runner's own log beside them, RUN-EXIT=<rc> as its last line. Non-private:
//! tests/test_pod_fleet.das reads it
def run_command(cfg : FleetArgs; run : string; index, n : int) : string {
    let log = "{FLEET_LOGS}/{run}/shard_{index}.log"
    let tmp = "{FLEET_LOGS}/{run}/pod_{index}"
    let inner = ("cd {POD_TREE} && export TMPDIR={tmp} {cfg.env} && ./bin/daslang -jit -module-cache"
        + " .cache/daslang/module_cache/dastest.dascache modules/dasLLAMA/tests/run.das -- {cfg.args} --shard {index}/{n};"
        + " echo RUN-EXIT=$?")
    //! `;` not `&&` ahead of the detached command: `a && b &` backgrounds a subshell that keeps the ssh session's
    //! output open, and the ssh call then lasts as long as the run
    return "mkdir -p {tmp}; setsid nohup sh -c '{inner}' > {log} 2>&1 < /dev/null & echo STARTED {index}/{n}"
}

//! the remote command that reads every shard of `run` off the volume: its DONE count, its SHARD, REPORT and NEXT
//! lines, its RUN-EXIT line, and every red DONE line
def status_command(run : string) : string =>
    ("for f in {FLEET_LOGS}/{run}/shard_*.log; do echo \"== $f: $(grep -cE 'DONE' $f) done\";"
        + " grep -E '^\\[I\\] (SHARD|REPORT|NEXT)|RUN-EXIT' $f; grep -E 'DONE' $f | grep -vE ' 0 failed, 0 errors'; done; true")

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

//! `up`: `--pods` more pods join the fleet, their indices after the pods already in it, each copying the tree once made
def private verb_up(cfg : FleetArgs; var fleet : Fleet) : int {
    let pub = strip(file_text("{home_path(cfg.key)}.pub"))
    if (empty(pub)) {
        to_log(LOG_ERROR, "pod_fleet: no public key at {home_path(cfg.key)}.pub\n")
        return 2
    }
    let first = length(fleet.pods)
    let first_index = next_pod_index(fleet)
    let want = first_index + cfg.pods - 1
    for (index in range(first_index, want + 1)) {
        to_log(LOG_INFO, "POD {index}/{want}: creating on {cfg.gpu} in {cfg.datacenter}\n")
        let made = run_capture(create_argv(cfg, index, pub), 900.0)
        var rec : PodRecord
        let at = find(made.out, "\{")
        if (made.rc != 0 || at < 0 || !sscan_json(slice(made.out, at), rec) || empty(rec.id) || empty(rec.ssh.ip)) {
            to_log(LOG_ERROR, "POD {index}/{want}: not made (exit {made.rc}):\n{made.out}\n")
            break
        }
        fleet.pods |> push(FleetPod(id = rec.id, name = rec.name, ip = rec.ssh.ip, port = rec.ssh.port, index = index))
        save_fleet(home_path(cfg.fleet), fleet)
        to_log(LOG_INFO, "POD {index}/{want}: {rec.id} at {rec.ssh.ip}:{rec.ssh.port}\n")
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
    to_log(LOG_INFO, "FLEET {length(fleet.pods)} pod(s) ready; `run` shards a command over them\n")
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
    let stamp = run_capture(ssh_argv(cfg, fleet.pods[0], "date -u +%Y%m%d-%H%M%S"), 60.0)
    let run = strip(stamp.out)
    if (stamp.rc != 0 || empty(run)) {
        to_log(LOG_ERROR, "pod_fleet: pod {fleet.pods[0].index} is not reachable (exit {stamp.rc}):\n{stamp.out}\n")
        return 1
    }
    let n = length(fleet.pods)
    fleet.last_run = run   //! saved before the starts, so `status` reads a run whose later shard failed to start
    save_fleet(home_path(cfg.fleet), fleet)
    for (pod, seat in fleet.pods, shard_plan(fleet)) {
        let started = run_capture(ssh_argv(cfg, pod, run_command(cfg, run, seat.shard, n)), 60.0)
        if (started.rc != 0 || find(started.out, "STARTED") < 0) {
            to_log(LOG_ERROR, "RUN {run} shard {seat.shard}/{n}: not started on pod {pod.index} (exit {started.rc}):\n{started.out}\n")
            return 1
        }
        to_log(LOG_INFO, "RUN {run} shard {seat.shard}/{n} started on pod {pod.index} ({pod.id}): run.das -- {cfg.args} --shard {seat.shard}/{n}\n")
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
        let gone = run_capture([cfg.runpodctl, "pod", "delete", pod.id], 120.0)
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
    } elif (cfg.verb == "down") {
        return verb_down(cfg, fleet)
    }
    to_log(LOG_ERROR, "pod_fleet: unknown verb '{cfg.verb}' (up | sync | run | status | down)\n")
    return 2
}
