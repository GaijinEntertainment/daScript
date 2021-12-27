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
void Module_dasSFML::initFunctions_2() {
	using _method_79 = das::das_call_member< const sf::ContextSettings & (sf::Window::*)() const,&sf::Window::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_79), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Window::*)() const , &sf::Window::getSettings >::invoke")
		->args({"self"});
	using _method_80 = das::das_call_member< void (sf::Window::*)(bool),&sf::Window::setVerticalSyncEnabled >;
	addExtern<DAS_CALL_METHOD(_method_80)>(*this,lib,"setVerticalSyncEnabled",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(bool) , &sf::Window::setVerticalSyncEnabled >::invoke")
		->args({"self","enabled"});
	using _method_81 = das::das_call_member< void (sf::Window::*)(unsigned int),&sf::Window::setFramerateLimit >;
	addExtern<DAS_CALL_METHOD(_method_81)>(*this,lib,"setFramerateLimit",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(unsigned int) , &sf::Window::setFramerateLimit >::invoke")
		->args({"self","limit"});
	using _method_82 = das::das_call_member< bool (sf::Window::*)(bool) const,&sf::Window::setActive >;
	addExtern<DAS_CALL_METHOD(_method_82)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Window::*)(bool) const , &sf::Window::setActive >::invoke")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_83 = das::das_call_member< void (sf::Window::*)(),&sf::Window::display >;
	addExtern<DAS_CALL_METHOD(_method_83)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)() , &sf::Window::display >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::BlendMode>(*this,lib,"BlendMode","sf::BlendMode");
	addCtorAndUsing<sf::BlendMode,sf::BlendMode::Factor,sf::BlendMode::Factor,sf::BlendMode::Equation>(*this,lib,"BlendMode","sf::BlendMode")
		->args({"sourceFactor","destinationFactor","blendEquation"})
		->arg_init(2,make_smart<ExprConstEnumeration>(0,makeType<sf::BlendMode::Equation>(lib)));
	addCtorAndUsing<sf::BlendMode,sf::BlendMode::Factor,sf::BlendMode::Factor,sf::BlendMode::Equation,sf::BlendMode::Factor,sf::BlendMode::Factor,sf::BlendMode::Equation>(*this,lib,"BlendMode","sf::BlendMode")
		->args({"colorSourceFactor","colorDestinationFactor","colorBlendEquation","alphaSourceFactor","alphaDestinationFactor","alphaBlendEquation"});
	addCtorAndUsing<sf::Transform>(*this,lib,"Transform","sf::Transform");
	addCtorAndUsing<sf::Transform,float,float,float,float,float,float,float,float,float>(*this,lib,"Transform","sf::Transform")
		->args({"a00","a01","a02","a10","a11","a12","a20","a21","a22"});
	using _method_84 = das::das_call_member< const float * (sf::Transform::*)() const,&sf::Transform::getMatrix >;
	addExtern<DAS_CALL_METHOD(_method_84)>(*this,lib,"getMatrix",SideEffects::worstDefault,"das_call_member< const float * (sf::Transform::*)() const , &sf::Transform::getMatrix >::invoke")
		->args({"self"});
	using _method_85 = das::das_call_member< sf::Transform (sf::Transform::*)() const,&sf::Transform::getInverse >;
	addExtern<DAS_CALL_METHOD(_method_85),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getInverse",SideEffects::worstDefault,"das_call_member< sf::Transform (sf::Transform::*)() const , &sf::Transform::getInverse >::invoke")
		->args({"self"});
	using _method_86 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_86)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","x","y"});
	using _method_87 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_87)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","point"});
	using _method_88 = das::das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const,&sf::Transform::transformRect >;
	addExtern<DAS_CALL_METHOD(_method_88)>(*this,lib,"transformRect",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const , &sf::Transform::transformRect >::invoke")
		->args({"self","rectangle"});
	using _method_89 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &),&sf::Transform::combine >;
	addExtern<DAS_CALL_METHOD(_method_89), SimNode_ExtFuncCallRef>(*this,lib,"combine",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &) , &sf::Transform::combine >::invoke")
		->args({"self","transform"});
	using _method_90 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_90), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::translate >::invoke")
		->args({"self","x","y"});
	using _method_91 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_91), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::translate >::invoke")
		->args({"self","offset"});
	using _method_92 = das::das_call_member< sf::Transform & (sf::Transform::*)(float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_92), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle"});
	using _method_93 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_93), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","centerX","centerY"});
	using _method_94 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_94), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","center"});
	using _method_95 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_95), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY"});
	using _method_96 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_96), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY","centerX","centerY"});
	using _method_97 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_97), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors"});
	using _method_98 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_98), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors","center"});
	addCtorAndUsing<sf::RenderStates>(*this,lib,"RenderStates","sf::RenderStates");
	addCtorAndUsing<sf::RenderStates,const sf::BlendMode &>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theBlendMode"});
	addCtorAndUsing<sf::RenderStates,const sf::Transform &>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theTransform"});
	addCtorAndUsing<sf::RenderStates,const sf::Texture *>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theTexture"});
	addCtorAndUsing<sf::RenderStates,const sf::Shader *>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theShader"});
	addCtorAndUsing<sf::RenderStates,const sf::BlendMode &,const sf::Transform &,const sf::Texture *,const sf::Shader *>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theBlendMode","theTransform","theTexture","theShader"});
	addCtorAndUsing<sf::Transformable>(*this,lib,"Transformable","sf::Transformable");
	using _method_99 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_99)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setPosition >::invoke")
		->args({"self","x","y"});
	using _method_100 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_100)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setPosition >::invoke")
		->args({"self","position"});
	using _method_101 = das::das_call_member< void (sf::Transformable::*)(float),&sf::Transformable::setRotation >;
	addExtern<DAS_CALL_METHOD(_method_101)>(*this,lib,"setRotation",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float) , &sf::Transformable::setRotation >::invoke")
		->args({"self","angle"});
	using _method_102 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setScale >;
	addExtern<DAS_CALL_METHOD(_method_102)>(*this,lib,"setScale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setScale >::invoke")
		->args({"self","factorX","factorY"});
	using _method_103 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setScale >;
	addExtern<DAS_CALL_METHOD(_method_103)>(*this,lib,"setScale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setScale >::invoke")
		->args({"self","factors"});
	using _method_104 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setOrigin >;
	addExtern<DAS_CALL_METHOD(_method_104)>(*this,lib,"setOrigin",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setOrigin >::invoke")
		->args({"self","x","y"});
	using _method_105 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setOrigin >;
	addExtern<DAS_CALL_METHOD(_method_105)>(*this,lib,"setOrigin",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setOrigin >::invoke")
		->args({"self","origin"});
	using _method_106 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getPosition >;
	addExtern<DAS_CALL_METHOD(_method_106), SimNode_ExtFuncCallRef>(*this,lib,"getPosition",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getPosition >::invoke")
		->args({"self"});
	using _method_107 = das::das_call_member< float (sf::Transformable::*)() const,&sf::Transformable::getRotation >;
	addExtern<DAS_CALL_METHOD(_method_107)>(*this,lib,"getRotation",SideEffects::worstDefault,"das_call_member< float (sf::Transformable::*)() const , &sf::Transformable::getRotation >::invoke")
		->args({"self"});
	using _method_108 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getScale >;
	addExtern<DAS_CALL_METHOD(_method_108), SimNode_ExtFuncCallRef>(*this,lib,"getScale",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getScale >::invoke")
		->args({"self"});
	using _method_109 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getOrigin >;
	addExtern<DAS_CALL_METHOD(_method_109), SimNode_ExtFuncCallRef>(*this,lib,"getOrigin",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getOrigin >::invoke")
		->args({"self"});
	using _method_110 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_110)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_111 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_111)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::move >::invoke")
		->args({"self","offset"});
	using _method_112 = das::das_call_member< void (sf::Transformable::*)(float),&sf::Transformable::rotate >;
	addExtern<DAS_CALL_METHOD(_method_112)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float) , &sf::Transformable::rotate >::invoke")
		->args({"self","angle"});
	using _method_113 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_113)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::scale >::invoke")
		->args({"self","factorX","factorY"});
	using _method_114 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_114)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::scale >::invoke")
		->args({"self","factor"});
	using _method_115 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_115), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getTransform >::invoke")
		->args({"self"});
	using _method_116 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_116), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getInverseTransform >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Color>(*this,lib,"Color","sf::Color");
	addCtorAndUsing<sf::Color,unsigned char,unsigned char,unsigned char,unsigned char>(*this,lib,"Color","sf::Color")
		->args({"red","green","blue","alpha"});
	addCtorAndUsing<sf::Color,unsigned int>(*this,lib,"Color","sf::Color")
		->args({"color"});
	using _method_117 = das::das_call_member< unsigned int (sf::Color::*)() const,&sf::Color::toInteger >;
	addExtern<DAS_CALL_METHOD(_method_117)>(*this,lib,"toInteger",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Color::*)() const , &sf::Color::toInteger >::invoke")
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
	using _method_118 = das::das_call_member< std::size_t (sf::VertexArray::*)() const,&sf::VertexArray::getVertexCount >;
	addExtern<DAS_CALL_METHOD(_method_118)>(*this,lib,"getVertexCount",SideEffects::worstDefault,"das_call_member< std::size_t (sf::VertexArray::*)() const , &sf::VertexArray::getVertexCount >::invoke")
		->args({"self"});
	using _method_119 = das::das_call_member< sf::Vertex & (sf::VertexArray::*)(std::size_t),&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_119), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< sf::Vertex & (sf::VertexArray::*)(std::size_t) , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_120 = das::das_call_member< const sf::Vertex & (sf::VertexArray::*)(std::size_t) const,&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_120), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< const sf::Vertex & (sf::VertexArray::*)(std::size_t) const , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_121 = das::das_call_member< void (sf::VertexArray::*)(),&sf::VertexArray::clear >;
	addExtern<DAS_CALL_METHOD(_method_121)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)() , &sf::VertexArray::clear >::invoke")
		->args({"self"});
	using _method_122 = das::das_call_member< void (sf::VertexArray::*)(std::size_t),&sf::VertexArray::resize >;
	addExtern<DAS_CALL_METHOD(_method_122)>(*this,lib,"resize",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(std::size_t) , &sf::VertexArray::resize >::invoke")
		->args({"self","vertexCount"});
	using _method_123 = das::das_call_member< void (sf::VertexArray::*)(const sf::Vertex &),&sf::VertexArray::append >;
	addExtern<DAS_CALL_METHOD(_method_123)>(*this,lib,"append",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(const sf::Vertex &) , &sf::VertexArray::append >::invoke")
		->args({"self","vertex"});
	using _method_124 = das::das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType),&sf::VertexArray::setPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_124)>(*this,lib,"setPrimitiveType",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType) , &sf::VertexArray::setPrimitiveType >::invoke")
		->args({"self","type"});
	using _method_125 = das::das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const,&sf::VertexArray::getPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_125)>(*this,lib,"getPrimitiveType",SideEffects::worstDefault,"das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const , &sf::VertexArray::getPrimitiveType >::invoke")
		->args({"self"});
	using _method_126 = das::das_call_member< sf::Rect<float> (sf::VertexArray::*)() const,&sf::VertexArray::getBounds >;
	addExtern<DAS_CALL_METHOD(_method_126)>(*this,lib,"getBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::VertexArray::*)() const , &sf::VertexArray::getBounds >::invoke")
		->args({"self"});
	using _method_127 = das::das_call_member< void (sf::Shape::*)(const sf::Texture *,bool),&sf::Shape::setTexture >;
	addExtern<DAS_CALL_METHOD(_method_127)>(*this,lib,"setTexture",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Texture *,bool) , &sf::Shape::setTexture >::invoke")
		->args({"self","texture","resetRect"})
		->arg_init(2,make_smart<ExprConstBool>(false));
	using _method_128 = das::das_call_member< void (sf::Shape::*)(const sf::Rect<int> &),&sf::Shape::setTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_128)>(*this,lib,"setTextureRect",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Rect<int> &) , &sf::Shape::setTextureRect >::invoke")
		->args({"self","rect"});
	using _method_129 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setFillColor >;
	addExtern<DAS_CALL_METHOD(_method_129)>(*this,lib,"setFillColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setFillColor >::invoke")
		->args({"self","color"});
	using _method_130 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_130)>(*this,lib,"setOutlineColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setOutlineColor >::invoke")
		->args({"self","color"});
	using _method_131 = das::das_call_member< void (sf::Shape::*)(float),&sf::Shape::setOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_131)>(*this,lib,"setOutlineThickness",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(float) , &sf::Shape::setOutlineThickness >::invoke")
		->args({"self","thickness"});
	using _method_132 = das::das_call_member< const sf::Texture * (sf::Shape::*)() const,&sf::Shape::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_132)>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture * (sf::Shape::*)() const , &sf::Shape::getTexture >::invoke")
		->args({"self"});
	using _method_133 = das::das_call_member< const sf::Rect<int> & (sf::Shape::*)() const,&sf::Shape::getTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_133), SimNode_ExtFuncCallRef>(*this,lib,"getTextureRect",SideEffects::worstDefault,"das_call_member< const sf::Rect<int> & (sf::Shape::*)() const , &sf::Shape::getTextureRect >::invoke")
		->args({"self"});
	using _method_134 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getFillColor >;
	addExtern<DAS_CALL_METHOD(_method_134), SimNode_ExtFuncCallRef>(*this,lib,"getFillColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getFillColor >::invoke")
		->args({"self"});
	using _method_135 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_135), SimNode_ExtFuncCallRef>(*this,lib,"getOutlineColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getOutlineColor >::invoke")
		->args({"self"});
	using _method_136 = das::das_call_member< float (sf::Shape::*)() const,&sf::Shape::getOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_136)>(*this,lib,"getOutlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Shape::*)() const , &sf::Shape::getOutlineThickness >::invoke")
		->args({"self"});
	using _method_137 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getLocalBounds >;
	addExtern<DAS_CALL_METHOD(_method_137)>(*this,lib,"getLocalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getLocalBounds >::invoke")
		->args({"self"});
	using _method_138 = das::das_call_member< sf::Rect<float> (sf::Shape::*)() const,&sf::Shape::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_138)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Shape::*)() const , &sf::Shape::getGlobalBounds >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::CircleShape,float,unsigned long long>(*this,lib,"CircleShape","sf::CircleShape")
		->args({"radius","pointCount"})
		->arg_init(0,make_smart<ExprConstFloat>(0.00000000000000000))
		->arg_init(1,make_smart<ExprConstUInt64>(0x1e));
	using _method_139 = das::das_call_member< void (sf::CircleShape::*)(float),&sf::CircleShape::setRadius >;
	addExtern<DAS_CALL_METHOD(_method_139)>(*this,lib,"setRadius",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(float) , &sf::CircleShape::setRadius >::invoke")
		->args({"self","radius"});
	using _method_140 = das::das_call_member< float (sf::CircleShape::*)() const,&sf::CircleShape::getRadius >;
	addExtern<DAS_CALL_METHOD(_method_140)>(*this,lib,"getRadius",SideEffects::worstDefault,"das_call_member< float (sf::CircleShape::*)() const , &sf::CircleShape::getRadius >::invoke")
		->args({"self"});
	using _method_141 = das::das_call_member< void (sf::CircleShape::*)(std::size_t),&sf::CircleShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_141)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::CircleShape::*)(std::size_t) , &sf::CircleShape::setPointCount >::invoke")
		->args({"self","count"});
	addExtern< std::size_t (*)(const sf::CircleShape &) , _dasSFML_virtual_41_getPointCount >(*this,lib,"getPointCount",SideEffects::worstDefault,"_dasSFML_virtual_41_getPointCount")
		->args({"self"});
	addExtern< sf::Vector2<float> (*)(const sf::CircleShape &,std::size_t) , _dasSFML_virtual_42_getPoint >(*this,lib,"getPoint",SideEffects::worstDefault,"_dasSFML_virtual_42_getPoint")
		->args({"self","index"});
	addCtorAndUsing<sf::ConvexShape,unsigned long long>(*this,lib,"ConvexShape","sf::ConvexShape")
		->args({"pointCount"})
		->arg_init(0,make_smart<ExprConstUInt64>(0x0));
	using _method_144 = das::das_call_member< void (sf::ConvexShape::*)(std::size_t),&sf::ConvexShape::setPointCount >;
	addExtern<DAS_CALL_METHOD(_method_144)>(*this,lib,"setPointCount",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(std::size_t) , &sf::ConvexShape::setPointCount >::invoke")
		->args({"self","count"});
	addExtern< std::size_t (*)(const sf::ConvexShape &) , _dasSFML_virtual_43_getPointCount >(*this,lib,"getPointCount",SideEffects::worstDefault,"_dasSFML_virtual_43_getPointCount")
		->args({"self"});
	using _method_146 = das::das_call_member< void (sf::ConvexShape::*)(std::size_t,const sf::Vector2<float> &),&sf::ConvexShape::setPoint >;
	addExtern<DAS_CALL_METHOD(_method_146)>(*this,lib,"setPoint",SideEffects::worstDefault,"das_call_member< void (sf::ConvexShape::*)(std::size_t,const sf::Vector2<float> &) , &sf::ConvexShape::setPoint >::invoke")
		->args({"self","index","point"});
	addExtern< sf::Vector2<float> (*)(const sf::ConvexShape &,std::size_t) , _dasSFML_virtual_44_getPoint >(*this,lib,"getPoint",SideEffects::worstDefault,"_dasSFML_virtual_44_getPoint")
		->args({"self","index"});
	addCtorAndUsing<sf::Glyph>(*this,lib,"Glyph","sf::Glyph");
	addCtorAndUsing<sf::Image>(*this,lib,"Image","sf::Image");
	using _method_148 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_148)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const sf::Color &) , &sf::Image::create >::invoke")
		->args({"self","width","height","color"});
	using _method_149 = das::das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *),&sf::Image::create >;
	addExtern<DAS_CALL_METHOD(_method_149)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< void (sf::Image::*)(unsigned int,unsigned int,const unsigned char *) , &sf::Image::create >::invoke")
		->args({"self","width","height","pixels"});
	using _method_150 = das::das_call_member< bool (sf::Image::*)(const std::basic_string<char> &),&sf::Image::loadFromFile >;
	addExtern<DAS_CALL_METHOD(_method_150)>(*this,lib,"loadFromFile",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const std::basic_string<char> &) , &sf::Image::loadFromFile >::invoke")
		->args({"self","filename"});
	using _method_151 = das::das_call_member< bool (sf::Image::*)(const void *,std::size_t),&sf::Image::loadFromMemory >;
	addExtern<DAS_CALL_METHOD(_method_151)>(*this,lib,"loadFromMemory",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(const void *,std::size_t) , &sf::Image::loadFromMemory >::invoke")
		->args({"self","data","size"});
	using _method_152 = das::das_call_member< bool (sf::Image::*)(sf::InputStream &),&sf::Image::loadFromStream >;
	addExtern<DAS_CALL_METHOD(_method_152)>(*this,lib,"loadFromStream",SideEffects::worstDefault,"das_call_member< bool (sf::Image::*)(sf::InputStream &) , &sf::Image::loadFromStream >::invoke")
		->args({"self","stream"});
}
}

