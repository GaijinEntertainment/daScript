options gen2
options indenting = 4

module kill_switch shared

require daslib/fio
require daslib/jobque_boost
require strings
require math

//! The exit code of a process its kill switch ended, where the
//! guard names no other
let KILL_SWITCH_EXIT_CODE = 125

//! The guards a process's kill switch holds at once: each has a
//! deadline, a line and an exit code of its own
let KILL_SWITCH_GUARDS = 2
let KILL_SWITCH_TEST = 0    //! the guard of one test
let KILL_SWITCH_SUITE = 1   //! the guard of a whole run

let private KILL_SWITCH_LABEL_BYTES = 256
let private KILL_SWITCH_POLL_MSEC = 100
let private KILL_SWITCH_STOP_MSEC = 2000

let private WORD_ARM = 0
let private WORD_SAY = 1
let private WORD_DISARM = 2
let private WORD_STOP = 3

//! One word to the switch's thread, and the thread's record of a
//! guard: the ticks it was armed at - 0 while disarmed - its
//! seconds, the exit code it ends the process with and the clause
//! it prints. Fixed storage: a word crosses to the thread as bytes
//! on a stream, and no memory is read by both sides
struct private KillSwitchWord {
    verb : int
    guard : int
    armed_at : int64
    seconds : float
    exit_code : int
    what : uint8[KILL_SWITCH_LABEL_BYTES]
}

//! The words to the thread - null until a guard is first armed -
//! and the thread's answer: 1 once it let go of the stream, its
//! last act
var private g_words : Stream?
var private g_stopped : Atomic32?
var private g_armed : bool[KILL_SWITCH_GUARDS]

def private word_says(var word : KillSwitchWord; what : string) {
    let n = min(length(what), KILL_SWITCH_LABEL_BYTES - 1)
    peek_data(what) $(bytes) {
        for (i in range(n)) {
            word.what[i] = bytes[i]
        }
    }
    word.what[n] = 0u8
}

//! The thread takes one word into its guards; false on the word
//! that ends it
def private word_taken(var guards : KillSwitchWord[KILL_SWITCH_GUARDS];
                       word : KillSwitchWord) : bool {
    return false if (word.verb == WORD_STOP)
    if (word.verb == WORD_SAY) {
        guards[word.guard].what = word.what
    } elif (word.verb == WORD_DISARM) {
        guards[word.guard].armed_at = 0l
    } else {
        guards[word.guard] = word
    }
    return true
}

//! Ends the process when a guard is past its seconds
def private guard_checked(g : KillSwitchWord) {
    let past = double(g.seconds) * 1000000000.0lf
    return if (g.armed_at == 0l || double(get_time_nsec(g.armed_at)) <= past)
    let said = build_string() $(w) {
        for (b in g.what) {
            break if (b == 0u8)
            w |> write_char(int(b))
        }
    }
    print("{said}; the process ends here with exit code {g.exit_code}\n")
    //! not fio::exit - it runs the job queue teardown, which waits
    //! on this very thread
    unsafe(fio::exit_now(g.exit_code))
}

def private kill_switch_start() {
    g_words = unsafe(stream_create())
    //! one producer stays open, so the thread's timed pop waits:
    //! a stream with none answers at once, and the thread spins
    g_words |> append(1)
    g_stopped = atomic32_create()
    var words = g_words
    var stopped = g_stopped
    unsafe {
        new_thread <| @capture(= words, = stopped) {
            var guards : KillSwitchWord[KILL_SWITCH_GUARDS]
            var run = true
            while (run && !is_job_que_shutting_down()) {
                //! every queued word is taken before a deadline is
                //! read: a disarm waits behind its arm
                let poll = KILL_SWITCH_POLL_MSEC
                var got = words |> pop_archive_with_timeout(poll) $(var word : KillSwitchWord&) {
                    run = word_taken(guards, word)
                }
                while (got && run) {
                    got = words |> try_pop_archive() $(var word : KillSwitchWord&) {
                        run = word_taken(guards, word)
                    }
                }
                for (g in guards) {
                    guard_checked(g)
                }
            }
            words |> release
            stopped |> set(1)
        }
    }
}

//! Arms guard `guard` of the process's kill switch for `seconds` -
//! on a test, a whole run, any step that must not outlast them.
//! Unless the guard is armed again or disarmed before they pass,
//! the switch's thread prints `what` - the clause naming what ran
//! past what - and ends the process at once with `exit_code`.
//! Arming a guard again replaces what it guards; the guards do not
//! see each other
def kill_switch_arm(guard : int; seconds : float; what : string;
                    exit_code : int = KILL_SWITCH_EXIT_CODE) {
    if (g_words == null) {
        kill_switch_start()
    }
    var word = KillSwitchWord(verb = WORD_ARM, guard = guard, armed_at = ref_time_ticks(),
        seconds = seconds, exit_code = exit_code)
    word_says(word, what)
    g_armed[guard] = true
    g_words |> push_archive(word)
}

//! Replaces the clause an armed guard prints, its deadline kept: a
//! run that moved on to its next step says so
def kill_switch_say(guard : int; what : string) {
    return if (!g_armed[guard])
    var word = KillSwitchWord(verb = WORD_SAY, guard = guard)
    word_says(word, what)
    g_words |> push_archive(word)
}

//! Disarms a guard: what it guarded ended in time
def kill_switch_disarm(guard : int) {
    return if (!g_armed[guard])
    g_armed[guard] = false
    g_words |> push_archive(KillSwitchWord(verb = WORD_DISARM, guard = guard))
}

//! The context that armed the switch ends: its thread is told to
//! stop and waited for, then the stream and the answer are freed.
//! A thread that does not answer in time keeps both
[finalize]
def private kill_switch_stop() {
    return if (g_words == null)
    g_words |> push_archive(KillSwitchWord(verb = WORD_STOP))
    let t0 = ref_time_ticks()
    let wait_nsec = int64(KILL_SWITCH_STOP_MSEC) * 1000000l
    while (g_stopped |> get() == 0 && get_time_nsec(t0) < wait_nsec) {
        fio::sleep(5u)
    }
    return if (g_stopped |> get() == 0)
    unsafe {
        stream_remove(g_words)
        atomic32_remove(g_stopped)
    }
}
