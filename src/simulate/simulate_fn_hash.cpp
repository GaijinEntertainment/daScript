#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/simulate/hash.h"

#include <cinttypes>

namespace das {

#if 1
#define debug_hash(...)
#else
#define debug_hash  printf
#endif


    struct SimFnHashVisitor :  SimVisitor {
        Context * context = nullptr;
        SimFnHashVisitor ( Context * ctx )
            : context(ctx) {
        }
        // 64 bit FNV1a
        const uint64_t fnv_prime = 1099511628211ul;
        uint64_t offset_basis = 14695981039346656037ul;
        __forceinline void write ( const void * pb, uint32_t size ) {
            const uint8_t * block = (const uint8_t *) pb;
            while ( size-- ) {
                offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            }
            debug_hash("%" PRIx64 " ", offset_basis);
        }
        __forceinline void write ( const void * pb ) {
            const uint8_t * block = (const uint8_t *) pb;
            for (; *block; block++) {
                offset_basis = ( offset_basis ^ *block ) * fnv_prime;
            }
            debug_hash("[%s] %" PRIx64 " ", (const char *) pb, offset_basis);
        }
        __forceinline uint64_t getHash() const  {
            return (offset_basis <= 1) ? fnv_prime : offset_basis;
        }
        // semantic walker
        virtual void op ( const char * name, uint32_t sz, const string & TT ) override {
            write(name);
            if ( sz ) write(&sz, sizeof(sz));
            if ( !TT.empty() ) write(TT.c_str());
        }
        virtual void sp ( uint32_t stackTop,  const char * op ) override {
            write(&stackTop, sizeof(stackTop));
            write(op);
        }
        virtual void arg ( Func, const char * mangledName, const char * argN ) override {
            write(mangledName);
            write(argN);
        }
        virtual void arg ( Func, uint32_t mangledNameHash, const char * argN ) override {
            write(&mangledNameHash, sizeof(mangledNameHash));
            write(argN);
        }
        virtual void arg ( int32_t argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( uint32_t argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( const char * argV,  const char * argN  ) override {
            if ( argV ) {
                write(argV);
                if ( auto blobLength = context && context->constStringHeap ? context->constStringHeap->blobLength(argV) : 0 ) write(argV, blobLength);
            }
            write(argN);
        }
        virtual void arg ( vec4f argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( uint64_t argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( bool argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( float argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void arg ( double argV,  const char * argN ) override {
            write(&argV,sizeof(argV));
            write(argN);
        }
        virtual void sub ( SimNode ** nodes, uint32_t count, const char * argN ) override {
            write(&count, sizeof(count));
            write(argN);
            for ( uint32_t t=0; t!=count; ++t ) {
                nodes[t] = nodes[t]->visit(*this);
            }
        }
        virtual SimNode * sub ( SimNode * node, const char * opN ) override {
            write(opN);
            return SimVisitor::sub(node, opN);
        }
    };

    uint64_t getSemanticHash ( SimNode * node, Context * context ) {
        debug_hash("\n");
        SimFnHashVisitor hashV(context);
        node->visit(hashV);
        debug_hash("\n");
        return hashV.getHash();
    }

    uint64_t getFunctionHash ( Function * fun, SimNode * node, Context * context ) {
        debug_hash("\n%s\n", fun->getMangledName().c_str());
        SimFnHashVisitor hashV(context);
        // append return type and result type
        uint64_t resT = fun->result->getSemanticHash();
        hashV.write(&resT, sizeof(uint64_t));
        debug_hash("\nresult <%s> = %" PRIx64 "\n", fun->result->getMangledName().c_str(), resT);
        for ( auto & arg : fun->arguments ) {
            uint64_t argT = arg->type->getSemanticHash();
            hashV.write(&argT, sizeof(argT));
            debug_hash("arg %s <%s> = %" PRIx64 "\n", arg->type->getMangledName().c_str(), arg->name.c_str(), argT);
        }

        // append code
        node->visit(hashV);
        if ( fun->aotHashDeppendsOnArguments ) {
            for ( auto & arg : fun->arguments ) {
                hashV.write(arg->name.c_str());
                if ( arg->init ) {
                    TextWriter tw;
                    tw << *(arg->init);
                    hashV.write(tw.str().c_str());
                }
            }
        }
        uint64_t res = hashV.getHash();
        debug_hash("\n%s = %" PRIx64 "\n", fun->getMangledName().c_str(), res);
        return res;
    }
}
