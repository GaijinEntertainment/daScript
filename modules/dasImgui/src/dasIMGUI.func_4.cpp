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
void Module_dasIMGUI::initFunctions_4() {
// from imgui.h:424:29
	makeExtern< float (*)() , ImGui::GetScrollX , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetScrollX","ImGui::GetScrollX")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:425:29
	makeExtern< float (*)() , ImGui::GetScrollY , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetScrollY","ImGui::GetScrollY")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:426:29
	makeExtern< void (*)(float) , ImGui::SetScrollX , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollX","ImGui::SetScrollX")
		->args({"scroll_x"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:427:29
	makeExtern< void (*)(float) , ImGui::SetScrollY , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollY","ImGui::SetScrollY")
		->args({"scroll_y"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:428:29
	makeExtern< float (*)() , ImGui::GetScrollMaxX , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetScrollMaxX","ImGui::GetScrollMaxX")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:429:29
	makeExtern< float (*)() , ImGui::GetScrollMaxY , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetScrollMaxY","ImGui::GetScrollMaxY")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:430:29
	makeExtern< void (*)(float) , ImGui::SetScrollHereX , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollHereX","ImGui::SetScrollHereX")
		->args({"center_x_ratio"})
		->arg_init(0,new ExprConstFloat(0.5))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:431:29
	makeExtern< void (*)(float) , ImGui::SetScrollHereY , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollHereY","ImGui::SetScrollHereY")
		->args({"center_y_ratio"})
		->arg_init(0,new ExprConstFloat(0.5))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:432:29
	makeExtern< void (*)(float,float) , ImGui::SetScrollFromPosX , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollFromPosX","ImGui::SetScrollFromPosX")
		->args({"local_x","center_x_ratio"})
		->arg_init(1,new ExprConstFloat(0.5))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:433:29
	makeExtern< void (*)(float,float) , ImGui::SetScrollFromPosY , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetScrollFromPosY","ImGui::SetScrollFromPosY")
		->args({"local_y","center_y_ratio"})
		->arg_init(1,new ExprConstFloat(0.5))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:436:29
	makeExtern< void (*)(ImFont *) , ImGui::PushFont , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushFont","ImGui::PushFont")
		->args({"font"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:437:29
	makeExtern< void (*)() , ImGui::PopFont , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopFont","ImGui::PopFont")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:438:29
	makeExtern< void (*)(int,unsigned int) , ImGui::PushStyleColor , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushStyleColor","ImGui::PushStyleColor")
		->args({"idx","col"})
		->arg_type(0,makeType<ImGuiCol_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:439:29
	makeExtern< void (*)(int,const ImVec4 &) , ImGui::PushStyleColor , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushStyleColor","ImGui::PushStyleColor")
		->args({"idx","col"})
		->arg_type(0,makeType<ImGuiCol_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:440:29
	makeExtern< void (*)(int) , ImGui::PopStyleColor , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopStyleColor","ImGui::PopStyleColor")
		->args({"count"})
		->arg_init(0,new ExprConstInt(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:441:29
	makeExtern< void (*)(int,float) , ImGui::PushStyleVar , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushStyleVar","ImGui::PushStyleVar")
		->args({"idx","val"})
		->arg_type(0,makeType<ImGuiStyleVar_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:442:29
	makeExtern< void (*)(int,const ImVec2 &) , ImGui::PushStyleVar , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushStyleVar","ImGui::PushStyleVar")
		->args({"idx","val"})
		->arg_type(0,makeType<ImGuiStyleVar_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:443:29
	makeExtern< void (*)(int) , ImGui::PopStyleVar , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopStyleVar","ImGui::PopStyleVar")
		->args({"count"})
		->arg_init(0,new ExprConstInt(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:444:29
	makeExtern< void (*)(bool) , ImGui::PushTabStop , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushTabStop","ImGui::PushTabStop")
		->args({"tab_stop"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:445:29
	makeExtern< void (*)() , ImGui::PopTabStop , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopTabStop","ImGui::PopTabStop")
		->addToModule(*this, SideEffects::worstDefault);
}
}

