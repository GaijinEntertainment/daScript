options gen2

module worker_policy shared

require math
require daslib/cmp

//! Half the logical threads (an SMT box's cores); past 16 a sweep's
//! wall is its longest files.
def public default_isolated_threads(hw_threads : int) : int {
    let hw = max(1, hw_threads)
    return clamp(hw / 2, 4, 16)
}

//! Batches in start order, each a list of file indices;
//! `budgets[i]` sums file i's `[test(timeout = N)]`.
def public isolated_batches(budgets : array<float>; batch : int) : array<array<int>> {
    var alone <- [for (b, i in budgets, count()); i; where b > 0.0]
    alone |> sort() <| cmp_by(-budgets[_], _)
    var batches : array<array<int>>
    batches |> reserve(length(alone) + (length(budgets) - length(alone)) / batch + 1)
    for (i in alone) {
        var one <- [i]
        batches |> emplace(one)
    }
    var rest : array<int>
    for (b, i in budgets, count()) {
        continue if (b > 0.0)
        rest |> push(i)
        if (length(rest) == batch) {
            batches |> emplace(rest)
        }
    }
    if (!empty(rest)) {
        batches |> emplace(rest)
    }
    return <- batches
}
