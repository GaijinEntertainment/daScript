options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ../harness/pod_fleet.das
require daslib/command_line
require daslib/fio
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

[test(description = "The fleet's ssh, prep, shard run, times, status and home-path planners emit the expected command text, `times_report` sums file seconds and keeps each cell's longest, and sparse pod indices shard 1..n")]
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
    t |> success(find(run, "LD_LIBRARY_PATH=/root/daScript/lib ") >= 0, "the children load the pod's own runtime libraries, not the volume tree's")
    t |> success(find(run, "DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_GPU=1") >= 0, "the children's environment")
    t |> success(find(run, "> /workspace/logs/fleet/20261027-000000/shard_2.log") >= 0, "the runner's log beside them")
    t |> success(find(run, "echo RUN-EXIT=$?") >= 0, "RUN-EXIT closes the log")
    t |> success(find(run, "setsid nohup") >= 0 && find(run, "; setsid") >= 0, "detached, so the ssh call returns at once")
    let times = times_command("20261027-000000")
    t |> success(find(times, "/workspace/logs/fleet/20261027-000000/pod_*/dasllama_run_*.log") >= 0, "the times read every child log of the run")
    t |> success(find(times, "F ") >= 0 && find(times, "T ") >= 0, "a file line and a cell line each carry their mark")
    let report = times_report("F 12.5 modules/dasLLAMA/tests/test_a.das\nF 7.5 modules/dasLLAMA/tests/test_a.das\nF 5 modules/dasLLAMA/tests/test_b.das\nT 9 cell_x\nT 11 cell_x\nT 2 cell_y\nnoise\n", 1)
    t |> success(find(report, "test time 25 s over 2 file(s)") >= 0, "a file's seconds sum over the children that ran it: {report}")
    t |> success(find(report, "  20 s  80%  test_a.das") >= 0, "the top file carries its share")
    t |> success(find(report, "  11 s  cell_x") >= 0 && find(report, "cell_y") < 0, "a cell keeps its longest reading, the top list cuts at `top`")
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

// The supervisor cancel-minting rail, end to end in a child process: while the file named by
// DAS_TUNE_CONTROL exists, interrupt_gate() aborts between kernel families - the @tune abort
// event fires and nothing past the gate runs (minting is end-gated, so an interrupted run
// writes no sidecar). The env is load-once, so a spawned child is the only honest probe.

def private run_probe(control_env : string) : string {
    let win = get_platform_name() == "windows"
    // an empty control_env is spelled as an explicit UNSET - the parent (the watchdog sets
    // this very variable) must never leak its own value into the "unset" cell
    let envs = (empty(control_env)
        ? (win ? "set \"DAS_TUNE_CONTROL=\"&& " : "env -u DAS_TUNE_CONTROL ")
        : (win ? "set \"DAS_TUNE_CONTROL={control_env}\"&& "
               : "DAS_TUNE_CONTROL=\"{control_env}\" "))
    let cmd = "{envs}\"{get_das_exe()}\" \"modules/dasLLAMA/tests/_interrupt_probe.das\" -dasroot \"{get_das_root()}\" 2>&1"
    var out = ""
    unsafe(popen(cmd) $(f) {
        if (f != null) {
            out = build_string() $(w) {
                while (!feof(f)) {
                    w |> write(fgets(f))
                }
            }
        }
    })
    return out
}

[test(description = "In a spawned child, an existing DAS_TUNE_CONTROL file makes `interrupt_gate` fire the tune abort event and stop before anything past the gate runs, while a named-but-absent file or an unset variable passes through", timeout = 1200)]
def test_interrupt_aborts_without_minting(t : T?) {
    t |> run("a present control file aborts at the gate") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        let out = run_probe(control)
        tt |> success(out |> find("PROBE: before gate") >= 0, "the probe ran to the gate: {out}")
        tt |> success(out |> find("@tune abort reason=interrupted at=probe_family") >= 0, "the abort event fired: {out}")
        tt |> success(out |> find("gate passed through") < 0, "nothing past the gate ran - minting is end-gated, so nothing was written: {out}")
        remove(control)
    }
    t |> run("a named-but-absent control file passes through") @(tt : T?) {
        let res = create_temp_file_result("tune_control_", ".stop")
        if (res is error) {
            tt |> failure("could not create a temp control file")
            return
        }
        let control = res as value
        remove(control)
        let out = run_probe(control)
        tt |> success(out |> find("gate passed through") >= 0, "the gate is armed only while the file EXISTS: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort without the file: {out}")
    }
    t |> run("no DAS_TUNE_CONTROL passes through") @(tt : T?) {
        let out = run_probe("")
        tt |> success(out |> find("gate passed through") >= 0, "unset env never interrupts: {out}")
        tt |> success(out |> find("@tune abort") < 0, "no abort with the env unset: {out}")
    }
}

