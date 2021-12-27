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
void Module_dasSFML::initFunctions_5() {
	using _method_331 = das::das_call_member< sf::Rect<float> (sf::Text::*)() const,&sf::Text::getGlobalBounds >;
	addExtern<DAS_CALL_METHOD(_method_331)>(*this,lib,"getGlobalBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Text::*)() const , &sf::Text::getGlobalBounds >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::VertexBuffer>(*this,lib,"VertexBuffer","sf::VertexBuffer");
	addCtorAndUsing<sf::VertexBuffer,sf::PrimitiveType>(*this,lib,"VertexBuffer","sf::VertexBuffer")
		->args({"type"});
	addCtorAndUsing<sf::VertexBuffer,sf::VertexBuffer::Usage>(*this,lib,"VertexBuffer","sf::VertexBuffer")
		->args({"usage"});
	addCtorAndUsing<sf::VertexBuffer,sf::PrimitiveType,sf::VertexBuffer::Usage>(*this,lib,"VertexBuffer","sf::VertexBuffer")
		->args({"type","usage"});
	addCtorAndUsing<sf::VertexBuffer,const sf::VertexBuffer &>(*this,lib,"VertexBuffer","sf::VertexBuffer")
		->args({"copy"});
	using _method_332 = das::das_call_member< bool (sf::VertexBuffer::*)(std::size_t),&sf::VertexBuffer::create >;
	addExtern<DAS_CALL_METHOD(_method_332)>(*this,lib,"create",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(std::size_t) , &sf::VertexBuffer::create >::invoke")
		->args({"self","vertexCount"});
	using _method_333 = das::das_call_member< std::size_t (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getVertexCount >;
	addExtern<DAS_CALL_METHOD(_method_333)>(*this,lib,"getVertexCount",SideEffects::worstDefault,"das_call_member< std::size_t (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getVertexCount >::invoke")
		->args({"self"});
	using _method_334 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_334)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertices"});
	using _method_335 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *,std::size_t,unsigned int),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_335)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::Vertex *,std::size_t,unsigned int) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertices","vertexCount","offset"});
	using _method_336 = das::das_call_member< bool (sf::VertexBuffer::*)(const sf::VertexBuffer &),&sf::VertexBuffer::update >;
	addExtern<DAS_CALL_METHOD(_method_336)>(*this,lib,"update",SideEffects::worstDefault,"das_call_member< bool (sf::VertexBuffer::*)(const sf::VertexBuffer &) , &sf::VertexBuffer::update >::invoke")
		->args({"self","vertexBuffer"});
	using _method_337 = das::das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer &),&sf::VertexBuffer::swap >;
	addExtern<DAS_CALL_METHOD(_method_337)>(*this,lib,"swap",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer &) , &sf::VertexBuffer::swap >::invoke")
		->args({"self","right"});
	using _method_338 = das::das_call_member< unsigned int (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getNativeHandle >;
	addExtern<DAS_CALL_METHOD(_method_338)>(*this,lib,"getNativeHandle",SideEffects::worstDefault,"das_call_member< unsigned int (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getNativeHandle >::invoke")
		->args({"self"});
	using _method_339 = das::das_call_member< void (sf::VertexBuffer::*)(sf::PrimitiveType),&sf::VertexBuffer::setPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_339)>(*this,lib,"setPrimitiveType",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::PrimitiveType) , &sf::VertexBuffer::setPrimitiveType >::invoke")
		->args({"self","type"});
	using _method_340 = das::das_call_member< sf::PrimitiveType (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_340)>(*this,lib,"getPrimitiveType",SideEffects::worstDefault,"das_call_member< sf::PrimitiveType (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getPrimitiveType >::invoke")
		->args({"self"});
	using _method_341 = das::das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer::Usage),&sf::VertexBuffer::setUsage >;
	addExtern<DAS_CALL_METHOD(_method_341)>(*this,lib,"setUsage",SideEffects::worstDefault,"das_call_member< void (sf::VertexBuffer::*)(sf::VertexBuffer::Usage) , &sf::VertexBuffer::setUsage >::invoke")
		->args({"self","usage"});
	using _method_342 = das::das_call_member< sf::VertexBuffer::Usage (sf::VertexBuffer::*)() const,&sf::VertexBuffer::getUsage >;
	addExtern<DAS_CALL_METHOD(_method_342)>(*this,lib,"getUsage",SideEffects::worstDefault,"das_call_member< sf::VertexBuffer::Usage (sf::VertexBuffer::*)() const , &sf::VertexBuffer::getUsage >::invoke")
		->args({"self"});
	addExtern< void (*)(const sf::VertexBuffer &,const sf::VertexBuffer *) , _dasSFML_static_61_bind >(*this,lib,"bind",SideEffects::worstDefault,"_dasSFML_static_61_bind")
		->args({"self","vertexBuffer"});
	addExtern< bool (*)(const sf::VertexBuffer &) , _dasSFML_static_62_isAvailable >(*this,lib,"isAvailable",SideEffects::worstDefault,"_dasSFML_static_62_isAvailable")
		->args({"self"});
	addExtern< decltype(&with_interface<sf::Sprite,sf::Transformable>), with_interface<sf::Sprite,sf::Transformable> >(*this,lib,"interface",SideEffects::invoke,"with_interface<sf::Sprite,sf::Transformable>");
	addExtern< decltype(&with_interface<sf::Shape,sf::Transformable>), with_interface<sf::Shape,sf::Transformable> >(*this,lib,"interface",SideEffects::invoke,"with_interface<sf::Shape,sf::Transformable>");
	addExtern< decltype(&with_interface<sf::Text,sf::Transformable>), with_interface<sf::Text,sf::Transformable> >(*this,lib,"interface",SideEffects::invoke,"with_interface<sf::Text,sf::Transformable>");
	addExtern< decltype(&with_interface<sf::RenderWindow,sf::RenderTarget>), with_interface<sf::RenderWindow,sf::RenderTarget> >(*this,lib,"interface",SideEffects::invoke,"with_interface<sf::RenderWindow,sf::RenderTarget>");
}
}

