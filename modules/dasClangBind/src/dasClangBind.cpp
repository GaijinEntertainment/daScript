// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasClangBind.h"
#include "need_dasClangBind.h"
#include "dasClangBind.struct.impl.inc"
namespace das {
#include "dasClangBind.enum.class.inc"
#include "dasClangBind.struct.class.inc"
Module_dasClangBind::Module_dasClangBind() : Module("cbind") {
}
bool Module_dasClangBind::initDependencies() {
	if ( initialized ) return true;
	initialized = true;
	lib.addModule(this);
	lib.addBuiltInModule();
	#include "dasClangBind.enum.add.inc"
	#include "dasClangBind.dummy.add.inc"
	#include "dasClangBind.struct.add.inc"
	#include "dasClangBind.struct.postadd.inc"
	#include "dasClangBind.alias.add.inc"
	#include "dasClangBind.func.reg.inc"
	initMain();
	return true;
}
}
REGISTER_MODULE_IN_NAMESPACE(Module_dasClangBind,das);

