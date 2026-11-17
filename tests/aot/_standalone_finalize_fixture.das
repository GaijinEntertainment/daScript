options gen2

// [finalize] functions, plain and late, with nothing that reads RTTI - the context runs them from its
// shutdown list, so they carry no function info

var g_bye = 0

[finalize]
def bye {
    g_bye++
}

[finalize(late = true)]
def bye_late {
    g_bye += 10
}

[export]
def count_bye : int {
    return g_bye
}
