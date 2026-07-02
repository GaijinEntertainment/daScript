options gen2

// Fixture for test_collect_issues.das — one LINT003 (var-can-be-let), one
// PERF018 (index-only range loop), one STYLE024 (redundant unsafe wrap).
// The test asserts on the LINE NUMBERS below — keep them pinned.

[export]
def fixture_function() : int {
    var can_be_const <- [1, 2, 3]                   // line 9  — LINT003
    var total = 0
    for (i in range(length(can_be_const))) {        // line 11 — PERF018
        total += can_be_const[i]
    }
    let wrapped = unsafe(3 + 4)                     // line 14 — STYLE024
    return total + wrapped
}
