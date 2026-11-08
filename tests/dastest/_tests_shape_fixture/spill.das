options gen2

require dastest/testing_boost public

//! three tests, one of whose annotations spills over two lines

[test(description = "the first of three")]
def test_s1(t : T?) {
    t |> equal(1, 1)
}

[test(description = "an annotation that spills",
      timeout = 3)]
def test_s2(t : T?) {
    t |> equal(1, 1)
}

[test(description = "the third of three")]
def test_s3(t : T?) {
    t |> equal(1, 1)
}
