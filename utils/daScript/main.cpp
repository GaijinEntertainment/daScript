#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

void require_project_specific_modules();//link time resolved dependencies
das::FileAccessPtr get_file_access( char * pak );//link time resolved dependencies

TextPrinter tout;

static bool debuggerRequired = false;
static bool pauseAfterErrors = false;
static bool quiet = false;
static bool paranoid_validation = false;

das::Context * get_context ( int stackSize=0 );

bool saveToFile ( const string & fname, const string & str ) {
    if ( !quiet )  {
        tout << "saving to " << fname << "\n";
    }
    FILE * f = fopen ( fname.c_str(), "w" );
    if ( !f ) {
        tout << "can't open " << fname << "\n";
        return false;
    }
    fwrite ( str.c_str(), str.length(), 1, f );
    fclose ( f );
    return true;
}

bool compile ( const string & fn, const string & cppFn, bool dryRun ) {
    auto access = get_file_access(nullptr);
    ModuleGroup dummyGroup;
    CodeOfPolicies policies;
    policies.aot = false;
    policies.aot_module = true;
    policies.fail_on_lack_of_aot_export = true;
    if ( auto program = compileDaScript(fn,access,tout,dummyGroup,false,policies) ) {
        if ( program->failed() ) {
            tout << "failed to compile\n";
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
            }
            return false;
        } else {
            smart_ptr<Context> pctx ( get_context(program->getContextStackSize()) );
            if ( !program->simulate(*pctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
                }
                return false;
            }
            // AOT time
            TextWriter tw;
            bool noAotOption = program->options.getBoolOption("no_aot",false);
            bool noAotModule = false;
            // header
            tw << "#include \"daScript/misc/platform.h\"\n\n";

            tw << "#include \"daScript/simulate/simulate.h\"\n";
            tw << "#include \"daScript/simulate/aot.h\"\n";
            tw << "#include \"daScript/simulate/aot_library.h\"\n";
            tw << "\n";
            // lets comment on required modules
            program->library.foreach([&](Module * mod){
                if ( mod->name=="" ) {
                    // nothing, its main program module. i.e ::
                } else {
                    if ( mod->name=="$" ) {
                        tw << " // require builtin\n";
                    } else {
                        tw << " // require " << mod->name << "\n";
                    }
                    if ( mod->aotRequire(tw)==ModuleAotType::no_aot ) {
                        tw << "  // AOT disabled due to this module\n";
                        noAotModule = true;
                    }
                }
                return true;
            },"*");
            if (dryRun) {
                tout << "dry run success, no changes will be written\n";
                return true;
            }
            if (noAotOption) {
                TextWriter noTw;
                if (!noAotModule)
                  noTw << "// AOT disabled due to options no_aot=true. There are no modules which require no_aot\n\n";
                else
                  noTw << "// AOT disabled due to options no_aot=true. There are also some modules which require no_aot\n\n";
                return saveToFile(cppFn, noTw.str());
            } else if ( noAotModule ) {
                TextWriter noTw;
                noTw << "// AOT disabled due to module requirements\n";
                noTw << "#if 0\n\n";
                noTw << tw.str();
                noTw << "\n#endif\n";
                return saveToFile(cppFn, noTw.str());
            } else {
                tw << "\n";
                tw << "#if defined(_MSC_VER)\n";
                tw << "#pragma warning(push)\n";
                tw << "#pragma warning(disable:4100)   // unreferenced formal parameter\n";
                tw << "#pragma warning(disable:4189)   // local variable is initialized but not referenced\n";
                tw << "#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data\n";
                tw << "#pragma warning(disable:4114)   // same qualifier more than once\n";
                tw << "#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted\n";
                tw << "#pragma warning(disable:4946)   // reinterpret_cast used between related classes\n";
                tw << "#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results\n";
                tw << "#pragma warning(disable:4555)   // result of expression not used\n";
                tw << "#endif\n";
                tw << "#if defined(__GNUC__) && !defined(__clang__)\n";
                tw << "#pragma GCC diagnostic push\n";
                tw << "#pragma GCC diagnostic ignored \"-Wunused-parameter\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wunused-variable\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wunused-function\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wwrite-strings\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wreturn-local-addr\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wignored-qualifiers\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wsign-compare\"\n";
                tw << "#pragma GCC diagnostic ignored \"-Wsubobject-linkage\"\n";
                tw << "#endif\n";
                tw << "#if defined(__clang__)\n";
                tw << "#pragma clang diagnostic push\n";
                tw << "#pragma clang diagnostic ignored \"-Wunused-parameter\"\n";
                tw << "#pragma clang diagnostic ignored \"-Wwritable-strings\"\n";
                tw << "#pragma clang diagnostic ignored \"-Wunused-variable\"\n";
                tw << "#pragma clang diagnostic ignored \"-Wunsequenced\"\n";
                tw << "#pragma clang diagnostic ignored \"-Wunused-function\"\n";
                tw << "#endif\n";
                tw << "\n";
                tw << "namespace das {\n";
                tw << "namespace " << program->thisNamespace << " {\n"; // anonymous
                daScriptEnvironment::bound->g_Program = program;    // setting it for the AOT macros
                program->aotCpp(*pctx, tw);
                daScriptEnvironment::bound->g_Program.reset();
                // list STUFF
                tw << "\nstatic void registerAotFunctions ( AotLibrary & aotLib ) {\n";
                program->registerAotCpp(tw, *pctx, false);
                tw << "\tresolveTypeInfoAnnotations();\n";
                tw << "};\n";
                tw << "\n";
                tw << "AotListBase impl(registerAotFunctions);\n";
                // validation stuff
                if ( paranoid_validation ) {
                    program->validateAotCpp(tw,*pctx);
                    tw << "\n";
                }
                // footter
                tw << "}\n";
                tw << "}\n";
                tw << "#if defined(_MSC_VER)\n";
                tw << "#pragma warning(pop)\n";
                tw << "#endif\n";
                tw << "#if defined(__GNUC__) && !defined(__clang__)\n";
                tw << "#pragma GCC diagnostic pop\n";
                tw << "#endif\n";
                tw << "#if defined(__clang__)\n";
                tw << "#pragma clang diagnostic pop\n";
                tw << "#endif\n";
                return saveToFile(cppFn, tw.str());
            }
        }
    } else {
        tout << "failed to compile\n";
        return false;
    }
}

