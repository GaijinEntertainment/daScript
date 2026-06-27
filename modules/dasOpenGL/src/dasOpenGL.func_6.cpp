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
void Module_dasOpenGL::initFunctions_6() {
// from GLES3/gl3.h:1165:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,int *) , glGetActiveUniformBlockiv , SimNode_ExtFuncCall >(lib,"glGetActiveUniformBlockiv","glGetActiveUniformBlockiv")
		->args({"program","uniformBlockIndex","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1166:29
	makeExtern< void (*)(unsigned int,unsigned int,int,int *,char *) , glGetActiveUniformBlockName , SimNode_ExtFuncCall >(lib,"glGetActiveUniformBlockName","glGetActiveUniformBlockName")
		->args({"program","uniformBlockIndex","bufSize","length","uniformBlockName"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1167:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int) , glUniformBlockBinding , SimNode_ExtFuncCall >(lib,"glUniformBlockBinding","glUniformBlockBinding")
		->args({"program","uniformBlockIndex","uniformBlockBinding"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1168:29
	makeExtern< void (*)(unsigned int,int,int,int) , glDrawArraysInstanced , SimNode_ExtFuncCall >(lib,"glDrawArraysInstanced","glDrawArraysInstanced")
		->args({"mode","first","count","instancecount"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1169:29
	makeExtern< void (*)(unsigned int,int,unsigned int,const void *,int) , glDrawElementsInstanced , SimNode_ExtFuncCall >(lib,"glDrawElementsInstanced","glDrawElementsInstanced")
		->args({"mode","count","type","indices","instancecount"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1170:31
	makeExtern< __GLsync * (*)(unsigned int,unsigned int) , glFenceSync , SimNode_ExtFuncCall >(lib,"glFenceSync","glFenceSync")
		->args({"condition","flags"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1171:34
	makeExtern< unsigned char (*)(__GLsync *) , glIsSync , SimNode_ExtFuncCall >(lib,"glIsSync","glIsSync")
		->args({"sync"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1172:29
	makeExtern< void (*)(__GLsync *) , glDeleteSync , SimNode_ExtFuncCall >(lib,"glDeleteSync","glDeleteSync")
		->args({"sync"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1173:31
	makeExtern< unsigned int (*)(__GLsync *,unsigned int,GLuint64) , glClientWaitSync , SimNode_ExtFuncCall >(lib,"glClientWaitSync","glClientWaitSync")
		->args({"sync","flags","timeout"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1174:29
	makeExtern< void (*)(__GLsync *,unsigned int,GLuint64) , glWaitSync , SimNode_ExtFuncCall >(lib,"glWaitSync","glWaitSync")
		->args({"sync","flags","timeout"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1175:29
	makeExtern< void (*)(unsigned int,GLint64 *) , glGetInteger64v , SimNode_ExtFuncCall >(lib,"glGetInteger64v","glGetInteger64v")
		->args({"pname","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1176:29
	makeExtern< void (*)(__GLsync *,unsigned int,int,int *,int *) , glGetSynciv , SimNode_ExtFuncCall >(lib,"glGetSynciv","glGetSynciv")
		->args({"sync","pname","count","length","values"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1177:29
	makeExtern< void (*)(unsigned int,unsigned int,GLint64 *) , glGetInteger64i_v , SimNode_ExtFuncCall >(lib,"glGetInteger64i_v","glGetInteger64i_v")
		->args({"target","index","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1178:29
	makeExtern< void (*)(unsigned int,unsigned int,GLint64 *) , glGetBufferParameteri64v , SimNode_ExtFuncCall >(lib,"glGetBufferParameteri64v","glGetBufferParameteri64v")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1179:29
	makeExtern< void (*)(int,unsigned int *) , glGenSamplers , SimNode_ExtFuncCall >(lib,"glGenSamplers","glGenSamplers")
		->args({"count","samplers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1180:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteSamplers , SimNode_ExtFuncCall >(lib,"glDeleteSamplers","glDeleteSamplers")
		->args({"count","samplers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1181:34
	makeExtern< unsigned char (*)(unsigned int) , glIsSampler , SimNode_ExtFuncCall >(lib,"glIsSampler","glIsSampler")
		->args({"sampler"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1182:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindSampler , SimNode_ExtFuncCall >(lib,"glBindSampler","glBindSampler")
		->args({"unit","sampler"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1183:29
	makeExtern< void (*)(unsigned int,unsigned int,int) , glSamplerParameteri , SimNode_ExtFuncCall >(lib,"glSamplerParameteri","glSamplerParameteri")
		->args({"sampler","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1184:29
	makeExtern< void (*)(unsigned int,unsigned int,const int *) , glSamplerParameteriv , SimNode_ExtFuncCall >(lib,"glSamplerParameteriv","glSamplerParameteriv")
		->args({"sampler","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1185:29
	makeExtern< void (*)(unsigned int,unsigned int,float) , glSamplerParameterf , SimNode_ExtFuncCall >(lib,"glSamplerParameterf","glSamplerParameterf")
		->args({"sampler","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1186:29
	makeExtern< void (*)(unsigned int,unsigned int,const float *) , glSamplerParameterfv , SimNode_ExtFuncCall >(lib,"glSamplerParameterfv","glSamplerParameterfv")
		->args({"sampler","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1187:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetSamplerParameteriv , SimNode_ExtFuncCall >(lib,"glGetSamplerParameteriv","glGetSamplerParameteriv")
		->args({"sampler","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1188:29
	makeExtern< void (*)(unsigned int,unsigned int,float *) , glGetSamplerParameterfv , SimNode_ExtFuncCall >(lib,"glGetSamplerParameterfv","glGetSamplerParameterfv")
		->args({"sampler","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1189:29
	makeExtern< void (*)(unsigned int,unsigned int) , glVertexAttribDivisor , SimNode_ExtFuncCall >(lib,"glVertexAttribDivisor","glVertexAttribDivisor")
		->args({"index","divisor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1190:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindTransformFeedback , SimNode_ExtFuncCall >(lib,"glBindTransformFeedback","glBindTransformFeedback")
		->args({"target","id"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1191:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteTransformFeedbacks , SimNode_ExtFuncCall >(lib,"glDeleteTransformFeedbacks","glDeleteTransformFeedbacks")
		->args({"n","ids"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1192:29
	makeExtern< void (*)(int,unsigned int *) , glGenTransformFeedbacks , SimNode_ExtFuncCall >(lib,"glGenTransformFeedbacks","glGenTransformFeedbacks")
		->args({"n","ids"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1193:34
	makeExtern< unsigned char (*)(unsigned int) , glIsTransformFeedback , SimNode_ExtFuncCall >(lib,"glIsTransformFeedback","glIsTransformFeedback")
		->args({"id"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1194:29
	makeExtern< void (*)() , glPauseTransformFeedback , SimNode_ExtFuncCall >(lib,"glPauseTransformFeedback","glPauseTransformFeedback")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1195:29
	makeExtern< void (*)() , glResumeTransformFeedback , SimNode_ExtFuncCall >(lib,"glResumeTransformFeedback","glResumeTransformFeedback")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1196:29
	makeExtern< void (*)(unsigned int,int,int *,unsigned int *,void *) , glGetProgramBinary , SimNode_ExtFuncCall >(lib,"glGetProgramBinary","glGetProgramBinary")
		->args({"program","bufSize","length","binaryFormat","binary"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1197:29
	makeExtern< void (*)(unsigned int,unsigned int,const void *,int) , glProgramBinary , SimNode_ExtFuncCall >(lib,"glProgramBinary","glProgramBinary")
		->args({"program","binaryFormat","binary","length"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1198:29
	makeExtern< void (*)(unsigned int,unsigned int,int) , glProgramParameteri , SimNode_ExtFuncCall >(lib,"glProgramParameteri","glProgramParameteri")
		->args({"program","pname","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1199:29
	makeExtern< void (*)(unsigned int,int,const unsigned int *) , glInvalidateFramebuffer , SimNode_ExtFuncCall >(lib,"glInvalidateFramebuffer","glInvalidateFramebuffer")
		->args({"target","numAttachments","attachments"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1200:29
	makeExtern< void (*)(unsigned int,int,const unsigned int *,int,int,int,int) , glInvalidateSubFramebuffer , SimNode_ExtFuncCall >(lib,"glInvalidateSubFramebuffer","glInvalidateSubFramebuffer")
		->args({"target","numAttachments","attachments","x","y","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1201:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int) , glTexStorage2D , SimNode_ExtFuncCall >(lib,"glTexStorage2D","glTexStorage2D")
		->args({"target","levels","internalformat","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1202:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int,int) , glTexStorage3D , SimNode_ExtFuncCall >(lib,"glTexStorage3D","glTexStorage3D")
		->args({"target","levels","internalformat","width","height","depth"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:1203:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,int,int *) , glGetInternalformativ , SimNode_ExtFuncCall >(lib,"glGetInternalformativ","glGetInternalformativ")
		->args({"target","internalformat","pname","count","params"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

