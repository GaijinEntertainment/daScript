#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/aot_builtin.h"
#include "daScript/simulate/sim_policy.h"

namespace das
{
    struct LockDataWalker : DataWalker {
        bool locked = false;
        virtual bool canVisitArrayData ( TypeInfo * ti ) {
            return (ti->flags & TypeInfo::flag_lockCheck) == TypeInfo::flag_lockCheck;
        }
        virtual bool canVisitTableData ( TypeInfo * ti ) override {
            if ( ti->secondType ) {
                return (ti->secondType->flags & TypeInfo::flag_lockCheck) == TypeInfo::flag_lockCheck;
            } else {
                return false;
            }
        }
        virtual bool canVisitHandle ( char *, TypeInfo * ) override {
            return false;
        }
        virtual bool canVisitPointer ( TypeInfo * ) override {
            return false; }
        virtual bool canVisitLambda ( TypeInfo * ) override {
            return false;
        }
        virtual bool canVisitIterator ( TypeInfo * ) override {
            return false;
        }
        virtual bool canVisitStructure ( char *, StructInfo * si ) {
            return (si->flags & StructInfo::flag_lockCheck) == StructInfo::flag_lockCheck;
        }
        virtual void beforeArray ( Array * pa, TypeInfo * ) override {
            if ( pa->lock ) {
                locked = true;
                cancel = true;
            }
        }
        virtual void beforeTable ( Table * pa, TypeInfo * ) override {
            if ( pa->lock ) {
                locked = true;
                cancel = true;
            }
        }
    };

    vec4f builtin_verify_locks ( Context & context, SimNode_CallBase * node, vec4f * args ) {
        auto typeInfo = node->types[0];
        auto value = args[0];
        LockDataWalker walker;
        walker.walk(value,typeInfo);
        if ( walker.locked ) context.throw_error_at(node->debugInfo, "object contains locked elements and can't be resized");
        return v_zero();
    }

}