int das_aot_main ( int argc, char * argv[] ) {
    setCommandLineArguments(argc, argv);
    #ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, 0);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    #endif
    if ( argc<=3 ) {
        tout << "daScript -aot <in_script.das> <out_script.das.cpp> [-q] [-j] [-dry-run]\n";
        return -1;
    }
    bool dryRun = false;
    bool scriptArgs = false;
    if ( argc>3  ) {
        for (int ai = 4; ai != argc; ++ai) {
            if ( strcmp(argv[ai],"-q")==0 ) {
                quiet = true;
            } else if ( strcmp(argv[ai],"-p")==0 ) {
                paranoid_validation = true;
            } else if ( strcmp(argv[ai],"-dry-run")==0 ) {
                dryRun = true;
            } else if ( strcmp(argv[ai],"--")==0 ) {
                scriptArgs = true;
            } else if ( !scriptArgs ) {
                tout << "unsupported option " << argv[ai];
                return -1;
            }
        }
    }
    // register modules
    if (!Module::require("$")) {
        NEED_MODULE(Module_BuiltIn);
    }
    if (!Module::require("math")) {
        NEED_MODULE(Module_Math);
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
    if (!Module::require("debugapi")) {
        NEED_MODULE(Module_Debugger);
    }
    if (!Module::require("network")) {
        NEED_MODULE(Module_Network);
    }
    if (!Module::require("uriparser")) {
        NEED_MODULE(Module_UriParser);
    }
    if (!Module::require("jobque")) {
        NEED_MODULE(Module_JobQue);
    }
    if (!Module::require("fio")) {
        NEED_MODULE(Module_FIO);
    }
    if (!Module::require("dasbind")) {
        NEED_MODULE(Module_DASBIND);
    }
    require_project_specific_modules();
    #include "modules/external_need.inc"
    Module::Initialize();
    daScriptEnvironment::bound->g_isInAot = true;
    bool compiled = compile(argv[2], argv[3], dryRun);
    Module::Shutdown();
    return compiled ? 0 : -1;
}

bool compile_and_run ( const string & fn, const string & mainFnName, bool outputProgramCode, bool dryRun, const char * introFile = nullptr ) {
    auto access = get_file_access(nullptr);
    if ( introFile ) {
        auto fileInfo = make_unique<TextFileInfo>(introFile, uint32_t(strlen(introFile)), false);
        access->setFileInfo("____intro____", move(fileInfo));
    }
    bool success = false;
    ModuleGroup dummyGroup;
    CodeOfPolicies policies;
    policies.debugger = debuggerRequired;
    if ( debuggerRequired ) {
        policies.debug_module = getDasRoot() + "/daslib/debug.das";
    }
    policies.fail_on_no_aot = false;
    policies.fail_on_lack_of_aot_export = false;
    if ( auto program = compileDaScript(fn,access,tout,dummyGroup,false,policies) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            if ( pauseAfterErrors ) {
                getchar();
            }
        } else {
            if ( outputProgramCode )
                tout << *program << "\n";
            smart_ptr<Context> pctx ( get_context(program->getContextStackSize()) );
            if ( !program->simulate(*pctx, tout) ) {
                tout << "failed to simulate\n";
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
                }
            } else if ( program->thisModule->isModule ) {
                tout<< "WARNING: program is setup as both module, and endpoint.\n";
            } else if ( dryRun ) {
                success = true;
                tout << "dry run: " << fn << "\n";
            } else {
                auto fnVec = pctx->findFunctions(mainFnName.c_str());
                das::vector<SimFunction *> fnMVec;
                for ( auto fnAS : fnVec ) {
                    if ( verifyCall<void>(fnAS->debugInfo, dummyGroup) || verifyCall<bool>(fnAS->debugInfo, dummyGroup) ) {
                        fnMVec.push_back(fnAS);
                    }
                }
                if ( fnMVec.size()==0 ) {
                    tout << "function '"  << mainFnName << "' not found\n";
                } else if ( fnMVec.size()>1 ) {
                    tout << "too many options for '" << mainFnName << "'\ncandidates are:\n";
                    for ( auto fnAS : fnMVec ) {
                        tout << "\t" << fnAS->mangledName << "\n";
                    }
                } else {
                    success = true;
                    auto fnTest = fnMVec.back();
                    pctx->restart();
                    pctx->eval(fnTest, nullptr);
                }
            }
        }
    }
    return success;
}

