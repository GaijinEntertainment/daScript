// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

class Enumeration_sf_ContextSettings_Attribute : public das::Enumeration {
public:
	Enumeration_sf_ContextSettings_Attribute() : das::Enumeration("Attribute") {
		external = true;
		cppName = "sf::ContextSettings::Attribute";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Default", "Default", int64_t(sf::ContextSettings::Attribute::Default), das::LineInfo());
		addIEx("Core", "Core", int64_t(sf::ContextSettings::Attribute::Core), das::LineInfo());
		addIEx("Debug", "Debug", int64_t(sf::ContextSettings::Attribute::Debug), das::LineInfo());
	}
};

class Enumeration_sf_Cursor_Type : public das::Enumeration {
public:
	Enumeration_sf_Cursor_Type() : das::Enumeration("CursorType") {
		external = true;
		cppName = "sf::Cursor::Type";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Arrow", "Arrow", int64_t(sf::Cursor::Type::Arrow), das::LineInfo());
		addIEx("ArrowWait", "ArrowWait", int64_t(sf::Cursor::Type::ArrowWait), das::LineInfo());
		addIEx("Wait", "Wait", int64_t(sf::Cursor::Type::Wait), das::LineInfo());
		addIEx("Text", "Text", int64_t(sf::Cursor::Type::Text), das::LineInfo());
		addIEx("Hand", "Hand", int64_t(sf::Cursor::Type::Hand), das::LineInfo());
		addIEx("SizeHorizontal", "SizeHorizontal", int64_t(sf::Cursor::Type::SizeHorizontal), das::LineInfo());
		addIEx("SizeVertical", "SizeVertical", int64_t(sf::Cursor::Type::SizeVertical), das::LineInfo());
		addIEx("SizeTopLeftBottomRight", "SizeTopLeftBottomRight", int64_t(sf::Cursor::Type::SizeTopLeftBottomRight), das::LineInfo());
		addIEx("SizeBottomLeftTopRight", "SizeBottomLeftTopRight", int64_t(sf::Cursor::Type::SizeBottomLeftTopRight), das::LineInfo());
		addIEx("SizeLeft", "SizeLeft", int64_t(sf::Cursor::Type::SizeLeft), das::LineInfo());
		addIEx("SizeRight", "SizeRight", int64_t(sf::Cursor::Type::SizeRight), das::LineInfo());
		addIEx("SizeTop", "SizeTop", int64_t(sf::Cursor::Type::SizeTop), das::LineInfo());
		addIEx("SizeBottom", "SizeBottom", int64_t(sf::Cursor::Type::SizeBottom), das::LineInfo());
		addIEx("SizeTopLeft", "SizeTopLeft", int64_t(sf::Cursor::Type::SizeTopLeft), das::LineInfo());
		addIEx("SizeBottomRight", "SizeBottomRight", int64_t(sf::Cursor::Type::SizeBottomRight), das::LineInfo());
		addIEx("SizeBottomLeft", "SizeBottomLeft", int64_t(sf::Cursor::Type::SizeBottomLeft), das::LineInfo());
		addIEx("SizeTopRight", "SizeTopRight", int64_t(sf::Cursor::Type::SizeTopRight), das::LineInfo());
		addIEx("SizeAll", "SizeAll", int64_t(sf::Cursor::Type::SizeAll), das::LineInfo());
		addIEx("Cross", "Cross", int64_t(sf::Cursor::Type::Cross), das::LineInfo());
		addIEx("Help", "Help", int64_t(sf::Cursor::Type::Help), das::LineInfo());
		addIEx("NotAllowed", "NotAllowed", int64_t(sf::Cursor::Type::NotAllowed), das::LineInfo());
	}
};

