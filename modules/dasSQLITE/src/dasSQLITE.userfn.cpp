#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/aot.h"

#include "dasSQLITE.h"
#include "aot_sqlite.h"

namespace das {

// ============================================================================
// User-defined SQL scalar functions: register_function trampoline.
//
// The daslang `register_function` call macro emits a call to
// `sqlite3_register_function` with the function pointer, the per-arg
// SqlFnTag array, and the return-type tag. We pick a trampoline for the
// arity (0..4) and stash a heap-owned `RegisteredScalarFn` as the SQLite
// user-data; SQLite calls `register_fn_destroy` when the connection is
// closed (or the function is replaced), so the daslang allocation is
// freed deterministically.
//
// NULL handling (v1): if any input cell is SQLITE_NULL, we short-circuit
// and emit SQLITE_NULL without invoking the daslang function — matches the
// idiomatic SQL behavior of built-in scalars (`abs(NULL) -> NULL` etc.).
// The Option<T> path planned in the chunk-10 design is deferred to v2.
// Panic in the daslang function is caught via Context::runWithCatch and
// surfaced to SQLite via sqlite3_result_error.
// ============================================================================

enum class SqlFnTag : uint8_t {
    Int    = 0,  // 32-bit signed
    Int64  = 1,  // 64-bit signed
    Float  = 2,  // 32-bit IEEE 754
    Double = 3,  // 64-bit IEEE 754
    Bool   = 4,  // marshaled via int64 (0/1)
    String = 5,
};

static constexpr int MAX_SQL_FN_ARGS = 4;

struct RegisteredScalarFn {
    Context * context = nullptr;
    Func      fn;
    SqlFnTag  retTag = SqlFnTag::Int64;
    uint8_t   nArgs  = 0;
    SqlFnTag  argTags[MAX_SQL_FN_ARGS] = { SqlFnTag::Int64, SqlFnTag::Int64, SqlFnTag::Int64, SqlFnTag::Int64 };
    LineInfo  at;
};

static void register_fn_destroy ( void * p ) {
    delete static_cast<RegisteredScalarFn *>(p);
}

static __forceinline bool marshal_one_arg (
        sqlite3_value * v, SqlFnTag tag, vec4f & out, sqlite3_context * sctx ) {
    switch ( tag ) {
        case SqlFnTag::Int: {
            int32_t x = (int32_t) sqlite3_value_int64(v);
            out = cast<int32_t>::from(x);
            return true;
        }
        case SqlFnTag::Int64: {
            int64_t x = sqlite3_value_int64(v);
            out = cast<int64_t>::from(x);
            return true;
        }
        case SqlFnTag::Float: {
            float x = (float) sqlite3_value_double(v);
            out = cast<float>::from(x);
            return true;
        }
        case SqlFnTag::Double: {
            double x = sqlite3_value_double(v);
            out = cast<double>::from(x);
            return true;
        }
        case SqlFnTag::Bool: {
            bool x = sqlite3_value_int64(v) != 0;
            out = cast<bool>::from(x);
            return true;
        }
        case SqlFnTag::String: {
            // sqlite returns const unsigned char *; the daslang side expects
            // a `string` (pointer + null terminator). Lifetime: SQLite owns
            // the buffer for the duration of the function call (until the
            // next sqlite3_value_* call on the same value), which is more
            // than enough — we invoke immediately and don't retain.
            const char * s = (const char *) sqlite3_value_text(v);
            if ( !s ) s = "";
            out = cast<const char *>::from(s);
            return true;
        }
    }
    sqlite3_result_error(sctx, "register_function: internal: unknown SqlFnTag", -1);
    return false;
}

static __forceinline void marshal_result (
        sqlite3_context * sctx, vec4f result, SqlFnTag retTag ) {
    switch ( retTag ) {
        case SqlFnTag::Int:
            sqlite3_result_int64(sctx, (int64_t) cast<int32_t>::to(result));
            return;
        case SqlFnTag::Int64:
            sqlite3_result_int64(sctx, cast<int64_t>::to(result));
            return;
        case SqlFnTag::Float:
            sqlite3_result_double(sctx, (double) cast<float>::to(result));
            return;
        case SqlFnTag::Double:
            sqlite3_result_double(sctx, cast<double>::to(result));
            return;
        case SqlFnTag::Bool:
            sqlite3_result_int64(sctx, cast<bool>::to(result) ? 1 : 0);
            return;
        case SqlFnTag::String: {
            const char * s = cast<const char *>::to(result);
            if ( !s ) {
                sqlite3_result_null(sctx);
            } else {
                // SQLITE_TRANSIENT — sqlite makes its own copy.
                sqlite3_result_text(sctx, s, -1, SQLITE_TRANSIENT);
            }
            return;
        }
    }
    // Defensive: matches the fallback in marshal_one_arg. The macro emit
    // always passes a valid tag, but if a future contributor adds an
    // SqlFnTag without extending the switch, SQLite gets a defined error
    // instead of an undefined no-result-was-set call.
    sqlite3_result_error(sctx, "register_function: internal: unknown SqlFnTag", -1);
}

static void scalar_trampoline ( sqlite3_context * sctx, int argc, sqlite3_value ** argv ) {
    auto R = (RegisteredScalarFn *) sqlite3_user_data(sctx);
    if ( !R || !R->context || !R->fn.PTR ) {
        sqlite3_result_error(sctx, "register_function: invalid registration", -1);
        return;
    }
    if ( argc != R->nArgs ) {
        sqlite3_result_error(sctx, "register_function: argument count mismatch", -1);
        return;
    }
    // NULL short-circuit: any NULL input ⇒ NULL output, no invocation.
    for ( int i = 0; i < argc; ++i ) {
        if ( sqlite3_value_type(argv[i]) == SQLITE_NULL ) {
            sqlite3_result_null(sctx);
            return;
        }
    }
    vec4f args[MAX_SQL_FN_ARGS] = { v_zero(), v_zero(), v_zero(), v_zero() };
    for ( int i = 0; i < argc; ++i ) {
        if ( !marshal_one_arg(argv[i], R->argTags[i], args[i], sctx) ) {
            return;
        }
    }
    vec4f result = v_zero();
    bool ok = R->context->runWithCatch([&]() {
        result = R->context->callOrFastcall(R->fn.PTR, args, &R->at);
    });
    if ( !ok ) {
        // Read the panic message, then clear the context's exception state
        // so the outer daslang stack (which is currently running the SQL
        // statement that called us) sees a clean recovery — otherwise the
        // exception leaks past the SQLite layer and re-throws on the caller.
        const char * msg = R->context->getException();
        std::string copy = msg ? msg : "register_function: panic in daslang function";
        R->context->last_exception = R->context->exception;
        R->context->exception = nullptr;
        R->context->stopFlags = 0;
        sqlite3_result_error(sctx, copy.c_str(), -1);
        return;
    }
    marshal_result(sctx, result, R->retTag);
}

// Daslang-facing entry. Owns the RegisteredScalarFn allocation; on success,
// SQLite takes ownership via xDestroy. On failure to register, we delete
// here (sqlite3_create_function_v2 documents that xDestroy is NOT called
// if the registration call itself fails).
//
// Tags are passed as four scalar uint8 slots rather than a TArray to keep
// the daslang-side macro emit simple — unused slots are ignored according
// to nArgs (0..MAX_SQL_FN_ARGS).
int sqlite3_register_function (
        sqlite3 * db, const char * name, Func fn, int nArgs,
        uint8_t tag0, uint8_t tag1, uint8_t tag2, uint8_t tag3,
        uint8_t retTag, bool deterministic, bool directonly,
        Context * context, LineInfoArg * at ) {
    if ( !db ) return SQLITE_MISUSE;
    if ( !name || !*name ) return SQLITE_MISUSE;
    if ( !fn.PTR ) return SQLITE_MISUSE;
    if ( nArgs < 0 || nArgs > MAX_SQL_FN_ARGS ) return SQLITE_MISUSE;
    auto R = new RegisteredScalarFn();
    R->context = context;
    R->fn      = fn;
    R->retTag  = SqlFnTag(retTag);
    R->nArgs   = uint8_t(nArgs);
    R->at      = at ? *at : LineInfo();
    const uint8_t tagSlots[MAX_SQL_FN_ARGS] = { tag0, tag1, tag2, tag3 };
    for ( int i = 0; i < nArgs; ++i ) {
        R->argTags[i] = SqlFnTag(tagSlots[i]);
    }
    int eTextRep = SQLITE_UTF8;
    if ( deterministic ) eTextRep |= SQLITE_DETERMINISTIC;
    if ( directonly )    eTextRep |= SQLITE_DIRECTONLY;
    int rc = sqlite3_create_function_v2(
        db, name, nArgs, eTextRep,
        R, scalar_trampoline, nullptr, nullptr,
        register_fn_destroy);
    if ( rc != SQLITE_OK ) {
        delete R;
    }
    return rc;
}

}
