require daslib/strings_boost
require fio
require rtti
require debugapi
require math

var LOG_TEST_COMPILATION_TIME = false

var failed = 0

def compile_and_run ( fileName:string; useAot:bool; useJit:bool )
    var t0 = ref_time_ticks()
    var inscope access <- make_file_access("")
    using <| $(var mg:ModuleGroup)
        using <| $(var cop:CodeOfPolicies)
            cop.aot_module = true
            cop.threadlock_context = true
            if useJit
                cop.jit = jit_enabled()
                cop.jit_module := "{get_das_root()}/daslib/just_in_time.das"
            elif useAot
                cop.aot = true
            else
                cop.jit = false
                cop.aot = false
            compile_file(fileName,access,unsafe(addr(mg)),cop) <| $(ok,program,issues)
                if !ok
                    print("failed to compile {fileName}\n{issues}\n")
                    failed ++
                    return
                simulate(program) <| $ ( sok; context; serrors ) // no aot for now
                    if !sok
                        print("failed to simulate {fileName}\n{serrors}\n")
                        failed ++
                        return
                    var dt = get_time_usec(t0)
                    var sec = double(dt)/1000000.0lf
                    if LOG_TEST_COMPILATION_TIME
                        print("compiled in {sec} sec\n")
                    try
                        unsafe(invoke_in_context(context,"main"))
                    recover
                        let ctx = unsafe(reinterpret<Context?> context)
                        print("failed to run {fileName}\n{ctx.exceptionAt}: {ctx.exception}\n")
                        failed ++


def test_single_file ( fileName:string )
    // print("{fileName}\n")
    /*
    if!(fileName |> ends_with("dict.das") || fileName |> ends_with("exp.das"))
        return
    */
    print("\"DAS AOT\", ")
    compile_and_run(fileName, true, false)
    print("\"DAS JIT\", ")
    compile_and_run(fileName, false, true)
    print("\"DAS INTERPRETER\", ")
    compile_and_run(fileName, false, false)
    print("\n\n")

def run_dir ( appDir : string )
    dir(appDir) <| $ ( fileName )
        if (!fileName |> starts_with("_")) && fileName |> ends_with(".das")
            let appfile = "{appDir}/{fileName}"
            test_single_file(appfile)

struct ProfileEntry
    language : string
    category : string
    time     : double
    count    : int

enum ParseProfileEntryError
    Ok
    InvalidInput
    InvalidLanguage
    InvalidCategory
    InvalidTime
    InvalidCount

[no_jit,no_aot]
def parse_profile_entry ( invocation:string; blk : block<(res:ProfileEntry):void> ) : ParseProfileEntryError
    var sub <- invocation |> split(",")
    if length(sub)!=4
        return ParseProfileEntryError InvalidInput
    for s in sub
        s = s |> trim
    var language = sub[0]
    if language |> starts_with("\"") && language |> ends_with("\"")
        language = language |> slice(1, -1)
    else
        return ParseProfileEntryError InvalidLanguage
    var category = sub[1]
    if category |> starts_with("\"") && category |> ends_with("\"")
        category = category |> slice(1, -1)
    else
        return ParseProfileEntryError InvalidCategory
    var time = DBL_MAX
    try
        time = double(sub[2])
    recover
        pass
    if time == DBL_MAX
        return ParseProfileEntryError InvalidTime
    var count = INT_MAX
    try
        count = int(sub[3])
    recover
        pass
    if count == INT_MAX
        return ParseProfileEntryError InvalidCount
    blk |> invoke([[ProfileEntry language=language, category=category, time=time, count=count]])
    return ParseProfileEntryError Ok