class Enumeration_sf_Joystick_Axis : public das::Enumeration {
public:
	Enumeration_sf_Joystick_Axis() : das::Enumeration("Axis") {
		external = true;
		cppName = "sf::Joystick::Axis";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("X", "X", int64_t(sf::Joystick::Axis::X), das::LineInfo());
		addIEx("Y", "Y", int64_t(sf::Joystick::Axis::Y), das::LineInfo());
		addIEx("Z", "Z", int64_t(sf::Joystick::Axis::Z), das::LineInfo());
		addIEx("R", "R", int64_t(sf::Joystick::Axis::R), das::LineInfo());
		addIEx("U", "U", int64_t(sf::Joystick::Axis::U), das::LineInfo());
		addIEx("V", "V", int64_t(sf::Joystick::Axis::V), das::LineInfo());
		addIEx("PovX", "PovX", int64_t(sf::Joystick::Axis::PovX), das::LineInfo());
		addIEx("PovY", "PovY", int64_t(sf::Joystick::Axis::PovY), das::LineInfo());
	}
};

class Enumeration_sf_Keyboard_Key : public das::Enumeration {
public:
	Enumeration_sf_Keyboard_Key() : das::Enumeration("Key") {
		external = true;
		cppName = "sf::Keyboard::Key";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Unknown", "Unknown", int64_t(sf::Keyboard::Key::Unknown), das::LineInfo());
		addIEx("A", "A", int64_t(sf::Keyboard::Key::A), das::LineInfo());
		addIEx("B", "B", int64_t(sf::Keyboard::Key::B), das::LineInfo());
		addIEx("C", "C", int64_t(sf::Keyboard::Key::C), das::LineInfo());
		addIEx("D", "D", int64_t(sf::Keyboard::Key::D), das::LineInfo());
		addIEx("E", "E", int64_t(sf::Keyboard::Key::E), das::LineInfo());
		addIEx("F", "F", int64_t(sf::Keyboard::Key::F), das::LineInfo());
		addIEx("G", "G", int64_t(sf::Keyboard::Key::G), das::LineInfo());
		addIEx("H", "H", int64_t(sf::Keyboard::Key::H), das::LineInfo());
		addIEx("I", "I", int64_t(sf::Keyboard::Key::I), das::LineInfo());
		addIEx("J", "J", int64_t(sf::Keyboard::Key::J), das::LineInfo());
		addIEx("K", "K", int64_t(sf::Keyboard::Key::K), das::LineInfo());
		addIEx("L", "L", int64_t(sf::Keyboard::Key::L), das::LineInfo());
		addIEx("M", "M", int64_t(sf::Keyboard::Key::M), das::LineInfo());
		addIEx("N", "N", int64_t(sf::Keyboard::Key::N), das::LineInfo());
		addIEx("O", "O", int64_t(sf::Keyboard::Key::O), das::LineInfo());
		addIEx("P", "P", int64_t(sf::Keyboard::Key::P), das::LineInfo());
		addIEx("Q", "Q", int64_t(sf::Keyboard::Key::Q), das::LineInfo());
		addIEx("R", "R", int64_t(sf::Keyboard::Key::R), das::LineInfo());
		addIEx("S", "S", int64_t(sf::Keyboard::Key::S), das::LineInfo());
		addIEx("T", "T", int64_t(sf::Keyboard::Key::T), das::LineInfo());
		addIEx("U", "U", int64_t(sf::Keyboard::Key::U), das::LineInfo());
		addIEx("V", "V", int64_t(sf::Keyboard::Key::V), das::LineInfo());
		addIEx("W", "W", int64_t(sf::Keyboard::Key::W), das::LineInfo());
		addIEx("X", "X", int64_t(sf::Keyboard::Key::X), das::LineInfo());
		addIEx("Y", "Y", int64_t(sf::Keyboard::Key::Y), das::LineInfo());
		addIEx("Z", "Z", int64_t(sf::Keyboard::Key::Z), das::LineInfo());
		addIEx("Num0", "Num0", int64_t(sf::Keyboard::Key::Num0), das::LineInfo());
		addIEx("Num1", "Num1", int64_t(sf::Keyboard::Key::Num1), das::LineInfo());
		addIEx("Num2", "Num2", int64_t(sf::Keyboard::Key::Num2), das::LineInfo());
		addIEx("Num3", "Num3", int64_t(sf::Keyboard::Key::Num3), das::LineInfo());
		addIEx("Num4", "Num4", int64_t(sf::Keyboard::Key::Num4), das::LineInfo());
		addIEx("Num5", "Num5", int64_t(sf::Keyboard::Key::Num5), das::LineInfo());
		addIEx("Num6", "Num6", int64_t(sf::Keyboard::Key::Num6), das::LineInfo());
		addIEx("Num7", "Num7", int64_t(sf::Keyboard::Key::Num7), das::LineInfo());
		addIEx("Num8", "Num8", int64_t(sf::Keyboard::Key::Num8), das::LineInfo());
		addIEx("Num9", "Num9", int64_t(sf::Keyboard::Key::Num9), das::LineInfo());
		addIEx("Escape", "Escape", int64_t(sf::Keyboard::Key::Escape), das::LineInfo());
		addIEx("LControl", "LControl", int64_t(sf::Keyboard::Key::LControl), das::LineInfo());
		addIEx("LShift", "LShift", int64_t(sf::Keyboard::Key::LShift), das::LineInfo());
		addIEx("LAlt", "LAlt", int64_t(sf::Keyboard::Key::LAlt), das::LineInfo());
		addIEx("LSystem", "LSystem", int64_t(sf::Keyboard::Key::LSystem), das::LineInfo());
		addIEx("RControl", "RControl", int64_t(sf::Keyboard::Key::RControl), das::LineInfo());
		addIEx("RShift", "RShift", int64_t(sf::Keyboard::Key::RShift), das::LineInfo());
		addIEx("RAlt", "RAlt", int64_t(sf::Keyboard::Key::RAlt), das::LineInfo());
		addIEx("RSystem", "RSystem", int64_t(sf::Keyboard::Key::RSystem), das::LineInfo());
		addIEx("Menu", "Menu", int64_t(sf::Keyboard::Key::Menu), das::LineInfo());
		addIEx("LBracket", "LBracket", int64_t(sf::Keyboard::Key::LBracket), das::LineInfo());
		addIEx("RBracket", "RBracket", int64_t(sf::Keyboard::Key::RBracket), das::LineInfo());
		addIEx("Semicolon", "Semicolon", int64_t(sf::Keyboard::Key::Semicolon), das::LineInfo());
		addIEx("Comma", "Comma", int64_t(sf::Keyboard::Key::Comma), das::LineInfo());
		addIEx("Period", "Period", int64_t(sf::Keyboard::Key::Period), das::LineInfo());
		addIEx("Quote", "Quote", int64_t(sf::Keyboard::Key::Quote), das::LineInfo());
		addIEx("Slash", "Slash", int64_t(sf::Keyboard::Key::Slash), das::LineInfo());
		addIEx("Backslash", "Backslash", int64_t(sf::Keyboard::Key::Backslash), das::LineInfo());
		addIEx("Tilde", "Tilde", int64_t(sf::Keyboard::Key::Tilde), das::LineInfo());
		addIEx("Equal", "Equal", int64_t(sf::Keyboard::Key::Equal), das::LineInfo());
		addIEx("Hyphen", "Hyphen", int64_t(sf::Keyboard::Key::Hyphen), das::LineInfo());
		addIEx("Space", "Space", int64_t(sf::Keyboard::Key::Space), das::LineInfo());
		addIEx("Enter", "Enter", int64_t(sf::Keyboard::Key::Enter), das::LineInfo());
		addIEx("Backspace", "Backspace", int64_t(sf::Keyboard::Key::Backspace), das::LineInfo());
		addIEx("Tab", "Tab", int64_t(sf::Keyboard::Key::Tab), das::LineInfo());
		addIEx("PageUp", "PageUp", int64_t(sf::Keyboard::Key::PageUp), das::LineInfo());
		addIEx("PageDown", "PageDown", int64_t(sf::Keyboard::Key::PageDown), das::LineInfo());
		addIEx("End", "End", int64_t(sf::Keyboard::Key::End), das::LineInfo());
		addIEx("Home", "Home", int64_t(sf::Keyboard::Key::Home), das::LineInfo());
		addIEx("Insert", "Insert", int64_t(sf::Keyboard::Key::Insert), das::LineInfo());
		addIEx("Delete", "Delete", int64_t(sf::Keyboard::Key::Delete), das::LineInfo());
		addIEx("Add", "Add", int64_t(sf::Keyboard::Key::Add), das::LineInfo());
		addIEx("Subtract", "Subtract", int64_t(sf::Keyboard::Key::Subtract), das::LineInfo());
		addIEx("Multiply", "Multiply", int64_t(sf::Keyboard::Key::Multiply), das::LineInfo());
		addIEx("Divide", "Divide", int64_t(sf::Keyboard::Key::Divide), das::LineInfo());
		addIEx("Left", "Left", int64_t(sf::Keyboard::Key::Left), das::LineInfo());
		addIEx("Right", "Right", int64_t(sf::Keyboard::Key::Right), das::LineInfo());
		addIEx("Up", "Up", int64_t(sf::Keyboard::Key::Up), das::LineInfo());
		addIEx("Down", "Down", int64_t(sf::Keyboard::Key::Down), das::LineInfo());
		addIEx("Numpad0", "Numpad0", int64_t(sf::Keyboard::Key::Numpad0), das::LineInfo());
		addIEx("Numpad1", "Numpad1", int64_t(sf::Keyboard::Key::Numpad1), das::LineInfo());
		addIEx("Numpad2", "Numpad2", int64_t(sf::Keyboard::Key::Numpad2), das::LineInfo());
		addIEx("Numpad3", "Numpad3", int64_t(sf::Keyboard::Key::Numpad3), das::LineInfo());
		addIEx("Numpad4", "Numpad4", int64_t(sf::Keyboard::Key::Numpad4), das::LineInfo());
		addIEx("Numpad5", "Numpad5", int64_t(sf::Keyboard::Key::Numpad5), das::LineInfo());
		addIEx("Numpad6", "Numpad6", int64_t(sf::Keyboard::Key::Numpad6), das::LineInfo());
		addIEx("Numpad7", "Numpad7", int64_t(sf::Keyboard::Key::Numpad7), das::LineInfo());
		addIEx("Numpad8", "Numpad8", int64_t(sf::Keyboard::Key::Numpad8), das::LineInfo());
		addIEx("Numpad9", "Numpad9", int64_t(sf::Keyboard::Key::Numpad9), das::LineInfo());
		addIEx("F1", "F1", int64_t(sf::Keyboard::Key::F1), das::LineInfo());
		addIEx("F2", "F2", int64_t(sf::Keyboard::Key::F2), das::LineInfo());
		addIEx("F3", "F3", int64_t(sf::Keyboard::Key::F3), das::LineInfo());
		addIEx("F4", "F4", int64_t(sf::Keyboard::Key::F4), das::LineInfo());
		addIEx("F5", "F5", int64_t(sf::Keyboard::Key::F5), das::LineInfo());
		addIEx("F6", "F6", int64_t(sf::Keyboard::Key::F6), das::LineInfo());
		addIEx("F7", "F7", int64_t(sf::Keyboard::Key::F7), das::LineInfo());
		addIEx("F8", "F8", int64_t(sf::Keyboard::Key::F8), das::LineInfo());
		addIEx("F9", "F9", int64_t(sf::Keyboard::Key::F9), das::LineInfo());
		addIEx("F10", "F10", int64_t(sf::Keyboard::Key::F10), das::LineInfo());
		addIEx("F11", "F11", int64_t(sf::Keyboard::Key::F11), das::LineInfo());
		addIEx("F12", "F12", int64_t(sf::Keyboard::Key::F12), das::LineInfo());
		addIEx("F13", "F13", int64_t(sf::Keyboard::Key::F13), das::LineInfo());
		addIEx("F14", "F14", int64_t(sf::Keyboard::Key::F14), das::LineInfo());
		addIEx("F15", "F15", int64_t(sf::Keyboard::Key::F15), das::LineInfo());
		addIEx("Pause", "Pause", int64_t(sf::Keyboard::Key::Pause), das::LineInfo());
		addIEx("Count", "KeyCount", int64_t(sf::Keyboard::Key::KeyCount), das::LineInfo());
		addIEx("Dash", "Dash", int64_t(sf::Keyboard::Key::Dash), das::LineInfo());
		addIEx("BackSpace", "BackSpace", int64_t(sf::Keyboard::Key::BackSpace), das::LineInfo());
		addIEx("BackSlash", "BackSlash", int64_t(sf::Keyboard::Key::BackSlash), das::LineInfo());
		addIEx("SemiColon", "SemiColon", int64_t(sf::Keyboard::Key::SemiColon), das::LineInfo());
		addIEx("Return", "Return", int64_t(sf::Keyboard::Key::Return), das::LineInfo());
	}
};

