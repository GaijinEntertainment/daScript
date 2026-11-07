options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/fio   // base_name: a file's record names it by its name alone
require dasllama/dasllama_qwen3a   // Qwen3aFlavor: the record a qwen3a load hands its flavor pick
require dasllama/dasllama_tower_image   // tower_dev_flavor_announced
require dasllama/dasllama_tower   // TowerDev
require ?cuda dasllama/dasllama_cuda_tower_seat   // nolint:STYLE030,LINT019 - cut_lane_arms and the tower knob, inside the dasCUDA half

// The fixture test_asr_verbs.das and test_cuda_tower_blocks.das spawn at the info floor: the line a load's flavor pick says where
// it names a source's conversion prints through to_log, which no in-process cell reads. `src <gguf>...` asks qwen3a's pick under a
// CUDA flavor whose chain serves every lane and then under no driver over each file, on both lanes, one
// `FLAVOR <file> <backend> q8=<lane> <flavor>` record each;
// `cut` asks the CUDA seat's lane arms on a dasCUDA build, the tower knob off and then on with no home armed, one
// `CUT <case> <answer>` record each. Every case opens with a `CASE <name>` record.

[unused_argument(lane)]
def private serves_every_lane(lane : bool) : bool => true

def private src_case(paths : array<string>) {
    to_log(LOG_INFO, "CASE src\n")
    for (backend in fixed_array(TowerDev.cuda, TowerDev.none)) {
        let f = Qwen3aFlavor(backend = backend, serves = @@serves_every_lane)
        for (p in paths) {
            for (q8 in fixed_array(true, false)) {
                to_log(LOG_INFO, "FLAVOR {base_name(p)} {backend} q8={q8} {tower_dev_flavor_announced(f, q8, p, "qwen3a")}\n")
            }
        }
    }
}

def private cut_case() {
    static_if (typeinfo builtin_module_exists(cuda)) {
        to_log(LOG_INFO, "CASE cut\n")
        let was = cuda_tower_on()
        set_cuda_tower(false)
        to_log(LOG_INFO, "CUT knob {cut_lane_arms("qwen3a", true)}\n")
        set_cuda_tower(was)
        to_log(LOG_INFO, "CUT device {cut_lane_arms("qwen3a", false)}\n")
    }
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let at = find_index(args, "--")
    var rest <- [for (i in range(at + 1, length(args))); args[i]; where at >= 0]
    if (!empty(rest) && rest[0] == "src") {
        rest |> erase(0)
        src_case(rest)
    } elif (!empty(rest) && rest[0] == "cut") {
        cut_case()
    }
    to_log(LOG_INFO, "ROOT_DONE\n")
}
