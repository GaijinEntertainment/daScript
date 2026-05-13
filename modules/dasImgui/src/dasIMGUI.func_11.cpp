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
void Module_dasIMGUI::initFunctions_11() {
// from imgui.h:676:29
	makeExtern< bool (*)(const char *,int *,const char *const[],int,int) , ImGui::ListBox , SimNode_ExtFuncCall , imguiTempFn>(lib,"ListBox","ImGui::ListBox")
		->args({"label","current_item","items","items_count","height_in_items"})
		->arg_init(4,new ExprConstInt(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:681:29
	makeExtern< void (*)(const char *,const float *,int,int,const char *,float,float,ImVec2,int) , ImGui::PlotLines , SimNode_ExtFuncCall , imguiTempFn>(lib,"PlotLines","ImGui::PlotLines")
		->args({"label","values","values_count","values_offset","overlay_text","scale_min","scale_max","graph_size","stride"})
		->arg_init(3,new ExprConstInt(0))
		->arg_init(4,new ExprConstString(""))
		->arg_init(5,new ExprConstFloat(3.4028234663852886e+38))
		->arg_init(6,new ExprConstFloat(3.4028234663852886e+38))
		->arg_init(8,new ExprConstInt(4))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:683:29
	makeExtern< void (*)(const char *,const float *,int,int,const char *,float,float,ImVec2,int) , ImGui::PlotHistogram , SimNode_ExtFuncCall , imguiTempFn>(lib,"PlotHistogram","ImGui::PlotHistogram")
		->args({"label","values","values_count","values_offset","overlay_text","scale_min","scale_max","graph_size","stride"})
		->arg_init(3,new ExprConstInt(0))
		->arg_init(4,new ExprConstString(""))
		->arg_init(5,new ExprConstFloat(3.4028234663852886e+38))
		->arg_init(6,new ExprConstFloat(3.4028234663852886e+38))
		->arg_init(8,new ExprConstInt(4))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:688:29
	makeExtern< void (*)(const char *,bool) , ImGui::Value , SimNode_ExtFuncCall , imguiTempFn>(lib,"Value","ImGui::Value")
		->args({"prefix","b"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:689:29
	makeExtern< void (*)(const char *,int) , ImGui::Value , SimNode_ExtFuncCall , imguiTempFn>(lib,"Value","ImGui::Value")
		->args({"prefix","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:690:29
	makeExtern< void (*)(const char *,unsigned int) , ImGui::Value , SimNode_ExtFuncCall , imguiTempFn>(lib,"Value","ImGui::Value")
		->args({"prefix","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:691:29
	makeExtern< void (*)(const char *,float,const char *) , ImGui::Value , SimNode_ExtFuncCall , imguiTempFn>(lib,"Value","ImGui::Value")
		->args({"prefix","v","float_format"})
		->arg_init(2,new ExprConstString(""))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:698:29
	makeExtern< bool (*)() , ImGui::BeginMenuBar , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginMenuBar","ImGui::BeginMenuBar")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:699:29
	makeExtern< void (*)() , ImGui::EndMenuBar , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndMenuBar","ImGui::EndMenuBar")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:700:29
	makeExtern< bool (*)() , ImGui::BeginMainMenuBar , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginMainMenuBar","ImGui::BeginMainMenuBar")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:701:29
	makeExtern< void (*)() , ImGui::EndMainMenuBar , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndMainMenuBar","ImGui::EndMainMenuBar")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:702:29
	makeExtern< bool (*)(const char *,bool) , ImGui::BeginMenu , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginMenu","ImGui::BeginMenu")
		->args({"label","enabled"})
		->arg_init(1,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:703:29
	makeExtern< void (*)() , ImGui::EndMenu , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndMenu","ImGui::EndMenu")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:704:29
	makeExtern< bool (*)(const char *,const char *,bool,bool) , ImGui::MenuItem , SimNode_ExtFuncCall , imguiTempFn>(lib,"MenuItem","ImGui::MenuItem")
		->args({"label","shortcut","selected","enabled"})
		->arg_init(1,new ExprConstString(""))
		->arg_init(2,new ExprConstBool(false))
		->arg_init(3,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:705:29
	makeExtern< bool (*)(const char *,const char *,bool *,bool) , ImGui::MenuItem , SimNode_ExtFuncCall , imguiTempFn>(lib,"MenuItem","ImGui::MenuItem")
		->args({"label","shortcut","p_selected","enabled"})
		->arg_init(3,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:710:29
	makeExtern< bool (*)() , ImGui::BeginTooltip , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginTooltip","ImGui::BeginTooltip")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:711:29
	makeExtern< void (*)() , ImGui::EndTooltip , SimNode_ExtFuncCall , imguiTempFn>(lib,"EndTooltip","ImGui::EndTooltip")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:719:29
	makeExtern< bool (*)() , ImGui::BeginItemTooltip , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginItemTooltip","ImGui::BeginItemTooltip")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:733:29
	makeExtern< bool (*)(const char *,int) , ImGui::BeginPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopup","ImGui::BeginPopup")
		->args({"str_id","flags"})
		->arg_type(1,makeType<ImGuiWindowFlags_>(lib))
		->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiWindowFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:734:29
	makeExtern< bool (*)(const char *,bool *,int) , ImGui::BeginPopupModal , SimNode_ExtFuncCall , imguiTempFn>(lib,"BeginPopupModal","ImGui::BeginPopupModal")
		->args({"name","p_open","flags"})
		->arg_init(1,new ExprConstPtr())
		->arg_type(2,makeType<ImGuiWindowFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiWindowFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
}
}

