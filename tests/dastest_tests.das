
require dastest/testing_boost
require dastest/suite
require debugapi


def file_ctx()
    var ctx <- [[SuiteFileCtx uriPaths=true]]
    unsafe
        ctx.context = addr(this_context())
    return <- ctx


[test]
def test_deref(t: T?)
    var ctx <- file_ctx()
    let res = test_func("deref null", ctx) <| @@(tt: T?)
        var i: int?
        *i = 0

    equal(t, res, [[SuiteResult total=1, errors=1]])


[test]
def test_division_by_zero(t: T?)
    var ctx <- file_ctx()
    let res = test_func("division by zero", ctx) <| @@(tt: T?)
        var div = 0
        tt->log("{1 / div}")

    equal(t, res, [[SuiteResult total=1, errors=1]])


[test]
def success_test(t: T?)
    var ctx <- file_ctx()
    let res = test_func("success test", ctx) <| @@(tt: T?)
        tt->log("success")

    equal(t, res, [[SuiteResult total=1, passed=1]])


[test]
def skipped_test(t: T?)
    var ctx <- file_ctx()
    let res = test_func("skipped test", ctx) <| @@(tt: T?)
        tt->skip("skip now")
        tt->error("this error should be ignored")

    equal(t, res, [[SuiteResult total=1, skipped=1]])


[test]
def failed_test(t: T?)
    var ctx <- file_ctx()
    let res = test_func("failed test", ctx) <| @@(tt: T?)
        tt->fail()

    equal(t, res, [[SuiteResult total=1, failed=1]])


[test]
def failed_now_test(t: T?)
    var ctx <- file_ctx()
    let res = test_func("fail now test", ctx) <| @@(tt: T?)
        tt->failNow()
        panic("this panic should be skipped")

    equal(t, res, [[SuiteResult total=1, failed=1]])
