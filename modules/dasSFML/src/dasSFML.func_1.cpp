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
void Module_dasSFML::initFunctions_1() {
	using _method_1 = das::das_call_member< float (sf::Time::*)() const,&sf::Time::asSeconds >;
	addExtern<DAS_CALL_METHOD(_method_1)>(*this,lib,"asSeconds",SideEffects::worstDefault,"das_call_member< float (sf::Time::*)() const , &sf::Time::asSeconds >::invoke")
		->args({"self"});
	using _method_2 = das::das_call_member< int (sf::Time::*)() const,&sf::Time::asMilliseconds >;
	addExtern<DAS_CALL_METHOD(_method_2)>(*this,lib,"asMilliseconds",SideEffects::worstDefault,"das_call_member< int (sf::Time::*)() const , &sf::Time::asMilliseconds >::invoke")
		->args({"self"});
	using _method_3 = das::das_call_member< long long (sf::Time::*)() const,&sf::Time::asMicroseconds >;
	addExtern<DAS_CALL_METHOD(_method_3)>(*this,lib,"asMicroseconds",SideEffects::worstDefault,"das_call_member< long long (sf::Time::*)() const , &sf::Time::asMicroseconds >::invoke")
		->args({"self"});
	addExtern< sf::Time (*)(float) , sf::seconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"seconds",SideEffects::worstDefault,"sf::seconds")
		->args({"amount"});
	addExtern< sf::Time (*)(int) , sf::milliseconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"milliseconds",SideEffects::worstDefault,"sf::milliseconds")
		->args({"amount"});
	addExtern< sf::Time (*)(long long) , sf::microseconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"microseconds",SideEffects::worstDefault,"sf::microseconds")
		->args({"amount"});
	using _method_4 = das::das_call_member< sf::Time (sf::Clock::*)() const,&sf::Clock::getElapsedTime >;
	addExtern<DAS_CALL_METHOD(_method_4),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getElapsedTime",SideEffects::worstDefault,"das_call_member< sf::Time (sf::Clock::*)() const , &sf::Clock::getElapsedTime >::invoke")
		->args({"self"});
	using _method_5 = das::das_call_member< sf::Time (sf::Clock::*)(),&sf::Clock::restart >;
	addExtern<DAS_CALL_METHOD(_method_5),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"restart",SideEffects::worstDefault,"das_call_member< sf::Time (sf::Clock::*)() , &sf::Clock::restart >::invoke")
		->args({"self"});
	using _method_6 = das::das_call_member< bool (sf::FileInputStream::*)(const std::basic_string<char> &),&sf::FileInputStream::open >;
	addExtern<DAS_CALL_METHOD(_method_6)>(*this,lib,"open",SideEffects::worstDefault,"das_call_member< bool (sf::FileInputStream::*)(const std::basic_string<char> &) , &sf::FileInputStream::open >::invoke")
		->args({"self","filename"});
	using _method_7 = das::das_call_member< void (sf::MemoryInputStream::*)(const void *,unsigned long long),&sf::MemoryInputStream::open >;
	addExtern<DAS_CALL_METHOD(_method_7)>(*this,lib,"open",SideEffects::worstDefault,"das_call_member< void (sf::MemoryInputStream::*)(const void *,unsigned long long) , &sf::MemoryInputStream::open >::invoke")
		->args({"self","data","sizeInBytes"});
	using _method_8 = das::das_call_member< void (sf::Mutex::*)(),&sf::Mutex::lock >;
	addExtern<DAS_CALL_METHOD(_method_8)>(*this,lib,"lock",SideEffects::worstDefault,"das_call_member< void (sf::Mutex::*)() , &sf::Mutex::lock >::invoke")
		->args({"self"});
	using _method_9 = das::das_call_member< void (sf::Mutex::*)(),&sf::Mutex::unlock >;
	addExtern<DAS_CALL_METHOD(_method_9)>(*this,lib,"unlock",SideEffects::worstDefault,"das_call_member< void (sf::Mutex::*)() , &sf::Mutex::unlock >::invoke")
		->args({"self"});
	addExtern< void (*)(sf::Time) , sf::sleep >(*this,lib,"sleep",SideEffects::worstDefault,"sf::sleep")
		->args({"duration"});
	using _method_10 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::launch >;
	addExtern<DAS_CALL_METHOD(_method_10)>(*this,lib,"launch",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::launch >::invoke")
		->args({"self"});
	using _method_11 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::wait >;
	addExtern<DAS_CALL_METHOD(_method_11)>(*this,lib,"wait",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::wait >::invoke")
		->args({"self"});
	using _method_12 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::terminate >;
	addExtern<DAS_CALL_METHOD(_method_12)>(*this,lib,"terminate",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::terminate >::invoke")
		->args({"self"});
	using _method_13 = das::das_call_member< void (sf::ThreadLocal::*)(void *),&sf::ThreadLocal::setValue >;
	addExtern<DAS_CALL_METHOD(_method_13)>(*this,lib,"setValue",SideEffects::worstDefault,"das_call_member< void (sf::ThreadLocal::*)(void *) , &sf::ThreadLocal::setValue >::invoke")
		->args({"self","value"});
	using _method_14 = das::das_call_member< void * (sf::ThreadLocal::*)() const,&sf::ThreadLocal::getValue >;
	addExtern<DAS_CALL_METHOD(_method_14)>(*this,lib,"getValue",SideEffects::worstDefault,"das_call_member< void * (sf::ThreadLocal::*)() const , &sf::ThreadLocal::getValue >::invoke")
		->args({"self"});
	using _method_15 = das::das_call_member< bool (sf::Context::*)(bool),&sf::Context::setActive >;
	addExtern<DAS_CALL_METHOD(_method_15)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Context::*)(bool) , &sf::Context::setActive >::invoke")
		->args({"self","active"});
	using _method_16 = das::das_call_member< const sf::ContextSettings & (sf::Context::*)() const,&sf::Context::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_16), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Context::*)() const , &sf::Context::getSettings >::invoke")
		->args({"self"});
	using _method_17 = das::das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>),&sf::Cursor::loadFromPixels >;
	addExtern<DAS_CALL_METHOD(_method_17)>(*this,lib,"loadFromPixels",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>) , &sf::Cursor::loadFromPixels >::invoke")
		->args({"self","pixels","size","hotspot"});
	using _method_18 = das::das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type),&sf::Cursor::loadFromSystem >;
	addExtern<DAS_CALL_METHOD(_method_18)>(*this,lib,"loadFromSystem",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type) , &sf::Cursor::loadFromSystem >::invoke")
		->args({"self","type"});
	using _method_19 = das::das_call_member< bool (sf::VideoMode::*)() const,&sf::VideoMode::isValid >;
	addExtern<DAS_CALL_METHOD(_method_19)>(*this,lib,"isValid",SideEffects::worstDefault,"das_call_member< bool (sf::VideoMode::*)() const , &sf::VideoMode::isValid >::invoke")
		->args({"self"});
	using _method_20 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::isOpen >;
	addExtern<DAS_CALL_METHOD(_method_20)>(*this,lib,"isOpen",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::isOpen >::invoke")
		->args({"self"});
	using _method_21 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::pollEvent >;
	addExtern<DAS_CALL_METHOD(_method_21)>(*this,lib,"pollEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::pollEvent >::invoke")
		->args({"self","event"});
	using _method_22 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::waitEvent >;
	addExtern<DAS_CALL_METHOD(_method_22)>(*this,lib,"waitEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::waitEvent >::invoke")
		->args({"self","event"});
	using _method_23 = das::das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const,&sf::WindowBase::getPosition >;
	addExtern<DAS_CALL_METHOD(_method_23)>(*this,lib,"getPosition",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const , &sf::WindowBase::getPosition >::invoke")
		->args({"self"});
	using _method_24 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &),&sf::WindowBase::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_24)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &) , &sf::WindowBase::setPosition >::invoke")
		->args({"self","position"});
	using _method_25 = das::das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const,&sf::WindowBase::getSize >;
	addExtern<DAS_CALL_METHOD(_method_25)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const , &sf::WindowBase::getSize >::invoke")
		->args({"self"});
	using _method_26 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &),&sf::WindowBase::setSize >;
	addExtern<DAS_CALL_METHOD(_method_26)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &) , &sf::WindowBase::setSize >::invoke")
		->args({"self","size"});
	using _method_27 = das::das_call_member< void (sf::WindowBase::*)(const sf::String &),&sf::WindowBase::setTitle >;
	addExtern<DAS_CALL_METHOD(_method_27)>(*this,lib,"setTitle",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::String &) , &sf::WindowBase::setTitle >::invoke")
		->args({"self","title"});
	using _method_28 = das::das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *),&sf::WindowBase::setIcon >;
	addExtern<DAS_CALL_METHOD(_method_28)>(*this,lib,"setIcon",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *) , &sf::WindowBase::setIcon >::invoke")
		->args({"self","width","height","pixels"});
	using _method_29 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setVisible >;
	addExtern<DAS_CALL_METHOD(_method_29)>(*this,lib,"setVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setVisible >::invoke")
		->args({"self","visible"});
	using _method_30 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorVisible >;
	addExtern<DAS_CALL_METHOD(_method_30)>(*this,lib,"setMouseCursorVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorVisible >::invoke")
		->args({"self","visible"});
	using _method_31 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorGrabbed >;
	addExtern<DAS_CALL_METHOD(_method_31)>(*this,lib,"setMouseCursorGrabbed",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorGrabbed >::invoke")
		->args({"self","grabbed"});
	using _method_32 = das::das_call_member< void (sf::WindowBase::*)(const sf::Cursor &),&sf::WindowBase::setMouseCursor >;
	addExtern<DAS_CALL_METHOD(_method_32)>(*this,lib,"setMouseCursor",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Cursor &) , &sf::WindowBase::setMouseCursor >::invoke")
		->args({"self","cursor"});
	using _method_33 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setKeyRepeatEnabled >;
	addExtern<DAS_CALL_METHOD(_method_33)>(*this,lib,"setKeyRepeatEnabled",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setKeyRepeatEnabled >::invoke")
		->args({"self","enabled"});
	using _method_34 = das::das_call_member< void (sf::WindowBase::*)(float),&sf::WindowBase::setJoystickThreshold >;
	addExtern<DAS_CALL_METHOD(_method_34)>(*this,lib,"setJoystickThreshold",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(float) , &sf::WindowBase::setJoystickThreshold >::invoke")
		->args({"self","threshold"});
	using _method_35 = das::das_call_member< void (sf::WindowBase::*)(),&sf::WindowBase::requestFocus >;
	addExtern<DAS_CALL_METHOD(_method_35)>(*this,lib,"requestFocus",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)() , &sf::WindowBase::requestFocus >::invoke")
		->args({"self"});
	using _method_36 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::hasFocus >;
	addExtern<DAS_CALL_METHOD(_method_36)>(*this,lib,"hasFocus",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::hasFocus >::invoke")
		->args({"self"});
	using _method_37 = das::das_call_member< HWND__ * (sf::WindowBase::*)() const,&sf::WindowBase::getSystemHandle >;
	addExtern<DAS_CALL_METHOD(_method_37)>(*this,lib,"getSystemHandle",SideEffects::worstDefault,"das_call_member< HWND__ * (sf::WindowBase::*)() const , &sf::WindowBase::getSystemHandle >::invoke")
		->args({"self"});
	using _method_38 = das::das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *),&sf::WindowBase::createVulkanSurface >;
	addExtern<DAS_CALL_METHOD(_method_38)>(*this,lib,"createVulkanSurface",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *) , &sf::WindowBase::createVulkanSurface >::invoke")
		->args({"self","instance","surface","allocator"})
		->arg_init(3,make_smart<ExprConstPtr>());
	using _method_39 = das::das_call_member< const sf::ContextSettings & (sf::Window::*)() const,&sf::Window::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_39), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Window::*)() const , &sf::Window::getSettings >::invoke")
		->args({"self"});
	using _method_40 = das::das_call_member< void (sf::Window::*)(bool),&sf::Window::setVerticalSyncEnabled >;
	addExtern<DAS_CALL_METHOD(_method_40)>(*this,lib,"setVerticalSyncEnabled",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(bool) , &sf::Window::setVerticalSyncEnabled >::invoke")
		->args({"self","enabled"});
	using _method_41 = das::das_call_member< void (sf::Window::*)(unsigned int),&sf::Window::setFramerateLimit >;
	addExtern<DAS_CALL_METHOD(_method_41)>(*this,lib,"setFramerateLimit",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(unsigned int) , &sf::Window::setFramerateLimit >::invoke")
		->args({"self","limit"});
	using _method_42 = das::das_call_member< bool (sf::Window::*)(bool) const,&sf::Window::setActive >;
	addExtern<DAS_CALL_METHOD(_method_42)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Window::*)(bool) const , &sf::Window::setActive >::invoke")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_43 = das::das_call_member< void (sf::Window::*)(),&sf::Window::display >;
	addExtern<DAS_CALL_METHOD(_method_43)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)() , &sf::Window::display >::invoke")
		->args({"self"});
	using _method_44 = das::das_call_member< const float * (sf::Transform::*)() const,&sf::Transform::getMatrix >;
	addExtern<DAS_CALL_METHOD(_method_44)>(*this,lib,"getMatrix",SideEffects::worstDefault,"das_call_member< const float * (sf::Transform::*)() const , &sf::Transform::getMatrix >::invoke")
		->args({"self"});
	using _method_45 = das::das_call_member< sf::Transform (sf::Transform::*)() const,&sf::Transform::getInverse >;
	addExtern<DAS_CALL_METHOD(_method_45),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getInverse",SideEffects::worstDefault,"das_call_member< sf::Transform (sf::Transform::*)() const , &sf::Transform::getInverse >::invoke")
		->args({"self"});
	using _method_46 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_46)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","x","y"});
	using _method_47 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_47)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","point"});
	using _method_48 = das::das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const,&sf::Transform::transformRect >;
	addExtern<DAS_CALL_METHOD(_method_48)>(*this,lib,"transformRect",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const , &sf::Transform::transformRect >::invoke")
		->args({"self","rectangle"});
	using _method_49 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &),&sf::Transform::combine >;
	addExtern<DAS_CALL_METHOD(_method_49), SimNode_ExtFuncCallRef>(*this,lib,"combine",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &) , &sf::Transform::combine >::invoke")
		->args({"self","transform"});
	using _method_50 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_50), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::translate >::invoke")
		->args({"self","x","y"});
	using _method_51 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_51), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::translate >::invoke")
		->args({"self","offset"});
	using _method_52 = das::das_call_member< sf::Transform & (sf::Transform::*)(float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_52), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle"});
	using _method_53 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_53), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","centerX","centerY"});
	using _method_54 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_54), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","center"});
	using _method_55 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_55), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY"});
	using _method_56 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_56), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY","centerX","centerY"});
	using _method_57 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_57), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors"});
	using _method_58 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_58), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors","center"});
	using _method_59 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_59)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setPosition >::invoke")
		->args({"self","x","y"});
	using _method_60 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_60)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setPosition >::invoke")
		->args({"self","position"});
	using _method_61 = das::das_call_member< void (sf::Transformable::*)(float),&sf::Transformable::setRotation >;
	addExtern<DAS_CALL_METHOD(_method_61)>(*this,lib,"setRotation",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float) , &sf::Transformable::setRotation >::invoke")
		->args({"self","angle"});
	using _method_62 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setScale >;
	addExtern<DAS_CALL_METHOD(_method_62)>(*this,lib,"setScale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setScale >::invoke")
		->args({"self","factorX","factorY"});
	using _method_63 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setScale >;
	addExtern<DAS_CALL_METHOD(_method_63)>(*this,lib,"setScale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setScale >::invoke")
		->args({"self","factors"});
	using _method_64 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::setOrigin >;
	addExtern<DAS_CALL_METHOD(_method_64)>(*this,lib,"setOrigin",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::setOrigin >::invoke")
		->args({"self","x","y"});
	using _method_65 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::setOrigin >;
	addExtern<DAS_CALL_METHOD(_method_65)>(*this,lib,"setOrigin",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::setOrigin >::invoke")
		->args({"self","origin"});
	using _method_66 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getPosition >;
	addExtern<DAS_CALL_METHOD(_method_66), SimNode_ExtFuncCallRef>(*this,lib,"getPosition",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getPosition >::invoke")
		->args({"self"});
	using _method_67 = das::das_call_member< float (sf::Transformable::*)() const,&sf::Transformable::getRotation >;
	addExtern<DAS_CALL_METHOD(_method_67)>(*this,lib,"getRotation",SideEffects::worstDefault,"das_call_member< float (sf::Transformable::*)() const , &sf::Transformable::getRotation >::invoke")
		->args({"self"});
	using _method_68 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getScale >;
	addExtern<DAS_CALL_METHOD(_method_68), SimNode_ExtFuncCallRef>(*this,lib,"getScale",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getScale >::invoke")
		->args({"self"});
	using _method_69 = das::das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const,&sf::Transformable::getOrigin >;
	addExtern<DAS_CALL_METHOD(_method_69), SimNode_ExtFuncCallRef>(*this,lib,"getOrigin",SideEffects::worstDefault,"das_call_member< const sf::Vector2<float> & (sf::Transformable::*)() const , &sf::Transformable::getOrigin >::invoke")
		->args({"self"});
	using _method_70 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_70)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::move >::invoke")
		->args({"self","offsetX","offsetY"});
	using _method_71 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::move >;
	addExtern<DAS_CALL_METHOD(_method_71)>(*this,lib,"move",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::move >::invoke")
		->args({"self","offset"});
	using _method_72 = das::das_call_member< void (sf::Transformable::*)(float),&sf::Transformable::rotate >;
	addExtern<DAS_CALL_METHOD(_method_72)>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float) , &sf::Transformable::rotate >::invoke")
		->args({"self","angle"});
	using _method_73 = das::das_call_member< void (sf::Transformable::*)(float,float),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_73)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(float,float) , &sf::Transformable::scale >::invoke")
		->args({"self","factorX","factorY"});
	using _method_74 = das::das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &),&sf::Transformable::scale >;
	addExtern<DAS_CALL_METHOD(_method_74)>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< void (sf::Transformable::*)(const sf::Vector2<float> &) , &sf::Transformable::scale >::invoke")
		->args({"self","factor"});
	using _method_75 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getTransform >;
	addExtern<DAS_CALL_METHOD(_method_75), SimNode_ExtFuncCallRef>(*this,lib,"getTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getTransform >::invoke")
		->args({"self"});
	using _method_76 = das::das_call_member< const sf::Transform & (sf::Transformable::*)() const,&sf::Transformable::getInverseTransform >;
	addExtern<DAS_CALL_METHOD(_method_76), SimNode_ExtFuncCallRef>(*this,lib,"getInverseTransform",SideEffects::worstDefault,"das_call_member< const sf::Transform & (sf::Transformable::*)() const , &sf::Transformable::getInverseTransform >::invoke")
		->args({"self"});
	using _method_77 = das::das_call_member< unsigned int (sf::Color::*)() const,&sf::Color::toInteger >;
	addExtern<DAS_CALL_METHOD(_method_77)>(*this,lib,"toInteger",SideEffects::worstDefault,"das_call_member< unsigned int (sf::Color::*)() const , &sf::Color::toInteger >::invoke")
		->args({"self"});
	using _method_78 = das::das_call_member< unsigned long long (sf::VertexArray::*)() const,&sf::VertexArray::getVertexCount >;
	addExtern<DAS_CALL_METHOD(_method_78)>(*this,lib,"getVertexCount",SideEffects::worstDefault,"das_call_member< unsigned long long (sf::VertexArray::*)() const , &sf::VertexArray::getVertexCount >::invoke")
		->args({"self"});
	using _method_79 = das::das_call_member< sf::Vertex & (sf::VertexArray::*)(unsigned long long),&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_79), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< sf::Vertex & (sf::VertexArray::*)(unsigned long long) , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_80 = das::das_call_member< const sf::Vertex & (sf::VertexArray::*)(unsigned long long) const,&sf::VertexArray::operator[] >;
	addExtern<DAS_CALL_METHOD(_method_80), SimNode_ExtFuncCallRef>(*this,lib,"[]",SideEffects::worstDefault,"das_call_member< const sf::Vertex & (sf::VertexArray::*)(unsigned long long) const , &sf::VertexArray::operator[] >::invoke")
		->args({"self","index"});
	using _method_81 = das::das_call_member< void (sf::VertexArray::*)(),&sf::VertexArray::clear >;
	addExtern<DAS_CALL_METHOD(_method_81)>(*this,lib,"clear",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)() , &sf::VertexArray::clear >::invoke")
		->args({"self"});
	using _method_82 = das::das_call_member< void (sf::VertexArray::*)(unsigned long long),&sf::VertexArray::resize >;
	addExtern<DAS_CALL_METHOD(_method_82)>(*this,lib,"resize",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(unsigned long long) , &sf::VertexArray::resize >::invoke")
		->args({"self","vertexCount"});
	using _method_83 = das::das_call_member< void (sf::VertexArray::*)(const sf::Vertex &),&sf::VertexArray::append >;
	addExtern<DAS_CALL_METHOD(_method_83)>(*this,lib,"append",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(const sf::Vertex &) , &sf::VertexArray::append >::invoke")
		->args({"self","vertex"});
	using _method_84 = das::das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType),&sf::VertexArray::setPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_84)>(*this,lib,"setPrimitiveType",SideEffects::worstDefault,"das_call_member< void (sf::VertexArray::*)(sf::PrimitiveType) , &sf::VertexArray::setPrimitiveType >::invoke")
		->args({"self","type"});
	using _method_85 = das::das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const,&sf::VertexArray::getPrimitiveType >;
	addExtern<DAS_CALL_METHOD(_method_85)>(*this,lib,"getPrimitiveType",SideEffects::worstDefault,"das_call_member< sf::PrimitiveType (sf::VertexArray::*)() const , &sf::VertexArray::getPrimitiveType >::invoke")
		->args({"self"});
	using _method_86 = das::das_call_member< sf::Rect<float> (sf::VertexArray::*)() const,&sf::VertexArray::getBounds >;
	addExtern<DAS_CALL_METHOD(_method_86)>(*this,lib,"getBounds",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::VertexArray::*)() const , &sf::VertexArray::getBounds >::invoke")
		->args({"self"});
	using _method_87 = das::das_call_member< void (sf::Shape::*)(const sf::Texture *,bool),&sf::Shape::setTexture >;
	addExtern<DAS_CALL_METHOD(_method_87)>(*this,lib,"setTexture",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Texture *,bool) , &sf::Shape::setTexture >::invoke")
		->args({"self","texture","resetRect"})
		->arg_init(2,make_smart<ExprConstBool>(false));
	using _method_88 = das::das_call_member< void (sf::Shape::*)(const sf::Rect<int> &),&sf::Shape::setTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_88)>(*this,lib,"setTextureRect",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Rect<int> &) , &sf::Shape::setTextureRect >::invoke")
		->args({"self","rect"});
	using _method_89 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setFillColor >;
	addExtern<DAS_CALL_METHOD(_method_89)>(*this,lib,"setFillColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setFillColor >::invoke")
		->args({"self","color"});
	using _method_90 = das::das_call_member< void (sf::Shape::*)(const sf::Color &),&sf::Shape::setOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_90)>(*this,lib,"setOutlineColor",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(const sf::Color &) , &sf::Shape::setOutlineColor >::invoke")
		->args({"self","color"});
	using _method_91 = das::das_call_member< void (sf::Shape::*)(float),&sf::Shape::setOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_91)>(*this,lib,"setOutlineThickness",SideEffects::worstDefault,"das_call_member< void (sf::Shape::*)(float) , &sf::Shape::setOutlineThickness >::invoke")
		->args({"self","thickness"});
	using _method_92 = das::das_call_member< const sf::Texture * (sf::Shape::*)() const,&sf::Shape::getTexture >;
	addExtern<DAS_CALL_METHOD(_method_92)>(*this,lib,"getTexture",SideEffects::worstDefault,"das_call_member< const sf::Texture * (sf::Shape::*)() const , &sf::Shape::getTexture >::invoke")
		->args({"self"});
	using _method_93 = das::das_call_member< const sf::Rect<int> & (sf::Shape::*)() const,&sf::Shape::getTextureRect >;
	addExtern<DAS_CALL_METHOD(_method_93), SimNode_ExtFuncCallRef>(*this,lib,"getTextureRect",SideEffects::worstDefault,"das_call_member< const sf::Rect<int> & (sf::Shape::*)() const , &sf::Shape::getTextureRect >::invoke")
		->args({"self"});
	using _method_94 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getFillColor >;
	addExtern<DAS_CALL_METHOD(_method_94), SimNode_ExtFuncCallRef>(*this,lib,"getFillColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getFillColor >::invoke")
		->args({"self"});
	using _method_95 = das::das_call_member< const sf::Color & (sf::Shape::*)() const,&sf::Shape::getOutlineColor >;
	addExtern<DAS_CALL_METHOD(_method_95), SimNode_ExtFuncCallRef>(*this,lib,"getOutlineColor",SideEffects::worstDefault,"das_call_member< const sf::Color & (sf::Shape::*)() const , &sf::Shape::getOutlineColor >::invoke")
		->args({"self"});
	using _method_96 = das::das_call_member< float (sf::Shape::*)() const,&sf::Shape::getOutlineThickness >;
	addExtern<DAS_CALL_METHOD(_method_96)>(*this,lib,"getOutlineThickness",SideEffects::worstDefault,"das_call_member< float (sf::Shape::*)() const , &sf::Shape::getOutlineThickness >::invoke")
		->args({"self"});
}
}

