#include "daScript/daScript.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

TextPrinter tout;

bool debuggerRequired = false;
bool pauseAfterErrors = false;

das::Context * get_context ( int stackSize=0 );

void compile_and_run ( const string & fn, const string & mainFnName, bool outputProgramCode, const char * introFile = nullptr ) {
    auto access = make_smart<FsFileAccess>();
    if ( introFile ) {
        auto fileInfo = make_unique<TextFileInfo>(introFile, uint32_t(strlen(introFile)), false);
        access->setFileInfo("____intro____", move(fileInfo));
    }
    ModuleGroup dummyGroup;
    CodeOfPolicies policies;
    policies.debugger = debuggerRequired;
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
            } else {
                if ( program->thisModule->isModule ) {
                    tout<< "WARNING: program is setup as both module, and endpoint.\n";
                }
                auto fnVec = pctx->findFunctions(mainFnName.c_str());
                vector<SimFunction *> fnMVec;
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
            }
        }
    }
}

void print_help() {
    tout << "daScript scriptName1 {scriptName2} .. {-main mainFnName} {-log}\n";
}

void require_project_specific_modules();//link time resolved dependencies

const char * debugger_intro =
    "options debugger\n"
    "require daslib/debug\n"
    "[export]\n"
    "def main {}\n"
    "\n";

int main(int argc, char * argv[]) {
    if ( argc<=1 ) {
        print_help();
        return -1;
    }
    setCommandLineArguments(argc,argv);
    vector<string> files;
    string mainName = "main";
    bool scriptArgs = false;
    bool outputProgramCode = false;
    bool pauseAfterDone = false;
    for ( int i=1; i < argc; ++i ) {
        if ( argv[i][0]=='-' ) {
            string cmd(argv[i]+1);
            if ( cmd=="-" ) {
                scriptArgs = true;
            }
            if ( cmd=="main" ) {
                if (i+1 > argc)
                {
                    print_help();
                    return -1;
                }
                mainName = argv[i+1];
                i += 1;
            } else if ( cmd=="log" ) {
                outputProgramCode = true;
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
    if (files.empty())
    {
        print_help();
        return -1;
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
    // ask for debugger
    if ( debuggerRequired ) {
        compile_and_run("____intro____","main",false,debugger_intro);
    }
    // compile and run
    for ( const auto & fn : files ) {
        compile_and_run(fn, mainName, outputProgramCode);
    }
    // and done
    if ( pauseAfterDone ) getchar();
    Module::Shutdown();
    return 0;
}


