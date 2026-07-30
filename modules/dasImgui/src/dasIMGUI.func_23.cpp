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
void Module_dasIMGUI::initFunctions_23() {
	using _method_55 = das::das_call_member< void ** (ImGuiStorage::*)(unsigned int,void *),&ImGuiStorage::GetVoidPtrRef >;
// from imgui.h:2946:25
	makeExtern<DAS_CALL_METHOD(_method_55), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetVoidPtrRef","das_call_member< void ** (ImGuiStorage::*)(unsigned int,void *) , &ImGuiStorage::GetVoidPtrRef >::invoke")
		->args({"self","key","default_val"})
		->arg_init(2,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_56 = das::das_call_member< void (ImGuiStorage::*)(),&ImGuiStorage::BuildSortByKey >;
// from imgui.h:2949:25
	makeExtern<DAS_CALL_METHOD(_method_56), SimNode_ExtFuncCall , imguiTempFn>(lib,"BuildSortByKey","das_call_member< void (ImGuiStorage::*)() , &ImGuiStorage::BuildSortByKey >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_57 = das::das_call_member< void (ImGuiStorage::*)(int),&ImGuiStorage::SetAllInt >;
// from imgui.h:2951:25
	makeExtern<DAS_CALL_METHOD(_method_57), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetAllInt","das_call_member< void (ImGuiStorage::*)(int) , &ImGuiStorage::SetAllInt >::invoke")
		->args({"self","val"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiListClipper>(*this,lib,"ImGuiListClipper","ImGuiListClipper");
	using _method_58 = das::das_call_member< void (ImGuiListClipper::*)(int,float),&ImGuiListClipper::Begin >;
// from imgui.h:3001:21
	makeExtern<DAS_CALL_METHOD(_method_58), SimNode_ExtFuncCall , imguiTempFn>(lib,"Begin","das_call_member< void (ImGuiListClipper::*)(int,float) , &ImGuiListClipper::Begin >::invoke")
		->args({"self","items_count","items_height"})
		->arg_init(2,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_59 = das::das_call_member< void (ImGuiListClipper::*)(),&ImGuiListClipper::End >;
// from imgui.h:3002:21
	makeExtern<DAS_CALL_METHOD(_method_59), SimNode_ExtFuncCall , imguiTempFn>(lib,"End","das_call_member< void (ImGuiListClipper::*)() , &ImGuiListClipper::End >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_60 = das::das_call_member< bool (ImGuiListClipper::*)(),&ImGuiListClipper::Step >;
// from imgui.h:3003:21
	makeExtern<DAS_CALL_METHOD(_method_60), SimNode_ExtFuncCall , imguiTempFn>(lib,"Step","das_call_member< bool (ImGuiListClipper::*)() , &ImGuiListClipper::Step >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_61 = das::das_call_member< void (ImGuiListClipper::*)(int),&ImGuiListClipper::IncludeItemByIndex >;
// from imgui.h:3007:21
	makeExtern<DAS_CALL_METHOD(_method_61), SimNode_ExtFuncCall , imguiTempFn>(lib,"IncludeItemByIndex","das_call_member< void (ImGuiListClipper::*)(int) , &ImGuiListClipper::IncludeItemByIndex >::invoke")
		->args({"self","item_index"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_62 = das::das_call_member< void (ImGuiListClipper::*)(int,int),&ImGuiListClipper::IncludeItemsByIndex >;
// from imgui.h:3008:21
	makeExtern<DAS_CALL_METHOD(_method_62), SimNode_ExtFuncCall , imguiTempFn>(lib,"IncludeItemsByIndex","das_call_member< void (ImGuiListClipper::*)(int,int) , &ImGuiListClipper::IncludeItemsByIndex >::invoke")
		->args({"self","item_begin","item_end"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_63 = das::das_call_member< void (ImGuiListClipper::*)(int),&ImGuiListClipper::SeekCursorForItem >;
// from imgui.h:3013:21
	makeExtern<DAS_CALL_METHOD(_method_63), SimNode_ExtFuncCall , imguiTempFn>(lib,"SeekCursorForItem","das_call_member< void (ImGuiListClipper::*)(int) , &ImGuiListClipper::SeekCursorForItem >::invoke")
		->args({"self","item_index"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiSelectionBasicStorage>(*this,lib,"ImGuiSelectionBasicStorage","ImGuiSelectionBasicStorage");
	using _method_64 = das::das_call_member< void (ImGuiSelectionBasicStorage::*)(ImGuiMultiSelectIO *),&ImGuiSelectionBasicStorage::ApplyRequests >;
// from imgui.h:3230:21
	makeExtern<DAS_CALL_METHOD(_method_64), SimNode_ExtFuncCall , imguiTempFn>(lib,"ApplyRequests","das_call_member< void (ImGuiSelectionBasicStorage::*)(ImGuiMultiSelectIO *) , &ImGuiSelectionBasicStorage::ApplyRequests >::invoke")
		->args({"self","ms_io"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_65 = das::das_call_member< bool (ImGuiSelectionBasicStorage::*)(unsigned int) const,&ImGuiSelectionBasicStorage::Contains >;
// from imgui.h:3231:21
	makeExtern<DAS_CALL_METHOD(_method_65), SimNode_ExtFuncCall , imguiTempFn>(lib,"Contains","das_call_member< bool (ImGuiSelectionBasicStorage::*)(unsigned int) const , &ImGuiSelectionBasicStorage::Contains >::invoke")
		->args({"self","id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_66 = das::das_call_member< void (ImGuiSelectionBasicStorage::*)(),&ImGuiSelectionBasicStorage::Clear >;
// from imgui.h:3232:21
	makeExtern<DAS_CALL_METHOD(_method_66), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImGuiSelectionBasicStorage::*)() , &ImGuiSelectionBasicStorage::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_67 = das::das_call_member< void (ImGuiSelectionBasicStorage::*)(ImGuiSelectionBasicStorage &),&ImGuiSelectionBasicStorage::Swap >;
// from imgui.h:3233:21
	makeExtern<DAS_CALL_METHOD(_method_67), SimNode_ExtFuncCall , imguiTempFn>(lib,"Swap","das_call_member< void (ImGuiSelectionBasicStorage::*)(ImGuiSelectionBasicStorage &) , &ImGuiSelectionBasicStorage::Swap >::invoke")
		->args({"self","r"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_68 = das::das_call_member< void (ImGuiSelectionBasicStorage::*)(unsigned int,bool),&ImGuiSelectionBasicStorage::SetItemSelected >;
// from imgui.h:3234:21
	makeExtern<DAS_CALL_METHOD(_method_68), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetItemSelected","das_call_member< void (ImGuiSelectionBasicStorage::*)(unsigned int,bool) , &ImGuiSelectionBasicStorage::SetItemSelected >::invoke")
		->args({"self","id","selected"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_69 = das::das_call_member< bool (ImGuiSelectionBasicStorage::*)(void **,unsigned int *),&ImGuiSelectionBasicStorage::GetNextSelectedItem >;
// from imgui.h:3235:21
	makeExtern<DAS_CALL_METHOD(_method_69), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetNextSelectedItem","das_call_member< bool (ImGuiSelectionBasicStorage::*)(void **,unsigned int *) , &ImGuiSelectionBasicStorage::GetNextSelectedItem >::invoke")
		->args({"self","opaque_it","out_id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_70 = das::das_call_member< unsigned int (ImGuiSelectionBasicStorage::*)(int),&ImGuiSelectionBasicStorage::GetStorageIdFromIndex >;
// from imgui.h:3236:21
	makeExtern<DAS_CALL_METHOD(_method_70), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetStorageIdFromIndex","das_call_member< unsigned int (ImGuiSelectionBasicStorage::*)(int) , &ImGuiSelectionBasicStorage::GetStorageIdFromIndex >::invoke")
		->args({"self","idx"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiSelectionExternalStorage>(*this,lib,"ImGuiSelectionExternalStorage","ImGuiSelectionExternalStorage");
	using _method_71 = das::das_call_member< void (ImGuiSelectionExternalStorage::*)(ImGuiMultiSelectIO *),&ImGuiSelectionExternalStorage::ApplyRequests >;
// from imgui.h:3249:21
	makeExtern<DAS_CALL_METHOD(_method_71), SimNode_ExtFuncCall , imguiTempFn>(lib,"ApplyRequests","das_call_member< void (ImGuiSelectionExternalStorage::*)(ImGuiMultiSelectIO *) , &ImGuiSelectionExternalStorage::ApplyRequests >::invoke")
		->args({"self","ms_io"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

