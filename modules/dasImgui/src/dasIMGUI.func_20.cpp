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
	using _method_7 = das::das_call_member< void (ImGuiIO::*)(float,float),&ImGuiIO::AddMouseWheelEvent >;
// from imgui.h:2626:21
	makeExtern<DAS_CALL_METHOD(_method_7), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseWheelEvent","das_call_member< void (ImGuiIO::*)(float,float) , &ImGuiIO::AddMouseWheelEvent >::invoke")
		->args({"self","wheel_x","wheel_y"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_8 = das::das_call_member< void (ImGuiIO::*)(ImGuiMouseSource),&ImGuiIO::AddMouseSourceEvent >;
// from imgui.h:2627:21
	makeExtern<DAS_CALL_METHOD(_method_8), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseSourceEvent","das_call_member< void (ImGuiIO::*)(ImGuiMouseSource) , &ImGuiIO::AddMouseSourceEvent >::invoke")
		->args({"self","source"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_9 = das::das_call_member< void (ImGuiIO::*)(unsigned int),&ImGuiIO::AddMouseViewportEvent >;
// from imgui.h:2628:21
	makeExtern<DAS_CALL_METHOD(_method_9), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseViewportEvent","das_call_member< void (ImGuiIO::*)(unsigned int) , &ImGuiIO::AddMouseViewportEvent >::invoke")
		->args({"self","id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_10 = das::das_call_member< void (ImGuiIO::*)(bool),&ImGuiIO::AddFocusEvent >;
// from imgui.h:2629:21
	makeExtern<DAS_CALL_METHOD(_method_10), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFocusEvent","das_call_member< void (ImGuiIO::*)(bool) , &ImGuiIO::AddFocusEvent >::invoke")
		->args({"self","focused"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_11 = das::das_call_member< void (ImGuiIO::*)(unsigned int),&ImGuiIO::AddInputCharacter >;
// from imgui.h:2630:21
	makeExtern<DAS_CALL_METHOD(_method_11), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharacter","das_call_member< void (ImGuiIO::*)(unsigned int) , &ImGuiIO::AddInputCharacter >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_12 = das::das_call_member< void (ImGuiIO::*)(unsigned short),&ImGuiIO::AddInputCharacterUTF16 >;
// from imgui.h:2631:21
	makeExtern<DAS_CALL_METHOD(_method_12), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharacterUTF16","das_call_member< void (ImGuiIO::*)(unsigned short) , &ImGuiIO::AddInputCharacterUTF16 >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_13 = das::das_call_member< void (ImGuiIO::*)(const char *),&ImGuiIO::AddInputCharactersUTF8 >;
// from imgui.h:2632:21
	makeExtern<DAS_CALL_METHOD(_method_13), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharactersUTF8","das_call_member< void (ImGuiIO::*)(const char *) , &ImGuiIO::AddInputCharactersUTF8 >::invoke")
		->args({"self","str"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_14 = das::das_call_member< void (ImGuiIO::*)(ImGuiKey,int,int,int),&ImGuiIO::SetKeyEventNativeData >;
// from imgui.h:2634:21
	makeExtern<DAS_CALL_METHOD(_method_14), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetKeyEventNativeData","das_call_member< void (ImGuiIO::*)(ImGuiKey,int,int,int) , &ImGuiIO::SetKeyEventNativeData >::invoke")
		->args({"self","key","native_keycode","native_scancode","native_legacy_index"})
		->arg_init(4,new ExprConstInt(-1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_15 = das::das_call_member< void (ImGuiIO::*)(bool),&ImGuiIO::SetAppAcceptingEvents >;
// from imgui.h:2635:21
	makeExtern<DAS_CALL_METHOD(_method_15), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetAppAcceptingEvents","das_call_member< void (ImGuiIO::*)(bool) , &ImGuiIO::SetAppAcceptingEvents >::invoke")
		->args({"self","accepting_events"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_16 = das::das_call_member< void (ImGuiIO::*)(),&ImGuiIO::ClearEventsQueue >;
// from imgui.h:2636:21
	makeExtern<DAS_CALL_METHOD(_method_16), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearEventsQueue","das_call_member< void (ImGuiIO::*)() , &ImGuiIO::ClearEventsQueue >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_17 = das::das_call_member< void (ImGuiIO::*)(),&ImGuiIO::ClearInputKeys >;
// from imgui.h:2637:21
	makeExtern<DAS_CALL_METHOD(_method_17), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearInputKeys","das_call_member< void (ImGuiIO::*)() , &ImGuiIO::ClearInputKeys >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_18 = das::das_call_member< void (ImGuiIO::*)(),&ImGuiIO::ClearInputMouse >;
// from imgui.h:2638:21
	makeExtern<DAS_CALL_METHOD(_method_18), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearInputMouse","das_call_member< void (ImGuiIO::*)() , &ImGuiIO::ClearInputMouse >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiIO>(*this,lib,"ImGuiIO","ImGuiIO");
	addCtorAndUsing<ImGuiInputTextCallbackData>(*this,lib,"ImGuiInputTextCallbackData","ImGuiInputTextCallbackData");
	using _method_19 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(int,int),&ImGuiInputTextCallbackData::DeleteChars >;
// from imgui.h:2772:25
	makeExtern<DAS_CALL_METHOD(_method_19), SimNode_ExtFuncCall , imguiTempFn>(lib,"DeleteChars","das_call_member< void (ImGuiInputTextCallbackData::*)(int,int) , &ImGuiInputTextCallbackData::DeleteChars >::invoke")
		->args({"self","pos","bytes_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_20 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(),&ImGuiInputTextCallbackData::SelectAll >;
// from imgui.h:2774:25
	makeExtern<DAS_CALL_METHOD(_method_20), SimNode_ExtFuncCall , imguiTempFn>(lib,"SelectAll","das_call_member< void (ImGuiInputTextCallbackData::*)() , &ImGuiInputTextCallbackData::SelectAll >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_21 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(int,int),&ImGuiInputTextCallbackData::SetSelection >;
// from imgui.h:2775:25
	makeExtern<DAS_CALL_METHOD(_method_21), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetSelection","das_call_member< void (ImGuiInputTextCallbackData::*)(int,int) , &ImGuiInputTextCallbackData::SetSelection >::invoke")
		->args({"self","s","e"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_22 = das::das_call_member< void (ImGuiInputTextCallbackData::*)(),&ImGuiInputTextCallbackData::ClearSelection >;
// from imgui.h:2776:25
	makeExtern<DAS_CALL_METHOD(_method_22), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearSelection","das_call_member< void (ImGuiInputTextCallbackData::*)() , &ImGuiInputTextCallbackData::ClearSelection >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_23 = das::das_call_member< bool (ImGuiInputTextCallbackData::*)() const,&ImGuiInputTextCallbackData::HasSelection >;
// from imgui.h:2777:25
	makeExtern<DAS_CALL_METHOD(_method_23), SimNode_ExtFuncCall , imguiTempFn>(lib,"HasSelection","das_call_member< bool (ImGuiInputTextCallbackData::*)() const , &ImGuiInputTextCallbackData::HasSelection >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiWindowClass>(*this,lib,"ImGuiWindowClass","ImGuiWindowClass");
}
}

