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
void Module_dasIMGUI::initFunctions_7() {
// from imgui.h:514:29
	makeExtern< void (*)(const void *) , ImGui::PushID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushID","ImGui::PushID")
		->args({"ptr_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:515:29
	makeExtern< void (*)(int) , ImGui::PushID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PushID","ImGui::PushID")
		->args({"int_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:516:29
	makeExtern< void (*)() , ImGui::PopID , SimNode_ExtFuncCall , imguiTempFn>(lib,"PopID","ImGui::PopID")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:517:29
	makeExtern< unsigned int (*)(const char *) , ImGui::GetID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetID","ImGui::GetID")
		->args({"str_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:518:29
	makeExtern< unsigned int (*)(const char *,const char *) , ImGui::GetID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetID","ImGui::GetID")
		->args({"str_id_begin","str_id_end"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:519:29
	makeExtern< unsigned int (*)(const void *) , ImGui::GetID , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetID","ImGui::GetID")
		->args({"ptr_id"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:535:29
	makeExtern< void (*)(const char *) , ImGui::SeparatorText , SimNode_ExtFuncCall , imguiTempFn>(lib,"SeparatorText","ImGui::SeparatorText")
		->args({"label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:540:29
	makeExtern< bool (*)(const char *,const ImVec2 &) , ImGui::Button , SimNode_ExtFuncCall , imguiTempFn>(lib,"Button","ImGui::Button")
		->args({"label","size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:541:29
	makeExtern< bool (*)(const char *) , ImGui::SmallButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"SmallButton","ImGui::SmallButton")
		->args({"label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:542:29
	makeExtern< bool (*)(const char *,const ImVec2 &,int) , ImGui::InvisibleButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"InvisibleButton","ImGui::InvisibleButton")
		->args({"str_id","size","flags"})
		->arg_type(2,makeType<ImGuiButtonFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiButtonFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:543:29
	makeExtern< bool (*)(const char *,int) , ImGui::ArrowButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"ArrowButton","ImGui::ArrowButton")
		->args({"str_id","dir"})
		->arg_type(1,makeType<ImGuiDir_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:544:29
	makeExtern< bool (*)(const char *,bool *) , ImGui::Checkbox , SimNode_ExtFuncCall , imguiTempFn>(lib,"Checkbox","ImGui::Checkbox")
		->args({"label","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:545:29
	makeExtern< bool (*)(const char *,int *,int) , ImGui::CheckboxFlags , SimNode_ExtFuncCall , imguiTempFn>(lib,"CheckboxFlags","ImGui::CheckboxFlags")
		->args({"label","flags","flags_value"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:546:29
	makeExtern< bool (*)(const char *,unsigned int *,unsigned int) , ImGui::CheckboxFlags , SimNode_ExtFuncCall , imguiTempFn>(lib,"CheckboxFlags","ImGui::CheckboxFlags")
		->args({"label","flags","flags_value"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:547:29
	makeExtern< bool (*)(const char *,bool) , ImGui::RadioButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"RadioButton","ImGui::RadioButton")
		->args({"label","active"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:548:29
	makeExtern< bool (*)(const char *,int *,int) , ImGui::RadioButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"RadioButton","ImGui::RadioButton")
		->args({"label","v","v_button"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:549:29
	makeExtern< void (*)(float,const ImVec2 &,const char *) , ImGui::ProgressBar , SimNode_ExtFuncCall , imguiTempFn>(lib,"ProgressBar","ImGui::ProgressBar")
		->args({"fraction","size_arg","overlay"})
		->arg_init(2,new ExprConstString(""))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:550:29
	makeExtern< void (*)() , ImGui::Bullet , SimNode_ExtFuncCall , imguiTempFn>(lib,"Bullet","ImGui::Bullet")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:556:29
	makeExtern< void (*)(void *,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec4 &,const ImVec4 &) , ImGui::Image , SimNode_ExtFuncCall , imguiTempFn>(lib,"Image","ImGui::Image")
		->args({"user_texture_id","image_size","uv0","uv1","tint_col","border_col"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:557:29
	makeExtern< bool (*)(const char *,void *,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec4 &,const ImVec4 &) , ImGui::ImageButton , SimNode_ExtFuncCall , imguiTempFn>(lib,"ImageButton","ImGui::ImageButton")
		->args({"str_id","user_texture_id","image_size","uv0","uv1","bg_col","tint_col"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

