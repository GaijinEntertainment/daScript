#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasClangBind.h"
#include "needClangBind.h"
namespace das {
void Module_ClangBind::initFunctions_17( ModuleLibrary & lib ) {
	addExtern<void (*)(CXIdxLoc,void **,void **,unsigned int *,unsigned int *,unsigned int *),clang_indexLoc_getFileLocation>(*this,lib,"clang_indexLoc_getFileLocation",SideEffects::worstDefault,"clang_indexLoc_getFileLocation")
		->args({"loc","indexFile","file","line","column","offset"});
	addExtern<CXSourceLocation (*)(CXIdxLoc),clang_indexLoc_getCXSourceLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_indexLoc_getCXSourceLocation",SideEffects::worstDefault,"clang_indexLoc_getCXSourceLocation")
		->args({"loc"});
	addExtern<unsigned int (*)(CXType,CXVisitorResult (*)(CXCursor, void *),void *),clang_Type_visitFields>(*this,lib,"clang_Type_visitFields",SideEffects::worstDefault,"clang_Type_visitFields")
		->args({"T","visitor","client_data"});
}
}

