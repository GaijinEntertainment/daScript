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
	using _method_67 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getScale >;
	addExtern<DAS_CALL_METHOD(_method_67), SimNode_ExtFuncCallRef>(*this,lib,"getScale",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getScale >::invoke")
		->args({"self"});
	using _method_68 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getOrigin >;
	addExtern<DAS_CALL_METHOD(_method_68), SimNode_ExtFuncCallRef>(*this,lib,"getOrigin",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getOrigin >::invoke")
		->args({"self"});
	using _method_69 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_69)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_70 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_70)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::move >::invoke")
		->args({"self","offset"});
	using _method_71 = das::das_call_member< void (sf::Transformable::*)(float),&sf::Transformable::rotate >;
	addExtern<DAS_CALL_METHOD(_method_71)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float) , &sf::Transformable::rotate >::invoke")
		->args({"self","angle"});
	using _method_72 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_72)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::scale >::invoke")
		->args({"self","factorX","factorY"});
	using _method_73 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_73)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::scale >::invoke")
		->args({"self","factor"});
	using _method_74 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_74), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getTransform >::invoke")
		->args({"self"});
	using _method_75 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_75), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getInverseTransform >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Color>(*this,lib,"Color","sf::Color");
	addCtorAndUsing<sf::Color,unsigned char,unsigned char,unsigned char,unsigned char>(*this,lib,"Color","sf::Color")
		->args({"red","green","blue","alpha"});
	addCtorAndUsing<sf::Color,unsigned int>(*this,lib,"Color","sf::Color")
		->args({"color"});
	using _method_76 = das::das_call_member< unsigned int (sf::Color::*)() const,&sf::Color::toInteger >;
	addExtern<DAS_CALL_METHOD(_method_76)>(*this,lib,"toInteger",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Color::*)() const , &sf::Color::toInteger >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Vertex>(*this,lib,"Vertex","sf::Vertex");
	addCtorAndUsing<sf::Vertex,const sf::Vector2<float> &>(*this,lib,"Vertex","sf::Vertex")
		->args({"thePosition"});
	addCtorAndUsing<sf::Vertex,const sf::Vector2<float> &,const sf::Color &>(*this,lib,"Vertex","sf::Vertex")
		->args({"thePosition","theColor"});
	addCtorAndUsing<sf::Vertex,const sf::Vector2<float> &,const sf::Vector2<float> &>(*this,lib,"Vertex","sf::Vertex")
		->args({"thePosition","theTexCoords"});
	addCtorAndUsing<sf::Vertex,const sf::Vector2<float> &,const sf::Color &,const sf::Vector2<float> &>(*this,lib,"Vertex","sf::Vertex")
		->args({"thePosition","theColor","theTexCoords"});
	addCtorAndUsing<sf::VertexArray>(*this,lib,"VertexArray","sf::VertexArray");
	addCtorAndUsing<sf::VertexArray,sf::PrimitiveType,unsigned long long>(*this,lib,"VertexArray","sf::VertexArray")
		->args({"type","vertexCount"})
		->arg_init(1,make_smart<ExprConstUInt64>(0x0));
	using _method_77 = das::das_call_member< unsigned long long (sf::VertexArray::*)() const,&sf::VertexArray::getVertexCount >;
	addExtern<DAS_CALL_METHOD(_method_77)>(*this,lib,"getVertexCount",SideEffects::worstDefault,"das_call_member< unsigned long long (sf::VertexArray::*)() const , &sf::VertexArray::getVertexCount >::invoke")
		->args({"self"});
	using _method_78 = das::das_call_member< sf::Vertex & (sf::VertexArray::*)(unsigned long long),&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_78), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< sf::Vertex & (sf::VertexArray::*)(unsigned long long) , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_79 = das::das_call_member< const sf::Vertex & (sf::VertexArray::*)(unsigned long long) const,&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_79), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< const sf::Vertex & (sf::VertexArray::*)(unsigned long long) const , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_80 = das::das_call_member< void (sf::VertexArray::*)(),&sf::VertexArray::clear >;
	addExtern<DAS_CALL_METHOD(_method_80)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)() , &sf::VertexArray::clear >::invoke")
		->args({"self"});
	using _method_81 = das::das_call_member< void (sf::VertexArray::*)(unsigned long long),&sf::VertexArray::resize >;
	addExtern<DAS_CALL_METHOD(_method_81)>(*this,lib,"resize",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(unsigned long long) , &sf::VertexArray::resize >::invoke")
		->args({"self","vertexCount"});
	using _method_82 = das::das_call_member< void (sf::VertexArray::*)(const sf::Vertex &),&sf::VertexArray::append >;
	addExtern<DAS_CALL_METHOD(_method_82)>(*this,lib,"append",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(const sf::Vertex &) , &sf::VertexArray::append >::invoke")
		->args({"self","vertex"});
	using _method_83 = das::das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType),&sf::VertexArray::setPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_83)>(*this,lib,"setPrimitiveType",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType) , &sf::VertexArray::setPrimitiveType >::invoke")
		->args({"self","type"});
	using _method_84 = das::das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const,&sf::VertexArray::getPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_84)>(*this,lib,"getPrimitiveType",SideEffects::worstDefault,"das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const , &sf::VertexArray::getPrimitiveType >::invoke")
		->args({"self"});
	using _method_85 = das::das_call_member< sf::Rect<float> (sf::VertexArray::*)() const,&sf::VertexArray::getBounds >;
	addExtern<DAS_CALL_METHOD(_method_85)>(*this,lib,"getBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::VertexArray::*)() const , &sf::VertexArray::getBounds >::invoke")
		->args({"self"});
	using _method_86 = das::das_call_member< void (sf::Shape::*)(const sf::Texture *,bool),&sf::Shape::setTexture >;
	addExtern<DAS_CALL_METHOD(_method_86)>(*this,lib,"setTexture",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Texture *,bool) , &sf::Shape::setTexture >::invoke")
		->args({"self","texture","resetRect"})
		->arg_init(2,make_smart<ExprConstBool>(false));
	using _method_87 = das::das_call_member< void (sf::Shape::*)(const sf::Rect<int> &),&sf::Shape::setTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_87)>(*this,lib,"setTextureRect",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Rect<int> &) , &sf::Shape::setTextureRect >::invoke")
		->args({"self","rect"});
	using _method_88 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setFillColor >;
	addExtern<DAS_CALL_METHOD(_method_88)>(*this,lib,"setFillColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setFillColor >::invoke")
		->args({"self","color"});
	using _method_89 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_89)>(*this,lib,"setOutlineColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setOutlineColor >::invoke")
		->args({"self","color"});
	using _method_90 = das::das_call_member< void (sf::Shape::*)(float),&sf::Shape::setOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_90)>(*this,lib,"setOutlineThickness",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(float) , &sf::Shape::setOutlineThickness >::invoke")
		->args({"self","thickness"});
	using _method_91 = das::das_call_member< const sf::Texture * (sf::Shape::*)() const,&sf::Shape::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_91)>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture * (sf::Shape::*)() const , &sf::Shape::getTexture >::invoke")
		->args({"self"});
	using _method_92 = das::das_call_member< const sf::Rect<int> & (sf::Shape::*)() const,&sf::Shape::getTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_92), SimNode_ExtFuncCallRef>(*this,lib,"getTextureRect",SideEffects::worstDefault,"das_call_member< const sf::Rect<int> & (sf::Shape::*)() const , &sf::Shape::getTextureRect >::invoke")
		->args({"self"});
	using _method_93 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getFillColor >;
	addExtern<DAS_CALL_METHOD(_method_93), SimNode_ExtFuncCallRef>(*this,lib,"getFillColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getFillColor >::invoke")
		->args({"self"});
	using _method_94 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_94), SimNode_ExtFuncCallRef>(*this,lib,"getOutlineColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getOutlineColor >::invoke")
		->args({"self"});
	using _method_95 = das::das_call_member< float (sf::Shape::*)() const,&sf::Shape::getOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_95)>(*this,lib,"getOutlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Shape::*)() const , &sf::Shape::getOutlineThickness >::invoke")
		->args({"self"});
	using _method_96 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_96)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getLocalBounds >::invoke")
		->args({"self"});
	using _method_97 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_97)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getGlobalBounds >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::CircleShape,float,unsigned long long>(*this,lib,"CircleShape","sf::CircleShape")
		->args({"radius","pointCount"})
		->arg_init(0,make_smart<ExprConstFloat>(0.00000000000000000))
		->arg_init(1,make_smart<ExprConstUInt64>(0x1e));
	using _method_98 = das::das_call_member< void (sf::CircleShape::*)(float),&sf::CircleShape::setRadius >;
	addExtern<DAS_CALL_METHOD(_method_98)>(*this,lib,"setRadius",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(float) , &sf::CircleShape::setRadius >::invoke")
		->args({"self","radius"});
	using _method_99 = das::das_call_member< float (sf::CircleShape::*)() const,&sf::CircleShape::getRadius >;
	addExtern<DAS_CALL_METHOD(_method_99)>(*this,lib,"getRadius",SideEffects::worstDefault,"das_call_member< float (sf::CircleShape::*)() const , &sf::CircleShape::getRadius >::invoke")
		->args({"self"});
	using _method_100 = das::das_call_member< void (sf::CircleShape::*)(unsigned long long),&sf::CircleShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_100)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(unsigned long long) , &sf::CircleShape::setPointCount >::invoke")
		->args({"self","count"});
	addCtorAndUsing<sf::ConvexShape,unsigned long long>(*this,lib,"ConvexShape","sf::ConvexShape")
		->args({"pointCount"})
		->arg_init(0,make_smart<ExprConstUInt64>(0x0));
	using _method_101 = das::das_call_member< void (sf::ConvexShape::*)(unsigned long long),&sf::ConvexShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_101)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(unsigned long long) , &sf::ConvexShape::setPointCount >::invoke")
		->args({"self","count"});
	using _method_102 = das::das_call_member< void (sf::ConvexShape::*)(unsigned long long,const sf::Vector2<float> &),&sf::ConvexShape::setPoint >;
	addExtern<DAS_CALL_METHOD(_method_102)>(*this,lib,"setPoint",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(unsigned long long,const sf::Vector2<float> &) , &sf::ConvexShape::setPoint >::invoke")
		->args({"self","index","point"});
	addCtorAndUsing<sf::Glyph>(*this,lib,"Glyph","sf::Glyph");
	addCtorAndUsing<sf::Image>(*this,lib,"Image","sf::Image");
	using _method_103 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_103)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &) , &sf::Image::create >::invoke")
		->args({"self","width","height","color"});
	using _method_104 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_104)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *) , &sf::Image::create >::invoke")
		->args({"self","width","height","pixels"});
	using _method_105 = das::das_call_member< bool (sf::Image::*)(const std::basic_string<char> &),&sf::Image::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_105)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) , &sf::Image::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_106 = das::das_call_member< bool (sf::Image::*)(const void *,unsigned long long),&sf::Image::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_106)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const void *,unsigned long long) , &sf::Image::loadFromMemory >::invoke")
		->args({"self","data","size"});
	using _method_107 = das::das_call_member< bool (sf::Image::*)(sf::InputStream &),&sf::Image::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_107)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(sf::InputStream &) , &sf::Image::loadFromStream >::invoke")
		->args({"self","stream"});
	using _method_108 = das::das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) const,&sf::Image::saveToFile >;
	addExtern<DAS_CALL_METHOD(_method_108)>(*this,lib,"saveToFile",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) const , &sf::Image::saveToFile >::invoke")
		->args({"self","filename"});
	using _method_109 = das::das_call_member< bool (sf::Image::*)(std::vector<unsigned char> &,const std::basic_string<char> &) const,&sf::Image::saveToMemory >;
	addExtern<DAS_CALL_METHOD(_method_109)>(*this,lib,"saveToMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(std::vector<unsigned char> &,const std::basic_string<char> &) const , &sf::Image::saveToMemory >::invoke")
		->args({"self","output","format"});
	using _method_110 = das::das_call_member< sf::Vector2<unsigned int> (sf::Image::*)() const,&sf::Image::getSize >;
	addExtern<DAS_CALL_METHOD(_method_110)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::Image::*)() const , &sf::Image::getSize >::invoke")
		->args({"self"});
	using _method_111 = das::das_call_member< void (sf::Image::*)(const sf::Color &,unsigned char),&sf::Image::createMaskFromColor >;
	addExtern<DAS_CALL_METHOD(_method_111)>(*this,lib,"createMaskFromColor",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(const sf::Color &,unsigned char) , &sf::Image::createMaskFromColor >::invoke")
		->args({"self","color","alpha"});
	using _method_112 = das::das_call_member< void (sf::Image::*)(const sf::Image &,unsigned int,unsigned int,const sf::Rect<int> &,bool),&sf::Image::copy >;
	addExtern<DAS_CALL_METHOD(_method_112)>(*this,lib,"copy",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(const sf::Image &,unsigned int,unsigned int,const sf::Rect<int> &,bool) , &sf::Image::copy >::invoke")
		->args({"self","source","destX","destY","sourceRect","applyAlpha"})
		->arg_init(5,make_smart<ExprConstBool>(false));
	using _method_113 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &),&sf::Image::setPixel >;
	addExtern<DAS_CALL_METHOD(_method_113)>(*this,lib,"setPixel",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &) , &sf::Image::setPixel >::invoke")
		->args({"self","x","y","color"});
	using _method_114 = das::das_call_member< sf::Color (sf::Image::*)(unsigned int,unsigned int) const,&sf::Image::getPixel >;
	addExtern<DAS_CALL_METHOD(_method_114),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getPixel",SideEffects::worstDefault,"das_call_member< sf::Color (sf::Image::*)(unsigned int,unsigned int) const , &sf::Image::getPixel >::invoke")
		->args({"self","x","y"});
	using _method_115 = das::das_call_member< const unsigned char * (sf::Image::*)() const,&sf::Image::getPixelsPtr >;
	addExtern<DAS_CALL_METHOD(_method_115)>(*this,lib,"getPixelsPtr",SideEffects::worstDefault,"das_call_member< const unsigned char * (sf::Image::*)() const , &sf::Image::getPixelsPtr >::invoke")
		->args({"self"});
	using _method_116 = das::das_call_member< void (sf::Image::*)(),&sf::Image::flipHorizontally >;
	addExtern<DAS_CALL_METHOD(_method_116)>(*this,lib,"flipHorizontally",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)() , &sf::Image::flipHorizontally >::invoke")
		->args({"self"});
	using _method_117 = das::das_call_member< void (sf::Image::*)(),&sf::Image::flipVertically >;
	addExtern<DAS_CALL_METHOD(_method_117)>(*this,lib,"flipVertically",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)() , &sf::Image::flipVertically >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Texture>(*this,lib,"Texture","sf::Texture");
	addCtorAndUsing<sf::Texture,const sf::Texture &>(*this,lib,"Texture","sf::Texture")
		->args({"copy"});
	using _method_118 = das::das_call_member< bool (sf::Texture::*)(unsigned int,unsigned int),&sf::Texture::create >;
	addExtern<DAS_CALL_METHOD(_method_118)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(unsigned int,unsigned int) , &sf::Texture::create >::invoke")
		->args({"self","width","height"});
	using _method_119 = das::das_call_member< bool (sf::Texture::*)(const std::basic_string<char> &,const sf::Rect<int> &),&sf::Texture::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_119)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const std::basic_string<char> &,const sf::Rect<int> &) , &sf::Texture::loadFromFile >::invoke")
		->args({"self","filename","area"});
	using _method_120 = das::das_call_member< bool (sf::Texture::*)(const void *,unsigned long long,const sf::Rect<int> &),&sf::Texture::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_120)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const void *,unsigned long long,const sf::Rect<int> &) , &sf::Texture::loadFromMemory >::invoke")
		->args({"self","data","size","area"});
	using _method_121 = das::das_call_member< bool (sf::Texture::*)(sf::InputStream &,const sf::Rect<int> &),&sf::Texture::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_121)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(sf::InputStream &,const sf::Rect<int> &) , &sf::Texture::loadFromStream >::invoke")
		->args({"self","stream","area"});
	using _method_122 = das::das_call_member< bool (sf::Texture::*)(const sf::Image &,const sf::Rect<int> &),&sf::Texture::loadFromImage >;
	addExtern<DAS_CALL_METHOD(_method_122)>(*this,lib,"loadFromImage",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)(const sf::Image &,const sf::Rect<int> &) , &sf::Texture::loadFromImage >::invoke")
		->args({"self","image","area"});
	using _method_123 = das::das_call_member< sf::Vector2<unsigned int> (sf::Texture::*)() const,&sf::Texture::getSize >;
	addExtern<DAS_CALL_METHOD(_method_123)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::Texture::*)() const , &sf::Texture::getSize >::invoke")
		->args({"self"});
	using _method_124 = das::das_call_member< void (sf::Texture::*)(const unsigned char *),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_124)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const unsigned char *) , &sf::Texture::update >::invoke")
		->args({"self","pixels"});
	using _method_125 = das::das_call_member< void (sf::Texture::*)(const unsigned char *,unsigned int,unsigned int,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_125)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const unsigned char *,unsigned int,unsigned int,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","pixels","width","height","x","y"});
	using _method_126 = das::das_call_member< void (sf::Texture::*)(const sf::Texture &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_126)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Texture &) , &sf::Texture::update >::invoke")
		->args({"self","texture"});
	using _method_127 = das::das_call_member< void (sf::Texture::*)(const sf::Texture &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_127)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Texture &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","texture","x","y"});
	using _method_128 = das::das_call_member< void (sf::Texture::*)(const sf::Image &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_128)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Image &) , &sf::Texture::update >::invoke")
		->args({"self","image"});
	using _method_129 = das::das_call_member< void (sf::Texture::*)(const sf::Image &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_129)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Image &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","image","x","y"});
	using _method_130 = das::das_call_member< void (sf::Texture::*)(const sf::Window &),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_130)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Window &) , &sf::Texture::update >::invoke")
		->args({"self","window"});
	using _method_131 = das::das_call_member< void (sf::Texture::*)(const sf::Window &,unsigned int,unsigned int),&sf::Texture::update >;
	addExtern<DAS_CALL_METHOD(_method_131)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(const sf::Window &,unsigned int,unsigned int) , &sf::Texture::update >::invoke")
		->args({"self","window","x","y"});
	using _method_132 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setSmooth >;
	addExtern<DAS_CALL_METHOD(_method_132)>(*this,lib,"setSmooth",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setSmooth >::invoke")
		->args({"self","smooth"});
	using _method_133 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isSmooth >;
	addExtern<DAS_CALL_METHOD(_method_133)>(*this,lib,"isSmooth",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isSmooth >::invoke")
		->args({"self"});
	using _method_134 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setSrgb >;
	addExtern<DAS_CALL_METHOD(_method_134)>(*this,lib,"setSrgb",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setSrgb >::invoke")
		->args({"self","sRgb"});
	using _method_135 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isSrgb >;
	addExtern<DAS_CALL_METHOD(_method_135)>(*this,lib,"isSrgb",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isSrgb >::invoke")
		->args({"self"});
	using _method_136 = das::das_call_member< void (sf::Texture::*)(bool),&sf::Texture::setRepeated >;
	addExtern<DAS_CALL_METHOD(_method_136)>(*this,lib,"setRepeated",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(bool) , &sf::Texture::setRepeated >::invoke")
		->args({"self","repeated"});
	using _method_137 = das::das_call_member< bool (sf::Texture::*)() const,&sf::Texture::isRepeated >;
	addExtern<DAS_CALL_METHOD(_method_137)>(*this,lib,"isRepeated",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() const , &sf::Texture::isRepeated >::invoke")
		->args({"self"});
	using _method_138 = das::das_call_member< bool (sf::Texture::*)(),&sf::Texture::generateMipmap >;
	addExtern<DAS_CALL_METHOD(_method_138)>(*this,lib,"generateMipmap",SideEffects::worstDefault,"das_call_member< bool (sf::Texture::*)() , &sf::Texture::generateMipmap >::invoke")
		->args({"self"});
	using _method_139 = das::das_call_member< void (sf::Texture::*)(sf::Texture &),&sf::Texture::swap >;
	addExtern<DAS_CALL_METHOD(_method_139)>(*this,lib,"swap",SideEffects::worstDefault,"das_call_member< void (sf::Texture::*)(sf::Texture &) , &sf::Texture::swap >::invoke")
		->args({"self","right"});
	using _method_140 = das::das_call_member< unsigned int (sf::Texture::*)() const,&sf::Texture::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_140)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Texture::*)() const , &sf::Texture::getNativeHandle >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Font>(*this,lib,"Font","sf::Font");
	addCtorAndUsing<sf::Font,const sf::Font &>(*this,lib,"Font","sf::Font")
		->args({"copy"});
	using _method_141 = das::das_call_member< bool (sf::Font::*)(const std::basic_string<char> &),&sf::Font::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_141)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const std::basic_string<char> &) , &sf::Font::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_142 = das::das_call_member< bool (sf::Font::*)(const void *,unsigned long long),&sf::Font::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_142)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(const void *,unsigned long long) , &sf::Font::loadFromMemory >::invoke")
		->args({"self","data","sizeInBytes"});
	using _method_143 = das::das_call_member< bool (sf::Font::*)(sf::InputStream &),&sf::Font::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_143)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(sf::InputStream &) , &sf::Font::loadFromStream >::invoke")
		->args({"self","stream"});
	using _method_144 = das::das_call_member< const sf::Font::Info & (sf::Font::*)() const,&sf::Font::getInfo >;
	addExtern<DAS_CALL_METHOD(_method_144), SimNode_ExtFuncCallRef>(*this,lib,"getInfo",SideEffects::worstDefault,"das_call_member< const sf::Font::Info & (sf::Font::*)() const , &sf::Font::getInfo >::invoke")
		->args({"self"});
	using _method_145 = das::das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const,&sf::Font::getGlyph >;
	addExtern<DAS_CALL_METHOD(_method_145), SimNode_ExtFuncCallRef>(*this,lib,"getGlyph",SideEffects::worstDefault,"das_call_member< const sf::Glyph & (sf::Font::*)(unsigned int,unsigned int,bool,float) const , &sf::Font::getGlyph >::invoke")
		->args({"self","codePoint","characterSize","bold","outlineThickness"})
		->arg_init(4,make_smart<ExprConstFloat>(0.00000000000000000));
	using _method_146 = das::das_call_member< bool (sf::Font::*)(unsigned int) const,&sf::Font::hasGlyph >;
	addExtern<DAS_CALL_METHOD(_method_146)>(*this,lib,"hasGlyph",SideEffects::worstDefault,"das_call_member< bool (sf::Font::*)(unsigned int) const , &sf::Font::hasGlyph >::invoke")
		->args({"self","codePoint"});
	using _method_147 = das::das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const,&sf::Font::getKerning >;
	addExtern<DAS_CALL_METHOD(_method_147)>(*this,lib,"getKerning",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int,unsigned int,unsigned int,bool) const , &sf::Font::getKerning >::invoke")
		->args({"self","first","second","characterSize","bold"})
		->arg_init(4,make_smart<ExprConstBool>(false));
	using _method_148 = das::das_call_member< float (sf::Font::*)(unsigned int) const,&sf::Font::getLineSpacing >;
	addExtern<DAS_CALL_METHOD(_method_148)>(*this,lib,"getLineSpacing",SideEffects::worstDefault,"das_call_member< float (sf::Font::*)(unsigned int) const , &sf::Font::getLineSpacing >::invoke")
		->args({"self","characterSize"});
}
}

