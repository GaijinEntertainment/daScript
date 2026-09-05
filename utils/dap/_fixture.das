options gen2
options debugger

require daslib/fio


def dap_add_one(value : int) : int {
    var result = value + 1
    result += 1
    return result
}


[export]
def main() {
    var seed = 40
    let answer = dap_add_one(seed)
    var guard = 0
    let started = ref_time_ticks()
    while (get_time_usec(started) < 300000) {
        guard += 1
        sleep(1u)
    }
    print("DAP_RESULT={answer}\n")
}
