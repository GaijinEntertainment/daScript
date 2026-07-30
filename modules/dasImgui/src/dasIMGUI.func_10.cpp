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
// from imgui.h:733:29
	makeExtern< bool (*)(const char *,double *,double,double,const char *,int) , ImGui::InputDouble , SimNode_ExtFuncCall , imguiTempFn>(lib,"InputDouble","ImGui::InputDouble")
		->args({"label","v","step","step_fast","format","flags"})
		->arg_init(2,new ExprConstDouble(0))
		->arg_init(3,new ExprConstDouble(0))
		->arg_init(4,new ExprConstString("%.6f"))
		->arg_type(5,makeType<ImGuiInputTextFlags_>(lib))
		->arg_init(5,new ExprConstEnumeration(0,makeType<ImGuiInputTextFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:734:29
	makeExtern< bool (*)(const char *,int,void *,const void *,const void *,const char *,int) , ImGui::InputScalar , SimNode_ExtFuncCall , imguiTempFn>(lib,"InputScalar","ImGui::InputScalar")
		->args({"label","data_type","p_data","p_step","p_step_fast","format","flags"})
		->arg_type(1,makeType<ImGuiDataType_>(lib))
		->arg_init(3,new ExprConstPtr())
		->arg_init(4,new ExprConstPtr())
		->arg_init(5,new ExprConstString(""))
		->arg_type(6,makeType<ImGuiInputTextFlags_>(lib))
		->arg_init(6,new ExprConstEnumeration(0,makeType<ImGuiInputTextFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:735:29
	makeExtern< bool (*)(const char *,int,void *,int,const void *,const void *,const char *,int) , ImGui::InputScalarN , SimNode_ExtFuncCall , imguiTempFn>(lib,"InputScalarN","ImGui::InputScalarN")
		->args({"label","data_type","p_data","components","p_step","p_step_fast","format","flags"})
		->arg_type(1,makeType<ImGuiDataType_>(lib))
		->arg_init(4,new ExprConstPtr())
		->arg_init(5,new ExprConstPtr())
		->arg_init(6,new ExprConstString(""))
		->arg_type(7,makeType<ImGuiInputTextFlags_>(lib))
		->arg_init(7,new ExprConstEnumeration(0,makeType<ImGuiInputTextFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:740:29
	makeExtern< bool (*)(const char *,float[3],int) , ImGui::ColorEdit3 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorEdit3","ImGui::ColorEdit3")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:741:29
	makeExtern< bool (*)(const char *,float[4],int) , ImGui::ColorEdit4 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorEdit4","ImGui::ColorEdit4")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:742:29
	makeExtern< bool (*)(const char *,float[3],int) , ImGui::ColorPicker3 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorPicker3","ImGui::ColorPicker3")
		->args({"label","col","flags"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:743:29
	makeExtern< bool (*)(const char *,float[4],int,const float *) , ImGui::ColorPicker4 , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorPicker4","ImGui::ColorPicker4")
		->args({"label","col","flags","ref_col"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->arg_init(3,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:744:29
	makeExtern< bool (*)(const char *,const ImVec4 &,int,const ImVec2 &) , ImGui::ColorButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"ColorButton","ImGui::ColorButton")
		->args({"desc_id","col","flags","size"})
		->arg_type(2,makeType<ImGuiColorEditFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiColorEditFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:745:29
	makeExtern< void (*)(int) , ImGui::SetColorEditOptions , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetColorEditOptions","ImGui::SetColorEditOptions")
		->args({"flags"})
		->arg_type(0,makeType<ImGuiColorEditFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:749:29
	makeExtern< bool (*)(const char *) , ImGui::TreeNode , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreeNode","ImGui::TreeNode")
		->args({"label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:754:29
	makeExtern< bool (*)(const char *,int) , ImGui::TreeNodeEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreeNodeEx","ImGui::TreeNodeEx")
		->args({"label","flags"})
		->arg_type(1,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:759:29
	makeExtern< void (*)(const char *) , ImGui::TreePush , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePush","ImGui::TreePush")
		->args({"str_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:760:29
	makeExtern< void (*)(const void *) , ImGui::TreePush , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePush","ImGui::TreePush")
		->args({"ptr_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:761:29
	makeExtern< void (*)() , ImGui::TreePop , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePop","ImGui::TreePop")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:762:29
	makeExtern< float (*)() , ImGui::GetTreeNodeToLabelSpacing , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTreeNodeToLabelSpacing","ImGui::GetTreeNodeToLabelSpacing")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:763:29
	makeExtern< bool (*)(const char *,int) , ImGui::CollapsingHeader , SimNode_ExtFuncCall , imguiTempFn>(lib,"CollapsingHeader","ImGui::CollapsingHeader")
		->args({"label","flags"})
		->arg_type(1,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:764:29
	makeExtern< bool (*)(const char *,bool *,int) , ImGui::CollapsingHeader , SimNode_ExtFuncCall , imguiTempFn>(lib,"CollapsingHeader","ImGui::CollapsingHeader")
		->args({"label","p_visible","flags"})
		->arg_type(2,makeType<ImGuiTreeNodeFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:765:29
	makeExtern< void (*)(bool,int) , ImGui::SetNextItemOpen , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextItemOpen","ImGui::SetNextItemOpen")
		->args({"is_open","cond"})
		->arg_type(1,makeType<ImGuiCond_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiCond_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:766:29
	makeExtern< void (*)(unsigned int) , ImGui::SetNextItemStorageID , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextItemStorageID","ImGui::SetNextItemStorageID")
		->args({"storage_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:771:29
	makeExtern< bool (*)(const char *,bool,int,const ImVec2 &) , ImGui::Selectable , SimNode_ExtFuncCall , imguiTempFn>(lib,"Selectable","ImGui::Selectable")
		->args({"label","selected","flags","size"})
		->arg_init(1,new ExprConstBool(false))
		->arg_type(2,makeType<ImGuiSelectableFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiSelectableFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
}
}

