// The scheduler tests' shared rig: build a request, step a scheduler until it is idle, read a stream's events back.

options gen2
options _dasllama_internal = true

module _scheduler_rig shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_scheduler public
require dasllama/dasllama public

def make_req(id : int64; prompt : array<int64>; params : SamplingParams; max_new : int64;
             stop_ids : array<int64>; seed : int = 0) : PendingReq {
    var r <- PendingReq(id = id, params = params, max_new = max_new, seed = seed)
    r.prompt := prompt
    r.stop_ids := stop_ids
    return <- r
}

def make_req(id : int64; prompt : array<int64>; max_new : int64) : PendingReq {
    var r <- PendingReq(id = id, params = SamplingParams(), max_new = max_new)
    r.prompt := prompt
    return <- r
}

//! step until idle, bounded: a runaway scheduler must not hang the suite
def drain(m : Model; var sch : Scheduler; var events : array<SchedEvent>) {
    var steps = 0
    while (scheduler_step(m, sch, events)) {
        steps ++
        if (steps > 4096) {
            panic("_scheduler_rig: the scheduler did not go idle in 4096 steps")
        }
    }
}

def toks_of(events : array<SchedEvent>; id : int64) : array<int64>
    => <- [for (e in events); e.token; where e.stream == id && e.kind == SchedEventKind.piece]

//! stream `id`'s finished event, or one whose `stream` is -1 when it never finished
def finished_of(events : array<SchedEvent>; id : int64) : SchedEvent {
    for (e in events) {
        if (e.stream == id && e.kind == SchedEventKind.finished) {
            return e
        }
    }
    return SchedEvent(stream = -1l)
}
