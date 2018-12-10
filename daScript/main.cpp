#include "precomp.h"

#include "ast.h"
#include "interop.h"

using namespace std;
using namespace yzg;


void compile_and_run ( const string & fn, const string & mainFnName, bool outputProgramCode ) {
    string str;
    ifstream t(fn);
    if ( !t.is_open() ) {
        cout << "can't open " << endl;
        return;
    }
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    if ( auto program = parseDaScript(str.c_str(), nullptr) ) {
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
            }
        } else {
            if ( outputProgramCode )
                cout << *program << "\n";
            Context ctx(&str);
            program->simulate(ctx);
            int fnTest = ctx.findFunction(mainFnName.c_str());
            if ( fnTest != -1 ) {
                ctx.restart();
                ctx.eval(fnTest, nullptr);
            } else {
                cout << "function '"  << mainFnName << " ' not found\n";
            }
        }
    }
}

void print_help() {
    cout << "daScript [scriptName1] {scriptName2} .. {-main mainFnName} {-log}\n";
}

int main(int argc, const char * argv[]) {
    if ( argc<=1 ) {
        cout << "daScript [scriptName1] {scriptName2} .. {-main mainFnName} {-log}\n";
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
    for ( const auto & fn : files ) {
        compile_and_run(fn, mainName, outputProgramCode);
    }
    return 0;
}


