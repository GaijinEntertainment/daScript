options gen2


def case_broken_survivor(a, b : int) : int {
    return a * b + a
}


def case_broken_offender(a : int) : int {
    let s : string = a
    return length(s)
}
