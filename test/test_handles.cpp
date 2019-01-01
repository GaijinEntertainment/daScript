#include "precomp.h"

#include "ast.h"
#include "ast_interop.h"
#include "ast_handle.h"
#include "interop.h"

using namespace std;
using namespace yzg;

struct TestObjectFoo {
    int fooData;
};

template <>
struct typeFactory<TestObjectFoo> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectFoo");
    }
};

struct TestObjectBar {
    TestObjectFoo * fooPtr;
    float           barData;
};

template <>
struct typeFactory<TestObjectBar> {
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        return library.makeHandleType("TestObjectBar");
    }
};

struct TestObjectFooAnnotation : ManagedStructureAnnotation <TestObjectFoo> {
    TestObjectFooAnnotation() : ManagedStructureAnnotation ("TestObjectFoo") {
        addField("fooData", offsetof(TestObjectFoo,fooData),make_shared<TypeDecl>(Type::tInt));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        TestObjectFoo * of = (TestObjectFoo *) data;
        ss << "{fooData=" << of->fooData << "}";
    }
};

struct TestObjectBarAnnotation : ManagedStructureAnnotation <TestObjectBar> {
    TestObjectBarAnnotation(ModuleLibrary & lib) : ManagedStructureAnnotation ("TestObjectBar") {
        auto fooPtr = make_shared<TypeDecl>(Type::tPointer);
        fooPtr->firstType = lib.makeHandleType("TestObjectFoo");
        addField("fooPtr", offsetof(TestObjectBar,fooPtr),fooPtr);
        addField("barData", offsetof(TestObjectBar,barData),make_shared<TypeDecl>(Type::tFloat));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        TestObjectBar * ob = (TestObjectBar *) data;
        ss << "{";
        if ( ob->fooPtr ) {
            ss << "fooPtr.fooData=" << ob->fooPtr->fooData;
        } else {
            ss << "fooPtr=null";
        }
        ss << ",barData=" << ob->barData << "}";
    }
};

void testFoo ( TestObjectFoo & foo ) {
    foo.fooData = 1234;
}

void testAdd ( int & a, int b ) {
    a += b;
}

struct IntFields {
    map<string,int32_t> fields;
};

struct IntFieldsAnnotation : StructureTypeAnnotation {
    
    // NOTE - SafeGetFieldPtr is not necessary, since its Int always
    
    // FIELD .
    struct SimNode_IntFieldDeref : SimNode {
        SimNode_IntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode(at), value(rv), name(n) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 rv = value->eval(context);
            YZG_EXCEPTION_POINT;
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return cast<int32_t *>::from(&it->second);
                } else {
                    context.throw_error("field not found");
                    return _mm_setzero_ps();
                }
            } else {
                context.throw_error("dereferencing null pointer");
                return _mm_setzero_ps();
            }
        }
        SimNode *   value;
        char *      name;
    };
    
    // FIELD ?.
    struct SimNode_SafeIntFieldDeref : SimNode_IntFieldDeref {
        SimNode_SafeIntFieldDeref ( const LineInfo & at, SimNode * rv, char * n ) : SimNode_IntFieldDeref(at,rv,n) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 rv = value->eval(context);
            YZG_EXCEPTION_POINT;
            if ( IntFields * prv = cast<IntFields *>::to(rv) ) {
                auto it = prv->fields.find(name);
                if ( it != prv->fields.end() ) {
                    return cast<int32_t *>::from(&it->second);
                } else {
                    return _mm_setzero_ps();
                }
            } else {
                return _mm_setzero_ps();
            }
        }
    };
    
    IntFieldsAnnotation() : StructureTypeAnnotation("IntFields") {}
    virtual TypeAnnotationPtr clone ( const TypeAnnotationPtr & p = nullptr ) const override {
        shared_ptr<IntFieldsAnnotation> cp =  p ? static_pointer_cast<IntFieldsAnnotation>(p) : make_shared<IntFieldsAnnotation>();
        return StructureTypeAnnotation::clone(cp);
    }
    virtual bool create ( const shared_ptr<Structure> & st, const AnnotationArgumentList & args, string & err ) override {
        if( !StructureTypeAnnotation::create(st,args,err) )
            return false;
        bool fail = false;
        for ( auto & f : st->fields ) {
            if ( !f.type->isSimpleType(Type::tInt) ) {
                err += "field " + f.name + " must be int\n";
                fail = true;
            }
        }
        return !fail;
    }
    virtual TypeDeclPtr makeFieldType ( const string & name ) const override {
        if ( auto pF = makeSafeFieldType(name) ) {
            pF->ref = true;
            return pF;
        } else {
            return nullptr;
        }
    }
    virtual TypeDeclPtr makeSafeFieldType ( const string & name ) const override {
        auto pF = structureType->findField(name);
        return pF ? make_shared<TypeDecl>(*pF->type) : nullptr;
    }
    virtual SimNode * simulateGetField ( const string & name, Context & context, const LineInfo & at, SimNode * rv ) const  override {
        return context.makeNode<SimNode_IntFieldDeref>(at,rv,context.allocateName(name));
    }
    virtual SimNode * simulateSafeGetField ( const string & name, Context & context, const LineInfo & at, SimNode * rv ) const  override {
        return context.makeNode<SimNode_SafeIntFieldDeref>(at,rv,context.allocateName(name));
    }
    virtual void debug ( stringstream & ss, void * data ) const override {
        IntFields * prv = (IntFields *) data;
        if ( !prv ) {
            ss << "null";
            return;
        }
        ss << "{";
        for ( auto & f : prv->fields )
            ss << " " << f.first << ":" << f.second;
        ss << " }";
    }
};

