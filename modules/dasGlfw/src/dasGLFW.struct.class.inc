// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

// from GLFW/glfw3.h:2026:16
struct GLFWvidmode_GeneratedAnnotation : ManagedStructureAnnotation<GLFWvidmode> {
	GLFWvidmode_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWvidmode", ml, "GLFWvidmode") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(width)>("width","width");
		addField<DAS_BIND_MANAGED_FIELD(height)>("height","height");
		addField<DAS_BIND_MANAGED_FIELD(redBits)>("redBits","redBits");
		addField<DAS_BIND_MANAGED_FIELD(greenBits)>("greenBits","greenBits");
		addField<DAS_BIND_MANAGED_FIELD(blueBits)>("blueBits","blueBits");
		addField<DAS_BIND_MANAGED_FIELD(refreshRate)>("refreshRate","refreshRate");
	}
};

// from GLFW/glfw3.h:2060:16
struct GLFWgammaramp_GeneratedAnnotation : ManagedStructureAnnotation<GLFWgammaramp> {
	GLFWgammaramp_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWgammaramp", ml, "GLFWgammaramp") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(red)>("red","red");
		addField<DAS_BIND_MANAGED_FIELD(green)>("green","green");
		addField<DAS_BIND_MANAGED_FIELD(blue)>("blue","blue");
		addField<DAS_BIND_MANAGED_FIELD(size)>("size","size");
	}
};

// from GLFW/glfw3.h:2089:16
struct GLFWimage_GeneratedAnnotation : ManagedStructureAnnotation<GLFWimage> {
	GLFWimage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWimage", ml, "GLFWimage") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(width)>("width","width");
		addField<DAS_BIND_MANAGED_FIELD(height)>("height","height");
		addField<DAS_BIND_MANAGED_FIELD(pixels)>("pixels","pixels");
	}
};

// from GLFW/glfw3.h:2113:16
struct GLFWgamepadstate_GeneratedAnnotation : ManagedStructureAnnotation<GLFWgamepadstate> {
	GLFWgamepadstate_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWgamepadstate", ml, "GLFWgamepadstate") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(buttons)>("buttons","buttons");
		addField<DAS_BIND_MANAGED_FIELD(axes)>("axes","axes");
	}
};

// from GLFW/glfw3.h:2137:16
struct GLFWallocator_GeneratedAnnotation : ManagedStructureAnnotation<GLFWallocator> {
	GLFWallocator_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWallocator", ml, "GLFWallocator") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(user)>("user","user");
	}
};

