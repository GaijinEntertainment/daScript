//! a table literal keyed by a float16 constant does not compile:
//! float16 is not a table key type
options gen2
expect 30254:2, 30814, 30820

[export]
def main {
    return {0h => 1}
}
