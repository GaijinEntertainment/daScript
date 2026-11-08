options gen2

require dastest/testing_boost public
require requirefix
require daslib/fio

def testdata_path(name : string) : string => "utils/internal/requirefix/testdata/{name}"

def run_analysis(target : string) : ModuleAnalysisResult {
    var config = RequirefixConfig(
        target = target,
        abs_filename_fn <- @(filename : string) : string {
            return get_full_file_name(filename)
        },
        read_file_fn <- @(filename : string) : string {
            let f = fopen(filename, "rb")     // text mode + fread breaks on CRLF checkouts
            if (f == null) return ""
            let s = fread(f)
            fclose(f)
            return s
        },
    )
    return <- analyze_module_usages(config)
}

def find_module_tag(var result : ModuleAnalysisResult; name : string) : string {
    for (mod in result.modules) {
        if (mod.name == name) return mod.tag
    }
    return ""
}

def find_module_refs(var result : ModuleAnalysisResult; name : string) : array<string> {
    for (mod in result.modules) {
        if (mod.name == name) {
            var inscope refs : array<string>
            reserve(refs, length(mod.refs))
            for (ref in mod.refs) {
                refs |> push("{ref.kind}/{ref.key}:{ref.line}")
            }
            return <- refs
        }
    }
    return <- default<array<string>>
}

def find_module_is_public(var result : ModuleAnalysisResult; name : string) : bool {
    for (mod in result.modules) {
        if (mod.name == name) return mod.is_public
    }
    return false
}

[test(description = "a required module nothing uses is tagged unused with no references")]
def test_unused_require(t : T?) {
    t |> run("unused_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_unused_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math"), "unused")
        t |> equal(length(find_module_refs(result, "math")), 0)
    }
}

[test(description = "a module whose function is called is tagged used:referenced with the call's line")]
def test_used_func_require(t : T?) {
    t |> run("used_func_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_used_func_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math"), "used:referenced")
        t |> equal(find_module_refs(result, "math"), ["function_call/sin:7"])
    }
}

[test(description = "a require marked public is reported as a public re-export")]
def test_public_require(t : T?) {
    t |> run("public_reexport_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_public_1.das"))
        t |> equal(result.err, "")
        t |> success(find_module_is_public(result, "strings"))
    }
}

[test(description = "in one file an unused require is tagged unused while a used one carries its call reference")]
def test_mixed_requires(t : T?) {
    t |> run("mixed_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_mixed_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math"), "unused")
        t |> equal(find_module_tag(result, "strings"), "used:referenced")
        t |> equal(find_module_refs(result, "strings"), ["function_call/to_int:8"])
    }
}

[test(description = "a module whose global variable is read is tagged used:referenced with a variable reference")]
def test_used_var_require(t : T?) {
    t |> run("used_var_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_used_var_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math"), "used:referenced")
        t |> equal(find_module_refs(result, "math"), ["variable/PI:10"])
    }
}

[test(description = "a module whose enum value is used is tagged used:referenced with enum-type references")]
def test_used_enum_require(t : T?) {
    t |> run("used_enum_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_used_enum_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "strings"), "used:referenced")
        t |> equal(find_module_refs(result, "strings"), [
            "type_enum/ConversionResult:9",
            "type_enum/ConversionResult:9",
        ])
    }
}

[test(description = "a module whose struct is used by value, by pointer or inside an array is tagged used:referenced with a reference per use")]
def test_used_struct_require(t : T?) {
    t |> run("used_struct_1") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_used_struct_1.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math_boost"), "used:referenced")
        t |> equal(find_module_refs(result, "math_boost"), [
            "type_struct/AABB:5",
            "type_struct/AABB:7",
            "type_struct/AABB:11",
            "type_struct/AABB:13",
        ])
    }

    t |> run("used_struct_2") @(t : T?) {
        var result = run_analysis(testdata_path("_fixture_used_struct_2.das"))
        t |> equal(result.err, "")
        t |> equal(find_module_tag(result, "math_boost"), "used:referenced")
        t |> equal(find_module_refs(result, "math_boost"), [
            "type_struct/AABB:5",
            "type_struct/AABB:9",
        ])
    }
}

[test(description = "an unreadable target sets the error and reports no modules")]
def test_err_missing_file(t : T?) {
    t |> run("nonexistent_file") <| @(t : T?) {
        var config = RequirefixConfig(
            target = "nonexistent_requirefix_test_file.das",
            abs_filename_fn <- @(filename : string) : string {
                return get_full_file_name(filename)
            },
            read_file_fn <- @(filename : string) : string {
                return ""
            },
        )
        let result = analyze_module_usages(config)
        t |> success(result.err != "", "should set error for missing file")
        t |> equal(length(result.modules), 0)
    }
}
