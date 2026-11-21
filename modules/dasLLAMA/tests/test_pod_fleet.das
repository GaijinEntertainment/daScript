options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ../harness/pod_fleet.das
require _model_categories   // CUDA_PASS_ONLY: the CUDA home's pass
require daslib/strings_boost
require strings
require math   // abs: the cores' float compare

// The pod fleet's planners, model-free and pod-free: the pod the tool makes (the models volume, its data center, the
// key, ssh, the boot that installs sshd, rsync, git and the Vulkan loader), the copy that gives each pod its own tree,
// the home each pod runs (the Vulkan home's and the CUDA home's
// command, the children's logs on the volume, RUN-EXIT closing the
// log), the status read, and the home path.

def private fleet_args() : FleetArgs {
    var cfg = FleetArgs()
    cfg.verb = "up"
    cfg.pods = 2
    cfg.gpu = "NVIDIA RTX PRO 4500 Blackwell"
    cfg.volume = "vol123"
    cfg.datacenter = "EU-RO-1"
    cfg.image = "nvidia/cuda:12.8.1-devel-ubuntu24.04"
    cfg.disk = 60
    cfg.key = "~/.ssh/runpod"
    cfg.env = "DASLLAMA_MODELS_DIR=/root/models DASLLAMA_GPU=1"
    cfg.args = "--changed --base origin/master"
    cfg.runpodctl = "runpodctl"
    return <- cfg
}

[test(description = "The pod fleet's `create_argv` builds a runpodctl pod create named by index, on the models volume and its data center, with the GPU, disk, ssh port, public key, and a boot that installs sshd, rsync, git and the Vulkan loaders")]
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

