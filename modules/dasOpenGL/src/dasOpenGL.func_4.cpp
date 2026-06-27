// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasOpenGL.h"
#include "need_dasOpenGL.h"
namespace das {
#include "dasOpenGL.func.aot.decl.inc"
void Module_dasOpenGL::initFunctions_4() {
// from GLES3/gl3.h:649:29
	makeExtern< void (*)(int,float,float,float,float) , glUniform4f , SimNode_ExtFuncCall >(lib,"glUniform4f","glUniform4f")
		->args({"location","v0","v1","v2","v3"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:650:29
	makeExtern< void (*)(int,int,const float *) , glUniform4fv , SimNode_ExtFuncCall >(lib,"glUniform4fv","glUniform4fv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:651:29
	makeExtern< void (*)(int,int,int,int,int) , glUniform4i , SimNode_ExtFuncCall >(lib,"glUniform4i","glUniform4i")
		->args({"location","v0","v1","v2","v3"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:652:29
	makeExtern< void (*)(int,int,const int *) , glUniform4iv , SimNode_ExtFuncCall >(lib,"glUniform4iv","glUniform4iv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:653:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix2fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix2fv","glUniformMatrix2fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:654:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix3fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix3fv","glUniformMatrix3fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:655:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix4fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix4fv","glUniformMatrix4fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:656:29
	makeExtern< void (*)(unsigned int) , glUseProgram , SimNode_ExtFuncCall >(lib,"glUseProgram","glUseProgram")
		->args({"program"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:657:29
	makeExtern< void (*)(unsigned int) , glValidateProgram , SimNode_ExtFuncCall >(lib,"glValidateProgram","glValidateProgram")
		->args({"program"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:658:29
	makeExtern< void (*)(unsigned int,float) , glVertexAttrib1f , SimNode_ExtFuncCall >(lib,"glVertexAttrib1f","glVertexAttrib1f")
		->args({"index","x"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:659:29
	makeExtern< void (*)(unsigned int,const float *) , glVertexAttrib1fv , SimNode_ExtFuncCall >(lib,"glVertexAttrib1fv","glVertexAttrib1fv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:660:29
	makeExtern< void (*)(unsigned int,float,float) , glVertexAttrib2f , SimNode_ExtFuncCall >(lib,"glVertexAttrib2f","glVertexAttrib2f")
		->args({"index","x","y"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:661:29
	makeExtern< void (*)(unsigned int,const float *) , glVertexAttrib2fv , SimNode_ExtFuncCall >(lib,"glVertexAttrib2fv","glVertexAttrib2fv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:662:29
	makeExtern< void (*)(unsigned int,float,float,float) , glVertexAttrib3f , SimNode_ExtFuncCall >(lib,"glVertexAttrib3f","glVertexAttrib3f")
		->args({"index","x","y","z"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:663:29
	makeExtern< void (*)(unsigned int,const float *) , glVertexAttrib3fv , SimNode_ExtFuncCall >(lib,"glVertexAttrib3fv","glVertexAttrib3fv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:664:29
	makeExtern< void (*)(unsigned int,float,float,float,float) , glVertexAttrib4f , SimNode_ExtFuncCall >(lib,"glVertexAttrib4f","glVertexAttrib4f")
		->args({"index","x","y","z","w"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:665:29
	makeExtern< void (*)(unsigned int,const float *) , glVertexAttrib4fv , SimNode_ExtFuncCall >(lib,"glVertexAttrib4fv","glVertexAttrib4fv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:666:29
	makeExtern< void (*)(unsigned int,int,unsigned int,unsigned char,int,const void *) , glVertexAttribPointer , SimNode_ExtFuncCall >(lib,"glVertexAttribPointer","glVertexAttribPointer")
		->args({"index","size","type","normalized","stride","pointer"})
		->arg_type(3,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:667:29
	makeExtern< void (*)(int,int,int,int) , glViewport , SimNode_ExtFuncCall >(lib,"glViewport","glViewport")
		->args({"x","y","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1100:29
	makeExtern< void (*)(unsigned int) , glReadBuffer , SimNode_ExtFuncCall >(lib,"glReadBuffer","glReadBuffer")
		->args({"src"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1101:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,int,unsigned int,const void *) , glDrawRangeElements , SimNode_ExtFuncCall >(lib,"glDrawRangeElements","glDrawRangeElements")
		->args({"mode","start","end","count","type","indices"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1102:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,int,unsigned int,unsigned int,const void *) , glTexImage3D , SimNode_ExtFuncCall >(lib,"glTexImage3D","glTexImage3D")
		->args({"target","level","internalformat","width","height","depth","border","format","type","pixels"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1103:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,int,int,unsigned int,unsigned int,const void *) , glTexSubImage3D , SimNode_ExtFuncCall >(lib,"glTexSubImage3D","glTexSubImage3D")
		->args({"target","level","xoffset","yoffset","zoffset","width","height","depth","format","type","pixels"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1104:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,int,int,int) , glCopyTexSubImage3D , SimNode_ExtFuncCall >(lib,"glCopyTexSubImage3D","glCopyTexSubImage3D")
		->args({"target","level","xoffset","yoffset","zoffset","x","y","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1105:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int,int,int,int,const void *) , glCompressedTexImage3D , SimNode_ExtFuncCall >(lib,"glCompressedTexImage3D","glCompressedTexImage3D")
		->args({"target","level","internalformat","width","height","depth","border","imageSize","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1106:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,int,int,unsigned int,int,const void *) , glCompressedTexSubImage3D , SimNode_ExtFuncCall >(lib,"glCompressedTexSubImage3D","glCompressedTexSubImage3D")
		->args({"target","level","xoffset","yoffset","zoffset","width","height","depth","format","imageSize","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1107:29
	makeExtern< void (*)(int,unsigned int *) , glGenQueries , SimNode_ExtFuncCall >(lib,"glGenQueries","glGenQueries")
		->args({"n","ids"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1108:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteQueries , SimNode_ExtFuncCall >(lib,"glDeleteQueries","glDeleteQueries")
		->args({"n","ids"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1109:34
	makeExtern< unsigned char (*)(unsigned int) , glIsQuery , SimNode_ExtFuncCall >(lib,"glIsQuery","glIsQuery")
		->args({"id"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1110:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBeginQuery , SimNode_ExtFuncCall >(lib,"glBeginQuery","glBeginQuery")
		->args({"target","id"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1111:29
	makeExtern< void (*)(unsigned int) , glEndQuery , SimNode_ExtFuncCall >(lib,"glEndQuery","glEndQuery")
		->args({"target"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1112:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetQueryiv , SimNode_ExtFuncCall >(lib,"glGetQueryiv","glGetQueryiv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1113:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int *) , glGetQueryObjectuiv , SimNode_ExtFuncCall >(lib,"glGetQueryObjectuiv","glGetQueryObjectuiv")
		->args({"id","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1114:34
	makeExtern< unsigned char (*)(unsigned int) , glUnmapBuffer , SimNode_ExtFuncCall >(lib,"glUnmapBuffer","glUnmapBuffer")
		->args({"target"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1115:29
	makeExtern< void (*)(unsigned int,unsigned int,void **) , glGetBufferPointerv , SimNode_ExtFuncCall >(lib,"glGetBufferPointerv","glGetBufferPointerv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1116:29
	makeExtern< void (*)(int,const unsigned int *) , glDrawBuffers , SimNode_ExtFuncCall >(lib,"glDrawBuffers","glDrawBuffers")
		->args({"n","bufs"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1117:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix2x3fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix2x3fv","glUniformMatrix2x3fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1118:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix3x2fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix3x2fv","glUniformMatrix3x2fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1119:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix2x4fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix2x4fv","glUniformMatrix2x4fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1120:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix4x2fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix4x2fv","glUniformMatrix4x2fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
}
}

