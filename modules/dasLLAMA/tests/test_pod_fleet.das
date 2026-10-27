options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ../harness/pod_fleet.das
require daslib/strings_boost
require strings

// The pod fleet's planners, model-free and pod-free: the pod the tool makes (the models volume, its data center, the
// key, ssh, the boot that installs sshd, rsync, git and the Vulkan loader), the copy that gives each pod its own tree,
// the shard each pod starts (its index of n, the children's logs on the volume, RUN-EXIT closing the log), the status
// read, and the home path.

def private fleet_args() : FleetArgs {
    var cfg = FleetArgs()
    cfg.verb = "up"
    cfg.pods = 3
    cfg.gpu = "NVIDIA RTX PRO 4500 Blackwell"
    cfg.volume = "vol123"
    cfg.datacenter = "EU-RO-1"
    cfg.image = "nvidia/cuda:12.8.1-devel-ubuntu24.04"
    cfg.disk = 60
    cfg.key = "~/.ssh/runpod"
    cfg.env = "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1"
    cfg.args = "--changed --base origin/master"
    cfg.runpodctl = "runpodctl"
    return <- cfg
}

[test]
def test_fleet_create_argv(t : T?) {
    let argv <- create_argv(fleet_args(), 2, "ssh-ed25519 AAAA key")
    let line = join(argv, " ")
    t |> equal(argv[0], "runpodctl", "the tool is runpodctl")
    t |> success(find(line, "pod create") >= 0, "a pod create")
    t |> success(find(line, "--name dasllama-fleet-2") >= 0, "named by its index")
    t |> success(find(line, "--network-volume-id vol123") >= 0 && find(line, "--data-center-ids EU-RO-1") >= 0, "on the models volume in its data center")
    t |> success(find(line, "--gpu-id NVIDIA RTX PRO 4500 Blackwell") >= 0, "the GPU asked")
    t |> success(find(line, "--container-disk-in-gb 60") >= 0 && find(line, "--ports 22/tcp") >= 0, "its disk and the ssh port")
    t |> success(find(line, "\"PUBLIC_KEY\":\"ssh-ed25519 AAAA key\"") >= 0, "the key in the environment the boot reads")
    t |> success(find(line, "openssh-server rsync git libvulkan1 libgl1 libegl1") >= 0 && find(line, "exec /usr/sbin/sshd -D") >= 0, "the boot installs sshd, rsync, git, the Vulkan loader and the GL and EGL loaders its NVIDIA ICD opens, then serves ssh")
    t |> success(find(line, "--wait") >= 0 && find(line, "-o json") >= 0, "waits for ssh and prints the record as JSON")
}

[test]
def test_fleet_commands(t : T?) {
    let cfg <- fleet_args()
    let pod = FleetPod(id = "abc", name = "dasllama-fleet-2", ip = "10.0.0.2", port = 2222, index = 2)
    let ssh <- ssh_argv(cfg, pod, "echo hi")
    t |> equal(ssh[0], "ssh", "ssh runs the command")
    t |> success(find_index(ssh, "root@10.0.0.2") >= 0 && find_index(ssh, "2222") >= 0 && find_index(ssh, "echo hi") >= 0, "on the pod's address and port: {join(ssh, " ")}")
    t |> success(find_index(ssh, "BatchMode=yes") >= 0, "never prompting")
    let prep = prep_command()
    t |> success(find(prep, "rsync -a --delete --exclude .cache --exclude build /workspace/daScript/ /root/daScript/") >= 0, "the pod's own copy of the volume's tree, its caches left out")
    let run = run_command(cfg, "20261027-000000", 2, 3)
    t |> success(find(run, "--changed --base origin/master --shard 2/3") >= 0, "the command with the pod's shard")
    t |> success(find(run, "TMPDIR=/workspace/logs/fleet/20261027-000000/pod_2") >= 0, "the children's logs under the run on the volume")
    t |> success(find(run, "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1") >= 0, "the children's environment")
    t |> success(find(run, "> /workspace/logs/fleet/20261027-000000/shard_2.log") >= 0, "the runner's log beside them")
    t |> success(find(run, "echo RUN-EXIT=$?") >= 0, "RUN-EXIT closes the log")
    t |> success(find(run, "setsid nohup") >= 0 && find(run, "; setsid") >= 0, "detached, so the ssh call returns at once")
    let status = status_command("20261027-000000")
    t |> success(find(status, "/workspace/logs/fleet/20261027-000000/shard_*.log") >= 0 && find(status, "REPORT") >= 0 && find(status, "RUN-EXIT") >= 0, "the status reads every shard's REPORT and RUN-EXIT")
    //! a partial `down` keeps a sparse index set: the shards still number 1..n over the live pods, and `up` numbers past the largest
    let sparse <- Fleet(pods <- [FleetPod(id = "b", index = 2), FleetPod(id = "e", index = 5)])
    let plan <- shard_plan(sparse)
    t |> equal(length(plan), 2, "a shard a live pod")
    t |> success(plan[0].pod == 2 && plan[0].shard == 1 && plan[1].pod == 5 && plan[1].shard == 2, "the shards number 1..n over the pods in order: {plan}")
    t |> equal(next_pod_index(sparse), 6, "the next pod's index sits past the largest live one")
    let none : Fleet
    t |> equal(next_pod_index(none), 1, "an empty fleet numbers from 1")
    t |> equal(home_path("/abs/path"), "/abs/path", "an absolute path stands")
    t |> success(!(home_path("~/.ssh/runpod") |> starts_with("~")) && home_path("~/.ssh/runpod") |> ends_with("/.ssh/runpod"), "a ~ path lands under the home")
}
