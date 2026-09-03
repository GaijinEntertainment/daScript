options gen2

// Fixture for test_c_header_emit: two exports competing for one C symbol, and a reserved name.

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
