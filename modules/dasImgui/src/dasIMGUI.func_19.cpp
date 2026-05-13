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
void Module_dasIMGUI::initFunctions_19() {
// from imgui.h:1028:33
	makeExtern< ImGuiViewport * (*)(void *) , ImGui::FindViewportByPlatformHandle , SimNode_ExtFuncCall , imguiTempFn>(lib,"FindViewportByPlatformHandle","ImGui::FindViewportByPlatformHandle")
		->args({"platform_handle"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiTableSortSpecs>(*this,lib,"ImGuiTableSortSpecs","ImGuiTableSortSpecs");
	addCtorAndUsing<ImGuiTableColumnSortSpecs>(*this,lib,"ImGuiTableColumnSortSpecs","ImGuiTableColumnSortSpecs");
	addCtorAndUsing<ImGuiStyle>(*this,lib,"ImGuiStyle","ImGuiStyle");
	using _method_1 = das::das_call_member< void (ImGuiStyle::*)(float),&ImGuiStyle::ScaleAllSizes >;
// from imgui.h:2131:20
	makeExtern<DAS_CALL_METHOD(_method_1), SimNode_ExtFuncCall , imguiTempFn>(lib,"ScaleAllSizes","das_call_member< void (ImGuiStyle::*)(float) , &ImGuiStyle::ScaleAllSizes >::invoke")
		->args({"self","scale_factor"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_2 = das::das_call_member< void (ImGuiIO::*)(ImGuiKey,bool),&ImGuiIO::AddKeyEvent >;
// from imgui.h:2261:21
	makeExtern<DAS_CALL_METHOD(_method_2), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddKeyEvent","das_call_member< void (ImGuiIO::*)(ImGuiKey,bool) , &ImGuiIO::AddKeyEvent >::invoke")
		->args({"self","key","down"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_3 = das::das_call_member< void (ImGuiIO::*)(ImGuiKey,bool,float),&ImGuiIO::AddKeyAnalogEvent >;
// from imgui.h:2262:21
	makeExtern<DAS_CALL_METHOD(_method_3), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddKeyAnalogEvent","das_call_member< void (ImGuiIO::*)(ImGuiKey,bool,float) , &ImGuiIO::AddKeyAnalogEvent >::invoke")
		->args({"self","key","down","v"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_4 = das::das_call_member< void (ImGuiIO::*)(float,float),&ImGuiIO::AddMousePosEvent >;
// from imgui.h:2263:21
	makeExtern<DAS_CALL_METHOD(_method_4), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMousePosEvent","das_call_member< void (ImGuiIO::*)(float,float) , &ImGuiIO::AddMousePosEvent >::invoke")
		->args({"self","x","y"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_5 = das::das_call_member< void (ImGuiIO::*)(int,bool),&ImGuiIO::AddMouseButtonEvent >;
// from imgui.h:2264:21
	makeExtern<DAS_CALL_METHOD(_method_5), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseButtonEvent","das_call_member< void (ImGuiIO::*)(int,bool) , &ImGuiIO::AddMouseButtonEvent >::invoke")
		->args({"self","button","down"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_6 = das::das_call_member< void (ImGuiIO::*)(float,float),&ImGuiIO::AddMouseWheelEvent >;
// from imgui.h:2265:21
	makeExtern<DAS_CALL_METHOD(_method_6), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseWheelEvent","das_call_member< void (ImGuiIO::*)(float,float) , &ImGuiIO::AddMouseWheelEvent >::invoke")
		->args({"self","wheel_x","wheel_y"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_7 = das::das_call_member< void (ImGuiIO::*)(ImGuiMouseSource),&ImGuiIO::AddMouseSourceEvent >;
// from imgui.h:2266:21
	makeExtern<DAS_CALL_METHOD(_method_7), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseSourceEvent","das_call_member< void (ImGuiIO::*)(ImGuiMouseSource) , &ImGuiIO::AddMouseSourceEvent >::invoke")
		->args({"self","source"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_8 = das::das_call_member< void (ImGuiIO::*)(unsigned int),&ImGuiIO::AddMouseViewportEvent >;
// from imgui.h:2267:21
	makeExtern<DAS_CALL_METHOD(_method_8), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddMouseViewportEvent","das_call_member< void (ImGuiIO::*)(unsigned int) , &ImGuiIO::AddMouseViewportEvent >::invoke")
		->args({"self","id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_9 = das::das_call_member< void (ImGuiIO::*)(bool),&ImGuiIO::AddFocusEvent >;
// from imgui.h:2268:21
	makeExtern<DAS_CALL_METHOD(_method_9), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFocusEvent","das_call_member< void (ImGuiIO::*)(bool) , &ImGuiIO::AddFocusEvent >::invoke")
		->args({"self","focused"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_10 = das::das_call_member< void (ImGuiIO::*)(unsigned int),&ImGuiIO::AddInputCharacter >;
// from imgui.h:2269:21
	makeExtern<DAS_CALL_METHOD(_method_10), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharacter","das_call_member< void (ImGuiIO::*)(unsigned int) , &ImGuiIO::AddInputCharacter >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_11 = das::das_call_member< void (ImGuiIO::*)(unsigned short),&ImGuiIO::AddInputCharacterUTF16 >;
// from imgui.h:2270:21
	makeExtern<DAS_CALL_METHOD(_method_11), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharacterUTF16","das_call_member< void (ImGuiIO::*)(unsigned short) , &ImGuiIO::AddInputCharacterUTF16 >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_12 = das::das_call_member< void (ImGuiIO::*)(const char *),&ImGuiIO::AddInputCharactersUTF8 >;
// from imgui.h:2271:21
	makeExtern<DAS_CALL_METHOD(_method_12), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddInputCharactersUTF8","das_call_member< void (ImGuiIO::*)(const char *) , &ImGuiIO::AddInputCharactersUTF8 >::invoke")
		->args({"self","str"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_13 = das::das_call_member< void (ImGuiIO::*)(ImGuiKey,int,int,int),&ImGuiIO::SetKeyEventNativeData >;
// from imgui.h:2273:21
	makeExtern<DAS_CALL_METHOD(_method_13), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetKeyEventNativeData","das_call_member< void (ImGuiIO::*)(ImGuiKey,int,int,int) , &ImGuiIO::SetKeyEventNativeData >::invoke")
		->args({"self","key","native_keycode","native_scancode","native_legacy_index"})
		->arg_init(4,new ExprConstInt(-1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_14 = das::das_call_member< void (ImGuiIO::*)(bool),&ImGuiIO::SetAppAcceptingEvents >;
// from imgui.h:2274:21
	makeExtern<DAS_CALL_METHOD(_method_14), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetAppAcceptingEvents","das_call_member< void (ImGuiIO::*)(bool) , &ImGuiIO::SetAppAcceptingEvents >::invoke")
		->args({"self","accepting_events"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_15 = das::das_call_member< void (ImGuiIO::*)(),&ImGuiIO::ClearEventsQueue >;
// from imgui.h:2275:21
	makeExtern<DAS_CALL_METHOD(_method_15), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearEventsQueue","das_call_member< void (ImGuiIO::*)() , &ImGuiIO::ClearEventsQueue >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_16 = das::das_call_member< void (ImGuiIO::*)(),&ImGuiIO::ClearInputKeys >;
// from imgui.h:2276:21
	makeExtern<DAS_CALL_METHOD(_method_16), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearInputKeys","das_call_member< void (ImGuiIO::*)() , &ImGuiIO::ClearInputKeys >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

