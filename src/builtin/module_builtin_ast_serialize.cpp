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

    AstSerializer & AstSerializer::operator << ( Enumeration * & enum_type ) {
        enum_type->serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Enumeration::EnumEntry & entry ) {
        entry.serialize(*this);
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
    AstSerializer & AstSerializer::operator << ( InferHistory & history ) {
        history.serialize(*this);
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

    void Enumeration::EnumEntry::serialize( AstSerializer & ser ) {
        ser << name;
        ser << cppName;
        ser << at;
        ser << value;
    }

    void Enumeration::serialize ( AstSerializer & ser ) {
        ser << name;
        ser << cppName;
        ser << at;
        ser << list;
        ser << module;
        ser << external;
        ser << baseType;
        ser << annotations;
        ser << isPrivate;
        ptr_ref_count::serialize(ser);
    }

    void TypeAnnotation::serialize ( AstSerializer & ser ) {
        Annotation::serialize(ser);
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

    void Variable::serialize ( AstSerializer & ser ) {
        ser << name;
        ser << aka;
        ser << type;
        ser << init;
        ser << source;
        ser << at;
        ser << index;
        ser << stackTop;
        ser << extraLocalOffset;
        ser << module;
        ser << useFunctions;
        ser << useGlobalVariables;
        ser << initStackSize;
        ser << flags;
        ser << access_flags;
        ser << annotation;
        ptr_ref_count::serialize(ser);
    }

    void Function::AliasInfo::serialize ( AstSerializer & ser ) {
        ser << var;
        ser << func;
        ser << viaPointer;
    }

    void InferHistory::serialize ( AstSerializer & ser ) {
        ser << at;
        ser << func;
    }

// function

    void Function::serialize ( AstSerializer & ser ) {
        ser.tag("Function");
        ser << annotations;
        ser << name;
        ser << arguments;
        ser << result;
        ser << body;
        ser << index;
        ser << totalStackSize;
        ser << totalGenLabel;
        ser << at;
        ser << atDecl;
        ser << module;
        ser << useFunctions;
        ser << useGlobalVariables;
        ser << classParent;
        ser << resultAliases;
        ser << argumentAliases;
        ser << resultAliasesGlobals;
        ser << flags;
        ser << moreFlags;
        ser << sideEffectFlags;
        ser << inferStack;
        ser << fromGeneric;
        ser << hash;
        ser << aotHash;
    }

    void Expression::serialize ( AstSerializer & /* ser */ ) {
        // TODO: implement
    }

}