class Enumeration_sf_Mouse_Button : public das::Enumeration {
public:
	Enumeration_sf_Mouse_Button() : das::Enumeration("Button") {
		external = true;
		cppName = "sf::Mouse::Button";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Left", "Left", int64_t(sf::Mouse::Button::Left), das::LineInfo());
		addIEx("Right", "Right", int64_t(sf::Mouse::Button::Right), das::LineInfo());
		addIEx("Middle", "Middle", int64_t(sf::Mouse::Button::Middle), das::LineInfo());
		addIEx("XButton1", "XButton1", int64_t(sf::Mouse::Button::XButton1), das::LineInfo());
		addIEx("XButton2", "XButton2", int64_t(sf::Mouse::Button::XButton2), das::LineInfo());
		addIEx("Count", "ButtonCount", int64_t(sf::Mouse::Button::ButtonCount), das::LineInfo());
	}
};

class Enumeration_sf_Mouse_Wheel : public das::Enumeration {
public:
	Enumeration_sf_Mouse_Wheel() : das::Enumeration("Wheel") {
		external = true;
		cppName = "sf::Mouse::Wheel";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("VerticalWheel", "VerticalWheel", int64_t(sf::Mouse::Wheel::VerticalWheel), das::LineInfo());
		addIEx("HorizontalWheel", "HorizontalWheel", int64_t(sf::Mouse::Wheel::HorizontalWheel), das::LineInfo());
	}
};

