// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#pragma once
namespace das {
class Module_dasClangBind : public Module {
public:
	Module_dasClangBind();
protected:
	void initMain ( ModuleLibrary & lib );
	virtual ModuleAotType aotRequire ( TextWriter & tw ) const override;
	#include "dasClangBind.func.decl.inc"
};
}

