options gen2
options persistent_heap

require dastest/testing_boost public
require user_state
require daslib/fio

// Where the server and the CLI keep what they write: the config lookup's three places and their
// order, the save path, the config write into a directory that does not exist yet, and the logs
// directory of a standalone exe against a run from the source tree. Model-free.

def private scratch(t : T?; what : string) : string {
    var terr = ""
    let tmp = temp_directory(terr)
    let dir = path_join(empty(tmp) ? "." : tmp, "dasllama_user_state_{what}_{ref_time_ticks()}")
    t |> success(mkdir_rec(dir), "scratch {dir}")
    return dir
}

[test]
def test_config_lookup(t : T?) {
    t |> run("the working directory, then the home, then beside the program; none is empty") @(t : T?) {
        let root = scratch(t, "lookup")
        let cwd = path_join(root, "cwd")
        let home = path_join(root, "home")
        let program = path_join(root, "program")
        mkdir_rec(cwd)
        mkdir_rec(home)
        mkdir_rec(program)
        let prior = getcwd()
        t |> success(chdir(cwd), "into the working directory")
        t |> equal(discover_config(program, home), "", "no config anywhere")
        fwrite(path_join(program, "dasllama-server.toml"), "")
        t |> equal(discover_config(program, home), path_join(program, "dasllama-server.toml"), "beside the program")
        fwrite(path_join(home, "dasllama-server.toml"), "")
        t |> equal(discover_config(program, home), path_join(home, "dasllama-server.toml"), "the home outranks beside the program")
        fwrite(path_join(cwd, "dasllama-server.toml"), "")
        t |> equal(discover_config(program, home), "dasllama-server.toml", "the working directory outranks both")
        t |> success(chdir(prior), "back")
        rmdir_rec(root)
    }
}

[test]
def test_config_save(t : T?) {
    t |> run("the save path is in the home; a write makes the missing directories") @(t : T?) {
        t |> equal(config_save_path("H/.dasllama"), path_join("H/.dasllama", "dasllama-server.toml"))
        let root = scratch(t, "save")
        let path = path_join(root, "not/yet/there/dasllama-server.toml")
        t |> success(write_config(path, "model = \"m.gguf\"\n"), "written")
        t |> equal(fread(path), "model = \"m.gguf\"\n")
        rmdir_rec(root)
    }
}

[test]
def test_logs_dir(t : T?) {
    t |> run("a standalone exe logs in the home, a run from the source tree in the das root") @(t : T?) {
        t |> equal(logs_dir(true, "H/.dasllama"), path_join("H/.dasllama", "logs"))
        t |> equal(logs_dir(false, "H/.dasllama"), path_join(get_das_root(), "logs"))
    }
}