class Enumeration_sf_Sensor_Type : public das::Enumeration {
public:
	Enumeration_sf_Sensor_Type() : das::Enumeration("SensorType") {
		external = true;
		cppName = "sf::Sensor::Type";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Accelerometer", "Accelerometer", int64_t(sf::Sensor::Type::Accelerometer), das::LineInfo());
		addIEx("Gyroscope", "Gyroscope", int64_t(sf::Sensor::Type::Gyroscope), das::LineInfo());
		addIEx("Magnetometer", "Magnetometer", int64_t(sf::Sensor::Type::Magnetometer), das::LineInfo());
		addIEx("Gravity", "Gravity", int64_t(sf::Sensor::Type::Gravity), das::LineInfo());
		addIEx("UserAcceleration", "UserAcceleration", int64_t(sf::Sensor::Type::UserAcceleration), das::LineInfo());
		addIEx("Orientation", "Orientation", int64_t(sf::Sensor::Type::Orientation), das::LineInfo());
		addIEx("Count", "Count", int64_t(sf::Sensor::Type::Count), das::LineInfo());
	}
};

class Enumeration_sf_Event_EventType : public das::Enumeration {
public:
	Enumeration_sf_Event_EventType() : das::Enumeration("EventType") {
		external = true;
		cppName = "sf::Event::EventType";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Closed", "Closed", int64_t(sf::Event::EventType::Closed), das::LineInfo());
		addIEx("Resized", "Resized", int64_t(sf::Event::EventType::Resized), das::LineInfo());
		addIEx("LostFocus", "LostFocus", int64_t(sf::Event::EventType::LostFocus), das::LineInfo());
		addIEx("GainedFocus", "GainedFocus", int64_t(sf::Event::EventType::GainedFocus), das::LineInfo());
		addIEx("TextEntered", "TextEntered", int64_t(sf::Event::EventType::TextEntered), das::LineInfo());
		addIEx("KeyPressed", "KeyPressed", int64_t(sf::Event::EventType::KeyPressed), das::LineInfo());
		addIEx("KeyReleased", "KeyReleased", int64_t(sf::Event::EventType::KeyReleased), das::LineInfo());
		addIEx("MouseWheelMoved", "MouseWheelMoved", int64_t(sf::Event::EventType::MouseWheelMoved), das::LineInfo());
		addIEx("MouseWheelScrolled", "MouseWheelScrolled", int64_t(sf::Event::EventType::MouseWheelScrolled), das::LineInfo());
		addIEx("MouseButtonPressed", "MouseButtonPressed", int64_t(sf::Event::EventType::MouseButtonPressed), das::LineInfo());
		addIEx("MouseButtonReleased", "MouseButtonReleased", int64_t(sf::Event::EventType::MouseButtonReleased), das::LineInfo());
		addIEx("MouseMoved", "MouseMoved", int64_t(sf::Event::EventType::MouseMoved), das::LineInfo());
		addIEx("MouseEntered", "MouseEntered", int64_t(sf::Event::EventType::MouseEntered), das::LineInfo());
		addIEx("MouseLeft", "MouseLeft", int64_t(sf::Event::EventType::MouseLeft), das::LineInfo());
		addIEx("JoystickButtonPressed", "JoystickButtonPressed", int64_t(sf::Event::EventType::JoystickButtonPressed), das::LineInfo());
		addIEx("JoystickButtonReleased", "JoystickButtonReleased", int64_t(sf::Event::EventType::JoystickButtonReleased), das::LineInfo());
		addIEx("JoystickMoved", "JoystickMoved", int64_t(sf::Event::EventType::JoystickMoved), das::LineInfo());
		addIEx("JoystickConnected", "JoystickConnected", int64_t(sf::Event::EventType::JoystickConnected), das::LineInfo());
		addIEx("JoystickDisconnected", "JoystickDisconnected", int64_t(sf::Event::EventType::JoystickDisconnected), das::LineInfo());
		addIEx("TouchBegan", "TouchBegan", int64_t(sf::Event::EventType::TouchBegan), das::LineInfo());
		addIEx("TouchMoved", "TouchMoved", int64_t(sf::Event::EventType::TouchMoved), das::LineInfo());
		addIEx("TouchEnded", "TouchEnded", int64_t(sf::Event::EventType::TouchEnded), das::LineInfo());
		addIEx("SensorChanged", "SensorChanged", int64_t(sf::Event::EventType::SensorChanged), das::LineInfo());
		addIEx("Count", "Count", int64_t(sf::Event::EventType::Count), das::LineInfo());
	}
};

