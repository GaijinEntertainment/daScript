options gen2
require dastest/testing_boost public

var step : int = 4

def through(n : int) : int => n <= 0 ? 1 : looks_pure(n - 1) + step

def looks_pure(n : int) : int => n <= 0 ? 2 : through(n - 1) * 2

var depth_unit : int = 3

def ring_a(n : int) : int => n <= 0 ? 0 : ring_b(n - 1) + 1

def ring_b(n : int) : int => n <= 0 ? 0 : ring_c(n - 1) + 1

def ring_c(n : int) : int => n <= 0 ? depth_unit : ring_a(n - 1) + 1

[test]
def test_a_constant_call_into_mutual_recursion_reading_a_global(t : T?) {
    t |> equal(looks_pure(2), 12)
    t |> equal(through(1), 6)
}

[test]
def test_a_constant_call_into_a_three_function_cycle_reading_a_global(t : T?) {
    t |> equal(ring_a(5), 8)
    t |> equal(ring_b(4), 7)
    t |> equal(ring_c(3), 6)
}
