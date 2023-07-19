#include "daScript/misc/platform.h"

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

// typedecl

    void TypeDecl::serialize ( AstSerializer & ser ) {
        ser.tag("TypeDecl");
        ser << baseType;
        ser << structType;
        // ser << enumType;
        // ser << annotation;
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

        ser << type;
        ser << name;
        ser << sValue;
        ser << iValue;
        ser << at;
    }

    void AnnotationArgumentList::serialize ( AstSerializer & ser ) {
        ser << * static_cast <AnnotationArguments *> (this);
    }

    void AnnotationDeclaration::serialize ( AstSerializer & ser ) {
        ser << annotation;
        ser << arguments;
        ser << at;
        ser << flags;
        ptr_ref_count::serialize(ser);
    }

    void Annotation::serialize ( AstSerializer & ser ) {
        ser << module;
        BasicAnnotation::serialize(ser);
    }

    void BasicAnnotation::serialize ( AstSerializer & ser ) {
        ser << name;
        ser << cppName;
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
        ser << name;
        ser << type;
        ser << init;
        ser << annotation;
        ser << at;
        ser << offset;
        ser << flags;
    }

    void Structure::serialize ( AstSerializer & ser ) {
        ser.tag("Structure");
        ser << name;
        ser << fields;
        ser << fieldLookup;
        ser << at;
        ser << module;
        ser << parent; // parent could be in the current module or in some other module
        ser << annotations;
        ser << flags;
        ptr_ref_count::serialize(ser);
    }

// function

    void Function::serialize ( AstSerializer & ser ) {
        ser.tag("Function");
        // ser << annotations;
        ser << name;
        // ser << arguments;
        ser << result;
        ser << body;
        ser << index;
        ser << totalStackSize;
        ser << totalGenLabel;
        ser << at;
        ser << atDecl;
        ser << module;
        // ser << useFunctions;

    }

/*
        AnnotationList      annotations;
        string              name;
        vector<VariablePtr> arguments;
        TypeDeclPtr         result;
        ExpressionPtr       body;
        int                 index = -1;
        uint32_t            totalStackSize = 0;
        int32_t             totalGenLabel = 0;
        LineInfo            at, atDecl;
        Module *            module = nullptr;
        das_set<Function *>     useFunctions;
        das_set<Variable *>     useGlobalVariables;
        Structure *         classParent = nullptr;
    // this is what we use for alias checking
        vector<int>         resultAliases;
        vector<vector<int>> argumentAliases;
        struct AliasInfo {
            Variable *  var = nullptr;
            Function *  func = nullptr;
            bool        viaPointer = false;
        };
        vector<AliasInfo>  resultAliasesGlobals;
    // end of what we use for alias checking
        union {
            struct {
                bool    builtIn : 1;
                bool    policyBased : 1;
                bool    callBased : 1;
                bool    interopFn : 1;
                bool    hasReturn: 1;
                bool    copyOnReturn : 1;
                bool    moveOnReturn : 1;
                bool    exports : 1;

                bool    init : 1;
                bool    addr : 1;
                bool    used : 1;
                bool    fastCall : 1;
                bool    knownSideEffects : 1;
                bool    hasToRunAtCompileTime : 1;
                bool    unsafeOperation : 1;
                bool    unsafeDeref : 1;

                bool    hasMakeBlock : 1;
                bool    aotNeedPrologue : 1;
                bool    noAot : 1;
                bool    aotHybrid : 1;
                bool    aotTemplate : 1;
                bool    generated : 1;
                bool    privateFunction : 1;
                bool    generator : 1;

                bool    lambda : 1;
                bool    firstArgReturnType : 1;
                bool    noPointerCast : 1;
                bool    isClassMethod : 1;
                bool    isTypeConstructor : 1;
                bool    shutdown : 1;
                bool    anyTemplate : 1;
                bool    macroInit : 1;
            };
            uint32_t flags = 0;
        };

        union {
            struct {
                bool    macroFunction : 1;
                bool    needStringCast : 1;
                bool    aotHashDeppendsOnArguments : 1;
                bool    lateInit : 1;
                bool    requestJit : 1;
                bool    unsafeOutsideOfFor : 1;
                bool    skipLockCheck : 1;
                bool    safeImplicit : 1;

                bool    deprecated : 1;
                bool    aliasCMRES : 1;
                bool    neverAliasCMRES : 1;
                bool    addressTaken : 1;
                bool    propertyFunction : 1;
                bool    pinvoke : 1;
            };
            uint32_t moreFlags = 0;

        };
        union {
            struct {
                bool unsafeFunction : 1;
                bool userScenario : 1;
                bool modifyExternal : 1;
                bool modifyArgument : 1;
                bool accessGlobal : 1;
                bool invoke : 1;
            };
            uint32_t    sideEffectFlags = 0;
        };
        vector<InferHistory> inferStack;
        Function * fromGeneric = nullptr;
        uint64_t hash = 0;
        uint64_t aotHash = 0;
*/

    void Expression::serialize ( AstSerializer & /* ser */ ) {
        // TODO: implement
    }

}