class Enumeration_sf_BlendMode_Factor : public das::Enumeration {
public:
	Enumeration_sf_BlendMode_Factor() : das::Enumeration("Factor") {
		external = true;
		cppName = "sf::BlendMode::Factor";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Zero", "Zero", int64_t(sf::BlendMode::Factor::Zero), das::LineInfo());
		addIEx("One", "One", int64_t(sf::BlendMode::Factor::One), das::LineInfo());
		addIEx("SrcColor", "SrcColor", int64_t(sf::BlendMode::Factor::SrcColor), das::LineInfo());
		addIEx("OneMinusSrcColor", "OneMinusSrcColor", int64_t(sf::BlendMode::Factor::OneMinusSrcColor), das::LineInfo());
		addIEx("DstColor", "DstColor", int64_t(sf::BlendMode::Factor::DstColor), das::LineInfo());
		addIEx("OneMinusDstColor", "OneMinusDstColor", int64_t(sf::BlendMode::Factor::OneMinusDstColor), das::LineInfo());
		addIEx("SrcAlpha", "SrcAlpha", int64_t(sf::BlendMode::Factor::SrcAlpha), das::LineInfo());
		addIEx("OneMinusSrcAlpha", "OneMinusSrcAlpha", int64_t(sf::BlendMode::Factor::OneMinusSrcAlpha), das::LineInfo());
		addIEx("DstAlpha", "DstAlpha", int64_t(sf::BlendMode::Factor::DstAlpha), das::LineInfo());
		addIEx("OneMinusDstAlpha", "OneMinusDstAlpha", int64_t(sf::BlendMode::Factor::OneMinusDstAlpha), das::LineInfo());
	}
};

