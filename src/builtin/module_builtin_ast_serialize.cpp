#include "daScript/misc/platform.h"

#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast.h"

namespace das {

    AstSerializer::AstSerializer ( const vector<uint8_t> & from ) {
        astModule = Module::require("ast");
        buffer = from;
        writing = false;
    }

    AstSerializer::AstSerializer ( void ) {
        astModule = Module::require("ast");
        writing = true;
    }

    // void AstSerializer::patch() {
    //     for ( auto & p : functionRefs ) {
    //         auto it = functionMap.find(p.second);
    //         if ( it == functionMap.end() ) {
    //             DAS_FATAL_ERROR("ast serializer function ref not found");
    //         } else {
    //             *p.first = it->second.get();
    //         }
    //     }
    // }

    void AstSerializer::write ( void * data, size_t size ) {
        auto oldSize = buffer.size();
        buffer.resize(oldSize+size);
        memcpy(buffer.data()+oldSize, data, size);
    }

    void AstSerializer::read  ( const void * data, size_t size ) {
        if ( readOffset + size > buffer.size() ) {
            DAS_FATAL_ERROR("ast serializer read overflow");
            return;
        }
        memcpy((void *)data, buffer.data()+readOffset, size);
    }

    void AstSerializer::serialize ( void * data, size_t size ) {
        if ( writing ) {
            write(data,size);
        } else {
            read(data,size);
        }
    }

