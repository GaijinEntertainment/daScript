// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

sf::Int64 _dasSFML_virtual_0_read(sf::FileInputStream & self,void * data,sf::Int64 size) {
	return self.read(data,size);
}

sf::Int64 _dasSFML_virtual_1_seek(sf::FileInputStream & self,sf::Int64 position) {
	return self.seek(position);
}

sf::Int64 _dasSFML_virtual_2_tell(sf::FileInputStream & self) {
	return self.tell();
}

sf::Int64 _dasSFML_virtual_3_getSize(sf::FileInputStream & self) {
	return self.getSize();
}

sf::Int64 _dasSFML_virtual_4_read(sf::MemoryInputStream & self,void * data,sf::Int64 size) {
	return self.read(data,size);
}

sf::Int64 _dasSFML_virtual_5_seek(sf::MemoryInputStream & self,sf::Int64 position) {
	return self.seek(position);
}

sf::Int64 _dasSFML_virtual_6_tell(sf::MemoryInputStream & self) {
	return self.tell();
}

sf::Int64 _dasSFML_virtual_7_getSize(sf::MemoryInputStream & self) {
	return self.getSize();
}

sf::String _dasSFML_static_8_getString(const sf::Clipboard &) {
	return sf::Clipboard::getString();
}

void _dasSFML_static_9_setString(const sf::Clipboard &,const sf::String & text) {
	sf::Clipboard::setString(text);
}

bool _dasSFML_static_10_isExtensionAvailable(const sf::Context &,const char * name) {
	return sf::Context::isExtensionAvailable(name);
}

const sf::Context * _dasSFML_static_11_getActiveContext(const sf::Context &) {
	return sf::Context::getActiveContext();
}

sf::Uint64 _dasSFML_static_12_getActiveContextId(const sf::Context &) {
	return sf::Context::getActiveContextId();
}

bool _dasSFML_static_13_isConnected(const sf::Joystick &,unsigned int joystick) {
	return sf::Joystick::isConnected(joystick);
}

unsigned int _dasSFML_static_14_getButtonCount(const sf::Joystick &,unsigned int joystick) {
	return sf::Joystick::getButtonCount(joystick);
}

bool _dasSFML_static_15_hasAxis(const sf::Joystick &,unsigned int joystick,sf::Joystick::Axis axis) {
	return sf::Joystick::hasAxis(joystick,axis);
}

bool _dasSFML_static_16_isButtonPressed(const sf::Joystick &,unsigned int joystick,unsigned int button) {
	return sf::Joystick::isButtonPressed(joystick,button);
}

float _dasSFML_static_17_getAxisPosition(const sf::Joystick &,unsigned int joystick,sf::Joystick::Axis axis) {
	return sf::Joystick::getAxisPosition(joystick,axis);
}

void _dasSFML_static_18_update(const sf::Joystick &) {
	sf::Joystick::update();
}

bool _dasSFML_static_19_isKeyPressed(const sf::Keyboard &,sf::Keyboard::Key key) {
	return sf::Keyboard::isKeyPressed(key);
}

void _dasSFML_static_20_setVirtualKeyboardVisible(const sf::Keyboard &,bool visible) {
	sf::Keyboard::setVirtualKeyboardVisible(visible);
}

bool _dasSFML_static_21_isButtonPressed(const sf::Mouse &,sf::Mouse::Button button) {
	return sf::Mouse::isButtonPressed(button);
}

sf::Vector2<int> _dasSFML_static_22_getPosition(const sf::Mouse &) {
	return sf::Mouse::getPosition();
}

sf::Vector2<int> _dasSFML_static_23_getPosition(const sf::Mouse &,const sf::WindowBase & relativeTo) {
	return sf::Mouse::getPosition(relativeTo);
}

void _dasSFML_static_24_setPosition(const sf::Mouse &,const sf::Vector2<int> & position) {
	sf::Mouse::setPosition(position);
}

void _dasSFML_static_25_setPosition(const sf::Mouse &,const sf::Vector2<int> & position,const sf::WindowBase & relativeTo) {
	sf::Mouse::setPosition(position,relativeTo);
}

bool _dasSFML_static_26_isAvailable(const sf::Sensor &,sf::Sensor::Type sensor) {
	return sf::Sensor::isAvailable(sensor);
}

void _dasSFML_static_27_setEnabled(const sf::Sensor &,sf::Sensor::Type sensor,bool enabled) {
	sf::Sensor::setEnabled(sensor,enabled);
}

sf::Vector3<float> _dasSFML_static_28_getValue(const sf::Sensor &,sf::Sensor::Type sensor) {
	return sf::Sensor::getValue(sensor);
}

bool _dasSFML_static_29_isDown(const sf::Touch &,unsigned int finger) {
	return sf::Touch::isDown(finger);
}