void testFields ( Context * ctx ) {
    int32_t t = 0;
    IntFields x;
    auto fx = ctx->findFunction("testFields");
    __m128 args[1] = { cast<IntFields *>::from(&x) };
    x.fields["a"] = 1;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(t==1);
    x.fields["b"] = 2;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(t==3);
    x.fields["c"] = 3;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(t==6);
    x.fields["d"] = 4;
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(t==10);
    x.fields.erase("b");
    t = cast<int32_t>::to ( ctx->eval(fx, args) );
    assert(t==8);
}

struct EsAttribute {
    EsAttribute() = default;
    EsAttribute ( const string & n, uint32_t sz, bool rf, __m128 d )
        : name(n), size(sz), ref(rf), def(d) {}
    string      name;
    uint32_t    size = 0;
    bool        ref;
    __m128      def = _mm_setzero_ps();
};

struct EsAttributeTable {
    vector<EsAttribute> attributes;
};

struct EsPassAttributeTable  : EsAttributeTable {
    string  pass;
    string  functionName;
    int32_t functionIndex = -2;
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
            __m128 def = _mm_setzero_ps();
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
    virtual bool apply ( ExprBlock * block, const AnnotationArgumentList & args, string & err ) override {
        assert(block->isClosure);
        if ( block->annotationData ) {
            err = "annotation already specified";
            return false;
        }
        if ( block->arguments.empty() ) {
            err = "block needs arguments";
            return false;
        }
        block->annotationData = (void *)(g_esBlockTable.size() | 0xbad00000);
        g_esBlockTable.resize(g_esBlockTable.size() + 1);
        return true;
    }
    virtual bool finalize ( ExprBlock * block, const AnnotationArgumentList & args, string & err ) override {
        size_t index = intptr_t(block->annotationData);
        if ( (index & 0xfff00000) != 0xbad00000 ) {
            err = "invalid block";
            return false;
        }
        index &= 0x000fffff;
        if ( index<0 || index>=g_esBlockTable.size() ) {
            err = "invalid block";
            return false;
        }
        buildAttributeTable(g_esBlockTable[index], block->arguments, err);
        return err.empty();
    }
    virtual bool apply ( const FunctionPtr & func, const AnnotationArgumentList & args, string & err ) override {
        if ( func->annotationData ) {
            err = "annotation already specified";
            return false;
        }
        EsPassAttributeTable tab;
        if ( auto pp = args.find("pass", Type::tString) ) {
            tab.pass = pp->sValue;
        } else {
            err = "pass is not specified";
            return false;
        }
        tab.functionName = func->name;
        func->annotationData = (void *)(g_esPassTable.size());
        g_esPassTable.push_back(tab);
        return true;
    };
    virtual bool finalize ( const FunctionPtr & func, const AnnotationArgumentList & args, string & err ) override {
        size_t index = intptr_t(func->annotationData);
        if ( index<0 || index>=g_esPassTable.size() || g_esPassTable[index].functionName!=func->name ) {
            err = "invalid function";
            return false;
        }
        buildAttributeTable(g_esPassTable[index], func->arguments, err);
        return err.empty();
    }
};

