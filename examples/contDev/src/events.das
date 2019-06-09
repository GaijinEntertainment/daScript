struct EventEntityCreated {}

struct EventEntityRecreated {}

struct EventEntityDestroyed {}

struct EventComponentChanged
	[cppType="const char*"] name:string
	flags:uint
	[cppType="component_t"] hash:uint
	[cppType="component_index_t"] index:uint

struct EventAnyComponentChanged
	[cppType="const char*"] name:string
	flags:uint
	[cppType="component_t"] hash:uint
	[cppType="component_index_t"] index:uint

