#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"
#include <iostream>
#include <optional>

#include "daScript/ast/ast.h"
#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_serializer.h"

#include "../src/parser/parser_state.h"

typedef void * yyscan_t;
union YYSTYPE;

#define YY_EXTRA_TYPE das::DasParserState *

#define YY_NO_UNISTD_H
#include "../src/parser/lex.yy.h"
#include "../src/parser/lex2.yy.h"
#include "formatter.h"

void das_yybegin(const char * str, uint32_t len, yyscan_t yyscanner);
int das_yyparse(yyscan_t yyscanner);


#include <vector>
#include <string>
#include <sstream>
#include <fstream>

typedef void * yyscan_t;
int das_yyparse(yyscan_t yyscanner);

using namespace das;


void InitModules() {
    // register modules
    if (!Module::require("$")) {
        NEED_MODULE(Module_BuiltIn);
    }
    if (!Module::require("math")) {
        NEED_MODULE(Module_Math);
    }
    if (!Module::require("raster")) {
        NEED_MODULE(Module_Raster);
    }
    if (!Module::require("strings")) {
        NEED_MODULE(Module_Strings);
    }
    if (!Module::require("rtti")) {
        NEED_MODULE(Module_Rtti);
    }
    if (!Module::require("ast")) {
        NEED_MODULE(Module_Ast);
    }
    if (!Module::require("jit")) {
        NEED_MODULE(Module_Jit);
    }
    if (!Module::require("debugapi")) {
        NEED_MODULE(Module_Debugger);
    }
    NEED_MODULE(Module_Network);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_JobQue);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
#include "modules/external_need.inc"
    Module::Initialize();
    // compile and run

}

das::FileAccessPtr get_file_access( char * pak );//link time resolved dependencies

string help() {
    return "Tool to convert dascript v1 syntax to v2\n"
           "das-fmt {-i} filename1 {filename2} ...:\n"
           "   -i inplace conversion, write to the same file. Multiple filenames only allowed in inplace mode\n"
           "   --tests Run tests, no filenames required\n"
           "";
}

struct Result {
    struct ErrorInfo {
        string content;
        string what;
    };
    optional<string> ok;
    optional<ErrorInfo> error;
};

void addNewModules ( ModuleGroup & libGroup, ProgramPtr program ) {
    libGroup.addModule(program->thisModule.release());
    program->library.foreach([&](Module * pm) -> bool {
        if ( !pm->name.empty() && pm->name!="$" ) {
            if ( !libGroup.findModule(pm->name) ) {
                libGroup.addModule(pm);
            }
        }
        return true;
    }, "*");
}

