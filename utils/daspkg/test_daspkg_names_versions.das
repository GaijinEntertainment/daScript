options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/defer

require daslib/json_boost


require utils
require index
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "package_name_from_source takes the last path segment of a URL, local path or bare name, whatever the separators, and gives nothing for a trailing slash")]
def test_package_name_from_source(t : T?) {
    t |> run("github URL") @(t : T?) {
        t |> equal(package_name_from_source("github.com/user/dasImgui"), "dasImgui")
    }
    t |> run("nested path") @(t : T?) {
        t |> equal(package_name_from_source("github.com/org/sub/mymod"), "mymod")
    }
    t |> run("local absolute path") @(t : T?) {
        t |> equal(package_name_from_source("/home/user/mymod"), "mymod")
    }
    t |> run("bare name") @(t : T?) {
        t |> equal(package_name_from_source("dasImgui"), "dasImgui")
    }
    t |> run("trailing slash stripped") @(t : T?) {
        t |> equal(package_name_from_source("github.com/user/repo/"), "")
    }
    t |> run("windows backslash path") @(t : T?) {
        t |> equal(package_name_from_source("C:\\Users\\mymod"), "mymod")
    }
    t |> run("mixed separators") @(t : T?) {
        t |> equal(package_name_from_source("C:/Users\\mymod"), "mymod")
    }
}

[test(description = "is_local_path accepts absolute, dotted-relative and drive-letter paths with either separator and rejects URLs and bare names")]
def test_is_local_path(t : T?) {
    t |> run("absolute unix path") @(t : T?) {
        t |> success(is_local_path("/home/user/mod"))
    }
    t |> run("relative dot path") @(t : T?) {
        t |> success(is_local_path("./mymod"))
    }
    t |> run("relative parent path") @(t : T?) {
        t |> success(is_local_path("../mymod"))
    }
    t |> run("windows drive letter") @(t : T?) {
        t |> success(is_local_path("C:/Users/mod"))
    }
    t |> run("github URL is not local") @(t : T?) {
        t |> success(!is_local_path("github.com/user/repo"))
    }
    t |> run("bare name is not local") @(t : T?) {
        t |> success(!is_local_path("dasImgui"))
    }
    t |> run("windows backslash dot path") @(t : T?) {
        t |> success(is_local_path(".\\mymod"))
    }
    t |> run("windows backslash parent path") @(t : T?) {
        t |> success(is_local_path("..\\mymod"))
    }
}

[test(description = "is_index_name accepts only a bare name, never a URL or a path")]
def test_is_index_name(t : T?) {
    t |> run("bare name is index name") @(t : T?) {
        t |> success(is_index_name("dasImgui"))
    }
    t |> run("github URL is not index name") @(t : T?) {
        t |> success(!is_index_name("github.com/user/repo"))
    }
    t |> run("local path is not index name") @(t : T?) {
        t |> success(!is_index_name("./mymod"))
    }
    t |> run("backslash path is not index name") @(t : T?) {
        t |> success(!is_index_name("path\\to\\mod"))
    }
}

[test(description = "parse_version reads major.minor.patch with an optional v prefix and zero-fills missing parts, and rejects an invalid or empty string")]
def test_parse_version(t : T?) {
    t |> run("parses major.minor.patch") @(t : T?) {
        let v = parse_version("1.2.3")
        t |> success(v.valid)
        t |> equal(1, v.major)
        t |> equal(2, v.minor)
        t |> equal(3, v.patch)
    }
    t |> run("parses with v prefix") @(t : T?) {
        let v = parse_version("v1.2.3")
        t |> success(v.valid)
        t |> equal(1, v.major)
        t |> equal(2, v.minor)
        t |> equal(3, v.patch)
    }
    t |> run("parses major.minor") @(t : T?) {
        let v = parse_version("2.5")
        t |> success(v.valid)
        t |> equal(2, v.major)
        t |> equal(5, v.minor)
        t |> equal(0, v.patch)
    }
    t |> run("parses major only") @(t : T?) {
        let v = parse_version("3")
        t |> success(v.valid)
        t |> equal(3, v.major)
        t |> equal(0, v.minor)
        t |> equal(0, v.patch)
    }
    t |> run("invalid version") @(t : T?) {
        let v = parse_version("abc")
        t |> success(!v.valid)
    }
    t |> run("empty version") @(t : T?) {
        let v = parse_version("")
        t |> success(!v.valid)
    }
}

