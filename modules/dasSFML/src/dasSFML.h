// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#pragma once
#include "cb_dasSFML.h"
namespace das {
class Module_dasSFML : public Module {
public:
	Module_dasSFML();
protected:
virtual bool initDependencies() override;
	void initMain ();
	void initAotAlias ();
	virtual ModuleAotType aotRequire ( TextWriter & tw ) const override;
	#include "dasSFML.func.decl.inc"
public:
	ModuleLibrary lib;
	bool initialized = false;
};
}

