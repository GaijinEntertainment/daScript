options gen2


// Non-generic baseline — must be visible to find_dupes.
def case_mono_baseline(a, b : int) : int {
    return a + b
}


// Generic template that IS called — the reified instance lands in
// `module->functions`, so the current pipeline sees it (one record
// after the per-source-line dedup in pipeline.das).
def case_generic_called(a, b : auto(T)) : T {
    return a + b
}


// Generic template that is NEVER called — stays in `module->generics`
// only. `for_each_function` (used by find_dupes) does not iterate that
// list, so this function is currently invisible to the tool.
def case_generic_uncalled(a, b : auto(T)) : T {
    return a + b
}


// Drive a reification of `case_generic_called` so the called-generic
// case is meaningfully exercised. Without this, even the called path
// would miss it.
def anchor_call_site() : int {
    return case_generic_called(1, 2)
}