// The float-A tmm2d gate (msl_emit): an UNLICENSED float A operand in any tmm2d form is a
// compile error - the half twin is measured 8-11% faster per GEMM, so a new family must
// either stamp a float16 XT twin or declare the f32 fallback deliberate with
// [metal_kernel(float_a_ok=true)]. Spawned compile-only fixtures, both directions.

def private fixture_source(flag : string) : string {
    return build_string() $(var w) {
        w |> write("options gen2\n")
        w |> write("require metal/msl_shader\n")
        w |> write("require metal/metal_builtins\n")
        w |> write("class FloatAFx \{\n")
        w |> write("    @ssbo @binding = 0 wsh : array<float16>\n")
        w |> write("    @ssbo @binding = 1 wqb : array<int8>\n")
        w |> write("    @ssbo @binding = 2 xf : array<float>\n")
        w |> write("    @ssbo @binding = 3 y : array<float>\n")
        w |> write("    @uniform @binding = 4 kdim : uint\n")
        w |> write("    @uniform @binding = 5 ndim : uint\n")
        w |> write("    @workgroup twb : float16[2048]\n")
        w |> write("    [metal_kernel{flag}]\n")
        w |> write("    def fx_kernel \{\n")
        w |> write("        let mBase = gl_WorkGroupID.x * 32u\n")
        w |> write("        let nBase = gl_WorkGroupID.y * 64u\n")
        w |> write("        var sp = unsafe(addr(wsh[nBase * (kdim / 32u) * 17u]))\n")
        w |> write("        var wp = unsafe(addr(wqb[nBase * (kdim / 32u) * 34u + 2u]))\n")
        w |> write("        var ap = unsafe(addr(xf[mBase * kdim]))\n")
        w |> write("        var cp = unsafe(addr(y[mBase * ndim + nBase]))\n")
        w |> write("        tmm2d_q8u_f32(32u, 64u, 4u, twb, sp, wp, ap, kdim, cp, ndim, kdim, kdim / 32u, gl_LocalInvocationIndex)\n")
        w |> write("    \}\n")
        w |> write("\}\n")
        w |> write("[export]\n")
        w |> write("def main \{\n")
        w |> write("    print(FloatAFx_fx_kernel_msl)\n")
        w |> write("\}\n")
    }
}

def private spawn_compile(flag, tag : string) : tuple<rc : int; out : string> {
    let dir_r = create_temp_directory_result("metal_float_a_{tag}")
    if (!(dir_r is value)) {
        return (rc = -1, out = "temp dir failed")
    }
    let dir = unsafe(dir_r.value)
    let path = path_join(dir, "prog.das")
    if (!fwrite(path, fixture_source(flag))) {
        return (rc = -1, out = "fwrite failed")
    }
    let argv <- get_command_line_arguments()
    let self = find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
    var out : string
    let rc = run_and_capture([self, "-compile-only", path], out, 120.0)
    rmdir_rec_result(dir)
    return (rc = rc, out = out)
}

[test(description = "A Metal kernel feeding a float A operand to a tmm2d form fails to compile with an error naming the FLOAT A rule, and the same kernel compiles once marked `float_a_ok=true`", timeout = 1200)]
def test_float_a_gate(t : T?) {
    t |> run("an unlicensed float A operand does not compile") @(t : T?) {
        let r = spawn_compile("", "bare")
        t |> success(r.rc != 0, "bare float-A fixture must fail (rc={r.rc})")
        t |> success(find(r.out, "FLOAT A operand") >= 0, "the failure names the rule: {slice(r.out, 0, 400)}")
    }
    t |> run("float_a_ok licenses the deliberate f32 fallback") @(t : T?) {
        let r = spawn_compile("(float_a_ok=true)", "licensed")
        t |> equal(0, r.rc, "licensed fixture compiles: {slice(r.out, 0, 400)}")
    }
}
