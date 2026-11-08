options gen2
require dastest/testing_boost public

[test(description = "assert(true) passes and the test continues")]
def test_assert_pass(t : T?) {
    t |> run("assert true works") @(t : T?) {
        assert(true)
        t |> success(true)
    }
}

[test(description = "assert(true, message) passes and the test continues")]
def test_assert_with_msg_pass(t : T?) {
    t |> run("assert with message works") @(t : T?) {
        assert(true, "this should not fail")
        t |> success(true)
    }
}

[test(description = "verify(true) passes and the test continues")]
def test_verify_pass(t : T?) {
    t |> run("verify true works") @(t : T?) {
        verify(true)
        t |> success(true)
    }
}

[test(description = "assert(false) panics and a recover block catches it")]
def test_assert_fail_is_caught(t : T?) {
    t |> run("assert false throws and is caught") @(t : T?) {
        var caught = false
        try {
            assert(false)
        } recover {
            caught = true
        }
        t |> equal(caught, true)
        t |> success(true)
    }
}

[test(description = "assert(false, message) panics and a recover block catches it")]
def test_assert_fail_msg_is_caught(t : T?) {
    t |> run("assertf false throws and is caught") @(t : T?) {
        var caught = false
        try {
            assert(false, "custom message")
        } recover {
            caught = true
        }
        t |> equal(caught, true)
        t |> success(true)
    }
}

[test(description = "A failing verify panics and the panic is catchable")]
def test_verify_fail_is_caught(t : T?) {
    t |> run("verify false throws and is caught") @(t : T?) {
        var caught = false
        try {
            verify(false)
        } recover {
            caught = true
        }
        t |> equal(caught, true)
        t |> success(true)
    }
}
