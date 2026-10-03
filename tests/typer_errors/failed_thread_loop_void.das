options gen2
expect 30341
require daslib/jobque_boost
[export]
def main {
    new_thread_loop() @ { print("void step is invalid\n") }
}
