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
void Module_dasIMGUI::initFunctions_6() {
// from imgui.h:480:29
	makeExtern< void (*)(const ImVec2 &) , ImGui::SetCursorPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCursorPos","ImGui::SetCursorPos")
		->args({"local_pos"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:481:29
	makeExtern< void (*)(float) , ImGui::SetCursorPosX , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCursorPosX","ImGui::SetCursorPosX")
		->args({"local_x"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:482:29
	makeExtern< void (*)(float) , ImGui::SetCursorPosY , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCursorPosY","ImGui::SetCursorPosY")
		->args({"local_y"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:483:29
	makeExtern< ImVec2 (*)() , ImGui::GetCursorStartPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCursorStartPos","ImGui::GetCursorStartPos")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:486:29
	makeExtern< void (*)() , ImGui::Separator , SimNode_ExtFuncCall , imguiTempFn>(lib,"Separator","ImGui::Separator")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:487:29
	makeExtern< void (*)(float,float) , ImGui::SameLine , SimNode_ExtFuncCall , imguiTempFn>(lib,"SameLine","ImGui::SameLine")
		->args({"offset_from_start_x","spacing"})
		->arg_init(0,new ExprConstFloat(0))
		->arg_init(1,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:488:29
	makeExtern< void (*)() , ImGui::NewLine , SimNode_ExtFuncCall , imguiTempFn>(lib,"NewLine","ImGui::NewLine")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:489:29
	makeExtern< void (*)() , ImGui::Spacing , SimNode_ExtFuncCall , imguiTempFn>(lib,"Spacing","ImGui::Spacing")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:490:29
	makeExtern< void (*)(const ImVec2 &) , ImGui::Dummy , SimNode_ExtFuncCall , imguiTempFn>(lib,"Dummy","ImGui::Dummy")
		->args({"size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:491:29
	makeExtern< void (*)(float) , ImGui::Indent , SimNode_ExtFuncCall , imguiTempFn>(lib,"Indent","ImGui::Indent")
		->args({"indent_w"})
		->arg_init(0,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:492:29
	makeExtern< void (*)(float) , ImGui::Unindent , SimNode_ExtFuncCall , imguiTempFn>(lib,"Unindent","ImGui::Unindent")
		->args({"indent_w"})
		->arg_init(0,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:493:29
	makeExtern< void (*)() , ImGui::BeginGroup , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginGroup","ImGui::BeginGroup")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:494:29
	makeExtern< void (*)() , ImGui::EndGroup , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndGroup","ImGui::EndGroup")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:495:29
	makeExtern< void (*)() , ImGui::AlignTextToFramePadding , SimNode_ExtFuncCall , imguiTempFn>(lib,"AlignTextToFramePadding","ImGui::AlignTextToFramePadding")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:496:29
	makeExtern< float (*)() , ImGui::GetTextLineHeight , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTextLineHeight","ImGui::GetTextLineHeight")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:497:29
	makeExtern< float (*)() , ImGui::GetTextLineHeightWithSpacing , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTextLineHeightWithSpacing","ImGui::GetTextLineHeightWithSpacing")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:498:29
	makeExtern< float (*)() , ImGui::GetFrameHeight , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFrameHeight","ImGui::GetFrameHeight")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:499:29
	makeExtern< float (*)() , ImGui::GetFrameHeightWithSpacing , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFrameHeightWithSpacing","ImGui::GetFrameHeightWithSpacing")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:512:29
	makeExtern< void (*)(const char *) , ImGui::PushID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushID","ImGui::PushID")
		->args({"str_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:513:29
	makeExtern< void (*)(const char *,const char *) , ImGui::PushID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushID","ImGui::PushID")
		->args({"str_id_begin","str_id_end"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

