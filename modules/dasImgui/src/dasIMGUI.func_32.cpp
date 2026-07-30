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
void Module_dasIMGUI::initFunctions_32() {
// from imgui_internal.h:3531:29
	makeExtern< float (*)(const ImVec2 &,float) , ImGui::CalcWrapWidthForPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcWrapWidthForPos","ImGui::CalcWrapWidthForPos")
		->args({"pos","wrap_pos_x"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3542:29
	makeExtern< void (*)(int,int) , ImGui::LogBegin , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogBegin","ImGui::LogBegin")
		->args({"flags","auto_open_depth"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3545:29
	makeExtern< void (*)(const char *,const char *) , ImGui::LogSetNextTextDecoration , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogSetNextTextDecoration","ImGui::LogSetNextTextDecoration")
		->args({"prefix","suffix"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3551:29
	makeExtern< bool (*)(unsigned int,int) , ImGui::BeginPopupEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopupEx","ImGui::BeginPopupEx")
		->args({"id","extra_window_flags"})
		->arg_type(1,makeType<ImGuiWindowFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3553:29
	makeExtern< void (*)(unsigned int,int) , ImGui::OpenPopupEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"OpenPopupEx","ImGui::OpenPopupEx")
		->args({"id","popup_flags"})
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3554:29
	makeExtern< void (*)(int,bool) , ImGui::ClosePopupToLevel , SimNode_ExtFuncCall , imguiTempFn>(lib,"ClosePopupToLevel","ImGui::ClosePopupToLevel")
		->args({"remaining","restore_focus_to_window_under_popup"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3556:29
	makeExtern< void (*)() , ImGui::ClosePopupsExceptModals , SimNode_ExtFuncCall , imguiTempFn>(lib,"ClosePopupsExceptModals","ImGui::ClosePopupsExceptModals")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3557:29
	makeExtern< bool (*)(unsigned int,int) , ImGui::IsPopupOpen , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsPopupOpen","ImGui::IsPopupOpen")
		->args({"id","popup_flags"})
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3567:29
	makeExtern< bool (*)(int,int) , ImGui::BeginTooltipEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginTooltipEx","ImGui::BeginTooltipEx")
		->args({"tooltip_flags","extra_window_flags"})
		->arg_type(1,makeType<ImGuiWindowFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3568:29
	makeExtern< bool (*)() , ImGui::BeginTooltipHidden , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginTooltipHidden","ImGui::BeginTooltipHidden")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3572:29
	makeExtern< bool (*)(const char *,const char *,bool) , ImGui::BeginMenuEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginMenuEx","ImGui::BeginMenuEx")
		->args({"label","icon","enabled"})
		->arg_init(2,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3573:29
	makeExtern< bool (*)(const char *,const char *,const char *,bool,bool) , ImGui::MenuItemEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"MenuItemEx","ImGui::MenuItemEx")
		->args({"label","icon","shortcut","selected","enabled"})
		->arg_init(2,new ExprConstString(""))
		->arg_init(3,new ExprConstBool(false))
		->arg_init(4,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3576:29
	makeExtern< bool (*)(unsigned int,const ImRect &,int) , ImGui::BeginComboPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginComboPopup","ImGui::BeginComboPopup")
		->args({"popup_id","bb","flags"})
		->arg_type(2,makeType<ImGuiComboFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3577:29
	makeExtern< bool (*)() , ImGui::BeginComboPreview , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginComboPreview","ImGui::BeginComboPreview")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3578:29
	makeExtern< void (*)() , ImGui::EndComboPreview , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndComboPreview","ImGui::EndComboPreview")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3602:29
	makeExtern< void (*)() , ImGui::FocusItem , SimNode_ExtFuncCall , imguiTempFn>(lib,"FocusItem","ImGui::FocusItem")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3603:29
	makeExtern< void (*)(unsigned int) , ImGui::ActivateItemByID , SimNode_ExtFuncCall , imguiTempFn>(lib,"ActivateItemByID","ImGui::ActivateItemByID")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3753:29
	makeExtern< void (*)(unsigned int) , ImGui::PushFocusScope , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushFocusScope","ImGui::PushFocusScope")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3754:29
	makeExtern< void (*)() , ImGui::PopFocusScope , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopFocusScope","ImGui::PopFocusScope")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3755:29
	makeExtern< unsigned int (*)() , ImGui::GetCurrentFocusScope , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCurrentFocusScope","ImGui::GetCurrentFocusScope")
		->addToModule(*this, SideEffects::worstDefault);
}
}

