#include "daScript/daScript.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace das {

    typedef rapidjson::Value JsValue;

    template <>
    struct typeFactory<JsValue> {
        static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {
            return library.makeHandleType("JsValue");
        }
    };

    template <typename Fun, Fun BOOL_PROP>
    struct SimNode_SafeAs : SimNode {
        DAS_PTR_NODE;
        SimNode_SafeAs(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv || !(pv->*BOOL_PROP)() ) { return nullptr; }
            return (char *) pv;
        }
        SimNode * subexpr;
    };

    struct SimNode_AsBool : SimNode {
        DAS_BOOL_NODE;
        SimNode_AsBool(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline bool compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error_at(debugInfo,"JSON dereferencing null pointer"); return false; }
            if ( !pv->IsBool() ) { context.throw_error_at(debugInfo,"JSON not a boolean"); return false; }
            return pv->GetBool();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsInt : SimNode {
        DAS_INT_NODE;
        SimNode_AsInt(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline int32_t compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error_at(debugInfo,"JSON dereferencing null pointer"); return 0; }
            if ( !pv->IsInt() ) { context.throw_error_at(debugInfo,"JSON not a int"); return 0; }
            return pv->GetInt();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsFloat : SimNode {
        DAS_FLOAT_NODE;
        SimNode_AsFloat(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline float compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error_at(debugInfo,"JSON dereferencing null pointer"); return 0.0f; }
            if ( !pv->IsFloat() ) { context.throw_error_at(debugInfo,"JSON not a float"); return 0.0f; }
            return pv->GetFloat();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsDouble : SimNode {
        DAS_DOUBLE_NODE;
        SimNode_AsDouble(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline double compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error_at(debugInfo,"JSON dereferencing null pointer"); return 0.0f; }
            if ( !pv->IsDouble() ) { context.throw_error_at(debugInfo,"JSON not a double"); return 0.0f; }
            return pv->GetDouble();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsString : SimNode {
        DAS_PTR_NODE;
        SimNode_AsString(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error_at(debugInfo,"JSON dereferencing null pointer"); return nullptr; }
            if ( !pv->IsString() ) { context.throw_error_at(debugInfo,"JSON not a string"); return nullptr; }
            auto ps = pv->GetString();
            auto psl = pv->GetStringLength();
            return context.heap.allocateString(ps,psl);
        }
        SimNode * subexpr;
    };

    struct SimNode_GetJsonArraySize : SimNode {
        DAS_INT_NODE;
        SimNode_GetJsonArraySize(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline int32_t compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) return 0;
            return pv->Size();
        }
        SimNode * subexpr;
    };

    template <bool SAFE>
    struct SimNode_GetJsonFieldConst : SimNode {
        DAS_PTR_NODE;
        SimNode_GetJsonFieldConst(const LineInfo & a, SimNode * se, char * i )
        : SimNode(a), subexpr(se), index(i) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) {
                if ( !SAFE ) context.throw_error_at(debugInfo,"JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsObject() ) {
                if ( !SAFE ) context.throw_error_at(debugInfo,"JSON field is not an object");
                return nullptr;
            }
            auto hf = pv->FindMember(index);
            if ( hf == pv->MemberEnd() ) {
                if ( !SAFE ) context.throw_error_at(debugInfo,"JSON field not found");
                return nullptr;
            }
            JsValue & value = hf->value;
            return (char *) &value;
        }
        SimNode * subexpr;
        char * index;
    };

    struct SimNode_GetJsonField : SimNode {
        DAS_PTR_NODE;
        SimNode_GetJsonField(const LineInfo & a, SimNode * se, SimNode * i)
            : SimNode(a), subexpr(se), index(i) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) {
                context.throw_error_at(debugInfo,"JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsObject() ) {
                context.throw_error_at(debugInfo,"JSON field is not an object");
                return nullptr;
            }
            auto pi = index->evalPtr(context);
            auto hf = pv->FindMember(pi);
            if ( hf == pv->MemberEnd() ) {
                context.throw_error_at(debugInfo,"JSON field not found");
                return nullptr;
            }
            JsValue & value = hf->value;
            return (char *) &value;
        }
        SimNode * subexpr, * index;
    };

    struct SimNode_GetJsonAt : SimNode {
        DAS_PTR_NODE;
        SimNode_GetJsonAt(const LineInfo & a, SimNode * se, SimNode * i)
            : SimNode(a), subexpr(se), index(i) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) {
                context.throw_error_at(debugInfo,"JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsArray() ) {
                context.throw_error_at(debugInfo,"JSON field is not an array");
                return nullptr;
            }
            auto idx = cast<uint32_t>::to(index->eval(context));
            if ( idx >= pv->Size() ) {
                context.throw_error_at(debugInfo,"JSON array index out of range");
                return nullptr;
            }
            JsValue & value = (*pv)[idx];
            return (char *) &value;
        }
        SimNode * subexpr, * index;
    };

    struct JsValueTypeAnnotation : TypeAnnotation {

        JsValueTypeAnnotation() : TypeAnnotation ("JsValue") {}

        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual bool canAot(set<Structure *> &) const { return false; }
        virtual bool canMove() const override { return false; }
        virtual bool canCopy() const override { return false; }
        virtual bool isPod() const override { return false; }
        virtual bool isRefType() const override { return true; }
        virtual size_t getSizeOf() const override { return sizeof(JsValue); }
        virtual size_t getAlignOf() const override { return alignof(JsValue); }

        TypeDeclPtr makeJsValuePtr() const {
            auto pT = make_shared<TypeDecl>(Type::tPointer);
            pT->firstType = make_shared<TypeDecl>(Type::tHandle);
            auto THAT = (JsValueTypeAnnotation *) this;
            pT->firstType->annotation = static_pointer_cast<TypeAnnotation>(THAT->shared_from_this());
            return pT;
        }

        virtual bool isIndexable ( const TypeDeclPtr & idt ) const override {
            if ( idt->isSimpleType(Type::tString) || idt->isIndex() ) {
                return true;
            }
            return false;
        }

        virtual TypeDeclPtr makeIndexType ( const ExpressionPtr &, const ExpressionPtr & ) const override {
            return makeJsValuePtr();
        }

        virtual TypeDeclPtr makeFieldType ( const string & fn ) const override {
            if ( fn=="is_int" || fn=="is_float" || fn=="is_double" || fn=="is_object" || fn=="is_array" ||
                fn=="is_null" || fn=="is_string" || fn=="is_bool" ) {
                return make_shared<TypeDecl>(Type::tBool);
            } else if ( fn=="as_bool" ) {
                return make_shared<TypeDecl>(Type::tBool);
            } else if ( fn=="as_int" || fn=="size" ) {
                return make_shared<TypeDecl>(Type::tInt);
            } else if ( fn=="as_float" ) {
                return make_shared<TypeDecl>(Type::tFloat);
            } else if ( fn=="as_double" ) {
                return make_shared<TypeDecl>(Type::tDouble);
            }else if ( fn=="as_string" ) {
                return make_shared<TypeDecl>(Type::tString);
            } else {
                return makeJsValuePtr();
            }
        }

        virtual TypeDeclPtr makeSafeFieldType ( const string & fn ) const override {
            Type bt;
            if ( fn=="is_int" || fn=="is_float" ||  fn=="is_double" || fn=="is_object" || fn=="is_array" ||
                fn=="is_null" || fn=="is_string" || fn=="is_bool" ) {
                return nullptr;
            }
            else if ( fn=="as_bool" ) {
                bt = Type::tBool;
            } else if ( fn=="as_int" || fn=="size" ) {
                bt = Type::tInt;
            } else if ( fn=="as_float" ) {
                bt = Type::tFloat;
            } else if ( fn=="as_double" ) {
                bt = Type::tDouble;
            } else if ( fn=="as_string" ) {
                bt = Type::tString;
            } else {
                return makeJsValuePtr();
            }
            auto pt = make_shared<TypeDecl>(Type::tPointer);
            pt->firstType = make_shared<TypeDecl>(bt);
            return pt;
        }
        virtual SimNode * simulateRef2Value ( Context &, const LineInfo &, SimNode * ) const override {
            assert(0 && "implement");
            return nullptr;
        }

        template <bool SAFE>
        SimNode * simulateIsField ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr ) const {
            if (fn == "is_object") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsObject),&JsValue::IsObject,SAFE>>(at, subexpr);
            } else if (fn == "is_array") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsArray),&JsValue::IsArray,SAFE>>(at, subexpr);
            } else if (fn == "is_string") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsString),&JsValue::IsString,SAFE>>(at, subexpr);
            } else if (fn == "is_null") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsNull),&JsValue::IsNull,SAFE>>(at, subexpr);
            } else if (fn == "is_bool") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsBool),&JsValue::IsBool,SAFE>>(at, subexpr);
            } else if (fn == "is_int") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsInt),&JsValue::IsInt,SAFE>>(at, subexpr);
            } else if (fn == "is_float") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsFloat),&JsValue::IsFloat,SAFE>>(at, subexpr);
            } else if (fn == "is_double") {
                return context.code->makeNode<SimNode_Property<
                    JsValue,decltype(&JsValue::IsDouble),&JsValue::IsDouble,SAFE>>(at, subexpr);
            } else if (fn == "is_number") {
                return context.code->makeNode<SimNode_Property<
                JsValue,decltype(&JsValue::IsNumber),&JsValue::IsNumber,SAFE>>(at, subexpr);
            } else {
                return nullptr;
            }
        }

        virtual SimNode * simulateGetField ( const string & fn, Context & context,
                                            const LineInfo & at, const ExpressionPtr & sube ) const override {
            auto subexpr = sube->simulate(context);
            // IS XXX
            auto isField = simulateIsField<false>(fn, context, at, subexpr);
            if ( isField ) {
                return isField;
            }
            // AS XXX
            else if (fn == "as_bool") {
                return context.code->makeNode<SimNode_AsBool>(at, subexpr);
            } else if (fn == "as_int") {
                return context.code->makeNode<SimNode_AsInt>(at, subexpr);
            } else if (fn == "as_float") {
                return context.code->makeNode<SimNode_AsFloat>(at, subexpr);
            } else if (fn == "as_double") {
                return context.code->makeNode<SimNode_AsDouble>(at, subexpr);
            }else if (fn == "as_string") {
                return context.code->makeNode<SimNode_AsString>(at, subexpr);
            }
            // ARRAY SIZE
            else if (fn == "size") {
                return context.code->makeNode<SimNode_GetJsonArraySize>(at, subexpr);
            }
            // FIELD
            else {
                auto fieldName = context.code->allocateName(fn);
                return context.code->makeNode<SimNode_GetJsonFieldConst<false>>(at, subexpr, fieldName);
            }
        }

        virtual SimNode * simulateGetFieldR2V ( const string &, Context &,
                                               const LineInfo &, const ExpressionPtr & ) const override {
            assert(0 && "implement");
            return nullptr;
        }

        virtual SimNode * simulateSafeGetField ( const string & fn, Context & context,
                                                const LineInfo & at, const ExpressionPtr & sube ) const override {
            auto subexpr = sube->simulate(context);
            // IS XXX
            auto isField = simulateIsField<true>(fn, context, at, subexpr);
            if ( isField ) {
                return isField;
            }
            assert(0 && "implement");
            return nullptr;
        }

        virtual SimNode * simulateSafeGetFieldPtr ( const string & fn, Context & context,
                                                   const LineInfo & at, const ExpressionPtr & sube) const override {
            auto subexpr = sube->simulate(context);
            // AS XXX
            if (fn == "as_bool") {
                return context.code->makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsBool),&JsValue::IsBool>>(at, subexpr);
            } else if (fn == "as_int") {
                return context.code->makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsInt),&JsValue::IsInt>>(at, subexpr);
            } else if (fn == "as_float") {
                return context.code->makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsFloat),&JsValue::IsFloat>>(at, subexpr);
            } else if (fn == "as_double") {
                return context.code->makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsDouble),&JsValue::IsDouble>>(at, subexpr);
            } else if (fn == "as_string") {
                return context.code->makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsString),&JsValue::IsString>>(at, subexpr);
            }
            // FIELD
            else {
                auto fieldName = context.code->allocateName(fn);
                return context.code->makeNode<SimNode_GetJsonFieldConst<true>>(at, subexpr, fieldName);
            }
            return nullptr;
        }

        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr & td,
                                         const ExpressionPtr & val, const ExpressionPtr & idx, uint32_t ofs ) const override {
            if ( ofs ) context.thisProgram->error("internal error, offset in JSON node", at);
            if ( td->isSimpleType(Type::tString) ) {
                return context.code->makeNode<SimNode_GetJsonField>(at,
                                                                    val->simulate(context),
                                                                    idx->simulate(context));
            } else if ( td->isIndex() ){
                return context.code->makeNode<SimNode_GetJsonAt>(at,
                                                                 val->simulate(context),
                                                                 idx->simulate(context));
            } else {
                return nullptr;
            }
        }

        virtual SimNode * simulateGetAtR2V ( Context & context, const LineInfo & at, const TypeDeclPtr & td,
                                         const ExpressionPtr & val, const ExpressionPtr & idx, uint32_t ofs ) const override {
            context.thisProgram->error("internal error, simulateGetAtR2V for JSON node", at);
            return simulateGetAt(context, at, td, val, idx, ofs);
        }

        void print(TextWriter & ss, const JsValue & value) const {
            rapidjson::StringBuffer sb;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
            value.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
            ss << sb.GetString();
        }

        virtual void walk ( DataWalker & walker, void * ptr ) override {
            auto pjs = (JsValue *) ptr;
            if ( walker.reading ) {
                assert(0 && "todo: implement reading json here");
            } else {
                rapidjson::StringBuffer sb;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
                pjs->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
                char * ss = (char *) sb.GetString();
                walker.String(ss);
            }
        }
    };

    void readJson ( char * str, const Block & block, Context * context ) {
        rapidjson::Document doc;
        vec4f args[2];
        if ( doc.Parse(str).HasParseError() ) {
            args[0] = v_zero();
            args[1] = cast<int32_t>::from ( int32_t(doc.GetParseError()) );
        } else {
            args[0] = cast<char *>::from((char *)&doc);
            args[1] = v_zero();
        }
        context->invoke(block, args, nullptr);
    }

    void json_set_i ( JsValue * rv, int32_t iv, Context * context ) {
        if ( !rv->IsInt() ) { context->throw_error("JSON, not an integer"); return; }
        rv->SetInt(iv);
    }

    void json_set_f ( JsValue * rv, float fv, Context * context ) {
        if ( !rv->IsFloat() ) { context->throw_error("JSON, not an float"); return; }
        rv->SetFloat(fv);
    }

    void json_set_b ( JsValue * rv, bool bv, Context * context ) {
        if ( !rv->IsBool() ) { context->throw_error("JSON, not an boolean"); return; }
        rv->SetBool(bv);
    }

    void json_set_s ( JsValue * rv, char * sv, Context * context ) {
        if ( !rv->IsString() ) { context->throw_error("JSON, not an string"); return; }
        auto len = stringLength(*context, sv);
        rv->SetString(sv, len);
    }
    
    // note: more efficient implementation would have two separate classes
    //          this, however, allows us to keep reader next to writer
    //          its likely that that "reading" should be a template
    struct JsonWalker : DataWalker {
        rapidjson::Document * root;
        JsValue * top;
        vector<JsValue *> stack;
        JsonWalker ( Context & ctx, rapidjson::Document * r, bool weRead ) {
            context = &ctx;
            reading = weRead;
            top = root = r;
        }
        void push() {
            stack.push_back(top);
        }
        void pop() {
            top = stack.back();
            stack.pop_back();
        }
        // data structures
        virtual void beforeStructure ( char *, StructInfo * ) override {
            if ( reading ) {
                if ( !top->IsObject() ) {
                    error("JSON, expecing object");
                    return;
                }
            } else {
                top->SetObject();
            }
        }
        virtual void beforeStructureField ( char *, StructInfo *, char *, VarInfo * vi, bool ) override {
            push();
            rapidjson::Document::StringRefType name (vi->name);
            if ( !reading ) {
                top->AddMember(name, false, root->GetAllocator());
                top = &top->FindMember(name)->value;
            } else if ( top ) {
                auto member = top->FindMember(name);
                if ( member != top->MemberEnd() ) {
                    top = &member->value;
                } else {
                    top = nullptr;
                }
            }
        }
        virtual void afterStructureField ( char *, StructInfo *, char *, VarInfo *, bool ) override {
            pop();
        }
        virtual void beforeArray ( Array * pa, TypeInfo * ti ) override {
            if ( reading ) {
                if ( top && top->IsArray() ) {
                    uint32_t capacity = top->Capacity();
                    uint32_t stride = getTypeSize(ti->firstType);
                    array_resize ( *context, *pa, capacity, stride, true );
                }
            }
        }
        virtual void beforeArrayData ( char *, uint32_t, uint32_t count, TypeInfo * ) override {
            if ( reading ) {
                if ( top ) {
                    if ( !top->IsArray() ) {
                        error("JSON, expecting array");
                        return;
                    } else if ( top->Capacity()!=count ) {
                        error("JSON, mismatching array capacity");
                        return;
                    }
                }
            } else {
                top->SetArray();
                top->Clear();
                top->Reserve(count, root->GetAllocator());
            }
        }
        virtual void beforeArrayElement ( char *, TypeInfo *, char *, uint32_t index, bool ) override {
            push();
            if ( reading ) {
                if ( top ) {
                    top = &(*top)[index];
                }
            } else {
                top->PushBack(false, root->GetAllocator());
                top = &(*top)[index];
            }
        }
        virtual void afterArrayElement ( char *, TypeInfo *, char *, uint32_t, bool ) override {
            pop();
        }
        virtual void beforeTable ( Table * tab, TypeInfo * info ) override {
            if ( reading ) {
                if ( top ) {
                    if ( !top->IsArray() ) {
                        error("JSON, expecting array");
                        return;
                    }
                    // here we clear the table, and reserve spots for all keys
                    // that way we can iterate though table elements, and read values
                    table_clear(*context, *tab);
                    auto stride = getTypeSize(info->secondType);
                    for ( auto it = top->Begin(); it != top->End(); ++it ) {
                        auto mKey = it->FindMember("key");
                        if ( mKey != it->MemberEnd() ) {
                            switch ( info->firstType->type ) {
                                case Type::tString:
                                    if ( mKey->value.IsString() ) {
                                        auto key = (char *) mKey->value.GetString();
                                        TableHash<char *> thh(context, stride);
                                        auto hfn = hash_function(*context, key);
                                        thh.reserve(*tab, key, hfn);
                                    } else {
                                        error("JSON, key type must be string");
                                    }
                                    break;
                                case Type::tInt:
                                case Type::tUInt:
                                    // code similar to that above for the string needs to be written
                                    // for each workhorse type
                                    assert(0 && "TODO: implment for all workhorse types");
                                default:
                                    error("JSON, unexpected key type");
                                    return;
                            }
                        } else {
                            error("JSON, expecting key");
                            return;
                        }
                    }
                }
            } else {
                top->SetArray();
                top->Clear();
                top->Reserve(tab->size, root->GetAllocator());
            }
        }
        virtual void beforeTableKey ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t index, bool ) override {
            push();
            if ( reading ) {
                top = nullptr;  // we already created key in beforeTable
            } else {
                top->PushBack(false, root->GetAllocator());
                top = &(*top)[index];
                top->SetObject();
                top->AddMember("key",false,root->GetAllocator());
                top = &top->FindMember("key")->value;
            }
        }
        virtual void afterTableKey ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t, bool ) override {
            pop();
        }
        virtual void beforeTableValue ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t index, bool ) override {
            push();
            if ( reading ) {
                if ( top ) {
                    top = &(*top)[index];
                    auto member = top->FindMember("value");
                    if ( member != top->MemberEnd() ) {
                        top = &member->value;
                    } else {
                        error("JSON, expecting value");
                        return;
                    }
                }
            } else {
                top = &(*top)[index];
                top->AddMember("value",false,root->GetAllocator());
                top = &top->FindMember("value")->value;
            }
        }
        virtual void afterTableValue ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t, bool ) override {
            pop();
        }
        virtual void beforePtr ( char *, TypeInfo * ) override {
            context->throw_error("can't serialize pointer");
        }
        virtual void beforeHandle ( char *, TypeInfo * ) override {
            // nothing, right?
        }
        // types
        virtual void Null ( TypeInfo * ) override {
            if ( reading ) {
                if ( top && !top->IsNull() ) {
                    error("JSON, expecting NULL");
                    return;
                }
            } else {
                top->SetNull();
            }
        }
        
        virtual void String ( char * & st ) override {
            if ( reading ) {
                if ( top ) {
                    if ( !top->IsString() ) {
                        error("JSON, type mismatch, expecting string");
                        return;
                    }
                    auto str = top->GetString();
                    auto strl = top->GetStringLength();
                    st = context->heap.allocateString(str, strl);
                }
            } else {
                if ( st ) {
                    top->SetString(st, stringLength(*context, st));
                } else {
                    top->SetString("",0);
                }
            }
        }

