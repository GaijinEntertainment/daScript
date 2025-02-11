#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"
#include <iostream>
#include <optional>

#include "daScript/ast/ast.h"
#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_serializer.h"

#include "../src/parser/parser_state.h"

//extern int das_yydebug;
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
                for (auto err: program->errors) {
                    std::cout << err.what << " " << err.at.describe() << std::endl;
                }
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
//    das_yydebug = 0;
    policies.version_2_syntax = false;
    const auto tmp_name1 = "/tmp/tmp1.das";
    {
        ofstream ostream(tmp_name1);
        ostream << src;
    }
    auto src_program = parseDaScript(tmp_name1, "", access, tout, libGroup, true, true, policies);
    while (prev != src) {
        prev = src;

        stringstream ss;
        format::init(&ss, src.c_str(), options, src_program);

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
            std::cout << program->errors.front().at.describe() << std::endl;
            return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
        }
        src = ss.str();
        iter++;
    }
    const auto tmp_name = "/tmp/tmp.das";
    {
        ofstream ostream(tmp_name);
        ostream << src;
        ostream.flush();
    }
    policies.version_2_syntax = options.contains(format::FormatOpt::AlwaysBraces);
    auto program = parseDaScript(tmp_name, "", access, tout, libGroup, true, true, policies);
    if (!program->failed()) {
        return {.ok=src};
    } else {
        std::cout << program->errors.front().at.describe() << std::endl;
        return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
    }
}

struct TestData {
    string original;
    string expected;
    format::FormatOptions options = {};
};

