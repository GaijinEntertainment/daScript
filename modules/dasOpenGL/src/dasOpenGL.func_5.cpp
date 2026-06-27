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
void Module_dasOpenGL::initFunctions_5() {
// from GLES3/gl3.h:1121:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix3x4fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix3x4fv","glUniformMatrix3x4fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1122:29
	makeExtern< void (*)(int,int,unsigned char,const float *) , glUniformMatrix4x3fv , SimNode_ExtFuncCall >(lib,"glUniformMatrix4x3fv","glUniformMatrix4x3fv")
		->args({"location","count","transpose","value"})
		->arg_type(2,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1123:29
	makeExtern< void (*)(int,int,int,int,int,int,int,int,unsigned int,unsigned int) , glBlitFramebuffer , SimNode_ExtFuncCall >(lib,"glBlitFramebuffer","glBlitFramebuffer")
		->args({"srcX0","srcY0","srcX1","srcY1","dstX0","dstY0","dstX1","dstY1","mask","filter"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1124:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int) , glRenderbufferStorageMultisample , SimNode_ExtFuncCall >(lib,"glRenderbufferStorageMultisample","glRenderbufferStorageMultisample")
		->args({"target","samples","internalformat","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1125:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,int,int) , glFramebufferTextureLayer , SimNode_ExtFuncCall >(lib,"glFramebufferTextureLayer","glFramebufferTextureLayer")
		->args({"target","attachment","texture","level","layer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1128:29
	makeExtern< void (*)(unsigned int) , glBindVertexArray , SimNode_ExtFuncCall >(lib,"glBindVertexArray","glBindVertexArray")
		->args({"array"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1129:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteVertexArrays , SimNode_ExtFuncCall >(lib,"glDeleteVertexArrays","glDeleteVertexArrays")
		->args({"n","arrays"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1130:29
	makeExtern< void (*)(int,unsigned int *) , glGenVertexArrays , SimNode_ExtFuncCall >(lib,"glGenVertexArrays","glGenVertexArrays")
		->args({"n","arrays"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1131:34
	makeExtern< unsigned char (*)(unsigned int) , glIsVertexArray , SimNode_ExtFuncCall >(lib,"glIsVertexArray","glIsVertexArray")
		->args({"array"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1132:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetIntegeri_v , SimNode_ExtFuncCall >(lib,"glGetIntegeri_v","glGetIntegeri_v")
		->args({"target","index","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1133:29
	makeExtern< void (*)(unsigned int) , glBeginTransformFeedback , SimNode_ExtFuncCall >(lib,"glBeginTransformFeedback","glBeginTransformFeedback")
		->args({"primitiveMode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1134:29
	makeExtern< void (*)() , glEndTransformFeedback , SimNode_ExtFuncCall >(lib,"glEndTransformFeedback","glEndTransformFeedback")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1136:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int) , glBindBufferBase , SimNode_ExtFuncCall >(lib,"glBindBufferBase","glBindBufferBase")
		->args({"target","index","buffer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1137:29
	makeExtern< void (*)(unsigned int,int,const char *const *,unsigned int) , glTransformFeedbackVaryings , SimNode_ExtFuncCall >(lib,"glTransformFeedbackVaryings","glTransformFeedbackVaryings")
		->args({"program","count","varyings","bufferMode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1138:29
	makeExtern< void (*)(unsigned int,unsigned int,int,int *,int *,unsigned int *,char *) , glGetTransformFeedbackVarying , SimNode_ExtFuncCall >(lib,"glGetTransformFeedbackVarying","glGetTransformFeedbackVarying")
		->args({"program","index","bufSize","length","size","type","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1139:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,const void *) , glVertexAttribIPointer , SimNode_ExtFuncCall >(lib,"glVertexAttribIPointer","glVertexAttribIPointer")
		->args({"index","size","type","stride","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1140:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetVertexAttribIiv , SimNode_ExtFuncCall >(lib,"glGetVertexAttribIiv","glGetVertexAttribIiv")
		->args({"index","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1141:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int *) , glGetVertexAttribIuiv , SimNode_ExtFuncCall >(lib,"glGetVertexAttribIuiv","glGetVertexAttribIuiv")
		->args({"index","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1142:29
	makeExtern< void (*)(unsigned int,int,int,int,int) , glVertexAttribI4i , SimNode_ExtFuncCall >(lib,"glVertexAttribI4i","glVertexAttribI4i")
		->args({"index","x","y","z","w"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1143:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,unsigned int,unsigned int) , glVertexAttribI4ui , SimNode_ExtFuncCall >(lib,"glVertexAttribI4ui","glVertexAttribI4ui")
		->args({"index","x","y","z","w"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1144:29
	makeExtern< void (*)(unsigned int,const int *) , glVertexAttribI4iv , SimNode_ExtFuncCall >(lib,"glVertexAttribI4iv","glVertexAttribI4iv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1145:29
	makeExtern< void (*)(unsigned int,const unsigned int *) , glVertexAttribI4uiv , SimNode_ExtFuncCall >(lib,"glVertexAttribI4uiv","glVertexAttribI4uiv")
		->args({"index","v"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1146:29
	makeExtern< void (*)(unsigned int,int,unsigned int *) , glGetUniformuiv , SimNode_ExtFuncCall >(lib,"glGetUniformuiv","glGetUniformuiv")
		->args({"program","location","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1147:30
	makeExtern< int (*)(unsigned int,const char *) , glGetFragDataLocation , SimNode_ExtFuncCall >(lib,"glGetFragDataLocation","glGetFragDataLocation")
		->args({"program","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1148:29
	makeExtern< void (*)(int,unsigned int) , glUniform1ui , SimNode_ExtFuncCall >(lib,"glUniform1ui","glUniform1ui")
		->args({"location","v0"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1149:29
	makeExtern< void (*)(int,unsigned int,unsigned int) , glUniform2ui , SimNode_ExtFuncCall >(lib,"glUniform2ui","glUniform2ui")
		->args({"location","v0","v1"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1150:29
	makeExtern< void (*)(int,unsigned int,unsigned int,unsigned int) , glUniform3ui , SimNode_ExtFuncCall >(lib,"glUniform3ui","glUniform3ui")
		->args({"location","v0","v1","v2"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1151:29
	makeExtern< void (*)(int,unsigned int,unsigned int,unsigned int,unsigned int) , glUniform4ui , SimNode_ExtFuncCall >(lib,"glUniform4ui","glUniform4ui")
		->args({"location","v0","v1","v2","v3"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1152:29
	makeExtern< void (*)(int,int,const unsigned int *) , glUniform1uiv , SimNode_ExtFuncCall >(lib,"glUniform1uiv","glUniform1uiv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1153:29
	makeExtern< void (*)(int,int,const unsigned int *) , glUniform2uiv , SimNode_ExtFuncCall >(lib,"glUniform2uiv","glUniform2uiv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1154:29
	makeExtern< void (*)(int,int,const unsigned int *) , glUniform3uiv , SimNode_ExtFuncCall >(lib,"glUniform3uiv","glUniform3uiv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1155:29
	makeExtern< void (*)(int,int,const unsigned int *) , glUniform4uiv , SimNode_ExtFuncCall >(lib,"glUniform4uiv","glUniform4uiv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1156:29
	makeExtern< void (*)(unsigned int,int,const int *) , glClearBufferiv , SimNode_ExtFuncCall >(lib,"glClearBufferiv","glClearBufferiv")
		->args({"buffer","drawbuffer","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1157:29
	makeExtern< void (*)(unsigned int,int,const unsigned int *) , glClearBufferuiv , SimNode_ExtFuncCall >(lib,"glClearBufferuiv","glClearBufferuiv")
		->args({"buffer","drawbuffer","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1158:29
	makeExtern< void (*)(unsigned int,int,const float *) , glClearBufferfv , SimNode_ExtFuncCall >(lib,"glClearBufferfv","glClearBufferfv")
		->args({"buffer","drawbuffer","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1159:29
	makeExtern< void (*)(unsigned int,int,float,int) , glClearBufferfi , SimNode_ExtFuncCall >(lib,"glClearBufferfi","glClearBufferfi")
		->args({"buffer","drawbuffer","depth","stencil"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1160:39
	makeExtern< const unsigned char * (*)(unsigned int,unsigned int) , glGetStringi , SimNode_ExtFuncCall >(lib,"glGetStringi","glGetStringi")
		->args({"name","index"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1162:29
	makeExtern< void (*)(unsigned int,int,const char *const *,unsigned int *) , glGetUniformIndices , SimNode_ExtFuncCall >(lib,"glGetUniformIndices","glGetUniformIndices")
		->args({"program","uniformCount","uniformNames","uniformIndices"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1163:29
	makeExtern< void (*)(unsigned int,int,const unsigned int *,unsigned int,int *) , glGetActiveUniformsiv , SimNode_ExtFuncCall >(lib,"glGetActiveUniformsiv","glGetActiveUniformsiv")
		->args({"program","uniformCount","uniformIndices","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1164:31
	makeExtern< unsigned int (*)(unsigned int,const char *) , glGetUniformBlockIndex , SimNode_ExtFuncCall >(lib,"glGetUniformBlockIndex","glGetUniformBlockIndex")
		->args({"program","uniformBlockName"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