void replace( string& str, const string& from, const string& to ) {
    size_t it = str.find(from);
    if( it != string::npos ) {
        str.replace(it, from.length(), to);
    }
}

void print_help() {
    tout
        << "daScript scriptName1 {scriptName2} .. {-main mainFnName} {-log} {-pause} -- {script arguments}\n"
        << "    -log        output program code\n"
        << "    -pause      pause after errors and pause again before exiting program\n"
        << "    -dry-run    compile and simulate script without execution\n"
        << "daScript -aot <in_script.das> <out_script.das.cpp> {-q} {-p}\n"
        << "    -p          paranoid validation of CPP AOT\n"
        << "    -q          supress all output\n"
        << "    -dry-run    no changes will be written\n"
    ;
}

#ifndef MAIN_FUNC_NAME
  #define MAIN_FUNC_NAME main
#endif

int MAIN_FUNC_NAME ( int argc, char * argv[] ) {
    if ( argc>2 && strcmp(argv[1],"-aot")==0 ) {
        return das_aot_main(argc, argv);
    }
    if ( argc<=1 ) {
        print_help();
        return -1;
    }
    setCommandLineArguments(argc,argv);
    das::vector<string> files;
    string mainName = "main";
    bool scriptArgs = false;
    bool outputProgramCode = false;
    bool pauseAfterDone = false;
    bool dryRun = false;
    for ( int i=1; i < argc; ++i ) {
        if ( argv[i][0]=='-' ) {
            string cmd(argv[i]+1);
            if ( cmd=="-" ) {
                scriptArgs = true;
            }
            if ( cmd=="main" ) {
                if ( i+1 > argc ) {
                    print_help();
                    return -1;
                }
                mainName = argv[i+1];
                i += 1;
            } else if ( cmd=="dasroot" ) {
                if ( i+1 > argc ) {
                    print_help();
                    return -1;
                }
                setDasRoot(argv[i+1]);
            } else if ( cmd=="log" ) {
                outputProgramCode = true;
            } else if ( cmd=="dry-run" ) {
                dryRun = true;
            } else if ( cmd=="args" ) {
                break;
            } else if ( cmd=="pause" ) {
                pauseAfterErrors = true;
                pauseAfterDone = true;
            } else if ( cmd=="-das-wait-debugger") {
                debuggerRequired = true;
            } else if ( !scriptArgs) {
                print_help();
                return -1;
            }
        }
        else if (!scriptArgs) {
            files.push_back(argv[i]);
        }
    }
    if (files.empty()) {
        print_help();
        return -1;
    }
    // register modules
    if (!Module::require("$")) {
        NEED_MODULE(Module_BuiltIn);
    }
    if (!Module::require("math")) {
        NEED_MODULE(Module_Math);
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
    if (!Module::require("debugapi")) {
        NEED_MODULE(Module_Debugger);
    }
    NEED_MODULE(Module_Network);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_JobQue);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
    require_project_specific_modules();
    #include "modules/external_need.inc"
    Module::Initialize();
    daScriptEnvironment::bound->g_isInAot = true;
    // compile and run
    int failedFiles = 0;
    for ( auto & fn : files ) {
        replace(fn, "_dasroot_", getDasRoot());
        if (!compile_and_run(fn, mainName, outputProgramCode, dryRun)) {
            failedFiles++;
        }
    }
    // and done
    if ( pauseAfterDone ) getchar();
    Module::Shutdown();
    return failedFiles;
}
