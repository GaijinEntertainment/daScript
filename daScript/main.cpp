#include "daScript/daScript.h"

#include <fstream>

using namespace das;

TextPrinter tout;

void compile_and_run ( const string & fn, const string & mainFnName, bool outputProgramCode ) {
    std::string str;
    std::ifstream t(fn.c_str());
    if ( !t.is_open() ) {
        tout << "can't open\n";
        return;
    }
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), tout) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                tout << reportError(str.c_str(), err.at.line, err.at.column, err.what, err.cerr );
            }
        } else {
            if ( outputProgramCode )
                tout << *program << "\n";
            string str2;
            Context ctx(&str2);
            program->simulate(ctx, tout);
            if ( auto fnTest = ctx.findFunction(mainFnName.c_str()) ) {
                ctx.restart();
                ctx.eval(fnTest, nullptr);
            } else {
                tout << "function '"  << mainFnName << " ' not found\n";
            }
        }
    }
}

void print_help() {
    tout << "daScript [scriptName1] {scriptName2} .. {-main mainFnName} {-log}\n";
}

int main(int argc, const char * argv[]) {
    if ( argc<=1 ) {
        tout << "daScript [scriptName1] {scriptName2} .. {-main mainFnName} {-log}\n";
        return -1;
    }
    vector<string> files;
    string mainName = "main";
    bool outputProgramCode = false;
    for ( int i=1; i < argc;  ) {
        if ( argv[i][0]=='-' ) {
            if ( i+1 >= argc ) {
                print_help();
                return -1;
            } else {
                string cmd(argv[i]+1);
                if ( cmd=="main" ) {
                    mainName = argv[i+1];
                    i += 2;
                } else if ( cmd=="log" ) {
                    outputProgramCode = true;
                    i ++;
                } else {
                    print_help();
                    return -1;
                }
            }
        } else {
            files.push_back(argv[i]);
            i ++;
        }
    }
    // register modules
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Math);
    // compile and run
    for ( const auto & fn : files ) {
        compile_and_run(fn, mainName, outputProgramCode);
    }
    // and done
    Module::Shutdown();
    return 0;
}