bool EsRunPass ( Context & context, EsPassAttributeTable & table, const vector<EsComponent> & components, uint32_t totalComponents ) {
    if ( table.functionIndex==-2 )
        table.functionIndex = context.findFunction(table.functionName.c_str());
    if ( table.functionIndex==-1 )
        return false;
    int fnIndex = table.functionIndex;
    context.restart();
	__m128 * _args = (__m128 *)(alloca(table.attributes.size() * sizeof(__m128)));
    context.callEx(fnIndex, _args, 0, [&](SimNode * code){
        uint32_t nAttr = (uint32_t) table.attributes.size();
        __m128 * args = _args;
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
                        args[a] = _mm_loadu_ps((float *)src);
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
    context.restart();
    uint32_t nAttr = (uint32_t) table.attributes.size();
    __m128 * args = (__m128 *)(alloca(table.attributes.size() * sizeof(__m128)));
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
                    args[a] = _mm_loadu_ps((float *)src);
                } else {
                    *((void **)&args[a]) = src;
                }
                data[a] += stride[a];
            }
        }
        context.invoke(block, args);
        context.stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
        if ( context.stopFlags & EvalFlags::stopForThrow ) {
            // TODO: report exception here??
            return false;
        }
    }
    return true;
}


constexpr int g_total = 10000;
vector<float3>   g_pos ( g_total );
vector<float3>   g_vel ( g_total );
vector<float3 *> g_velBoxed ( g_total );
vector<EsComponent> g_components;

void initEsComponents() {
    // build components
    for ( int i=0; i != g_total; ++i ) {
        float f = float(i);
        g_pos[i] = { f, f*2, f*3 };
        g_vel[i] = { f+1, f+2, f+3 };
        g_velBoxed[i] = &g_vel[i];
    }
    g_components.clear();
    g_components.emplace_back("pos",      g_pos.data(),      sizeof(float3), sizeof(float3),   false);
    g_components.emplace_back("vel",      g_vel.data(),      sizeof(float3), sizeof(float3),   false);
    g_components.emplace_back("velBoxed", g_velBoxed.data(), sizeof(float3), sizeof(float3 *), true );
}

void verifyEsComponents() {
    for ( int i=0; i != g_total; ++i ) {
        float f = float(i);
        float3 pos = { f, f*2, f*3 };
        float3 vel = { f+1, f+2, f+3 };
        pos.x += vel.x;
        pos.y += vel.y;
        pos.z += vel.z;
        assert(g_pos[i].x==pos.x && g_pos[i].y==pos.y &&g_pos[i].z==pos.z );
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

class Module_UnitTest : public Module {
public:
    Module_UnitTest() : Module("UnitTest") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // function annotations
        addAnnotation(make_shared<EsFunctionAnnotation>());
        // structure annotations
        addAnnotation(make_shared<IntFieldsAnnotation>());
        // register types
        addAnnotation(make_shared<TestObjectFooAnnotation>());
        addAnnotation(make_shared<TestObjectBarAnnotation>(lib));
        // register function
        addExtern<decltype(queryEs), queryEs>(*this, lib, "queryEs");
        addExtern<decltype(testFoo), testFoo>(*this, lib, "testFoo");
        addExtern<decltype(testAdd), testAdd>(*this, lib, "testAdd");
        addExtern<decltype(testFields), testFields>(*this, lib, "testFields");
        addExtern<decltype(testEsUpdate), testEsUpdate>(*this, lib, "testEsUpdate");
        addExtern<decltype(initEsComponents), initEsComponents>(*this, lib, "initEsComponents");
        addExtern<decltype(verifyEsComponents), verifyEsComponents>(*this, lib, "verifyEsComponents");
    }
};

REGISTER_MODULE(Module_UnitTest);
