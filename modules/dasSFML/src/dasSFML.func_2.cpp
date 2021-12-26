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
void Module_dasSFML::initFunctions_2() {
	using _method_97 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_97)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getLocalBounds >::invoke")
		->args({"self"});
	using _method_98 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_98)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getGlobalBounds >::invoke")
		->args({"self"});
	using _method_99 = das::das_call_member< void (sf::CircleShape::*)(float),&sf::CircleShape::setRadius >;
	addExtern<DAS_CALL_METHOD(_method_99)>(*this,lib,"setRadius",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(float) , &sf::CircleShape::setRadius >::invoke")
		->args({"self","radius"});
	using _method_100 = das::das_call_member< float (sf::CircleShape::*)() const,&sf::CircleShape::getRadius >;
	addExtern<DAS_CALL_METHOD(_method_100)>(*this,lib,"getRadius",SideEffects::worstDefault,"das_call_member< float (sf::CircleShape::*)() const , &sf::CircleShape::getRadius >::invoke")
		->args({"self"});
	using _method_101 = das::das_call_member< void (sf::CircleShape::*)(unsigned long long),&sf::CircleShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_101)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(unsigned long long) , &sf::CircleShape::setPointCount >::invoke")
		->args({"self","count"});
	using _method_102 = das::das_call_member< void (sf::ConvexShape::*)(unsigned long long),&sf::ConvexShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_102)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(unsigned long long) , &sf::ConvexShape::setPointCount >::invoke")
		->args({"self","count"});
	using _method_103 = das::das_call_member< void (sf::ConvexShape::*)(unsigned long long,const sf::Vector2<float> &),&sf::ConvexShape::setPoint >;
	addExtern<DAS_CALL_METHOD(_method_103)>(*this,lib,"setPoint",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(unsigned long long,const sf::Vector2<float> &) , &sf::ConvexShape::setPoint >::invoke")
		->args({"self","index","point"});
	using _method_104 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_104)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &) , &sf::Image::create >::invoke")
		->args({"self","width","height","color"});
	using _method_105 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_105)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *) , &sf::Image::create >::invoke")
		->args({"self","width","height","pixels"});
	using _method_106 = das::das_call_member< bool (sf::Image::*)(const std::basic_string<char> &),&sf::Image::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_106)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) , &sf::Image::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_107 = das::das_call_member< bool (sf::Image::*)(const void *,unsigned long long),&sf::Image::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_107)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const void *,unsigned long long) , &sf::Image::loadFromMemory >::invoke")
		->args({"self","data","size"});
	using _method_108 = das::das_call_member< bool (sf::Image::*)(sf::InputStream &),&sf::Image::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_108)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(sf::InputStream &) , &sf::Image::loadFromStream >::invoke")
		->args({"self","stream"});
	using _method_109 = das::das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) const,&sf::Image::saveToFile >;
	addExtern<DAS_CALL_METHOD(_method_109)>(*this,lib,"saveToFile",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) const , &sf::Image::saveToFile >::invoke")
		->args({"self","filename"});
	using _method_110 = das::das_call_member< bool (sf::Image::*)(std::vector<unsigned char> &,const std::basic_string<char> &) const,&sf::Image::saveToMemory >;
	addExtern<DAS_CALL_METHOD(_method_110)>(*this,lib,"saveToMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(std::vector<unsigned char> &,const std::basic_string<char> &) const , &sf::Image::saveToMemory >::invoke")
		->args({"self","output","format"});
	using _method_111 = das::das_call_member< sf::Vector2<unsigned int> (sf::Image::*)() const,&sf::Image::getSize >;
	addExtern<DAS_CALL_METHOD(_method_111)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::Image::*)() const , &sf::Image::getSize >::invoke")
		->args({"self"});
	using _method_112 = das::das_call_member< void (sf::Image::*)(const sf::Color &,unsigned char),&sf::Image::createMaskFromColor >;
	addExtern<DAS_CALL_METHOD(_method_112)>(*this,lib,"createMaskFromColor",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(const sf::Color &,unsigned char) , &sf::Image::createMaskFromColor >::invoke")
		->args({"self","color","alpha"});
	using _method_113 = das::das_call_member< void (sf::Image::*)(const sf::Image &,unsigned int,unsigned int,const sf::Rect<int> &,bool),&sf::Image::copy >;
	addExtern<DAS_CALL_METHOD(_method_113)>(*this,lib,"copy",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(const sf::Image &,unsigned int,unsigned int,const sf::Rect<int> &,bool) , &sf::Image::copy >::invoke")
		->args({"self","source","destX","destY","sourceRect","applyAlpha"})
		->arg_init(5,make_smart<ExprConstBool>(false));
	using _method_114 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &),&sf::Image::setPixel >;
	addExtern<DAS_CALL_METHOD(_method_114)>(*this,lib,"setPixel",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &) , &sf::Image::setPixel >::invoke")
		->args({"self","x","y","color"});
	using _method_115 = das::das_call_member< sf::Color (sf::Image::*)(unsigned int,unsigned int) const,&sf::Image::getPixel >;
	addExtern<DAS_CALL_METHOD(_method_115),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getPixel",SideEffects::worstDefault,"das_call_member< sf::Color (sf::Image::*)(unsigned int,unsigned int) const , &sf::Image::getPixel >::invoke")
		->args({"self","x","y"});
	using _method_116 = das::das_call_member< const unsigned char * (sf::Image::*)() const,&sf::Image::getPixelsPtr >;
	addExtern<DAS_CALL_METHOD(_method_116)>(*this,lib,"getPixelsPtr",SideEffects::worstDefault,"das_call_member< const unsigned char * (sf::Image::*)() const , &sf::Image::getPixelsPtr >::invoke")
		->args({"self"});
	using _method_117 = das::das_call_member< void (sf::Image::*)(),&sf::Image::flipHorizontally >;
	addExtern<DAS_CALL_METHOD(_method_117)>(*this,lib,"flipHorizontally",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)() , &sf::Image::flipHorizontally >::invoke")
		->args({"self"});
	using _method_118 = das::das_call_member< void (sf::Image::*)(),&sf::Image::flipVertically >;
	addExtern<DAS_CALL_METHOD(_method_118)>(*this,lib,"flipVertically",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)() , &sf::Image::flipVertically >::invoke")
		->args({"self"});
	using _method_119 = das::das_call_member< bool (sf::Texture::*)(unsigned int,unsigned int),&sf::Texture::create >;
	addExtern<DAS_CALL_METHOD(_method_119)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(unsigned int,unsigned int) , &sf::Texture::create >::invoke")
		->args({"self","width","height"});
	using _method_120 = das::das_call_member< bool (sf::Texture::*)(const std::basic_string<char> &,const sf::Rect<int> &),&sf::Texture::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_120)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const std::basic_string<char> &,const sf::Rect<int> &) , &sf::Texture::loadFromFile >::invoke")
		->args({"self","filename","area"});
	using _method_121 = das::das_call_member< bool (sf::Texture::*)(const void *,unsigned long long,const sf::Rect<int> &),&sf::Texture::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_121)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const void *,unsigned long long,const sf::Rect<int> &) , &sf::Texture::loadFromMemory >::invoke")
		->args({"self","data","size","area"});
	using _method_122 = das::das_call_member< bool (sf::Texture::*)(sf::InputStream &,const sf::Rect<int> &),&sf::Texture::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_122)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(sf::InputStream &,const sf::Rect<int> &) , &sf::Texture::loadFromStream >::invoke")
		->args({"self","stream","area"});
	using _method_123 = das::das_call_member< bool (sf::Texture::*)(const sf::Image &,const sf::Rect<int> &),&sf::Texture::loadFromImage >;
	addExtern<DAS_CALL_METHOD(_method_123)>(*this,lib,"loadFromImage",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const sf::Image &,const sf::Rect<int> &) , &sf::Texture::loadFromImage >::invoke")
		->args({"self","image","area"});
	using _method_124 = das::das_call_member< sf::Vector2<unsigned int> (sf::Texture::*)() const,&sf::Texture::getSize >;
	addExtern<DAS_CALL_METHOD(_method_124)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::Texture::*)() const , &sf::Texture::getSize >::invoke")
		->args({"self"});
	using _method_125 = das::das_call_member< void (sf::Texture::*)(const unsigned char *),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_125)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const unsigned char *) , &sf::Texture::update >::invoke")
		->args({"self","pixels"});
	using _method_126 = das::das_call_member< void (sf::Texture::*)(const unsigned char *,unsigned int,unsigned int,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_126)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const unsigned char *,unsigned int,unsigned int,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","pixels","width","height","x","y"});
	using _method_127 = das::das_call_member< void (sf::Texture::*)(const sf::Texture &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_127)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Texture &) , &sf::Texture::update >::invoke")
		->args({"self","texture"});
	using _method_128 = das::das_call_member< void (sf::Texture::*)(const sf::Texture &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_128)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Texture &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","texture","x","y"});
	using _method_129 = das::das_call_member< void (sf::Texture::*)(const sf::Image &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_129)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Image &) , &sf::Texture::update >::invoke")
		->args({"self","image"});
	using _method_130 = das::das_call_member< void (sf::Texture::*)(const sf::Image &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_130)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Image &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","image","x","y"});
	using _method_131 = das::das_call_member< void (sf::Texture::*)(const sf::Window &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_131)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Window &) , &sf::Texture::update >::invoke")
		->args({"self","window"});
	using _method_132 = das::das_call_member< void (sf::Texture::*)(const sf::Window &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_132)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Window &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","window","x","y"});
	using _method_133 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_133)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_134 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_134)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isSmooth >::invoke")
		->args({"self"});
	using _method_135 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setSrgb >;
	addExtern<DAS_CALL_METHOD(_method_135)>(*this,lib,"setSrgb",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setSrgb >::invoke")
		->args({"self","sRgb"});
	using _method_136 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isSrgb >;
	addExtern<DAS_CALL_METHOD(_method_136)>(*this,lib,"isSrgb",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isSrgb >::invoke")
		->args({"self"});
	using _method_137 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_137)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_138 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_138)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isRepeated >::invoke")
		->args({"self"});
	using _method_139 = das::das_call_member< bool (sf::Texture::*)(),&sf::Texture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_139)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() , &sf::Texture::generateMipmap >::invoke")
		->args({"self"});
	using _method_140 = das::das_call_member< void (sf::Texture::*)(sf::Texture &),&sf::Texture::swap >;
	addExtern<DAS_CALL_METHOD(_method_140)>(*this,lib,"swap",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(sf::Texture &) , &sf::Texture::swap >::invoke")
		->args({"self","right"});
	using _method_141 = das::das_call_member< unsigned int (sf::Texture::*)() const,&sf::Texture::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_141)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Texture::*)() const , &sf::Texture::getNativeHandle >::invoke")
		->args({"self"});
	using _method_142 = das::das_call_member< bool (sf::Font::*)(const std::basic_string<char> &),&sf::Font::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_142)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const std::basic_string<char> &) , &sf::Font::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_143 = das::das_call_member< bool (sf::Font::*)(const void *,unsigned long long),&sf::Font::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_143)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const void *,unsigned long long) , &sf::Font::loadFromMemory >::invoke")
		->args({"self","data","sizeInBytes"});
	using _method_144 = das::das_call_member< bool (sf::Font::*)(sf::InputStream &),&sf::Font::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_144)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(sf::InputStream &) , &sf::Font::loadFromStream >::invoke")
		->args({"self","stream"});
	using _method_145 = das::das_call_member< const sf::Font::Info & (sf::Font::*)() const,&sf::Font::getInfo >;
	addExtern<DAS_CALL_METHOD(_method_145), SimNode_ExtFuncCallRef>(*this,lib,"getInfo",SideEffects::worstDefault,"das_call_member< const sf::Font::Info & (sf::Font::*)() const , &sf::Font::getInfo >::invoke")
		->args({"self"});
	using _method_146 = das::das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const,&sf::Font::getGlyph >;
	addExtern<DAS_CALL_METHOD(_method_146), SimNode_ExtFuncCallRef>(*this,lib,"getGlyph",SideEffects::worstDefault,"das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const , &sf::Font::getGlyph >::invoke")
		->args({"self","codePoint","characterSize","bold","outlineThickness"})
		->arg_init(4,make_smart<ExprConstFloat>(0.00000000000000000));
	using _method_147 = das::das_call_member< bool (sf::Font::*)(unsigned int) const,&sf::Font::hasGlyph >;
	addExtern<DAS_CALL_METHOD(_method_147)>(*this,lib,"hasGlyph",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(unsigned int) const , &sf::Font::hasGlyph >::invoke")
		->args({"self","codePoint"});
	using _method_148 = das::das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const,&sf::Font::getKerning >;
	addExtern<DAS_CALL_METHOD(_method_148)>(*this,lib,"getKerning",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const , &sf::Font::getKerning >::invoke")
		->args({"self","first","second","characterSize","bold"})
		->arg_init(4,make_smart<ExprConstBool>(false));
	using _method_149 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_149)>(*this,lib,"getLineSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getLineSpacing >::invoke")
		->args({"self","characterSize"});
	using _method_150 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlinePosition >;
	addExtern<DAS_CALL_METHOD(_method_150)>(*this,lib,"getUnderlinePosition",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlinePosition >::invoke")
		->args({"self","characterSize"});
	using _method_151 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getUnderlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_151)>(*this,lib,"getUnderlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getUnderlineThickness >::invoke")
		->args({"self","characterSize"});
	using _method_152 = das::das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const,&sf::Font::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_152), SimNode_ExtFuncCallRef>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture & (sf::Font::*)(unsigned int) const , &sf::Font::getTexture >::invoke")
		->args({"self","characterSize"});
	using _method_153 = das::das_call_member< void (sf::Font::*)(bool),&sf::Font::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_153)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::Font::*)(bool) , &sf::Font::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_154 = das::das_call_member< bool (sf::Font::*)() const,&sf::Font::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_154)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)() const , &sf::Font::isSmooth >::invoke")
		->args({"self"});
	using _method_155 = das::das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &),&sf::RectangleShape::setSize >;
	addExtern<DAS_CALL_METHOD(_method_155)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::RectangleShape::*)(const sf::Vector2<float> &) , &sf::RectangleShape::setSize >::invoke")
		->args({"self","size"});
	using _method_156 = das::das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const,&sf::RectangleShape::getSize >;
	addExtern<DAS_CALL_METHOD(_method_156), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::RectangleShape::*)() const , &sf::RectangleShape::getSize >::invoke")
		->args({"self"});
	using _method_157 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_157)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setCenter >::invoke")
		->args({"self","x","y"});
	using _method_158 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setCenter >;
	addExtern<DAS_CALL_METHOD(_method_158)>(*this,lib,"setCenter",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setCenter >::invoke")
		->args({"self","center"});
	using _method_159 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_159)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::setSize >::invoke")
		->args({"self","width","height"});
	using _method_160 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::setSize >;
	addExtern<DAS_CALL_METHOD(_method_160)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::setSize >::invoke")
		->args({"self","size"});
	using _method_161 = das::das_call_member< void (sf::View::*)(float),&sf::View::setRotation >;
	addExtern<DAS_CALL_METHOD(_method_161)>(*this,lib,"setRotation",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::setRotation >::invoke")
		->args({"self","angle"});
	using _method_162 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::setViewport >;
	addExtern<DAS_CALL_METHOD(_method_162)>(*this,lib,"setViewport",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::setViewport >::invoke")
		->args({"self","viewport"});
	using _method_163 = das::das_call_member< void (sf::View::*)(const sf::Rect<float> &),&sf::View::reset >;
	addExtern<DAS_CALL_METHOD(_method_163)>(*this,lib,"reset",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Rect<float> &) , &sf::View::reset >::invoke")
		->args({"self","rectangle"});
	using _method_164 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getCenter >;
	addExtern<DAS_CALL_METHOD(_method_164), SimNode_ExtFuncCallRef>(*this,lib,"getCenter",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getCenter >::invoke")
		->args({"self"});
	using _method_165 = das::das_call_member< const sf::Vector2<float> & (sf::View::*)() const,&sf::View::getSize >;
	addExtern<DAS_CALL_METHOD(_method_165), SimNode_ExtFuncCallRef>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::View::*)() const , &sf::View::getSize >::invoke")
		->args({"self"});
	using _method_166 = das::das_call_member< float (sf::View::*)() const,&sf::View::getRotation >;
	addExtern<DAS_CALL_METHOD(_method_166)>(*this,lib,"getRotation",SideEffects::worstDefault,"das_call_member< float (sf::View::*)() const , &sf::View::getRotation >::invoke")
		->args({"self"});
	using _method_167 = das::das_call_member< const sf::Rect<float> & (sf::View::*)() const,&sf::View::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_167), SimNode_ExtFuncCallRef>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< const sf::Rect<float> & (sf::View::*)() const , &sf::View::getViewport >::invoke")
		->args({"self"});
	using _method_168 = das::das_call_member< void (sf::View::*)(float,float),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_168)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float,float) , &sf::View::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_169 = das::das_call_member< void (sf::View::*)(const sf::Vector2<float> &),&sf::View::move >;
	addExtern<DAS_CALL_METHOD(_method_169)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(const sf::Vector2<float> &) , &sf::View::move >::invoke")
		->args({"self","offset"});
	using _method_170 = das::das_call_member< void (sf::View::*)(float),&sf::View::rotate >;
	addExtern<DAS_CALL_METHOD(_method_170)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::rotate >::invoke")
		->args({"self","angle"});
	using _method_171 = das::das_call_member< void (sf::View::*)(float),&sf::View::zoom >;
	addExtern<DAS_CALL_METHOD(_method_171)>(*this,lib,"zoom",SideEffects::worstDefault,"das_call_member< void (sf::View::*)(float) , &sf::View::zoom >::invoke")
		->args({"self","factor"});
	using _method_172 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_172), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getTransform >::invoke")
		->args({"self"});
	using _method_173 = das::das_call_member< const sf::Transform & (sf::View::*)() const,&sf::View::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_173), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::View::*)() const , &sf::View::getInverseTransform >::invoke")
		->args({"self"});
	using _method_174 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Color &),&sf::RenderTarget::clear >;
	addExtern<DAS_CALL_METHOD(_method_174)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Color &) , &sf::RenderTarget::clear >::invoke")
		->args({"self","color"});
	using _method_175 = das::das_call_member< void (sf::RenderTarget::*)(const sf::View &),&sf::RenderTarget::setView >;
	addExtern<DAS_CALL_METHOD(_method_175)>(*this,lib,"setView",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::View &) , &sf::RenderTarget::setView >::invoke")
		->args({"self","view"});
	using _method_176 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getView >;
	addExtern<DAS_CALL_METHOD(_method_176), SimNode_ExtFuncCallRef>(*this,lib,"getView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getView >::invoke")
		->args({"self"});
	using _method_177 = das::das_call_member< const sf::View & (sf::RenderTarget::*)() const,&sf::RenderTarget::getDefaultView >;
	addExtern<DAS_CALL_METHOD(_method_177), SimNode_ExtFuncCallRef>(*this,lib,"getDefaultView",SideEffects::worstDefault,"das_call_member< const sf::View & (sf::RenderTarget::*)() const , &sf::RenderTarget::getDefaultView >::invoke")
		->args({"self"});
	using _method_178 = das::das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const,&sf::RenderTarget::getViewport >;
	addExtern<DAS_CALL_METHOD(_method_178)>(*this,lib,"getViewport",SideEffects::worstDefault,"das_call_member< sf::Rect<int> (sf::RenderTarget::*)(const sf::View &) const , &sf::RenderTarget::getViewport >::invoke")
		->args({"self","view"});
	using _method_179 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_179)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point"});
	using _method_180 = das::das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const,&sf::RenderTarget::mapPixelToCoords >;
	addExtern<DAS_CALL_METHOD(_method_180)>(*this,lib,"mapPixelToCoords",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::RenderTarget::*)(const sf::Vector2<int> &,const sf::View &) const , &sf::RenderTarget::mapPixelToCoords >::invoke")
		->args({"self","point","view"});
	using _method_181 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_181)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point"});
	using _method_182 = das::das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const,&sf::RenderTarget::mapCoordsToPixel >;
	addExtern<DAS_CALL_METHOD(_method_182)>(*this,lib,"mapCoordsToPixel",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::RenderTarget::*)(const sf::Vector2<float> &,const sf::View &) const , &sf::RenderTarget::mapCoordsToPixel >::invoke")
		->args({"self","point","view"});
	using _method_183 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_183)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Drawable &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","drawable","states"});
	using _method_184 = das::das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,unsigned long long,sf::PrimitiveType,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_184)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::Vertex *,unsigned long long,sf::PrimitiveType,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertices","vertexCount","type","states"});
	using _method_185 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_185)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","states"});
	using _method_186 = das::das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,unsigned long long,unsigned long long,const sf::RenderStates &),&sf::RenderTarget::draw >;
	addExtern<DAS_CALL_METHOD(_method_186)>(*this,lib,"draw",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)(const sf::VertexBuffer &,unsigned long long,unsigned long long,const sf::RenderStates &) , &sf::RenderTarget::draw >::invoke")
		->args({"self","vertexBuffer","firstVertex","vertexCount","states"});
	using _method_187 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::pushGLStates >;
	addExtern<DAS_CALL_METHOD(_method_187)>(*this,lib,"pushGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::pushGLStates >::invoke")
		->args({"self"});
	using _method_188 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::popGLStates >;
	addExtern<DAS_CALL_METHOD(_method_188)>(*this,lib,"popGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::popGLStates >::invoke")
		->args({"self"});
	using _method_189 = das::das_call_member< void (sf::RenderTarget::*)(),&sf::RenderTarget::resetGLStates >;
	addExtern<DAS_CALL_METHOD(_method_189)>(*this,lib,"resetGLStates",SideEffects::worstDefault,"das_call_member< void (sf::RenderTarget::*)() , &sf::RenderTarget::resetGLStates >::invoke")
		->args({"self"});
	using _method_190 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_190)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,bool) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","depthBuffer"});
	using _method_191 = das::das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &),&sf::RenderTexture::create >;
	addExtern<DAS_CALL_METHOD(_method_191)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)(unsigned int,unsigned int,const sf::ContextSettings &) , &sf::RenderTexture::create >::invoke")
		->args({"self","width","height","settings"});
	using _method_192 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_192)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_193 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_193)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isSmooth >::invoke")
		->args({"self"});
	using _method_194 = das::das_call_member< void (sf::RenderTexture::*)(bool),&sf::RenderTexture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_194)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::RenderTexture::*)(bool) , &sf::RenderTexture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_195 = das::das_call_member< bool (sf::RenderTexture::*)() const,&sf::RenderTexture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_195)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() const , &sf::RenderTexture::isRepeated >::invoke")
		->args({"self"});
	using _method_196 = das::das_call_member< bool (sf::RenderTexture::*)(),&sf::RenderTexture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_196)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::RenderTexture::*)() , &sf::RenderTexture::generateMipmap >::invoke")
		->args({"self"});
}
}

