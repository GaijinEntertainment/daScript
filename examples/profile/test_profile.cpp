#include "test_profile.h"

using namespace das;

#ifndef _MSC_VER
#define __noinline    __attribute__((noinline))
#else
#define __noinline    __declspec(noinline)
#endif

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
    ObjectStructureTypeAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Object",ml) {
        addField<DAS_BIND_MANAGED_FIELD(pos)>("position","pos");
        addField<DAS_BIND_MANAGED_FIELD(vel)>("velocity","vel");
    }
};

MAKE_TYPE_FACTORY(Object, Object)
MAKE_TYPE_FACTORY(ObjectArray, ObjectArray)

__noinline int AddOne(int a) {
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
    int32_t functionIndex;
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

struct EsGroupData : ModuleGroupUserData {
    EsGroupData() : ModuleGroupUserData("es") {
        THAT = this;
    }
    virtual ~EsGroupData() {
        THAT = nullptr;
    }
    vector<unique_ptr<EsPassAttributeTable>>    g_esPassTable;
    vector<unique_ptr<EsAttributeTable>>        g_esBlockTable;
    static EsGroupData * THAT;
};
EsGroupData * EsGroupData::THAT = nullptr;

struct EsFunctionAnnotation : FunctionAnnotation {
    EsFunctionAnnotation() : FunctionAnnotation("es") { }
    EsGroupData * getGroupData( ModuleGroup & group ) const {
        if ( auto data = group.getUserData("es") ) {
            return (EsGroupData *) data;
        }
        auto esData = new EsGroupData();
        group.setUserData(esData);
        return esData;
    }
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
    virtual bool apply ( ExprBlock * block, ModuleGroup &, const AnnotationArgumentList &, string & err ) override {
        if ( block->arguments.empty() ) {
            err = "block needs arguments";
            return false;
        }
        return true;
    }
    virtual bool finalize ( ExprBlock * block, ModuleGroup & group, const AnnotationArgumentList &,
                           const AnnotationArgumentList &, string & err ) override {
        auto esData = getGroupData(group);
        auto tab = make_unique<EsAttributeTable>();
        block->annotationData = uint64_t(tab.get());
        buildAttributeTable(*tab, block->arguments, err);
        esData->g_esBlockTable.emplace_back(move(tab));
        return err.empty();
    }
    virtual bool apply ( const FunctionPtr & func, ModuleGroup &, const AnnotationArgumentList &, string & err ) override {
        if ( func->arguments.empty() ) {
            err = "function needs arguments";
            return false;
        }
        func->exports = true;
        return true;
    };
    virtual bool finalize ( const FunctionPtr & func, ModuleGroup & group, const AnnotationArgumentList & args,
                           const AnnotationArgumentList &, string & err ) override {
        auto esData = getGroupData(group);
        auto tab = make_unique<EsPassAttributeTable>();
        if ( auto pp = args.find("pass", Type::tString) ) {
            tab->pass = pp->sValue;
        } else {
            err = "pass is not specified";
            return false;
        }
        tab->functionIndex = (int32_t) func->index;
        if ( tab->functionIndex<0 ) {
            err = "function is not there";
            return false;
        }
        buildAttributeTable(*tab, func->arguments, err);
        esData->g_esPassTable.emplace_back(move(tab));
        return err.empty();
    }
};

bool EsRunPass ( Context & context, EsPassAttributeTable & table, const vector<EsComponent> & components, uint32_t totalComponents ) {
    auto functionPtr = context.getFunction(table.functionIndex);
    vec4f * _args = (vec4f *)(alloca(table.attributes.size() * sizeof(vec4f)));
    context.callEx(functionPtr, _args, nullptr, 0, [&](SimNode * code){
        uint32_t nAttr = (uint32_t) table.attributes.size();
        vec4f * args = _args;
        char **        data    = (char **) alloca(nAttr * sizeof(char *));
        uint32_t *    stride    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        uint32_t *  size    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        bool *        boxed    = (bool *) alloca(nAttr * sizeof(bool));
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
            if ( context.getException() ) {
                // TODO: report exception here??
                return;
            }
        }
    });
    return true;
}