class Enumeration_sf_BlendMode_Equation : public das::Enumeration {
public:
	Enumeration_sf_BlendMode_Equation() : das::Enumeration("Equation") {
		external = true;
		cppName = "sf::BlendMode::Equation";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Add", "Add", int64_t(sf::BlendMode::Equation::Add), das::LineInfo());
		addIEx("Subtract", "Subtract", int64_t(sf::BlendMode::Equation::Subtract), das::LineInfo());
		addIEx("ReverseSubtract", "ReverseSubtract", int64_t(sf::BlendMode::Equation::ReverseSubtract), das::LineInfo());
		addIEx("Min", "Min", int64_t(sf::BlendMode::Equation::Min), das::LineInfo());
		addIEx("Max", "Max", int64_t(sf::BlendMode::Equation::Max), das::LineInfo());
	}
};

class Enumeration_sf_PrimitiveType : public das::Enumeration {
public:
	Enumeration_sf_PrimitiveType() : das::Enumeration("PrimitiveType") {
		external = true;
		cppName = "sf::PrimitiveType";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Points", "Points", int64_t(sf::PrimitiveType::Points), das::LineInfo());
		addIEx("Lines", "Lines", int64_t(sf::PrimitiveType::Lines), das::LineInfo());
		addIEx("LineStrip", "LineStrip", int64_t(sf::PrimitiveType::LineStrip), das::LineInfo());
		addIEx("Triangles", "Triangles", int64_t(sf::PrimitiveType::Triangles), das::LineInfo());
		addIEx("TriangleStrip", "TriangleStrip", int64_t(sf::PrimitiveType::TriangleStrip), das::LineInfo());
		addIEx("TriangleFan", "TriangleFan", int64_t(sf::PrimitiveType::TriangleFan), das::LineInfo());
		addIEx("Quads", "Quads", int64_t(sf::PrimitiveType::Quads), das::LineInfo());
		addIEx("LinesStrip", "LinesStrip", int64_t(sf::PrimitiveType::LinesStrip), das::LineInfo());
		addIEx("TrianglesStrip", "TrianglesStrip", int64_t(sf::PrimitiveType::TrianglesStrip), das::LineInfo());
		addIEx("TrianglesFan", "TrianglesFan", int64_t(sf::PrimitiveType::TrianglesFan), das::LineInfo());
	}
};

