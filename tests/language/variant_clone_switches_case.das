options gen2

require dastest/testing_boost public

struct WithArr {
    a : array<int>
}

variant BigVariant {
    big : int4[4]
    w : WithArr
}

def filled_big() : BigVariant => (BigVariant(
    big = fixed_array(int4(-1), int4(-1), int4(-1), int4(-1))))

[test]
def test_clone_into_a_case_whose_bytes_another_case_wrote(t : T?) {
    var src : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    var dst : BigVariant <- filled_big()
    dst := src
    (src as w).a[0] = 100
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[0], 1)
}

[test]
def test_clone_back_into_a_value_case(t : T?) {
    let src = filled_big()
    var dst : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    dst := src
    t |> success(dst is big)
    t |> equal((dst as big)[3], int4(-1))
}

[test]
def test_clone_into_the_same_case(t : T?) {
    var src : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    var dst : BigVariant <- BigVariant(w = WithArr(a <- [7]))
    dst := src
    t |> success(dst is w)
    t |> equal(length((dst as w).a), 3)
    t |> equal((dst as w).a[2], 3)
}

[test]
def test_clone_into_itself(t : T?) {
    var v : BigVariant <- BigVariant(w = WithArr(a <- [1, 2, 3]))
    v := v
    t |> success(v is w)
    t |> equal(length((v as w).a), 3)
    t |> equal((v as w).a[2], 3)
}
