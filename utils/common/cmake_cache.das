options gen2

module cmake_cache shared public

require strings
require daslib/strings_boost
require daslib/fio

//! The value the text of a `CMakeCache.txt` holds for `name`,
//! whatever its type; "" without the entry
def cmake_cache_value(cache_text, name : string) : string {
    let key = "{name}:"
    for (line in split(cache_text, "\n")) {
        if (starts_with(line, key)) {
            let eq = find(line, "=")
            return eq >= 0 ? strip(slice(line, eq + 1)) : ""
        }
    }
    return ""
}

//! The generator `build_dir` was configured with; "" if no cache
def cmake_build_generator(build_dir : string) : string {
    let cache = path_join(build_dir, "CMakeCache.txt")
    return fexist(cache) ? cmake_cache_value(fread(cache), "CMAKE_GENERATOR") : ""
}

//! Whether `cmake_generator` builds a folder per configuration
def is_multi_config_generator(cmake_generator : string) : bool =>
    (find(cmake_generator, "Visual Studio") >= 0 || find(cmake_generator, "Xcode") >= 0
        || find(cmake_generator, "Multi-Config") >= 0)
