#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"

#include "../parser/parser_state.h"

typedef void * yyscan_t;
union YYSTYPE;

#define YY_EXTRA_TYPE das::DasParserState *

#define YY_NO_UNISTD_H
#include "../parser/lex.yy.h"

void das_yybegin(const char * str, uint32_t len, yyscan_t yyscanner);
int das_yyparse(yyscan_t yyscanner);

namespace das {

    bool isUtf8Text ( const char * src, uint32_t length ) {
        if ( length>=3  ) {
            auto usrc = (const uint8_t *)src;
            if ( usrc[0]==0xef && usrc[1]==0xbb && usrc[2]==0xbf) {
                return true;
            }
        }
        return false;
    }

    __forceinline bool isalphaE ( int ch ) {
        return (ch>='a' && ch<='z') || (ch>='A' && ch<='Z');
    }

    __forceinline bool isalnumE ( int ch ) {
        return (ch>='0' && ch<='9') || (ch>='a' && ch<='z') || (ch>='A' && ch<='Z');
    }

    void getAllRequireReq ( FileInfo * fi, const FileAccessPtr & access, vector<string> & req, das_set<FileInfo *> & collected  ) {
        const char * src = nullptr;
        uint32_t length = 0;
        fi->getSourceAndLength(src, length);
        if ( isUtf8Text(src,length) ) { // skip utf8 byte order mark
            src += 3;
            length -= 3;
        }
        const char * src_end = src + length;
        bool wb = true;
        while ( src < src_end ) {
            if ( src[0]=='"' ) {
                src ++;
                while ( src < src_end && src[0]!='"' ) {
                    src ++;
                }
                src ++;
                wb = true;
                continue;
            } else if ( src[0]=='/' && src[1]=='/' ) {
                while ( src < src_end && !(src[0]=='\n') ) {
                    src ++;
                }
                src ++;
                wb = true;
                continue;
            } else if ( src[0]=='/' && src[1]=='*' ) {
                int depth = 0;
                while ( src < src_end ) {
                    if ( src[0]=='/' && src[1]=='*' ) {
                        depth ++;
                        src += 2;
                    } else if ( src[0]=='*' && src[1]=='/' ) {
                        if ( --depth==0 ) break;
                        src += 2;
                    } else {
                        src ++;
                    }
                }
                src +=2;
                wb = true;
                continue;
            } else if ( wb && ((src+8)<src_end) && (src[0]=='r' || src[0]=='i') ) {   // need space for 'require ' || 'include '
                bool isReq = memcmp(src, "require", 7)==0;
                bool isInc = !isReq && (memcmp(src, "include", 7)==0);
                if ( isReq || isInc ) {
                    src += 7;
                    if ( isspace(src[0]) ) {
                        while ( src < src_end && isspace(src[0]) ) {
                            src ++;
                        }
                        if ( src >= src_end ) {
                            continue;
                        }
                        if ( src[0]=='_' || isalphaE(src[0]) || src[0] == '%' || src[0] == '.' || src[0]=='/' ) {
                            string mod;
                            mod += *src++;
                            while ( src < src_end && (isalnumE(src[0]) || src[0]=='_' || src[0]=='.' || src[0]=='/') ) {
                                mod += *src ++;
                            }
                            if ( isReq ) {
                                req.push_back(mod);
                            } else if ( isInc ) {
                                string incFileName = access->getIncludeFileName(fi->name,mod);
                                auto info = access->getFileInfo(incFileName);
                                if ( info ) {
                                    if ( collected.find(info)==collected.end() ) {
                                        collected.insert(info);
                                        getAllRequireReq(info, access, req, collected);
                                    }
                                }
                            }
                            continue;
                        } else {
                            wb = true;
                            goto nextChar;
                        }
                    } else {
                        wb = false;
                        goto nextChar;
                    }
                } else {
                    goto nextChar;
                }
            }
        nextChar:
            wb = src[0]!='_' && (wb ? !isalnumE(src[0]) : !isalphaE(src[0]));
            src ++;
        }
    }

    vector<string> getAllRequire ( FileInfo * fi, const FileAccessPtr & access  ) {
        das_set<FileInfo *> collected;
        vector<string> req;
        getAllRequireReq(fi, access, req, collected);
        return req;
    }

