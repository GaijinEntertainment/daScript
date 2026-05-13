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
void Module_dasIMGUI::initFunctions_12() {
// from imgui.h:735:29
	makeExtern< void (*)() , ImGui::EndPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndPopup","ImGui::EndPopup")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:745:29
	makeExtern< void (*)(const char *,int) , ImGui::OpenPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"OpenPopup","ImGui::OpenPopup")
		->args({"str_id","popup_flags"})
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:746:29
	makeExtern< void (*)(unsigned int,int) , ImGui::OpenPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"OpenPopup","ImGui::OpenPopup")
		->args({"id","popup_flags"})
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:747:29
	makeExtern< void (*)(const char *,int) , ImGui::OpenPopupOnItemClick , SimNode_ExtFuncCall , imguiTempFn>(lib,"OpenPopupOnItemClick","ImGui::OpenPopupOnItemClick")
		->args({"str_id","popup_flags"})
		->arg_init(0,new ExprConstString(""))
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(1,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:748:29
	makeExtern< void (*)() , ImGui::CloseCurrentPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"CloseCurrentPopup","ImGui::CloseCurrentPopup")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:755:29
	makeExtern< bool (*)(const char *,int) , ImGui::BeginPopupContextItem , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopupContextItem","ImGui::BeginPopupContextItem")
		->args({"str_id","popup_flags"})
		->arg_init(0,new ExprConstString(""))
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(1,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:756:29
	makeExtern< bool (*)(const char *,int) , ImGui::BeginPopupContextWindow , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopupContextWindow","ImGui::BeginPopupContextWindow")
		->args({"str_id","popup_flags"})
		->arg_init(0,new ExprConstString(""))
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(1,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:757:29
	makeExtern< bool (*)(const char *,int) , ImGui::BeginPopupContextVoid , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopupContextVoid","ImGui::BeginPopupContextVoid")
		->args({"str_id","popup_flags"})
		->arg_init(0,new ExprConstString(""))
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(1,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:763:29
	makeExtern< bool (*)(const char *,int) , ImGui::IsPopupOpen , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsPopupOpen","ImGui::IsPopupOpen")
		->args({"str_id","flags"})
		->arg_type(1,makeType<ImGuiPopupFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiPopupFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:786:29
	makeExtern< bool (*)(const char *,int,int,const ImVec2 &,float) , ImGui::BeginTable , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginTable","ImGui::BeginTable")
		->args({"str_id","column","flags","outer_size","inner_width"})
		->arg_type(2,makeType<ImGuiTableFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiTableFlags_>(lib)))
		->arg_init(4,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:787:29
	makeExtern< void (*)() , ImGui::EndTable , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndTable","ImGui::EndTable")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:788:29
	makeExtern< void (*)(int,float) , ImGui::TableNextRow , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableNextRow","ImGui::TableNextRow")
		->args({"row_flags","min_row_height"})
		->arg_type(0,makeType<ImGuiTableRowFlags_>(lib))
		->arg_init(0,new ExprConstEnumeration(0,makeType<ImGuiTableRowFlags_>(lib)))
		->arg_init(1,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:789:29
	makeExtern< bool (*)() , ImGui::TableNextColumn , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableNextColumn","ImGui::TableNextColumn")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:790:29
	makeExtern< bool (*)(int) , ImGui::TableSetColumnIndex , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableSetColumnIndex","ImGui::TableSetColumnIndex")
		->args({"column_n"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:800:29
	makeExtern< void (*)(const char *,int,float,unsigned int) , ImGui::TableSetupColumn , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableSetupColumn","ImGui::TableSetupColumn")
		->args({"label","flags","init_width_or_weight","user_id"})
		->arg_type(1,makeType<ImGuiTableColumnFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTableColumnFlags_>(lib)))
		->arg_init(2,new ExprConstFloat(0))
		->arg_init(3,new ExprConstUInt(0x0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:801:29
	makeExtern< void (*)(int,int) , ImGui::TableSetupScrollFreeze , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableSetupScrollFreeze","ImGui::TableSetupScrollFreeze")
		->args({"cols","rows"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:802:29
	makeExtern< void (*)(const char *) , ImGui::TableHeader , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableHeader","ImGui::TableHeader")
		->args({"label"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:803:29
	makeExtern< void (*)() , ImGui::TableHeadersRow , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableHeadersRow","ImGui::TableHeadersRow")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:804:29
	makeExtern< void (*)() , ImGui::TableAngledHeadersRow , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableAngledHeadersRow","ImGui::TableAngledHeadersRow")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:812:37
	makeExtern< ImGuiTableSortSpecs * (*)() , ImGui::TableGetSortSpecs , SimNode_ExtFuncCall , imguiTempFn>(lib,"TableGetSortSpecs","ImGui::TableGetSortSpecs")
		->addToModule(*this, SideEffects::worstDefault);
}
}

