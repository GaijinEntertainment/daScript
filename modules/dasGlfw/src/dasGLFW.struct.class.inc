// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

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

struct GLFWimage_GeneratedAnnotation : ManagedStructureAnnotation<GLFWimage> {
	GLFWimage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWimage", ml, "GLFWimage") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(width)>("width","width");
		addField<DAS_BIND_MANAGED_FIELD(height)>("height","height");
		addField<DAS_BIND_MANAGED_FIELD(pixels)>("pixels","pixels");
	}
};

struct GLFWgamepadstate_GeneratedAnnotation : ManagedStructureAnnotation<GLFWgamepadstate> {
	GLFWgamepadstate_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("GLFWgamepadstate", ml, "GLFWgamepadstate") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(buttons)>("buttons","buttons");
		addField<DAS_BIND_MANAGED_FIELD(axes)>("axes","axes");
	}
};

