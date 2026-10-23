options gen2

require daslib/fio
require strings

typedef module_info = tuple<string; string; string> const

var private _logged = false

[export]
def module_get(req, from : string) : module_info {
    if (req == "hidden_mod") {
        let root = dir_name(dir_name(from))
        let patched = find(fread(from), "=> hidden_value()") >= 0
        let rejection_file = path_join(root, "reject_fix")
        if (fexist(rejection_file)) {
            if (!_logged) {
                _logged = true
                fopen(path_join(root, "compile_log"), "ab") $(f) {
                    let entry = patched ? "patched\n" : "original\n"
                    fwrite(f, entry) if (f != null)
                }
            }
            if (patched) {
                let dependency = (fread(rejection_file) == "missing"
                    ? "dep/missing.das" : "dep/invalid.das")
                return ("hidden_mod", path_join(root, dependency), "")
            }
        }
        return ("hidden_mod", path_join(root, "dep/hidden.das"), "")
    }
    if (req == "public_api") {
        return ("z_impl", path_join(dir_name(from), "z_impl.das"),
            "public_api")
    }
    return ("", "", "")
}
