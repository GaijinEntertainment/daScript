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
void Module_dasIMGUI::initFunctions_16() {
// from imgui.h:1047:29
	makeExtern< bool (*)() , ImGui::IsItemEdited , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsItemEdited","ImGui::IsItemEdited")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1048:29
	makeExtern< bool (*)() , ImGui::IsItemActivated , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsItemActivated","ImGui::IsItemActivated")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1049:29
	makeExtern< bool (*)() , ImGui::IsItemDeactivated , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsItemDeactivated","ImGui::IsItemDeactivated")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1050:29
	makeExtern< bool (*)() , ImGui::IsItemDeactivatedAfterEdit , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsItemDeactivatedAfterEdit","ImGui::IsItemDeactivatedAfterEdit")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1051:29
	makeExtern< bool (*)() , ImGui::IsItemToggledOpen , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsItemToggledOpen","ImGui::IsItemToggledOpen")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1052:29
	makeExtern< bool (*)() , ImGui::IsAnyItemHovered , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsAnyItemHovered","ImGui::IsAnyItemHovered")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1053:29
	makeExtern< bool (*)() , ImGui::IsAnyItemActive , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsAnyItemActive","ImGui::IsAnyItemActive")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1054:29
	makeExtern< bool (*)() , ImGui::IsAnyItemFocused , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsAnyItemFocused","ImGui::IsAnyItemFocused")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1055:29
	makeExtern< unsigned int (*)() , ImGui::GetItemID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetItemID","ImGui::GetItemID")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1056:29
	makeExtern< ImVec2 (*)() , ImGui::GetItemRectMin , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetItemRectMin","ImGui::GetItemRectMin")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1057:29
	makeExtern< ImVec2 (*)() , ImGui::GetItemRectMax , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetItemRectMax","ImGui::GetItemRectMax")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1058:29
	makeExtern< ImVec2 (*)() , ImGui::GetItemRectSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetItemRectSize","ImGui::GetItemRectSize")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1059:30
	makeExtern< int (*)() , ImGui::GetItemFlags , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetItemFlags","ImGui::GetItemFlags")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1065:30
	makeExtern< ImGuiViewport * (*)() , ImGui::GetMainViewport , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMainViewport","ImGui::GetMainViewport")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1068:29
	makeExtern< ImDrawList * (*)(ImGuiViewport *) , ImGui::GetBackgroundDrawList , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetBackgroundDrawList","ImGui::GetBackgroundDrawList")
		->args({"viewport"})
		->arg_init(0,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1069:29
	makeExtern< ImDrawList * (*)(ImGuiViewport *) , ImGui::GetForegroundDrawList , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetForegroundDrawList","ImGui::GetForegroundDrawList")
		->args({"viewport"})
		->arg_init(0,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1072:29
	makeExtern< bool (*)(const ImVec2 &) , ImGui::IsRectVisible , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsRectVisible","ImGui::IsRectVisible")
		->args({"size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1073:29
	makeExtern< bool (*)(const ImVec2 &,const ImVec2 &) , ImGui::IsRectVisible , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsRectVisible","ImGui::IsRectVisible")
		->args({"rect_min","rect_max"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1074:29
	makeExtern< double (*)() , ImGui::GetTime , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTime","ImGui::GetTime")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1075:29
	makeExtern< int (*)() , ImGui::GetFrameCount , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFrameCount","ImGui::GetFrameCount")
		->addToModule(*this, SideEffects::worstDefault);
}
}

