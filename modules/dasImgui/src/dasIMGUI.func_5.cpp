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
void Module_dasIMGUI::initFunctions_5() {
// from imgui.h:446:29
	makeExtern< void (*)(bool) , ImGui::PushButtonRepeat , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushButtonRepeat","ImGui::PushButtonRepeat")
		->args({"repeat"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:447:29
	makeExtern< void (*)() , ImGui::PopButtonRepeat , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopButtonRepeat","ImGui::PopButtonRepeat")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:450:29
	makeExtern< void (*)(float) , ImGui::PushItemWidth , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushItemWidth","ImGui::PushItemWidth")
		->args({"item_width"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:451:29
	makeExtern< void (*)() , ImGui::PopItemWidth , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopItemWidth","ImGui::PopItemWidth")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:452:29
	makeExtern< void (*)(float) , ImGui::SetNextItemWidth , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextItemWidth","ImGui::SetNextItemWidth")
		->args({"item_width"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:453:29
	makeExtern< float (*)() , ImGui::CalcItemWidth , SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcItemWidth","ImGui::CalcItemWidth")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:454:29
	makeExtern< void (*)(float) , ImGui::PushTextWrapPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushTextWrapPos","ImGui::PushTextWrapPos")
		->args({"wrap_local_pos_x"})
		->arg_init(0,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:455:29
	makeExtern< void (*)() , ImGui::PopTextWrapPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopTextWrapPos","ImGui::PopTextWrapPos")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:459:29
	makeExtern< ImFont * (*)() , ImGui::GetFont , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFont","ImGui::GetFont")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:460:29
	makeExtern< float (*)() , ImGui::GetFontSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFontSize","ImGui::GetFontSize")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:461:29
	makeExtern< ImVec2 (*)() , ImGui::GetFontTexUvWhitePixel , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFontTexUvWhitePixel","ImGui::GetFontTexUvWhitePixel")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:462:29
	makeExtern< unsigned int (*)(int,float) , ImGui::GetColorU32 , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetColorU32","ImGui::GetColorU32")
		->args({"idx","alpha_mul"})
		->arg_type(0,makeType<ImGuiCol_>(lib))
		->arg_init(1,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:463:29
	makeExtern< unsigned int (*)(const ImVec4 &) , ImGui::GetColorU32 , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetColorU32","ImGui::GetColorU32")
		->args({"col"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:464:29
	makeExtern< unsigned int (*)(unsigned int,float) , ImGui::GetColorU32 , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetColorU32","ImGui::GetColorU32")
		->args({"col","alpha_mul"})
		->arg_init(1,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:465:29
	makeExtern< const ImVec4 & (*)(int) , ImGui::GetStyleColorVec4 , SimNode_ExtFuncCallRef , imguiTempFn>(lib,"GetStyleColorVec4","ImGui::GetStyleColorVec4")
		->args({"idx"})
		->arg_type(0,makeType<ImGuiCol_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:475:29
	makeExtern< ImVec2 (*)() , ImGui::GetCursorScreenPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCursorScreenPos","ImGui::GetCursorScreenPos")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:476:29
	makeExtern< void (*)(const ImVec2 &) , ImGui::SetCursorScreenPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCursorScreenPos","ImGui::SetCursorScreenPos")
		->args({"pos"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:477:29
	makeExtern< ImVec2 (*)() , ImGui::GetCursorPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCursorPos","ImGui::GetCursorPos")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:478:29
	makeExtern< float (*)() , ImGui::GetCursorPosX , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCursorPosX","ImGui::GetCursorPosX")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:479:29
	makeExtern< float (*)() , ImGui::GetCursorPosY , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCursorPosY","ImGui::GetCursorPosY")
		->addToModule(*this, SideEffects::worstDefault);
}
}

