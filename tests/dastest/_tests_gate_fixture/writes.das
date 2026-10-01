options gen2
options no_unused_function_arguments = false

require daslib/fio

def fixture_path(name : string) : string {
    return "{get_das_root()}/tests/{name}"
}

let GLOBAL_OUT = "{get_das_root()}/build/probe"

struct Server {
    upload_dir : string
}

class Image {
    def save(path : string) : bool {
        return !empty(path)
    }
}

def writes(var server : Server; var img : Image) {
    let root = get_das_root()
    let out = root + "/x.tmp"
    fopen(out, "wb") $(f) {
        fwrite(f, "a")
    }
    fopen(fixture_path("y.tmp"), "w") $(f) {
        fwrite(f, "a")
    }
    fopen("relative.tmp", "ab") $(f) {
        fwrite(f, "a")
    }
    mkdir_rec(GLOBAL_OUT)
    remove(fixture_path("y.tmp"))
    rename(out, path_join(root, "renamed.tmp"))
    var err : string
    copy_file(out, path_join(root, "copied.tmp"), true, err)
    fwrite(path_join(get_das_root(), "written.tmp"), "a")
    server.upload_dir = "{get_das_root()}/uploads"
    mkdir_rec(server.upload_dir)
    img.save(fixture_path("image.png"))
}