#define WALK_PROPERTY(TYPE,CTYPE,JSTYPE) \
        virtual void TYPE ( CTYPE & b ) override { \
            if ( reading ) { \
                if ( top ) { \
                    if ( !top->Is##JSTYPE() ) { \
                        error("JSON, type mismatch"); \
                        return; \
                    } \
                    b = top->Get##JSTYPE(); \
                } \
            } else { \
                top->Set##JSTYPE(b); \
            } \
        }
        
        WALK_PROPERTY(Bool,bool,Bool);
        WALK_PROPERTY(Int64,int64_t,Int64);
        WALK_PROPERTY(UInt64,uint64_t,Uint64);
        WALK_PROPERTY(Float,float,Float);
        WALK_PROPERTY(Int,int32_t,Int);
        WALK_PROPERTY(UInt,uint32_t,Uint);

#define WALK_PROPERTY_VEC2(TYPE,CTYPE,JSTYPE) \
        virtual void TYPE ( CTYPE & v ) override { \
            if ( reading ) { \
                if ( top ) { \
                    if ( !top->IsObject() ) { \
                        error("JSON, type mismatch, expecting object with x and y"); \
                        return; \
                    } \
                    auto mX = top->FindMember("x"); \
                    auto mY = top->FindMember("y"); \
                    if ( mX != top->MemberEnd() ) v.x = mX->value.Get##JSTYPE(); \
                    if ( mY != top->MemberEnd() ) v.y = mY->value.Get##JSTYPE(); \
                } \
            } else { \
                top->SetObject(); \
                top->AddMember("x", v.x, root->GetAllocator()); \
                top->AddMember("y", v.y, root->GetAllocator()); \
            } \
        }
        
        WALK_PROPERTY_VEC2(Int2,int2,Int);
        WALK_PROPERTY_VEC2(UInt2,uint2,Uint);
        WALK_PROPERTY_VEC2(Float2,float2,Float);
 
