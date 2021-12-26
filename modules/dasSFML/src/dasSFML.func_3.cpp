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
void Module_dasSFML::initFunctions_3() {
	using _method_197 = das::das_call_member< void (sf::RenderTexture::*)(),&sf::RenderTexture::display >;
	addExtern<DAS_CALL_METHOD(_method_197)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)() , &sf::RenderTexture::display >::invoke")
		->args({"self"});
	using _method_198 = das::das_call_member< const sf::Texture & (sf::RenderTexture::*)() const,&sf::RenderTexture::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_198), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::RenderTexture::*)() const , &sf::RenderTexture::getTexture >::invoke")
		->args({"self"});
	addExtern< void (*)(const sf::Transform &,sf::priv::Matrix<3, 3> &) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","dest"});
	addExtern< void (*)(const sf::Transform &,sf::priv::Matrix<4, 4> &) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","dest"});
	addExtern< void (*)(const float *,unsigned long long,float *) , sf::priv::copyMatrix >(*this,lib,"copyMatrix",SideEffects::worstDefault,"sf::priv::copyMatrix")
		->args({"source","elements","dest"});
	addExtern< void (*)(const sf::Color &,sf::priv::Vector4<float> &) , sf::priv::copyVector >(*this,lib,"copyVector",SideEffects::worstDefault,"sf::priv::copyVector")
		->args({"source","dest"});
	addExtern< void (*)(const sf::Color &,sf::priv::Vector4<int> &) , sf::priv::copyVector >(*this,lib,"copyVector",SideEffects::worstDefault,"sf::priv::copyVector")
		->args({"source","dest"});
	using _method_199 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_199)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","filename","type"});
	using _method_200 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_200)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","fragmentShaderFilename"});
	using _method_201 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_201)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","geometryShaderFilename","fragmentShaderFilename"});
	using _method_202 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_202)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","shader","type"});
	using _method_203 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_203)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","fragmentShader"});
	using _method_204 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_204)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","geometryShader","fragmentShader"});
	using _method_205 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_205)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","stream","type"});
	using _method_206 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_206)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","fragmentShaderStream"});
	using _method_207 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_207)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","geometryShaderStream","fragmentShaderStream"});
	using _method_208 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_208)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_209 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_209)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_210 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_210)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_211 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_211)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_212 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_212)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_213 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_213)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_214 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_214)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_215 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_215)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_216 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_216)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_217 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_217)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_218 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_218)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_219 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_219)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_220 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_220)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_221 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_221)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_222 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_222)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","texture"});
	using _method_223 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_223)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType) , &sf::Shader::setUniform >::invoke")
		->args({"self","name",""});
	using _method_224 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_224)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","scalarArray","length"});
	using _method_225 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_225)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_226 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_226)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_227 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_227)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_228 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_228)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_229 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_229)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_230 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_230)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x"});
	using _method_231 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_231)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y"});
	using _method_232 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_232)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z"});
	using _method_233 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_233)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z","w"});
	using _method_234 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_234)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_235 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_235)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_236 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_236)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","color"});
	using _method_237 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Transform &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_237)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Transform &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","transform"});
	using _method_238 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_238)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","texture"});
	using _method_239 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_239)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType) , &sf::Shader::setParameter >::invoke")
		->args({"self","name",""});
	using _method_240 = das::das_call_member< unsigned int (sf::Shader::*)() const,&sf::Shader::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_240)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Shader::*)() const , &sf::Shader::getNativeHandle >::invoke")
		->args({"self"});
	using _method_241 = das::das_call_member< void (sf::Sprite::*)(const sf::Texture &,bool),&sf::Sprite::setTexture >;
	addExtern<DAS_CALL_METHOD(_method_241)>(*this,lib,"setTexture",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Texture &,bool) , &sf::Sprite::setTexture >::invoke")
		->args({"self","texture","resetRect"})
		->arg_init(2,make_smart<ExprConstBool>(false));
	using _method_242 = das::das_call_member< void (sf::Sprite::*)(const sf::Rect<int> &),&sf::Sprite::setTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_242)>(*this,lib,"setTextureRect",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Rect<int> &) , &sf::Sprite::setTextureRect >::invoke")
		->args({"self","rectangle"});
	using _method_243 = das::das_call_member< void (sf::Sprite::*)(const sf::Color &),&sf::Sprite::setColor >;
	addExtern<DAS_CALL_METHOD(_method_243)>(*this,lib,"setColor",SideEffects::worstDefault,"das_call_member< void (sf::Sprite::*)(const sf::Color &) , &sf::Sprite::setColor >::invoke")
		->args({"self","color"});
	using _method_244 = das::das_call_member< const sf::Texture * (sf::Sprite::*)() const,&sf::Sprite::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_244)>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture * (sf::Sprite::*)() const , &sf::Sprite::getTexture >::invoke")
		->args({"self"});
	using _method_245 = das::das_call_member< const sf::Rect<int> & (sf::Sprite::*)() const,&sf::Sprite::getTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_245), SimNode_ExtFuncCallRef>(*this,lib,"getTextureRect",SideEffects::worstDefault,"das_call_member< const sf::Rect<int> & (sf::Sprite::*)() const , &sf::Sprite::getTextureRect >::invoke")
		->args({"self"});
	using _method_246 = das::das_call_member< const sf::Color & (sf::Sprite::*)() const,&sf::Sprite::getColor >;
	addExtern<DAS_CALL_METHOD(_method_246), SimNode_ExtFuncCallRef>(*this,lib,"getColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Sprite::*)() const , &sf::Sprite::getColor >::invoke")
		->args({"self"});
	using _method_247 = das::das_call_member< sf::Rect<float> (sf::Sprite::*)() const,&sf::Sprite::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_247)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Sprite::*)() const , &sf::Sprite::getLocalBounds >::invoke")
		->args({"self"});
	using _method_248 = das::das_call_member< sf::Rect<float> (sf::Sprite::*)() const,&sf::Sprite::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_248)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Sprite::*)() const , &sf::Sprite::getGlobalBounds >::invoke")
		->args({"self"});
	using _method_249 = das::das_call_member< void (sf::Text::*)(const sf::String &),&sf::Text::setString >;
	addExtern<DAS_CALL_METHOD(_method_249)>(*this,lib,"setString",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::String &) , &sf::Text::setString >::invoke")
		->args({"self","string"});
	using _method_250 = das::das_call_member< void (sf::Text::*)(const sf::Font &),&sf::Text::setFont >;
	addExtern<DAS_CALL_METHOD(_method_250)>(*this,lib,"setFont",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Font &) , &sf::Text::setFont >::invoke")
		->args({"self","font"});
	using _method_251 = das::das_call_member< void (sf::Text::*)(unsigned int),&sf::Text::setCharacterSize >;
	addExtern<DAS_CALL_METHOD(_method_251)>(*this,lib,"setCharacterSize",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(unsigned int) , &sf::Text::setCharacterSize >::invoke")
		->args({"self","size"});
	using _method_252 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_252)>(*this,lib,"setLineSpacing",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setLineSpacing >::invoke")
		->args({"self","spacingFactor"});
	using _method_253 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setLetterSpacing >;
	addExtern<DAS_CALL_METHOD(_method_253)>(*this,lib,"setLetterSpacing",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setLetterSpacing >::invoke")
		->args({"self","spacingFactor"});
	using _method_254 = das::das_call_member< void (sf::Text::*)(unsigned int),&sf::Text::setStyle >;
	addExtern<DAS_CALL_METHOD(_method_254)>(*this,lib,"setStyle",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(unsigned int) , &sf::Text::setStyle >::invoke")
		->args({"self","style"});
	using _method_255 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setColor >;
	addExtern<DAS_CALL_METHOD(_method_255)>(*this,lib,"setColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setColor >::invoke")
		->args({"self","color"});
	using _method_256 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setFillColor >;
	addExtern<DAS_CALL_METHOD(_method_256)>(*this,lib,"setFillColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setFillColor >::invoke")
		->args({"self","color"});
	using _method_257 = das::das_call_member< void (sf::Text::*)(const sf::Color &),&sf::Text::setOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_257)>(*this,lib,"setOutlineColor",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(const sf::Color &) , &sf::Text::setOutlineColor >::invoke")
		->args({"self","color"});
	using _method_258 = das::das_call_member< void (sf::Text::*)(float),&sf::Text::setOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_258)>(*this,lib,"setOutlineThickness",SideEffects::worstDefault,"das_call_member< void (sf::Text::*)(float) , &sf::Text::setOutlineThickness >::invoke")
		->args({"self","thickness"});
	using _method_259 = das::das_call_member< const sf::String & (sf::Text::*)() const,&sf::Text::getString >;
	addExtern<DAS_CALL_METHOD(_method_259), SimNode_ExtFuncCallRef>(*this,lib,"getString",SideEffects::worstDefault,"das_call_member< const sf::String & (sf::Text::*)() const , &sf::Text::getString >::invoke")
		->args({"self"});
	using _method_260 = das::das_call_member< const sf::Font * (sf::Text::*)() const,&sf::Text::getFont >;
	addExtern<DAS_CALL_METHOD(_method_260)>(*this,lib,"getFont",SideEffects::worstDefault,"das_call_member< const sf::Font * (sf::Text::*)() const , &sf::Text::getFont >::invoke")
		->args({"self"});
	using _method_261 = das::das_call_member< unsigned int (sf::Text::*)() const,&sf::Text::getCharacterSize >;
	addExtern<DAS_CALL_METHOD(_method_261)>(*this,lib,"getCharacterSize",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Text::*)() const , &sf::Text::getCharacterSize >::invoke")
		->args({"self"});
	using _method_262 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getLetterSpacing >;
	addExtern<DAS_CALL_METHOD(_method_262)>(*this,lib,"getLetterSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getLetterSpacing >::invoke")
		->args({"self"});
	using _method_263 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_263)>(*this,lib,"getLineSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getLineSpacing >::invoke")
		->args({"self"});
	using _method_264 = das::das_call_member< unsigned int (sf::Text::*)() const,&sf::Text::getStyle >;
	addExtern<DAS_CALL_METHOD(_method_264)>(*this,lib,"getStyle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Text::*)() const , &sf::Text::getStyle >::invoke")
		->args({"self"});
	using _method_265 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getColor >;
	addExtern<DAS_CALL_METHOD(_method_265), SimNode_ExtFuncCallRef>(*this,lib,"getColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getColor >::invoke")
		->args({"self"});
	using _method_266 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getFillColor >;
	addExtern<DAS_CALL_METHOD(_method_266), SimNode_ExtFuncCallRef>(*this,lib,"getFillColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getFillColor >::invoke")
		->args({"self"});
	using _method_267 = das::das_call_member< const sf::Color & (sf::Text::*)() const,&sf::Text::getOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_267), SimNode_ExtFuncCallRef>(*this,lib,"getOutlineColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Text::*)() const , &sf::Text::getOutlineColor >::invoke")
		->args({"self"});
	using _method_268 = das::das_call_member< float (sf::Text::*)() const,&sf::Text::getOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_268)>(*this,lib,"getOutlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Text::*)() const , &sf::Text::getOutlineThickness >::invoke")
		->args({"self"});
	using _method_269 = das::das_call_member< sf::Vector2<float> (sf::Text::*)(unsigned long long) const,&sf::Text::findCharacterPos >;
	addExtern<DAS_CALL_METHOD(_method_269)>(*this,lib,"findCharacterPos",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Text::*)(unsigned long long) const , &sf::Text::findCharacterPos >::invoke")
		->args({"self","index"});
	using _method_270 = das::das_call_member< sf::Rect<float> (sf::Text::*)() const,&sf::Text::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_270)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Text::*)() const , &sf::Text::getLocalBounds >::invoke")
		->args({"self"});
	using _method_271 = das::das_call_member< sf::Rect<float> (sf::Text::*)() const,&sf::Text::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_271)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Text::*)() const , &sf::Text::getGlobalBounds >::invoke")
		->args({"self"});
	using _method_272 = das::das_call_member< bool (sf::VertexBuffer::*)(unsigned long long),&sf::VertexBuffer::create >;
	addExtern<DAS_CALL_METHOD(_method_272)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(unsigned long long) , &sf::VertexBuffer::create >::invoke")
		->args({"self","vertexCount"});
	using _method_273 = das::das_call_member< unsigned long long (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getVertexCount >;
	addExtern<DAS_CALL_METHOD(_method_273)>(*this,lib,"getVertexCount",SideEffects::worstDefault,"das_call_member< unsigned long long (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getVertexCount >::invoke")
		->args({"self"});
	using _method_274 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_274)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertices"});
	using _method_275 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *,unsigned long long,unsigned int),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_275)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *,unsigned long long,unsigned int) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertices","vertexCount","offset"});
	using _method_276 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::VertexBuffer &),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_276)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::VertexBuffer &) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertexBuffer"});
	using _method_277 = das::das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer &),&sf::VertexBuffer::swap >;
	addExtern<DAS_CALL_METHOD(_method_277)>(*this,lib,"swap",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer &) , &sf::VertexBuffer::swap >::invoke")
		->args({"self","right"});
	using _method_278 = das::das_call_member< unsigned int (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_278)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getNativeHandle >::invoke")
		->args({"self"});
	using _method_279 = das::das_call_member< void (sf::VertexBuffer::*)(sf::PrimitiveType),&sf::VertexBuffer::setPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_279)>(*this,lib,"setPrimitiveType",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::PrimitiveType) , &sf::VertexBuffer::setPrimitiveType >::invoke")
		->args({"self","type"});
	using _method_280 = das::das_call_member< sf::PrimitiveType (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_280)>(*this,lib,"getPrimitiveType",SideEffects::worstDefault,"das_call_member< sf::PrimitiveType (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getPrimitiveType >::invoke")
		->args({"self"});
	using _method_281 = das::das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer::Usage),&sf::VertexBuffer::setUsage >;
	addExtern<DAS_CALL_METHOD(_method_281)>(*this,lib,"setUsage",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer::Usage) , &sf::VertexBuffer::setUsage >::invoke")
		->args({"self","usage"});
	using _method_282 = das::das_call_member< sf::VertexBuffer::Usage (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getUsage >;
	addExtern<DAS_CALL_METHOD(_method_282)>(*this,lib,"getUsage",SideEffects::worstDefault,"das_call_member< sf::VertexBuffer::Usage (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getUsage >::invoke")
		->args({"self"});
}
}