    void AstSerializer::tag ( const char * name ) {
        uint64_t hash = hash64z(name);
        if ( writing ) {
            *this << hash;
        } else  {
            uint64_t hash2 = 0;
            *this << hash2;
            if ( hash != hash2 ) {
                DAS_FATAL_ERROR("ast serializer tag mismatch");
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    AstSerializer & AstSerializer::operator << ( Type & baseType ) {
        serialize_enum(baseType);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExpressionPtr & expr ) {
        if ( writing ) {
            string rtti = expr->__rtti;
            *this << rtti;
            expr->serialize(*this);
        } else {
            string rtti;
            *this << rtti;
            auto annotation = astModule->findAnnotation(rtti);
            DAS_VERIFYF(annotation!=nullptr, "annotation '%s' is not found", rtti.c_str());
            expr.reset((Expression *) static_pointer_cast<TypeAnnotation>(annotation)->factory());
            expr->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FunctionPtr & func ) {
        if ( writing ) {
            DAS_ASSERTF(!func->builtIn, "cannot serialize built-in function");
            DAS_ASSERTF(func->module==thisModule, "function is not from this module");
            uint64_t fid = intptr_t(func.get());
            *this << fid;
            if ( functionMap.find(fid) == functionMap.end() ) {
                functionMap[fid] = func;
                func->serialize(*this);
            }
        } else {
            uint64_t fid = 0;
            *this << fid;
            if ( fid ) {
                auto it = functionMap.find(fid);
                if ( it == functionMap.end() ) {
                    func = make_smart<Function>();
                    functionMap[fid] = func;
                    func->serialize(*this);
                } else {
                    func = it->second;
                }
            } else {
                func = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Function * & func ) {
        if ( writing ) {
            bool inThisModule = func->module == thisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                uint64_t fid = intptr_t(func);
                *this << fid;
            } else {
                *this << func->module->name;
                string mangeldName = func->getMangledName();
                *this << mangeldName;
            }
        } else {
            bool inThisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                uint64_t fid;
                *this << fid;
                if ( fid ) {
                    auto it = functionMap.find(fid);
                    if ( it == functionMap.end() ) {
                        functionRefs.push_back(make_pair(&func,fid));
                        func = nullptr;
                    } else {
                        func = it->second.get();
                    }
                } else {
                    func = nullptr;
                }
            } else {
                string moduleName, mangledName;
                *this << moduleName;
                auto funcModule = moduleLibrary->findModule(moduleName);
                DAS_VERIFYF(funcModule!=nullptr, "module '%s' is not found", moduleName.c_str());
                *this << mangledName;
                func = funcModule->findFunction(mangledName).get();
                DAS_VERIFYF(func!=nullptr, "function '%s' is not found", mangledName.c_str());
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeDeclPtr & type ) {
        if ( !writing ) type = make_smart<TypeDecl>();
        type->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( AnnotationArgument & arg ) {
        arg.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( AnnotationDeclarationPtr & annotation_decl ) {
        if ( !writing ) annotation_decl = make_smart<AnnotationDeclaration>();
        annotation_decl->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( AnnotationPtr & anno ) {
        if ( !writing ) anno = make_smart<Annotation>("", "");
        anno->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Structure::FieldDeclaration & field_declaration ) {
        field_declaration.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Structure::FieldDeclaration * & field_declaration ) {
        bool null = field_declaration == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) field_declaration = new Structure::FieldDeclaration;
            field_declaration->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( string & str ) {
        if ( writing ) {
            uint32_t size = (uint32_t) str.size();
            *this << size;
            write((void *)str.data(), size);
        } else {
            uint32_t size = 0;
            * this << size;
            str.resize(size);
            read(&str[0], size);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( LineInfo & at ) {
        *this << at.fileInfo;
        *this << at.column << at.line;
        *this << at.last_column << at.last_line;
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FileInfo * & info ) {
        // TODO: implement (likely add 'serialize' and 'factory' to FileAccess)
        if ( !writing ) {
            info = nullptr; // temporary!!!
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Structure * & struct_ ) {
        if ( writing && struct_ == nullptr ) return *this;
        if ( struct_ == nullptr ) struct_ = new Structure;
        struct_->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Enumeration * & enum_type ) {
        //TODO:
        enum_type->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( EnumerationPtr & enum_type ) {
        enum_type->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Enumeration::EnumEntry & entry ) {
        entry.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeAnnotationPtr & type_anno ) {
        type_anno->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeAnnotation * & type_anno ) {
        type_anno->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( VariablePtr & var ) {
        var->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Variable * & var ) {
        var->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Module * & module ) {
        if ( writing ) {
            *this << module->name;
        } else {
            DAS_VERIFYF(moduleLibrary!=nullptr, "module library is not set");
            string name;
            *this << name;
            module = moduleLibrary->findModule(name);
            DAS_VERIFYF(module!=nullptr, "module '%s' is not found", name.c_str());
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Function::AliasInfo & alias_info ) {
        alias_info.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ReaderMacroPtr & history ) {
        if ( !writing ) history = make_smart<ReaderMacro>();
        history->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExprBlock * & block ) {
        bool null = block == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) block = new ExprBlock;
            block->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExprClone * & clone ) {
        bool null = clone == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) clone = new ExprClone;
            clone->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( InferHistory & history ) {
        history.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExprCallMacro * & macro ) {
        bool null = macro == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) macro = new ExprCallMacro();
            macro->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( CallMacro * & macro ) {
        bool null = macro == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) macro = new CallMacro();
            macro->serialize(*this);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( CaptureEntry & entry ) {
        *this << entry.name;
        serialize_enum<CaptureMode>(entry.mode);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( MakeFieldDeclPtr & make_field_decl_ptr ) {
        make_field_decl_ptr->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( MakeStructPtr & make_struct_ptr ) {
        make_struct_ptr->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeInfoMacro * & macro ) {
        bool null = macro == nullptr;
        *this << null;
        if ( !null ) {
            if ( !writing ) macro = new TypeInfoMacro("");
            macro->serialize(*this);
        }
        return *this;
    }

// typedecl

    void TypeDecl::serialize ( AstSerializer & ser ) {
        ser.tag("TypeDecl");
        ser << baseType;
        ser << structType;
        ser << enumType;
        ser << annotation;
        ser << dim;
        ser << dimExpr;
        ser << flags;
        ser << alias;
        ser << at;
        ser << module;
        ser << firstType;
        ser << secondType;
        ser << argTypes;
        ser << argNames;
    }

    void AnnotationArgument::serialize ( AstSerializer & ser ) {
        ser.tag("AnnotationArgument");
        ser << type << name << sValue << iValue << at;
    }

    void AnnotationArgumentList::serialize ( AstSerializer & ser ) {
        ser << * static_cast <AnnotationArguments *> (this);
    }

    void AnnotationDeclaration::serialize ( AstSerializer & ser ) {
        ser << annotation << arguments << at << flags;
        ptr_ref_count::serialize(ser);
    }

    void Annotation::serialize ( AstSerializer & ser ) {
        ser << module;
        BasicAnnotation::serialize(ser);
    }

    void BasicAnnotation::serialize ( AstSerializer & ser ) {
        ser << name << cppName;
        ptr_ref_count::serialize(ser);
    }

    void ptr_ref_count::serialize ( AstSerializer & ser ) {
#if DAS_SMART_PTR_ID
        ser << ref_count_id;
#endif
#if DAS_SMART_PTR_MAGIC
        ser << magic;
#endif
        ser << ref_count;
    }

    void Structure::FieldDeclaration::serialize ( AstSerializer & ser ) {
        ser.tag("FieldDeclaration");
        ser << name << type << ini << annotation << at << offset << flags;
    }

    void Enumeration::EnumEntry::serialize( AstSerializer & ser ) {
        ser << name << cppName << at << value;
    }

    void Enumeration::serialize ( AstSerializer & ser ) {
        ser << name << cppName << at << list << module << external << baseType
            << annotations << isPrivate;
        ptr_ref_count::serialize(ser);
    }

    void TypeAnnotation::serialize ( AstSerializer & ser ) {
        Annotation::serialize(ser);
    }

    void Structure::serialize ( AstSerializer & ser ) {
        ser.tag("Structure");
        ser << name << fields << fieldLookup << at << module
            << parent // parent could be in the current module or in some other
                      // module
            << annotations << flags;
        ptr_ref_count::serialize(ser);
    }

    void Variable::serialize ( AstSerializer & ser ) {
        ser << name << aka << type << init << source << at << index << stackTop
            << extraLocalOffset << module << useFunctions << useGlobalVariables
            << initStackSize << flags << access_flags << annotation;
        ptr_ref_count::serialize(ser);
    }

    void Function::AliasInfo::serialize ( AstSerializer & ser ) {
        ser << var << func << viaPointer;
    }

    void InferHistory::serialize ( AstSerializer & ser ) {
        ser << at << func;
    }

// function

    void Function::serialize ( AstSerializer & ser ) {
        ser.tag("Function");
        ser << annotations << name << arguments << result << body << index
            << totalStackSize << totalGenLabel << at << atDecl << module
            << useFunctions << useGlobalVariables << classParent
            << resultAliases << argumentAliases << resultAliasesGlobals << flags
            << moreFlags << sideEffectFlags << inferStack << fromGeneric << hash
            << aotHash;
    }

// Expressions

    void ReaderMacro::serialize ( AstSerializer & ser ) {
        ser << name << module;
        ptr_ref_count::serialize(ser);
    }

    void ExprReader::serialize ( AstSerializer & ser ) {
        ser << macro << sequence;
        Expression::serialize(ser);
    }

    void ExprLabel::serialize ( AstSerializer & ser ) {
        ser << label << comment;
        Expression::serialize(ser);
    }

    void ExprGoto::serialize ( AstSerializer & ser ) {
        ser << label << subexpr;
        Expression::serialize(ser);
    }

    void ExprRef2Value::serialize ( AstSerializer & ser ) {
        ser << subexpr;
        Expression::serialize(ser);
    }

    void ExprRef2Ptr::serialize ( AstSerializer & ser ) {
        ser << subexpr;
        Expression::serialize(ser);
    }

    void ExprPtr2Ref::serialize ( AstSerializer & ser ) {
        ser << subexpr << unsafeDeref;
        Expression::serialize(ser);
    }

    void ExprAddr::serialize ( AstSerializer & ser ) {
        ser << target << funcType << func;
        Expression::serialize(ser);
    }

    void ExprNullCoalescing::serialize ( AstSerializer & ser ) {
        ser << defaultValue;
        ExprPtr2Ref::serialize(ser);
    }

    void ExprDelete::serialize ( AstSerializer & ser ) {
        ser << subexpr << sizeexpr << native;
        Expression::serialize(ser);
    }

    void ExprAt::serialize ( AstSerializer & ser ) {
        ser << subexpr << index;
        ser << atFlags;
        Expression::serialize(ser);
    }

    void ExprSafeAt::serialize ( AstSerializer & ser ) {
        ExprAt::serialize(ser);
    }

    void ExprBlock::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
        ser << list << finalList << returnType << arguments << stackTop
            << stackVarTop << stackVarBottom << stackCleanVars << maxLabelIndex
            << annotations << annotationData << annotationDataSid << blockFlags
            << inFunction;
    }

    void ExprVar::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
        ser << name << variable << pBlock << argumentIndex << varFlags;
    }

    void ExprTag::serialize ( AstSerializer & ser ) {
        ser << subexpr << value << name;
        Expression::serialize(ser);
    }

    void ExprField::serialize ( AstSerializer & ser ) {
        ser << value << name << atField
            // << field // TODO: operator << const FieldDeclaration*
            << fieldIndex << annotation << underClone << derefFlags
            << fieldFlags;
        Expression::serialize(ser);
    }

    void ExprIsVariant::serialize ( AstSerializer & ser ) {
        ExprField::serialize(ser);
    }

    void ExprAsVariant::serialize ( AstSerializer & ser ) {
        ExprField::serialize(ser);
    }

    void ExprSafeAsVariant::serialize ( AstSerializer & ser ) {
        ser << skipQQ;
        ExprField::serialize(ser);
    }
    void ExprSwizzle::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
        ser << value << mask << fields << fieldFlags;
    }
    void ExprSafeField::serialize ( AstSerializer & ser ) {
        ser << skipQQ;
        ExprField::serialize(ser);
    }

    void ExprLooksLikeCall::serialize ( AstSerializer & ser ) {
        ser << name << arguments
            << argumentsFailedToInfer << aliasSubstitution << atEnclosure;
        Expression::serialize(ser);
    }

    void ExprCallMacro::serialize ( AstSerializer & ser ) {
        ser << macro;
        ExprLooksLikeCall::serialize(ser);
    }

    void ExprCallFunc::serialize ( AstSerializer & ser ) {
        ser << func << stackTop;
        ExprLooksLikeCall::serialize(ser);
    }

    void ExprOp::serialize ( AstSerializer & ser ) {
        ser << op;
        ExprCallFunc::serialize(ser);
    }

    void ExprOp1::serialize ( AstSerializer & ser ) {
        ser << subexpr;
        ExprOp::serialize(ser);
    }

    void ExprOp2::serialize ( AstSerializer & ser ) {
        ser << left << right;
        ExprOp::serialize(ser);
    }

    void ExprCopy::serialize ( AstSerializer & ser ) {
        ser << copyFlags;
        ExprOp2::serialize(ser);
    }

    void ExprMove::serialize ( AstSerializer & ser ) {
        ser << moveFlags;
        ExprOp2::serialize(ser);
    }

    void ExprClone::serialize ( AstSerializer & ser ) {
        ser << cloneSet;
        ExprOp2::serialize(ser);
    }

    void ExprSequence::serialize ( AstSerializer & ser ) {
        ExprOp2::serialize(ser);
    }

    void ExprOp3::serialize ( AstSerializer & ser ) {
        ser << subexpr << left << right;
        ExprOp::serialize(ser);
    }

    void ExprTryCatch::serialize ( AstSerializer & ser ) {
        ser << try_block << catch_block;
        Expression::serialize(ser);
    }

    void ExprReturn::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
        ser << subexpr << returnFlags << stackTop << refStackTop << returnFunc
            << block << returnType;
    }

    void ExprBreak::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
    }

    void ExprContinue::serialize ( AstSerializer & ser ) {
        Expression::serialize(ser);
    }

    void ExprConst::serialize ( AstSerializer & ser ) {
        ser << baseType << value;
        Expression::serialize(ser);
    }

    void ExprConstPtr::serialize( AstSerializer & ser ) {
       ser <<  isSmartPtr <<
         ptrType;
        ExprConstT<void *,ExprConstPtr>::serialize(ser);
    }

     void ExprConstEnumeration::serialize( AstSerializer & ser ) {
        ser << enumType << text;
        ExprConst::serialize(ser);
     }

    void ExprConstString::serialize(AstSerializer& ser) {
        ser << text;
        ExprConst::serialize(ser);
    }

    void ExprStringBuilder::serialize(AstSerializer& ser) {
        ser << elements;
        Expression::serialize(ser);
    }

    void ExprLet::serialize(AstSerializer& ser) {
        ser << variables << visibility << atInit << letFlags;
        Expression::serialize(ser);
    }

    void ExprFor::serialize(AstSerializer& ser) {
        ser << iterators << iteratorsAka << iteratorsAt << iteratorsTags
            << iteratorVariables << sources << body << visibility
            << allowIteratorOptimization << canShadow;
        Expression::serialize(ser);
    }

    void ExprUnsafe::serialize(AstSerializer& ser) {
        ser << body;
        Expression::serialize(ser);
    }

    void ExprWhile::serialize(AstSerializer& ser) {
        ser << cond << body;
        Expression::serialize(ser);
    }

    void ExprWith::serialize(AstSerializer& ser) {
        ser << with << body;
        Expression::serialize(ser);
    }

    void ExprAssume::serialize(AstSerializer& ser) {
        ser << alias << subexpr;
        Expression::serialize(ser);
    }

    // struct ExprLikeCall -- no new fields

    void ExprMakeBlock::serialize(AstSerializer & ser) {
        ser << capture << block << stackTop << mmFlags;
        Expression::serialize(ser);
    }

    void ExprMakeGenerator::serialize(AstSerializer & ser) {
        ser << iterType << capture;
        ExprLooksLikeCall::serialize(ser);
    }

    void ExprYield::serialize(AstSerializer & ser) {
        ser << subexpr << returnFlags;
        Expression::serialize(ser);
    }

    void ExprInvoke::serialize(AstSerializer & ser) {
        ser << stackTop << doesNotNeedSp << isInvokeMethod << cmresAlias;
        ExprLikeCall<ExprInvoke>::serialize(ser);
    }

    void ExprAssert::serialize(AstSerializer & ser) {
        ser << isVerify;
        ExprLikeCall<ExprAssert>::serialize(ser);
    }

    void ExprQuote::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprQuote>::serialize(ser);
    }

    void ExprStaticAssert::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprStaticAssert>::serialize(ser);
    }

    void ExprDebug::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprDebug>::serialize(ser);
    }

    void ExprMemZero::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprMemZero>::serialize(ser);
    }

    template <typename It, typename SimNodeT, bool first>
    void ExprTableKeysOrValues<It,SimNodeT,first>::serialize(AstSerializer & ser) {
        ExprLooksLikeCall::serialize(ser);
    }

    template <typename It, typename SimNodeT>
    void ExprArrayCallWithSizeOrIndex<It,SimNodeT>::serialize(AstSerializer & ser) {
        ExprLooksLikeCall::serialize(ser);
    }

    void ExprErase::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprErase>::serialize(ser);
    }

    void ExprFind::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprFind>::serialize(ser);
    }

    void ExprKeyExists::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprKeyExists>::serialize(ser);
    }

    void ExprSetInsert::serialize(AstSerializer & ser) {
        ExprLikeCall<ExprSetInsert>::serialize(ser);
    }

    void ExprTypeInfo::serialize(AstSerializer & ser) {
        ser << trait << subexpr << typeexpr << subtrait << extratrait << macro;
        Expression::serialize(ser);
    }

    void ExprIs::serialize(AstSerializer & ser) {
        ser << subexpr << typeexpr;
        Expression::serialize(ser);
    }

    void ExprAscend::serialize(AstSerializer & ser) {
        ser << subexpr << ascType << stackTop << ascendFlags;
        Expression::serialize(ser);
    }

    void ExprCast::serialize(AstSerializer & ser) {
        ser << subexpr << castType << castFlags;
        Expression::serialize(ser);
    }

    void ExprNew::serialize(AstSerializer & ser) {
        ser << typeexpr << initializer;
        ExprCallFunc::serialize(ser);
    }

    void ExprCall::serialize(AstSerializer & ser) {
        ser << doesNotNeedSp << cmresAlias;
        ExprCallFunc::serialize(ser);
    }

    void ExprIfThenElse::serialize ( AstSerializer & ser ) {
        ser << cond << if_true << if_false << ifFlags;
        Expression::serialize(ser);
    }

    void MakeFieldDecl::serialize ( AstSerializer & ser ) {
        ser << at << name << value << tag << flags;
        ptr_ref_count::serialize(ser);
    }

    void MakeStruct::serialize( AstSerializer & ser ) {
        ser << static_cast<vector<MakeFieldDeclPtr>&>(*this);
        ptr_ref_count::serialize(ser);
    }

    void ExprNamedCall::serialize ( AstSerializer & ser ) {
        ser << name << nonNamedArguments << arguments << argumentsFailedToInfer;
        Expression::serialize(ser);
    }

    void ExprMakeLocal::serialize ( AstSerializer & ser ) {
        ser << makeType << stackTop << extraOffset << makeFlags;
        Expression::serialize(ser);
    }

    void ExprMakeStruct::serialize ( AstSerializer & ser ) {
        ser << structs << block << makeStructFlags;
        ExprMakeLocal::serialize(ser);
    }

    void ExprMakeVariant::serialize ( AstSerializer & ser ) {
        ser << variants;
        ExprMakeLocal::serialize(ser);
    }

    void ExprMakeArray::serialize ( AstSerializer & ser ) {
        ser << recordType << values;
        ExprMakeLocal::serialize(ser);
    }

    void ExprMakeTuple::serialize ( AstSerializer & ser ) {
        ser << isKeyValue;
        ExprMakeArray::serialize(ser);
    }

    void ExprArrayComprehension::serialize ( AstSerializer & ser ) {
        ser << exprFor << exprWhere << subexpr << generatorSyntax;
        Expression::serialize(ser);
    }

    void ExprTypeDecl::serialize ( AstSerializer & ser ) {
        ser << typeexpr;
        Expression::serialize(ser);
    }


    void Expression::serialize ( AstSerializer & /* ser */ ) {
        // TODO: implement
    }

    void TypeInfoMacro::serialize ( AstSerializer & ser ) {
        ser << name << module;
        ptr_ref_count::serialize(ser);
    }

    void CallMacro::serialize ( AstSerializer & ser ) {
        ser << name << module;
        ptr_ref_count::serialize(ser);
    }
}

