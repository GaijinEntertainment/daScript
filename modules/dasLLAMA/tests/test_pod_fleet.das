options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ../harness/pod_fleet.das
require _model_categories   // CUDA_PASS_ONLY: the CUDA home's pass
require _child_cuda_cache   // CudaWarm: the prewarm step's line and exit code
require daslib/strings_boost
require daslib/fio
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
    cfg.disk = FleetArgs().disk
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
    t |> equal(FleetArgs().disk, 200, "the disk's default holds the tree and the copies of the models a run opens")
    for (token in ["DASLLAMA_WHISPER_DIR=/root/models ", "DASLLAMA_CORPUS_DIR=/root/models/asr_corpus ", "DASLLAMA_SCRATCH_DIR=/root/scratch "]) {
        t |> success(find(FleetArgs().env, token) >= 0, "the children's default environment names {token}: {FleetArgs().env}")
    }
    t |> success(find(line, "--container-disk-in-gb 200") >= 0 && find(line, "--ports 22/tcp") >= 0, "its disk and the ssh port")
    t |> success(find(line, "\"PUBLIC_KEY\":\"ssh-ed25519 AAAA key\"") >= 0, "the key in the environment the boot reads")
    t |> success(find(line, "openssh-server rsync git curl ca-certificates libvulkan1 libgl1 libegl1") >= 0 && find(line, "exec /usr/sbin/sshd -D") >= 0, "the boot installs sshd, rsync, git, curl for the model fetch, the Vulkan loader and the GL and EGL loaders its NVIDIA ICD opens, then serves ssh")
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
    t |> success(find(prep, "rm -rf") < 0 && find(prep, "mkdir -p /root/models && find /root/models -type l -delete") >= 0, "the pod's own models directory keeps the copies a run made, its links removed: {prep}")
    t |> success(find(prep, "find /root/models -type f \\( -name \"*.dlim\" -o -name \"*.tmp\" \\) -delete && cd /workspace/models") >= 0, "the images and save temp files a run left on the pod go")
    t |> success(find(prep, "find . -type d -exec mkdir -p /root/models/\{\} \\;") >= 0, "the volume's folders are made again, never linked: a write under one stays on the pod")
    t |> success(find(prep, "find . ! -type d ! -name \"*.dlim\" ! -name \"*.tmp\" -exec sh -c '[ -e \"$1/$0\" ] || ln -s \"$2/$0\" \"$1/$0\"' \{\} /root/models /workspace/models \\;") >= 0, "a link a file of the volume's where no copy stands, at any depth, its images and save temp files left out: {prep}")
    let at = "/workspace/logs/fleet/20261027-000000"
    let run = run_command(cfg, "20261027-000000", "cuda", true)
    let given = ("--changed --base origin/master --only {CUDA_PASS_ONLY} --batch 30 --reap-images"
        + " --await-models {at}/cuda/copy --stop-file {at}/STOP;")
    t |> success(find(run, given) >= 0,
        "the command as given, the runner reaping the images its children mint")
    let tracker = "sh modules/dasLLAMA/harness/pod_mem_track.sh {at}/cuda/mem_peak.txt /root/models &"
    t |> success(find(run, tracker) >= 0,
        "the memory tracker starts beside the runner, its peak under the home's folder, the model copies it trims from the page cache its second argument")
    t |> success(find(run, "rc=$?; \}; rc=$\{rc:-$?\}; kill $cp 2>/dev/null; kill $(cat {at}/cuda/mem_track.pid);") >= 0
        && rfind(run, "echo RUN-EXIT=$rc'") > find(run, "kill $(cat {at}/cuda/mem_track.pid)"),
        "the copier and the tracker stop when the runs end, and RUN-EXIT is the last line: {run}")
    t |> success(find(run, "'") == rfind(run, "sh -c '") + 6 && find(run, "'", find(run, "'") + 1) == rfind(run, "'"), "the detached command sits between one pair of single quotes: {run}")
    t |> success(find(run, "TMPDIR={at}/cuda ") >= 0,
        "the children's logs under the run on the volume")
    t |> success(find(run, "LD_LIBRARY_PATH=/root/daScript/lib ") >= 0, "the children load the pod's own runtime libraries, not the volume tree's")
    t |> success(find(run, "DASLLAMA_MODELS_DIR=/root/models DASLLAMA_GPU=1") >= 0, "the children's environment, the models read through the pod's own directory")
    let own = FleetArgs()
    t |> success(find(own.env, "DASLLAMA_MODELS_DIR=/root/models ") >= 0 && find(own.env, "DASLLAMA_IMAGE=0") >= 0, "the tool's own environment names the pod's models directory, the image rail off: {own.env}")
    let own_run = run_command(own, "20261027-000000", "vulkan", false)
    t |> success(find(own_run, "--reap-images --await-models") >= 0,
        "... and under it the runner reaps")
    t |> success(find(own_run, "DASLLAMA_PARITY_FULL") < 0,
        "the tool's own run keeps the size tier")
    var on_volume <- fleet_args()
    on_volume.env = "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1"
    let volume_run = run_command(on_volume, "20261027-000000", "vulkan", false)
    t |> success(find(volume_run, "origin/master --batch 30 --exclude {CUDA_ONLY_EXCLUDE} --await-models") >= 0
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

[test(description = "The fleet's two remote commands run the same run.das command thirty files a child unless the caller names a --batch, the model-free suite on its own batch, the CUDA home's alone under DASLLAMA_CUDA=1 and `--only` the CUDA pass - the media towers' files among it -, neither under --shard, `--full` on both, an `--only` of the caller's refused; the status reads the home-named logs, labelled by home")]
def test_fleet_home_commands(t : T?) {
    let cfg = FleetArgs()
    let vulkan = run_command(cfg, "20261027-000000", "vulkan", false)
    let cuda = run_command(cfg, "20261027-000000", "cuda", true)
    t |> success(find(vulkan, "DASLLAMA_CUDA") < 0,
        "the Vulkan home's command carries no CUDA ask: {vulkan}")
    t |> success(find(cuda, "DASLLAMA_CUDA=1 ./bin/daslang -jit -module-cache .cache/daslang/module_cache/dastest.dascache modules/dasLLAMA/tests/run.das -- --changed") >= 0,
        "the CUDA home's given command runs under DASLLAMA_CUDA=1: {cuda}")
    t |> success(find(vulkan, "--shard") < 0 && find(cuda, "--shard") < 0,
        "neither home's command is a shard")
    let given = "run.das -- --changed --base origin/master "
    t |> success(find(vulkan, given) >= 0 && find(cuda, given) >= 0, "both run the command given")
    let cuda_pass = "--base origin/master --only {CUDA_PASS_ONLY} --batch 30 --reap-images"
    t |> success(find(cuda, cuda_pass) >= 0,
        "the CUDA home's command keeps the CUDA pass's files: {cuda}")
    let tokens <- split(CUDA_PASS_ONLY, ",")
    for (file in fixed_array("test_whisper.das", "test_audio.das", "test_gigaam.das", "test_vad.das",
            "test_gemma3v.das", "test_gemma4uv.das", "test_gemma4v.das", "test_qwen3v.das", "test_qwen25v.das")) {
        var kept = false
        for (tok in tokens) {
            kept = find(file, tok) >= 0
            break if (kept)
        }
        t |> success(kept, "the CUDA pass keeps a media tower's file, whose twin seats on the CUDA tower: {file}")
    }
    let every_file = "run.das -- --changed --base origin/master --batch 30 "
    t |> success(find(vulkan, " --only {CUDA_PASS_ONLY}") < 0 && find(vulkan, every_file) >= 0,
        "the Vulkan home's given command keeps every file: {vulkan}")
    let batch = "--base origin/master --batch 30 --exclude {CUDA_ONLY_EXCLUDE} --reap-images"
    t |> success(find(vulkan, batch) >= 0 && find(cuda, "{CUDA_PASS_ONLY} --batch 30 --reap-images") >= 0,
        "both homes run thirty files a child: {vulkan}")
    t |> success(find(vulkan, "--batch 30 --exclude {CUDA_ONLY_EXCLUDE} --list-models") >= 0,
        "and list the models of that command")
    var own_batch = FleetArgs()
    own_batch.args = "--changed --batch 12"
    let caller = run_command(own_batch, "20261027-000000", "vulkan", false)
    t |> success(find(caller, "--changed --batch 12 --exclude {CUDA_ONLY_EXCLUDE} --reap-images") >= 0
            && find(caller, "--batch 30") < 0,
        "the caller's --batch wins, one --batch on the line: {caller}")
    t |> equal(batched_args("--changed --batch=12"), "--changed --batch=12", "--batch=<n> is the caller's too")
    t |> equal(batched_args("--suite model-free"), "--suite model-free",
        "the model-free suite keeps its own whole-suite batch")
    t |> equal(batched_args("--changed"), "--changed --batch {FLEET_BATCH}", "the fleet's batch is the one constant")
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
    t |> success(find(run_command(whole, "20261027-000000", "vulkan", false),
        "DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1 &&") >= 0,
        "--full lifts the size tier in the Vulkan home's environment")
    t |> success(find(run_command(whole, "20261027-000000", "cuda", true),
        "DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1 &&") >= 0,
        "--full lifts the size tier in the CUDA home's environment")
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
    let warned = cpu_max_cores("Warning: Permanently added '[10.0.0.1]:22' (ED25519) to the list of known hosts.\r\n2380000 100000\n")
    t |> success(warned is cores && abs((warned as cores) - 23.8) < 0.001,
        "the ssh call's warning ahead of the line does not hide it: {warned}")
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
    let refused = floor_refusal(2, 20.0, 1.0, ["10.2 cores, 2.0 GB/s of fresh memory", "27.2 cores, 0.3 GB/s of fresh memory"])
    t |> success(find(refused, "POD 2: all 2 tries") >= 0 && find(refused, "the 20.0-core floor or the 1.0 GB/s memory floor") >= 0,
        "the failure names the pod and both floors: {refused}")
    t |> success(find(refused, "(10.2 cores, 2.0 GB/s of fresh memory; 27.2 cores, 0.3 GB/s of fresh memory)") >= 0,
        "and what each try read: {refused}")
    t |> success(find(refused, "`--min-cores 0` and `--min-gbs 0` keep such a pod") >= 0, "and the two ways to keep such a pod: {refused}")
    let cores_only = floor_refusal(1, 20.0, 0.0, ["10.2 cores"])
    t |> success(find(cores_only, "under the 20.0-core floor (10.2 cores); `--min-cores 0` keeps such a pod") >= 0,
        "a floor that is off is not named: {cores_only}")
    let memory_only = floor_refusal(1, 0.0, 1.0, ["27.2 cores, 0.3 GB/s of fresh memory"])
    t |> success(find(memory_only, "under the 1.0 GB/s memory floor (") >= 0 && find(memory_only, "core floor") < 0,
        "nor is the core floor where it is off: {memory_only}")
}

[test(description = "`up`'s memory read: `memory_rate` turns the last line of dd's output into GB a second and any other text into a failure naming it, `keeps_memory` keeps a pod at or over the floor and every pod with the check off and replaces a slower or unread one, `pod_reading` keeps a pod only over both floors and names what it read, the floor defaults to 1 GB/s, and a run's children take DAS_NO_HUGEPAGE_HINT")]
def test_fleet_memory_floor(t : T?) {
    t |> equal(memory_rate_command(), "LC_ALL=C dd if=/dev/zero of=/dev/null bs=2000M count=1 2>&1 | tail -1", "the read command")
    let healthy = memory_rate("2097152000 bytes (2.1 GB, 2.0 GiB) copied, 1.04279 s, 2.0 GB/s\n")
    t |> success(healthy is gbs && (healthy as gbs) > 2.0 && (healthy as gbs) < 2.02, "two gigabytes in a second and a bit: {healthy}")
    let warned = memory_rate("Warning: Permanently added '[1.2.3.4]:22' (ED25519) to the list of known hosts.\n"
        + "2097152000 bytes (2.1 GB, 2.0 GiB) copied, 1.04279 s, 2.0 GB/s\n\n")
    t |> success(warned is gbs && (warned as gbs) == (healthy as gbs), "the ssh call's own warning ahead of dd's line changes nothing: {warned}")
    let slow = memory_rate("2097152000 bytes (2.1 GB, 2.0 GiB) copied, 7.5 s, 280 MB/s")
    t |> success(slow is gbs && (slow as gbs) > 0.27 && (slow as gbs) < 0.29, "the rate is the bytes over the seconds, whatever unit dd prints: {slow}")
    for (text in ["", "dd: failed to open", "2097152000 bytes copied", "x bytes copied, 1.0 s", "5 bytes copied, 0 s, Infinity B/s",
            "5 bytes copied,", "5 bytes copied, x s", "5 bytes copied, 1,04 s"]) {
        let unread = memory_rate(text)
        t |> success(unread is failure, "no rate out of `{text}`")
    }
    let named = memory_rate("sh: 1: dd: not found")
    t |> success(named is failure && find(named as failure, "'sh: 1: dd: not found'") >= 0, "a failure carries the line it could not read: {named}")
    t |> success(keeps_memory(1.0, MemoryRate(gbs = 1.0)) && keeps_memory(1.0, MemoryRate(gbs = 2.0)), "a pod at or over the floor stays")
    t |> success(!keeps_memory(1.0, MemoryRate(gbs = 0.3)), "a pod under the floor is replaced")
    t |> success(!keeps_memory(1.0, MemoryRate(failure = "ssh exit 255")), "an unread rate is replaced")
    t |> success(keeps_memory(0.0, MemoryRate(gbs = 0.3)) && keeps_memory(0.0, MemoryRate(failure = "x")), "`--min-gbs 0` keeps every pod")
    t |> equal(FleetArgs().min_gbs, 1.0, "the floor's default")
    t |> equal(memory_reading(MemoryRate(gbs = 2.011)), "2.0 GB/s of fresh memory", "the log line's reading")
    t |> equal(memory_reading(MemoryRate(failure = "ssh exit 255")), "no memory rate read (ssh exit 255)", "an unread rate's reading says why")
    let dd_line = "2097152000 bytes (2.1 GB, 2.0 GiB) copied, 1.04279 s, 2.0 GB/s"
    let good = pod_reading(20.0, 1.0, CpuMax(cores = 27.2), 0, dd_line)
    t |> success(good.kept, "a pod over both floors stays")
    t |> equal(good.text, "27.2 cores, 2.0 GB/s of fresh memory", "its log line carries both readings")
    let starved = pod_reading(20.0, 1.0, CpuMax(cores = 27.2), 0, "2097152000 bytes copied, 7.5 s, 280 MB/s")
    t |> success(!starved.kept, "a pod with the cores and slow memory is replaced: {starved.text}")
    let short = pod_reading(20.0, 1.0, CpuMax(cores = 10.2), 0, dd_line)
    t |> success(!short.kept, "a pod with the memory and too few cores is replaced: {short.text}")
    let lost = pod_reading(20.0, 1.0, CpuMax(cores = 27.2), 255, dd_line)
    t |> success(!lost.kept, "a memory read that failed is an unread rate, whatever it printed")
    t |> equal(lost.text, "27.2 cores, no memory rate read (ssh exit 255)", "and its reading says so")
    let unasked = pod_reading(20.0, 0.0, CpuMax(cores = 27.2), 255, "")
    t |> success(unasked.kept, "with the memory floor off the cores alone decide")
    t |> equal(unasked.text, "27.2 cores", "and the log line names no memory")
    let run = run_command(FleetArgs(), "20261027-000000", "vulkan", true)
    t |> success(find(run, " DAS_NO_HUGEPAGE_HINT=1 ") >= 0, "the children allocate with no huge-page hint: {run}")
}

[test(description = "A home's run lists the models with the given command's own arguments, starts the copier on the list ahead of both runs, runs the model-free suite on the home the plan gives it without the CUDA ask or the stop file and then the given command, both under --await-models; a failed list or copy and a red model-free suite reach RUN-EXIT, and an --args that is the model-free suite runs it once")]
def test_fleet_model_copy(t : T?) {
    let cfg = FleetArgs()
    let state = "/workspace/logs/fleet/20261027-000000/cuda/copy"
    let cuda = run_command(cfg, "20261027-000000", "cuda", true)
    let given = "run.das -- --changed --base origin/master --only {CUDA_PASS_ONLY} --batch 30"
    let list = "DASLLAMA_CUDA=1 ./bin/daslang -jit modules/dasLLAMA/tests/{given} --list-models > {state}/models.txt"
    let copier = "|| (while IFS= read -r p; do"
    let model_free = "/run.das -- --suite model-free --await-models {state}; mf=$?;"
    let main = "DASLLAMA_CUDA=1 ./bin/daslang -jit -module-cache .cache/daslang/module_cache/dastest.dascache modules/dasLLAMA/tests/{given} --reap-images --await-models {state} --stop-file"
    t |> success(find(cuda, list) >= 0, "the list is the given command's, its arguments and CUDA ask, and no cache flag whose line would join it: {cuda}")
    t |> success(find(cuda, list) < find(cuda, copier) && find(cuda, copier) < find(cuda, model_free)
        && find(cuda, model_free) < find(cuda, main), "the list, then the copier, then the model-free suite, then the given command: {cuda}")
    t |> success(find(cuda, ") > {state}/copy.log 2>&1 & cp=$!;") >= 0, "the copier runs in the background, its own log beside its markers")
    t |> success(find(cuda, "[ -f {state}/failed ] {copier}") >= 0, "no copy starts off a list that failed")
    t |> success(find(cuda, "|| echo \"the model list: exit $?\" > {state}/failed;") >= 0, "a failed list is the copy's failure")
    let mf_at = find(cuda, model_free)
    t |> success(find(slice(cuda, mf_at - 120, mf_at), "DASLLAMA_CUDA") < 0 && find(slice(cuda, mf_at, find(cuda, "mf=$?")), "--stop-file") < 0,
        "the model-free suite runs without the CUDA ask and without the stop file: {cuda}")
    let vulkan = run_command(cfg, "20261027-000000", "vulkan", true)
    t |> success(find(vulkan, "/run.das -- --suite model-free --await-models /workspace/logs/fleet/20261027-000000/vulkan/copy; mf=$?;") >= 0
        && find(vulkan, "--base origin/master --batch 30 --reap-images --await-models /workspace/logs/fleet/20261027-000000/vulkan/copy --stop-file") >= 0,
        "a lone pod's Vulkan home runs the model-free suite, and its given command under the wait: {vulkan}")
    let red_copy = "[ -f {state}/failed ] && echo \"COPY-FAILED $(cat {state}/failed)\" && [ $rc = 0 ] && rc=1;"
    t |> success(find(cuda, red_copy) >= 0 && find(cuda, red_copy) < find(cuda, "echo RUN-EXIT=$rc"), "a failed copy makes a green run red: {cuda}")
    t |> success(find(cuda, "[ $rc = 0 ] && rc=$\{mf:-1\}; echo MODEL-FREE-EXIT=$\{mf:-none\}; echo RUN-EXIT=$rc'") >= 0, "a red model-free suite makes a green run red, and says its own code")
    var mf_cfg = FleetArgs()
    mf_cfg.args = "--suite model-free"
    let once = run_command(mf_cfg, "20261027-000000", "vulkan", true)
    t |> success(find(once, "--suite model-free --await-models") == rfind(once, "--suite model-free --await-models") && find(once, " mf=0;") >= 0, "an --args that is the model-free suite runs it once: {once}")
    var skip_cfg = FleetArgs()
    skip_cfg.args = "--area llm --rerun"
    let rerun = run_command(skip_cfg, "20261027-000000", "vulkan", true)
    t |> success(find(rerun, "/run.das -- --suite model-free --rerun --await-models") >= 0, "an --args with --rerun hands it to the model-free suite too: {rerun}")
    t |> success(find(vulkan, "--rerun") < 0, "a run that names no such flag forces none: {vulkan}")
    t |> success(runs_model_free("--suite model-free") && runs_model_free("-s model-free") && runs_model_free("--suite=model-free"), "the model-free suite's spellings")
    t |> success(!runs_model_free("--changed --base origin/master") && !runs_model_free("--suite stocked --exclude model-free"), "another run is not it")
}

[test(description = "The fleet runs the model-free suite once: on a two-pod fleet the CUDA home's command carries it whole and the Vulkan home's only its Vulkan files (`VULKAN_FREE_ONLY`), which the CUDA home runs without their module, each home's exit in its MODEL-FREE-EXIT and RUN-EXIT; on a one-pod fleet the lone pod's Vulkan command carries it whole")]
def test_fleet_model_free_once(t : T?) {
    let cfg = FleetArgs()
    let two <- Fleet(pods <- [FleetPod(id = "b", index = 2), FleetPod(id = "e", index = 5)])
    for (seat in home_plan(two)) {
        let run = run_command(cfg, "20261027-000000", seat.home, seat.model_free)
        let whole = find(run, "--suite model-free --await-models") >= 0
        let slice_args = "--suite model-free --only {VULKAN_FREE_ONLY} --exclude {CUDA_ONLY_EXCLUDE} --await-models"
        let vk_slice = find(run, slice_args) >= 0
        t |> success(seat.home == "cuda" ? whole && !vk_slice : vk_slice && !whole,
            "two pods: the CUDA home runs the suite whole, the Vulkan home its Vulkan files: {run}")
    }
    let vulkan = run_command(cfg, "20261027-000000", "vulkan", false)
    let reds = "[ $rc = 0 ] && rc=$\{mf:-1\}; echo MODEL-FREE-EXIT=$\{mf:-none\}; echo RUN-EXIT=$rc"
    t |> success(find(vulkan, " mf=0;") < 0 && find(vulkan, reds) >= 0,
        "the Vulkan home's slice reds its RUN-EXIT and says its own code: {vulkan}")
    let one <- Fleet(pods <- [FleetPod(id = "e", index = 5)])
    let lone <- home_plan(one)
    t |> success(lone[0].model_free && !lone[1].model_free, "a lone pod's home holds the model-free suite: {lone}")
    let solo = run_command(cfg, "20261027-000000", lone[0].home, lone[0].model_free)
    t |> success(find(solo, "--suite model-free --await-models") >= 0, "the lone pod's Vulkan command carries it: {solo}")
}

[test(description = "A two-pod fleet's Vulkan home leaves out the CUDA-only files (`CUDA_ONLY_EXCLUDE`) in its given command, its model list and its model-free slice, with one `--exclude` that joins a caller's own; the CUDA home and a lone pod leave out none")]
def test_fleet_cuda_only_excluded(t : T?) {
    let cfg = FleetArgs()
    let exclude = " --exclude {CUDA_ONLY_EXCLUDE}"
    let vulkan = run_command(cfg, "20261027-000000", "vulkan", false)
    t |> success(find(vulkan, "--batch 30{exclude} --list-models") >= 0
            && find(vulkan, "--batch 30{exclude} --reap-images") >= 0,
        "the paired Vulkan home's model list and given command leave the CUDA-only files out: {vulkan}")
    t |> success(find(vulkan, "--only {VULKAN_FREE_ONLY}{exclude} --await-models") >= 0,
        "and so does its model-free slice: {vulkan}")
    let cuda = run_command(cfg, "20261027-000000", "cuda", true)
    t |> success(find(cuda, CUDA_ONLY_EXCLUDE) < 0, "the CUDA home leaves out none: {cuda}")
    let solo = run_command(cfg, "20261027-000000", "vulkan", true)
    t |> success(find(solo, CUDA_ONLY_EXCLUDE) < 0, "nor does a lone pod, which runs both homes' files: {solo}")
    var own = FleetArgs()
    own.args = "--changed --exclude test_gemma"
    let merged = run_command(own, "20261027-000000", "vulkan", false)
    t |> success(find(merged, "--exclude test_gemma,{CUDA_ONLY_EXCLUDE} --batch 30 --reap-images") >= 0,
        "a caller's --exclude takes the tokens: {merged}")
    t |> equal(length(split(merged, "--exclude test_")) - 1, 3,
        "one --exclude a command - the model list's, the slice's, the run's: {merged}")
    t |> equal(excluded_args("--changed --exclude=a", "b"), "--changed --exclude=a,b", "--exclude=<tokens> takes them too")
    t |> equal(excluded_args("--changed", "b"), "--changed --exclude b", "and arguments with none gain one")
}

[test(description = "Each home's run warms the CUDA kernels (cuda_prewarm.das) after the copier starts and ahead of the model-free suite, in the pod's tree; a red prewarm reds RUN-EXIT without stopping the suites; the status reads its line; the tool's line and exit code read its counts")]
def test_fleet_prewarm(t : T?) {
    let cfg = FleetArgs()
    for (home in fleet_homes()) {
        let run = run_command(cfg, "20261027-000000", home, true)
        let flags = home == "cuda" ? " --disable-module dasVulkan" : ""   // a lone pod keeps both
        let prewarm = (" && \{ ./bin/daslang{flags} -jit modules/dasLLAMA/harness/cuda_prewarm.das;"
            + " pw=$?; \} && \{")
        let at = find(run, prewarm)
        t |> success(at >= 0, "the {home} home warms the kernels, and its suites follow whatever the exit: {run}")
        t |> success(find(run, "cd /root/daScript && ") < at && find(run, "cp=$!;") < at,
            "in the pod's tree, after the copier starts: {run}")
        t |> success(at < find(run, "--suite model-free --await-models"), "ahead of the model-free suite: {run}")
        let red = "[ $rc = 0 ] && rc=$\{pw:-1\}; echo PREWARM-EXIT=$\{pw:-none\};"
        t |> success(find(run, red) >= 0 && find(run, red) < find(run, "echo RUN-EXIT=$rc"),
            "a red prewarm makes a green run red, and says its own code: {run}")
    }
    t |> success(find(status_command("20261027-000000"), "|^PREWARM|") >= 0, "the status reads the prewarm's lines")
    let warm = CudaWarm(compiled = 3, cached = 120, failed = 0, wall_ms = 41250l)
    t |> equal(cuda_warm_line(warm), "PREWARM compiled=3 cached=120 failed=0 refused=0 in 41.2 s", "the tool's line")
    t |> equal(cuda_warm_exit(warm), 0, "every kernel compiled: exit 0")
    t |> equal(cuda_warm_exit(CudaWarm(failed = 1)), 1, "a kernel that did not compile: exit 1")
    t |> equal(cuda_warm_exit(CudaWarm(refused <- ["k"])), 1, "a kernel whose load failed: exit 1")
    let none = CudaWarm(why = "no CUDA driver on this box")
    t |> equal(cuda_warm_line(none), "PREWARM none: no CUDA driver on this box", "a home that does not arm says why")
    t |> equal(cuda_warm_exit(none), 0, "... and is no failure")
}

[test(description = "The copier walks the list in order, skips the child lines, keeps a copy of the volume file's size and time, removes the link before rsync writes a new file in its place - no in-place write, no link followed - so nothing is written into the volume, and marks done or the file it failed on; the status reads each home's two REPORT lines green or red, the WAIT, MODEL-FREE-EXIT and COPY-FAILED lines, and the copy's state")]
def test_fleet_copier(t : T?) {
    let copy = copy_command("/x/copy")
    t |> success(find(copy, "while IFS= read -r p; do case \"$p\" in \"#\"*|\"\") continue;; esac;") == 0 && find(copy, "done < /x/copy/models.txt;") >= 0, "the list's paths in order, its child lines skipped: {copy}")
    t |> success(find(copy, "d=/root/models/$p; s=/workspace/models/$p;") >= 0, "a copy from the volume into the pod's models directory")
    t |> success(find(copy, "if [ -f \"$d\" ] && [ ! -L \"$d\" ] && [ \"$(stat -c %s.%Y \"$d\")\" = \"$(stat -c %s.%Y \"$s\")\" ]; then continue; fi;") >= 0, "a copy of the volume file's size and time stays: a second run copies nothing, a replaced file is copied again")
    t |> success(find(copy, "rm -f \"$d\" && rsync -a --whole-file \"$s\" \"$d\"") >= 0, "the link goes before rsync writes its own file in its place")
    let dropped = "&& sync \"$d\" && sh modules/dasLLAMA/harness/pod_mem_track.sh drop \"$d\" \"$s\" && echo \"COPIED $p\""
    t |> success(find(copy, dropped) > find(copy, "rsync"), "a copy is synced and dropped from the page cache, its volume file too, before it counts as copied")
    let tracker = fread("modules/dasLLAMA/harness/pod_mem_track.sh")
    let drop_one = "dd if=\"$f\" iflag=nocache count=0 status=none || rc=1"
    t |> success(find(tracker, drop_one) >= 0 && find(tracker, "iflag=nocache") == rfind(tracker, "iflag=nocache")
        && find(tracker, "-exec sh \"$0\" drop \{\} +") >= 0, "the tracker's drop mode is the one spelling of a page drop, its own loop's too: {tracker}")
    t |> success(find(copy, "--inplace") < 0 && find(copy, "--copy-links") < 0 && find(copy, "rsync") == rfind(copy, "rsync") && find(copy, "> /workspace") < 0, "no write through a link: rsync renames its temp file into place")
    t |> success(find(copy, "|| \{ echo \"$p\" > /x/copy/failed; exit 1; \};") >= 0 && find(copy, "; echo ok > /x/copy/done") >= 0, "a failed copy marks its file and ends the walk, a whole walk marks done")
    t |> success(find(copy, "'") < 0, "the copier holds no single quote: it rides inside the detached command's")
    let status = status_command("20261027-000000")
    t |> success(find(status, "grep -E '^(\\[[IWE]\\] )?(REPORT|NEXT|WAIT) |") >= 0, "every REPORT line, a red one too (its line carries no [I] mark), and the WAIT lines: {status}")
    t |> success(find(status, "MODEL-FREE-EXIT|COPY-FAILED|RUN-EXIT") >= 0, "the two runs' verdicts and the copy's failure")
    t |> success(find(status, "c=/workspace/logs/fleet/20261027-000000/$h/copy; if [ -f $c/failed ]; then echo \"COPY $h: failed on $(cat $c/failed)\";") >= 0
        && find(status, "elif [ -f $c/done ]; then") >= 0 && find(status, "else echo \"COPY $h: running\"; fi;") >= 0, "the copy's state a home")
}

[test(description = "A home's run copies its own volume cache onto the pod ahead of the model list and the prewarm and back once both runs end, ahead of RUN-EXIT and outside its code, by rsync -a with no --delete, the volume's files past fourteen days pruned; the status reads the CACHE-IN and CACHE-OUT lines")]
def test_fleet_caches(t : T?) {
    let cfg = FleetArgs()
    for (home in fleet_homes()) {
        let run = run_command(cfg, "20261027-000000", home, true)
        let cache_in = cache_in_command(home)
        let cache_out = cache_out_command(home)
        let in_at = find(run, cache_in)
        let out_at = find(run, cache_out)
        t |> success(in_at >= 0 && in_at < find(run, "--list-models") && in_at < find(run, "cuda_prewarm.das"),
            "the {home} home's caches come in ahead of the model list and the prewarm: {run}")
        t |> success(out_at > find(run, "--suite model-free --await-models") && out_at > find(run, "--stop-file")
            && out_at < find(run, "echo RUN-EXIT=$rc"), "and go back after both runs, ahead of RUN-EXIT: {run}")
        t |> success(find(cache_out, "rc=") < 0 && find(cache_in, "rc=") < 0, "a cache copy never sets the run's code")
        let other = home == "cuda" ? "vulkan" : "cuda"
        t |> success(find(cache_in, "/workspace/cache/{home}/daslang/ /root/daScript/.cache/daslang/") >= 0
            && find(cache_in, "/workspace/cache/{home}/nvidia/ /root/.cache/nvidia/") >= 0
            && find(cache_out, "/root/daScript/.cache/daslang/ /workspace/cache/{home}/daslang/") >= 0
            && find(cache_out, "/root/.cache/nvidia/ /workspace/cache/{home}/nvidia/") >= 0
            && find(run, "/workspace/cache/{other}") < 0, "each home names its own volume folder: {cache_in} | {cache_out}")
        t |> success(find(cache_in, "--delete") < 0 && find(cache_out, "--delete") < 0
            && find(cache_in, "rsync -a ") >= 0 && find(cache_out, "rsync -a ") >= 0,
            "rsync -a keeps the mtimes the caches' keys read, and the copy back is a merge")
        t |> success(find(cache_in, "'") < 0 && find(cache_out, "'") < 0,
            "neither holds a single quote: both ride inside the detached command's")
    }
    let cache_in = cache_in_command("vulkan")
    t |> success(find(cache_in, "if [ -d /workspace/cache/vulkan ]; then") == 0 && find(cache_in, "else echo \"CACHE-IN none\"; fi") >= 0,
        "a home with no volume cache starts cold and says so: {cache_in}")
    t |> success(find(cache_in, "echo \"CACHE-IN $(du -sh /workspace/cache/vulkan | cut -f1)\" || echo \"CACHE-IN failed: exit $?\";") >= 0,
        "one line: the size read in, or the failed copy")
    let cache_out = cache_out_command("vulkan")
    t |> success(find(cache_out, "find /workspace/cache/vulkan -type f -mmin +20160 -delete") > find(cache_out, "rsync"),
        "the files not modified for fourteen days go after the copy back: {cache_out}")
    t |> equal(CACHE_DAYS, 14, "the prune's age")
    t |> success(find(cache_out, "rsync -a --exclude module_cache/dastest.dascache /root/daScript/.cache/daslang/") >= 0
        && find(cache_in_command("vulkan"), "--exclude") < 0, "the runner's unkeyed module record stays on the pod, and nothing else does")
    t |> success(find(cache_out, "echo \"CACHE-OUT $(du -sh /workspace/cache/vulkan | cut -f1)\" || echo \"CACHE-OUT failed: exit $?\"") >= 0,
        "one line: the size kept, or the failed step")
    t |> success(find(status_command("20261027-000000"), "|^CACHE-(IN|OUT) |") >= 0, "the status reads the caches' lines")
    t |> success(find(prep_command(), "--exclude .cache") >= 0 && find(prep_command(), "/workspace/cache") < 0,
        "the tree's copy leaves the caches to the home's run")
}

[test(description = "On a two-pod fleet each home's run leaves out the other home's dynamic module - the CUDA home DASLLAMA_DISABLE_MODULES=dasVulkan, the Vulkan home dasCUDA - in its environment, which its children read, and on its prewarm's command line; a lone pod leaves out none; a caller's --env value wins on both")]
def test_fleet_home_modules(t : T?) {
    let cfg = FleetArgs()
    for (home, model_free, disabled in fixed_array("cuda", "vulkan", "vulkan"),
            fixed_array(true, false, true), fixed_array("dasVulkan", "dasCUDA", "")) {
        let run = run_command(cfg, "20261027-000000", home, model_free)
        let env = " DASLLAMA_DISABLE_MODULES={disabled} "
        let flags = empty(disabled) ? "" : " --disable-module {disabled}"
        let paired = home == "cuda" || !model_free
        let tag = "{home}, model-free {model_free}"
        t |> equal(home_disabled_modules(cfg.env, home, paired), disabled, "{tag}: the modules it leaves out")
        let no_env = find(run, "DASLLAMA_DISABLE_MODULES") < 0
        let in_env = empty(disabled) ? no_env : find(run, env) >= 0
        t |> success(in_env, "{tag}: the environment leaves out {disabled}: {run}")
        let prewarm = "./bin/daslang{flags} -jit modules/dasLLAMA/harness/cuda_prewarm.das"
        t |> success(find(run, prewarm) >= 0, "{tag}: the prewarm compiles without it: {run}")
    }
    var own = FleetArgs()
    own.env = "DASLLAMA_DISABLE_MODULES=dasCUDA,dasVulkan"
    let run = run_command(own, "20261027-000000", "cuda", true)
    let owns = find(run, " DASLLAMA_DISABLE_MODULES=dasCUDA,dasVulkan") >= 0
    let home_default = find(run, "DASLLAMA_DISABLE_MODULES=dasVulkan") >= 0
    t |> success(owns && !home_default, "the caller's value is the run's: {run}")
    let flags = "./bin/daslang --disable-module dasCUDA --disable-module dasVulkan -jit"
    t |> success(find(run, flags) >= 0, "and its prewarm's: {run}")
}
