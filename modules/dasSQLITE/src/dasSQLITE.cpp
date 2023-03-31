// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasSQLITE.h"
#include "need_dasSQLITE.h"
#include "dasSQLITE.struct.impl.inc"
namespace das {
#include "dasSQLITE.enum.class.inc"
#include "dasSQLITE.struct.class.inc"
#include "dasSQLITE.func.aot.inc"
Module_dasSQLITE::Module_dasSQLITE() : Module("sqlite") {
}
bool Module_dasSQLITE::initDependencies() {
	if ( initialized ) return true;
	initialized = true;
	lib.addModule(this);
	lib.addBuiltInModule();
	#include "dasSQLITE.const.inc"
	#include "dasSQLITE.enum.add.inc"
	#include "dasSQLITE.dummy.add.inc"
	#include "dasSQLITE.struct.add.inc"
	#include "dasSQLITE.struct.postadd.inc"
	#include "dasSQLITE.alias.add.inc"
	#include "dasSQLITE.func.reg.inc"
	initMain();
	return true;
}
}
REGISTER_MODULE_IN_NAMESPACE(Module_dasSQLITE,das);

