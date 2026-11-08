options gen2

require dastest/testing_boost public

//! a one-test file that declares why it cannot share a program: the
//! gate accepts it

[test(description = "a program-wide registry filled here would collide with any other test's", standalone = "fills a program-wide registry")]
def test_alone(t : T?) {
    t |> equal(1, 1)
}