sf::Vector2<int> _dasSFML_static_30_getPosition(const sf::Touch &,unsigned int finger) {
	return sf::Touch::getPosition(finger);
}

sf::Vector2<int> _dasSFML_static_31_getPosition(const sf::Touch &,unsigned int finger,const sf::WindowBase & relativeTo) {
	return sf::Touch::getPosition(finger,relativeTo);
}

sf::VideoMode _dasSFML_static_32_getDesktopMode(const sf::VideoMode &) {
	return sf::VideoMode::getDesktopMode();
}

const std::vector<sf::VideoMode> & _dasSFML_static_33_getFullscreenModes(const sf::VideoMode &) {
	return sf::VideoMode::getFullscreenModes();
}

bool _dasSFML_static_34_isAvailable(const sf::Vulkan &,bool requireGraphics) {
	return sf::Vulkan::isAvailable(requireGraphics);
}

const std::vector<const char *> & _dasSFML_static_35_getGraphicsRequiredInstanceExtensions(const sf::Vulkan &) {
	return sf::Vulkan::getGraphicsRequiredInstanceExtensions();
}

void _dasSFML_virtual_36_create(sf::WindowBase & self,sf::VideoMode mode,const sf::String & title,unsigned int style) {
	self.create(mode,title,style);
}

void _dasSFML_virtual_37_close(sf::WindowBase & self) {
	self.close();
}

void _dasSFML_virtual_38_create(sf::Window & self,sf::VideoMode mode,const sf::String & title,unsigned int style) {
	self.create(mode,title,style);
}

void _dasSFML_virtual_39_create(sf::Window & self,sf::VideoMode mode,const sf::String & title,unsigned int style,const sf::ContextSettings & settings) {
	self.create(mode,title,style,settings);
}

void _dasSFML_virtual_40_close(sf::Window & self) {
	self.close();
}

std::size_t _dasSFML_virtual_41_getPointCount(const sf::CircleShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_42_getPoint(const sf::CircleShape & self,std::size_t index) {
	return self.getPoint(index);
}

std::size_t _dasSFML_virtual_43_getPointCount(const sf::ConvexShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_44_getPoint(const sf::ConvexShape & self,std::size_t index) {
	return self.getPoint(index);
}

void _dasSFML_static_45_bind(const sf::Texture &,const sf::Texture * texture,sf::Texture::CoordinateType coordinateType) {
	sf::Texture::bind(texture,coordinateType);
}

unsigned int _dasSFML_static_46_getMaximumSize(const sf::Texture &) {
	return sf::Texture::getMaximumSize();
}

std::size_t _dasSFML_virtual_47_getPointCount(const sf::RectangleShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_48_getPoint(const sf::RectangleShape & self,std::size_t index) {
	return self.getPoint(index);
}

bool _dasSFML_virtual_49_isSrgb(const sf::RenderTarget & self) {
	return self.isSrgb();
}

bool _dasSFML_virtual_50_setActive(sf::RenderTarget & self,bool active) {
	return self.setActive(active);
}

unsigned int _dasSFML_static_51_getMaximumAntialiasingLevel(const sf::RenderTexture &) {
	return sf::RenderTexture::getMaximumAntialiasingLevel();
}

bool _dasSFML_virtual_52_setActive(sf::RenderTexture & self,bool active) {
	return self.setActive(active);
}

sf::Vector2<unsigned int> _dasSFML_virtual_53_getSize(const sf::RenderTexture & self) {
	return self.getSize();
}

bool _dasSFML_virtual_54_isSrgb(const sf::RenderTexture & self) {
	return self.isSrgb();
}

sf::Vector2<unsigned int> _dasSFML_virtual_55_getSize(const sf::RenderWindow & self) {
	return self.getSize();
}

bool _dasSFML_virtual_56_isSrgb(const sf::RenderWindow & self) {
	return self.isSrgb();
}

bool _dasSFML_virtual_57_setActive(sf::RenderWindow & self,bool active) {
	return self.setActive(active);
}

void _dasSFML_static_58_bind(const sf::Shader &,const sf::Shader * shader) {
	sf::Shader::bind(shader);
}

bool _dasSFML_static_59_isAvailable(const sf::Shader &) {
	return sf::Shader::isAvailable();
}

bool _dasSFML_static_60_isGeometryAvailable(const sf::Shader &) {
	return sf::Shader::isGeometryAvailable();
}

void _dasSFML_static_61_bind(const sf::VertexBuffer &,const sf::VertexBuffer * vertexBuffer) {
	sf::VertexBuffer::bind(vertexBuffer);
}

bool _dasSFML_static_62_isAvailable(const sf::VertexBuffer &) {
	return sf::VertexBuffer::isAvailable();
}