Result transform_syntax(const string& filename, const string content, format::FormatOptions options = {}) {
    auto src = content;
    string prev;
    vector<ModuleInfo> req;
    vector<RequireRecord> missing, circular, notAllowed;
    vector<FileInfo *> chain;
    das_set<string> dependencies;
    ModuleGroup libGroup;
    CodeOfPolicies policies;
    policies.aot = false;
    policies.aot_module = false;
    policies.fail_on_lack_of_aot_export = false;
    policies.version_2_syntax = false;

    uint64_t preqT = 0;
    auto access = get_file_access(nullptr);
    TextPrinter tout;
    if ( getPrerequisits(filename, access, req, missing, circular, notAllowed, chain,
                         dependencies, libGroup, nullptr, 1, !policies.ignore_shared_modules) ) {
        for ( auto & mod : req ) {
            if ( libGroup.findModule(mod.moduleName) ) {
                continue;
            }
            auto program = parseDaScript(mod.fileName, mod.moduleName, access, tout, libGroup, true, true, policies);
            policies.threadlock_context |= program->options.getBoolOption("threadlock_context",false);
            if ( program->failed() ) {
                return {};
            }
            if ( program->thisModule->name.empty() ) {
                program->thisModule->name = mod.moduleName;
                program->thisModule->wasParsedNameless = true;
            }
            program->thisModule->fileName = mod.fileName;
            addNewModules(libGroup, program);
        }
    }

    int iter = 0;
    while (prev != src) {
        prev = src;

        stringstream ss;
        format::init(&ss, src.c_str(), options);

        // All initialization and parsing took from daslang source
        yyscan_t scanner = nullptr;
        ProgramPtr program = make_smart<Program>();
        daScriptEnvironment::bound->g_Program = program;
        daScriptEnvironment::bound->g_compilerLog = &tout;
        program->promoteToBuiltin = false;
        program->isCompiling = true;
        program->isDependency = false;
        program->needMacroModule = false;
        program->policies = policies;
        program->thisModuleGroup = &libGroup;
        program->thisModuleName = program->thisModule->name;
        libGroup.foreach([&](Module * pm){
            program->library.addModule(pm);
            return true;
        },"*");


        DasParserState parserState;
        parserState.g_Access = access;
        parserState.g_FileAccessStack.push_back(access->getFileInfo(filename));
        parserState.g_Program = program;
        parserState.das_def_tab_size = daScriptEnvironment::bound->das_def_tab_size;
        parserState.das_gen2_make_syntax = false;
        libGroup.foreach([&](Module * mod){
            if ( mod->commentReader ) {
                parserState.g_CommentReaders.push_back(mod->commentReader.get());
            }
            return true;
        },"*");

        das_yylex_init_extra(&parserState, &scanner);
        das_yybegin(src.c_str(), src.size(), scanner);
        auto err = das_yyparse(scanner);

        // end of parsing

        format::destroy();
        das_yylex_destroy(scanner);
        if (err != 0) {
            if (iter == 0) {
                return {};
            }
            return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
        }
        src = ss.str();
        iter++;
    }
    const auto tmp_name = "/tmp/tmp.das";
    {
        ofstream ostream(tmp_name);
        ostream << src;
    }
    auto program = parseDaScript(tmp_name, "", access, tout, libGroup, true, true, policies);
    if (!program->failed()) {
        return {.ok=src};
    } else {
        return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
    }
}

struct TestData {
    string original;
    string expected;
    format::FormatOptions options = {};
};

