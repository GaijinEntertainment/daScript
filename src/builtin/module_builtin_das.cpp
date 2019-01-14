#include "daScript/misc/platform.h"

#include "module_builtin.h"

namespace das {

#include "builtin.das.inc"


	bool Module_BuiltIn::appendCompiledFunctions() {
		stringstream issues;
		const char * src = (const char *) builtin_das;
		if (auto program = parseDaScript(src, issues)) {
			if (program->failed()) {
#if 0
				string str(src);
				for (auto & err : program->errors) {
					issues << reportError(&str, err.at.line, err.at.column, err.what, err.cerr);
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