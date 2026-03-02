// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#pragma once
#include "cb_dasGLFW.h"
namespace das {
class Module_dasGLFW : public Module {
public:
	Module_dasGLFW();
protected:
virtual bool initDependencies() override;
	void initMain ();
	virtual ModuleAotType aotRequire ( TextWriter & tw ) const override;
	#include "dasGLFW.func.decl.inc"
public:
	ModuleLibrary lib;
	bool initialized = false;
	public:
		virtual ~Module_dasGLFW();
	public:
		static thread_local das_map<void *, GlswCallbacks> g_Callbacks;
};
}