    string getModuleName ( const string & nameWithDots ) {
        auto idx = nameWithDots.find_last_of("./");
        if ( idx==string::npos ) return nameWithDots;
        return nameWithDots.substr(idx+1);
    }

    string getModuleFileName ( const string & nameWithDots ) {
        auto fname = nameWithDots;
        // TODO: should we?
        replace ( fname.begin(), fname.end(), '.', '/' );
        return fname;
    }

    bool addRequirements(const string & fileName, ModuleGroup & libGroup, Module * mod, const FileAccessPtr & access, vector<string> & notAllowed, TextWriter * log, int tab ) {
        if ( !access->isModuleAllowed(mod->name, fileName) ) {
            notAllowed.push_back(mod->name);
            if ( log ) {
                *log << string(tab,'\t') << "dependency " << mod->name << " - NOT ALLOWED\n";
            }
            return false;
        } else {
            if ( log ) {
                *log << string(tab,'\t') << "add dependency " << mod->name << "\n";
            }
            if ( libGroup.addModule(mod) ) {
                tab ++;
                for ( const auto & dep : mod->requireModule ) {
                    if ( !addRequirements(fileName, libGroup, dep.first, access, notAllowed, log, tab) ) {
                        return false;
                    }
                }
                tab --;
            }
            return true;
        }
    }

    string getDasRoot ( void );

    bool getPrerequisits ( const string & fileName,
                          const FileAccessPtr & access,
                          vector<ModuleInfo> & req,
                          vector<string> & missing,
                          vector<string> & circular,
                          vector<string> & notAllowed,
                          das_set<string> & dependencies,
                          ModuleGroup & libGroup,
                          TextWriter * log,
                          int tab,
                          bool allowPromoted ) {
        if ( auto fi = access->getFileInfo(fileName) ) {
            if ( log ) {
                *log << string(tab,'\t') << "in " << fileName << "\n";
            }
            vector<string> ownReq = getAllRequire(fi, access);
            for ( auto & mod : ownReq ) {
                if ( log ) {
                    *log << string(tab,'\t') << "require " << mod << "\n";
                }
                auto module = Module::requireEx(mod, allowPromoted); // try native with that name
                if ( !module ) {
                    auto info = access->getModuleInfo(mod, fileName);
                    if ( !info.moduleName.empty() ) {
                        mod = info.moduleName;
                        if ( log ) {
                            *log << string(tab,'\t') << " resolved as " << mod << "\n";
                        }
                    }
                    module = Module::requireEx(mod, allowPromoted); // try native with that name AGAIN (promoted?)
                    if ( !module ) {
                        auto it_r = find_if(req.begin(), req.end(), [&] ( const ModuleInfo & reqM ) {
                            return reqM.moduleName == mod;
                        });
                        if ( it_r==req.end() ) {
                            if ( dependencies.find(mod) != dependencies.end() ) {
                                // circular dependency
                                if ( log ) {
                                    *log << string(tab,'\t') << "from " << fileName << " require " << mod << " - CIRCULAR DEPENDENCY\n";
                                }
                                circular.push_back(mod);
                                return false;
                            }
                            dependencies.insert(mod);
                            // module file name
                            if ( info.moduleName.empty() ) {
                                // request can't be translated to module name
                                if ( log ) {
                                    *log << string(tab,'\t') << "from " << fileName << " require " << mod << " - MODULE INFO NOT FOUND\n";
                                }
                                missing.push_back(mod);
                                return false;
                            }
                            if ( !getPrerequisits(info.fileName, access, req, missing, circular, notAllowed, dependencies, libGroup, log, tab + 1, allowPromoted) ) {
                                return false;
                            }
                            if ( log ) {
                                *log << string(tab,'\t') << "from " << fileName << " require " << mod
                                    << " - ok, new module " << info.moduleName << " at " << info.fileName << "\n";
                            }
                            req.push_back(info);
                        } else {
                            if ( log ) {
                                *log << string(tab,'\t') << "from " << fileName << " require " << mod << " - already required\n";
                            }
                        }
                    } else {
                        if ( log ) {
                            *log << string(tab,'\t') << "from " << fileName << " require " << mod << " - shared, ok\n";
                        }
                        if ( !addRequirements(fileName, libGroup, module, access, notAllowed, log, tab) ) {
                            return false;
                        }
                    }
                } else {
                    if ( log ) {
                        *log << string(tab,'\t') << "from " << fileName << " require " << mod << " - ok\n";
                    }
                    if ( !access->isModuleAllowed(module->name, fileName) ) {
                        notAllowed.push_back(module->name);
                        if ( log ) {
                            *log << string(tab,'\t') << "in " << fileName << " module " << module->name << " - NOT ALLOWED\n";
                        }
                        return false;
                    } else {
                        libGroup.addModule(module);
                    }
                }
            }
            return true;
        } else {
            if ( log ) {
                *log    << string(tab,'\t') << "in " << fileName << " - FILE NOT FOUND\n"
                        << string(tab+1,'\t') << "getDasRoot()=`" << getDasRoot() << "`\n";
            }
            missing.push_back(fileName);
            return false;
        }
    }

