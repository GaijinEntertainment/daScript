#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

void das_yybegin(const char * str);
int das_yyparse();
int das_yylex_destroy();

namespace das {

    vector<string> getAllRequie ( const char * src, uint32_t length ) {
        vector<string> req;
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
            } else if ( wb && ((src+8)<src_end) && src[0]=='r') {   // need space for 'require '
                if ( memcmp(src, "require", 7)==0 ) {
                    src += 7;
                    if ( isspace(src[0]) ) {
                        while ( src < src_end && isspace(src[0]) ) {
                            src ++;
                        }
                        if ( src >= src_end ) {
                            continue;
                        }
                        if ( src[0]=='_' || isalpha(src[0]) || src[0] ) {
                            string mod;
                            while ( src < src_end && (isalnum(src[0]) || src[0]=='_' || src[0]=='.' || src[0]=='/') ) {
                                mod += *src ++;
                            }
                            req.push_back(mod);
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
            wb = src[0]!='_' && (wb ? !isalnum(src[0]) : !isalpha(src[0]));
            src ++;
        }
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

    bool getPrerequisits ( const string & fileName,
                          const FileAccessPtr & access,
                          vector<string> & req,
                          vector<string> & missing,
                          set<string> & dependencies,
                          ModuleGroup & libGroup) {
        if ( auto fi = access->getFileInfo(fileName) ) {
            vector<string> ownReq = getAllRequie(fi->source, fi->sourceLength);
            for ( auto & mod : ownReq ) {
                auto modName = getModuleName(mod);
                auto module = Module::require(modName);
                if ( !module ) {
                    if ( find(req.begin(), req.end(), mod)==req.end() ) {
                        if ( dependencies.find(mod) != dependencies.end() ) {
                            // circular dependency
                            missing.push_back(mod);
                            return false;
                        }
                        dependencies.insert(mod);
                        // module file name
                        string modFName = getModuleFileName(mod);
                        string modFn = access->getIncludeFileName(fileName, modFName) + ".das";
                        if ( !getPrerequisits(modFn, access, req, missing, dependencies, libGroup) ) {
                            return false;
                        }
                        req.push_back(mod);
                    }
                } else {
                    libGroup.addModule(module);
                }
            }
            return true;
        } else {
            missing.push_back(fileName);
            return false;
        }
    }

    // PARSER

    ProgramPtr g_Program;
    FileAccessPtr g_Access;
    vector<FileInfo *>  g_FileAccessStack;

    extern "C" int64_t ref_time_ticks ();
    extern "C" int get_time_usec (int64_t reft);

    ProgramPtr parseDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleGroup & libGroup, bool exportAll ) {
        auto time0 = ref_time_ticks();
        int err;
        auto program = g_Program = make_shared<Program>();
        g_Access = access;
        program->thisModuleGroup = &libGroup;
        libGroup.foreach([&](Module * pm){
            g_Program->library.addModule(pm);
            return true;
        },"*");
        g_FileAccessStack.clear();
        if ( auto fi = access->getFileInfo(fileName) ) {
            g_FileAccessStack.push_back(fi);
            das_yybegin(fi->source);
        } else {
            g_Program->error(fileName + " not found", LineInfo());
            g_Program.reset();
            g_Access.reset();
            g_FileAccessStack.clear();
            return program;
        }
        err = das_yyparse();        // TODO: add mutex or make thread safe?
        das_yylex_destroy();
        g_Program.reset();
        g_Access.reset();
        g_FileAccessStack.clear();
        if ( err || program->failed() ) {
            sort(program->errors.begin(),program->errors.end());
            return program;
        } else {
            program->inferTypes(logs);
            if ( !program->failed() ) {
                program->lint();
                if (program->options.getOption("optimize", true)) {
                    program->optimize(logs);
                } else {
                    program->buildAccessFlags(logs);
                }
                if (!program->failed())
                    program->verifyAndFoldContracts();
                if (!program->failed())
                    program->markOrRemoveUnusedSymbols(exportAll);
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
            if ( program->options.getOption("log_compile_time",false) ) {
                auto dt = get_time_usec(time0) / 1000000.;
                logs << "compiler took " << dt << "\n";
            }
            return program;
        }
    }

    ProgramPtr compileDaScript ( const string & fileName, const FileAccessPtr & access, TextWriter & logs, ModuleGroup & libGroup, bool exportAll ) {
        vector<string> req, missing;
        set<string> dependencies;
        if ( getPrerequisits(fileName, access, req, missing, dependencies, libGroup) ) {
            for ( auto & mod : req ) {
                auto modName = getModuleName(mod);
                if ( !libGroup.findModule(modName) ) {
                    string modFName = getModuleFileName(mod);
                    string modFn = access->getIncludeFileName(fileName, modFName) + ".das";
                    auto program = parseDaScript(modFn, access, logs, libGroup, true);
                    if ( program->failed() ) {
                        return program;
                    }
                    if ( program->thisModule->name.empty() ) {
                        program->thisModule->name = modName;
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
            return parseDaScript(fileName, access, logs, libGroup, exportAll);
        } else {
            auto program = make_shared<Program>();
            program->thisModuleGroup = &libGroup;
            for ( auto & mis : missing ) {
                program->error("missing prerequisit " + mis + ", or circular dependency",
                               LineInfo(), CompilationError:: module_not_found);
            }
            return program;
        }
    }
}

