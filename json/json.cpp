#include "daScript/daScript.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h" 

using namespace das;
using namespace std;

typedef rapidjson::Value JsValue;

MAKE_TYPE_FACTORY(JsValue, JsValue);

#define DAS_MEMBER_PTR(memX)	decltype(&(memX)),&(memX)

struct JsValueTypeAnnotation : TypeAnnotation {

	template <typename Fun, Fun BOOL_PROP>
	struct SimNode_GetBoolProperty : SimNode {
		DAS_BOOL_NODE;
		SimNode_GetBoolProperty(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
		__forceinline bool compute(Context & context) {
			auto pv = (JsValue *) subexpr->evalPtr(context);
			return (pv->*BOOL_PROP)();
		}
		SimNode * subexpr;
	};

	JsValueTypeAnnotation() : TypeAnnotation ("JsValue") {}

	virtual bool canMove() const { return false; }
	virtual bool canCopy() const { return false; }
	virtual bool isPod() const { return false; }
	virtual bool isRefType() const { return true; }
	virtual size_t getSizeOf() const { return sizeof(JsValue); }
	virtual size_t getAlignOf() const { return alignof(JsValue); }

	virtual TypeDeclPtr makeFieldType ( const string & fn ) const { 
		if (fn == "is_object") {
			return make_shared<TypeDecl>(Type::tBool);
		}
		return nullptr; 
	}

	virtual TypeDeclPtr makeSafeFieldType ( const string & ) const { 
		assert(0 && "implement");
		return nullptr; 
	}
	virtual SimNode * simulateRef2Value ( Context &, const LineInfo &, SimNode * ) const { 
		assert(0 && "implement");
		return nullptr; 
	}

	virtual SimNode * simulateGetField ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr ) const { 
		if (fn == "is_object") return context.code.makeNode<SimNode_GetBoolProperty<DAS_MEMBER_PTR(JsValue::IsObject)>>(at, subexpr);
		return nullptr; 
	}

	virtual SimNode * simulateGetFieldR2V ( const string &, Context &, const LineInfo &, SimNode * ) const { 
		assert(0 && "implement");
		return nullptr; 
	}

	virtual SimNode * simulateSafeGetField ( const string &, Context &, const LineInfo &, SimNode * ) const { 
		assert(0 && "implement");
		return nullptr; 
	}

	virtual SimNode * simulateSafeGetFieldPtr ( const string &, Context &, const LineInfo &, SimNode * ) const { 
		assert(0 && "implement");
		return nullptr; 
	}

	void print(stringstream & ss, const JsValue & value) const {
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		value.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
		ss << sb.GetString();
	}

	virtual void debug ( stringstream & ss, void * ptr, PrintFlags ) const { 
		print(ss, *((JsValue *)ptr));
	}
	virtual void debug ( stringstream & ss, vec4f vptr, PrintFlags ) const { 
		print(ss, *cast<JsValue *>::to(vptr));
	}

};


class Module_RapidJson : public Module {
public:
	Module_RapidJson() : Module("RapidJson") {
		ModuleLibrary lib;
		lib.addModule(this);
		lib.addBuiltInModule();
	}
};

REGISTER_MODULE(Module_RapidJson);
