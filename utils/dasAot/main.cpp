#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

namespace das {
    extern bool g_isInAot;
    extern ProgramPtr g_Program;
}

void require_project_specific_modules();//link time resolved dependencies
das::FileAccessPtr get_file_access( char * pak );//link time resolved dependencies

#if !defined(DAS_GLOBAL_NEW) && defined(_MSC_VER) && !defined(_WIN64)

void * operator new(std::size_t n) throw(std::bad_alloc)
{
    return das_aligned_alloc16(n);
}
void operator delete(void * p) throw()
{
    das_aligned_free16(p);
}

#endif

static bool quiet = false;

static bool paranoid_validation = false;

TextPrinter tout;

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

das::Context * get_context ( int stackSize=0 );

bool compile ( const string & fn, const string & cppFn, const string &mainFnName ) {
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
            if (!mainFnName.empty()) {
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
                    auto fnTest = fnMVec.back();
                    pctx->restart();
                    pctx->eval(fnTest, nullptr);
                }
                return true;
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
                g_Program = program;
                program->aotCpp(*pctx, tw);
                g_Program.reset();
                // list STUFF
                tw << "\tstatic void registerAotFunctions ( AotLibrary & aotLib ) {\n";
                program->registerAotCpp(tw, *pctx, false);
                tw << "\t};\n";
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

#ifndef MAIN_FUNC_NAME
  #define MAIN_FUNC_NAME main
#endif

int MAIN_FUNC_NAME(int argc, char * argv[]) {
    g_isInAot = true;
    setCommandLineArguments(argc, argv);
    #ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, 0);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    #endif
    if ( argc<3 ) {
        tout << "dasAot <in_script.das> <out_script.das.cpp> [-q] [-j]\n";
        return -1;
    }
    string mainName;
    if ( argc>3  ) {
        bool scriptArgs = false;
        for (int ai = 3; ai != argc; ++ai) {\
            if ( strcmp(argv[ai],"-main")==0  ) {
                if (ai+1 > argc)
                {
                    tout << "dasAot <in_script.das> <out_script.das.cpp> [-q] [-j]\n";
                    return -1;
                }
                mainName = argv[ai+1];
                ai += 1;
            } else if ( strcmp(argv[ai],"-q")==0 ) {
                quiet = true;
            } else if ( strcmp(argv[ai],"-p")==0 ) {
                paranoid_validation = true;
            } else if ( strcmp(argv[ai],"--")==0 ) {
                scriptArgs = true;
            } else if ( !scriptArgs ) {
                tout << "unsupported option " << argv[ai];
                return -1;
            }
        }
    }
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    NEED_MODULE(Module_Strings);
    NEED_MODULE(Module_Rtti);
    NEED_MODULE(Module_Ast);
    NEED_MODULE(Module_Debugger);
    NEED_MODULE(Module_Network);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_JobQue);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
    require_project_specific_modules();
    #include "modules/external_need.inc"
    Module::Initialize();
    bool compiled = compile(argv[1], argv[2], mainName);
    Module::Shutdown();
    return compiled ? 0 : -1;
}


