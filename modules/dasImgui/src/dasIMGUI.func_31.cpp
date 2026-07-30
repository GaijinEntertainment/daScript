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
void Module_dasIMGUI::initFunctions_31() {
	addCtorAndUsing<ImGuiViewport>(*this,lib,"ImGuiViewport","ImGuiViewport");
	using _method_200 = das::das_call_member< ImVec2 (ImGuiViewport::*)() const,&ImGuiViewport::GetCenter >;
// from imgui.h:4112:25
	makeExtern<DAS_CALL_METHOD(_method_200), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCenter","das_call_member< ImVec2 (ImGuiViewport::*)() const , &ImGuiViewport::GetCenter >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_201 = das::das_call_member< ImVec2 (ImGuiViewport::*)() const,&ImGuiViewport::GetWorkCenter >;
// from imgui.h:4113:25
	makeExtern<DAS_CALL_METHOD(_method_201), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetWorkCenter","das_call_member< ImVec2 (ImGuiViewport::*)() const , &ImGuiViewport::GetWorkCenter >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiPlatformIO>(*this,lib,"ImGuiPlatformIO","ImGuiPlatformIO");
	using _method_202 = das::das_call_member< void (ImGuiPlatformIO::*)(),&ImGuiPlatformIO::ClearPlatformHandlers >;
// from imgui.h:4273:20
	makeExtern<DAS_CALL_METHOD(_method_202), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearPlatformHandlers","das_call_member< void (ImGuiPlatformIO::*)() , &ImGuiPlatformIO::ClearPlatformHandlers >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_203 = das::das_call_member< void (ImGuiPlatformIO::*)(),&ImGuiPlatformIO::ClearRendererHandlers >;
// from imgui.h:4274:20
	makeExtern<DAS_CALL_METHOD(_method_203), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearRendererHandlers","das_call_member< void (ImGuiPlatformIO::*)() , &ImGuiPlatformIO::ClearRendererHandlers >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiPlatformMonitor>(*this,lib,"ImGuiPlatformMonitor","ImGuiPlatformMonitor");
	addCtorAndUsing<ImGuiPlatformImeData>(*this,lib,"ImGuiPlatformImeData","ImGuiPlatformImeData");
// from imgui_internal.h:3510:29
	makeExtern< unsigned int (*)() , ImGui::GetFocusID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFocusID","ImGui::GetFocusID")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3514:29
	makeExtern< unsigned int (*)() , ImGui::GetHoveredID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetHoveredID","ImGui::GetHoveredID")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3516:29
	makeExtern< void (*)(unsigned int) , ImGui::KeepAliveID , SimNode_ExtFuncCall , imguiTempFn>(lib,"KeepAliveID","ImGui::KeepAliveID")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3517:29
	makeExtern< void (*)(unsigned int) , ImGui::MarkItemEdited , SimNode_ExtFuncCall , imguiTempFn>(lib,"MarkItemEdited","ImGui::MarkItemEdited")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3518:29
	makeExtern< void (*)(unsigned int) , ImGui::PushOverrideID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushOverrideID","ImGui::PushOverrideID")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3519:29
	makeExtern< unsigned int (*)(const char *,const char *,unsigned int) , ImGui::GetIDWithSeed , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetIDWithSeed","ImGui::GetIDWithSeed")
		->args({"str_id_begin","str_id_end","seed"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3520:29
	makeExtern< unsigned int (*)(int,unsigned int) , ImGui::GetIDWithSeed , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetIDWithSeed","ImGui::GetIDWithSeed")
		->args({"n","seed"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3523:29
	makeExtern< void (*)(const ImVec2 &,float) , ImGui::ItemSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"ItemSize","ImGui::ItemSize")
		->args({"size","text_baseline_y"})
		->arg_init(1,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3524:29
	makeExtern< void (*)(const ImRect &,float) , ImGui::ItemSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"ItemSize","ImGui::ItemSize")
		->args({"bb","text_baseline_y"})
		->arg_init(1,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3526:29
	makeExtern< bool (*)(const ImRect &,unsigned int,int) , ImGui::ItemHoverable , SimNode_ExtFuncCall , imguiTempFn>(lib,"ItemHoverable","ImGui::ItemHoverable")
		->args({"bb","id","item_flags"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3528:29
	makeExtern< bool (*)(const ImRect &,unsigned int) , ImGui::IsClippedEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsClippedEx","ImGui::IsClippedEx")
		->args({"bb","id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3530:29
	makeExtern< ImVec2 (*)(ImVec2,float,float) , ImGui::CalcItemSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcItemSize","ImGui::CalcItemSize")
		->args({"size","default_w","default_h"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

