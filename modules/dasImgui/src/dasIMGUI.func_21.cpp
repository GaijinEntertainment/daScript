// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasIMGUI.h"
#include "need_dasIMGUI.h"
namespace das {
#include "dasIMGUI.func.aot.decl.inc"
void Module_dasIMGUI::initFunctions_21() {
	using _method_29 = das::das_call_member< bool (ImGuiTextFilter::ImGuiTextRange::*)() const,&ImGuiTextFilter::ImGuiTextRange::empty >;
// from imgui.h:2499:25
	makeExtern<DAS_CALL_METHOD(_method_29), SimNode_ExtFuncCall , imguiTempFn>(lib,"empty","das_call_member< bool (ImGuiTextFilter::ImGuiTextRange::*)() const , &ImGuiTextFilter::ImGuiTextRange::empty >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_30 = das::das_call_member< void (ImGuiTextFilter::ImGuiTextRange::*)(char,ImVector<ImGuiTextFilter::ImGuiTextRange> *) const,&ImGuiTextFilter::ImGuiTextRange::split >;
// from imgui.h:2500:25
	makeExtern<DAS_CALL_METHOD(_method_30), SimNode_ExtFuncCall , imguiTempFn>(lib,"split","das_call_member< void (ImGuiTextFilter::ImGuiTextRange::*)(char,ImVector<ImGuiTextFilter::ImGuiTextRange> *) const , &ImGuiTextFilter::ImGuiTextRange::split >::invoke")
		->args({"self","separator","out"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiTextBuffer>(*this,lib,"ImGuiTextBuffer","ImGuiTextBuffer");
	using _method_31 = das::das_call_member< char (ImGuiTextBuffer::*)(int) const,&ImGuiTextBuffer::operator[] >;
// from imgui.h:2515:25
	makeExtern<DAS_CALL_METHOD(_method_31), SimNode_ExtFuncCall , imguiTempFn>(lib,"[]","das_call_member< char (ImGuiTextBuffer::*)(int) const , &ImGuiTextBuffer::operator[] >::invoke")
		->args({"self","i"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_32 = das::das_call_member< const char * (ImGuiTextBuffer::*)() const,&ImGuiTextBuffer::begin >;
// from imgui.h:2516:25
	makeExtern<DAS_CALL_METHOD(_method_32), SimNode_ExtFuncCall , imguiTempFn>(lib,"begin","das_call_member< const char * (ImGuiTextBuffer::*)() const , &ImGuiTextBuffer::begin >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_33 = das::das_call_member< const char * (ImGuiTextBuffer::*)() const,&ImGuiTextBuffer::end >;
// from imgui.h:2517:25
	makeExtern<DAS_CALL_METHOD(_method_33), SimNode_ExtFuncCall , imguiTempFn>(lib,"end","das_call_member< const char * (ImGuiTextBuffer::*)() const , &ImGuiTextBuffer::end >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_34 = das::das_call_member< int (ImGuiTextBuffer::*)() const,&ImGuiTextBuffer::size >;
// from imgui.h:2518:25
	makeExtern<DAS_CALL_METHOD(_method_34), SimNode_ExtFuncCall , imguiTempFn>(lib,"size","das_call_member< int (ImGuiTextBuffer::*)() const , &ImGuiTextBuffer::size >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_35 = das::das_call_member< bool (ImGuiTextBuffer::*)() const,&ImGuiTextBuffer::empty >;
// from imgui.h:2519:25
	makeExtern<DAS_CALL_METHOD(_method_35), SimNode_ExtFuncCall , imguiTempFn>(lib,"empty","das_call_member< bool (ImGuiTextBuffer::*)() const , &ImGuiTextBuffer::empty >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_36 = das::das_call_member< void (ImGuiTextBuffer::*)(),&ImGuiTextBuffer::clear >;
// from imgui.h:2520:25
	makeExtern<DAS_CALL_METHOD(_method_36), SimNode_ExtFuncCall , imguiTempFn>(lib,"clear","das_call_member< void (ImGuiTextBuffer::*)() , &ImGuiTextBuffer::clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_37 = das::das_call_member< void (ImGuiTextBuffer::*)(int),&ImGuiTextBuffer::reserve >;
// from imgui.h:2521:25
	makeExtern<DAS_CALL_METHOD(_method_37), SimNode_ExtFuncCall , imguiTempFn>(lib,"reserve","das_call_member< void (ImGuiTextBuffer::*)(int) , &ImGuiTextBuffer::reserve >::invoke")
		->args({"self","capacity"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_38 = das::das_call_member< const char * (ImGuiTextBuffer::*)() const,&ImGuiTextBuffer::c_str >;
// from imgui.h:2522:25
	makeExtern<DAS_CALL_METHOD(_method_38), SimNode_ExtFuncCall , imguiTempFn>(lib,"c_str","das_call_member< const char * (ImGuiTextBuffer::*)() const , &ImGuiTextBuffer::c_str >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiStorage::ImGuiStoragePair,unsigned int,int>(*this,lib,"ImGuiStoragePair","ImGuiStorage::ImGuiStoragePair")
		->args({"_key","_val"});
	addCtorAndUsing<ImGuiStorage::ImGuiStoragePair,unsigned int,float>(*this,lib,"ImGuiStoragePair","ImGuiStorage::ImGuiStoragePair")
		->args({"_key","_val"});
	addCtorAndUsing<ImGuiStorage::ImGuiStoragePair,unsigned int,void *>(*this,lib,"ImGuiStoragePair","ImGuiStorage::ImGuiStoragePair")
		->args({"_key","_val"});
	using _method_39 = das::das_call_member< void (ImGuiStorage::*)(),&ImGuiStorage::Clear >;
// from imgui.h:2553:25
	makeExtern<DAS_CALL_METHOD(_method_39), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImGuiStorage::*)() , &ImGuiStorage::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_40 = das::das_call_member< int (ImGuiStorage::*)(unsigned int,int) const,&ImGuiStorage::GetInt >;
// from imgui.h:2554:25
	makeExtern<DAS_CALL_METHOD(_method_40), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetInt","das_call_member< int (ImGuiStorage::*)(unsigned int,int) const , &ImGuiStorage::GetInt >::invoke")
		->args({"self","key","default_val"})
		->arg_init(2,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_41 = das::das_call_member< void (ImGuiStorage::*)(unsigned int,int),&ImGuiStorage::SetInt >;
// from imgui.h:2555:25
	makeExtern<DAS_CALL_METHOD(_method_41), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetInt","das_call_member< void (ImGuiStorage::*)(unsigned int,int) , &ImGuiStorage::SetInt >::invoke")
		->args({"self","key","val"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_42 = das::das_call_member< bool (ImGuiStorage::*)(unsigned int,bool) const,&ImGuiStorage::GetBool >;
// from imgui.h:2556:25
	makeExtern<DAS_CALL_METHOD(_method_42), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetBool","das_call_member< bool (ImGuiStorage::*)(unsigned int,bool) const , &ImGuiStorage::GetBool >::invoke")
		->args({"self","key","default_val"})
		->arg_init(2,new ExprConstBool(false))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_43 = das::das_call_member< void (ImGuiStorage::*)(unsigned int,bool),&ImGuiStorage::SetBool >;
// from imgui.h:2557:25
	makeExtern<DAS_CALL_METHOD(_method_43), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetBool","das_call_member< void (ImGuiStorage::*)(unsigned int,bool) , &ImGuiStorage::SetBool >::invoke")
		->args({"self","key","val"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_44 = das::das_call_member< float (ImGuiStorage::*)(unsigned int,float) const,&ImGuiStorage::GetFloat >;
// from imgui.h:2558:25
	makeExtern<DAS_CALL_METHOD(_method_44), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFloat","das_call_member< float (ImGuiStorage::*)(unsigned int,float) const , &ImGuiStorage::GetFloat >::invoke")
		->args({"self","key","default_val"})
		->arg_init(2,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
}
}

