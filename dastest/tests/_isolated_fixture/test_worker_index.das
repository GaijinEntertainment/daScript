options gen2

require dastest/testing_boost public
require daslib/clargs
require strings

[test]
def test_worker_index_round_trip(tt : T?) {
    tt |> run("--worker-index N is visible via get_user_args() and in range") <| @(t : T?) {
        let args <- get_user_args()
        let idx = find_index(args, "--worker-index")
        t |> success(idx >= 0, "--worker-index not found in get_user_args() args={args}")
        if (idx < 0) {
            return
        }
        t |> success(idx + 1 < length(args), "no value after --worker-index")
        if (idx + 1 >= length(args)) {
            return
        }
        let val = to_int(args[idx + 1])
        t |> success(val >= 0 && val < 4, "worker_index {val} out of range [0, 4)")
    }
}
