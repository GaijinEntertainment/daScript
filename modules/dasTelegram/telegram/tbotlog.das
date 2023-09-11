options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module tbotlog shared private

require debugapi
require fio
require strings

class LoggerAgent : DapiDebugAgent
    flog : FILE const?
    def LoggerAgent
        let temp_dir = "{get_das_root()}/temp"
        mkdir(temp_dir)
        let date_time = "{get_clock()}" |> replace(":","_") |> replace(" ","_")
        let log_file = "{temp_dir}/bot.{date_time}.log"
        flog = fopen(log_file,"wb")
        if flog == null
            panic("Cannot open log file {log_file}")
        let bang = [[uint8 0xefu8; 0xbbu8; 0xbfu8]]
        fwrite(flog, bang)
        print("Telegram bot log file at {log_file}\n")
    def operator delete
        fclose(flog)
        flog = null
    def override onLog ( context : Context?; at : LineInfo const?; level:int; text:string# ) : bool
        let date_time = "{get_clock()}"
        if level >= LOG_ERROR
            print("{level}:{date_time}: {text}")
        fprint(flog, "{level}:{date_time}: {text}")
        fflush(flog)
        return true

def state_agent ( ctx : Context )
    install_new_debug_agent(new LoggerAgent(), "tbot_log")

[_macro]
def private auto_start_state()
    if is_compiling_macros_in_module("tbotlog") && !is_in_completion()
        if !is_in_debug_agent_creation()
            if !has_debug_agent_context("tbot_log")
                fork_debug_agent_context(@@state_agent)
