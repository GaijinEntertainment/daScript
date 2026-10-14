options gen2

variant Tags {
    nada
    anything : int
}

[export, never_inline]
def bare() : Tags {
    return Tags(nada)
}

[export, never_inline]
def valued() : Tags {
    return Tags(anything = 7)
}
