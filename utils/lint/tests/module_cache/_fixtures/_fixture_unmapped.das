options gen2

require daslib/fio

typedef module_info = tuple<string; string; string> const

[export]
def module_get(req, from : string) : module_info {
    if (req == "hidden_mod") {
        let root = dir_name(dir_name(from))
        return ("hidden_mod", path_join(root, "dep/missing.das"), "")
    }
    return ("", "", "")
}
