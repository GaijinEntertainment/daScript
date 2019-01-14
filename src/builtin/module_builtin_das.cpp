#include "daScript/misc/platform.h"

#include "module_builtin.h"

namespace das {

#include "builtin.das.inc"


	bool Module_BuiltIn::appendCompiledFunctions() {
		stringstream issues;
        string src_str ( (const char *) builtin_das, size_t(builtin_das_len) );
		if (auto program = parseDaScript(src_str.c_str(), issues)) {
			if (program->failed()) {
#if 1
				for (auto & err : program->errors) {
					issues << reportError(&src_str, err.at.line, err.at.column, err.what, err.cerr);
				}
				printf("%s\n", issues.str().c_str());
#endif
				assert(0 && "this happens when builtin module does not compile");
				return false;
			}
			// ok, now let's rip content
			for (auto & gen : program->thisModule->generics) {
				addGeneric(gen.second);
			}
			for (auto & glob : program->thisModule->globals) {
				addVariable(glob.second);
			}
			for (auto & fun : program->thisModule->functions) {
				addFunction(fun.second);
			}
			return true;
		} else {
			assert(0 && "this happens when builtin module does not parse");
			return false;
		}
	}
}
