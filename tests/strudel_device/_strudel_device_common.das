options gen2
options indenting = 4

module _strudel_device_common public

require strudel/strudel_player
require daslib/fio

//! A worker whose clock stands still this long has stalled. A slow worker - a sanitizer build on a busy runner - is not a failure.
let WORKER_STALL_MS = 30000
let private POLL_MS = 20u

//! The playback worker's own clock: seconds of audio it has rendered, so it slows down with the worker.
def worker_seconds() : double {
    return strudel_get_playback_time()._0
}

//! Polls until the worker's clock reaches `until`, running `each_poll` after every poll; false when the clock stands still for WORKER_STALL_MS of real time first.
def wait_worker_until(until : double; each_poll : block<() : void>) : bool {
    var clock = worker_seconds()
    var moved = ref_time_ticks()
    while (clock < until) {
        sleep(POLL_MS)
        invoke(each_poll)
        let now = worker_seconds()
        if (now > clock) {
            clock = now
            moved = ref_time_ticks()
        } elif (get_time_usec(moved) >= WORKER_STALL_MS * 1000) {
            return false
        }
    }
    return true
}

def wait_worker_until(until : double) : bool {
    return wait_worker_until(until) $ {
        pass
    }
}
