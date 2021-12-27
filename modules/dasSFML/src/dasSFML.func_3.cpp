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
void Module_dasSFML::initFunctions_3() {
	using _method_151 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setSrgb >;
	addExtern<DAS_CALL_METHOD(_method_151)>(*this,lib,"setSrgb",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setSrgb >::invoke")
		->args({"self","sRgb"});
	using _method_152 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isSrgb >;
	addExtern<DAS_CALL_METHOD(_method_152)>(*this,lib,"isSrgb",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isSrgb >::invoke")
		->args({"self"});
	using _method_153 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_153)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_154 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_154)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isRepeated >::invoke")
		->args({"self"});
	using _method_155 = das::das_call_member< bool (sf::Texture::*)(),&sf::Texture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_155)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() , &sf::Texture::generateMipmap >::invoke")
		->args({"self"});
	using _method_156 = das::das_call_member< void (sf::Texture::*)(sf::Texture &),&sf::Texture::swap >;
	addExtern<DAS_CALL_METHOD(_method_156)>(*this,lib,"swap",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(sf::Texture &) , &sf::Texture::swap >::invoke")
		->args({"self","right"});
	using _method_157 = das::das_call_member< unsigned int (sf::Texture::*)() const,&sf::Texture::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_157)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Texture::*)() const , &sf::Texture::getNativeHandle >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Font>(*this,lib,"Font","sf::Font");
	addCtorAndUsing<sf::Font,const sf::Font &>(*this,lib,"Font","sf::Font")
		->args({"copy"});
	using _method_158 = das::das_call_member< bool (sf::Font::*)(const std::basic_string<char> &),&sf::Font::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_158)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const std::basic_string<char> &) , &sf::Font::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_159 = das::das_call_member< bool (sf::Font::*)(const void *,std::size_t),&sf::Font::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_159)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const void *,std::size_t) , &sf::Font::loadFromMemory >::invoke")
		->args({"self","data","sizeInBytes"});
	using _method_160 = das::das_call_member< bool (sf::Font::*)(sf::InputStream &),&sf::Font::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_160)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(sf::InputStream &) , &sf::Font::loadFromStream >::invoke")
		->args({"self","stream"});
	using _method_161 = das::das_call_member< const sf::Font::Info & (sf::Font::*)() const,&sf::Font::getInfo >;
	addExtern<DAS_CALL_METHOD(_method_161), SimNode_ExtFuncCallRef>(*this,lib,"getInfo",SideEffects::worstDefault,"das_call_member< const sf::Font::Info & (sf::Font::*)() const , &sf::Font::getInfo >::invoke")
		->args({"self"});
	using _method_162 = das::das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const,&sf::Font::getGlyph >;
	addExtern<DAS_CALL_METHOD(_method_162), SimNode_ExtFuncCallRef>(*this,lib,"getGlyph",SideEffects::worstDefault,"das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const , &sf::Font::getGlyph >::invoke")
		->args({"self","codePoint","characterSize","bold","outlineThickness"})
		->arg_init(4,make_smart<ExprConstFloat>(0.00000000000000000));
	using _method_163 = das::das_call_member< bool (sf::Font::*)(unsigned int) const,&sf::Font::hasGlyph >;
	addExtern<DAS_CALL_METHOD(_method_163)>(*this,lib,"hasGlyph",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(unsigned int) const , &sf::Font::hasGlyph >::invoke")
		->args({"self","codePoint"});
	using _method_164 = das::das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const,&sf::Font::getKerning >;
	addExtern<DAS_CALL_METHOD(_method_164)>(*this,lib,"getKerning",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const , &sf::Font::getKerning >::invoke")
		->args({"self","first","second","characterSize","bold"})
		->arg_init(4,make_smart<ExprConstBool>(false));
	using _method_165 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_165)>(*this,lib,"getLineSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getLineSpacing >::invoke")
		->args({"self","characterSize"});
	using _method_166 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlinePosition >;
	addExtern<DAS_CALL_METHOD(_method_166)>(*this,lib,"getUnderlinePosition",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlinePosition >::invoke")
		->args({"self","characterSize"});
	using _method_167 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_167)>(*this,lib,"getUnderlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlineThickness >::invoke")
		->args({"self","characterSize"});
	using _method_168 = das::das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const,&sf::Font::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_168), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const , &sf::Font::getTexture >::invoke")
		->args({"self","characterSize"});
	using _method_169 = das::das_call_member< void (sf::Font::*)(bool),&sf::Font::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_169)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::Font::*)(bool) , &sf::Font::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_170 = das::das_call_member< bool (sf::Font::*)() const,&sf::Font::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_170)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)() const , &sf::Font::isSmooth >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RectangleShape,const sf::Vector2<float> &>(*this,lib,"RectangleShape","sf::RectangleShape")
		->args({"size"});
	using _method_171 = das::das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &),&sf::RectangleShape::setSize >;
	addExtern<DAS_CALL_METHOD(_method_171)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &) , &sf::RectangleShape::setSize >::invoke")
		->args({"self","size"});
	using _method_172 = das::das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const,&sf::RectangleShape::getSize >;
	addExtern<DAS_CALL_METHOD(_method_172), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const , &sf::RectangleShape::getSize >::invoke")
		->args({"self"});
	addExtern< std::size_t (*)(const sf::RectangleShape &) , _dasSFML_virtual_17_getPointCount >(*this,lib,"getPointCount",SideEffects::worstDefault,"_dasSFML_virtual_17_getPointCount")
		->args({"self"});
	addExtern< sf::Vector2<float> (*)(const sf::RectangleShape &,std::size_t) , _dasSFML_virtual_18_getPoint >(*this,lib,"getPoint",SideEffects::worstDefault,"_dasSFML_virtual_18_getPoint")
		->args({"self","index"});
	addCtorAndUsing<sf::View>(*this,lib,"View","sf::View");
	addCtorAndUsing<sf::View,const sf::Rect<float> &>(*this,lib,"View","sf::View")
		->args({"rectangle"});
	addCtorAndUsing<sf::View,const sf::Vector2<float> &,const sf::Vector2<float> &>(*this,lib,"View","sf::View")
		->args({"center","size"});
	using _method_175 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_175)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setCenter >::invoke")
		->args({"self","x","y"});
	using _method_176 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_176)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setCenter >::invoke")
		->args({"self","center"});
	using _method_177 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_177)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setSize >::invoke")
		->args({"self","width","height"});
	using _method_178 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_178)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setSize >::invoke")
		->args({"self","size"});
	using _method_179 = das::das_call_member< void (sf::View::*)(float),&sf::View::setRotation >;
	addExtern<DAS_CALL_METHOD(_method_179)>(*this,lib,"setRotation",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::setRotation >::invoke")
		->args({"self","angle"});
	using _method_180 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::setViewport >;
	addExtern<DAS_CALL_METHOD(_method_180)>(*this,lib,"setViewport",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::setViewport >::invoke")
		->args({"self","viewport"});
	using _method_181 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::reset >;
	addExtern<DAS_CALL_METHOD(_method_181)>(*this,lib,"reset",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::reset >::invoke")
		->args({"self","rectangle"});
	using _method_182 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getCenter >;
	addExtern<DAS_CALL_METHOD(_method_182), SimNode_ExtFuncCallRef>(*this,lib,"getCenter",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getCenter >::invoke")
		->args({"self"});
	using _method_183 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getSize >;
	addExtern<DAS_CALL_METHOD(_method_183), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getSize >::invoke")
		->args({"self"});
	using _method_184 = das::das_call_member< float (sf::View::*)() const,&sf::View::getRotation >;
	addExtern<DAS_CALL_METHOD(_method_184)>(*this,lib,"getRotation",SideEffects::worstDefault,"das_call_member< float (sf::View::*)() const , &sf::View::getRotation >::invoke")
		->args({"self"});
	using _method_185 = das::das_call_member< const sf::Rect<float> & (sf::View::*)() const,&sf::View::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_185), SimNode_ExtFuncCallRef>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< const sf::Rect<float> & (sf::View::*)() const , &sf::View::getViewport >::invoke")
		->args({"self"});
	using _method_186 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_186)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_187 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_187)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::move >::invoke")
		->args({"self","offset"});
	using _method_188 = das::das_call_member< void (sf::View::*)(float),&sf::View::rotate >;
	addExtern<DAS_CALL_METHOD(_method_188)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::rotate >::invoke")
		->args({"self","angle"});
	using _method_189 = das::das_call_member< void (sf::View::*)(float),&sf::View::zoom >;
	addExtern<DAS_CALL_METHOD(_method_189)>(*this,lib,"zoom",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::zoom >::invoke")
		->args({"self","factor"});
	using _method_190 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_190), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getTransform >::invoke")
		->args({"self"});
	using _method_191 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_191), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getInverseTransform >::invoke")
		->args({"self"});
	using _method_192 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Color &),&sf::RenderTarget::clear >;
	addExtern<DAS_CALL_METHOD(_method_192)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Color &) , &sf::RenderTarget::clear >::invoke")
		->args({"self","color"});
	using _method_193 = das::das_call_member< void (sf::RenderTarget::*)(const sf::View &),&sf::RenderTarget::setView >;
	addExtern<DAS_CALL_METHOD(_method_193)>(*this,lib,"setView",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::View &) , &sf::RenderTarget::setView >::invoke")
		->args({"self","view"});
	using _method_194 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getView >;
	addExtern<DAS_CALL_METHOD(_method_194), SimNode_ExtFuncCallRef>(*this,lib,"getView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getView >::invoke")
		->args({"self"});
	using _method_195 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getDefaultView >;
	addExtern<DAS_CALL_METHOD(_method_195), SimNode_ExtFuncCallRef>(*this,lib,"getDefaultView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getDefaultView >::invoke")
		->args({"self"});
	using _method_196 = das::das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const,&sf::RenderTarget::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_196)>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const , &sf::RenderTarget::getViewport >::invoke")
		->args({"self","view"});
	using _method_197 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_197)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point"});
	using _method_198 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_198)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point","view"});
	using _method_199 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_199)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point"});
	using _method_200 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_200)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point","view"});
	using _method_201 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_201)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","drawable","states"});
	using _method_202 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,std::size_t,sf::PrimitiveType,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_202)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,std::size_t,sf::PrimitiveType,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertices","vertexCount","type","states"});
	using _method_203 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_203)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","states"});
	using _method_204 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,std::size_t,std::size_t,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_204)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,std::size_t,std::size_t,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","firstVertex","vertexCount","states"});
	addExtern< bool (*)(const sf::RenderTarget &) , _dasSFML_virtual_19_isSrgb >(*this,lib,"isSrgb",SideEffects::worstDefault,"_dasSFML_virtual_19_isSrgb")
		->args({"self"});
	addExtern< bool (*)(sf::RenderTarget &,bool) , _dasSFML_virtual_20_setActive >(*this,lib,"setActive",SideEffects::worstDefault,"_dasSFML_virtual_20_setActive")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_207 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::pushGLStates >;
	addExtern<DAS_CALL_METHOD(_method_207)>(*this,lib,"pushGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::pushGLStates >::invoke")
		->args({"self"});
	using _method_208 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::popGLStates >;
	addExtern<DAS_CALL_METHOD(_method_208)>(*this,lib,"popGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::popGLStates >::invoke")
		->args({"self"});
	using _method_209 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::resetGLStates >;
	addExtern<DAS_CALL_METHOD(_method_209)>(*this,lib,"resetGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::resetGLStates >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RenderTexture>(*this,lib,"RenderTexture","sf::RenderTexture");
	using _method_210 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_210)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","depthBuffer"});
	using _method_211 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_211)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","settings"});
	using _method_212 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_212)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_213 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_213)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isSmooth >::invoke")
		->args({"self"});
	using _method_214 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_214)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_215 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_215)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isRepeated >::invoke")
		->args({"self"});
	using _method_216 = das::das_call_member< bool (sf::RenderTexture::*)(),&sf::RenderTexture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_216)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() , &sf::RenderTexture::generateMipmap >::invoke")
		->args({"self"});
	addExtern< bool (*)(sf::RenderTexture &,bool) , _dasSFML_virtual_21_setActive >(*this,lib,"setActive",SideEffects::worstDefault,"_dasSFML_virtual_21_setActive")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_218 = das::das_call_member< void (sf::RenderTexture::*)(),&sf::RenderTexture::display >;
	addExtern<DAS_CALL_METHOD(_method_218)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)() , &sf::RenderTexture::display >::invoke")
		->args({"self"});
	addExtern< sf::Vector2<unsigned int> (*)(const sf::RenderTexture &) , _dasSFML_virtual_22_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_22_getSize")
		->args({"self"});
	addExtern< bool (*)(const sf::RenderTexture &) , _dasSFML_virtual_23_isSrgb >(*this,lib,"isSrgb",SideEffects::worstDefault,"_dasSFML_virtual_23_isSrgb")
		->args({"self"});
	using _method_221 = das::das_call_member< const sf::Texture & (sf::RenderTexture::*)() const,&sf::RenderTexture::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_221), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::RenderTexture::*)() const , &sf::RenderTexture::getTexture >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::RenderWindow>(*this,lib,"RenderWindow","sf::RenderWindow");
	addCtorAndUsing<sf::RenderWindow,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &>(*this,lib,"RenderWindow","sf::RenderWindow")
		->args({"mode","title","style","settings"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< sf::Vector2<unsigned int> (*)(const sf::RenderWindow &) , _dasSFML_virtual_24_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_24_getSize")
		->args({"self"});
	addExtern< bool (*)(const sf::RenderWindow &) , _dasSFML_virtual_25_isSrgb >(*this,lib,"isSrgb",SideEffects::worstDefault,"_dasSFML_virtual_25_isSrgb")
		->args({"self"});
	addExtern< bool (*)(sf::RenderWindow &,bool) , _dasSFML_virtual_26_setActive >(*this,lib,"setActive",SideEffects::worstDefault,"_dasSFML_virtual_26_setActive")
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
	using _method_225 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_225)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","filename","type"});
	using _method_226 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_226)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","fragmentShaderFilename"});
	using _method_227 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_227)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromFile >::invoke")
		->args({"self","vertexShaderFilename","geometryShaderFilename","fragmentShaderFilename"});
	using _method_228 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_228)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,sf::Shader::Type) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","shader","type"});
	using _method_229 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_229)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","fragmentShader"});
	using _method_230 = das::das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &),&sf::Shader::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_230)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(const std::basic_string<char> &,const std::basic_string<char> &,const std::basic_string<char> &) , &sf::Shader::loadFromMemory >::invoke")
		->args({"self","vertexShader","geometryShader","fragmentShader"});
	using _method_231 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_231)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::Shader::Type) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","stream","type"});
	using _method_232 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_232)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","fragmentShaderStream"});
	using _method_233 = das::das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &),&sf::Shader::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_233)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Shader::*)(sf::InputStream &,sf::InputStream &,sf::InputStream &) , &sf::Shader::loadFromStream >::invoke")
		->args({"self","vertexShaderStream","geometryShaderStream","fragmentShaderStream"});
	using _method_234 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_234)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,float) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","x"});
	using _method_235 = das::das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &),&sf::Shader::setUniform >;
	addExtern<DAS_CALL_METHOD(_method_235)>(*this,lib,"setUniform",SideEffects::worstDefault,"das_call_member< void (sf::Shader::*)(const std::basic_string<char> &,const sf::Vector2<float> &) , &sf::Shader::setUniform >::invoke")
		->args({"self","name","vector"});
}
}

