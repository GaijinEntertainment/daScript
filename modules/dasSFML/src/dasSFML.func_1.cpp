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
void Module_dasSFML::initFunctions_1() {
	addCtorAndUsing<sf::Time>(*this,lib,"Time","sf::Time");
	using _method_1 = das::das_call_member< float (sf::Time::*)() const,&sf::Time::asSeconds >;
	addExtern<DAS_CALL_METHOD(_method_1)>(*this,lib,"asSeconds",SideEffects::worstDefault,"das_call_member< float (sf::Time::*)() const , &sf::Time::asSeconds >::invoke")
		->args({"self"});
	using _method_2 = das::das_call_member< int (sf::Time::*)() const,&sf::Time::asMilliseconds >;
	addExtern<DAS_CALL_METHOD(_method_2)>(*this,lib,"asMilliseconds",SideEffects::worstDefault,"das_call_member< int (sf::Time::*)() const , &sf::Time::asMilliseconds >::invoke")
		->args({"self"});
	using _method_3 = das::das_call_member< sf::Int64 (sf::Time::*)() const,&sf::Time::asMicroseconds >;
	addExtern<DAS_CALL_METHOD(_method_3)>(*this,lib,"asMicroseconds",SideEffects::worstDefault,"das_call_member< sf::Int64 (sf::Time::*)() const , &sf::Time::asMicroseconds >::invoke")
		->args({"self"});
	addExtern< sf::Time (*)(float) , sf::seconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"seconds",SideEffects::worstDefault,"sf::seconds")
		->args({"amount"});
	addExtern< sf::Time (*)(int) , sf::milliseconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"milliseconds",SideEffects::worstDefault,"sf::milliseconds")
		->args({"amount"});
	addExtern< sf::Time (*)(sf::Int64) , sf::microseconds ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"microseconds",SideEffects::worstDefault,"sf::microseconds")
		->args({"amount"});
	addCtorAndUsing<sf::Clock>(*this,lib,"Clock","sf::Clock");
	using _method_4 = das::das_call_member< sf::Time (sf::Clock::*)() const,&sf::Clock::getElapsedTime >;
	addExtern<DAS_CALL_METHOD(_method_4),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getElapsedTime",SideEffects::worstDefault,"das_call_member< sf::Time (sf::Clock::*)() const , &sf::Clock::getElapsedTime >::invoke")
		->args({"self"});
	using _method_5 = das::das_call_member< sf::Time (sf::Clock::*)(),&sf::Clock::restart >;
	addExtern<DAS_CALL_METHOD(_method_5),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"restart",SideEffects::worstDefault,"das_call_member< sf::Time (sf::Clock::*)() , &sf::Clock::restart >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::FileInputStream>(*this,lib,"FileInputStream","sf::FileInputStream");
	using _method_6 = das::das_call_member< bool (sf::FileInputStream::*)(const std::basic_string<char> &),&sf::FileInputStream::open >;
	addExtern<DAS_CALL_METHOD(_method_6)>(*this,lib,"open",SideEffects::worstDefault,"das_call_member< bool (sf::FileInputStream::*)(const std::basic_string<char> &) , &sf::FileInputStream::open >::invoke")
		->args({"self","filename"});
	addExtern< sf::Int64 (*)(sf::FileInputStream &,void *,sf::Int64) , _dasSFML_virtual_0_read >(*this,lib,"read",SideEffects::worstDefault,"_dasSFML_virtual_0_read")
		->args({"self","data","size"});
	addExtern< sf::Int64 (*)(sf::FileInputStream &,sf::Int64) , _dasSFML_virtual_1_seek >(*this,lib,"seek",SideEffects::worstDefault,"_dasSFML_virtual_1_seek")
		->args({"self","position"});
	addExtern< sf::Int64 (*)(sf::FileInputStream &) , _dasSFML_virtual_2_tell >(*this,lib,"tell",SideEffects::worstDefault,"_dasSFML_virtual_2_tell")
		->args({"self"});
	addExtern< sf::Int64 (*)(sf::FileInputStream &) , _dasSFML_virtual_3_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_3_getSize")
		->args({"self"});
	addCtorAndUsing<sf::Lock,sf::Mutex &>(*this,lib,"Lock","sf::Lock")
		->args({"mutex"});
	addCtorAndUsing<sf::MemoryInputStream>(*this,lib,"MemoryInputStream","sf::MemoryInputStream");
	using _method_11 = das::das_call_member< void (sf::MemoryInputStream::*)(const void *,std::size_t),&sf::MemoryInputStream::open >;
	addExtern<DAS_CALL_METHOD(_method_11)>(*this,lib,"open",SideEffects::worstDefault,"das_call_member< void (sf::MemoryInputStream::*)(const void *,std::size_t) , &sf::MemoryInputStream::open >::invoke")
		->args({"self","data","sizeInBytes"});
	addExtern< sf::Int64 (*)(sf::MemoryInputStream &,void *,sf::Int64) , _dasSFML_virtual_4_read >(*this,lib,"read",SideEffects::worstDefault,"_dasSFML_virtual_4_read")
		->args({"self","data","size"});
	addExtern< sf::Int64 (*)(sf::MemoryInputStream &,sf::Int64) , _dasSFML_virtual_5_seek >(*this,lib,"seek",SideEffects::worstDefault,"_dasSFML_virtual_5_seek")
		->args({"self","position"});
	addExtern< sf::Int64 (*)(sf::MemoryInputStream &) , _dasSFML_virtual_6_tell >(*this,lib,"tell",SideEffects::worstDefault,"_dasSFML_virtual_6_tell")
		->args({"self"});
	addExtern< sf::Int64 (*)(sf::MemoryInputStream &) , _dasSFML_virtual_7_getSize >(*this,lib,"getSize",SideEffects::worstDefault,"_dasSFML_virtual_7_getSize")
		->args({"self"});
	addCtorAndUsing<sf::Mutex>(*this,lib,"Mutex","sf::Mutex");
	using _method_16 = das::das_call_member< void (sf::Mutex::*)(),&sf::Mutex::lock >;
	addExtern<DAS_CALL_METHOD(_method_16)>(*this,lib,"lock",SideEffects::worstDefault,"das_call_member< void (sf::Mutex::*)() , &sf::Mutex::lock >::invoke")
		->args({"self"});
	using _method_17 = das::das_call_member< void (sf::Mutex::*)(),&sf::Mutex::unlock >;
	addExtern<DAS_CALL_METHOD(_method_17)>(*this,lib,"unlock",SideEffects::worstDefault,"das_call_member< void (sf::Mutex::*)() , &sf::Mutex::unlock >::invoke")
		->args({"self"});
	addExtern< void (*)(sf::Time) , sf::sleep >(*this,lib,"sleep",SideEffects::worstDefault,"sf::sleep")
		->args({"duration"});
	using _method_18 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::launch >;
	addExtern<DAS_CALL_METHOD(_method_18)>(*this,lib,"launch",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::launch >::invoke")
		->args({"self"});
	using _method_19 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::wait >;
	addExtern<DAS_CALL_METHOD(_method_19)>(*this,lib,"wait",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::wait >::invoke")
		->args({"self"});
	using _method_20 = das::das_call_member< void (sf::Thread::*)(),&sf::Thread::terminate >;
	addExtern<DAS_CALL_METHOD(_method_20)>(*this,lib,"terminate",SideEffects::worstDefault,"das_call_member< void (sf::Thread::*)() , &sf::Thread::terminate >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::ThreadLocal,void *>(*this,lib,"ThreadLocal","sf::ThreadLocal")
		->args({"value"})
		->arg_init(0,make_smart<ExprConstPtr>());
	using _method_21 = das::das_call_member< void (sf::ThreadLocal::*)(void *),&sf::ThreadLocal::setValue >;
	addExtern<DAS_CALL_METHOD(_method_21)>(*this,lib,"setValue",SideEffects::worstDefault,"das_call_member< void (sf::ThreadLocal::*)(void *) , &sf::ThreadLocal::setValue >::invoke")
		->args({"self","value"});
	using _method_22 = das::das_call_member< void * (sf::ThreadLocal::*)() const,&sf::ThreadLocal::getValue >;
	addExtern<DAS_CALL_METHOD(_method_22)>(*this,lib,"getValue",SideEffects::worstDefault,"das_call_member< void * (sf::ThreadLocal::*)() const , &sf::ThreadLocal::getValue >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::ContextSettings,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,bool>(*this,lib,"ContextSettings","sf::ContextSettings")
		->args({"depth","stencil","antialiasing","major","minor","attributes","sRgb"})
		->arg_init(0,make_smart<ExprConstUInt>(0x0))
		->arg_init(1,make_smart<ExprConstUInt>(0x0))
		->arg_init(2,make_smart<ExprConstUInt>(0x0))
		->arg_init(3,make_smart<ExprConstUInt>(0x1))
		->arg_init(4,make_smart<ExprConstUInt>(0x1))
		->arg_init(5,make_smart<ExprConstUInt>(0x0))
		->arg_init(6,make_smart<ExprConstBool>(false));
	addCtorAndUsing<sf::Context>(*this,lib,"Context","sf::Context");
	using _method_23 = das::das_call_member< bool (sf::Context::*)(bool),&sf::Context::setActive >;
	addExtern<DAS_CALL_METHOD(_method_23)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Context::*)(bool) , &sf::Context::setActive >::invoke")
		->args({"self","active"});
	using _method_24 = das::das_call_member< const sf::ContextSettings & (sf::Context::*)() const,&sf::Context::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_24), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Context::*)() const , &sf::Context::getSettings >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::Context,const sf::ContextSettings &,unsigned int,unsigned int>(*this,lib,"Context","sf::Context")
		->args({"settings","width","height"});
	addCtorAndUsing<sf::Cursor>(*this,lib,"Cursor","sf::Cursor");
	using _method_25 = das::das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>),&sf::Cursor::loadFromPixels >;
	addExtern<DAS_CALL_METHOD(_method_25)>(*this,lib,"loadFromPixels",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>) , &sf::Cursor::loadFromPixels >::invoke")
		->args({"self","pixels","size","hotspot"});
	using _method_26 = das::das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type),&sf::Cursor::loadFromSystem >;
	addExtern<DAS_CALL_METHOD(_method_26)>(*this,lib,"loadFromSystem",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type) , &sf::Cursor::loadFromSystem >::invoke")
		->args({"self","type"});
	addCtorAndUsing<sf::Joystick::Identification>(*this,lib,"Identification","sf::Joystick::Identification");
	addCtorAndUsing<sf::VideoMode>(*this,lib,"VideoMode","sf::VideoMode");
	addCtorAndUsing<sf::VideoMode,unsigned int,unsigned int,unsigned int>(*this,lib,"VideoMode","sf::VideoMode")
		->args({"modeWidth","modeHeight","modeBitsPerPixel"})
		->arg_init(2,make_smart<ExprConstUInt>(0x20));
	using _method_27 = das::das_call_member< bool (sf::VideoMode::*)() const,&sf::VideoMode::isValid >;
	addExtern<DAS_CALL_METHOD(_method_27)>(*this,lib,"isValid",SideEffects::worstDefault,"das_call_member< bool (sf::VideoMode::*)() const , &sf::VideoMode::isValid >::invoke")
		->args({"self"});
	addCtorAndUsing<sf::WindowBase>(*this,lib,"WindowBase","sf::WindowBase");
	addCtorAndUsing<sf::WindowBase,sf::VideoMode,const sf::String &,unsigned int>(*this,lib,"WindowBase","sf::WindowBase")
		->args({"mode","title","style"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::WindowBase &,sf::VideoMode,const sf::String &,unsigned int) , _dasSFML_virtual_8_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_8_create")
		->args({"self","mode","title","style"})
		->arg_init(3,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::WindowBase &) , _dasSFML_virtual_9_close >(*this,lib,"close",SideEffects::worstDefault,"_dasSFML_virtual_9_close")
		->args({"self"});
	using _method_30 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::isOpen >;
	addExtern<DAS_CALL_METHOD(_method_30)>(*this,lib,"isOpen",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::isOpen >::invoke")
		->args({"self"});
	using _method_31 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::pollEvent >;
	addExtern<DAS_CALL_METHOD(_method_31)>(*this,lib,"pollEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::pollEvent >::invoke")
		->args({"self","event"});
	using _method_32 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::waitEvent >;
	addExtern<DAS_CALL_METHOD(_method_32)>(*this,lib,"waitEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::waitEvent >::invoke")
		->args({"self","event"});
	using _method_33 = das::das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const,&sf::WindowBase::getPosition >;
	addExtern<DAS_CALL_METHOD(_method_33)>(*this,lib,"getPosition",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const , &sf::WindowBase::getPosition >::invoke")
		->args({"self"});
	using _method_34 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &),&sf::WindowBase::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_34)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &) , &sf::WindowBase::setPosition >::invoke")
		->args({"self","position"});
	using _method_35 = das::das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const,&sf::WindowBase::getSize >;
	addExtern<DAS_CALL_METHOD(_method_35)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const , &sf::WindowBase::getSize >::invoke")
		->args({"self"});
	using _method_36 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &),&sf::WindowBase::setSize >;
	addExtern<DAS_CALL_METHOD(_method_36)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &) , &sf::WindowBase::setSize >::invoke")
		->args({"self","size"});
	using _method_37 = das::das_call_member< void (sf::WindowBase::*)(const sf::String &),&sf::WindowBase::setTitle >;
	addExtern<DAS_CALL_METHOD(_method_37)>(*this,lib,"setTitle",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::String &) , &sf::WindowBase::setTitle >::invoke")
		->args({"self","title"});
	using _method_38 = das::das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *),&sf::WindowBase::setIcon >;
	addExtern<DAS_CALL_METHOD(_method_38)>(*this,lib,"setIcon",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *) , &sf::WindowBase::setIcon >::invoke")
		->args({"self","width","height","pixels"});
	using _method_39 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setVisible >;
	addExtern<DAS_CALL_METHOD(_method_39)>(*this,lib,"setVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setVisible >::invoke")
		->args({"self","visible"});
	using _method_40 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorVisible >;
	addExtern<DAS_CALL_METHOD(_method_40)>(*this,lib,"setMouseCursorVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorVisible >::invoke")
		->args({"self","visible"});
	using _method_41 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorGrabbed >;
	addExtern<DAS_CALL_METHOD(_method_41)>(*this,lib,"setMouseCursorGrabbed",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorGrabbed >::invoke")
		->args({"self","grabbed"});
	using _method_42 = das::das_call_member< void (sf::WindowBase::*)(const sf::Cursor &),&sf::WindowBase::setMouseCursor >;
	addExtern<DAS_CALL_METHOD(_method_42)>(*this,lib,"setMouseCursor",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Cursor &) , &sf::WindowBase::setMouseCursor >::invoke")
		->args({"self","cursor"});
	using _method_43 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setKeyRepeatEnabled >;
	addExtern<DAS_CALL_METHOD(_method_43)>(*this,lib,"setKeyRepeatEnabled",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setKeyRepeatEnabled >::invoke")
		->args({"self","enabled"});
	using _method_44 = das::das_call_member< void (sf::WindowBase::*)(float),&sf::WindowBase::setJoystickThreshold >;
	addExtern<DAS_CALL_METHOD(_method_44)>(*this,lib,"setJoystickThreshold",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(float) , &sf::WindowBase::setJoystickThreshold >::invoke")
		->args({"self","threshold"});
	using _method_45 = das::das_call_member< void (sf::WindowBase::*)(),&sf::WindowBase::requestFocus >;
	addExtern<DAS_CALL_METHOD(_method_45)>(*this,lib,"requestFocus",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)() , &sf::WindowBase::requestFocus >::invoke")
		->args({"self"});
	using _method_46 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::hasFocus >;
	addExtern<DAS_CALL_METHOD(_method_46)>(*this,lib,"hasFocus",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::hasFocus >::invoke")
		->args({"self"});
	using _method_47 = das::das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *),&sf::WindowBase::createVulkanSurface >;
	addExtern<DAS_CALL_METHOD(_method_47)>(*this,lib,"createVulkanSurface",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *) , &sf::WindowBase::createVulkanSurface >::invoke")
		->args({"self","instance","surface","allocator"})
		->arg_init(3,make_smart<ExprConstPtr>());
	addCtorAndUsing<sf::Window>(*this,lib,"Window","sf::Window");
	addCtorAndUsing<sf::Window,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &>(*this,lib,"Window","sf::Window")
		->args({"mode","title","style","settings"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::Window &,sf::VideoMode,const sf::String &,unsigned int) , _dasSFML_virtual_10_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_10_create")
		->args({"self","mode","title","style"})
		->arg_init(3,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::Window &,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &) , _dasSFML_virtual_11_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_11_create")
		->args({"self","mode","title","style","settings"});
	addExtern< void (*)(sf::Window &) , _dasSFML_virtual_12_close >(*this,lib,"close",SideEffects::worstDefault,"_dasSFML_virtual_12_close")
		->args({"self"});
	using _method_51 = das::das_call_member< const sf::ContextSettings & (sf::Window::*)() const,&sf::Window::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_51), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Window::*)() const , &sf::Window::getSettings >::invoke")
		->args({"self"});
	using _method_52 = das::das_call_member< void (sf::Window::*)(bool),&sf::Window::setVerticalSyncEnabled >;
	addExtern<DAS_CALL_METHOD(_method_52)>(*this,lib,"setVerticalSyncEnabled",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(bool) , &sf::Window::setVerticalSyncEnabled >::invoke")
		->args({"self","enabled"});
	using _method_53 = das::das_call_member< void (sf::Window::*)(unsigned int),&sf::Window::setFramerateLimit >;
	addExtern<DAS_CALL_METHOD(_method_53)>(*this,lib,"setFramerateLimit",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)(unsigned int) , &sf::Window::setFramerateLimit >::invoke")
		->args({"self","limit"});
	using _method_54 = das::das_call_member< bool (sf::Window::*)(bool) const,&sf::Window::setActive >;
	addExtern<DAS_CALL_METHOD(_method_54)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Window::*)(bool) const , &sf::Window::setActive >::invoke")
		->args({"self","active"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	using _method_55 = das::das_call_member< void (sf::Window::*)(),&sf::Window::display >;
	addExtern<DAS_CALL_METHOD(_method_55)>(*this,lib,"display",SideEffects::worstDefault,"das_call_member< void (sf::Window::*)() , &sf::Window::display >::invoke")
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
	using _method_56 = das::das_call_member< const float * (sf::Transform::*)() const,&sf::Transform::getMatrix >;
	addExtern<DAS_CALL_METHOD(_method_56)>(*this,lib,"getMatrix",SideEffects::worstDefault,"das_call_member< const float * (sf::Transform::*)() const , &sf::Transform::getMatrix >::invoke")
		->args({"self"});
	using _method_57 = das::das_call_member< sf::Transform (sf::Transform::*)() const,&sf::Transform::getInverse >;
	addExtern<DAS_CALL_METHOD(_method_57),SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getInverse",SideEffects::worstDefault,"das_call_member< sf::Transform (sf::Transform::*)() const , &sf::Transform::getInverse >::invoke")
		->args({"self"});
	using _method_58 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_58)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(float,float) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","x","y"});
	using _method_59 = das::das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const,&sf::Transform::transformPoint >;
	addExtern<DAS_CALL_METHOD(_method_59)>(*this,lib,"transformPoint",SideEffects::worstDefault,"das_call_member< sf::Vector2<float> (sf::Transform::*)(const sf::Vector2<float> &) const , &sf::Transform::transformPoint >::invoke")
		->args({"self","point"});
	using _method_60 = das::das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const,&sf::Transform::transformRect >;
	addExtern<DAS_CALL_METHOD(_method_60)>(*this,lib,"transformRect",SideEffects::worstDefault,"das_call_member< sf::Rect<float> (sf::Transform::*)(const sf::Rect<float> &) const , &sf::Transform::transformRect >::invoke")
		->args({"self","rectangle"});
	using _method_61 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &),&sf::Transform::combine >;
	addExtern<DAS_CALL_METHOD(_method_61), SimNode_ExtFuncCallRef>(*this,lib,"combine",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Transform &) , &sf::Transform::combine >::invoke")
		->args({"self","transform"});
	using _method_62 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_62), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::translate >::invoke")
		->args({"self","x","y"});
	using _method_63 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::translate >;
	addExtern<DAS_CALL_METHOD(_method_63), SimNode_ExtFuncCallRef>(*this,lib,"translate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::translate >::invoke")
		->args({"self","offset"});
	using _method_64 = das::das_call_member< sf::Transform & (sf::Transform::*)(float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_64), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle"});
	using _method_65 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_65), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","centerX","centerY"});
	using _method_66 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &),&sf::Transform::rotate >;
	addExtern<DAS_CALL_METHOD(_method_66), SimNode_ExtFuncCallRef>(*this,lib,"rotate",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,const sf::Vector2<float> &) , &sf::Transform::rotate >::invoke")
		->args({"self","angle","center"});
	using _method_67 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_67), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY"});
	using _method_68 = das::das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_68), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(float,float,float,float) , &sf::Transform::scale >::invoke")
		->args({"self","scaleX","scaleY","centerX","centerY"});
	using _method_69 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_69), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors"});
	using _method_70 = das::das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &),&sf::Transform::scale >;
	addExtern<DAS_CALL_METHOD(_method_70), SimNode_ExtFuncCallRef>(*this,lib,"scale",SideEffects::worstDefault,"das_call_member< sf::Transform & (sf::Transform::*)(const sf::Vector2<float> &,const sf::Vector2<float> &) , &sf::Transform::scale >::invoke")
		->args({"self","factors","center"});
	addCtorAndUsing<sf::RenderStates>(*this,lib,"RenderStates","sf::RenderStates");
	addCtorAndUsing<sf::RenderStates,const sf::BlendMode &>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theBlendMode"});
	addCtorAndUsing<sf::RenderStates,const sf::Transform &>(*this,lib,"RenderStates","sf::RenderStates")
		->args({"theTransform"});
}
}