[test(description = "compare_versions orders versions by major, then minor, then patch")]
def test_compare_versions(t : T?) {
    t |> run("equal versions") @(t : T?) {
        let a = parse_version("1.2.3")
        let b = parse_version("1.2.3")
        t |> equal(0, compare_versions(a, b))
    }
    t |> run("major differs") @(t : T?) {
        let a = parse_version("1.0.0")
        let b = parse_version("2.0.0")
        t |> equal(-1, compare_versions(a, b))
        t |> equal(1, compare_versions(b, a))
    }
    t |> run("minor differs") @(t : T?) {
        let a = parse_version("1.2.0")
        let b = parse_version("1.3.0")
        t |> equal(-1, compare_versions(a, b))
    }
    t |> run("patch differs") @(t : T?) {
        let a = parse_version("1.2.3")
        let b = parse_version("1.2.4")
        t |> equal(-1, compare_versions(a, b))
    }
}

[test(description = "satisfies_constraint evaluates exact, comparison and comma-range constraints with optional v prefixes and rejects invalid versions or constraints")]
def test_satisfies_constraint(t : T?) {
    t |> run("empty constraint matches anything") @(t : T?) {
        t |> success(satisfies_constraint("1.0.0", ""))
        t |> success(satisfies_constraint("v99.0.0", ""))
    }
    t |> run("exact match") @(t : T?) {
        t |> success(satisfies_constraint("1.2.3", "1.2.3"))
        t |> success(!satisfies_constraint("1.2.4", "1.2.3"))
    }
    t |> run("exact with = prefix") @(t : T?) {
        t |> success(satisfies_constraint("1.2.3", "=1.2.3"))
        t |> success(!satisfies_constraint("1.2.4", "=1.2.3"))
    }
    t |> run("greater or equal") @(t : T?) {
        t |> success(satisfies_constraint("1.0.0", ">=1.0"))
        t |> success(satisfies_constraint("2.0.0", ">=1.0"))
        t |> success(!satisfies_constraint("0.9.0", ">=1.0"))
    }
    t |> run("greater than") @(t : T?) {
        t |> success(!satisfies_constraint("1.0.0", ">1.0"))
        t |> success(satisfies_constraint("1.0.1", ">1.0"))
    }
    t |> run("less or equal") @(t : T?) {
        t |> success(satisfies_constraint("1.0.0", "<=1.0"))
        t |> success(satisfies_constraint("0.9.0", "<=1.0"))
        t |> success(!satisfies_constraint("1.0.1", "<=1.0"))
    }
    t |> run("less than") @(t : T?) {
        t |> success(satisfies_constraint("0.9.0", "<1.0"))
        t |> success(!satisfies_constraint("1.0.0", "<1.0"))
    }
    t |> run("range with comma") @(t : T?) {
        t |> success(satisfies_constraint("1.5.0", ">=1.0,<2.0"))
        t |> success(satisfies_constraint("1.0.0", ">=1.0,<2.0"))
        t |> success(!satisfies_constraint("2.0.0", ">=1.0,<2.0"))
        t |> success(!satisfies_constraint("0.9.0", ">=1.0,<2.0"))
    }
    t |> run("v prefix on version") @(t : T?) {
        t |> success(satisfies_constraint("v1.2.3", ">=1.0"))
        t |> success(satisfies_constraint("v1.2.3", "1.2.3"))
    }
    t |> run("v prefix on constraint") @(t : T?) {
        t |> success(satisfies_constraint("1.2.3", ">=v1.0"))
        t |> success(satisfies_constraint("1.2.3", "v1.2.3"))
    }
    t |> run("invalid version fails") @(t : T?) {
        t |> success(!satisfies_constraint("abc", ">=1.0"))
    }
    t |> run("invalid constraint fails") @(t : T?) {
        t |> success(!satisfies_constraint("1.0.0", ">=abc"))
    }
    t |> run("spaces in range") @(t : T?) {
        t |> success(satisfies_constraint("1.5.0", ">= 1.0 , < 2.0"))
    }
}
