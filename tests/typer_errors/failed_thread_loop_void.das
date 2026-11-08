//! new_thread_loop with a void step block must not compile (30341),
//! because the step must return a bool.
options gen2
expect 30341
require daslib/jobque_boost
[export]
def main {
    new_thread_loop() @ { print("void step is invalid\n") }
}
