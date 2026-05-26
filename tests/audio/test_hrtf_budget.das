options gen2
options indenting = 4

require dastest/testing_boost
require audio/audio_boost

// Unit tests for the HRTF/simulated-3D budget classifier. The classifier is the pure rank-vs-budget
// decision update_hrtf consults each frame to decide whether a channel runs HRTF (expensive, top-N
// closest) or simulated 3D (constant-power pan + attenuation, cheaper). Sticky margin in rank space
// prevents flapping when channels swap rank between frames, but must NOT preserve HRTF status when
// the budget is 0 (otherwise "all simulated" doesn't clear in-flight HRTF channels).

[test]
def test_full_hrtf_budget_keeps_everything_hrtf(t : T?) {
    t |> run("budget=999: every rank routed to HRTF regardless of prior state") @(t : T?) {
        t |> success(hrtf_budget_classify(0, 999, true),  "rank 0, wasHrtf -> HRTF")
        t |> success(hrtf_budget_classify(0, 999, false), "rank 0, !wasHrtf -> HRTF (flip in)")
        t |> success(hrtf_budget_classify(31, 999, true), "rank 31, wasHrtf -> HRTF")
        t |> success(hrtf_budget_classify(998, 999, false), "rank below budget flips in")
        t |> success(!hrtf_budget_classify(999, 999, false), "rank == budget for !wasHrtf is NOT under-budget")
    }
}

[test]
def test_zero_budget_clears_all_hrtf(t : T?) {
    t |> run("budget=0: sticky margin must NOT preserve HRTF — all channels go simulated") @(t : T?) {
        // This is the regression case for the sticky-margin bug: stickyTop is clamped to 0 when budget=0,
        // so previously-HRTF channels don't get to keep HRTF status.
        t |> success(!hrtf_budget_classify(0, 0, true), "rank 0, wasHrtf -> SIM (clears in-flight)")
        t |> success(!hrtf_budget_classify(0, 0, false), "rank 0, !wasHrtf -> SIM")
        t |> success(!hrtf_budget_classify(5, 0, true), "rank 5, wasHrtf -> SIM")
        t |> success(!hrtf_budget_classify(99, 0, false), "rank 99, !wasHrtf -> SIM")
    }
}

[test]
def test_mixed_budget_routes_top_n(t : T?) {
    t |> run("budget=32: top-32 ranks HRTF, rest simulated (no sticky effect for fresh entries)") @(t : T?) {
        t |> success(hrtf_budget_classify(0, 32, false), "rank 0 below budget -> HRTF (flip in)")
        t |> success(hrtf_budget_classify(31, 32, false), "rank 31 below budget -> HRTF")
        t |> success(!hrtf_budget_classify(32, 32, false), "rank 32 == budget for !wasHrtf -> SIM")
        t |> success(!hrtf_budget_classify(100, 32, false), "rank 100 -> SIM")
    }
}

[test]
def test_sticky_margin_in_rank_space(t : T?) {
    t |> run("HRTF channels keep HRTF status across the budget+10% margin to avoid rank-swap flapping") @(t : T?) {
        // For budget=32: stickyTop = 32 + max(2, 32/10) = 32 + 3 = 35
        t |> success(hrtf_budget_classify(32, 32, true), "rank 32, wasHrtf -> stays HRTF (sticky)")
        t |> success(hrtf_budget_classify(34, 32, true), "rank 34, wasHrtf -> stays HRTF (sticky)")
        t |> success(!hrtf_budget_classify(35, 32, true), "rank 35, wasHrtf -> SIM (past stickyTop)")
        t |> success(!hrtf_budget_classify(36, 32, true), "rank 36, wasHrtf -> SIM")
        // Note: a channel at rank 33 that was simulated must NOT flip to HRTF — the budget is 32.
        t |> success(!hrtf_budget_classify(33, 32, false), "rank 33, !wasHrtf -> SIM (budget unchanged)")
    }
}

[test]
def test_sticky_margin_floor_at_small_budgets(t : T?) {
    t |> run("small budgets enforce a minimum margin of 2 so single-channel swaps don't immediately flip") @(t : T?) {
        // For budget=10: stickyTop = 10 + max(2, 1) = 10 + 2 = 12
        t |> success(hrtf_budget_classify(10, 10, true), "rank 10, wasHrtf -> stays HRTF")
        t |> success(hrtf_budget_classify(11, 10, true), "rank 11, wasHrtf -> stays HRTF")
        t |> success(!hrtf_budget_classify(12, 10, true), "rank 12 -> SIM (past min-margin top)")

        // For budget=1: stickyTop = 1 + max(2, 0) = 3
        t |> success(hrtf_budget_classify(0, 1, true), "budget=1: rank 0 stays HRTF")
        t |> success(hrtf_budget_classify(2, 1, true), "budget=1: rank 2 still sticky")
        t |> success(!hrtf_budget_classify(3, 1, true), "budget=1: rank 3 falls off")
    }
}
