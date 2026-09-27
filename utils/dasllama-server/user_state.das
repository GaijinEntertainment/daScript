options gen2
options indenting = 4
options _dasllama_internal = true   // dasllama_home_dir rides the env rail, as model_catalog's models dir does

module user_state shared public

require dasllama/dasllama_env   // dasllama_home_dir
require dashv/dashv_boost       // hv_set_log_file
require daslib/logger
require daslib/fio

//! Where the server and the CLI keep what they write, so an install directory can stay read-only:
//! everything under the per-user `~/.dasllama` (`dasllama_home_dir`).

let CONFIG_NAME = "dasllama-server.toml"

//! The config a program reads with no `--config`: in the working directory, else in `home`
//! (`~/.dasllama`), else beside the program (`program_dir`); "" when none exists.
def discover_config(program_dir, home : string) : string {
    return dasllama_find_config(CONFIG_NAME, home, program_dir)
}

def discover_config(program_dir : string) : string {
    return discover_config(program_dir, dasllama_home_dir())
}

//! The config the control page saves when no `--config` named one, under `home`.
def config_save_path(home : string) : string {
    return path_join(home, CONFIG_NAME)
}

def config_save_path() : string {
    return config_save_path(dasllama_home_dir())
}

//! Write a config's text to `path`, making its directory first; false when it could not.
def write_config(path, text : string) : bool {
    mkdir_rec(dir_name(path))
    return fwrite(path, text)
}

//! Where a program's logs go: a standalone exe logs under `<dasllama home>/logs`, a JIT run from
//! the SDK under the das root's `logs/`, as every daslang tool does.
def logs_dir(standalone : bool; dasllama_home : string) : string {
    return standalone ? path_join(dasllama_home, "logs") : path_join(get_das_root(), "logs")
}

//! Open `tool`'s log (teed to the console when `tee`) in `logs_dir`, created when missing; a
//! standalone exe also moves libhv's daily log there from the working directory. Returns the
//! log's path.
def open_logs(tool : string; tee : bool) : string {
    let standalone = is_standalone_exe()
    let dir = logs_dir(standalone, dasllama_home_dir())
    mkdir_rec(dir)
    if (tee) {
        logger_init_tee(tool)
    } else {
        logger_init(tool)
    }
    if (standalone) {
        logger_set_path(path_join(dir, "{tool}.log"))
        hv_set_log_file(path_join(dir, "libhv"))
    }
    return logger_get_path()
}
