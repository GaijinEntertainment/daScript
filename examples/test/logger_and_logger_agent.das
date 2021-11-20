require debugapi

class LogAgent : DapiDebugAgent
    def override onLog ( level:int; text:string# ) : bool
        print("onLog - {level} - '{text}'\n")
        return true // false for not consuming

[export]
def logger_agent ( ctx : Context )
    install_new_debug_agent(new LogAgent(), "log_example")

[export]
def main
    to_log(LOG_SAY,     "hello, world!")
    to_log(LOG_VERBOSE, "verbose")          // this one will not show, level is set to 'say'
    to_log(LOG_WARNING, "warning")
    to_log(LOG_ERROR,   "error")

    fork_debug_agent_context(@@logger_agent)

    to_log(LOG_SAY,     "one more time, hello, world!")
    to_log(LOG_VERBOSE, "one more time, verbose")
    to_log(LOG_WARNING, "one more time, warning")
    to_log(LOG_ERROR,   "one more time, error")