#define WALK_PROPERTY_VEC3(TYPE,CTYPE,JSTYPE) \
        virtual void TYPE ( CTYPE & v ) override { \
            if ( reading ) { \
                if ( top ) { \
                    if ( !top->IsObject() ) { \
                        error("JSON, type mismatch, expecting object with x and y"); \
                        return; \
                    } \
                    auto mX = top->FindMember("x"); \
                    auto mY = top->FindMember("y"); \
                    auto mZ = top->FindMember("z"); \
                    if ( mX != top->MemberEnd() ) v.x = mX->value.Get##JSTYPE(); \
                    if ( mY != top->MemberEnd() ) v.y = mY->value.Get##JSTYPE(); \
                    if ( mZ != top->MemberEnd() ) v.z = mZ->value.Get##JSTYPE(); \
                } \
            } else { \
                top->SetObject(); \
                top->AddMember("x", v.x, root->GetAllocator()); \
                top->AddMember("y", v.y, root->GetAllocator()); \
                top->AddMember("z", v.z, root->GetAllocator()); \
            } \
        }
        
        WALK_PROPERTY_VEC3(Int3,int3,Int);
        WALK_PROPERTY_VEC3(UInt3,uint3,Uint);
        WALK_PROPERTY_VEC3(Float3,float3,Float);
        
