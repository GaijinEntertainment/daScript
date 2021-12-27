// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasSFML.h"
#include "need_dasSFML.h"
namespace das {
#include "dasSFML.func.aot.decl.inc"
void Module_dasSFML::initFunctions_4() {
	using _method_244 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_244)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isSmooth >::invoke")
		->args({"self"});
	using _method_245 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_245)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_246 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_246)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isRepeated >::invoke")
		->args({"self"});
	using _method_247 = das::das_call_member< bool (sf::RenderTexture::*)(),&sf::RenderTexture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_247)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() , &sf::RenderTexture::generateMipmap >::invoke")
		->args({"self"});
	addExtern< bool (*)(sf::RenderTexture &,bool) , _dasSFML_virtual_52_setActive >(*this,lib,"setActive",SideEffects::worstDefault,"_dasSFML_virtual_52_setActive")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_249 = das::das_call_member< void (sf::RenderTexture::*)(),&sf::RenderTexture::display >;
	addExtern<DAS_CALL_METHOD(_method_249)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)() , &sf::RenderTexture::display >::invoke")
		->args({"self"});
	addExtern< sf::Vector2<unsigned int> (*)(const sf::RenderTexture &) , _dasSFML_virtual_53_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_53_getSize")
		->args({"self"});
	addExtern< bool (*)(const sf::RenderTexture &) , _dasSFML_virtual_54_isSrgb >(*this,lib,"isSrgb",SideEffects::worstDefault,"_dasSFML_virtual_54_isSrgb")
		->args({"self"});
	using _method_252 = das::das_call_member< const sf::Texture & (sf::RenderTexture::*)() const,&sf::RenderTexture::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_252), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::RenderTexture::*)() const , &sf::RenderTexture::getTexture >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RenderWindow>(*this,lib,"RenderWindow","sf::RenderWindow");
	addCtorAndUsing<sf::RenderWindow,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &>(*this,lib,"RenderWindow","sf::RenderWindow")
		->args({"mode","title","style","settings"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< sf::Vector2<unsigned int> (*)(const sf::RenderWindow &) , _dasSFML_virtual_55_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_55_getSize")
		->args({"self"});
	addExtern< bool (*)(const sf::RenderWindow &) , _dasSFML_virtual_56_isSrgb >(*this,lib,"isSrgb",SideEffects::worstDefault,"_dasSFML_virtual_56_isSrgb")
		->args({"self"});
	addExtern< bool (*)(sf::RenderWindow &,bool) , _dasSFML_virtual_57_setActive >(*this,lib,"setActive",SideEffects::worstDefault,"_dasSFML_virtual_57_setActive")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	addExtern< void (*)(const sf::Transform &,sf::priv::Matrix<3, 3> &) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","dest"});
	addExtern< void (*)(const sf::Transform &,sf::priv::Matrix<4, 4> &) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","dest"});
	addExtern< void (*)(const float *,std::size_t,float *) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","elements","dest"});
	addExtern< void (*)(const sf::Color &,sf::priv::Vector4<float> &) , sf::priv::copyVector >(*this,lib,"copyVector",SideEffects::worstDefault,"sf::priv::copyVector")
		->args({"source","dest"});
	addExtern< void (*)(const sf::Color &,sf::priv::Vector4<int> &) , sf::priv::copyVector >(*this,lib,"copyVector",SideEffects::worstDefault,"sf::priv::copyVector")
		->args({"source","dest"});
	addCtorAndUsing<sf::Shader>(*this,lib,"Shader","sf::Shader");
	using _method_256 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_256)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","filename","type"});
	using _method_257 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_257)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","fragmentShaderFilename"});
	using _method_258 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_258)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","geometryShaderFilename","fragmentShaderFilename"});
	using _method_259 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_259)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","shader","type"});
	using _method_260 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_260)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","fragmentShader"});
	using _method_261 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_261)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","geometryShader","fragmentShader"});
	using _method_262 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_262)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","stream","type"});
	using _method_263 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_263)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","fragmentShaderStream"});
	using _method_264 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_264)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","geometryShaderStream","fragmentShaderStream"});
	using _method_265 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_265)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_266 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_266)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_267 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_267)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_268 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_268)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_269 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_269)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_270 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_270)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_271 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_271)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_272 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_272)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_273 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_273)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_274 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_274)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_275 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_275)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_276 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_276)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_277 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_277)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_278 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_278)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_279 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_279)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","texture"});
	using _method_280 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_280)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType) , &sf::Shader::setUniform >::invoke")
		->args({"self","name",""});
	using _method_281 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_281)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","scalarArray","length"});
	using _method_282 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_282)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_283 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_283)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_284 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_284)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_285 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_285)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_286 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,std::size_t),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_286)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,std::size_t) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_287 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_287)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x"});
	using _method_288 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_288)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y"});
	using _method_289 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_289)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z"});
	using _method_290 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_290)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z","w"});
	using _method_291 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_291)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_292 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_292)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_293 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_293)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","color"});
	using _method_294 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Transform &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_294)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Transform &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","transform"});
	using _method_295 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_295)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","texture"});
	using _method_296 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_296)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType) , &sf::Shader::setParameter >::invoke")
		->args({"self","name",""});
	using _method_297 = das::das_call_member< unsigned int (sf::Shader::*)() const,&sf::Shader::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_297)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Shader::*)() const , &sf::Shader::getNativeHandle >::invoke")
		->args({"self"});
	addExtern< void (*)(const sf::Shader &,const sf::Shader *) , _dasSFML_static_58_bind >(*this,lib,"bind",SideEffects::worstDefault,"_dasSFML_static_58_bind")
		->args({"self","shader"});
	addExtern< bool (*)(const sf::Shader &) , _dasSFML_static_59_isAvailable >(*this,lib,"isAvailable",SideEffects::worstDefault,"_dasSFML_static_59_isAvailable")
		->args({"self"});
	addExtern< bool (*)(const sf::Shader &) , _dasSFML_static_60_isGeometryAvailable >(*this,lib,"isGeometryAvailable",SideEffects::worstDefault,"_dasSFML_static_60_isGeometryAvailable")
		->args({"self"});
	addCtorAndUsing<sf::Sprite>(*this,lib,"Sprite","sf::Sprite");
	addCtorAndUsing<sf::Sprite,const sf::Texture &>(*this,lib,"Sprite","sf::Sprite")
		->args({"texture"});
	addCtorAndUsing<sf::Sprite,const sf::Texture &,const sf::Rect<int> &>(*this,lib,"Sprite","sf::Sprite")
		->args({"texture","rectangle"});
	using _method_301 = das::das_call_member< void (sf::Sprite::*)(const sf::Texture &,bool),&sf::Sprite::setTexture >;
	addExtern<DAS_CALL_METHOD(_method_301)>(*this,lib,"setTexture",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Texture &,bool) , &sf::Sprite::setTexture >::invoke")
		->args({"self","texture","resetRect"})
		->arg_init(2,make_smart<ExprConstBool>(false));
	using _method_302 = das::das_call_member< void (sf::Sprite::*)(const sf::Rect<int> &),&sf::Sprite::setTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_302)>(*this,lib,"setTextureRect",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Rect<int> &) , &sf::Sprite::setTextureRect >::invoke")
		->args({"self","rectangle"});
	using _method_303 = das::das_call_member< void (sf::Sprite::*)(const sf::Color &),&sf::Sprite::setColor >;
	addExtern<DAS_CALL_METHOD(_method_303)>(*this,lib,"setColor",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Color &) , &sf::Sprite::setColor >::invoke")
		->args({"self","color"});
	using _method_304 = das::das_call_member< const sf::Texture * (sf::Sprite::*)() const,&sf::Sprite::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_304)>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture * (sf::Sprite::*)() const , &sf::Sprite::getTexture >::invoke")
		->args({"self"});
	using _method_305 = das::das_call_member< const sf::Rect<int> & (sf::Sprite::*)() const,&sf::Sprite::getTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_305), SimNode_ExtFuncCallRef>(*this,lib,"getTextureRect",SideEffects::worstDefault,"das_call_member< const sf::Rect<int> & (sf::Sprite::*)() const , &sf::Sprite::getTextureRect >::invoke")
		->args({"self"});
	using _method_306 = das::das_call_member< const sf::Color & (sf::Sprite::*)() const,&sf::Sprite::getColor >;
	addExtern<DAS_CALL_METHOD(_method_306), SimNode_ExtFuncCallRef>(*this,lib,"getColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Sprite::*)() const , &sf::Sprite::getColor >::invoke")
		->args({"self"});
	using _method_307 = das::das_call_member< sf::Rect<float> (sf::Sprite::*)() const,&sf::Sprite::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_307)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Sprite::*)() const , &sf::Sprite::getLocalBounds >::invoke")
		->args({"self"});
	using _method_308 = das::das_call_member< sf::Rect<float> (sf::Sprite::*)() const,&sf::Sprite::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_308)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Sprite::*)() const , &sf::Sprite::getGlobalBounds >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Text>(*this,lib,"Text","sf::Text");
	addCtorAndUsing<sf::Text,const sf::String &,const sf::Font &,unsigned int>(*this,lib,"Text","sf::Text")
		->args({"string","font","characterSize"})
		->arg_init(2,make_smart<ExprConstUInt>(0x1e));
	using _method_309 = das::das_call_member< void (sf::Text::*)(const sf::String &),&sf::Text::setString >;
	addExtern<DAS_CALL_METHOD(_method_309)>(*this,lib,"setString",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::String &) , &sf::Text::setString >::invoke")
		->args({"self","string"});
	using _method_310 = das::das_call_member< void (sf::Text::*)(const sf::Font &),&sf::Text::setFont >;
	addExtern<DAS_CALL_METHOD(_method_310)>(*this,lib,"setFont",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Font &) , &sf::Text::setFont >::invoke")
		->args({"self","font"});
	using _method_311 = das::das_call_member< void (sf::Text::*)(unsigned int),&sf::Text::setCharacterSize >;
	addExtern<DAS_CALL_METHOD(_method_311)>(*this,lib,"setCharacterSize",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(unsigned int) , &sf::Text::setCharacterSize >::invoke")
		->args({"self","size"});
	using _method_312 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_312)>(*this,lib,"setLineSpacing",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setLineSpacing >::invoke")
		->args({"self","spacingFactor"});
	using _method_313 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setLetterSpacing >;
	addExtern<DAS_CALL_METHOD(_method_313)>(*this,lib,"setLetterSpacing",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setLetterSpacing >::invoke")
		->args({"self","spacingFactor"});
	using _method_314 = das::das_call_member< void (sf::Text::*)(unsigned int),&sf::Text::setStyle >;
	addExtern<DAS_CALL_METHOD(_method_314)>(*this,lib,"setStyle",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(unsigned int) , &sf::Text::setStyle >::invoke")
		->args({"self","style"});
	using _method_315 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setColor >;
	addExtern<DAS_CALL_METHOD(_method_315)>(*this,lib,"setColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setColor >::invoke")
		->args({"self","color"});
	using _method_316 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setFillColor >;
	addExtern<DAS_CALL_METHOD(_method_316)>(*this,lib,"setFillColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setFillColor >::invoke")
		->args({"self","color"});
	using _method_317 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_317)>(*this,lib,"setOutlineColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setOutlineColor >::invoke")
		->args({"self","color"});
	using _method_318 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_318)>(*this,lib,"setOutlineThickness",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setOutlineThickness >::invoke")
		->args({"self","thickness"});
	using _method_319 = das::das_call_member< const sf::String & (sf::Text::*)() const,&sf::Text::getString >;
	addExtern<DAS_CALL_METHOD(_method_319), SimNode_ExtFuncCallRef>(*this,lib,"getString",SideEffects::worstDefault,"das_call_member< const sf::String & (sf::Text::*)() const , &sf::Text::getString >::invoke")
		->args({"self"});
	using _method_320 = das::das_call_member< const sf::Font * (sf::Text::*)() const,&sf::Text::getFont >;
	addExtern<DAS_CALL_METHOD(_method_320)>(*this,lib,"getFont",SideEffects::worstDefault,"das_call_member< const sf::Font * (sf::Text::*)() const , &sf::Text::getFont >::invoke")
		->args({"self"});
	using _method_321 = das::das_call_member< unsigned int (sf::Text::*)() const,&sf::Text::getCharacterSize >;
	addExtern<DAS_CALL_METHOD(_method_321)>(*this,lib,"getCharacterSize",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Text::*)() const , &sf::Text::getCharacterSize >::invoke")
		->args({"self"});
	using _method_322 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getLetterSpacing >;
	addExtern<DAS_CALL_METHOD(_method_322)>(*this,lib,"getLetterSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getLetterSpacing >::invoke")
		->args({"self"});
	using _method_323 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_323)>(*this,lib,"getLineSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getLineSpacing >::invoke")
		->args({"self"});
	using _method_324 = das::das_call_member< unsigned int (sf::Text::*)() const,&sf::Text::getStyle >;
	addExtern<DAS_CALL_METHOD(_method_324)>(*this,lib,"getStyle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Text::*)() const , &sf::Text::getStyle >::invoke")
		->args({"self"});
	using _method_325 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getColor >;
	addExtern<DAS_CALL_METHOD(_method_325), SimNode_ExtFuncCallRef>(*this,lib,"getColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getColor >::invoke")
		->args({"self"});
	using _method_326 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getFillColor >;
	addExtern<DAS_CALL_METHOD(_method_326), SimNode_ExtFuncCallRef>(*this,lib,"getFillColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getFillColor >::invoke")
		->args({"self"});
	using _method_327 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_327), SimNode_ExtFuncCallRef>(*this,lib,"getOutlineColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getOutlineColor >::invoke")
		->args({"self"});
	using _method_328 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_328)>(*this,lib,"getOutlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getOutlineThickness >::invoke")
		->args({"self"});
	using _method_329 = das::das_call_member< sf::Vector2<float> (sf::Text::*)(std::size_t) const,&sf::Text::findCharacterPos >;
	addExtern<DAS_CALL_METHOD(_method_329)>(*this,lib,"findCharacterPos",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Text::*)(std::size_t) const , &sf::Text::findCharacterPos >::invoke")
		->args({"self","index"});
	using _method_330 = das::das_call_member< sf::Rect<float> (sf::Text::*)() const,&sf::Text::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_330)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Text::*)() const , &sf::Text::getLocalBounds >::invoke")
		->args({"self"});
}
}

