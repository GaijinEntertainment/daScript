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
void Module_dasIMGUI::initFunctions_20() {
	addCtorAndUsing<ImGuiIO>(*this,lib,"ImGuiIO","ImGuiIO");
	addCtorAndUsing<ImGuiInputTextCallbackData>(*this,lib,"ImGuiInputTextCallbackData","ImGuiInputTextCallbackData");
	using _method_17 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(int,int),&ImGuiInputTextCallbackData::DeleteChars >;
// from imgui.h:2399:25
	makeExtern<DAS_CALL_METHOD(_method_17), SimNode_ExtFuncCall , imguiTempFn>(lib,"DeleteChars","das_call_member< void (ImGuiInputTextCallbackData::*)(int,int) , &ImGuiInputTextCallbackData::DeleteChars >::invoke")
		->args({"self","pos","bytes_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_18 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(),&ImGuiInputTextCallbackData::SelectAll >;
// from imgui.h:2401:25
	makeExtern<DAS_CALL_METHOD(_method_18), SimNode_ExtFuncCall , imguiTempFn>(lib,"SelectAll","das_call_member< void (ImGuiInputTextCallbackData::*)() , &ImGuiInputTextCallbackData::SelectAll >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_19 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(),&ImGuiInputTextCallbackData::ClearSelection >;
// from imgui.h:2402:25
	makeExtern<DAS_CALL_METHOD(_method_19), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearSelection","das_call_member< void (ImGuiInputTextCallbackData::*)() , &ImGuiInputTextCallbackData::ClearSelection >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_20 = das::das_call_member< bool (ImGuiInputTextCallbackData::*)() const,&ImGuiInputTextCallbackData::HasSelection >;
// from imgui.h:2403:25
	makeExtern<DAS_CALL_METHOD(_method_20), SimNode_ExtFuncCall , imguiTempFn>(lib,"HasSelection","das_call_member< bool (ImGuiInputTextCallbackData::*)() const , &ImGuiInputTextCallbackData::HasSelection >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiWindowClass>(*this,lib,"ImGuiWindowClass","ImGuiWindowClass");
	addCtorAndUsing<ImGuiPayload>(*this,lib,"ImGuiPayload","ImGuiPayload");
	using _method_21 = das::das_call_member< void (ImGuiPayload::*)(),&ImGuiPayload::Clear >;
// from imgui.h:2454:10
	makeExtern<DAS_CALL_METHOD(_method_21), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImGuiPayload::*)() , &ImGuiPayload::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_22 = das::das_call_member< bool (ImGuiPayload::*)(const char *) const,&ImGuiPayload::IsDataType >;
// from imgui.h:2455:10
	makeExtern<DAS_CALL_METHOD(_method_22), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsDataType","das_call_member< bool (ImGuiPayload::*)(const char *) const , &ImGuiPayload::IsDataType >::invoke")
		->args({"self","type"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_23 = das::das_call_member< bool (ImGuiPayload::*)() const,&ImGuiPayload::IsPreview >;
// from imgui.h:2456:10
	makeExtern<DAS_CALL_METHOD(_method_23), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsPreview","das_call_member< bool (ImGuiPayload::*)() const , &ImGuiPayload::IsPreview >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_24 = das::das_call_member< bool (ImGuiPayload::*)() const,&ImGuiPayload::IsDelivery >;
// from imgui.h:2457:10
	makeExtern<DAS_CALL_METHOD(_method_24), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsDelivery","das_call_member< bool (ImGuiPayload::*)() const , &ImGuiPayload::IsDelivery >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiOnceUponAFrame>(*this,lib,"ImGuiOnceUponAFrame","ImGuiOnceUponAFrame");
	addCtorAndUsing<ImGuiTextFilter,const char *>(*this,lib,"ImGuiTextFilter","ImGuiTextFilter")
		->args({"default_filter"})
		->arg_init(0,new ExprConstString(""));
	using _method_25 = das::das_call_member< bool (ImGuiTextFilter::*)(const char *,float),&ImGuiTextFilter::Draw >;
// from imgui.h:2485:25
	makeExtern<DAS_CALL_METHOD(_method_25), SimNode_ExtFuncCall , imguiTempFn>(lib,"Draw","das_call_member< bool (ImGuiTextFilter::*)(const char *,float) , &ImGuiTextFilter::Draw >::invoke")
		->args({"self","label","width"})
		->arg_init(1,new ExprConstString("Filter (inc,-exc)"))
		->arg_init(2,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_26 = das::das_call_member< void (ImGuiTextFilter::*)(),&ImGuiTextFilter::Build >;
// from imgui.h:2487:25
	makeExtern<DAS_CALL_METHOD(_method_26), SimNode_ExtFuncCall , imguiTempFn>(lib,"Build","das_call_member< void (ImGuiTextFilter::*)() , &ImGuiTextFilter::Build >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_27 = das::das_call_member< void (ImGuiTextFilter::*)(),&ImGuiTextFilter::Clear >;
// from imgui.h:2488:25
	makeExtern<DAS_CALL_METHOD(_method_27), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImGuiTextFilter::*)() , &ImGuiTextFilter::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_28 = das::das_call_member< bool (ImGuiTextFilter::*)() const,&ImGuiTextFilter::IsActive >;
// from imgui.h:2489:25
	makeExtern<DAS_CALL_METHOD(_method_28), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsActive","das_call_member< bool (ImGuiTextFilter::*)() const , &ImGuiTextFilter::IsActive >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiTextFilter::ImGuiTextRange>(*this,lib,"ImGuiTextRange","ImGuiTextFilter::ImGuiTextRange");
	addCtorAndUsing<ImGuiTextFilter::ImGuiTextRange,const char *,const char *>(*this,lib,"ImGuiTextRange","ImGuiTextFilter::ImGuiTextRange")
		->args({"_b","_e"});
}
}

