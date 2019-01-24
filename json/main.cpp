#include "daScript/daScript.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace das;

bool unit_test ( const string & fn ) {
	cout << fn << " ";
	string str;
	ifstream t(fn);
	if ( !t.is_open() ) {
		cout << "not found\n";
		return false;
	}
	t.seekg(0, ios::end);
	str.reserve(t.tellg());
	t.seekg(0, ios::beg);
	str.assign((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	if ( auto program = parseDaScript(str.c_str(), cout) ) {
		if ( program->failed() ) {
			cout << "failed to compile\n";
			for ( auto & err : program->errors ) {
				cout << reportError(&str, err.at.line, err.at.column, err.what, err.cerr );
			}
			return false;
		} else {
			Context ctx(&str);
			program->simulate(ctx, cout);
			if ( auto fnTest = ctx.findFunction("test") ) {
				ctx.restart();
				bool result = cast<bool>::to(ctx.eval(fnTest, nullptr));
				if ( auto ex = ctx.getException() ) {
					cout << "exception: " << ex << "\n";
					return false;
				}
				if ( !result ) {
					cout << "failed\n";
					return false;
				}
				cout << "ok\n";
				return true;
			} else {
				cout << "function 'test' not found\n";
				return false;
			}
		}
	} else {
		return false;
	}
}


int main() {
#ifdef _MSC_VER
#define	TEST_PATH "../"
#else
#define TEST_PATH "../../"
#endif
	// register modules
	NEED_MODULE(Module_BuiltIn);
	NEED_MODULE(Module_Math);
	NEED_MODULE(Module_RapidJson);
	unit_test(TEST_PATH "json/test.das");
	Module::Shutdown();
	return 0;
}
