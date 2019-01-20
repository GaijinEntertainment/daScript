#include "daScript/daScript.h"

#include "test_profile.h"

using namespace std;
using namespace das;

__noinline void updateObject ( Object & obj ) {
    obj.pos.x += obj.vel.x;
    obj.pos.y += obj.vel.y;
    obj.pos.z += obj.vel.z;
}

void updateTest ( ObjectArray & objects ) {
    for ( auto & obj : objects ) {
        updateObject(obj);
    }
}

void update10000 ( ObjectArray & objects, Context * context ) {
    auto updateFn = context->findFunction("update");
	if (!updateFn) {
		context->throw_error("update not exported");
		return;
	}
	for ( auto & obj : objects ) {
        vec4f args[1] = { cast<Object &>::from(obj) };
        context->eval(updateFn,  args);
    }
}

void update10000ks ( ObjectArray & objects, Context * context ) {
    auto ksUpdateFn = context->findFunction("ks_update");
	if (!ksUpdateFn) {
		context->throw_error("ks_update not exported");
		return;
	}
    for ( auto & obj : objects ) {
        vec4f args[2] = { cast<float3 &>::from(obj.pos), cast<float3>::from(obj.vel) };
        context->eval(ksUpdateFn,  args);
    }
}

struct ObjectStructureTypeAnnotation : ManagedStructureAnnotation <Object> {
    ObjectStructureTypeAnnotation() : ManagedStructureAnnotation ("Object") {
        addField("position", offsetof(Object,pos),make_shared<TypeDecl>(Type::tFloat3));
        addField("velocity", offsetof(Object,vel),make_shared<TypeDecl>(Type::tFloat3));
    }
};

MAKE_TYPE_FACTORY(Object, Object)
MAKE_TYPE_FACTORY(ObjectArray, ObjectArray)

int AddOne(int a) {
    return a+1;
}

// ES

struct EsAttribute {
    EsAttribute() = default;
    EsAttribute ( const string & n, uint32_t sz, bool rf, vec4f d )
        : def(d), name(n), size(sz), ref(rf) {}
	vec4f       def = v_zero();
    string      name;
    uint32_t    size = 0;
    bool        ref;
};

struct EsAttributeTable {
    vector<EsAttribute> attributes;
};

struct EsPassAttributeTable  : EsAttributeTable {
    string  pass;
    string  functionName;
    SimFunction * functionPtr = nullptr;
};

struct EsComponent {
    string      name;
    void *      data = nullptr;
    uint32_t    size = 0;
    uint32_t    stride = 0;
    bool        boxed = false;

	EsComponent() = default;
	EsComponent(const string & n, void * d, size_t sz, size_t st, bool bx) :
		name(n), data(d), size(uint32_t(sz)), stride(uint32_t(st)), boxed(bx) {}
};

vector<EsPassAttributeTable>    g_esPassTable;
vector<EsAttributeTable>        g_esBlockTable;

