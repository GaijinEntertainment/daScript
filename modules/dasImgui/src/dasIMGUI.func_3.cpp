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
void Module_dasIMGUI::initFunctions_3() {
// from imgui.h:395:29
	makeExtern< void (*)(const ImVec2 &,int) , ImGui::SetNextWindowSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowSize","ImGui::SetNextWindowSize")
		->args({"size","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:397:29
	makeExtern< void (*)(const ImVec2 &) , ImGui::SetNextWindowContentSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowContentSize","ImGui::SetNextWindowContentSize")
		->args({"size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:398:29
	makeExtern< void (*)(bool,int) , ImGui::SetNextWindowCollapsed , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowCollapsed","ImGui::SetNextWindowCollapsed")
		->args({"collapsed","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:399:29
	makeExtern< void (*)() , ImGui::SetNextWindowFocus , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowFocus","ImGui::SetNextWindowFocus")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:400:29
	makeExtern< void (*)(const ImVec2 &) , ImGui::SetNextWindowScroll , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowScroll","ImGui::SetNextWindowScroll")
		->args({"scroll"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:401:29
	makeExtern< void (*)(float) , ImGui::SetNextWindowBgAlpha , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowBgAlpha","ImGui::SetNextWindowBgAlpha")
		->args({"alpha"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:402:29
	makeExtern< void (*)(unsigned int) , ImGui::SetNextWindowViewport , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowViewport","ImGui::SetNextWindowViewport")
		->args({"viewport_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:403:29
	makeExtern< void (*)(const ImVec2 &,int) , ImGui::SetWindowPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowPos","ImGui::SetWindowPos")
		->args({"pos","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:404:29
	makeExtern< void (*)(const ImVec2 &,int) , ImGui::SetWindowSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowSize","ImGui::SetWindowSize")
		->args({"size","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:405:29
	makeExtern< void (*)(bool,int) , ImGui::SetWindowCollapsed , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowCollapsed","ImGui::SetWindowCollapsed")
		->args({"collapsed","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:406:29
	makeExtern< void (*)() , ImGui::SetWindowFocus , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowFocus","ImGui::SetWindowFocus")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:407:29
	makeExtern< void (*)(float) , ImGui::SetWindowFontScale , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowFontScale","ImGui::SetWindowFontScale")
		->args({"scale"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:408:29
	makeExtern< void (*)(const char *,const ImVec2 &,int) , ImGui::SetWindowPos , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowPos","ImGui::SetWindowPos")
		->args({"name","pos","cond"})
		->arg_type(2,makeType<ImGuiCond_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:409:29
	makeExtern< void (*)(const char *,const ImVec2 &,int) , ImGui::SetWindowSize , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowSize","ImGui::SetWindowSize")
		->args({"name","size","cond"})
		->arg_type(2,makeType<ImGuiCond_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:410:29
	makeExtern< void (*)(const char *,bool,int) , ImGui::SetWindowCollapsed , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowCollapsed","ImGui::SetWindowCollapsed")
		->args({"name","collapsed","cond"})
		->arg_type(2,makeType<ImGuiCond_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:411:29
	makeExtern< void (*)(const char *) , ImGui::SetWindowFocus , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetWindowFocus","ImGui::SetWindowFocus")
		->args({"name"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:416:29
	makeExtern< ImVec2 (*)() , ImGui::GetContentRegionAvail , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetContentRegionAvail","ImGui::GetContentRegionAvail")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:417:29
	makeExtern< ImVec2 (*)() , ImGui::GetContentRegionMax , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetContentRegionMax","ImGui::GetContentRegionMax")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:418:29
	makeExtern< ImVec2 (*)() , ImGui::GetWindowContentRegionMin , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetWindowContentRegionMin","ImGui::GetWindowContentRegionMin")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:419:29
	makeExtern< ImVec2 (*)() , ImGui::GetWindowContentRegionMax , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetWindowContentRegionMax","ImGui::GetWindowContentRegionMax")
		->addToModule(*this, SideEffects::worstDefault);
}
}