    // PARSER

    extern "C" int64_t ref_time_ticks ();
    extern "C" int get_time_usec (int64_t reft);

    static DAS_THREAD_LOCAL int64_t totParse = 0;
    static DAS_THREAD_LOCAL int64_t totInfer = 0;
    static DAS_THREAD_LOCAL int64_t totOpt = 0;
    static DAS_THREAD_LOCAL int64_t totM = 0;

    ProgramPtr parseDaScript ( const string & fileName,
                              const FileAccessPtr & access,
                              TextWriter & logs,
                              ModuleGroup & libGroup,
                              bool exportAll,
                              bool isDep,
                              CodeOfPolicies policies ) {
        ReuseCacheGuard rcg;
        auto time0 = ref_time_ticks();
        int err;
        auto program = make_smart<Program>();
        daScriptEnvironment::bound->g_Program = program;
        program->promoteToBuiltin = false;
        program->isCompiling = true;
        program->isDependency = isDep;
        program->needMacroModule = false;
        program->policies = policies;
        program->thisModuleGroup = &libGroup;
        program->thisModuleName.clear();
        libGroup.foreach([&](Module * pm){
            program->library.addModule(pm);
            return true;
        },"*");
        DasParserState parserState;
        parserState.g_Access = access;
        parserState.g_Program = program;
        parserState.das_def_tab_size = daScriptEnvironment::bound->das_def_tab_size;
        yyscan_t scanner = nullptr;
        das_yylex_init_extra(&parserState, &scanner);
        if ( auto fi = access->getFileInfo(fileName) ) {
            parserState.g_FileAccessStack.push_back(fi);
            const char * src = nullptr;
            uint32_t len = 0;
            fi->getSourceAndLength(src,len);
            if (isUtf8Text(src, len)) {
                das_yybegin(src + 3, len-3, scanner);
            } else {
                das_yybegin(src, len, scanner);
            }
            libGroup.foreach([&](Module * mod){
                if ( mod->commentReader ) {
                    parserState.g_CommentReaders.push_back(mod->commentReader.get());
                }
                return true;
            },"*");
        } else {
            program->error(fileName + " not found", "","",LineInfo());
            program->isCompiling = false;
            daScriptEnvironment::bound->g_Program.reset();
            return program;
        }
        err = das_yyparse(scanner);
        das_yylex_destroy(scanner);
        parserState = DasParserState();
        totParse += get_time_usec(time0);
        if ( err || program->failed() ) {
            daScriptEnvironment::bound->g_Program.reset();
            sort(program->errors.begin(),program->errors.end());
            program->isCompiling = false;
            return program;
        } else {
            if ( policies.solid_context || program->options.getBoolOption("solid_context",false) ) {
                program->thisModule->isSolidContext = true;
            }
            auto timeI = ref_time_ticks();
            restartInfer: program->inferTypes(logs, libGroup);
            totInfer += get_time_usec(timeI);
            if ( !program->failed() ) {
                if ( program->patchAnnotations() ) {
                    goto restartInfer;
                }
            }
            if ( !program->failed() ) {
                program->lint(libGroup);
                program->foldUnsafe();
                auto timeO = ref_time_ticks();
                if (program->getOptimize()) {
                    program->optimize(logs,libGroup);
                } else {
                    program->buildAccessFlags(logs);
                }
                totOpt += get_time_usec(timeO);
                if (!program->failed())
                    program->verifyAndFoldContracts();
                if (!program->failed()) {
                    if ( program->thisModule->isModule || exportAll ) {
                        program->markModuleSymbolUse();
                    } else {
                        program->markExecutableSymbolUse();
                        program->removeUnusedSymbols();
                    }
                }
                if (!program->failed())
                    program->fixupAnnotations();
                if (!program->failed())
                    program->allocateStack(logs);
                if (!program->failed())
                    program->finalizeAnnotations();
            }
            if (!program->failed()) {
                if (program->options.getBoolOption("log")) {
                    logs << *program;
                }
            }
            daScriptEnvironment::bound->g_Program.reset();
            sort(program->errors.begin(), program->errors.end());
            program->isCompiling = false;
            if ( !program->failed() ) {
                if ( program->needMacroModule ) {
                    if ( !program->thisModule->isModule ) { // checking if its a module
                        program->error("Module " + fileName + " is not setup correctly for macros",
                            "module Module_Name is required", "", LineInfo(),
                                CompilationError::module_does_not_have_a_name);
                    }
                    auto timeM = ref_time_ticks();
                    if (!program->failed())
                        program->markMacroSymbolUse();
                    if (!program->failed())
                        program->allocateStack(logs);
                    if (!program->failed())
                        program->makeMacroModule(logs);
                    totM += get_time_usec(timeM);
                }
            }
            if ( program->options.getBoolOption("log_compile_time",false) ) {
                auto dt = get_time_usec(time0) / 1000000.;
                logs << "compiler took " << dt << ", " << fileName << "\n";
            }
            return program;
        }
    }

