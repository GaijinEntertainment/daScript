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

void _dasSFML_virtual_8_create(sf::WindowBase & self,sf::VideoMode mode,const sf::String & title,unsigned int style) {
	self.create(mode,title,style);
}

void _dasSFML_virtual_9_close(sf::WindowBase & self) {
	self.close();
}

void _dasSFML_virtual_10_create(sf::Window & self,sf::VideoMode mode,const sf::String & title,unsigned int style) {
	self.create(mode,title,style);
}

void _dasSFML_virtual_11_create(sf::Window & self,sf::VideoMode mode,const sf::String & title,unsigned int style,const sf::ContextSettings & settings) {
	self.create(mode,title,style,settings);
}

void _dasSFML_virtual_12_close(sf::Window & self) {
	self.close();
}

std::size_t _dasSFML_virtual_13_getPointCount(const sf::CircleShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_14_getPoint(const sf::CircleShape & self,std::size_t index) {
	return self.getPoint(index);
}

std::size_t _dasSFML_virtual_15_getPointCount(const sf::ConvexShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_16_getPoint(const sf::ConvexShape & self,std::size_t index) {
	return self.getPoint(index);
}

std::size_t _dasSFML_virtual_17_getPointCount(const sf::RectangleShape & self) {
	return self.getPointCount();
}

sf::Vector2<float> _dasSFML_virtual_18_getPoint(const sf::RectangleShape & self,std::size_t index) {
	return self.getPoint(index);
}

bool _dasSFML_virtual_19_isSrgb(const sf::RenderTarget & self) {
	return self.isSrgb();
}

bool _dasSFML_virtual_20_setActive(sf::RenderTarget & self,bool active) {
	return self.setActive(active);
}

bool _dasSFML_virtual_21_setActive(sf::RenderTexture & self,bool active) {
	return self.setActive(active);
}

sf::Vector2<unsigned int> _dasSFML_virtual_22_getSize(const sf::RenderTexture & self) {
	return self.getSize();
}

bool _dasSFML_virtual_23_isSrgb(const sf::RenderTexture & self) {
	return self.isSrgb();
}

sf::Vector2<unsigned int> _dasSFML_virtual_24_getSize(const sf::RenderWindow & self) {
	return self.getSize();
}

bool _dasSFML_virtual_25_isSrgb(const sf::RenderWindow & self) {
	return self.isSrgb();
}

bool _dasSFML_virtual_26_setActive(sf::RenderWindow & self,bool active) {
	return self.setActive(active);
}

