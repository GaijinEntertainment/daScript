#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

#include <regex>

void yybegin(const char * str);
int yyparse();
int yylex_destroy();

namespace das {

    std::regex REG_require ("\\brequire\\s+(\\w+)");

    vector<string> getAllRequie ( const string & src ) {
        vector<string> req;
        std::match_results<string::const_iterator> sm;
        auto at = src.begin();
        while ( std::regex_search(at, src.end(), sm, REG_require) ) {
            req.emplace_back(sm[1].first, sm[1].second);
            at = sm.suffix().first;
        }
        return req;
    }

    bool getPrerequisits ( const string & fileName, const FileAccessPtr & access, vector<string> & req ) {
        if ( auto fi = access->getFileInfo(fileName) ) {
            string src = string ( fi->source, fi->sourceLength );  // stringview?
            vector<string> ownReq = getAllRequie(fi->source);
            for ( auto & mod : ownReq ) {
                if ( !Module::require(mod) ) {
                    if ( find(req.begin(), req.end(), mod)==req.end() ) {
                        req.push_back(mod);
                        // module file name
                        string modFn = access->getIncludeFileName(fileName, mod) + ".das";
                        if ( !getPrerequisits(modFn, access, req) ) {
                            return false;
                        }
                    }
                }
            }
            return true;
        } else {
            return false;
        }
    }

    // PARSER

    ProgramPtr g_Program;
    vector<FileInfoPtr>  g_AccessStack;

    extern "C" int64_t ref_time_ticks ();
    extern "C" int get_time_usec (int64_t reft);

    ProgramPtr parseDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleLibrary & libGroup ) {
        auto time0 = ref_time_ticks();
        int err;
        auto program = g_Program = make_shared<Program>();
        program->access = access;
        libGroup.foreach([&](Module * pm){
            g_Program->library.addModule(pm);
            return true;
        },"*");
        g_AccessStack.clear();
        if ( auto fi = g_Program->access->getFileInfo(fileName) ) {
            g_AccessStack.push_back(fi);
            yybegin(fi->source);
        } else {
            g_Program->error(fileName + " not found", LineInfo());
            g_Program.reset();
            return program;
        }
        err = yyparse();        // TODO: add mutex or make thread safe?
        yylex_destroy();
        g_Program.reset();
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->inferTypes(logs);
            if ( !program->failed() ) {
                if (program->options.getOption("optimize", true)) {
                    program->optimize(logs);
                } else {
                    program->buildAccessFlags(logs);
                }
                if (!program->failed())
                    program->verifyAndFoldContracts();
                if (!program->failed())
                    program->markOrRemoveUnusedSymbols();
                if (!program->failed())
                    program->allocateStack(logs);
                if (!program->failed())
                    program->finalizeAnnotations();
            }
            if (!program->failed()) {
                if (program->options.getOption("log")) {
                    logs << *program;
                }
                if (program->options.getOption("plot")) {
                    logs << "\n" << program->dotGraph() << "\n";
                }
            }
            sort(program->errors.begin(), program->errors.end());
            if ( program->options.getOption("logCompileTime",false) ) {
                auto dt = get_time_usec(time0) / 1000000.;
                logs << "compiler took " << dt << "\n";
            }
            return program;
        }
    }

    ProgramPtr compileDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleGroup & libGroup ) {
        vector<string> req;
        if ( getPrerequisits(fileName, access, req) ) {
            reverse(req.begin(), req.end());
            for ( auto & mod : req ) {
                if ( !libGroup.findModule(mod) ) {
                    string modFn = access->getIncludeFileName(fileName, mod) + ".das";
                    auto prog = parseDaScript(modFn, access, logs, libGroup);
                    if ( prog->failed() ) {
                        return prog;
                    }
                    prog->thisModule->name = mod;
                    libGroup.addModule(prog->thisModule.release());
                }
            }
        }
        return parseDaScript(fileName, access, logs, libGroup);
    }
}

