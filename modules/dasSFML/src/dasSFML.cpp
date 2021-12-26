// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasSFML.h"
#include "need_dasSFML.h"
#include "dasSFML.struct.impl.inc"
namespace das {
#include "dasSFML.enum.class.inc"
#include "dasSFML.struct.class.inc"
Module_dasSFML::Module_dasSFML() : Module("sfml") {
}
bool Module_dasSFML::initDependencies() {
	if ( initialized ) return true;
	initialized = true;
	lib.addModule(this);
	lib.addBuiltInModule();
	initAotAlias();
	#include "dasSFML.enum.add.inc"
	#include "dasSFML.dummy.add.inc"
	#include "dasSFML.struct.add.inc"
	#include "dasSFML.struct.postadd.inc"
	#include "dasSFML.alias.add.inc"
	#include "dasSFML.func.reg.inc"
	initMain();
	return true;
}
}
REGISTER_MODULE_IN_NAMESPACE(Module_dasSFML,das);

