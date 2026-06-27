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
void Module_dasOpenGL::initFunctions_1() {
// from GLES3/gl3.h:526:29
	makeExtern< void (*)(unsigned int) , glActiveTexture , SimNode_ExtFuncCall >(lib,"glActiveTexture","glActiveTexture")
		->args({"texture"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:527:29
	makeExtern< void (*)(unsigned int,unsigned int) , glAttachShader , SimNode_ExtFuncCall >(lib,"glAttachShader","glAttachShader")
		->args({"program","shader"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:528:29
	makeExtern< void (*)(unsigned int,unsigned int,const char *) , glBindAttribLocation , SimNode_ExtFuncCall >(lib,"glBindAttribLocation","glBindAttribLocation")
		->args({"program","index","name"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:529:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindBuffer , SimNode_ExtFuncCall >(lib,"glBindBuffer","glBindBuffer")
		->args({"target","buffer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:530:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindFramebuffer , SimNode_ExtFuncCall >(lib,"glBindFramebuffer","glBindFramebuffer")
		->args({"target","framebuffer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:531:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindRenderbuffer , SimNode_ExtFuncCall >(lib,"glBindRenderbuffer","glBindRenderbuffer")
		->args({"target","renderbuffer"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:532:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBindTexture , SimNode_ExtFuncCall >(lib,"glBindTexture","glBindTexture")
		->args({"target","texture"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:533:29
	makeExtern< void (*)(float,float,float,float) , glBlendColor , SimNode_ExtFuncCall >(lib,"glBlendColor","glBlendColor")
		->args({"red","green","blue","alpha"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:534:29
	makeExtern< void (*)(unsigned int) , glBlendEquation , SimNode_ExtFuncCall >(lib,"glBlendEquation","glBlendEquation")
		->args({"mode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:535:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBlendEquationSeparate , SimNode_ExtFuncCall >(lib,"glBlendEquationSeparate","glBlendEquationSeparate")
		->args({"modeRGB","modeAlpha"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:536:29
	makeExtern< void (*)(unsigned int,unsigned int) , glBlendFunc , SimNode_ExtFuncCall >(lib,"glBlendFunc","glBlendFunc")
		->args({"sfactor","dfactor"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:537:29
	makeExtern< void (*)(unsigned int,unsigned int,unsigned int,unsigned int) , glBlendFuncSeparate , SimNode_ExtFuncCall >(lib,"glBlendFuncSeparate","glBlendFuncSeparate")
		->args({"sfactorRGB","dfactorRGB","sfactorAlpha","dfactorAlpha"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:540:31
	makeExtern< unsigned int (*)(unsigned int) , glCheckFramebufferStatus , SimNode_ExtFuncCall >(lib,"glCheckFramebufferStatus","glCheckFramebufferStatus")
		->args({"target"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:541:29
	makeExtern< void (*)(unsigned int) , glClear , SimNode_ExtFuncCall >(lib,"glClear","glClear")
		->args({"mask"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:542:29
	makeExtern< void (*)(float,float,float,float) , glClearColor , SimNode_ExtFuncCall >(lib,"glClearColor","glClearColor")
		->args({"red","green","blue","alpha"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:543:29
	makeExtern< void (*)(float) , glClearDepthf , SimNode_ExtFuncCall >(lib,"glClearDepthf","glClearDepthf")
		->args({"d"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:544:29
	makeExtern< void (*)(int) , glClearStencil , SimNode_ExtFuncCall >(lib,"glClearStencil","glClearStencil")
		->args({"s"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:545:29
	makeExtern< void (*)(unsigned char,unsigned char,unsigned char,unsigned char) , glColorMask , SimNode_ExtFuncCall >(lib,"glColorMask","glColorMask")
		->args({"red","green","blue","alpha"})
		->arg_type(0,makeType<bool>(lib))
		->arg_type(1,makeType<bool>(lib))
		->arg_type(2,makeType<bool>(lib))
		->arg_type(3,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:546:29
	makeExtern< void (*)(unsigned int) , glCompileShader , SimNode_ExtFuncCall >(lib,"glCompileShader","glCompileShader")
		->args({"shader"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:547:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int,int,int,const void *) , glCompressedTexImage2D , SimNode_ExtFuncCall >(lib,"glCompressedTexImage2D","glCompressedTexImage2D")
		->args({"target","level","internalformat","width","height","border","imageSize","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:548:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,unsigned int,int,const void *) , glCompressedTexSubImage2D , SimNode_ExtFuncCall >(lib,"glCompressedTexSubImage2D","glCompressedTexSubImage2D")
		->args({"target","level","xoffset","yoffset","width","height","format","imageSize","data"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:549:29
	makeExtern< void (*)(unsigned int,int,unsigned int,int,int,int,int,int) , glCopyTexImage2D , SimNode_ExtFuncCall >(lib,"glCopyTexImage2D","glCopyTexImage2D")
		->args({"target","level","internalformat","x","y","width","height","border"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:550:29
	makeExtern< void (*)(unsigned int,int,int,int,int,int,int,int) , glCopyTexSubImage2D , SimNode_ExtFuncCall >(lib,"glCopyTexSubImage2D","glCopyTexSubImage2D")
		->args({"target","level","xoffset","yoffset","x","y","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:551:31
	makeExtern< unsigned int (*)() , glCreateProgram , SimNode_ExtFuncCall >(lib,"glCreateProgram","glCreateProgram")
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:552:31
	makeExtern< unsigned int (*)(unsigned int) , glCreateShader , SimNode_ExtFuncCall >(lib,"glCreateShader","glCreateShader")
		->args({"type"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:553:29
	makeExtern< void (*)(unsigned int) , glCullFace , SimNode_ExtFuncCall >(lib,"glCullFace","glCullFace")
		->args({"mode"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:554:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteBuffers , SimNode_ExtFuncCall >(lib,"glDeleteBuffers","glDeleteBuffers")
		->args({"n","buffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:555:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteFramebuffers , SimNode_ExtFuncCall >(lib,"glDeleteFramebuffers","glDeleteFramebuffers")
		->args({"n","framebuffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:556:29
	makeExtern< void (*)(unsigned int) , glDeleteProgram , SimNode_ExtFuncCall >(lib,"glDeleteProgram","glDeleteProgram")
		->args({"program"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:557:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteRenderbuffers , SimNode_ExtFuncCall >(lib,"glDeleteRenderbuffers","glDeleteRenderbuffers")
		->args({"n","renderbuffers"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:558:29
	makeExtern< void (*)(unsigned int) , glDeleteShader , SimNode_ExtFuncCall >(lib,"glDeleteShader","glDeleteShader")
		->args({"shader"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:559:29
	makeExtern< void (*)(int,const unsigned int *) , glDeleteTextures , SimNode_ExtFuncCall >(lib,"glDeleteTextures","glDeleteTextures")
		->args({"n","textures"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:560:29
	makeExtern< void (*)(unsigned int) , glDepthFunc , SimNode_ExtFuncCall >(lib,"glDepthFunc","glDepthFunc")
		->args({"func"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:561:29
	makeExtern< void (*)(unsigned char) , glDepthMask , SimNode_ExtFuncCall >(lib,"glDepthMask","glDepthMask")
		->args({"flag"})
		->arg_type(0,makeType<bool>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:562:29
	makeExtern< void (*)(float,float) , glDepthRangef , SimNode_ExtFuncCall >(lib,"glDepthRangef","glDepthRangef")
		->args({"n","f"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:563:29
	makeExtern< void (*)(unsigned int,unsigned int) , glDetachShader , SimNode_ExtFuncCall >(lib,"glDetachShader","glDetachShader")
		->args({"program","shader"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:564:29
	makeExtern< void (*)(unsigned int) , glDisable , SimNode_ExtFuncCall >(lib,"glDisable","glDisable")
		->args({"cap"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:565:29
	makeExtern< void (*)(unsigned int) , glDisableVertexAttribArray , SimNode_ExtFuncCall >(lib,"glDisableVertexAttribArray","glDisableVertexAttribArray")
		->args({"index"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:566:29
	makeExtern< void (*)(unsigned int,int,int) , glDrawArrays , SimNode_ExtFuncCall >(lib,"glDrawArrays","glDrawArrays")
		->args({"mode","first","count"})
		->addToModule(*this, SideEffects::worstDefault);
// from GLES3/gl3.h:567:29
	makeExtern< void (*)(unsigned int,int,unsigned int,const void *) , glDrawElements , SimNode_ExtFuncCall >(lib,"glDrawElements","glDrawElements")
		->args({"mode","count","type","indices"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

