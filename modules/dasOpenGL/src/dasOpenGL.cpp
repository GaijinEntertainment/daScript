// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasOpenGL.h"
#include "need_dasOpenGL.h"
#include "dasOpenGL.struct.impl.inc"
namespace das {
#include "dasOpenGL.enum.class.inc"
#include "dasOpenGL.struct.class.inc"
#include "dasOpenGL.func.aot.inc"
Module_dasOpenGL::Module_dasOpenGL() : Module("opengl") {
}
bool Module_dasOpenGL::initDependencies() {
	if ( initialized ) return true;
	initialized = true;
	lib.addModule(this);
	lib.addBuiltInModule();
	#include "dasOpenGL.const.inc"
	#include "dasOpenGL.enum.add.inc"
	#include "dasOpenGL.dummy.add.inc"
	#include "dasOpenGL.struct.add.inc"
	#include "dasOpenGL.struct.postadd.inc"
	#include "dasOpenGL.alias.add.inc"
	#include "dasOpenGL.func.reg.inc"
	initMain();
	return true;
}
REGISTER_DYN_MODULE(Module_dasOpenGL,Module_dasOpenGL);
}
REGISTER_MODULE_IN_NAMESPACE(Module_dasOpenGL,das);

