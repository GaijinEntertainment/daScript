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
void Module_dasIMGUI::initFunctions_10() {
// from imgui.h:630:29
	makeExtern< bool (*)(const char *,int,void *,int,const void *,const void *,const char *,int) , ImGui::InputScalarN , SimNode_ExtFuncCall , imguiTempFn>(lib,"InputScalarN","ImGui::InputScalarN")
		->args({"label","data_type","p_data","components","p_step","p_step_fast","format","flags"})
		->arg_type(1,makeType<ImGuiDataType_>(lib))
		->arg_init(4,new ExprConstPtr())
		->arg_init(5,new ExprConstPtr())
		->arg_init(6,new ExprConstString(""))
		->arg_type(7,makeType<ImGuiInputTextFlags_>(lib))
		->arg_init(7,new ExprConstEnumeration(0,makeType<ImGuiInputTextFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:635:29
	makeExtern< bool (*)(const char *,float[3],int) , ImGui::ColorEdit3 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorEdit3","ImGui::ColorEdit3")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:636:29
	makeExtern< bool (*)(const char *,float[4],int) , ImGui::ColorEdit4 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorEdit4","ImGui::ColorEdit4")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:637:29
	makeExtern< bool (*)(const char *,float[3],int) , ImGui::ColorPicker3 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorPicker3","ImGui::ColorPicker3")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:638:29
	makeExtern< bool (*)(const char *,float[4],int,const float *) , ImGui::ColorPicker4 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorPicker4","ImGui::ColorPicker4")
		->args({"label","col","flags","ref_col"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->arg_init(3,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:639:29
	makeExtern< bool (*)(const char *,const ImVec4 &,int,const ImVec2 &) , ImGui::ColorButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorButton","ImGui::ColorButton")
		->args({"desc_id","col","flags","size"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:640:29
	makeExtern< void (*)(int) , ImGui::SetColorEditOptions , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetColorEditOptions","ImGui::SetColorEditOptions")
		->args({"flags"})
		->arg_type(0,makeType<ImGuiColorEditFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:644:29
	makeExtern< bool (*)(const char *) , ImGui::TreeNode , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreeNode","ImGui::TreeNode")
		->args({"label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:649:29
	makeExtern< bool (*)(const char *,int) , ImGui::TreeNodeEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreeNodeEx","ImGui::TreeNodeEx")
		->args({"label","flags"})
		->arg_type(1,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:654:29
	makeExtern< void (*)(const char *) , ImGui::TreePush , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePush","ImGui::TreePush")
		->args({"str_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:655:29
	makeExtern< void (*)(const void *) , ImGui::TreePush , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePush","ImGui::TreePush")
		->args({"ptr_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:656:29
	makeExtern< void (*)() , ImGui::TreePop , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePop","ImGui::TreePop")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:657:29
	makeExtern< float (*)() , ImGui::GetTreeNodeToLabelSpacing , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTreeNodeToLabelSpacing","ImGui::GetTreeNodeToLabelSpacing")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:658:29
	makeExtern< bool (*)(const char *,int) , ImGui::CollapsingHeader , SimNode_ExtFuncCall , imguiTempFn>(lib,"CollapsingHeader","ImGui::CollapsingHeader")
		->args({"label","flags"})
		->arg_type(1,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:659:29
	makeExtern< bool (*)(const char *,bool *,int) , ImGui::CollapsingHeader , SimNode_ExtFuncCall , imguiTempFn>(lib,"CollapsingHeader","ImGui::CollapsingHeader")
		->args({"label","p_visible","flags"})
		->arg_type(2,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:660:29
	makeExtern< void (*)(bool,int) , ImGui::SetNextItemOpen , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextItemOpen","ImGui::SetNextItemOpen")
		->args({"is_open","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:665:29
	makeExtern< bool (*)(const char *,bool,int,const ImVec2 &) , ImGui::Selectable , SimNode_ExtFuncCall , imguiTempFn>(lib,"Selectable","ImGui::Selectable")
		->args({"label","selected","flags","size"})
		->arg_init(1,new ExprConstBool(false))
		->arg_type(2,makeType<ImGuiSelectableFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiSelectableFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:666:29
	makeExtern< bool (*)(const char *,bool *,int,const ImVec2 &) , ImGui::Selectable , SimNode_ExtFuncCall , imguiTempFn>(lib,"Selectable","ImGui::Selectable")
		->args({"label","p_selected","flags","size"})
		->arg_type(2,makeType<ImGuiSelectableFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiSelectableFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:674:29
	makeExtern< bool (*)(const char *,const ImVec2 &) , ImGui::BeginListBox , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginListBox","ImGui::BeginListBox")
		->args({"label","size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:675:29
	makeExtern< void (*)() , ImGui::EndListBox , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndListBox","ImGui::EndListBox")
		->addToModule(*this, SideEffects::worstDefault);
}
}

