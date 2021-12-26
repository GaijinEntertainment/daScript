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
	using _method_149 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlinePosition >;
	addExtern<DAS_CALL_METHOD(_method_149)>(*this,lib,"getUnderlinePosition",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlinePosition >::invoke")
		->args({"self","characterSize"});
	using _method_150 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_150)>(*this,lib,"getUnderlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlineThickness >::invoke")
		->args({"self","characterSize"});
	using _method_151 = das::das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const,&sf::Font::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_151), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const , &sf::Font::getTexture >::invoke")
		->args({"self","characterSize"});
	using _method_152 = das::das_call_member< void (sf::Font::*)(bool),&sf::Font::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_152)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::Font::*)(bool) , &sf::Font::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_153 = das::das_call_member< bool (sf::Font::*)() const,&sf::Font::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_153)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)() const , &sf::Font::isSmooth >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RectangleShape,const sf::Vector2<float> &>(*this,lib,"RectangleShape","sf::RectangleShape")
		->args({"size"});
	using _method_154 = das::das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &),&sf::RectangleShape::setSize >;
	addExtern<DAS_CALL_METHOD(_method_154)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &) , &sf::RectangleShape::setSize >::invoke")
		->args({"self","size"});
	using _method_155 = das::das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const,&sf::RectangleShape::getSize >;
	addExtern<DAS_CALL_METHOD(_method_155), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const , &sf::RectangleShape::getSize >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::View>(*this,lib,"View","sf::View");
	addCtorAndUsing<sf::View,const sf::Rect<float> &>(*this,lib,"View","sf::View")
		->args({"rectangle"});
	addCtorAndUsing<sf::View,const sf::Vector2<float> &,const sf::Vector2<float> &>(*this,lib,"View","sf::View")
		->args({"center","size"});
	using _method_156 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_156)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setCenter >::invoke")
		->args({"self","x","y"});
	using _method_157 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_157)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setCenter >::invoke")
		->args({"self","center"});
	using _method_158 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_158)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setSize >::invoke")
		->args({"self","width","height"});
	using _method_159 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_159)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setSize >::invoke")
		->args({"self","size"});
	using _method_160 = das::das_call_member< void (sf::View::*)(float),&sf::View::setRotation >;
	addExtern<DAS_CALL_METHOD(_method_160)>(*this,lib,"setRotation",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::setRotation >::invoke")
		->args({"self","angle"});
	using _method_161 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::setViewport >;
	addExtern<DAS_CALL_METHOD(_method_161)>(*this,lib,"setViewport",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::setViewport >::invoke")
		->args({"self","viewport"});
	using _method_162 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::reset >;
	addExtern<DAS_CALL_METHOD(_method_162)>(*this,lib,"reset",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::reset >::invoke")
		->args({"self","rectangle"});
	using _method_163 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getCenter >;
	addExtern<DAS_CALL_METHOD(_method_163), SimNode_ExtFuncCallRef>(*this,lib,"getCenter",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getCenter >::invoke")
		->args({"self"});
	using _method_164 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getSize >;
	addExtern<DAS_CALL_METHOD(_method_164), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getSize >::invoke")
		->args({"self"});
	using _method_165 = das::das_call_member< float (sf::View::*)() const,&sf::View::getRotation >;
	addExtern<DAS_CALL_METHOD(_method_165)>(*this,lib,"getRotation",SideEffects::worstDefault,"das_call_member< float (sf::View::*)() const , &sf::View::getRotation >::invoke")
		->args({"self"});
	using _method_166 = das::das_call_member< const sf::Rect<float> & (sf::View::*)() const,&sf::View::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_166), SimNode_ExtFuncCallRef>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< const sf::Rect<float> & (sf::View::*)() const , &sf::View::getViewport >::invoke")
		->args({"self"});
	using _method_167 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_167)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_168 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_168)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::move >::invoke")
		->args({"self","offset"});
	using _method_169 = das::das_call_member< void (sf::View::*)(float),&sf::View::rotate >;
	addExtern<DAS_CALL_METHOD(_method_169)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::rotate >::invoke")
		->args({"self","angle"});
	using _method_170 = das::das_call_member< void (sf::View::*)(float),&sf::View::zoom >;
	addExtern<DAS_CALL_METHOD(_method_170)>(*this,lib,"zoom",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::zoom >::invoke")
		->args({"self","factor"});
	using _method_171 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_171), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getTransform >::invoke")
		->args({"self"});
	using _method_172 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_172), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getInverseTransform >::invoke")
		->args({"self"});
	using _method_173 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Color &),&sf::RenderTarget::clear >;
	addExtern<DAS_CALL_METHOD(_method_173)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Color &) , &sf::RenderTarget::clear >::invoke")
		->args({"self","color"});
	using _method_174 = das::das_call_member< void (sf::RenderTarget::*)(const sf::View &),&sf::RenderTarget::setView >;
	addExtern<DAS_CALL_METHOD(_method_174)>(*this,lib,"setView",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::View &) , &sf::RenderTarget::setView >::invoke")
		->args({"self","view"});
	using _method_175 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getView >;
	addExtern<DAS_CALL_METHOD(_method_175), SimNode_ExtFuncCallRef>(*this,lib,"getView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getView >::invoke")
		->args({"self"});
	using _method_176 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getDefaultView >;
	addExtern<DAS_CALL_METHOD(_method_176), SimNode_ExtFuncCallRef>(*this,lib,"getDefaultView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getDefaultView >::invoke")
		->args({"self"});
	using _method_177 = das::das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const,&sf::RenderTarget::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_177)>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const , &sf::RenderTarget::getViewport >::invoke")
		->args({"self","view"});
	using _method_178 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_178)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point"});
	using _method_179 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_179)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point","view"});
	using _method_180 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_180)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point"});
	using _method_181 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_181)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point","view"});
	using _method_182 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_182)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","drawable","states"});
	using _method_183 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,unsigned long long,sf::PrimitiveType,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_183)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,unsigned long long,sf::PrimitiveType,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertices","vertexCount","type","states"});
	using _method_184 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_184)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","states"});
	using _method_185 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,unsigned long long,unsigned long long,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_185)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,unsigned long long,unsigned long long,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","firstVertex","vertexCount","states"});
	using _method_186 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::pushGLStates >;
	addExtern<DAS_CALL_METHOD(_method_186)>(*this,lib,"pushGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::pushGLStates >::invoke")
		->args({"self"});
	using _method_187 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::popGLStates >;
	addExtern<DAS_CALL_METHOD(_method_187)>(*this,lib,"popGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::popGLStates >::invoke")
		->args({"self"});
	using _method_188 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::resetGLStates >;
	addExtern<DAS_CALL_METHOD(_method_188)>(*this,lib,"resetGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::resetGLStates >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RenderTexture>(*this,lib,"RenderTexture","sf::RenderTexture");
	using _method_189 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_189)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","depthBuffer"});
	using _method_190 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_190)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","settings"});
	using _method_191 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_191)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_192 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_192)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isSmooth >::invoke")
		->args({"self"});
	using _method_193 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_193)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_194 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_194)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isRepeated >::invoke")
		->args({"self"});
	using _method_195 = das::das_call_member< bool (sf::RenderTexture::*)(),&sf::RenderTexture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_195)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() , &sf::RenderTexture::generateMipmap >::invoke")
		->args({"self"});
	using _method_196 = das::das_call_member< void (sf::RenderTexture::*)(),&sf::RenderTexture::display >;
	addExtern<DAS_CALL_METHOD(_method_196)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)() , &sf::RenderTexture::display >::invoke")
		->args({"self"});
	using _method_197 = das::das_call_member< const sf::Texture & (sf::RenderTexture::*)() const,&sf::RenderTexture::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_197), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::RenderTexture::*)() const , &sf::RenderTexture::getTexture >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RenderWindow>(*this,lib,"RenderWindow","sf::RenderWindow");
	addCtorAndUsing<sf::RenderWindow,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &>(*this,lib,"RenderWindow","sf::RenderWindow")
		->args({"mode","title","style","settings"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
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
	addCtorAndUsing<sf::Shader>(*this,lib,"Shader","sf::Shader");
	using _method_198 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_198)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","filename","type"});
	using _method_199 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_199)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","fragmentShaderFilename"});
	using _method_200 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_200)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","geometryShaderFilename","fragmentShaderFilename"});
	using _method_201 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_201)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","shader","type"});
	using _method_202 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_202)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","fragmentShader"});
	using _method_203 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_203)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","geometryShader","fragmentShader"});
	using _method_204 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_204)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","stream","type"});
	using _method_205 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_205)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","fragmentShaderStream"});
	using _method_206 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_206)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","geometryShaderStream","fragmentShaderStream"});
	using _method_207 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_207)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_208 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_208)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_209 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_209)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_210 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_210)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_211 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_211)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,int) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_212 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_212)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_213 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_213)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_214 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_214)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<int> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_215 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_215)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,bool) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_216 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_216)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_217 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_217)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_218 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_218)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<bool> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
	using _method_219 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_219)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_220 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_220)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","matrix"});
	using _method_221 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_221)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Texture &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","texture"});
	using _method_222 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_222)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::CurrentTextureType) , &sf::Shader::setUniform >::invoke")
		->args({"self","name",""});
	using _method_223 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_223)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const float *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","scalarArray","length"});
	using _method_224 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_224)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_225 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_225)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_226 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_226)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Vector4<float> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","vectorArray","length"});
	using _method_227 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_227)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<3, 3> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_228 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,unsigned long long),&sf::Shader::setUniformArray >;
	addExtern<DAS_CALL_METHOD(_method_228)>(*this,lib,"setUniformArray",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::priv::Matrix<4, 4> *,unsigned long long) , &sf::Shader::setUniformArray >::invoke")
		->args({"self","name","matrixArray","length"});
	using _method_229 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_229)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x"});
	using _method_230 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_230)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y"});
	using _method_231 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_231)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z"});
	using _method_232 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_232)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float,float,float,float) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","x","y","z","w"});
	using _method_233 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_233)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_234 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_234)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector3<float> &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","vector"});
	using _method_235 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &),&sf::Shader::setParameter >;
	addExtern<DAS_CALL_METHOD(_method_235)>(*this,lib,"setParameter",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Color &) , &sf::Shader::setParameter >::invoke")
		->args({"self","name","color"});
}
}

