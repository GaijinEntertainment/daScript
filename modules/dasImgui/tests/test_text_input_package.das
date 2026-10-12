options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/fio
require daslib/rtti
require daslib/json_boost
require daslib/sha_256

[test]
def relocated_text_input_package_starts_with_bundled_fonts(t : T?) {
    if (!has_module("llvm")) { t |> skip("native package compilation requires dasLLVM"); return }
    let made = create_temp_directory_result("imgui_text_input_package")
    if (!(made is value)) { t |> failure("could not create release directory"); return }
    let temporary = made as value
    {
        let root = path_join(get_das_root(), "modules/dasImgui/examples/features/text_input")
        let output = path_join(temporary, "built")
        var log : string
        let built = run_and_capture([get_executable_path(), "-no-module-cache", path_join(get_das_root(), "utils/daspkg/main.das"), "--", "release", "--root", root, "--out", output], log, 180.0)
        t |> equal(built, 0, log)
        if (built != 0) return
        let bundle = path_join(output, get_platform_name() == "darwin" ? "text_input.app" : "text_input")
        let relocated = path_join(temporary, "relocated")
        t |> success(rename(bundle, relocated), "release can be moved away from its output path")
        let artifacts = get_platform_name() == "darwin" ? path_join(relocated, "Contents/MacOS") : relocated
        var error : string
        var manifest = read_json(fread(path_join(artifacts, "fonts/manifest.json")), error)
        t |> success(empty(error), error)
        {
            for (font in (manifest?["fonts"].value as _array)) {
                let file = font?["file"] ?? ""
                t |> success(!empty(file))
                t |> equal(sha256_file_hex(path_join(artifacts, "fonts/{file}")), font?["sha256"] ?? "", "bundled font bytes are unchanged")
                for (license in (font?["license_files"].value as _array)) {
                    let file_name = license ?? ""
                    t |> equal(fread(path_join(artifacts, "fonts/{file_name}")), fread(path_join(root, "fonts/{file_name}")), "bundled license is unchanged")
                }
            }
        } finally { delete_json(manifest) }
        let executable = path_join(artifacts, get_platform_name() == "darwin" ? "text_input" : "text_input.exe")
        let ran = run_and_capture([executable, "--", "--headless", "--headless-frames=5", "--no-imgui-ini"], log, 30.0)
        t |> equal(ran, 0, "relocated native input example starts and exits cleanly: {log}")
    } finally { rmdir_rec_result(temporary) }
}
