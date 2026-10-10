options gen2

require dastest/testing_boost public
require greeter/greeter

//! compiles only when the run registers this project's own modules/greeter
[test]
def test_project_module(t : T?) {
    t |> equal(greet(), "greeter=project module")
}
