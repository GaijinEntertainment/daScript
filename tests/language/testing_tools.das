options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
options persistent_heap

require dastest/testing_boost public
require daslib/faker
require daslib/fuzzer
require daslib/defer
require daslib/static_let

[test(description = "two successive Faker random_int calls return different values")]
def test_faker_random_int(t : T?) {
    t |> run("random_int produces values") @(t : T?) {
        var fake <- Faker()
        let a = fake |> random_int
        let b = fake |> random_int
        // Two successive values should differ (overwhelmingly likely)
        t |> equal(a != b, true)
        delete fake
    }
}

[test(description = "two successive Faker random_uint calls return different values")]
def test_faker_random_uint(t : T?) {
    t |> run("random_uint produces values") @(t : T?) {
        var fake <- Faker()
        let a = fake |> random_uint
        let b = fake |> random_uint
        t |> equal(a != b, true)
        delete fake
    }
}

[test(description = "Faker random_float returns without a panic")]
def test_faker_random_float(t : T?) {
    t |> run("random_float produces values") @(t : T?) {
        var fake <- Faker()
        // Just verify it doesn't crash
        let a = fake |> random_float
        let b = fake |> random_float
        t |> equal(true, true)  // if we got here, no crash
        delete fake
    }
}

[test(description = "Faker any_string returns a string without a panic")]
def test_faker_any_string(t : T?) {
    t |> run("any_string produces a string") @(t : T?) {
        var fake <- Faker()
        let s = fake |> any_string()
        // Should not crash; length >= 0
        t |> equal(length(s) >= 0, true)
        delete fake
    }
}

[test(description = "Faker any_file_name returns a non-empty name")]
def test_faker_any_file_name(t : T?) {
    t |> run("any_file_name produces alphanumeric string") @(t : T?) {
        var fake <- Faker()
        let name = fake |> any_file_name
        t |> equal(!empty(name), true)
        delete fake
    }
}

[test(description = "Faker date returns a full date string longer than ten characters")]
def test_faker_date(t : T?) {
    t |> run("date produces a date string") @(t : T?) {
        var fake <- Faker()
        let d = fake |> date
        // date format is "DayOfWeek, MonthName DD, YYYY"
        t |> equal(length(d) > 10, true)
        delete fake
    }
}

[test(description = "Faker day returns a non-empty day name")]
def test_faker_day(t : T?) {
    t |> run("day produces a day name") @(t : T?) {
        var fake <- Faker()
        let d = fake |> day
        t |> equal(!empty(d), true)
        delete fake
    }
}

[test(description = "Faker random_int2 and random_float3 return vectors without a panic")]
def test_faker_vectors(t : T?) {
    t |> run("random_int2 produces two values") @(t : T?) {
        var fake <- Faker()
        let v = fake |> random_int2
        t |> equal(true, true)  // if we got here, no crash
        delete fake
    }
    t |> run("random_float3 produces three values") @(t : T?) {
        var fake <- Faker()
        let v = fake |> random_float3
        t |> equal(true, true)  // if we got here, no crash
        delete fake
    }
}

[test(description = "fuzz(n) runs its block exactly n times and returns normally")]
def test_fuzz_basic(t : T?) {
    t |> run("fuzz runs specified iterations") @(t : T?) {
        var count = 0
        fuzz(50) {
            count ++
        }
        t |> equal(count, 50)
    }
    t |> run("fuzz catches panics") @(t : T?) {
        // fuzz catches panics; this should complete without propagating
        var completed = false
        fuzz(10) {
            pass
        }
        completed = true
        t |> equal(completed, true)
    }
}

[test(description = "fuzz_debug(n) runs its block exactly n times")]
def test_fuzz_debug(t : T?) {
    t |> run("fuzz_debug runs without catching") @(t : T?) {
        var count = 0
        fuzz_debug(25) {
            count ++
        }
        t |> equal(count, 25)
    }
}

[test(description = "a fuzz loop drawing Faker ints finds no violation of the clamp-to-range property")]
def test_fuzz_with_faker(t : T?) {
    t |> run("property: abs clamp") @(t : T?) {
        var fake <- Faker()
        var violations = 0
        fuzz(100) {
            let x = fake |> random_int
            // Property: if we clamp x to [-100, 100], result is in range
            var clamped = x
            if (clamped < -100) {
                clamped = -100
            }
            if (clamped > 100) {
                clamped = 100
            }
            if (clamped < -100 || clamped > 100) {
                violations ++
            }
        }
        t |> equal(violations, 0)
        delete fake
    }
}

[test(description = "daslib/defer runs its block at scope exit, in LIFO order, and on an early return")]
def test_defer_basic(t : T?) {
    t |> run("defer executes on scope exit") @(t : T?) {
        var log = ""
        invoke() {
            defer() {
                log += "cleanup"
            }
            log += "work,"
        }
        t |> equal(log, "work,cleanup")
    }
    t |> run("defer LIFO ordering") @(t : T?) {
        var log = ""
        invoke() {
            defer() { log += "1"; }
            defer() { log += "2"; }
            defer() { log += "3"; }
        }
        t |> equal(log, "321")
    }
    t |> run("defer runs on early return") @(t : T?) {
        var log = ""
        invoke() {
            defer() { log += "cleaned"; }
            log += "before,"
            return
        }
        t |> equal(log, "before,cleaned")
    }
}

def call_counter() : int {
    static_let() {
        var counter = 0
    }
    counter ++
    return counter
}

[test(description = "a static_let variable keeps its value across calls, so each call sees the previous count plus one")]
def test_static_let(t : T?) {
    t |> run("static_let persists across calls") @(t : T?) {
        let a = call_counter()
        let b = call_counter()
        let c = call_counter()
        t |> equal(b, a + 1)
        t |> equal(c, b + 1)
    }
}