[test(description = "The fleet's ssh, prep, home run, times, status and home-path planners emit the expected command text, `times_report` sums file seconds and keeps each cell's longest, and `up` numbers past the largest live pod")]
def test_fleet_commands(t : T?) {
    let cfg <- fleet_args()
    let pod = FleetPod(id = "abc", name = "dasllama-fleet-2", ip = "10.0.0.2", port = 2222, index = 2)
    let ssh <- ssh_argv(cfg, pod, "echo hi")
    t |> equal(ssh[0], "ssh", "ssh runs the command")
    t |> success(find_index(ssh, "root@10.0.0.2") >= 0 && find_index(ssh, "2222") >= 0 && find_index(ssh, "echo hi") >= 0, "on the pod's address and port: {join(ssh, " ")}")
    t |> success(find_index(ssh, "BatchMode=yes") >= 0, "never prompting")
    let prep = prep_command()
    t |> success(find(prep, "rsync -a --delete --exclude .cache --exclude build /workspace/daScript/ /root/daScript/") >= 0, "the pod's own copy of the volume's tree, its caches left out")
    t |> success(find(prep, "rm -rf /root/models && mkdir -p /root/models && cd /workspace/models") >= 0, "the pod's own models directory, made afresh: {prep}")
    t |> success(find(prep, "find . -type d -exec mkdir -p /root/models/\{\} \\;") >= 0, "the volume's folders are made again, never linked: a write under one stays on the pod")
    t |> success(find(prep, "find . ! -type d ! -name \"*.dlim\" ! -name \"*.tmp\" -exec ln -s /workspace/models/\{\} /root/models/\{\} \\;") >= 0, "a link a file of the volume's, at any depth, its images and save temp files left out")
    let at = "/workspace/logs/fleet/20261027-000000"
    let run = run_command(cfg, "20261027-000000", "cuda")
    let given = ("--changed --base origin/master --only {CUDA_PASS_ONLY} --reap-images"
        + " --stop-file {at}/STOP;")
    t |> success(find(run, given) >= 0,
        "the command as given, the runner reaping the images its children mint")
    let tracker = "sh modules/dasLLAMA/harness/pod_mem_track.sh {at}/cuda/mem_peak.txt &"
    t |> success(find(run, tracker) >= 0,
        "the memory tracker starts beside the runner, its peak under the home's folder")
    t |> success(find(run, "rc=$?; kill $(cat {at}/cuda/mem_track.pid); echo RUN-EXIT=$rc") >= 0,
        "the tracker stops when the runner ends, and RUN-EXIT carries the runner's code")
    t |> success(find(run, "'") == rfind(run, "sh -c '") + 6 && find(run, "'", find(run, "'") + 1) == rfind(run, "'"), "the detached command sits between one pair of single quotes: {run}")
    t |> success(find(run, "TMPDIR={at}/cuda ") >= 0,
        "the children's logs under the run on the volume")
    t |> success(find(run, "LD_LIBRARY_PATH=/root/daScript/lib ") >= 0, "the children load the pod's own runtime libraries, not the volume tree's")
    t |> success(find(run, "DASLLAMA_MODELS_DIR=/root/models DASLLAMA_GPU=1") >= 0, "the children's environment, the models read through the pod's own directory")
    let own = FleetArgs()
    t |> success(find(own.env, "DASLLAMA_MODELS_DIR=/root/models ") >= 0 && find(own.env, "DASLLAMA_IMAGE=0") >= 0, "the tool's own environment names the pod's models directory, the image rail off: {own.env}")
    let own_run = run_command(own, "20261027-000000", "vulkan")
    t |> success(find(own_run, "--reap-images --stop-file") >= 0,
        "... and under it the runner reaps")
    t |> success(find(own_run, "DASLLAMA_PARITY_FULL") < 0,
        "the tool's own run leaves the large tier off")
    var on_volume <- fleet_args()
    on_volume.env = "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1"
    let volume_run = run_command(on_volume, "20261027-000000", "vulkan")
    t |> success(find(volume_run, "origin/master --stop-file") >= 0
        && find(volume_run, "--reap-images") < 0,
        "a run on the volume's models directory reaps nothing: the images there are every pod's")
    t |> success(find(run, "> {at}/cuda.log") >= 0,
        "the runner's log beside them, named by its home")
    t |> success(find(run, "setsid nohup") >= 0 && find(run, "; setsid") >= 0, "detached, so the ssh call returns at once")
    let times = times_command("20261027-000000")
    t |> success(find(times, "{at}/*/dasllama_run_*.log") >= 0,
        "the times read every child log of the run")
    t |> success(find(times, "F ") >= 0 && find(times, "T ") >= 0, "a file line and a cell line each carry their mark")
    let report = times_report("F 12.5 modules/dasLLAMA/tests/test_a.das\nF 7.5 modules/dasLLAMA/tests/test_a.das\nF 5 modules/dasLLAMA/tests/test_b.das\nT 9 cell_x\nT 11 cell_x\nT 2 cell_y\nnoise\n", 1)
    t |> success(find(report, "test time 25 s over 2 file(s)") >= 0, "a file's seconds sum over the children that ran it: {report}")
    t |> success(find(report, "  20 s  80%  test_a.das") >= 0, "the top file carries its share")
    t |> success(find(report, "  11 s  cell_x") >= 0 && find(report, "cell_y") < 0, "a cell keeps its longest reading, the top list cuts at `top`")
    let status = status_command("20261027-000000")
    t |> success(find(status, "REPORT") >= 0 && find(status, "RUN-EXIT") >= 0
        && find(status, "MISSING") >= 0,
        "the status reads each home's REPORT, RUN-EXIT and MISSING lines")
    t |> success(find(status, "|WALL |LOST |STOPPED'") >= 0, "and the lines of a run that stopped short: the wall, the lost batch, the stop")
    //! a partial `down` leaves the indices sparse: `up` numbers past
    //! the largest
    let sparse <- Fleet(pods <- [FleetPod(id = "b", index = 2), FleetPod(id = "e", index = 5)])
    t |> equal(next_pod_index(sparse), 6, "the next pod's index sits past the largest live one")
    let none : Fleet
    t |> equal(next_pod_index(none), 1, "an empty fleet numbers from 1")
    t |> equal(home_path("/abs/path"), "/abs/path", "an absolute path stands")
    t |> success(!(home_path("~/.ssh/runpod") |> starts_with("~")) && home_path("~/.ssh/runpod") |> ends_with("/.ssh/runpod"), "a ~ path lands under the home")
}

