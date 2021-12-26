// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

template <> struct cast_arg<sf::Time> {
	static __forceinline const sf::Time & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Time *>::to(res);
	}
};
template <> struct cast_arg<sf::Clock> {
	static __forceinline const sf::Clock & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Clock *>::to(res);
	}
};
template <> struct cast_arg<sf::InputStream> {
	static __forceinline const sf::InputStream & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::InputStream *>::to(res);
	}
};
template <> struct cast_arg<sf::NonCopyable> {
	static __forceinline const sf::NonCopyable & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::NonCopyable *>::to(res);
	}
};
template <> struct cast_arg<sf::FileInputStream> {
	static __forceinline const sf::FileInputStream & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::FileInputStream *>::to(res);
	}
};
template <> struct cast_arg<sf::Lock> {
	static __forceinline const sf::Lock & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Lock *>::to(res);
	}
};
template <> struct cast_arg<sf::MemoryInputStream> {
	static __forceinline const sf::MemoryInputStream & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::MemoryInputStream *>::to(res);
	}
};
template <> struct cast_arg<sf::Mutex> {
	static __forceinline const sf::Mutex & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Mutex *>::to(res);
	}
};
template <> struct cast_arg<sf::Thread> {
	static __forceinline const sf::Thread & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Thread *>::to(res);
	}
};
template <> struct cast_arg<sf::priv::ThreadFunc> {
	static __forceinline const sf::priv::ThreadFunc & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::priv::ThreadFunc *>::to(res);
	}
};
template <> struct cast_arg<sf::ThreadLocal> {
	static __forceinline const sf::ThreadLocal & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::ThreadLocal *>::to(res);
	}
};
template <> struct cast_arg<sf::Clipboard> {
	static __forceinline const sf::Clipboard & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Clipboard *>::to(res);
	}
};
template <> struct cast_arg<sf::GlResource> {
	static __forceinline const sf::GlResource & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::GlResource *>::to(res);
	}
};
template <> struct cast_arg<sf::ContextSettings> {
	static __forceinline const sf::ContextSettings & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::ContextSettings *>::to(res);
	}
};
template <> struct cast_arg<sf::Context> {
	static __forceinline const sf::Context & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Context *>::to(res);
	}
};
template <> struct cast_arg<sf::Cursor> {
	static __forceinline const sf::Cursor & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Cursor *>::to(res);
	}
};
template <> struct cast_arg<sf::Joystick::Identification> {
	static __forceinline const sf::Joystick::Identification & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Joystick::Identification *>::to(res);
	}
};
template <> struct cast_arg<sf::Joystick> {
	static __forceinline const sf::Joystick & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Joystick *>::to(res);
	}
};
template <> struct cast_arg<sf::Keyboard> {
	static __forceinline const sf::Keyboard & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Keyboard *>::to(res);
	}
};
template <> struct cast_arg<sf::Mouse> {
	static __forceinline const sf::Mouse & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Mouse *>::to(res);
	}
};
template <> struct cast_arg<sf::Sensor> {
	static __forceinline const sf::Sensor & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Sensor *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::SizeEvent> {
	static __forceinline const sf::Event::SizeEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::SizeEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::KeyEvent> {
	static __forceinline const sf::Event::KeyEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::KeyEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::TextEvent> {
	static __forceinline const sf::Event::TextEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::TextEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::MouseMoveEvent> {
	static __forceinline const sf::Event::MouseMoveEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::MouseMoveEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::MouseButtonEvent> {
	static __forceinline const sf::Event::MouseButtonEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::MouseButtonEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::MouseWheelEvent> {
	static __forceinline const sf::Event::MouseWheelEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::MouseWheelEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::MouseWheelScrollEvent> {
	static __forceinline const sf::Event::MouseWheelScrollEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::MouseWheelScrollEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::JoystickConnectEvent> {
	static __forceinline const sf::Event::JoystickConnectEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::JoystickConnectEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::JoystickMoveEvent> {
	static __forceinline const sf::Event::JoystickMoveEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::JoystickMoveEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::JoystickButtonEvent> {
	static __forceinline const sf::Event::JoystickButtonEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::JoystickButtonEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::TouchEvent> {
	static __forceinline const sf::Event::TouchEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::TouchEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event::SensorEvent> {
	static __forceinline const sf::Event::SensorEvent & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event::SensorEvent *>::to(res);
	}
};
template <> struct cast_arg<sf::Event> {
	static __forceinline const sf::Event & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Event *>::to(res);
	}
};
template <> struct cast_arg<sf::Touch> {
	static __forceinline const sf::Touch & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Touch *>::to(res);
	}
};
template <> struct cast_arg<sf::VideoMode> {
	static __forceinline const sf::VideoMode & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::VideoMode *>::to(res);
	}
};
template <> struct cast_arg<sf::Vulkan> {
	static __forceinline const sf::Vulkan & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Vulkan *>::to(res);
	}
};
template <> struct cast_arg<sf::WindowBase> {
	static __forceinline const sf::WindowBase & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::WindowBase *>::to(res);
	}
};
template <> struct cast_arg<sf::Window> {
	static __forceinline const sf::Window & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<sf::Window *>::to(res);
	}
};