class Enumeration_sf_Texture_CoordinateType : public das::Enumeration {
public:
	Enumeration_sf_Texture_CoordinateType() : das::Enumeration("CoordinateType") {
		external = true;
		cppName = "sf::Texture::CoordinateType";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Normalized", "Normalized", int64_t(sf::Texture::CoordinateType::Normalized), das::LineInfo());
		addIEx("Pixels", "Pixels", int64_t(sf::Texture::CoordinateType::Pixels), das::LineInfo());
	}
};

class Enumeration_sf_Shader_Type : public das::Enumeration {
public:
	Enumeration_sf_Shader_Type() : das::Enumeration("Type") {
		external = true;
		cppName = "sf::Shader::Type";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Vertex", "Vertex", int64_t(sf::Shader::Type::Vertex), das::LineInfo());
		addIEx("Geometry", "Geometry", int64_t(sf::Shader::Type::Geometry), das::LineInfo());
		addIEx("Fragment", "Fragment", int64_t(sf::Shader::Type::Fragment), das::LineInfo());
	}
};

class Enumeration_sf_Text_Style : public das::Enumeration {
public:
	Enumeration_sf_Text_Style() : das::Enumeration("Style") {
		external = true;
		cppName = "sf::Text::Style";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Regular", "Regular", int64_t(sf::Text::Style::Regular), das::LineInfo());
		addIEx("Bold", "Bold", int64_t(sf::Text::Style::Bold), das::LineInfo());
		addIEx("Italic", "Italic", int64_t(sf::Text::Style::Italic), das::LineInfo());
		addIEx("Underlined", "Underlined", int64_t(sf::Text::Style::Underlined), das::LineInfo());
		addIEx("StrikeThrough", "StrikeThrough", int64_t(sf::Text::Style::StrikeThrough), das::LineInfo());
	}
};

class Enumeration_sf_VertexBuffer_Usage : public das::Enumeration {
public:
	Enumeration_sf_VertexBuffer_Usage() : das::Enumeration("Usage") {
		external = true;
		cppName = "sf::VertexBuffer::Usage";
		baseType = (das::Type) das::ToBasicType<int>::type;
		addIEx("Stream", "Stream", int64_t(sf::VertexBuffer::Usage::Stream), das::LineInfo());
		addIEx("Dynamic", "Dynamic", int64_t(sf::VertexBuffer::Usage::Dynamic), das::LineInfo());
		addIEx("Static", "Static", int64_t(sf::VertexBuffer::Usage::Static), das::LineInfo());
	}
};