[test(description = "The fleet seats a pod a GPU home: two pods run the Vulkan then the CUDA home in the fleet's order, one pod the Vulkan home with the CUDA home unseated, and a third pod is refused")]
def test_fleet_home_plan(t : T?) {
    let two <- Fleet(pods <- [FleetPod(id = "b", index = 2), FleetPod(id = "e", index = 5)])
    let plan <- home_plan(two)
    t |> equal(length(plan), 2, "a seat a home: {plan}")
    t |> success(plan[0].home == "vulkan" && plan[0].seat == 0,
        "the fleet's first pod runs the Vulkan home: {plan}")
    t |> success(plan[1].home == "cuda" && plan[1].seat == 1,
        "its second the CUDA home, whatever the pods' indices: {plan}")
    let one <- Fleet(pods <- [FleetPod(id = "e", index = 5)])
    let lone <- home_plan(one)
    t |> success(lone[0].home == "vulkan" && lone[0].seat == 0,
        "a lone pod runs the Vulkan home: {lone}")
    t |> success(lone[1].home == "cuda" && lone[1].seat == -1, "the CUDA home has no pod: {lone}")
    t |> equal(fleet_refusal(0, 2), "", "an empty fleet takes two pods")
    t |> equal(fleet_refusal(1, 1), "", "a lone pod takes the CUDA home's")
    let third = fleet_refusal(2, 1)
    t |> success(find(third, "one pod a GPU home") >= 0 && find(third, "never adds a pod") >= 0,
        "a third pod is refused with the rule: {third}")
    t |> success(!empty(fleet_refusal(0, 3)),
        "`up --pods 3`, and `run` over a fleet of three, are refused")
    t |> success(!empty(fleet_refusal(0, 0)), "`up --pods 0` is refused")
}

[test(description = "The fleet's two remote commands run the same run.das command, the CUDA home's alone under DASLLAMA_CUDA=1 and `--only` the CUDA pass, neither under --shard, `--full` on both, an `--only` of the caller's refused; the status reads the home-named logs, labelled by home")]
def test_fleet_home_commands(t : T?) {
    let cfg = FleetArgs()
    let vulkan = run_command(cfg, "20261027-000000", "vulkan")
    let cuda = run_command(cfg, "20261027-000000", "cuda")
    t |> success(find(vulkan, "DASLLAMA_CUDA") < 0,
        "the Vulkan home's command carries no CUDA ask: {vulkan}")
    t |> success(find(cuda, "DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_CUDA=1 &&") >= 0,
        "the CUDA home's command carries DASLLAMA_CUDA=1: {cuda}")
    t |> success(find(vulkan, "--shard") < 0 && find(cuda, "--shard") < 0,
        "neither home's command is a shard")
    let given = "run.das -- --changed --base origin/master "
    t |> success(find(vulkan, given) >= 0 && find(cuda, given) >= 0, "both run the command given")
    let cuda_pass = "--base origin/master --only {CUDA_PASS_ONLY} --reap-images"
    t |> success(find(cuda, cuda_pass) >= 0,
        "the CUDA home's command keeps the CUDA pass's files: {cuda}")
    t |> success(find(vulkan, "--only") < 0, "the Vulkan home's command keeps every file: {vulkan}")
    t |> equal(args_refusal(cfg.args), "", "the tool's own arguments stand")
    let narrowed = args_refusal("--changed --only gemma")
    t |> success(find(narrowed, "carries --only") >= 0 && find(narrowed, "whole per-PR set") >= 0,
        "a fleet run whose arguments carry --only is refused with the rule: {narrowed}")
    t |> success(!empty(args_refusal("--suite stocked --only=cuda")),
        "--only=<tokens> is refused too")
    t |> success(find(vulkan, "/vulkan.log 2>&1") >= 0 && find(vulkan, "echo STARTED vulkan") >= 0,
        "the Vulkan home's log named by its home")
    t |> success(find(cuda, "/cuda.log 2>&1") >= 0 && find(cuda, "echo STARTED cuda") >= 0,
        "the CUDA home's log named by its home")
    let whole = FleetArgs(full = true)
    t |> success(find(run_command(whole, "20261027-000000", "vulkan"),
        "DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1 &&") >= 0,
        "--full adds the large tier to the Vulkan home's environment")
    t |> success(find(run_command(whole, "20261027-000000", "cuda"),
        "DASLLAMA_CUDA=1 DASLLAMA_PARITY_FULL=1 &&") >= 0,
        "--full adds the large tier to the CUDA home's environment")
    let at = "/workspace/logs/fleet/20261027-000000"
    let status = status_command("20261027-000000")
    let reader = "for h in vulkan cuda; do f={at}/$h.log; [ -f $f ] || continue;"
    t |> success(find(status, reader) >= 0,
        "the status reads each home's log by its name, a home no pod ran skipped: {status}")
    t |> success(find(status, "echo \"== $h: ") >= 0, "and labels each by its home")
    t |> success(find(status, "p={at}/$h/mem_peak.txt;") >= 0 && find(status, "PEAK-KB $h: ") >= 0,
        "and each home's memory peak under its home")
    t |> success(find(status, "SHARD") < 0, "and reads no shard line")
}

