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
void Module_dasIMGUI::initFunctions_14() {
// from imgui.h:839:29
	makeExtern< void (*)(const char *) , ImGui::SetTabItemClosed , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetTabItemClosed","ImGui::SetTabItemClosed")
		->args({"tab_or_docked_window_label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:854:29
	makeExtern< unsigned int (*)(unsigned int,const ImVec2 &,int,const ImGuiWindowClass *) , ImGui::DockSpace , SimNode_ExtFuncCall , imguiTempFn>(lib,"DockSpace","ImGui::DockSpace")
		->args({"id","size","flags","window_class"})
		->arg_init(2,new ExprConstInt(0))
		->arg_init(3,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:855:29
	makeExtern< unsigned int (*)(const ImGuiViewport *,int,const ImGuiWindowClass *) , ImGui::DockSpaceOverViewport , SimNode_ExtFuncCall , imguiTempFn>(lib,"DockSpaceOverViewport","ImGui::DockSpaceOverViewport")
		->args({"viewport","flags","window_class"})
		->arg_init(0,new ExprConstPtr())
		->arg_init(1,new ExprConstInt(0))
		->arg_init(2,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:856:29
	makeExtern< void (*)(unsigned int,int) , ImGui::SetNextWindowDockID , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowDockID","ImGui::SetNextWindowDockID")
		->args({"dock_id","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:857:29
	makeExtern< void (*)(const ImGuiWindowClass *) , ImGui::SetNextWindowClass , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextWindowClass","ImGui::SetNextWindowClass")
		->args({"window_class"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:858:29
	makeExtern< unsigned int (*)() , ImGui::GetWindowDockID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetWindowDockID","ImGui::GetWindowDockID")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:859:29
	makeExtern< bool (*)() , ImGui::IsWindowDocked , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsWindowDocked","ImGui::IsWindowDocked")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:863:29
	makeExtern< void (*)(int) , ImGui::LogToTTY , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogToTTY","ImGui::LogToTTY")
		->args({"auto_open_depth"})
		->arg_init(0,new ExprConstInt(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:864:29
	makeExtern< void (*)(int,const char *) , ImGui::LogToFile , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogToFile","ImGui::LogToFile")
		->args({"auto_open_depth","filename"})
		->arg_init(0,new ExprConstInt(-1))
		->arg_init(1,new ExprConstString(""))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:865:29
	makeExtern< void (*)(int) , ImGui::LogToClipboard , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogToClipboard","ImGui::LogToClipboard")
		->args({"auto_open_depth"})
		->arg_init(0,new ExprConstInt(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:866:29
	makeExtern< void (*)() , ImGui::LogFinish , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogFinish","ImGui::LogFinish")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:867:29
	makeExtern< void (*)() , ImGui::LogButtons , SimNode_ExtFuncCall , imguiTempFn>(lib,"LogButtons","ImGui::LogButtons")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:876:29
	makeExtern< bool (*)(int) , ImGui::BeginDragDropSource , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginDragDropSource","ImGui::BeginDragDropSource")
		->args({"flags"})
		->arg_type(0,makeType<ImGuiDragDropFlags_>(lib))
		->arg_init(0,new ExprConstEnumeration(0,makeType<ImGuiDragDropFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:877:29
	makeExtern< bool (*)(const char *,const void *,size_t,int) , ImGui::SetDragDropPayload , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetDragDropPayload","ImGui::SetDragDropPayload")
		->args({"type","data","sz","cond"})
		->arg_type(3,makeType<ImGuiCond_>(lib))
		->arg_init(3,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:878:29
	makeExtern< void (*)() , ImGui::EndDragDropSource , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndDragDropSource","ImGui::EndDragDropSource")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:879:37
	makeExtern< bool (*)() , ImGui::BeginDragDropTarget , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginDragDropTarget","ImGui::BeginDragDropTarget")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:880:37
	makeExtern< const ImGuiPayload * (*)(const char *,int) , ImGui::AcceptDragDropPayload , SimNode_ExtFuncCall , imguiTempFn>(lib,"AcceptDragDropPayload","ImGui::AcceptDragDropPayload")
		->args({"type","flags"})
		->arg_type(1,makeType<ImGuiDragDropFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiDragDropFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:881:37
	makeExtern< void (*)() , ImGui::EndDragDropTarget , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndDragDropTarget","ImGui::EndDragDropTarget")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:882:37
	makeExtern< const ImGuiPayload * (*)() , ImGui::GetDragDropPayload , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetDragDropPayload","ImGui::GetDragDropPayload")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:888:29
	makeExtern< void (*)(bool) , ImGui::BeginDisabled , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginDisabled","ImGui::BeginDisabled")
		->args({"disabled"})
		->arg_init(0,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
}
}

