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
	addExtern< sf::String (*)(const sf::Clipboard &) , _dasSFML_static_8_getString >(*this,lib,"getString",SideEffects::worstDefault,"_dasSFML_static_8_getString")
		->args({"self"});
	addExtern< void (*)(const sf::Clipboard &,const sf::String &) , _dasSFML_static_9_setString >(*this,lib,"setString",SideEffects::worstDefault,"_dasSFML_static_9_setString")
		->args({"self","text"});
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
	using _method_25 = das::das_call_member< bool (sf::Context::*)(bool),&sf::Context::setActive >;
	addExtern<DAS_CALL_METHOD(_method_25)>(*this,lib,"setActive",SideEffects::worstDefault,"das_call_member< bool (sf::Context::*)(bool) , &sf::Context::setActive >::invoke")
		->args({"self","active"});
	using _method_26 = das::das_call_member< const sf::ContextSettings & (sf::Context::*)() const,&sf::Context::getSettings >;
	addExtern<DAS_CALL_METHOD(_method_26), SimNode_ExtFuncCallRef>(*this,lib,"getSettings",SideEffects::worstDefault,"das_call_member< const sf::ContextSettings & (sf::Context::*)() const , &sf::Context::getSettings >::invoke")
		->args({"self"});
	addExtern< bool (*)(const sf::Context &,const char *) , _dasSFML_static_10_isExtensionAvailable >(*this,lib,"isExtensionAvailable",SideEffects::worstDefault,"_dasSFML_static_10_isExtensionAvailable")
		->args({"self","name"});
	addExtern< const sf::Context * (*)(const sf::Context &) , _dasSFML_static_11_getActiveContext >(*this,lib,"getActiveContext",SideEffects::worstDefault,"_dasSFML_static_11_getActiveContext")
		->args({"self"});
	addExtern< sf::Uint64 (*)(const sf::Context &) , _dasSFML_static_12_getActiveContextId >(*this,lib,"getActiveContextId",SideEffects::worstDefault,"_dasSFML_static_12_getActiveContextId")
		->args({"self"});
	addCtorAndUsing<sf::Context,const sf::ContextSettings &,unsigned int,unsigned int>(*this,lib,"Context","sf::Context")
		->args({"settings","width","height"});
	addCtorAndUsing<sf::Cursor>(*this,lib,"Cursor","sf::Cursor");
	using _method_30 = das::das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>),&sf::Cursor::loadFromPixels >;
	addExtern<DAS_CALL_METHOD(_method_30)>(*this,lib,"loadFromPixels",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(const unsigned char *,sf::Vector2<unsigned int>,sf::Vector2<unsigned int>) , &sf::Cursor::loadFromPixels >::invoke")
		->args({"self","pixels","size","hotspot"});
	using _method_31 = das::das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type),&sf::Cursor::loadFromSystem >;
	addExtern<DAS_CALL_METHOD(_method_31)>(*this,lib,"loadFromSystem",SideEffects::worstDefault,"das_call_member< bool (sf::Cursor::*)(sf::Cursor::Type) , &sf::Cursor::loadFromSystem >::invoke")
		->args({"self","type"});
	addCtorAndUsing<sf::Joystick::Identification>(*this,lib,"Identification","sf::Joystick::Identification");
	addExtern< bool (*)(const sf::Joystick &,unsigned int) , _dasSFML_static_13_isConnected >(*this,lib,"isConnected",SideEffects::worstDefault,"_dasSFML_static_13_isConnected")
		->args({"self","joystick"});
	addExtern< unsigned int (*)(const sf::Joystick &,unsigned int) , _dasSFML_static_14_getButtonCount >(*this,lib,"getButtonCount",SideEffects::worstDefault,"_dasSFML_static_14_getButtonCount")
		->args({"self","joystick"});
	addExtern< bool (*)(const sf::Joystick &,unsigned int,sf::Joystick::Axis) , _dasSFML_static_15_hasAxis >(*this,lib,"hasAxis",SideEffects::worstDefault,"_dasSFML_static_15_hasAxis")
		->args({"self","joystick","axis"});
	addExtern< bool (*)(const sf::Joystick &,unsigned int,unsigned int) , _dasSFML_static_16_isButtonPressed >(*this,lib,"isButtonPressed",SideEffects::worstDefault,"_dasSFML_static_16_isButtonPressed")
		->args({"self","joystick","button"});
	addExtern< float (*)(const sf::Joystick &,unsigned int,sf::Joystick::Axis) , _dasSFML_static_17_getAxisPosition >(*this,lib,"getAxisPosition",SideEffects::worstDefault,"_dasSFML_static_17_getAxisPosition")
		->args({"self","joystick","axis"});
	addExtern< void (*)(const sf::Joystick &) , _dasSFML_static_18_update >(*this,lib,"update",SideEffects::worstDefault,"_dasSFML_static_18_update")
		->args({"self"});
	addExtern< bool (*)(const sf::Keyboard &,sf::Keyboard::Key) , _dasSFML_static_19_isKeyPressed >(*this,lib,"isKeyPressed",SideEffects::worstDefault,"_dasSFML_static_19_isKeyPressed")
		->args({"self","key"});
	addExtern< void (*)(const sf::Keyboard &,bool) , _dasSFML_static_20_setVirtualKeyboardVisible >(*this,lib,"setVirtualKeyboardVisible",SideEffects::worstDefault,"_dasSFML_static_20_setVirtualKeyboardVisible")
		->args({"self","visible"});
	addExtern< bool (*)(const sf::Mouse &,sf::Mouse::Button) , _dasSFML_static_21_isButtonPressed >(*this,lib,"isButtonPressed",SideEffects::worstDefault,"_dasSFML_static_21_isButtonPressed")
		->args({"self","button"});
	addExtern< sf::Vector2<int> (*)(const sf::Mouse &) , _dasSFML_static_22_getPosition >(*this,lib,"getPosition",SideEffects::worstDefault,"_dasSFML_static_22_getPosition")
		->args({"self"});
	addExtern< sf::Vector2<int> (*)(const sf::Mouse &,const sf::WindowBase &) , _dasSFML_static_23_getPosition >(*this,lib,"getPosition",SideEffects::worstDefault,"_dasSFML_static_23_getPosition")
		->args({"self","relativeTo"});
	addExtern< void (*)(const sf::Mouse &,const sf::Vector2<int> &) , _dasSFML_static_24_setPosition >(*this,lib,"setPosition",SideEffects::worstDefault,"_dasSFML_static_24_setPosition")
		->args({"self","position"});
	addExtern< void (*)(const sf::Mouse &,const sf::Vector2<int> &,const sf::WindowBase &) , _dasSFML_static_25_setPosition >(*this,lib,"setPosition",SideEffects::worstDefault,"_dasSFML_static_25_setPosition")
		->args({"self","position","relativeTo"});
	addExtern< bool (*)(const sf::Sensor &,sf::Sensor::Type) , _dasSFML_static_26_isAvailable >(*this,lib,"isAvailable",SideEffects::worstDefault,"_dasSFML_static_26_isAvailable")
		->args({"self","sensor"});
	addExtern< void (*)(const sf::Sensor &,sf::Sensor::Type,bool) , _dasSFML_static_27_setEnabled >(*this,lib,"setEnabled",SideEffects::worstDefault,"_dasSFML_static_27_setEnabled")
		->args({"self","sensor","enabled"});
	addExtern< sf::Vector3<float> (*)(const sf::Sensor &,sf::Sensor::Type) , _dasSFML_static_28_getValue ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getValue",SideEffects::worstDefault,"_dasSFML_static_28_getValue")
		->args({"self","sensor"});
	addExtern< bool (*)(const sf::Touch &,unsigned int) , _dasSFML_static_29_isDown >(*this,lib,"isDown",SideEffects::worstDefault,"_dasSFML_static_29_isDown")
		->args({"self","finger"});
	addExtern< sf::Vector2<int> (*)(const sf::Touch &,unsigned int) , _dasSFML_static_30_getPosition >(*this,lib,"getPosition",SideEffects::worstDefault,"_dasSFML_static_30_getPosition")
		->args({"self","finger"});
	addExtern< sf::Vector2<int> (*)(const sf::Touch &,unsigned int,const sf::WindowBase &) , _dasSFML_static_31_getPosition >(*this,lib,"getPosition",SideEffects::worstDefault,"_dasSFML_static_31_getPosition")
		->args({"self","finger","relativeTo"});
	addCtorAndUsing<sf::VideoMode>(*this,lib,"VideoMode","sf::VideoMode");
	addCtorAndUsing<sf::VideoMode,unsigned int,unsigned int,unsigned int>(*this,lib,"VideoMode","sf::VideoMode")
		->args({"modeWidth","modeHeight","modeBitsPerPixel"})
		->arg_init(2,make_smart<ExprConstUInt>(0x20));
	addExtern< sf::VideoMode (*)(const sf::VideoMode &) , _dasSFML_static_32_getDesktopMode ,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"getDesktopMode",SideEffects::worstDefault,"_dasSFML_static_32_getDesktopMode")
		->args({"self"});
	addExtern< const std::vector<sf::VideoMode> & (*)(const sf::VideoMode &) , _dasSFML_static_33_getFullscreenModes , SimNode_ExtFuncCallRef>(*this,lib,"getFullscreenModes",SideEffects::worstDefault,"_dasSFML_static_33_getFullscreenModes")
		->args({"self"});
	using _method_53 = das::das_call_member< bool (sf::VideoMode::*)() const,&sf::VideoMode::isValid >;
	addExtern<DAS_CALL_METHOD(_method_53)>(*this,lib,"isValid",SideEffects::worstDefault,"das_call_member< bool (sf::VideoMode::*)() const , &sf::VideoMode::isValid >::invoke")
		->args({"self"});
	addExtern< bool (*)(const sf::Vulkan &,bool) , _dasSFML_static_34_isAvailable >(*this,lib,"isAvailable",SideEffects::worstDefault,"_dasSFML_static_34_isAvailable")
		->args({"self","requireGraphics"})
		->arg_init(1,make_smart<ExprConstBool>(true));
	addExtern< const std::vector<const char *> & (*)(const sf::Vulkan &) , _dasSFML_static_35_getGraphicsRequiredInstanceExtensions , SimNode_ExtFuncCallRef>(*this,lib,"getGraphicsRequiredInstanceExtensions",SideEffects::worstDefault,"_dasSFML_static_35_getGraphicsRequiredInstanceExtensions")
		->args({"self"});
	addCtorAndUsing<sf::WindowBase>(*this,lib,"WindowBase","sf::WindowBase");
	addCtorAndUsing<sf::WindowBase,sf::VideoMode,const sf::String &,unsigned int>(*this,lib,"WindowBase","sf::WindowBase")
		->args({"mode","title","style"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::WindowBase &,sf::VideoMode,const sf::String &,unsigned int) , _dasSFML_virtual_36_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_36_create")
		->args({"self","mode","title","style"})
		->arg_init(3,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::WindowBase &) , _dasSFML_virtual_37_close >(*this,lib,"close",SideEffects::worstDefault,"_dasSFML_virtual_37_close")
		->args({"self"});
	using _method_58 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::isOpen >;
	addExtern<DAS_CALL_METHOD(_method_58)>(*this,lib,"isOpen",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::isOpen >::invoke")
		->args({"self"});
	using _method_59 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::pollEvent >;
	addExtern<DAS_CALL_METHOD(_method_59)>(*this,lib,"pollEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::pollEvent >::invoke")
		->args({"self","event"});
	using _method_60 = das::das_call_member< bool (sf::WindowBase::*)(sf::Event &),&sf::WindowBase::waitEvent >;
	addExtern<DAS_CALL_METHOD(_method_60)>(*this,lib,"waitEvent",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(sf::Event &) , &sf::WindowBase::waitEvent >::invoke")
		->args({"self","event"});
	using _method_61 = das::das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const,&sf::WindowBase::getPosition >;
	addExtern<DAS_CALL_METHOD(_method_61)>(*this,lib,"getPosition",SideEffects::worstDefault,"das_call_member< sf::Vector2<int> (sf::WindowBase::*)() const , &sf::WindowBase::getPosition >::invoke")
		->args({"self"});
	using _method_62 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &),&sf::WindowBase::setPosition >;
	addExtern<DAS_CALL_METHOD(_method_62)>(*this,lib,"setPosition",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<int> &) , &sf::WindowBase::setPosition >::invoke")
		->args({"self","position"});
	using _method_63 = das::das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const,&sf::WindowBase::getSize >;
	addExtern<DAS_CALL_METHOD(_method_63)>(*this,lib,"getSize",SideEffects::worstDefault,"das_call_member< sf::Vector2<unsigned int> (sf::WindowBase::*)() const , &sf::WindowBase::getSize >::invoke")
		->args({"self"});
	using _method_64 = das::das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &),&sf::WindowBase::setSize >;
	addExtern<DAS_CALL_METHOD(_method_64)>(*this,lib,"setSize",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Vector2<unsigned int> &) , &sf::WindowBase::setSize >::invoke")
		->args({"self","size"});
	using _method_65 = das::das_call_member< void (sf::WindowBase::*)(const sf::String &),&sf::WindowBase::setTitle >;
	addExtern<DAS_CALL_METHOD(_method_65)>(*this,lib,"setTitle",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::String &) , &sf::WindowBase::setTitle >::invoke")
		->args({"self","title"});
	using _method_66 = das::das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *),&sf::WindowBase::setIcon >;
	addExtern<DAS_CALL_METHOD(_method_66)>(*this,lib,"setIcon",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(unsigned int,unsigned int,const unsigned char *) , &sf::WindowBase::setIcon >::invoke")
		->args({"self","width","height","pixels"});
	using _method_67 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setVisible >;
	addExtern<DAS_CALL_METHOD(_method_67)>(*this,lib,"setVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setVisible >::invoke")
		->args({"self","visible"});
	using _method_68 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorVisible >;
	addExtern<DAS_CALL_METHOD(_method_68)>(*this,lib,"setMouseCursorVisible",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorVisible >::invoke")
		->args({"self","visible"});
	using _method_69 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setMouseCursorGrabbed >;
	addExtern<DAS_CALL_METHOD(_method_69)>(*this,lib,"setMouseCursorGrabbed",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setMouseCursorGrabbed >::invoke")
		->args({"self","grabbed"});
	using _method_70 = das::das_call_member< void (sf::WindowBase::*)(const sf::Cursor &),&sf::WindowBase::setMouseCursor >;
	addExtern<DAS_CALL_METHOD(_method_70)>(*this,lib,"setMouseCursor",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(const sf::Cursor &) , &sf::WindowBase::setMouseCursor >::invoke")
		->args({"self","cursor"});
	using _method_71 = das::das_call_member< void (sf::WindowBase::*)(bool),&sf::WindowBase::setKeyRepeatEnabled >;
	addExtern<DAS_CALL_METHOD(_method_71)>(*this,lib,"setKeyRepeatEnabled",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(bool) , &sf::WindowBase::setKeyRepeatEnabled >::invoke")
		->args({"self","enabled"});
	using _method_72 = das::das_call_member< void (sf::WindowBase::*)(float),&sf::WindowBase::setJoystickThreshold >;
	addExtern<DAS_CALL_METHOD(_method_72)>(*this,lib,"setJoystickThreshold",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)(float) , &sf::WindowBase::setJoystickThreshold >::invoke")
		->args({"self","threshold"});
	using _method_73 = das::das_call_member< void (sf::WindowBase::*)(),&sf::WindowBase::requestFocus >;
	addExtern<DAS_CALL_METHOD(_method_73)>(*this,lib,"requestFocus",SideEffects::worstDefault,"das_call_member< void (sf::WindowBase::*)() , &sf::WindowBase::requestFocus >::invoke")
		->args({"self"});
	using _method_74 = das::das_call_member< bool (sf::WindowBase::*)() const,&sf::WindowBase::hasFocus >;
	addExtern<DAS_CALL_METHOD(_method_74)>(*this,lib,"hasFocus",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)() const , &sf::WindowBase::hasFocus >::invoke")
		->args({"self"});
	using _method_75 = das::das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *),&sf::WindowBase::createVulkanSurface >;
	addExtern<DAS_CALL_METHOD(_method_75)>(*this,lib,"createVulkanSurface",SideEffects::worstDefault,"das_call_member< bool (sf::WindowBase::*)(VkInstance_T *const &,VkSurfaceKHR_T *&,const VkAllocationCallbacks *) , &sf::WindowBase::createVulkanSurface >::invoke")
		->args({"self","instance","surface","allocator"})
		->arg_init(3,make_smart<ExprConstPtr>());
	addCtorAndUsing<sf::Window>(*this,lib,"Window","sf::Window");
	addCtorAndUsing<sf::Window,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &>(*this,lib,"Window","sf::Window")
		->args({"mode","title","style","settings"})
		->arg_init(2,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::Window &,sf::VideoMode,const sf::String &,unsigned int) , _dasSFML_virtual_38_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_38_create")
		->args({"self","mode","title","style"})
		->arg_init(3,make_smart<ExprConstUInt>(0x7));
	addExtern< void (*)(sf::Window &,sf::VideoMode,const sf::String &,unsigned int,const sf::ContextSettings &) , _dasSFML_virtual_39_create >(*this,lib,"create",SideEffects::worstDefault,"_dasSFML_virtual_39_create")
		->args({"self","mode","title","style","settings"});
	addExtern< void (*)(sf::Window &) , _dasSFML_virtual_40_close >(*this,lib,"close",SideEffects::worstDefault,"_dasSFML_virtual_40_close")
		->args({"self"});
}
}

