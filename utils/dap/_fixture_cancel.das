options gen2

require jobque

[export]
def main() {
    new_debugger_thread() {
        panic("cancelled debugger worker ran")
    }
    print("DAP_CANCEL_READY\n")
}
