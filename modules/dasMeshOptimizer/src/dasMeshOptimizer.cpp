#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "dasMeshOptimizer.h"
#include "meshoptimizer.h"
#include <vector>
namespace das {
template<class T> static T* ptr(TArray<T>& a){return reinterpret_cast<T*>(a.data);}
template<class T> static const T* ptr(const TArray<T>& a){return reinterpret_cast<const T*>(a.data);}
static uint32_t check_vertices(const TArray<uint8_t>& v,uint32_t stride,Context* c,LineInfoArg* at){
    if(!stride || stride>256 || stride%4 || v.size%stride || v.size>INT_MAX) c->throw_error_at(at,"meshoptimizer: invalid vertex stride");
    return v.size/stride;
}
static void check_indices(const TArray<uint32_t>& i,uint32_t n,Context* c,LineInfoArg* at){
    if(i.size%3 || i.size>INT_MAX/4) c->throw_error_at(at,"meshoptimizer: expected triangle indices");
    for(uint32_t k=0;k<i.size;k++) if(ptr(i)[k]>=n)c->throw_error_at(at,"meshoptimizer: index out of bounds");
}
static int checked_size(uint64_t size,Context* c,LineInfoArg* at){
    if(size>INT_MAX)c->throw_error_at(at,"meshoptimizer: buffer too large");
    return int(size);
}
static void resize_output(Array& out,int size,int stride,Context* c,LineInfoArg* at){
    builtin_array_reserve(out,size,stride,c,at);builtin_array_resize(out,size,stride,c,at);
}
uint32_t mo_remap(TArray<uint32_t>& out,const TArray<uint32_t>& i,const TArray<uint8_t>& v,uint32_t stride,Context* c,LineInfoArg* at){
    const auto n=check_vertices(v,stride,c,at);check_indices(i,n,c,at);
    if(ptr(out) && ptr(out)==ptr(i))c->throw_error_at(at,"meshoptimizer: remap output aliases indices");
    resize_output(out,n,4,c,at);
    return uint32_t(meshopt_generateVertexRemap(ptr(out),ptr(i),i.size,ptr(v),n,stride));
}
void mo_remap_vertices(TArray<uint8_t>& out,const TArray<uint8_t>& v,const TArray<uint32_t>& r,uint32_t stride,uint32_t count,Context* c,LineInfoArg* at){
    const auto n=check_vertices(v,stride,c,at);
    if(r.size!=n || (ptr(out) && ptr(out)==ptr(v)))c->throw_error_at(at,"meshoptimizer: invalid remap buffers");
    for(uint32_t k=0;k<n;k++)if(ptr(r)[k]!=~0u && ptr(r)[k]>=count)c->throw_error_at(at,"meshoptimizer: invalid remap index");
    resize_output(out,checked_size(uint64_t(count)*stride,c,at),1,c,at);
    meshopt_remapVertexBuffer(ptr(out),ptr(v),n,stride,ptr(r));
}
void mo_remap_indices(TArray<uint32_t>& out,const TArray<uint32_t>& i,const TArray<uint32_t>& r,Context* c,LineInfoArg* at){
    if(ptr(out) && ptr(out)==ptr(r))c->throw_error_at(at,"meshoptimizer: output aliases remap table");
    check_indices(i,r.size,c,at);resize_output(out,i.size,4,c,at);meshopt_remapIndexBuffer(ptr(out),ptr(i),i.size,ptr(r));
}
void mo_cache(TArray<uint32_t>& i,uint32_t count,Context* c,LineInfoArg* at){
    check_indices(i,count,c,at);meshopt_optimizeVertexCache(ptr(i),ptr(i),i.size,count);
}
uint32_t mo_fetch(TArray<uint32_t>& i,TArray<uint8_t>& v,uint32_t stride,Context* c,LineInfoArg* at){
    const auto n=check_vertices(v,stride,c,at);check_indices(i,n,c,at);
    std::vector<uint8_t> out(v.size);
    const auto used=meshopt_optimizeVertexFetch(out.data(),ptr(i),i.size,ptr(v),n,stride);
    builtin_array_resize(v,checked_size(used*stride,c,at),1,c,at);
    if(v.size)memcpy(ptr(v),out.data(),v.size);
    return uint32_t(used);
}
void mo_encode_vertices(TArray<uint8_t>& out,const TArray<uint8_t>& v,uint32_t stride,Context* c,LineInfoArg* at){
    const auto n=check_vertices(v,stride,c,at);
    if(ptr(out) && ptr(out)==ptr(v))c->throw_error_at(at,"meshoptimizer: encoder output aliases input");
    resize_output(out,checked_size(meshopt_encodeVertexBufferBound(n,stride),c,at),1,c,at);
    const auto size=meshopt_encodeVertexBuffer(ptr(out),out.size,ptr(v),n,stride);
    builtin_array_resize(out,uint32_t(size),1,c,at);
}
bool mo_decode_vertices(TArray<uint8_t>& out,const TArray<uint8_t>& v,uint32_t n,uint32_t stride,Context* c,LineInfoArg* at){
    if(!stride || stride>256 || stride%4 || (ptr(out) && ptr(out)==ptr(v)))c->throw_error_at(at,"meshoptimizer: invalid decode buffers");
    resize_output(out,checked_size(uint64_t(n)*stride,c,at),1,c,at);
    const bool ok=meshopt_decodeVertexBuffer(ptr(out),n,stride,ptr(v),v.size)==0;
    if(!ok)builtin_array_resize(out,0,1,c,at);
    return ok;
}
void mo_encode_indices(TArray<uint8_t>& out,const TArray<uint32_t>& i,uint32_t n,Context* c,LineInfoArg* at){
    check_indices(i,n,c,at);resize_output(out,checked_size(meshopt_encodeIndexBufferBound(i.size,n),c,at),1,c,at);
    const auto size=meshopt_encodeIndexBuffer(ptr(out),out.size,ptr(i),i.size);builtin_array_resize(out,uint32_t(size),1,c,at);
}
bool mo_decode_indices(TArray<uint32_t>& out,const TArray<uint8_t>& v,uint32_t n,Context* c,LineInfoArg* at){
    if(n%3)c->throw_error_at(at,"meshoptimizer: expected triangle indices");
    checked_size(uint64_t(n)*4,c,at);resize_output(out,n,4,c,at);
    const bool ok=meshopt_decodeIndexBuffer(ptr(out),n,4,ptr(v),v.size)==0;
    if(!ok)builtin_array_resize(out,0,4,c,at);
    return ok;
}
class Module_MeshOptimizer : public Module {
public:
    Module_MeshOptimizer():Module("meshoptimizer"){
        ModuleLibrary lib;lib.addModule(this);lib.addBuiltInModule();
#define BIND(fn,name) addExtern<DAS_BIND_FUN(fn)>(*this,lib,name,SideEffects::modifyArgument,"das::" #fn)
        BIND(mo_remap,"meshopt_generate_remap")->args({"remap","indices","vertices","stride","context","at"});
        BIND(mo_remap_vertices,"meshopt_remap_vertices")->args({"output","vertices","remap","stride","count","context","at"});
        BIND(mo_remap_indices,"meshopt_remap_indices")->args({"output","indices","remap","context","at"});
        BIND(mo_cache,"meshopt_optimize_cache")->args({"indices","count","context","at"});
        BIND(mo_fetch,"meshopt_optimize_fetch")->args({"indices","vertices","stride","context","at"});
        BIND(mo_encode_vertices,"meshopt_encode_vertices")->args({"output","vertices","stride","context","at"});
        BIND(mo_decode_vertices,"meshopt_decode_vertices")->args({"output","encoded","count","stride","context","at"});
        BIND(mo_encode_indices,"meshopt_encode_indices")->args({"output","indices","count","context","at"});
        BIND(mo_decode_indices,"meshopt_decode_indices")->args({"output","encoded","count","context","at"});
#undef BIND
    }
    ModuleAotType aotRequire(TextWriter& tw) const override {
        tw << "#include \"../modules/dasMeshOptimizer/src/dasMeshOptimizer.h\"\n";return ModuleAotType::cpp;
    }
};
REGISTER_DYN_MODULE(Module_MeshOptimizer,Module_MeshOptimizer);
}
REGISTER_MODULE_IN_NAMESPACE(Module_MeshOptimizer,das);