def make_table ( tablename:string; entries:array<ProfileEntry>; languages:table<string> )
    var categories : table<string; double>
    for e in entries
        if languages |> key_exists(e.language)
            if categories |> key_exists(e.category)
                categories[e.category] = min(e.time, categories[e.category])
            else
                categories[e.category] = e.time
    var slanguages <- [{for l in keys(languages); l}]
    slanguages |> sort
    var scategories <- [{for c in keys(categories); c}]
    scategories |> sort
    print("<h2>{tablename}</h2>\n")
    print("<table border=\"1\">\n")
    print("<tr><th>Language</th>\n")
    for c in scategories
        print("<th>{c}<br>times slower</th>")
    print("</tr>\n")
    for l in slanguages
        // collect times for each category given language
        var cattime : table<string; double>
        for e in entries
            if e.language == l
                cattime[e.category] = e.time
        // print category times of specific language
        print("<tr><td>{l}</td>\n")
        for c in scategories
            if cattime |> key_exists(c)
                let time = cattime[c]
                let mintime = categories[c]
                let factor = time / mintime
                let ts = format("%.4f", float(time))
                let fs = format("%.2f", float(factor))
                if time == mintime
                    print("<td><b>{fs} ({ts})</b></td>")
                else
                    print("<td>{fs} ({ts})</td>")
            else
                print("<td>0</td>")
        print("</tr>\n")
    print("</table>\n")

def make_html
    let exe = get_command_line_arguments()[0]
    let cmdLine = "{exe}"
    var entries : array<ProfileEntry>
    unsafe
        var exitCode = popen(cmdLine) <| $ ( f )
            while !feof(f)
                let st = fgets(f)
                let err = parse_profile_entry(st) <| $ ( res )
                    entries |> push(res)
                    print("{res}\n")
                if err != ParseProfileEntryError Ok
                    print(st)
        if exitCode != 0
            print("Error: {exitCode}\n")
            return
    print("<!DOCTYPE html><html>\n")
    print("<body>\n")
    make_table("interpreted", entries, {{
        "DAS INTERPRETER";
        "MONO --interpreter";
        "LUAU";
        "LUA";
        "LUAJIT -joff";
        "SQUIRREL3";
        "QUIRREL";
        "QUICKJS";
    }})
    make_table("AOT or JIT", entries, {{
        "DAS AOT";
        "DAS JIT";
        "C++";
        "MONO";
        ".NET";
        "LUAJIT";
        "LUAU --codegen";
    }})
    print("</body>\n")
    print("</html>\n")

[export]
def main
    var singleTest = ""
    var mainTests = true
    var extraTests = false
    var hashTests = false
    print("{get_command_line_arguments()}\n")
    let nArgs = get_command_line_arguments() |> length()
    var i = 0
    while i < nArgs
        let arg = get_command_line_arguments()[i]
        if arg=="-test" && i < nArgs-1
            singleTest = get_command_line_arguments()[i+1]
            i += 2
            continue
        elif arg=="-html"
            make_html()
            return
        elif arg=="-log"
            LOG_TEST_COMPILATION_TIME = true
        elif arg=="-extra"
            extraTests = true
        elif arg=="-hash"
            hashTests = true
        elif arg=="-nomain"
            mainTests = false
        elif arg=="-help"
            print("Usage: profile_tests [-test <testname>] [-log] [-extra] [-hash]\n")
            print("  -test testname  runs /examples/profile/tests/test.das and exits immediately\n")
            print("  -log            logs compilation time\n")
            print("  -extra          runs extra tests\n")
            print("  -hash           runs hash tests\n")
            print("  -nomain         disables main tests\n")
            return
        i += 1
    if !empty(singleTest)
        let appfile = "{get_das_root()}/examples/profile/tests/{singleTest}.das"
        test_single_file(appfile)
        return
    if mainTests
        run_dir("{get_das_root()}/examples/profile/tests")
    if extraTests
        run_dir("{get_das_root()}/examples/profile/extra_tests")
    if hashTests
        run_dir("{get_das_root()}/examples/profile/hash_test")
    if failed != 0
        print("TOTAL FAILURES: {failed}\n")


