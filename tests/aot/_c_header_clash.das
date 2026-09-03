options gen2

require daslib/export_c


[export_c]
def twin(x : int) : int {
    return x
}

[export_c(name = "twin")]
def twin_float(y : float) : float {
    return y
}

[export_c(name = "create")]
def reserved() {}