uint32_t EsRunBlock ( Context & context, const Block & block, const vector<EsComponent> & components, uint32_t totalComponents ) {
    auto * closure = (SimNode_ClosureBlock *) block.body;
    EsAttributeTable * table = (EsAttributeTable *) closure->annotationData;
    if ( !table ) {
        context.throw_error("EsRunBlock - query missing annotation data");
    }
    uint32_t nAttr = (uint32_t) table->attributes.size();
    vec4f * _args = (vec4f *)(alloca(table->attributes.size() * sizeof(vec4f)));
    context.invokeEx(block, _args, nullptr, [&](SimNode * code){
        vec4f * args = _args;
        char **        data    = (char **) alloca(nAttr * sizeof(char *));
        uint32_t *    stride    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        uint32_t *  size    = (uint32_t *) alloca(nAttr * sizeof(uint32_t));
        bool *        boxed    = (bool *) alloca(nAttr * sizeof(bool));
        bool *      ref     = (bool *) alloca(nAttr * sizeof(bool));
        for ( uint32_t a=0; a!=nAttr; ++a ) {
            auto it = find_if ( components.begin(), components.end(), [&](const EsComponent & esc){
                return esc.name == table->attributes[a].name;
            });
            if ( it != components.end() ) {
                data[a]   = (char *) it->data;
                stride[a] = it->stride;
                boxed[a]  = it->boxed;
            } else {
                data[a] = nullptr;
                args[a] = table->attributes[a].def;
            }
            size[a] = table->attributes[a].size;
            ref[a] = table->attributes[a].ref;
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
            if ( context.getException() ) {
                // TODO: report exception here??
                return;
            }
        }
    });
    return totalComponents;
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
        DAS_ASSERT(g_pos[i].x==npos.x && g_pos[i].y==npos.y && g_pos[i].z==npos.z );
    }
}

void testEsUpdate ( char * pass, Context * ctx ) {
    if ( !EsGroupData::THAT ) {
        ctx->throw_error_ex("missing pass data for the pass %s", pass);
        return;
    }
    for ( auto & tab : EsGroupData::THAT->g_esPassTable ) {
        if ( tab->pass==pass ) {
            EsRunPass(*ctx, *tab, g_components, g_total);
        }
    }
}

uint32_t queryEs (const Block & block, Context * context) {
    return EsRunBlock(*context, block, g_components, g_total);
}
#if DAS_USE_EASTL
#include <EASTL/unordered_map.h>
#else
#include <unordered_map>
#endif

struct dictKeyEqual {
    __forceinline bool operator()( const char * lhs, const char * rhs ) const {
        return ( lhs==rhs ) ? true : strcmp(lhs,rhs)==0;
    }
};

struct dictKeyHash {
    __forceinline uint32_t operator () ( const char * str ) const {
        StringHeader * header = ((StringHeader *)str)-1;
        auto hh = header->hash;
        if ( !hh ) {
            header->hash = hh = hash_block32((uint8_t *)str, header->length);
        }
        return hh;
    }
};