[test(description = "`up`'s CPU check: `cpu_max_cores` reads a quota and period into cores, `max` as unlimited, a trailing newline allowed, garbage and a zero period as named failures; the read command cats the cgroup's cpu.max")]
def test_fleet_cpu_max(t : T?) {
    t |> equal(cpu_max_command(), "cat /sys/fs/cgroup/cpu.max", "the read command")
    let big = cpu_max_cores("2720000 100000")
    t |> success(big is cores && abs((big as cores) - 27.2) < 0.001, "27.2 cores: {big}")
    t |> equal(cpu_reading(big), "27.2 cores", "the log line's reading")
    t |> equal(cpu_reading(cpu_max_cores("1000000 300000")), "3.3 cores",
        "the reading carries one decimal")
    let small = cpu_max_cores("1020000 100000\n")
    t |> success(small is cores && abs((small as cores) - 10.2) < 0.001,
        "a trailing newline reads: {small}")
    let open = cpu_max_cores("max 100000\n")
    t |> success(open is unlimited, "the quota `max` is unlimited: {open}")
    t |> equal(cpu_reading(open), "unlimited cores", "an unlimited pod's reading")
    let junk = cpu_max_cores("garbage")
    t |> success(junk is failure && find(junk as failure, "not `<quota> <period>`") >= 0,
        "one word is no cpu.max: {junk}")
    let bad_quota = cpu_max_cores("12abc 100000")
    t |> success(bad_quota is failure && find(bad_quota as failure, "quota '12abc'") >= 0,
        "a quota with trailing garbage is named: {bad_quota}")
    let bad_period = cpu_max_cores("2720000 x")
    t |> success(bad_period is failure && find(bad_period as failure, "period 'x'") >= 0,
        "a period that is no number is named: {bad_period}")
    let zero = cpu_max_cores("2720000 0")
    t |> success(zero is failure && find(zero as failure, "period is 0") >= 0,
        "a zero period is a failure, not a division: {zero}")
    t |> success(find(cpu_reading(zero), "no allotment read") >= 0, "a failure's reading")
}

[test(description = "`up` keeps a pod at or over `--min-cores` and an unlimited one, replaces one under it or with no allotment read, keeps every pod with the check off, and fails naming the floor and each try's cores")]
def test_fleet_cpu_floor(t : T?) {
    t |> success(keeps_pod(20.0, CpuMax(cores = 20.0)), "a pod at the floor stays")
    t |> success(keeps_pod(20.0, CpuMax(cores = 27.2)), "a pod over the floor stays")
    t |> success(!keeps_pod(20.0, CpuMax(cores = 10.2)), "a pod under the floor is replaced")
    t |> success(keeps_pod(20.0, CpuMax(unlimited = true)), "an unlimited pod stays")
    t |> success(!keeps_pod(20.0, CpuMax(failure = "ssh exit 255")),
        "a pod whose allotment did not read is replaced")
    t |> success(keeps_pod(0.0, CpuMax(cores = 10.2)) && keeps_pod(0.0, CpuMax(failure = "x")),
        "`--min-cores 0` keeps every pod")
    t |> equal(FleetArgs().min_cores, 20.0, "the floor's default")
    let refused = cores_refusal(2, 20.0, ["10.2 cores", "12.0 cores", "8.5 cores"])
    t |> success(find(refused, "POD 2: all 3 tries") >= 0 && find(refused, "20.0-core floor") >= 0,
        "the failure names the pod and the floor: {refused}")
    t |> success(find(refused, "(10.2 cores, 12.0 cores, 8.5 cores)") >= 0,
        "and the cores each try got: {refused}")
}
