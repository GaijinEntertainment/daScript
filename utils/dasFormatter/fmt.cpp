#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"
#include "../src/parser/parser_state.h"

#include <fstream>

#include "daScript/ast/ast.h"

//extern int das_yydebug;
typedef void * yyscan_t;
union YYSTYPE;

#define YY_EXTRA_TYPE das::DasParserState *

#define YY_NO_UNISTD_H
#include "../src/parser/lex.yy.h"
#include "../src/parser/lex2.yy.h"
#include "formatter.h"
#include "fmt.h"
#include "../src/parser/parser_state.h"

void das_yybegin(const char * str, uint32_t len, yyscan_t yyscanner);
int fmt_yyparse(yyscan_t scanner);

das::FileAccessPtr get_file_access(char *pak);//link time resolved dependencies

namespace das::format {



void addNewModules(ModuleGroup &libGroup, ProgramPtr program) {
    libGroup.addModule(program->thisModule.release());
    program->library.foreach([&](Module *pm) -> bool {
        if (!pm->name.empty() && pm->name != "$") {
            if (!libGroup.findModule(pm->name)) {
                libGroup.addModule(pm);
            }
        }
        return true;
    }, "*");
}

Result transform_syntax(const string &filename, const string content, format::FormatOptions options) {
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

    TextPrinter tp;

    uint64_t preqT = 0;
    auto access = get_file_access(nullptr);
    TextPrinter tout;
    if (getPrerequisits(filename, access, req, missing, circular, notAllowed, chain,
                        dependencies, libGroup, nullptr, 1, !policies.ignore_shared_modules)) {
        for (auto &mod: req) {
            if (libGroup.findModule(mod.moduleName)) {
                continue;
            }
            auto program = parseDaScript(mod.fileName, mod.moduleName, access, tout, libGroup, true, true,
                                         policies);
            policies.threadlock_context |= program->options.getBoolOption("threadlock_context", false);
            if (program->failed()) {
                for (const auto& err: program->errors) {
                    tp << err.what << " " << err.at.describe() << '\n';
                }
                return {};
            }
            if (program->thisModule->name.empty()) {
                program->thisModule->name = mod.moduleName;
                program->thisModule->wasParsedNameless = true;
            }
            program->thisModule->fileName = mod.fileName;
            addNewModules(libGroup, program);
        }
    }

    int iter = 0;
//    das_fmt_yydebug = 0;
    policies.version_2_syntax = false;
    const auto tmp_name1 = "/tmp/tmp1.das";
    {
        std::ofstream ostream(tmp_name1);
        ostream << src.c_str();
    }
    auto src_program = parseDaScript(tmp_name1, "", access, tout, libGroup, true, true, policies);
    while (prev != src) {
        prev = src;

        TextWriter ss;
        format::init(&ss, src, options, src_program);

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
        libGroup.foreach([&](Module *pm) {
            program->library.addModule(pm);
            return true;
        }, "*");


        DasParserState parserState;
        parserState.g_Access = access;
        parserState.g_FileAccessStack.push_back(access->getFileInfo(filename));
        parserState.g_Program = program;
        parserState.das_def_tab_size = daScriptEnvironment::bound->das_def_tab_size;
        parserState.das_gen2_make_syntax = false;
        libGroup.foreach([&](Module *mod) {
            if (mod->commentReader) {
                parserState.g_CommentReaders.push_back(mod->commentReader.get());
            }
            return true;
        }, "*");

        das_yylex_init_extra(&parserState, &scanner);
        das_yybegin(src.c_str(), src.size(), scanner);
        auto err = fmt_yyparse(scanner);

        // end of parsing

        format::destroy();
        das_yylex_destroy(scanner);
        if (err != 0) {
            if (iter == 0) {
                return {};
            }
            tp << program->errors.front().at.describe() << '\n';
            return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
        }
        src = ss.str();
        iter++;
    }
    const auto tmp_name = "/tmp/tmp.das";
    {
        std::ofstream ostream(tmp_name);
        ostream << src.c_str();
        ostream.flush();
    }
    policies.version_2_syntax = options.contains(format::FormatOpt::V2Syntax);
    auto program = parseDaScript(tmp_name, "", access, tout, libGroup, true, true, policies);
    if (!program->failed()) {
        return {.ok=src};
    } else {
        tp << program->errors.front().at.describe() << '\n';
        return {.error=Result::ErrorInfo{prev, program->errors.front().what}};
    }
}

int run(FormatOptions opts, const vector<string> &files) {
    int ret_code = 0;
    TextPrinter tp;
    if (files.size() > 1 && !opts.contains(FormatOpt::Inplace)) {
        tp << "Expected inplace mode in case of more than 1 file\n";
    }
    for (const auto &file: files) {
        tp << "input file=" << file << '\n';
        std::ifstream t(file.c_str());

        string str(std::string((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>()).c_str());

        auto res = transform_syntax(file, str, opts);
        if (res.ok) {
            if (opts.contains(FormatOpt::Inplace)) {
                std::ofstream out(file.c_str());
                out << res.ok.value().c_str();
            } else {
                tp << res.ok.value().c_str();
            }
        } else if (res.error) {
            if (!opts.contains(FormatOpt::Inplace)) {
                tp << res.error->content << '\n';
            }
            tp << file << '\n' << res.error->what << '\n';
            ret_code = -1;
        } else {
            tp << "cant_compile=" << file << '\n';
        }
    }
    return ret_code;
}
}