typedef unordered_map<char *, int32_t, dictKeyHash, dictKeyEqual> dict_hash_map;

 int testDict(Array & arr) {
    dict_hash_map tab;
    char ** data = (char **) arr.data;
    int maxOcc = 0;
    for ( uint32_t t = 0; t !=arr.size; ++t ) {
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
    #if _CPPUNWIND || __cpp_exceptions
    int arr[1000];
    int cnt = 0;
    for (int j = 0; j != 100; ++j) {
        int fail = 0;
        for (int i = 0; i != 2000; ++i) {
            try {
                if (i < 0 || i>=1000) throw das::runtime_error("range check error");
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
    #endif
}

__noinline float testExpLoop(int count) {
    float ret = 0;
    for (int i = 0; i < count; ++i)
        ret += expf(1.f/(1.f+i));
    return ret;
}

#define pi 3.141592653589793f
#define solar_mass (4 * pi * pi)
#define days_per_year 365.24f

struct planet {
    float x, y, z; float pad;
    float vx, vy, vz; float mass;
};

void advance(int nbodies, struct planet * __restrict bodies, float dt)
{
    int i, j;

    for (i = 0; i < nbodies; i++) {
        struct planet * b = &(bodies[i]);
        for (j = i + 1; j < nbodies; j++) {
            struct planet * b2 = &(bodies[j]);
            float dx = b->x - b2->x;
            float dy = b->y - b2->y;
            float dz = b->z - b2->z;
            float distanced = dx * dx + dy * dy + dz * dz;
            float distance = sqrtf(distanced);
            float mag = dt / (distanced * distance);
            b->vx -= dx * b2->mass * mag;
            b->vy -= dy * b2->mass * mag;
            b->vz -= dz * b2->mass * mag;
            b2->vx += dx * b->mass * mag;
            b2->vy += dy * b->mass * mag;
            b2->vz += dz * b->mass * mag;
        }
    }
    for (i = 0; i < nbodies; i++) {
        struct planet * b = &(bodies[i]);
        b->x += dt * b->vx;
        b->y += dt * b->vy;
        b->z += dt * b->vz;
    }
}

void advancev(int nbodies, struct planet * __restrict bodies, float dt)
{
    vec4f vdt = v_splats(dt);
    struct planet *__restrict b = bodies, *be = bodies + nbodies;
    do {
        for (struct planet *__restrict b2 = b+1; b2 != be; ++b2) {
            vec4f dx = v_sub(v_ld(&b->x), v_ld(&b2->x));
            vec4f distanced = v_length3_sq_x(dx);
            vec4f distance = v_sqrt_x(distanced);
            vec4f mag = v_splat_x(v_div_x(vdt, v_mul_x(distanced, distance)));
            vec4f bv = v_ld(&b->vx), b2v = v_ld(&b2->vx);
            bv = v_sub(bv, v_mul(dx, v_mul(v_splat_w(b2v), mag)));//.w will be zero, since dx.w is zero
            b2v = v_add(b2v, v_mul(dx, v_mul(v_splat_w(bv), mag)));
            v_st(&b->vx, bv);
            v_st(&b2->vx, b2v);
        }
    } while(++b != be);

    for (struct planet *__restrict bB = bodies; bB != be; ++bB)
    {
        vec4f bx = v_ld(&bB->x), bv = v_ld(&b->vx);
        v_st(&bB->x, v_madd(vdt, bv, bx));
    }
}

float energy(int nbodies, struct planet * bodies)
{
    float e;
    int i, j;

    e = 0.0f;
    for (i = 0; i < nbodies; i++) {
        struct planet * b = &(bodies[i]);
        e += 0.5f * b->mass * (b->vx * b->vx + b->vy * b->vy + b->vz * b->vz);
        for (j = i + 1; j < nbodies; j++) {
            struct planet * b2 = &(bodies[j]);
            float dx = b->x - b2->x;
            float dy = b->y - b2->y;
            float dz = b->z - b2->z;
            float distance = sqrt(dx * dx + dy * dy + dz * dz);
            e -= (b->mass * b2->mass) / distance;
        }
    }
    return e;
}

void offset_momentum(int nbodies, struct planet * bodies)
{
    float px = 0.0f, py = 0.0f, pz = 0.0f;
    int i;
    for (i = 0; i < nbodies; i++) {
        px += bodies[i].vx * bodies[i].mass;
        py += bodies[i].vy * bodies[i].mass;
        pz += bodies[i].vz * bodies[i].mass;
    }
    bodies[0].vx = - px / solar_mass;
    bodies[0].vy = - py / solar_mass;
    bodies[0].vz = - pz / solar_mass;
}

#define NBODIES 5
struct planet bodies[NBODIES] = {
    {                               /* sun */
        0, 0, 0, 0, 0, 0, 0,solar_mass
    },
    {                               /* jupiter */
        4.84143144246472090e+00f,
        -1.16032004402742839e+00f,
        -1.03622044471123109e-01f,
        0.f,
        1.66007664274403694e-03f * days_per_year,
        7.69901118419740425e-03f * days_per_year,
        -6.90460016972063023e-05f * days_per_year,
        9.54791938424326609e-04f * solar_mass
    },
    {                               /* saturn */
        8.34336671824457987e+00f,
        4.12479856412430479e+00f,
        -4.03523417114321381e-01f,
        0.f,
        -2.76742510726862411e-03f * days_per_year,
        4.99852801234917238e-03f * days_per_year,
        2.30417297573763929e-05f * days_per_year,
        2.85885980666130812e-04f * solar_mass
    },
    {                               /* uranus */
        1.28943695621391310e+01f,
        -1.51111514016986312e+01f,
        -2.23307578892655734e-01f,
        0.f,
        2.96460137564761618e-03f * days_per_year,
        2.37847173959480950e-03f * days_per_year,
        -2.96589568540237556e-05f * days_per_year,
        4.36624404335156298e-05f * solar_mass
    },
    {                               /* neptune */
        1.53796971148509165e+01f,
        -2.59193146099879641e+01f,
        1.79258772950371181e-01f,
        0.f,
        2.68067772490389322e-03f * days_per_year,
        1.62824170038242295e-03f * days_per_year,
        -9.51592254519715870e-05f * days_per_year,
        5.15138902046611451e-05f * solar_mass
    }
};

void testNBodiesInit() {
    offset_momentum(NBODIES, bodies);
}

void testNBodies(int32_t N) {
    for (int32_t n = 0; n != N; ++n) {
        advancev(NBODIES, bodies, 0.01f);
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
        addAnnotation(make_shared<ObjectStructureTypeAnnotation>(lib));
        addAnnotation(make_shared<ManagedVectorAnnotation<Object>>("ObjectArray",lib));
        // register functions
        addExtern<DAS_BIND_FUN(AddOne)>(*this,lib,"AddOne",SideEffects::none);
        addExtern<DAS_BIND_FUN(updateObject)>(*this,lib,"interopUpdate",SideEffects::modifyExternal,"updateObject");
        addExtern<DAS_BIND_FUN(updateTest)>(*this,lib,"interopUpdateTest",SideEffects::modifyExternal,"updateTest");
        addExtern<DAS_BIND_FUN(update10000)>(*this,lib,"update10000",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(update10000ks)>(*this,lib,"update10000ks",SideEffects::modifyExternal);
        // es
        addExtern<DAS_BIND_FUN(queryEs)>(*this, lib, "queryEs",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testEsUpdate)>(*this, lib, "testEsUpdate",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(initEsComponents)>(*this, lib, "initEsComponents",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(verifyEsComponents)>(*this, lib, "verifyEsComponents",SideEffects::modifyExternal);
        // C++ copy of all tests
        addExtern<DAS_BIND_FUN(testPrimes)>(*this, lib, "testPrimes",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testDict)>(*this, lib, "testDict",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testFibR)>(*this, lib, "testFibR",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testFibI)>(*this, lib, "testFibI",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testParticles)>(*this, lib, "testParticles",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testParticlesI)>(*this, lib, "testParticlesI",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testTryCatch)>(*this, lib, "testTryCatch",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testExpLoop)>(*this, lib, "testExpLoop",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testNBodiesInit)>(*this, lib, "testNBodiesInit",SideEffects::modifyExternal);
        addExtern<DAS_BIND_FUN(testNBodies)>(*this, lib, "testNBodies",SideEffects::modifyExternal);
    }
};

REGISTER_MODULE(Module_TestProfile);