#define WALK_PROPERTY_VEC4(TYPE,CTYPE,JSTYPE) \
        virtual void TYPE ( CTYPE & v ) override { \
            if ( reading ) { \
                if ( top ) { \
                    if ( !top->IsObject() ) { \
                        error("JSON, type mismatch, expecting object with x and y"); \
                        return; \
                    } \
                    auto mX = top->FindMember("x"); \
                    auto mY = top->FindMember("y"); \
                    auto mZ = top->FindMember("z"); \
                    if ( mX != top->MemberEnd() ) v.x = mX->value.Get##JSTYPE(); \
                    if ( mY != top->MemberEnd() ) v.y = mY->value.Get##JSTYPE(); \
                    if ( mZ != top->MemberEnd() ) v.z = mZ->value.Get##JSTYPE(); \
                } \
            } else { \
                top->SetObject(); \
                top->AddMember("x", v.x, root->GetAllocator()); \
                top->AddMember("y", v.y, root->GetAllocator()); \
                top->AddMember("z", v.z, root->GetAllocator()); \
            } \
        }
        
        WALK_PROPERTY_VEC4(Int4,int4,Int);
        WALK_PROPERTY_VEC4(UInt4,uint4,Uint);
        WALK_PROPERTY_VEC4(Float4,float4,Float);
        
