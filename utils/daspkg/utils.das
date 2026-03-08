options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module utils shared public

require fio
require strings

def run_cmd(cmd : string; var output : string&) : int {
    var exit_code = -1
    unsafe {
        exit_code = popen(cmd) <| $(f) {
            if (f != null) {
                output := fread(f)
            }
        }
    }
    return exit_code
}

def package_name_from_source(source : string) : string {
    // "github.com/user/dasImgui" -> "dasImgui"
    // "/path/to/mymod" -> "mymod"
    // "C:\path\to\mymod" -> "mymod"
    var last_sep = rfind(source, "/")
    let last_back = rfind(source, "\\")
    if (last_back > last_sep) {
        last_sep = last_back
    }
    if (last_sep >= 0) {
        return slice(source, last_sep + 1)
    }
    return source
}

def is_local_path(source : string) : bool {
    if (starts_with(source, "/") || starts_with(source, "./") || starts_with(source, "../")) {
        return true
    }
    if (starts_with(source, ".\\") || starts_with(source, "..\\")) {
        return true
    }
    if (length(source) > 2 && character_at(source, 1) == ':') {
        return true
    }
    return false
}

def copy_dir_rec(src, dst : string) : bool {
    mkdir(dst)
    var ok = true
    dir(src) <| $(name) {
        if (name == "." || name == "..") {
            return
        }
        let src_child = "{src}/{name}"
        let dst_child = "{dst}/{name}"
        var st = stat(src_child)
        if (st.is_dir) {
            if (!copy_dir_rec(src_child, dst_child)) {
                ok = false
            }
        } elif (st.is_reg) {
            fopen(src_child, "rb") <| $(fin) {
                if (fin != null) {
                    let data = fread(fin)
                    fopen(dst_child, "wb") <| $(fout) {
                        if (fout != null) {
                            fwrite(fout, data)
                        } else {
                            ok = false
                        }
                    }
                } else {
                    ok = false
                }
            }
        }
    }
    return ok
}
