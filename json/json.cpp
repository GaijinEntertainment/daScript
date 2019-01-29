#include "daScript/daScript.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

namespace das {

    using namespace std;

    typedef rapidjson::Value JsValue;

    template <>
    struct das::typeFactory<JsValue> {
        static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {
            return library.makeHandleType("JsValue");
        }
    };

    template <>
    struct das::typeFactory<JsValue *> {
        static das::TypeDeclPtr make(const das::ModuleLibrary & library ) {
            auto pt = make_shared<TypeDecl>(Type::tPointer);
            pt->firstType = library.makeHandleType("JsValue");
            return pt;
        }
    };

    template <typename Fun, Fun BOOL_PROP>
    struct SimNode_GetBoolProperty : SimNode {
        DAS_BOOL_NODE;
        SimNode_GetBoolProperty(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline bool compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) return false;
            return (pv->*BOOL_PROP)();
        }
        SimNode * subexpr;
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
            if ( !pv ) { context.throw_error("JSON dereferencing null pointer"); return false; }
            if ( !pv->IsBool() ) { context.throw_error("JSON not a boolean"); return false; }
            return pv->GetBool();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsInt : SimNode {
        DAS_INT_NODE;
        SimNode_AsInt(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline int32_t compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error("JSON dereferencing null pointer"); return 0; }
            if ( !pv->IsInt() ) { context.throw_error("JSON not a int"); return 0; }
            return pv->GetInt();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsFloat : SimNode {
        DAS_FLOAT_NODE;
        SimNode_AsFloat(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline float compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error("JSON dereferencing null pointer"); return 0.0f; }
            if ( !pv->IsFloat() ) { context.throw_error("JSON not a float"); return 0.0f; }
            return (float) pv->GetDouble();
        }
        SimNode * subexpr;
    };

