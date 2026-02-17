options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require fio
require strings
require daslib/das_source_formatter_fio


def print_usage() {
    print("daslang Code Formatter\n")
    print("Usage: daslang.exe utils/dasCodeFormatter/main.das -- [options] <file1.das> [file2.das ...]\n\n")
    print("Options:\n")
    print("  --files:<path>   Read list of files to format from a text file (one per line)\n")
    print("  --help           Show this help message\n\n")
    print("Examples:\n")
    print("  daslang.exe utils/dasCodeFormatter/main.das -- my_script.das\n")
    print("  daslang.exe utils/dasCodeFormatter/main.das -- src/a.das src/b.das\n")
    print("  daslang.exe utils/dasCodeFormatter/main.das -- --files:file_list.txt\n")
}


[export]
def main() {
    var args <- get_command_line_arguments()
    var fileNames : array<string>
    var pastSeparator = false

    for (arg in args) {
        if (arg == "--") {
            pastSeparator = true
            continue
        }
        if (!pastSeparator) {
            continue
        }
        if (arg == "--help" || arg == "-h") {
            print_usage()
            return
        }
        if (starts_with(arg, "--files:")) {
            let listFile = slice(arg, 8)
            fopen(listFile, "rb") <| $(f) {
                if (f == null) {
                    print("Error: cannot open file list '{listFile}'\n")
                } else {
                    while (!feof(f)) {
                        var line = fgets(f)
                        if (!empty(line)) {
                            line = replace(line, "\n", "")
                            line = replace(line, "\r", "")
                            if (!empty(line)) {
                                fileNames |> push(line)
                            }
                        }
                    }
                }
            }
            continue
        }
        if (character_at(arg, 0) != '-') {
            fileNames |> push(arg)
        }
    }

    if (empty(fileNames)) {
        print("Error: no input files specified.\n\n")
        print_usage()
        return
    }

    format_files(fileNames)
}