vector<TestData> test_cases() {
    const string in_prefix = "[safe_when_uninitialized]\n"
                             "struct Foo\n"
                               "    a : int = 5\n"
                               "    b : float = 3.0\n"
                               "struct Bar\n"
                               "    a : Foo\n"
                               "var var1 = ";
    const string out_prefix = in_prefix;

    const string postfix = "\n";
    vector<TestData> base = {
        {"[[/**/Foo/*0*/a/*a*/=/**/1/*a*///abc\n,//dsa\n/*dsa*/\nb=2.0/**/\n//dsa\n]]",
         "/**/Foo(/*0*/a/*a*/=/**/1/*a*///abc\n,//dsa\n/*dsa*/\nb=2.0/**/\n//dsa\n)"}, // 1 // no uninit because it's redundant
        {"[[/**/Foo/**/]]", "/**/Foo(uninitialized/**/)"}, // 2
        {"[[/*a*/Foo(/*b*/)/*c*/]]", "/*a*/Foo(/*b*/)/*c*/"}, // 3
        {"[[/*a*/Foo(/*b*/)/*c*/ a=1/*d*/,/*e*/b=2.0/*f*/]]", "/*a*//*b*/Foo(/*c*/ a=1/*d*/,/*e*/b=2.0/*f*/)"}, // 4
        {"[[/*a*/auto/*b*/1/*c*/,/*d*/2/*e*/]]", "/*a*/(/*b*/1/*c*/,/*d*/2/*e*/)"}, // 13
//        {"[[for x in [1, 20]; x*x; where x%2 == 0]];", "[iterator for x in [1, 20]; x*x; where x%2 == 0];"}, // 5 // each result is discarded, which is unsaf
        {"[{/*a*/Foo/*b*/a=1/*c*/,/*d*/b=2./*e*/;/*f*/a=2/*g*/,/*h*/b=1./*i*/}]",
         "/*a*/[Foo(/*b*/a=1/*c*/,/*d*/b=2./*e*/), Foo(/*f*/a=2/*g*/,/*h*/b=1./*i*/)]"}, // 6
        {"[{/*a*/Foo()/*b*/a=1/*c*/,/*d*/b=2./*e*/;/*g*/a=2/*h*/,/*i*/b=1./*j*/}]",
         "/*a*/[Foo(/*b*/a=1/*c*/,/*d*/b=2./*e*/), Foo(/*g*/a=2/*h*/,/*i*/b=1./*j*/)]"}, // 7
//        {"[{Foo a=1,b=2.;a=2,b=1. <optional_block>}]", "[Foo(a=1,b=2.),Foo(a=2,b=1.)]"}, // what about optional block in new syntax
        {"[{/*a*/auto/*b*/1/*c*/;/*d*/2/*e*/;/*f*/3/*g*/;/*h*/4/*i*/}]",
         "/*a*/[/*b*/1/*c*/,/*d*/2/*e*/,/*f*/3/*g*/,/*h*/4/*i*/]"}, // 8
        {"[{/*a*/auto/*b*/1/*c*/,/*d*/2.2/*e*/}]", "/*a*/[(/*b*/1/*c*/,/*d*/2.2/*e*/)]"}, // 8
        {"[{/*a*/for/*b*/x/*c*/in/*d*/0..10/*e*/;/*f*/x*x/*g*/;/*h*/where/*i*/x%2==0/*j*/}]",
         "[/*a*/for/*b*/x/*c*/in/*d*/0..10/*e*/;/*f*/x*x/*g*/;/*h*/where/*i*/x%2==0/*j*/]"}, // 9
        {"{{/*a*/for/*b*/x/*c*/in/*d*/0..10/*e*/;/*f*/x*x/*g*/;/*h*/where/*i*/x%2==0/*j*/}}",
         "[/*a*/for/*b*/x/*c*/in/*d*/0..10/*e*/;/*f*/x*x/*g*/;/*h*/where/*i*/x%2==0/*j*/]"}, // 12
        {"{{/*a*/1/*b*/;/*c*/2/*d*/;/*e*/3/*f*/;/*g*/4/*h*/}}", "{/*a*/1/*b*/,/*c*/2/*d*/,/*e*/3/*f*/,/*g*/4/*h*/}"}, // 10
        {R"({{/*a*/1=>"a"/*b*/;/*c*/2=>"b"/*d*/;/*e*/3=>"c"/*f*/;/*g*/4=>"d"/*h*/}})",
         R"({/*a*/1=>"a"/*b*/,/*c*/2=>"b"/*d*/,/*e*/3=>"c"/*f*/,/*g*/4=>"d"/*h*/})"}, // 10
        {R"([[/*a*/auto/*b*/1/*c*/,/*d*/2./*e*/,/*f*/"3"/*g*/;/*h*/1/*i*/,/*j*/4./*k*/,/*l*/"2"/*m*/]])",
         R"(/*a*/fixed_array((/*b*/1/*c*/,/*d*/2./*e*/,/*f*/"3"/*g*/),(/*h*/1/*i*/,/*j*/4./*k*/,/*l*/"2"/*m*/)))"}, // 13
//
//        // anything
        {"[[/*a*/auto/*b*/1/*c*/;/*d*/2/*e*/]]",
         "/*a*/fixed_array(/*b*/1/*c*/,/*d*/2/*e*/)"},
        {"[[/*a*/Foo?/*b*/]]", "/*a*/default<Foo?>/*b*/"},
        {"[[/*a*/Foo#/*b*/]]", "/*a*/struct<Foo#>(uninitialized/*b*/)"},

        // nested

        {"[[/*a*/Bar/*b*/a=[[/*c*/Foo/*d*/a=1/*e*/,/*f*/b=2.0/*g*/]]/*h*/]]",
         "/*a*/Bar(uninitialized/*b*/a=/*c*/Foo(uninitialized/*d*/a=1/*e*/,/*f*/b=2.0/*g*/)/*h*/)"},
    };
    for (auto &[in, out, options]: base) {
        in = in_prefix + in + postfix;
        out = out_prefix + out + postfix;
    }

    vector<TestData> tuple_expansion = {
        // tuple expansion (doesn't work in global scope!)
        {"def main()\n    var [[/*a*/a/*b*/,/*c*/b/*d*/]] = (123, 321);",
         "def main()\n    var (/*a*/a/*b*/,/*c*/b/*d*/) = (123, 321);"},
        {"def main()\n    var [[/*a*/a/*b*/,/*c*/b/*d*/]]: tuple<int, int> = (123, 321);",
         "def main()\n    var (/*a*/a/*b*/,/*c*/b/*d*/): tuple<int, int> = (123, 321);"},
    };

    vector<TestData> braces_tests = {
        // test braces
        {"def b()/**/\n    /**/let a = 5", "def b()/**/ {\n    /**/let a = 5;\n}"},
        {"def b() /**/{\n    let a = 5;/**/\n}", "def b() /**/{\n    let a = 5;/**/\n}"},
        {"def b(it)\n    let x = typeinfo is_iterable (it)", "def b(it) {\n    let x = typeinfo is_iterable (it);\n}"},

        {"class C\n"
         "    a : int = 5\n"
         "class A : C\n"
         "    c : string = \"add_new_call_macro\"",
         "class C {\n"
         "    a : int = 5;\n"
         "}\n"
         "class A : C {\n"
          "    c : string = \"add_new_call_macro\";\n"
          "}",
        },
        {"def main()//aa\n    /**/let x = 1    // 123", "def main() {//aa\n    /**/let x = 1;    // 123\n}"},
        {"bitfield A\n    refCount\n\n", "bitfield A {\n    refCount,\n}\n"},

        {"def b(x, y)\n    for x in y\n        x = x + 1",
         "def b(x, y) {\n    for (x in y) {\n        x = x + 1;\n    };\n}"},
        {"def b()\n    let a = 5\n    if a<0\n        a = a + 1",
         "def b() {\n    let a = 5;\n    if (a<0) {\n        a = a + 1;\n    }\n}"},
        {"def lower_bound ( a:array<auto(TT)>; val : TT const-& )\n"
         "    // comment \n"
         "    return lower_bound(a,0,length(a),val)\n"
         "let x = 1;",
         "def lower_bound ( a:array<auto(TT)>; val : TT const-& ) {\n"
         "    // comment \n"
         "    return lower_bound(a,0,length(a),val);\n"
         "}\n"
         "let x = 1;"},
    };
    for (auto &[in, out, opt]: braces_tests) {
        opt = format::FormatOptions(std::unordered_set<format::FormatOpt>{format::FormatOpt::AlwaysBraces});
    }
    vector<TestData> res;
    res.insert(res.end(), base.begin(), base.end());
    res.insert(res.end(), tuple_expansion.begin(), tuple_expansion.end());
    res.insert(res.end(), braces_tests.begin(), braces_tests.end()); // did not implement yet
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
            cout << " output:\n" << res.ok.value() << "\nexpected:\n" << out << endl;
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

        auto res = transform_syntax(file, str, format::FormatOptions(std::unordered_set<format::FormatOpt>{format::FormatOpt::AlwaysBraces}));
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