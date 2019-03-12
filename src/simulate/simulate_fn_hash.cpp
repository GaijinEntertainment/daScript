#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"

namespace das {

    struct SimFnHashVisitor :  SimVisitor {
        // 64 bit FNV1a
        const uint64_t fnv_prime = 1099511628211ul;
        uint64_t offset_basis = 14695981039346656037ul;
        __forceinline void write ( const void * pb, uint32_t size ) {
            const uint8_t * block = (const uint8_t *) pb;
            while ( size-- ) {
                offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            }
            // printf("%llx", offset_basis);
        }
        __forceinline void write ( const void * pb ) {
            const uint8_t * block = (const uint8_t *) pb;
            for (; *block; block++) {
                offset_basis = ( offset_basis ^ *block ) * fnv_prime;
            }
            // printf("[%s] %llx ", (const char *) pb, offset_basis);
        }
        __forceinline uint64_t getHash() const  {
            return (offset_basis <= 1) ? fnv_prime : offset_basis;
        }
        // semantic walker
        virtual void op ( const char * name, size_t sz, const char * TT ) override {
            write(name);
            if ( sz ) write(&sz, sizeof(sz));
            if ( TT ) write(TT);
        }
        virtual void sp ( uint32_t stackTop,  const char * op ) override {
            write(&stackTop, sizeof(stackTop));
            write(op);
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
            write(argV);
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
        virtual void sub ( SimNode ** nodes, uint32_t count, const char * argN ) override {
            write(&count, sizeof(count));
            write(argN);
            for ( uint32_t t = 0; t!=count; ++t ) {
                nodes[t] = nodes[t]->visit(*this);
            }
        }
        virtual SimNode * sub ( SimNode * node, const char * opN ) override {
            write(opN);
            return SimVisitor::sub(node, opN);
        }
    };

    uint64_t getSemanticHash ( SimNode * node ) {
        SimFnHashVisitor hashV;
        node->visit(hashV);
        return hashV.getHash();
    }
}