struct EsFunctionAnnotation : FunctionAnnotation {
    EsFunctionAnnotation() : FunctionAnnotation("es") { }
    void buildAttributeTable ( EsAttributeTable & tab, const vector<VariablePtr> & arguments, string & err  ) {
        for ( const auto & arg : arguments ) {
            vec4f def = v_zero();
            if ( arg->init ) {
                if ( arg->init->rtti_isConstant() && !arg->init->rtti_isStringConstant() ) {
                    auto pConst = static_pointer_cast<ExprConst>(arg->init);
                    def = pConst->value;
                } else {
                    err += "default for " + arg->name + " is not a constant\n";
                }
            }
            tab.attributes.emplace_back(arg->name, arg->type->getSizeOf(), arg->type->isRef(), def);
        }
    }
    virtual bool apply ( ExprBlock * block, const AnnotationArgumentList &, string & err ) override {
        if ( block->arguments.empty() ) {
            err = "block needs arguments";
            return false;
        }
        return true;
    }
    virtual bool finalize ( ExprBlock * block, const AnnotationArgumentList &, string & err ) override {
        size_t index = g_esBlockTable.size();
        block->annotationData = (void *)(index | 0xbad00000);
        EsAttributeTable tab;
        buildAttributeTable(tab, block->arguments, err);
        g_esBlockTable.push_back(tab);
        return err.empty();
    }
    virtual bool apply ( const FunctionPtr & func, const AnnotationArgumentList &, string & err ) override {
        if ( func->arguments.empty() ) {
            err = "function needs arguments";
            return false;
        }
		func->exports = true;
        return true;
    };
    virtual bool finalize ( const FunctionPtr & func, const AnnotationArgumentList & args, string & err ) override {
        EsPassAttributeTable tab;
        if ( auto pp = args.find("pass", Type::tString) ) {
            tab.pass = pp->sValue;
        } else {
            err = "pass is not specified";
            return false;
        }
        tab.functionName = func->name;
        buildAttributeTable(tab, func->arguments, err);
        g_esPassTable.push_back(tab);
        return err.empty();
    }
};

