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
void Module_dasOpenGL::initFunctions_2() {
// from GLES3/gl3.h:568:29
	makeExtern< void (*)(unsigned int) , glEnable , SimNode_ExtFuncCall >(lib,"glEnable","glEnable")
		->args({"cap"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:569:29
	makeExtern< void (*)(unsigned int) , glEnableVertexAttribArray , SimNode_ExtFuncCall >(lib,"glEnableVertexAttribArray","glEnableVertexAttribArray")
		->args({"index"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:570:29
	makeExtern< void (*)() , glFinish , SimNode_ExtFuncCall >(lib,"glFinish","glFinish")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:571:29
	makeExtern< void (*)() , glFlush , SimNode_ExtFuncCall >(lib,"glFlush","glFlush")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:572:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,unsigned int) , glFramebufferRenderbuffer , SimNode_ExtFuncCall >(lib,"glFramebufferRenderbuffer","glFramebufferRenderbuffer")
		->args({"target","attachment","renderbuffertarget","renderbuffer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:573:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,unsigned int,int) , glFramebufferTexture2D , SimNode_ExtFuncCall >(lib,"glFramebufferTexture2D","glFramebufferTexture2D")
		->args({"target","attachment","textarget","texture","level"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:574:29
	makeExtern< void (*)(unsigned int) , glFrontFace , SimNode_ExtFuncCall >(lib,"glFrontFace","glFrontFace")
		->args({"mode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:575:29
	makeExtern< void (*)(int,unsigned int *) , glGenBuffers , SimNode_ExtFuncCall >(lib,"glGenBuffers","glGenBuffers")
		->args({"n","buffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:576:29
	makeExtern< void (*)(unsigned int) , glGenerateMipmap , SimNode_ExtFuncCall >(lib,"glGenerateMipmap","glGenerateMipmap")
		->args({"target"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:577:29
	makeExtern< void (*)(int,unsigned int *) , glGenFramebuffers , SimNode_ExtFuncCall >(lib,"glGenFramebuffers","glGenFramebuffers")
		->args({"n","framebuffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:578:29
	makeExtern< void (*)(int,unsigned int *) , glGenRenderbuffers , SimNode_ExtFuncCall >(lib,"glGenRenderbuffers","glGenRenderbuffers")
		->args({"n","renderbuffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:579:29
	makeExtern< void (*)(int,unsigned int *) , glGenTextures , SimNode_ExtFuncCall >(lib,"glGenTextures","glGenTextures")
		->args({"n","textures"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:580:29
	makeExtern< void (*)(unsigned int,unsigned int,int,int *,int *,unsigned int *,char *) , glGetActiveAttrib , SimNode_ExtFuncCall >(lib,"glGetActiveAttrib","glGetActiveAttrib")
		->args({"program","index","bufSize","length","size","type","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:581:29
	makeExtern< void (*)(unsigned int,unsigned int,int,int *,int *,unsigned int *,char *) , glGetActiveUniform , SimNode_ExtFuncCall >(lib,"glGetActiveUniform","glGetActiveUniform")
		->args({"program","index","bufSize","length","size","type","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:582:29
	makeExtern< void (*)(unsigned int,int,int *,unsigned int *) , glGetAttachedShaders , SimNode_ExtFuncCall >(lib,"glGetAttachedShaders","glGetAttachedShaders")
		->args({"program","maxCount","count","shaders"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:583:30
	makeExtern< int (*)(unsigned int,const char *) , glGetAttribLocation , SimNode_ExtFuncCall >(lib,"glGetAttribLocation","glGetAttribLocation")
		->args({"program","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:585:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetBufferParameteriv , SimNode_ExtFuncCall >(lib,"glGetBufferParameteriv","glGetBufferParameteriv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:586:31
	makeExtern< unsigned int (*)() , glGetError , SimNode_ExtFuncCall >(lib,"glGetError","glGetError")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:587:29
	makeExtern< void (*)(unsigned int,float *) , glGetFloatv , SimNode_ExtFuncCall >(lib,"glGetFloatv","glGetFloatv")
		->args({"pname","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:588:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,int *) , glGetFramebufferAttachmentParameteriv , SimNode_ExtFuncCall >(lib,"glGetFramebufferAttachmentParameteriv","glGetFramebufferAttachmentParameteriv")
		->args({"target","attachment","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:589:29
	makeExtern< void (*)(unsigned int,int *) , glGetIntegerv , SimNode_ExtFuncCall >(lib,"glGetIntegerv","glGetIntegerv")
		->args({"pname","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:590:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetProgramiv , SimNode_ExtFuncCall >(lib,"glGetProgramiv","glGetProgramiv")
		->args({"program","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:591:29
	makeExtern< void (*)(unsigned int,int,int *,char *) , glGetProgramInfoLog , SimNode_ExtFuncCall >(lib,"glGetProgramInfoLog","glGetProgramInfoLog")
		->args({"program","bufSize","length","infoLog"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:592:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetRenderbufferParameteriv , SimNode_ExtFuncCall >(lib,"glGetRenderbufferParameteriv","glGetRenderbufferParameteriv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:593:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetShaderiv , SimNode_ExtFuncCall >(lib,"glGetShaderiv","glGetShaderiv")
		->args({"shader","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:594:29
	makeExtern< void (*)(unsigned int,int,int *,char *) , glGetShaderInfoLog , SimNode_ExtFuncCall >(lib,"glGetShaderInfoLog","glGetShaderInfoLog")
		->args({"shader","bufSize","length","infoLog"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:595:29
	makeExtern< void (*)(unsigned int,unsigned int,int *,int *) , glGetShaderPrecisionFormat , SimNode_ExtFuncCall >(lib,"glGetShaderPrecisionFormat","glGetShaderPrecisionFormat")
		->args({"shadertype","precisiontype","range","precision"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:596:29
	makeExtern< void (*)(unsigned int,int,int *,char *) , glGetShaderSource , SimNode_ExtFuncCall >(lib,"glGetShaderSource","glGetShaderSource")
		->args({"shader","bufSize","length","source"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:597:39
	makeExtern< const unsigned char * (*)(unsigned int) , glGetString , SimNode_ExtFuncCall >(lib,"glGetString","glGetString")
		->args({"name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:598:29
	makeExtern< void (*)(unsigned int,unsigned int,float *) , glGetTexParameterfv , SimNode_ExtFuncCall >(lib,"glGetTexParameterfv","glGetTexParameterfv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:599:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetTexParameteriv , SimNode_ExtFuncCall >(lib,"glGetTexParameteriv","glGetTexParameteriv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:600:29
	makeExtern< void (*)(unsigned int,int,float *) , glGetUniformfv , SimNode_ExtFuncCall >(lib,"glGetUniformfv","glGetUniformfv")
		->args({"program","location","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:601:29
	makeExtern< void (*)(unsigned int,int,int *) , glGetUniformiv , SimNode_ExtFuncCall >(lib,"glGetUniformiv","glGetUniformiv")
		->args({"program","location","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:602:30
	makeExtern< int (*)(unsigned int,const char *) , glGetUniformLocation , SimNode_ExtFuncCall >(lib,"glGetUniformLocation","glGetUniformLocation")
		->args({"program","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:603:29
	makeExtern< void (*)(unsigned int,unsigned int,float *) , glGetVertexAttribfv , SimNode_ExtFuncCall >(lib,"glGetVertexAttribfv","glGetVertexAttribfv")
		->args({"index","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:604:29
	makeExtern< void (*)(unsigned int,unsigned int,int *) , glGetVertexAttribiv , SimNode_ExtFuncCall >(lib,"glGetVertexAttribiv","glGetVertexAttribiv")
		->args({"index","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:605:29
	makeExtern< void (*)(unsigned int,unsigned int,void **) , glGetVertexAttribPointerv , SimNode_ExtFuncCall >(lib,"glGetVertexAttribPointerv","glGetVertexAttribPointerv")
		->args({"index","pname","pointer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:606:29
	makeExtern< void (*)(unsigned int,unsigned int) , glHint , SimNode_ExtFuncCall >(lib,"glHint","glHint")
		->args({"target","mode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:607:34
	makeExtern< unsigned char (*)(unsigned int) , glIsBuffer , SimNode_ExtFuncCall >(lib,"glIsBuffer","glIsBuffer")
		->args({"buffer"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:608:34
	makeExtern< unsigned char (*)(unsigned int) , glIsEnabled , SimNode_ExtFuncCall >(lib,"glIsEnabled","glIsEnabled")
		->args({"cap"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
}
}