    ProgramPtr compileDaScript ( const string & fileName,
                                const FileAccessPtr & access,
                                TextWriter & logs,
                                ModuleGroup & libGroup,
                                bool exportAll,
                                CodeOfPolicies policies ) {
        ReuseCacheGuard rcg;
        auto time0 = ref_time_ticks();
        totParse = 0;
        vector<ModuleInfo> req;
        vector<string> missing, circular, notAllowed;
        das_set<string> dependencies;
        uint64_t preqT = 0;
        if ( getPrerequisits(fileName, access, req, missing, circular, notAllowed,
                dependencies, libGroup, nullptr, 1, !policies.ignore_shared_modules) ) {
            preqT = get_time_usec(time0);
            if ( policies.debugger ) {
                bool hasDebugger = false;
                for ( auto & mod : req ) {
                    if ( mod.moduleName=="debug") {
                        hasDebugger = true;
                        break;
                    }
                }
                if ( !hasDebugger ) {
                    getPrerequisits(policies.debug_module, access, req, missing, circular, notAllowed,
                        dependencies, libGroup, nullptr, 1, !policies.ignore_shared_modules);
                    auto finfo = access->getFileInfo(policies.debug_module);
                    ModuleInfo info;
                    info.fileName = finfo->name;
                    info.importName = "";
                    info.moduleName = "debug";
                    req.push_back(info);
                }
            }
            for ( auto & mod : req ) {
                if ( !libGroup.findModule(mod.moduleName) ) {
                    auto program = parseDaScript(mod.fileName, access, logs, libGroup, true, true, policies);
                    if ( program->failed() ) {
                        return program;
                    }
                    if ( policies.fail_on_lack_of_aot_export ) {
                        if ( !program->options.getBoolOption("no_aot",false) ) {
                            if ( program->thisModule->name.empty() ) {
                                program->error("Module " + mod.moduleName + " is not setup correctly for AOT",
                                    "module " + mod.moduleName + " is required", "", LineInfo(),
                                        CompilationError::module_does_not_have_a_name);
                                return program;
                            }
                        }
                    }
                    if ( program->thisModule->name.empty() ) {
                        program->thisModule->name = mod.moduleName;
                    }
                    if ( program->promoteToBuiltin ) {
                        bool regFromShar = false;
                        for ( auto & reqM : program->thisModule->requireModule ) {
                            if ( !reqM.first->builtIn ) {
                                program->error("Shared module " + program->thisModule->name + " has incorrect dependency type.",
                                    "Can't require " + reqM.first->name + " because its not shared", "", LineInfo(),
                                        CompilationError::module_required_from_shared);
                                regFromShar = true;
                            }
                        }
                        if (  regFromShar ) {
                            return program;
                        }
                        program->thisModule->promoteToBuiltin(access);
                    }
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
            }
            auto res = parseDaScript(fileName, access, logs, libGroup, exportAll, false, policies);
            if ( !res->failed() ) {
                if ( res->options.getBoolOption("log_symbol_use") ) {
                    res->markSymbolUse(false, false, false, &logs);
                }
            }
            if ( policies.aot_module && (res->promoteToBuiltin || res->thisModule->isModule || exportAll) ) {
                if (!res->failed()) {
                    if(exportAll)
                        res->markSymbolUse(false,true,true);
                    else
                        res->markModuleSymbolUse();
                }
                if (!res->failed() && !exportAll)
                    res->removeUnusedSymbols();
                if (!res->failed())
                    res->allocateStack(logs);
            } else {
                if (!res->failed())
                    res->markExecutableSymbolUse();
                if ( res->getDebugger()) {
                    TextWriter ss;
                    for ( const auto & arq : res->allRequireDecl ) {
                        ss << get<1>(arq) << " ";
                    }
                    ss << fileName;
                    auto rtti_require = make_smart<Variable>();
                    rtti_require->name = "__rtti_require";
                    rtti_require->type = make_smart<TypeDecl>(Type::tString);
                    rtti_require->init = make_smart<ExprConstString>(ss.str());
                    rtti_require->init->type = make_smart<TypeDecl>(Type::tString);
                    rtti_require->used = true;
                    rtti_require->private_variable = true;
                    res->thisModule->addVariable(rtti_require);
                }
                if (!res->failed())
                    res->removeUnusedSymbols();
                if (!res->failed())
                    res->allocateStack(logs);
            }
            if ( res->options.getBoolOption("log_require",false) ) {
                TextWriter tw;
                req.clear();
                missing.clear();
                circular.clear();
                notAllowed.clear();
                dependencies.clear();
                getPrerequisits(fileName, access, req, missing, circular, notAllowed, dependencies, libGroup, &tw, 1, false);
                logs << "module dependency graph:\n" << tw.str();
            }
            if ( !res->failed() ) {
                auto hf = hash_blockz64((uint8_t *)fileName.c_str());
                res->thisNamespace = "_anon_" + to_string(hf);
            }
            if ( res->options.getBoolOption("log_total_compile_time",false) ) {
                auto totT = get_time_usec(time0);
                logs << "compiler took " << (totT  / 1000000.) << "\n"
                     << "\trequire  " << (preqT    / 1000000.) << "\n"
                     << "\tparse    " << (totParse / 1000000.) << "\n"
                     << "\tinfer    " << (totInfer / 1000000.) << "\n"
                     << "\toptimize " << (totOpt   / 1000000.) << "\n"
                     << "\tmacro    " << (totM     / 1000000.) << "\n"
                ;
            }
            return res;
        } else {
            TextWriter tw;
            req.clear();
            missing.clear();
            circular.clear();
            dependencies.clear();
            getPrerequisits(fileName, access, req, missing, circular, notAllowed, dependencies, libGroup, &tw, 1, false);
            auto program = make_smart<Program>();
            program->policies = policies;
            program->thisModuleGroup = &libGroup;
            TextWriter err;
            for ( auto & mis : missing ) {
                err << "missing prerequisit " << mis << "\n";
            }
            for ( auto & mis : circular ) {
                err << "circular dependency " << mis << "\n";
            }
            for ( auto & mis : notAllowed ) {
                err << "module not allowed " << mis << "\n";
            }
            program->error(err.str(),"module dependency graph:\n" + tw.str(), "", LineInfo(),
                            CompilationError::module_not_found);
            return program;
        }
    }
}