bool EsRunPass ( Context & context, EsPassAttributeTable & table, const vector<EsComponent> & components, uint32_t totalComponents ) {
    if ( table.functionPtr==nullptr )
        table.functionPtr = context.findFunction(table.functionName.c_str());
	if (!table.functionPtr) {
		context.throw_error("function not found");
		return false;
	}
	vec4f * _args = (vec4f *)(alloca(table.attributes.size() * sizeof(vec4f)));
    context.callEx(table.functionPtr, _args, nullptr, 0, [&](SimNode * code){
        uint32_t nAttr = (uint32_t) table.attributes.size();
        vec4f * args = _args;
		char **		data	= (char **) alloca(nAttr * sizeof(char *));
		uint32_t *	stride	= (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        uint32_t *  size    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
		bool *		boxed	= (bool *) alloca(nAttr * sizeof(bool));
        bool *      ref     = (bool *) alloca(nAttr * sizeof(bool));
        for ( uint32_t a=0; a!=nAttr; ++a ) {
            auto it = find_if ( components.begin(), components.end(), [&](const EsComponent & esc){
                return esc.name == table.attributes[a].name;
            });
            if ( it != components.end() ) {
                data[a]   = (char *) it->data;
                stride[a] = it->stride;
                boxed[a]  = it->boxed;
            } else {
                data[a] = nullptr;
                args[a] = table.attributes[a].def;
            }
            size[a] = table.attributes[a].size;
            ref[a] = table.attributes[a].ref;
        }
        for ( uint32_t i=0; i != totalComponents; ++i ) {
            for ( uint32_t a=0; a!=nAttr; ++a ) {
                if ( data[a] ) {
                    char * src =  boxed[a] ? *((char **)data[a]) : data[a];
                    if ( !ref[a] ) {
                        args[a] = v_ldu((float *)src);
                    } else {
                        *((void **)&args[a]) = src;
                    }
                    data[a] += stride[a];
                }
            }
            code->eval(context);
            context.stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            if ( context.stopFlags & EvalFlags::stopForThrow ) {
                // TODO: report exception here??
                return;
            }
        }
    });
    return true;
}

bool EsRunBlock ( Context & context, Block block, const vector<EsComponent> & components, uint32_t totalComponents ) {
    auto * closure = (SimNode_ClosureBlock *) block.body;
    size_t index = intptr_t(closure->annotationData);
    if ( (index & 0xfff00000) != 0xbad00000 ) {
        context.throw_error("invalid block");
        return false;
    }
    index &= 0x000fffff;
    if ( index<0 || index>=g_esBlockTable.size() ) {
        context.throw_error("invalid block");
        return false;
    }
    EsAttributeTable & table = g_esBlockTable[index];
    uint32_t nAttr = (uint32_t) table.attributes.size();
    vec4f * _args = (vec4f *)(alloca(table.attributes.size() * sizeof(vec4f)));
    context.invokeEx(block, _args, nullptr, [&](SimNode * code){
        vec4f * args = _args;
        char **		data	= (char **) alloca(nAttr * sizeof(char *));
        uint32_t *	stride	= (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        uint32_t *  size    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        bool *		boxed	= (bool *) alloca(nAttr * sizeof(bool));
        bool *      ref     = (bool *) alloca(nAttr * sizeof(bool));
        for ( uint32_t a=0; a!=nAttr; ++a ) {
            auto it = find_if ( components.begin(), components.end(), [&](const EsComponent & esc){
                return esc.name == table.attributes[a].name;
            });
            if ( it != components.end() ) {
                data[a]   = (char *) it->data;
                stride[a] = it->stride;
                boxed[a]  = it->boxed;
            } else {
                data[a] = nullptr;
                args[a] = table.attributes[a].def;
            }
            size[a] = table.attributes[a].size;
            ref[a] = table.attributes[a].ref;
        }
        for ( uint32_t i=0; i != totalComponents; ++i ) {
            for ( uint32_t a=0; a!=nAttr; ++a ) {
                if ( data[a] ) {
                    char * src =  boxed[a] ? *((char **)data[a]) : data[a];
                    if ( !ref[a] ) {
                        args[a] = v_ldu((float *)src);
                    } else {
                        *((void **)&args[a]) = src;
                    }
                    data[a] += stride[a];
                }
            }
            code->eval(context);
            context.stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            if ( context.stopFlags & EvalFlags::stopForThrow ) {
                // TODO: report exception here??
                return;
            }
        }
    });
    return true;
}


constexpr int g_total = 100000;
vector<float3>   g_pos ( g_total );
vector<float3>   g_vel ( g_total );
vector<float3 *> g_velBoxed ( g_total );
vector<EsComponent> g_components;

void initEsComponents() {
    // build components
    float f = 1.0f;
    for ( int i=0; i != g_total; ++i ) {
        g_pos[i] = { f++, f+1, f+2 };
        g_vel[i] = { 1.0f, 2.0f, 3.0f };
        g_velBoxed[i] = &g_vel[i];
    }
    g_components.clear();
    g_components.emplace_back("pos",      g_pos.data(),      sizeof(float3), sizeof(float3),   false);
    g_components.emplace_back("vel",      g_vel.data(),      sizeof(float3), sizeof(float3),   false);
    g_components.emplace_back("velBoxed", g_velBoxed.data(), sizeof(float3), sizeof(float3 *), true );
}

void verifyEsComponents() {
	float t = 1.0f;
	float f = 1.0f;
	for (int i = 0; i != g_total; ++i) {
		float3 apos = { f++, f + 1, f + 2 };
		float3 avel = { 1.0f, 2.0f, 3.0f };
		float3 npos;
		npos.x = apos.x + avel.x * t;
        npos.y = apos.y + avel.y * t;
        npos.z = apos.z + avel.z * t;
        assert(g_pos[i].x==npos.x && g_pos[i].y==npos.y &&g_pos[i].z==npos.z );
    }
}

void testEsUpdate ( char * pass, Context * ctx ) {
    for ( auto & tab : g_esPassTable ) {
        if ( tab.pass==pass ) {
            EsRunPass(*ctx, tab, g_components, g_total);
        }
    }
}

void queryEs (Block block, Context * context) {
    EsRunBlock(*context, block, g_components, g_total);
}

#include <unordered_map>

__noinline int testDict(Array * arr) {
    unordered_map<string,int> tab;
    char ** data = (char **) arr->data;
    int maxOcc = 0;
    for ( uint32_t t = 0; t !=arr->size; ++t ) {
		maxOcc = max(++tab[data[t]], maxOcc);
    }
    return maxOcc;
}

__noinline bool isprime(int n) {
	for (int i = 2; i != n; ++i) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

__noinline int testPrimes(int n) {
	int count = 0;
	for (int i = 2; i != n + 1; ++i) {
		if (isprime(i)) {
			++count;
		}
	}
	return count;
}

__noinline int testFibR(int n) {
	if (n < 2) {
		return n;
	}
	else {
		return testFibR(n - 1) + testFibR(n - 2);
	}
}

__noinline int testFibI(int n) {
	int last = 0;
	int cur = 1;
	for (int i = 0; i != n - 1; ++i) {
		int tmp = cur;
		cur += last;
		last = tmp;
	}
	return cur;
}

__noinline void particles(ObjectArray & objects, int count) {
	for (int i = 0; i != count; ++i) {
		for (auto & obj : objects) {
			updateObject(obj);
		}
	}
}

__noinline void particlesI(ObjectArray & objects, int count) {
	for (int i = 0; i != count; ++i) {
		for (auto & obj : objects) {
			obj.pos.x += obj.vel.x;
			obj.pos.y += obj.vel.y;
			obj.pos.z += obj.vel.z;
		}
	}
}

__noinline void testParticles(int count) {
	ObjectArray objects;
	objects.resize(50000);
	particles(objects, count);
}

__noinline void testParticlesI(int count) {
	ObjectArray objects;
	objects.resize(50000);
	particlesI(objects, count);
}

__noinline void testTryCatch(Context * context) {
	int arr[1000];
	int cnt = 0;
	for (int j = 0; j != 100; ++j) {
		int fail = 0;
		for (int i = 0; i != 2000; ++i) {
			try {
				if (i < 0 || i>=1000) throw runtime_error("range check error");
				cnt += arr[i];
			}
			catch (...) {
				fail++;
			}
		}
		if (fail != 1000) {
			context->throw_error("test optimized out");
			return;
		}
	}
}

class Module_TestProfile : public Module {
public:
    Module_TestProfile() : Module("testProfile") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // function annotations
        addAnnotation(make_shared<EsFunctionAnnotation>());
        // register types
        addAnnotation(make_shared<ObjectStructureTypeAnnotation>());
        addAnnotation(make_shared<ManagedVectorAnnotation<Object>>("ObjectArray",lib.makeHandleType("Object")));
        // register functions
        addExtern<decltype(AddOne),AddOne>(*this,lib,"AddOne");
        addExtern<decltype(updateObject),updateObject>(*this,lib,"interopUpdate");
        addExtern<decltype(updateTest),updateTest>(*this,lib,"interopUpdateTest");
        addExtern<decltype(update10000), update10000>(*this,lib,"update10000");
        addExtern<decltype(update10000ks), update10000ks>(*this,lib,"update10000ks");
        // es
        addExtern<decltype(queryEs), queryEs>(*this, lib, "queryEs");
        addExtern<decltype(testEsUpdate), testEsUpdate>(*this, lib, "testEsUpdate");
        addExtern<decltype(initEsComponents), initEsComponents>(*this, lib, "initEsComponents");
        addExtern<decltype(verifyEsComponents), verifyEsComponents>(*this, lib, "verifyEsComponents");
        // C++ copy of all tests
		addExtern<decltype(testPrimes), testPrimes>(*this, lib, "testPrimes");
        addExtern<decltype(testDict), testDict>(*this, lib, "testDict");
		addExtern<decltype(testFibR), testFibR>(*this, lib, "testFibR");
		addExtern<decltype(testFibI), testFibI>(*this, lib, "testFibI");
		addExtern<decltype(testParticles), testParticles>(*this, lib, "testParticles");
		addExtern<decltype(testParticlesI), testParticlesI>(*this, lib, "testParticlesI");
		addExtern<decltype(testTryCatch), testTryCatch>(*this, lib, "testTryCatch");
    }
};

REGISTER_MODULE(Module_TestProfile);


