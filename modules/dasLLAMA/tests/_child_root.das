options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require daslib/fio
require daslib/clargs   // get_user_args: after `--` under daslang, after the exe's own name in a standalone build
require _child_audio_lane
require _child_home_words
require _child_single_thread
require _child_metal_tensor
require _child_override_announce
require _child_flavor_decline
require _child_jobque_tripwire
require _child_team_panic
require _child_resident_load
require _child_cuda_cache
require _child_disable_flags

// The test child: ONE program for every once-per-process contract the tests of this folder check - a knob the engine
// reads at context init, a line only a process's log carries, an exit a panic decides. A test spawns it with a verb and
// its own environment (`_child_run.das`); the runner builds it once as a standalone exe, so a spawn costs a process
// start and not an engine compile. A verb is one function of a `_child_<verb>.das` module, dispatched here. The program
// starts no thread of its own: a thread is a second context, whose init would say every once-a-process line again -
// the spawning test's timer is the verb's wall (`run_verb_child`).

[export]
def main() {
    let args <- get_user_args()
    let verb = empty(args) ? "" : args[0]
    if (verb == "ping") {
        print("PONG\n")
    } elif (verb == "audio-lane") {
        child_audio_lane()
    } elif (verb == "home-words") {
        child_home_words()
    } elif (verb == "single-thread") {
        child_single_thread()
    } elif (verb == "metal-tensor") {
        child_metal_tensor()
    } elif (verb == "override-announce") {
        child_override_announce()
    } elif (verb == "flavor-decline") {
        child_flavor_decline([for (i in range(1, length(args))); args[i]])
    } elif (verb == "jobque-tripwire") {
        child_jobque_tripwire()
    } elif (verb == "team-panic") {
        child_team_panic()
    } elif (verb == "cuda-cache") {
        child_cuda_cache([for (i in range(1, length(args))); args[i]])
    } elif (verb == "disable-flags") {
        child_disable_flags_report()
    } elif (verb == "resident-load") {
        child_resident_load([for (i in range(1, length(args))); args[i]])
    } else {
        print("the test child has no verb '{verb}'\n")
        unsafe(fio::exit_now(2))
    }
}