#define WALK_PROPERTY_RANGE(TYPE,CTYPE,JSTYPE) \
        virtual void TYPE ( CTYPE & v ) override { \
            if ( reading ) { \
                if ( top ) { \
                    if ( !top->IsObject() ) { \
                        error("JSON, type mismatch, expecting object with x and y"); \
                        return; \
                    } \
                    auto mX = top->FindMember("from"); \
                    auto mY = top->FindMember("to"); \
                    if ( mX != top->MemberEnd() ) v.from = mX->value.Get##JSTYPE(); \
                    if ( mY != top->MemberEnd() ) v.to = mY->value.Get##JSTYPE(); \
                } \
            } else { \
                top->SetObject(); \
                top->AddMember("from", v.from, root->GetAllocator()); \
                top->AddMember("to", v.to, root->GetAllocator()); \
            } \
        }
        
        WALK_PROPERTY_RANGE(Range,range,Int);
        WALK_PROPERTY_RANGE(URange,urange,Uint);
        
        virtual void WalkIterator ( struct Iterator * ) override {
            error("can't serialize iterator");
        }
        virtual void WalkBlock ( Block * ) override {
            error("can't serialize block");
        }
    };
    
    vec4f _builtin_save_json ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        rapidjson::Document document;
        JsonWalker writer(context, &document, false);
        // args
        Block * block = cast<Block *>::to(args[1]);
        auto info = call->types[0];
        writer.walk(args[0], info);
        vec4f bargs[1];
        bargs[0] = cast<void *>::from(&document);
        context.invoke(*block, bargs, nullptr);
        return v_zero();
    }
    
    vec4f _builtin_load_json ( Context & context, SimNode_CallBase * call, vec4f * args ) {
        rapidjson::Document * document = cast<rapidjson::Document *>::to(args[1]);
        auto info = call->types[0];
        JsonWalker writer(context, document, true);
        writer.walk(args[0], info);
        return v_zero();
    }
    
    #include "json.das.inc"

    class Module_RapidJson : public Module {
    public:
        Module_RapidJson() : Module("RapidJson") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // JsValue type
            addAnnotation(make_shared<JsValueTypeAnnotation>());
            // functionality
            addExtern<DAS_BIND_FUN(readJson)>(*this,lib,"_builtin_parse_json",SideEffects::modifyExternal);
            addInterop<_builtin_save_json,void,vec4f,const Block>(*this, lib, "_builtin_save_json",SideEffects::modifyExternal);
            addInterop<_builtin_load_json,void,vec4f,const JsValue*>(*this, lib, "readFromJson",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(json_set_i)>(*this,lib,"set",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(json_set_f)>(*this,lib,"set",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(json_set_b)>(*this,lib,"set",SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(json_set_s)>(*this,lib,"set",SideEffects::modifyExternal);
            // and builtin module
            compileBuiltinModule("json.das",json_das, json_das_len);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_RapidJson, das);
