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
void Module_dasOpenGL::initFunctions_3() {
// from GLES3/gl3.h:609:34
	makeExtern< unsigned char (*)(unsigned int) , glIsFramebuffer , SimNode_ExtFuncCall >(lib,"glIsFramebuffer","glIsFramebuffer")
		->args({"framebuffer"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:610:34
	makeExtern< unsigned char (*)(unsigned int) , glIsProgram , SimNode_ExtFuncCall >(lib,"glIsProgram","glIsProgram")
		->args({"program"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:611:34
	makeExtern< unsigned char (*)(unsigned int) , glIsRenderbuffer , SimNode_ExtFuncCall >(lib,"glIsRenderbuffer","glIsRenderbuffer")
		->args({"renderbuffer"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:612:34
	makeExtern< unsigned char (*)(unsigned int) , glIsShader , SimNode_ExtFuncCall >(lib,"glIsShader","glIsShader")
		->args({"shader"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:613:34
	makeExtern< unsigned char (*)(unsigned int) , glIsTexture , SimNode_ExtFuncCall >(lib,"glIsTexture","glIsTexture")
		->args({"texture"})
		->res_type(makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:614:29
	makeExtern< void (*)(float) , glLineWidth , SimNode_ExtFuncCall >(lib,"glLineWidth","glLineWidth")
		->args({"width"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:615:29
	makeExtern< void (*)(unsigned int) , glLinkProgram , SimNode_ExtFuncCall >(lib,"glLinkProgram","glLinkProgram")
		->args({"program"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:616:29
	makeExtern< void (*)(unsigned int,int) , glPixelStorei , SimNode_ExtFuncCall >(lib,"glPixelStorei","glPixelStorei")
		->args({"pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:617:29
	makeExtern< void (*)(float,float) , glPolygonOffset , SimNode_ExtFuncCall >(lib,"glPolygonOffset","glPolygonOffset")
		->args({"factor","units"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:618:29
	makeExtern< void (*)(int,int,int,int,unsigned int,unsigned int,void *) , glReadPixels , SimNode_ExtFuncCall >(lib,"glReadPixels","glReadPixels")
		->args({"x","y","width","height","format","type","pixels"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:619:29
	makeExtern< void (*)() , glReleaseShaderCompiler , SimNode_ExtFuncCall >(lib,"glReleaseShaderCompiler","glReleaseShaderCompiler")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:620:29
	makeExtern< void (*)(unsigned int,unsigned int,int,int) , glRenderbufferStorage , SimNode_ExtFuncCall >(lib,"glRenderbufferStorage","glRenderbufferStorage")
		->args({"target","internalformat","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:621:29
	makeExtern< void (*)(float,unsigned char) , glSampleCoverage , SimNode_ExtFuncCall >(lib,"glSampleCoverage","glSampleCoverage")
		->args({"value","invert"})
		->arg_type(1,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:622:29
	makeExtern< void (*)(int,int,int,int) , glScissor , SimNode_ExtFuncCall >(lib,"glScissor","glScissor")
		->args({"x","y","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:623:29
	makeExtern< void (*)(int,const unsigned int *,unsigned int,const void *,int) , glShaderBinary , SimNode_ExtFuncCall >(lib,"glShaderBinary","glShaderBinary")
		->args({"count","shaders","binaryformat","binary","length"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:624:29
	makeExtern< void (*)(unsigned int,int,const char *const *,const int *) , glShaderSource , SimNode_ExtFuncCall >(lib,"glShaderSource","glShaderSource")
		->args({"shader","count","string","length"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:625:29
	makeExtern< void (*)(unsigned int,int,unsigned int) , glStencilFunc , SimNode_ExtFuncCall >(lib,"glStencilFunc","glStencilFunc")
		->args({"func","ref","mask"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:626:29
	makeExtern< void (*)(unsigned int,unsigned int,int,unsigned int) , glStencilFuncSeparate , SimNode_ExtFuncCall >(lib,"glStencilFuncSeparate","glStencilFuncSeparate")
		->args({"face","func","ref","mask"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:627:29
	makeExtern< void (*)(unsigned int) , glStencilMask , SimNode_ExtFuncCall >(lib,"glStencilMask","glStencilMask")
		->args({"mask"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:628:29
	makeExtern< void (*)(unsigned int,unsigned int) , glStencilMaskSeparate , SimNode_ExtFuncCall >(lib,"glStencilMaskSeparate","glStencilMaskSeparate")
		->args({"face","mask"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:629:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int) , glStencilOp , SimNode_ExtFuncCall >(lib,"glStencilOp","glStencilOp")
		->args({"fail","zfail","zpass"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:630:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,unsigned int) , glStencilOpSeparate , SimNode_ExtFuncCall >(lib,"glStencilOpSeparate","glStencilOpSeparate")
		->args({"face","sfail","dpfail","dppass"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:631:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,unsigned int,unsigned int,const void *) , glTexImage2D , SimNode_ExtFuncCall >(lib,"glTexImage2D","glTexImage2D")
		->args({"target","level","internalformat","width","height","border","format","type","pixels"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:632:29
	makeExtern< void (*)(unsigned int,unsigned int,float) , glTexParameterf , SimNode_ExtFuncCall >(lib,"glTexParameterf","glTexParameterf")
		->args({"target","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:633:29
	makeExtern< void (*)(unsigned int,unsigned int,const float *) , glTexParameterfv , SimNode_ExtFuncCall >(lib,"glTexParameterfv","glTexParameterfv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:634:29
	makeExtern< void (*)(unsigned int,unsigned int,int) , glTexParameteri , SimNode_ExtFuncCall >(lib,"glTexParameteri","glTexParameteri")
		->args({"target","pname","param"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:635:29
	makeExtern< void (*)(unsigned int,unsigned int,const int *) , glTexParameteriv , SimNode_ExtFuncCall >(lib,"glTexParameteriv","glTexParameteriv")
		->args({"target","pname","params"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:636:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,unsigned int,unsigned int,const void *) , glTexSubImage2D , SimNode_ExtFuncCall >(lib,"glTexSubImage2D","glTexSubImage2D")
		->args({"target","level","xoffset","yoffset","width","height","format","type","pixels"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:637:29
	makeExtern< void (*)(int,float) , glUniform1f , SimNode_ExtFuncCall >(lib,"glUniform1f","glUniform1f")
		->args({"location","v0"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:638:29
	makeExtern< void (*)(int,int,const float *) , glUniform1fv , SimNode_ExtFuncCall >(lib,"glUniform1fv","glUniform1fv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:639:29
	makeExtern< void (*)(int,int) , glUniform1i , SimNode_ExtFuncCall >(lib,"glUniform1i","glUniform1i")
		->args({"location","v0"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:640:29
	makeExtern< void (*)(int,int,const int *) , glUniform1iv , SimNode_ExtFuncCall >(lib,"glUniform1iv","glUniform1iv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:641:29
	makeExtern< void (*)(int,float,float) , glUniform2f , SimNode_ExtFuncCall >(lib,"glUniform2f","glUniform2f")
		->args({"location","v0","v1"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:642:29
	makeExtern< void (*)(int,int,const float *) , glUniform2fv , SimNode_ExtFuncCall >(lib,"glUniform2fv","glUniform2fv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:643:29
	makeExtern< void (*)(int,int,int) , glUniform2i , SimNode_ExtFuncCall >(lib,"glUniform2i","glUniform2i")
		->args({"location","v0","v1"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:644:29
	makeExtern< void (*)(int,int,const int *) , glUniform2iv , SimNode_ExtFuncCall >(lib,"glUniform2iv","glUniform2iv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:645:29
	makeExtern< void (*)(int,float,float,float) , glUniform3f , SimNode_ExtFuncCall >(lib,"glUniform3f","glUniform3f")
		->args({"location","v0","v1","v2"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:646:29
	makeExtern< void (*)(int,int,const float *) , glUniform3fv , SimNode_ExtFuncCall >(lib,"glUniform3fv","glUniform3fv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:647:29
	makeExtern< void (*)(int,int,int,int) , glUniform3i , SimNode_ExtFuncCall >(lib,"glUniform3i","glUniform3i")
		->args({"location","v0","v1","v2"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:648:29
	makeExtern< void (*)(int,int,const int *) , glUniform3iv , SimNode_ExtFuncCall >(lib,"glUniform3iv","glUniform3iv")
		->args({"location","count","value"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

