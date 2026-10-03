options gen2
require daslib/jobque_boost
require daslib/fio

struct Value { number : int }
var commands : Stream?
var channel : Channel?
var replies : Stream?
var done : Channel?
var phase = 0
var received = 0
var started : int64

[export]
def init() {
    commands = unsafe(stream_create()); commands |> append(1)
    channel = unsafe(channel_create()); channel |> append(1)
    replies = unsafe(stream_create())
    done = unsafe(channel_create()); done |> append(2)
    var work = commands; var reply = replies; var finish = done
    new_thread() @() {
        reply |> push([0u8])
        work |> pop() $(bytes) {
            assert(length(bytes) == 1 && bytes[0] == 42u8)
            reply |> push([42u8])
        }
        work |> pop() $(bytes) { assert(empty(bytes)) }
        reply |> push([99u8])
        work |> release(); reply |> release(); finish |> notify_and_release()
    }
    var ch = channel
    new_thread() @() {
        reply |> push([0u8])
        ch |> pop_and_clone_one() $(value : Value#) {
            assert(value.number == 42)
            reply |> push([42u8])
        }
        var called = false
        ch |> pop_and_clone_one() $(value : Value#) { called = true }
        assert(!called)
        reply |> push([99u8])
        ch |> release(); reply |> release(); finish |> notify_and_release()
    }
}

[export]
def update() : bool {
    replies |> gather() $(bytes) {
        assert(length(bytes) == 1)
        assert(bytes[0] == (phase == 0 ? 0u8 : phase == 2 ? 42u8 : 99u8))
        received++
    }
    if ((phase == 0 || phase == 2) && received == 2) {
        phase++; received = 0; started = ref_time_ticks()
    }
    if ((phase == 1 || phase == 3) && get_time_usec(started) >= 500000) {
        if (phase == 1) {
            commands |> push([42u8]); channel |> push_clone(Value(number = 42))
        } else {
            commands |> notify(); channel |> notify()
        }
        phase++
    }
    if (phase == 4 && received == 2) {
        done |> join()
        unsafe { stream_remove(commands); channel_remove(channel); stream_remove(replies); channel_remove(done) }
        print("PASS idle pop wakes for payload and producer completion\n")
        return false
    }
    return true
}

[export]
def main() { init(); while (update()) { sleep(1u) } }