vector<TestData> test_cases() {
    const string in_prefix = "struct Foo\n"
                               "    a : int = 5\n"
                               "    b : float = 3.0\n"
                               "struct Bar\n"
                               "    a : Foo\n"
                               "var var1 = ";
    const string out_prefix = in_prefix;

    const string postfix = "\n";
    vector<TestData> res = {
        {"[[Foo a=1,b=2.0]]", "Foo(uninitialized a=1, b=2.0)"}, // 1
        {"[[Foo]]", "Foo(uninitialized)"}, // 2
        {"[[Foo()]]", "Foo()"}, // 3
        {"[[Foo() a=1,b=2.0]]", "Foo(a=1, b=2.0)"}, // 4
        {"[[auto 1,2]]", "(1, 2)"}, // 4
        {"[[for i in [1, 2]; x*x; where x%2 == 0]]", "[iterator for i in [1, 2]; x*x; where x%2 == 0]"}, // 5
        {"[{Foo a=1,b=2.;a=2,b=1.}]", "[Foo(a=1, b=2.), Foo(a=2, b=1.)]"}, // 6
        {"[{Foo() a=1,b=2.;a=2,b=1.}]", "[Foo(a=1, b=2.), Foo(a=2, b=1.)]"}, // 7
//        {"[{Foo a=1,b=2.;a=2,b=1. <optional_block>}]", "[Foo(a=1,b=2.),Foo(a=2,b=1.)]"}, // what about optional block in new syntax
        {"[{auto 1;2;3;4}]", "array(1, 2, 3, 4)"}, // 8
        {"[{auto 1,2.2}]", "array((1, 2.2))"}, // 8
        {"[{for x in 0..10; x*x; where x%2==0}]", "[for x in 0..10; x*x; where x%2==0]"}, // 9
        {"{{ 1; 2; 3; 4 }}", "{1, 2, 3, 4}"}, // 10
        {R"({{ 1=>"a"; 2=>"b"; 3=>"c"; 4=>"d" }})", R"({1=>"a", 2=>"b", 3=>"c", 4=>"d"})"}, // 10
        {"[[auto 1,2.,\"3\"; 1,4,\"2\"]]", "fixed_array((1, 2., \"3\"), (1, 4, \"2\"))"}, // 13

        // anything
        {"[[auto 1;\"ttt\"]]", "fixed_array(1, \"ttt\")"},
        {"[[Foo?]]", "default<Foo?>"},
        {"[[Foo#]]", "struct<Foo#>(uninitialized)"},
        {"[[Foo]]", "Foo(uninitialized)"},

        // nested

        {"[[Bar a=[[Foo a=1,b=2.0]]]]", "Bar(uninitialized a=Foo(uninitialized a=1, b=2.0))"},
    };
    for (auto &[in, out, options]: res) {
        in = in_prefix + in + postfix;
        out = out_prefix + out + postfix;
    }

    vector<TestData> tuple_expansion = {
        // tuple expansion (doesn't work in global scope!)
        {"def main()\n    var [[a, b]] = (123, 321);",
         "def main()\n    var (a, b) = (123, 321);"},
        {"def main()\n    var [[a, b]]: tuple<int, int> = (123, 321);",
         "def main()\n    var (a, b): tuple<int, int> = (123, 321);"},
    };

    vector<TestData> braces_tests = {
        // test braces
        {"def b()\n    let a = 5;", "def b() {\n    let a = 5;\n}"},
        {"def b() {\n    let a = 5;\n}", "def b() {\n    let a = 5;\n}"},

        {"def b()\n    let a = 5\n    if a<0\n        a = a + 1",
         "def b() {\n    let a = 5\n    if a<0 {\n        a = a + 1\n    }\n}"},
        {"def b() {\n    let a = 5\n}", "def b() {\n    let a = 5\n}"},
        {"def b() {{\n    let a = 5\n}}", "def b() {{\n    let a = 5\n}"},
    };
    for (auto &[in, out, opt]: braces_tests) {
        opt = format::FormatOptions({format::FormatOpt::AlwaysBraces});
    }
    res.insert(res.end(), tuple_expansion.begin(), tuple_expansion.end());
//    res.insert(res.end(), braces_tests.begin(), braces_tests.end()); // do not implemented yet
    return res;
}

int test() {
    auto cases = test_cases();
    int ret_code = 0;
    for (const auto &[in, out, cfg]: cases) {
        auto res = transform_syntax("test.das", in, cfg); // any filename
        if (!res.ok) {
            cout << "input:\n" << in << " \noutput:\n" << res.error->content << " \nerror:\n" << res.error->what << endl;
            ret_code = -1;
        } else if (res.ok != out) {
            cout << " output:\n" << res.ok.value() << " expected:\n" << out << endl;
            ret_code = -1;
        }
    }
    return ret_code;
}

int main(int argc, char** argv) {
    bool is_inplace = false;
    bool is_tests = false;
    vector<string> files;
    if (argc < 2) {
        cout << help();
        return -1;
    } else if (argc == 2) {
        if (string(argv[1]) == "--tests") {
            is_tests = true;
        } else {
            files = {string(argv[1])};
        }
    } else {
        if (string(argv[1]) == "-i") {
            is_inplace = true;
        } else {
            cout << help();
            return -1;
        }
        for (size_t i = 2; i < argc; i++) {
            files.emplace_back(argv[i]);
        }
    }
    if (files.empty()) {
        cout << "no files" << endl;
    }
    InitModules();
    if (is_tests) {
        return test();
    }

    int ret_code = 0;
    for (const auto& file: files) {
        cout << "input file=" << file << endl;
        ifstream t(file);

        string str((istreambuf_iterator<char>(t)),
                        istreambuf_iterator<char>());

        auto res = transform_syntax(file, str);
        if (res.ok) {
            if (is_inplace) {
                ofstream out(file);
                out << res.ok.value();
            } else {
                cout << res.ok.value();
            }
        } else if (res.error) {
            if (!is_inplace) {
                cout << res.error->content << endl;
            }
            cout << file << endl << res.error->what << endl;
            ret_code = -1;
        } else {
            cout << "cant_compile=" << file << endl;
        }
    }
    return ret_code;
}