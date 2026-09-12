options gen2
options debugger

require daslib/fio


def dap_add_one(value : int) : int {
    var result = value + 1
    result++
    return result
}


[export]
def main() {
    let seed = 40
    let answer = dap_add_one(seed)
    var guard = 0
    let started = ref_time_ticks()
    while (get_time_usec(started) < 300000) {
        guard++
        sleep(1u)
    }
    print("DAP_RESULT={answer}\n")
}
