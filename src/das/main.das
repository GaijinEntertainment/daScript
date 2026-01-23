require daslib/aot_cpp
require daslib/aot_standalone

require fio
require strings

require daslib/rtti


def updateCOP(var cop : CodeOfPolicies; gen2 : bool; gen2_make : bool) {
    cop.threadlock_context = false
    cop.aot = false;
    cop.aot_module = true;
    cop.fail_on_lack_of_aot_export = true;
    cop.ignore_shared_modules = true;
    cop.version_2_syntax = gen2;
    cop.gen2_make_syntax = gen2_make;
}


def find_argument_or(args : array<string>; key : string, def_val : string) {
    for (i in range(length(args) - 1)) {
        if (args[i] == key) {
            return args[i + 1]
        }
    }
    return def_val
}

def find_argument(args : array<string>; key : string) {
    for (i in range(length(args) - 1)) {
        if (args[i] == key) {
            return args[i + 1]
        }
    }
    panic("expected {key} argument!")
    return ""
}

def get_list_of_files(args : array<string>, key : string) {
    let len = length(args)
    var result = array<tuple<string; string>>()
    for (i in range(len)) {
        if (args[i] == key) {
            if (i + 2 >= len) {
                panic("Arguments expected in format `-aot <in> <out>`.")
            }
            result |> push((args[i + 1], args[i + 2]))
        }
    }
    return <- result
}

def find_bool(args : array<string>; key : string) {
    let idx = find_index(args, key)
    return idx >= 0 && idx + 1 < length(args)
}

[export]
def main() {
    let args <- get_command_line_arguments()

    let cross_platform = find_bool(args, "-cross_platform");
    let gen1 = find_bool(args, "-gen1");
    let gen2_make = find_bool(args, "-gen2-make");
    let aot_files = get_list_of_files(args, "-aot")
    let ctx_files = get_list_of_files(args, "-standalone-context")
    using <| $(var cop : CodeOfPolicies) {
        updateCOP(cop, !gen1, gen2_make)
        for ((aot_in, aot_out) in aot_files) {
            print("Aot to {aot_out}\n")
            let res = aot(aot_in, true, false, cross_platform, cop)
            if (res |> length() > 0) {
                fopen(aot_out, "wb") <| $(fw) {
                    if (fw != null) {
                        fwrite(fw, res)
                    } else {
                        print("Couldn't create output file {aot_out}\n")
                    }
                }
            }
        }
        for ((ctx_in, ctx_out) in ctx_files) {
            print("Standalone ctx to {ctx_out}\n")
            let res = standalone_aot(ctx_in, ctx_out, false, cross_platform, false, cop)
            if (res |> length() > 0) {
                fopen(ctx_out, "wb") <| $(fw) {
                    if (fw != null) {
                        fwrite(fw, res)
                    } else {
                        print("Couldn't create output file {ctx_out}\n")
                    }
                }
            }
        }
    }
}