    struct SimNode_AsString : SimNode {
        DAS_PTR_NODE;
        SimNode_AsString(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        __forceinline char * compute(Context & context) {
            auto pv = (JsValue *) subexpr->evalPtr(context);
            if ( !pv ) { context.throw_error("JSON dereferencing null pointer"); return nullptr; }
            if ( !pv->IsString() ) { context.throw_error("JSON not a string"); return nullptr; }
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
                if ( !SAFE ) context.throw_error("JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsObject() ) {
                if ( !SAFE ) context.throw_error("JSON field is not an object");
                return nullptr;
            }
            auto hf = pv->FindMember(index);
            if ( hf == pv->MemberEnd() ) {
                if ( !SAFE ) context.throw_error("JSON field not found");
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
                context.throw_error("JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsObject() ) {
                context.throw_error("JSON field is not an object");
                return nullptr;
            }
            auto pi = index->evalPtr(context);
            auto hf = pv->FindMember(pi);
            if ( hf == pv->MemberEnd() ) {
                context.throw_error("JSON field not found");
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
                context.throw_error("JSON dereferencing null pointer");
                return nullptr;
            }
            if ( !pv->IsArray() ) {
                context.throw_error("JSON field is not an array");
                return nullptr;
            }
            auto idx = cast<uint32_t>::to(index->eval(context));
            if ( idx >= pv->Size() ) {
                context.throw_error("JSON array index out of range");
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

        virtual TypeDeclPtr makeIndexType ( TypeDeclPtr & ) const override {
            return makeJsValuePtr();
        }

        virtual TypeDeclPtr makeFieldType ( const string & fn ) const override {
            if ( fn=="is_int" || fn=="is_float" || fn=="is_object" || fn=="is_array" ||
                fn=="is_null" || fn=="is_string" || fn=="is_bool" ) {
                return make_shared<TypeDecl>(Type::tBool);
            } else if ( fn=="as_bool" ) {
                return make_shared<TypeDecl>(Type::tBool);
            } else if ( fn=="as_int" || fn=="size" ) {
                return make_shared<TypeDecl>(Type::tInt);
            } else if ( fn=="as_float" ) {
                return make_shared<TypeDecl>(Type::tFloat);
            } else if ( fn=="as_string" ) {
                return make_shared<TypeDecl>(Type::tString);
            } else {
                return makeJsValuePtr();
            }
        }

        virtual TypeDeclPtr makeSafeFieldType ( const string & fn ) const override {
            Type bt;
            if ( fn=="is_int" || fn=="is_float" || fn=="is_object" || fn=="is_array" ||
                fn=="is_null" || fn=="is_string" || fn=="is_bool" ) {
                return nullptr;
            }
            else if ( fn=="as_bool" ) {
                bt = Type::tBool;
            } else if ( fn=="as_int" || fn=="size" ) {
                bt = Type::tInt;
            } else if ( fn=="as_float" ) {
                bt = Type::tFloat;
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

        SimNode * simulateIsField ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr ) const {
            if (fn == "is_object") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsObject),&JsValue::IsObject>>(at, subexpr);
            } else if (fn == "is_array") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsArray),&JsValue::IsArray>>(at, subexpr);
            } else if (fn == "is_string") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsString),&JsValue::IsString>>(at, subexpr);
            } else if (fn == "is_null") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsNull),&JsValue::IsNull>>(at, subexpr);
            } else if (fn == "is_bool") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsBool),&JsValue::IsBool>>(at, subexpr);
            } else if (fn == "is_int") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsInt),&JsValue::IsInt>>(at, subexpr);
            } else if (fn == "is_float") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsDouble),&JsValue::IsDouble>>(at, subexpr);
            } else if (fn == "is_number") {
                return context.code.makeNode<SimNode_GetBoolProperty<
                decltype(&JsValue::IsNumber),&JsValue::IsNumber>>(at, subexpr);
            } else {
                return nullptr;
            }
        }

        virtual SimNode * simulateGetField ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr ) const override {
            // IS XXX
            auto isField = simulateIsField(fn, context, at, subexpr);
            if ( isField ) {
                return isField;
            }
            // AS XXX
            else if (fn == "as_bool") {
                return context.code.makeNode<SimNode_AsBool>(at, subexpr);
            } else if (fn == "as_int") {
                return context.code.makeNode<SimNode_AsInt>(at, subexpr);
            } else if (fn == "as_float") {
                return context.code.makeNode<SimNode_AsFloat>(at, subexpr);
            } else if (fn == "as_string") {
                return context.code.makeNode<SimNode_AsString>(at, subexpr);
            }
            // ARRAY SIZE
            else if (fn == "size") {
                return context.code.makeNode<SimNode_GetJsonArraySize>(at, subexpr);
            }
            // FIELD
            else {
                auto fieldName = context.code.allocateName(fn);
                return context.code.makeNode<SimNode_GetJsonFieldConst<false>>(at, subexpr, fieldName);
            }
        }

        virtual SimNode * simulateGetFieldR2V ( const string &, Context &, const LineInfo &, SimNode * ) const override {
            assert(0 && "implement");
            return nullptr;
        }

        virtual SimNode * simulateSafeGetField ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr ) const override {
            // IS XXX
            auto isField = simulateIsField(fn, context, at, subexpr);
            if ( isField ) {
                return isField;
            }
            assert(0 && "implement");
            return nullptr;
        }

        virtual SimNode * simulateSafeGetFieldPtr ( const string & fn, Context & context, const LineInfo & at, SimNode * subexpr) const override {
            // AS XXX
            if (fn == "as_bool") {
                return context.code.makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsBool),&JsValue::IsBool>>(at, subexpr);
            } else if (fn == "as_int") {
                return context.code.makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsInt),&JsValue::IsInt>>(at, subexpr);
            } else if (fn == "as_float") {
                return context.code.makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsFloat),&JsValue::IsFloat>>(at, subexpr);
            } else if (fn == "as_string") {
                return context.code.makeNode<SimNode_SafeAs<
                    decltype(&JsValue::IsString),&JsValue::IsString>>(at, subexpr);
            }
            // FIELD
            else {
                auto fieldName = context.code.allocateName(fn);
                return context.code.makeNode<SimNode_GetJsonFieldConst<true>>(at, subexpr, fieldName);
            }
            return nullptr;
        }

        virtual SimNode * simulateGetAt ( Context & context, const LineInfo & at, const TypeDeclPtr & td,
                                         SimNode * val, SimNode * idx ) const override {
            if ( td->isSimpleType(Type::tString) ) {
                return context.code.makeNode<SimNode_GetJsonField>(at, val, idx);
            } else if ( td->isIndex() ){
                return context.code.makeNode<SimNode_GetJsonAt>(at, val, idx);
            } else {
                return nullptr;
            }
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

    void readJson ( char * str, Block block, Context * context ) {
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
            addExtern<decltype(readJson),readJson>(*this,lib,"_builtin_parse_json");
            addExtern<decltype(json_set_i),json_set_i>(*this,lib,"set");
            addExtern<decltype(json_set_f),json_set_f>(*this,lib,"set");
            addExtern<decltype(json_set_b),json_set_b>(*this,lib,"set");
            addExtern<decltype(json_set_s),json_set_s>(*this,lib,"set");
            // and builtin module
            compileBuiltinModule(json_das, json_das_len);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_RapidJson, das);
