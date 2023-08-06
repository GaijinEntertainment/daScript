#include "daScript/misc/platform.h"

#include "daScript/ast/ast_serialize_macro.h"
#include "daScript/ast/ast_serializer.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast.h"

namespace das {

    AstSerializer::AstSerializer ( ForReading, vector<uint8_t> && buffer_ ) : buffer(buffer_) {
        astModule = Module::require("ast");
        writing = false;
    }

    AstSerializer::AstSerializer ( void ) {
        astModule = Module::require("ast");
        writing = true;
    }

    // Mnemonic: TT*[old_addr] <- objects[old_addr]
    template <typename TT>
    void patchRefs ( vector<pair<TT**,uint64_t>> & refs, const das_hash_map<uint64_t,smart_ptr<TT>> & objects) {
        for ( auto & p : refs ) {
            auto it = objects.find(p.second);
            if ( it == objects.end() ) {
                DAS_FATAL_ERROR("ast serializer function ref not found");
            } else {
                *p.first = it->second.get();
            }
        }
        refs.clear();
    }

    void AstSerializer::patch () {
        patchRefs(functionRefs, smartFunctionMap);
        patchRefs(variableRefs, smartVariableMap);
        patchRefs(structureRefs, smartStructureMap);
        patchRefs(enumerationRefs, smartEnumerationMap);
    // finally, patch block refs (differenct container)
        for ( auto & p : blockRefs ) {
            auto it = exprBlockMap.find(p.second);
            if ( it == exprBlockMap.end() ) {
                DAS_FATAL_ERROR("ast serializer function ref not found");
            } else {
                *p.first = it->second;
            }
        }
        blockRefs.clear();
    }

    void AstSerializer::write ( const void * data, size_t size ) {
        auto oldSize = buffer.size();
        buffer.resize(oldSize + size);
        memcpy(buffer.data()+oldSize, data, size);
    }

    void AstSerializer::read ( void * data, size_t size ) {
        if ( readOffset + size > buffer.size() ) {
            DAS_FATAL_ERROR("ast serializer read overflow");
            return;
        }
        memcpy(data, buffer.data()+readOffset, size);
        readOffset += size;
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
                DAS_FATAL_ERROR("ast serializer tag '%s' mismatch", name);
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    AstSerializer & AstSerializer::operator << ( Type & baseType ) {
        tag("Type");
        serialize_enum(baseType);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExpressionPtr & expr ) {
        tag("ExpressionPtr");
        bool is_null = expr == nullptr;
        *this << is_null;
        if ( is_null ) {
            if ( !writing ) expr = nullptr;
            return *this;
        }
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

    AstSerializer & AstSerializer::operator << ( Function * & func ) {
        tag("Function pointer");
        uint64_t fid = uintptr_t(func);
        *this << fid;
        if ( !fid ) {
            if ( !writing ) func = nullptr;
            return *this;
        }
        if ( writing ) {
            bool inThisModule = func->module == thisModule;
            *this << inThisModule;
            if ( !inThisModule ) {
                *this << func->module->name;
                string mangeldName = func->getMangledName();
                *this << mangeldName;
            }
        } else {
            bool inThisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                auto it = smartFunctionMap.find(fid);
                if ( it == smartFunctionMap.end() ) {
                    func = ( Function * ) 1;
                    functionRefs.emplace_back(&func, fid);
                } else {
                    func = it->second.get();
                }
            } else {
                string moduleName, mangledName;
                *this << moduleName;
                auto funcModule = moduleLibrary->findModule(moduleName);
                DAS_VERIFYF(funcModule!=nullptr, "module '%s' is not found", moduleName.c_str());
                *this << mangledName;
                if ( auto f = funcModule->findFunction(mangledName) ) {
                    func = f.get();
                } else {
                    func = funcModule->findGeneric(mangledName).get();
                }
                DAS_VERIFYF(func!=nullptr, "function '%s' is not found", mangledName.c_str());
            }
        }
        return *this;
    }


    AstSerializer & AstSerializer::operator << ( Structure * & ptr ) {
        tag("Structure pointer");
        uint64_t fid = uintptr_t(ptr);
        *this << fid;
        if ( !fid ) {
            if ( !writing ) ptr = nullptr;
            return *this;
        }
        if ( writing ) {
            bool inThisModule = ptr->module == thisModule;
            *this << inThisModule;
            if ( !inThisModule ) {
                *this << ptr->module->name;
                *this << ptr->name;
            }
        } else {
            bool inThisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                auto it = smartStructureMap.find(fid);
                if ( it == smartStructureMap.end() ) {
                    ptr = ( Structure * ) 1;
                    structureRefs.emplace_back(&ptr, fid);
                } else {
                    ptr = it->second.get();
                }
            } else {
                string moduleName, mangledName;
                *this << moduleName;
                auto funcModule = moduleLibrary->findModule(moduleName);
                DAS_VERIFYF(funcModule!=nullptr, "module '%s' is not found", moduleName.c_str());
                *this << mangledName;
                auto f = funcModule->findStructure(mangledName);
                DAS_VERIFYF(f!=nullptr, "function '%s' is not found", mangledName.c_str());
                ptr = f.get();
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Enumeration * & ptr ) {
        tag("Enumeration pointer");
        uint64_t fid = uintptr_t(ptr);
        *this << fid;
        if ( !fid ) {
            if ( !writing ) ptr = nullptr;
            return *this;
        }
        if ( writing ) {
            bool inThisModule = ptr->module == thisModule;
            *this << inThisModule;
            if ( !inThisModule ) {
                *this << ptr->module->name;
                *this << ptr->name;
            }
        } else {
            bool inThisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                auto it = smartEnumerationMap.find(fid);
                if ( it == smartEnumerationMap.end() ) {
                    ptr = ( Enumeration * ) 1;
                    enumerationRefs.emplace_back(&ptr, fid);
                } else {
                    ptr = it->second.get();
                }
            } else {
                string moduleName, mangledName;
                *this << moduleName;
                auto funcModule = moduleLibrary->findModule(moduleName);
                DAS_VERIFYF(funcModule!=nullptr, "module '%s' is not found", moduleName.c_str());
                *this << mangledName;
                auto f = funcModule->findEnum(mangledName);
                DAS_VERIFYF(f!=nullptr, "function '%s' is not found", mangledName.c_str());
                ptr = f.get();
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Variable * & ptr ) {
        tag("Enumeration pointer");
        uint64_t fid = uintptr_t(ptr);
        *this << fid;
        if ( !fid ) {
            if ( !writing ) ptr = nullptr;
            return *this;
        }
        if ( writing ) {
            bool inThisModule = ptr->module == thisModule || ptr->module == nullptr; // local var
            *this << inThisModule;
            if ( !inThisModule ) {
                *this << ptr->module->name;
                *this << ptr->name;
            }
        } else {
            bool inThisModule;
            *this << inThisModule;
            if ( inThisModule ) {
                auto it = smartVariableMap.find(fid);
                if ( it == smartVariableMap.end() ) {
                    ptr = ( Variable * ) 1;
                    variableRefs.emplace_back(&ptr, fid);
                } else {
                    ptr = it->second.get();
                }
            } else {
                string moduleName, mangledName;
                *this << moduleName;
                auto funcModule = moduleLibrary->findModule(moduleName);
                DAS_VERIFYF(funcModule!=nullptr, "module '%s' is not found", moduleName.c_str());
                *this << mangledName;
                auto f = funcModule->findVariable(mangledName);
                DAS_VERIFYF(f!=nullptr, "function '%s' is not found", mangledName.c_str());
                ptr = f.get();
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FunctionPtr & func ) {
        tag("FunctionPtr");
        if ( writing ) {
            DAS_ASSERTF(!func->builtIn, "cannot serialize built-in function");
        }
        serializeSmartPtr(func, smartFunctionMap);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeInfoMacroPtr & ptr ) {
        tag("TypeInfoMacroPtr");
        uint64_t id = uintptr_t(ptr);
        *this << id;
        if ( writing ) {
            if ( id ) {
                bool inThisModule = ptr->module == thisModule;
                *this << inThisModule;
                if ( !inThisModule ) {
                    *this << ptr->module->name;
                    *this << ptr->name;
                } else {
                    DAS_ASSERTF(false, "did not expect to find typeinfo macro from the current module");
                }
            }
        } else {
            if ( id ) {
                bool inThisModule;
                *this << inThisModule;
                if ( inThisModule ) {
                    DAS_ASSERTF(false, "Unreachable");
                } else {
                    string moduleName, name;
                    *this << moduleName;
                    auto funcModule = moduleLibrary->findModule(moduleName);
                    DAS_VERIFYF(funcModule!=nullptr, "module '%s' not found", moduleName.c_str());
                    *this << name;
                    ptr = funcModule->findTypeInfoMacro(name).get();
                    DAS_VERIFYF(ptr!=nullptr, "typeinfo macro '%s' not found", name.c_str());
                }
            } else {
                ptr = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeInfoMacro * & ptr ) {
        tag("TypeInfoMacroPtr");
        uint64_t id = uintptr_t(ptr);
        *this << id;
        if ( writing ) {
            if ( id ) {
                bool inThisModule = ptr->module == thisModule;
                *this << inThisModule;
                if ( !inThisModule ) {
                    *this << ptr->module->name;
                    *this << ptr->name;
                } else {
                    DAS_ASSERTF(false, "did not expect to find typeinfo macro from the current module");
                }
            }
        } else {
            if ( id ) {
                bool inThisModule;
                *this << inThisModule;
                if ( inThisModule ) {
                    DAS_ASSERTF(false, "Unreachable");
                } else {
                    string moduleName, name;
                    *this << moduleName;
                    auto funcModule = moduleLibrary->findModule(moduleName);
                    DAS_VERIFYF(funcModule!=nullptr, "module '%s' not found", moduleName.c_str());
                    *this << name;
                    ptr = funcModule->findTypeInfoMacro(name).get();
                    DAS_VERIFYF(ptr!=nullptr, "typeinfo macro '%s' not found", name.c_str());
                }
            } else {
                ptr = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeDeclPtr & type ) {
        tag("TypeDeclPtr");
        uint64_t id = intptr_t(type.get());
        *this << id;
        if ( writing ) {
            if ( id ) {
                type->serialize(*this);
            }
        } else {
            if ( id ) {
                type = make_smart<TypeDecl>();
                type->serialize(*this);
            } else {
                type = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( AnnotationArgument & arg ) {
        tag("AnnotationArgument");
        arg.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( AnnotationDeclarationPtr & annotation_decl ) {
        tag("AnnotationDeclarationPtr");
        if ( !writing ) annotation_decl = make_smart<AnnotationDeclaration>();
        annotation_decl->serialize(*this);
        return *this;
    }

    void serializeAnnotationPointer( AstSerializer & ser, Annotation * & anno ) {
        uint64_t fid = uint64_t(anno);
        ser << fid;
        if ( ser.writing ) {
            if ( fid ) {
                bool inThisModule = anno->module == ser.thisModule;
                ser << inThisModule;
                if ( !inThisModule ) {
                    ser << anno->module->name;
                    ser << anno->name;
                } else {
                    // If the macro is from current module, do nothing
                    // it will probably take care of itself during compilation
                    DAS_ASSERTF( anno->module->macroContext,
                        "expected to see macro module '%s'", anno->module->name.c_str()
                    );
                }
            }
        } else {
            if ( fid ) {
                bool inThisModule;
                ser << inThisModule;
                if ( !inThisModule ) {
                    string moduleName, name;
                    ser << moduleName;
                    auto mod = ser.moduleLibrary->findModule(moduleName);
                    DAS_VERIFYF(mod!=nullptr, "module '%s' is not found", moduleName.c_str());
                    ser << name;
                    anno = mod->findAnnotation(name).get();
                    DAS_VERIFYF(anno!=nullptr, "annotation '%s' is not found", name.c_str());
                }
            } else {
                anno = nullptr;
            }
        }
    }

    AstSerializer & AstSerializer::operator << ( AnnotationPtr & anno ) {
        tag("AnnotationPtr");
        Annotation * a = anno.get();
        serializeAnnotationPointer(*this, a);
        anno = a;
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Structure::FieldDeclaration & field_declaration ) {
        field_declaration.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( string & str ) {
        tag("string");
        if ( writing ) {
            uint32_t size = (uint32_t) str.size();
            *this << size;
            write((void *)str.data(), size);
        } else {
            uint32_t size = 0;
            *this << size;
            str.resize(size);
            read(&str[0], size);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( const char * & value ) {
        tag("const char *");
        bool is_null = value == nullptr;
        *this << is_null;
        if ( writing ) {
            if ( !is_null ) {
                uint64_t len = strlen(value);
                *this << len;
                write(static_cast<const void*>(value), len);
            }
        } else {
            if ( !is_null ) {
                uint64_t len = 0;
                *this << len;
                auto data = new char [len + 1]();
                read(static_cast<void*>(data), len);
                data[len] = '\0';
                value = data;
            } else {
                value = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( LineInfo & at ) {
        tag("LineInfo");
        *this << at.fileInfo << at.column << at.line
              << at.last_column << at.last_line;
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FileInfo * & info ) {
        FileInfoPtr t;
        if ( writing ) {
            t.reset(info);
            *this << t;
            t.release();
        } else {
            *this << t;
            info = t.release();
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( FileInfoPtr & info ) {
        // TODO: talk about it; FileInfo points to somewhere else
        return *this;

        bool is_null = info == nullptr;
        *this << is_null;
        if ( writing ) {
            if ( !is_null ) {
                info->serialize(*this);
            }
        } else {
            if ( !is_null ) {
                int tag = 0; *this << tag;
                switch ( tag ) {
                    case 0: info.reset(new FileInfo); break;
                    case 1: info.reset(new TextFileInfo); break;
                    default: DAS_VERIFYF(false, "Unreachable");
                }
                info->serialize(*this);
            } else {
                info = nullptr;
            }
        }
        return *this;
    }


    AstSerializer & AstSerializer::operator << ( StructurePtr & struct_ ) {
        serializeSmartPtr(struct_, smartStructureMap);
        return *this;
    }

    void FileAccess::serialize ( AstSerializer & ser ) {
        if ( ser.writing ) {
            int tag = 0;
            ser << tag;
        }
        ser << files;
    }

    void ModuleFileAccess::serialize ( AstSerializer & ser ) {
        if ( ser.writing ) {
            int tag = 1;
            ser << tag;
        }
        ser << files;
    }

    AstSerializer & AstSerializer::operator << ( FileAccessPtr & ptr ) {
        tag("FileAccessPtr");
        bool is_null = ptr == nullptr;
        *this << is_null;
        if ( is_null ) {
            return *this;
        }
        if ( !writing ) {
            int tag; *this << tag;
            switch ( tag ) {
                case 0: ptr = new FileAccess; break;
                case 1: ptr = new ModuleFileAccess; break;
                default: DAS_VERIFYF(false, "Unreachable");
            }
        }
        ptr->serialize(*this);
        return *this;
    }

    // This method creates concrete (i.e. non-polymorphic types without duplications)
    template<typename T>
    void AstSerializer::serializeSmartPtr( smart_ptr<T> & obj, das_hash_map<uint64_t, smart_ptr<T>> & objMap) {
        uint64_t id = uint64_t(obj.get());
        *this << id;
        if ( id == 0 ) {
            if ( !writing ) obj = nullptr;
            return;
        }
        if ( writing ) {
            if ( objMap.find(id) == objMap.end() ) {
                objMap[id] = obj;
                obj->serialize(*this);
            }
        } else {
            auto it = objMap.find(id);
            if ( it == objMap.end() ) {
                obj = make_smart<T>();
                objMap[id] = obj;
                obj->serialize(*this);
            } else {
                obj = it->second;
            }
        }
    }

    AstSerializer & AstSerializer::operator << ( EnumerationPtr & enum_type ) {
        serializeSmartPtr(enum_type, smartEnumerationMap);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Enumeration::EnumEntry & entry ) {
        entry.serialize(*this);
        return *this;
    }

    // Note for review: this is the usual downward serialization, no need to backpatch
    AstSerializer & AstSerializer::operator << ( TypeAnnotationPtr & type_anno ) {
        AnnotationPtr a = static_pointer_cast<Annotation>(type_anno);
        *this << a;
        type_anno = static_pointer_cast<TypeAnnotation>(a);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( TypeAnnotation * & type_anno ) {
        TypeAnnotationPtr t = type_anno;
        *this << t;
        type_anno = t.orphan();
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( VariablePtr & var ) {
        serializeSmartPtr(var, smartVariableMap);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Module * & module ) {
        bool is_null = module == nullptr;
        *this << is_null;
        if ( writing ) {
            if ( !is_null ) {
                *this << module->name;
            }
        } else {
            if ( !is_null ) {
                string name; *this << name;
                module = moduleLibrary->findModule(name);
            } else {
                module = nullptr;
            }
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Function::AliasInfo & alias_info ) {
        alias_info.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ReaderMacroPtr & ptr ) {
        tag("ReaderMacroPtr");
        if ( writing ) {
            DAS_ASSERTF(ptr, "did not expext to see null ReaderMacroPtr");
            bool inThisModule = ptr->module == thisModule;
            DAS_ASSERTF(!inThisModule, "did not expect to find macro from the current module");
            *this << ptr->module->name;
            *this << ptr->name;
        } else {
            string moduleName, name;
            *this << moduleName << name;
            auto mod = moduleLibrary->findModule(moduleName);
            DAS_VERIFYF(mod!=nullptr, "module '%s' not found", moduleName.c_str());
            ptr = mod->findReaderMacro(name);
            DAS_VERIFYF(ptr, "Reader macro '%s' not found in the module '%s'",
                name.c_str(), mod->name.c_str()
            );
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( ExprBlock * & block ) {
        tag("ExprBlock*");
        void * addr = block;
        *this << addr;
        if ( !writing && addr ) {
            block = ( ExprBlock * ) 1;
            blockRefs.emplace_back(&block, (uint64_t) addr);
        }
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( InferHistory & history ) {
        tag("InferHistory");
        history.serialize(*this);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( CaptureEntry & entry ) {
        *this << entry.name;
        serialize_enum<CaptureMode>(entry.mode);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( MakeFieldDeclPtr & ptr ) {
        serializeSmartPtr(ptr, smartMakeFieldDeclMap);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( MakeStructPtr & ptr ) {
        serializeSmartPtr(ptr, smartMakeStructMap);
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( Module & module ) {
        thisModule = &module;
        module.serialize(*this);
        return *this;
    }

// typedecl

    void TypeDecl::serialize ( AstSerializer & ser ) {
        ser.tag("TypeDecl");
        ser << baseType;
        ser << structType << enumType;
        ser << firstType  << secondType;
        ser << annotation;
        ser << argTypes   << argNames;
        ser << dim        << dimExpr;
        ser << flags      << alias;
        ser << at         << module;
    }

    void AnnotationArgument::serialize ( AstSerializer & ser ) {
        ser.tag("AnnotationArgument");
        ser << type << name << sValue << iValue << at;
    }

    void AnnotationArgumentList::serialize ( AstSerializer & ser ) {
        ser.tag("AnnotationArgumentList");
        ser << * static_cast <AnnotationArguments *> (this);
    }

    void AnnotationDeclaration::serialize ( AstSerializer & ser ) {
        ser.tag("AnnotationDeclaration");
        ser << annotation << arguments << at << flags;
        ptr_ref_count::serialize(ser);
    }

    void ptr_ref_count::serialize ( AstSerializer & ser ) {
        ser.tag("ptr_ref_count");
        // Do nothing
    }

    void Structure::FieldDeclaration::serialize ( AstSerializer & ser ) {
        ser.tag("FieldDeclaration");
        ser << name << at;
        ser << type;
        ser << init << annotation << offset << flags;
    }

    void Enumeration::EnumEntry::serialize( AstSerializer & ser ) {
        ser.tag("EnumEntry");
        ser << name << cppName << at << value;
    }

    void serializeAnnotationList ( AstSerializer & ser, AnnotationList & list ) {
        if ( ser.writing ) {
            uint64_t size = 0;
        // count the real size without generated annotations
            for ( auto & it : list ) {
                bool inThisModule = it->annotation->module == ser.thisModule;
                if ( !inThisModule ) { size += 1; }
            }
            ser << size;
            for ( auto & it : list ) {
                bool inThisModule = it->annotation->module == ser.thisModule;
                if ( !inThisModule ) { ser << it; }
            }
        } else {
            uint64_t size = 0; ser << size;
            AnnotationList result; result.resize(size);
            for ( uint64_t i = 0; i < size; i++ ) {
                ser << result[i];
            }
        }
    }

    void Enumeration::serialize ( AstSerializer & ser ) {
        ser.tag("Enumeration");
        ser << name     << cppName  << at << list << module
            << external << baseType << isPrivate;
        serializeAnnotationList(ser, annotations);
        ptr_ref_count::serialize(ser);
    }

    void Structure::serialize ( AstSerializer & ser ) {
        ser.tag("Structure");
        ser << name;
        ser << fields;
        ser << fieldLookup << at << module
            << parent // parent could be in the current module or in some other
                      // module
            << flags;
        serializeAnnotationList(ser, annotations);
        ptr_ref_count::serialize(ser);
    }

    void Variable::serialize ( AstSerializer & ser ) {
        ser.tag("Variable");
        ser << name << aka << type << init << source << at << index << stackTop
            << extraLocalOffset << module
            << initStackSize << flags << access_flags << annotation;
        ptr_ref_count::serialize(ser);
    }

    void Function::AliasInfo::serialize ( AstSerializer & ser ) {
        ser.tag("AliasInfo");
        ser << var << func << viaPointer;
    }

    void InferHistory::serialize ( AstSerializer & ser ) {
        ser.tag("InferHistory");
        ser << at << func;
    }

// function

    void Function::serialize ( AstSerializer & ser ) {
        ser.tag("Function");
        ser << name ;
    // Note: importatnt fields are placed separately for easier debugging
        serializeAnnotationList(ser, annotations);
        ser << arguments;
        ser << result;
        ser << body;
        ser << classParent;
        ser << fromGeneric;
        ser << index         << totalStackSize  << totalGenLabel;
        ser << at            << atDecl          << module;
        ser << inferStack    << hash            << aotHash;
        ser << resultAliases << argumentAliases << resultAliasesGlobals;
        ser << flags         << moreFlags       << sideEffectFlags;
    }

// Expressions

    void ExprReader::serialize ( AstSerializer & ser ) {
        ser.tag("ExprReader");
        ser << macro << sequence;
        Expression::serialize(ser);
    }

    void ExprLabel::serialize ( AstSerializer & ser ) {
        ser.tag("ExprLabel");
        ser << label << comment;
        Expression::serialize(ser);
    }

    void ExprGoto::serialize ( AstSerializer & ser ) {
        ser.tag("ExprGoto");
        ser << label << subexpr;
        Expression::serialize(ser);
    }

    void ExprRef2Value::serialize ( AstSerializer & ser ) {
        ser.tag("ExprRef2Value");
        ser << subexpr;
        Expression::serialize(ser);
    }

    void ExprRef2Ptr::serialize ( AstSerializer & ser ) {
        ser.tag("ExprRef2Ptr");
        ser << subexpr;
        Expression::serialize(ser);
    }

    void ExprPtr2Ref::serialize ( AstSerializer & ser ) {
        ser.tag("ExprPtr2Ref");
        ser << subexpr << unsafeDeref;
        Expression::serialize(ser);
    }

    void ExprAddr::serialize ( AstSerializer & ser ) {
        ser.tag("ExprAddr");
        ser << target << funcType << func;
        Expression::serialize(ser);
    }

    void ExprNullCoalescing::serialize ( AstSerializer & ser ) {
        ser.tag("ExprNullCoalescing");
        ser << defaultValue;
        ExprPtr2Ref::serialize(ser);
    }

    void ExprDelete::serialize ( AstSerializer & ser ) {
        ser.tag("ExprDelete");
        ser << subexpr << sizeexpr << native;
        Expression::serialize(ser);
    }

    void ExprAt::serialize ( AstSerializer & ser ) {
        ser.tag("ExprAt");
        ser << subexpr << index;
        ser << atFlags;
        Expression::serialize(ser);
    }

    void ExprSafeAt::serialize ( AstSerializer & ser ) {
        ser.tag("ExprSafeAt");
        ExprAt::serialize(ser);
    }

    void ExprBlock::serialize ( AstSerializer & ser ) {
        ser.tag("ExprBlock");
        Expression::serialize(ser);

        if ( ser.writing ) {
            void * thisBlock = this;
            ser << thisBlock;
        } else {
            void * thisBlock; ser << thisBlock;
            ser.exprBlockMap.emplace((uint64_t) thisBlock, this);
        }

        ser << list << finalList << returnType << arguments << stackTop
            << stackVarTop << stackVarBottom << stackCleanVars << maxLabelIndex
            << annotationData << annotationDataSid << blockFlags
            << inFunction;

        serializeAnnotationList(ser, annotations);
    }

    void ExprVar::serialize ( AstSerializer & ser ) {
        ser.tag("ExprVar");
        Expression::serialize(ser);

        ser << name << argumentIndex << varFlags;
        ser << pBlock;

        // The variable is smart_ptr but we actually need
        // non-owning semantics
        if ( ser.writing ) {
            bool inThisModule =  variable == nullptr // this happens with [generic] functions, for example
                      || variable->module == nullptr
                      || variable->module == ser.thisModule;
            ser << inThisModule;
            if ( inThisModule ) {
                ser << variable; // serialize as smart pointer
            } else {
                ser << variable->name;
                ser << variable->module->name;
            }
        } else {

            bool inThisModule; ser << inThisModule;
            if ( inThisModule ) {
                ser << variable;
            } else {
                string varname, modname;
                ser << varname << modname;
                auto mod = ser.moduleLibrary->findModule(modname);
                DAS_VERIFYF(mod, "expected to find module '%s'", modname.c_str());
                variable = mod->findVariable(name);
            }

        }
    }

    void ExprTag::serialize ( AstSerializer & ser ) {
        ser.tag("ExprTag");
        ser << subexpr << value << name;
        Expression::serialize(ser);
    }

    void ExprField::serialize ( AstSerializer & ser ) {
        ser.tag("ExprField");
        Expression::serialize(ser);
        ser << value << name << atField
            // Note: `field` is const, save it for later backpatching
            << fieldIndex << annotation << derefFlags
            // Note: underClone is only used during infer and we don't need it
            << fieldFlags;

        bool is_null = value->type == nullptr;
        ser << is_null;
        if ( is_null ) return; // Sometimes generic functions contain

        bool is_weak = value->type->isTuple() || value->type->isVariant() || value->type->isBitfield();
        ser << is_weak;
        if ( is_weak ) return; // They don't have field `field` set

        bool is_handle = value->type->isHandle() || value->type->isPointerToAnnotation();
        ser << is_handle;
        if ( is_handle ) return; // Likewise

        if ( ser.writing ) {
            string mangledName;
            if ( value->type->isPointer() ) {
                DAS_VERIFYF(value->type->firstType->isStructure(), "expected to see structure field access via pointer");
                mangledName = value->type->firstType->structType->getMangledName();
            } else {
                DAS_VERIFYF(value->type->isStructure(), "expected to see structure field access");
                mangledName = value->type->structType->getMangledName();
            }
            ser << mangledName;
        } else {
            string mangledName;
            ser << mangledName;
            auto struct_ = ser.moduleLibrary->findStructure(mangledName, ser.thisModule);
            if ( struct_.size() == 0 ) {
                DAS_ASSERTF(false, "expected to find structure '%s'", mangledName.c_str());
            } else if ( struct_.size() > 1 ) {
                DAS_ASSERTF(false, "too many candidates for structure '%s'", mangledName.c_str());
            }
        // set the missing field field
            field = struct_.front()->findField(name);
        }
    }

    void ExprSafeAsVariant::serialize ( AstSerializer & ser ) {
        ser << skipQQ;
        ExprField::serialize(ser);
    }

    void ExprSwizzle::serialize ( AstSerializer & ser ) {
        ser << value << mask << fields << fieldFlags;
        Expression::serialize(ser);
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
        ser << subexpr    << returnFlags << stackTop << refStackTop
            << returnFunc << block       << returnType;
    }

    void ExprConst::serialize ( AstSerializer & ser ) {
        ser << baseType << value;
        Expression::serialize(ser);
    }

    void ExprConstPtr::serialize( AstSerializer & ser ) {
        ser << isSmartPtr << ptrType;
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

    template <typename It, typename SimNodeT, bool first>
    void ExprTableKeysOrValues<It,SimNodeT,first>::serialize(AstSerializer & ser) {
        ExprLooksLikeCall::serialize(ser);
    }

    template <typename It, typename SimNodeT>
    void ExprArrayCallWithSizeOrIndex<It,SimNodeT>::serialize(AstSerializer & ser) {
        ExprLooksLikeCall::serialize(ser);
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
        ser << static_cast <vector<MakeFieldDeclPtr> &> ( *this );
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

    void Expression::serialize ( AstSerializer & ser ) {
        ser << at
            << type
            << __rtti
            << genFlags
            << flags
            << printFlags;
        ptr_ref_count::serialize(ser);
    }

    void FileInfo::serialize ( AstSerializer & ser ) {
        ser.tag("FileInfo");
        int tag = 0;
        ser << tag << name << tabSize;
        // Note: we do not serialize profileData
    }

    void TextFileInfo::serialize ( AstSerializer & ser ) {
        ser.tag("TextFileInfo");
        int tag = 1; // Signify the text file info
        ser << tag    << name        << tabSize;
        ser << source << sourceLength << owner;
    }

    AstSerializer & AstSerializer::operator << ( CallMacro * & ptr ) {
        tag("CallMacro *");
        if ( writing ) {
            DAS_ASSERTF ( ptr, "did not expect to see a nullptr CallMacro *" );
            bool inThisModule = ptr->module == thisModule;
            DAS_ASSERTF ( !inThisModule, "did not expect to find macro from the current module" );
            *this << ptr->module->name;
            *this << ptr->name;
        } else {
            string moduleName, name;
            *this << moduleName << name;
            auto mod = moduleLibrary->findModule(moduleName);
            DAS_VERIFYF(mod!=nullptr, "module '%s' not found", moduleName.c_str());
        // perform a litte dance to access the internal macro;
        // for details see: src/builtin/module_builtin_ast_adapters.cpp
        // 1564: void addModuleCallMacro ( .... CallMacroPtr & .... )
            auto callFactory = mod->findCall(name);
            DAS_VERIFYF(
                callFactory, "could not find CallMacro '%s' in the module '%s'",
                name.c_str(), mod->name.c_str()
            );
            auto exprLooksLikeCall = (*callFactory)({});
            DAS_ASSERTF(
                strncmp("ExprCallMacro", exprLooksLikeCall->__rtti, 14) == 0,
                "excepted to see an ExprCallMacro"
            );
            ptr = static_cast<ExprCallMacro *>(exprLooksLikeCall)->macro;
        }
        return *this;
    }

    // Restores the internal state of macro module
    Module * reinstantiateMacroModuleState ( ModuleLibrary & lib, Module * this_mod ) {
        TextWriter ignore_logs;
        ReuseCacheGuard rcg;
    // initialize program
        auto program = make_smart<Program>();
        program->promoteToBuiltin = false;
        program->isDependency = true;
        program->thisModuleGroup = nullptr;
        program->thisModuleName.clear();
        program->library.reset();
        program->thisModule.release();
        program->thisModule.reset(this_mod);
        lib.foreach([&] ( Module * pm ) {
            program->library.addModule(pm);
            return true;
        },"*");
    // set the current module
    // create the module macro state
        program->isCompiling = false;
        program->markMacroSymbolUse();
        program->allocateStack(ignore_logs);
        program->makeMacroModule(ignore_logs);
    // unbind the module from the program
        return program->thisModule.release();
    }

    void serializeUseFunctions ( AstSerializer & ser, const FunctionPtr & f ) {
        ser.tag("serializeUseFunctions");
        if ( ser.writing ) {
            string name = f->name; ser << name;
            uint64_t sz = f->useFunctions.size();
            ser << sz;
            for ( auto & usedFun : f->useFunctions ) {
                void * addr = usedFun;
                ser << addr;
            }
        } else {
            string name; ser << name;
            DAS_ASSERTF(name == f->name, "expected to serialize in the same order");
            uint64_t size = 0; ser << size;
            f->useFunctions.reserve(size);
            for ( uint64_t i = 0; i < size; i++ ) {
                void* addr; ser << addr;
                auto fun = ser.smartFunctionMap.at((uint64_t) addr);
                f->useFunctions.emplace(fun.get());
            }
        }
    }

    void serializeUseFunctions ( AstSerializer & ser, const VariablePtr & f ) {
        ser.tag("serializeUseFunctions");
        if ( ser.writing ) {
            string name = f->name; ser << name;
            uint64_t sz = f->useFunctions.size();
            ser << sz;
            for ( auto & usedFun : f->useFunctions ) {
                void * addr = usedFun;
                ser << addr;
            }
        } else {
            string name; ser << name;
            DAS_ASSERTF(name == f->name, "expected to serialize in the same order");
            uint64_t size = 0; ser << size;
            f->useFunctions.reserve(size);
            for ( uint64_t i = 0; i < size; i++ ) {
                void* addr; ser << addr;
                auto fun = ser.smartFunctionMap.at((uint64_t) addr);
                f->useFunctions.emplace(fun.get());
            }
        }
    }

    void serializeUseVariables ( AstSerializer & ser, const FunctionPtr & f ) {
        ser.tag("serializeUseVariables");
        if ( ser.writing ) {
            string name = f->name; ser << name;
            uint64_t sz = f->useGlobalVariables.size();
            ser << sz;
            for ( auto & use : f->useGlobalVariables ) {
                void * addr = use;
                ser << use->name;
                ser << addr;
                ser << use;
            }
        } else {
            string name; ser << name;
            DAS_ASSERTF(name == f->name, "expected to serialize in the same order");
            uint64_t size = 0; ser << size;
            f->useGlobalVariables.reserve(size);
            for ( uint64_t i = 0; i < size; i++ ) {
                string varname; ser << varname;
                void * addr; ser << addr;
                Variable * fun; ser << fun;
                f->useGlobalVariables.emplace(fun);
            }
        }
    }

    void serializeUseVariables ( AstSerializer & ser, const VariablePtr & f ) {
        if ( ser.writing ) {
            string name = f->name; ser << name;
            uint64_t sz = f->useGlobalVariables.size();
            ser << sz;
            for ( auto & use : f->useGlobalVariables ) {
                void * addr = use;
                ser << addr;
            }
        } else {
            string name; ser << name;
            DAS_ASSERTF(name == f->name, "expected to serialize in the same order");
            uint64_t size = 0; ser << size;
            f->useGlobalVariables.reserve(size);
            for ( uint64_t i = 0; i < size; i++ ) {
                void* addr; ser << addr;
                auto fun = ser.smartVariableMap.at((uint64_t) addr);
                f->useGlobalVariables.emplace(fun.get());
            }
        }
    }

    void Module::serialize ( AstSerializer & ser ) {
        ser << name           << moduleFlags;
        ser << aliasTypes     << structures << enumerations << globals;
        ser << functions      << functionsByName;
        ser << generics       << genericsByName;
        ser << annotationData << requireModule;
        ser << ownFileInfo    << promotedAccess;

        functions.foreach_with_hash ([&](smart_ptr<Function> f, uint64_t hash) {
            if ( ser.writing ) {
                ser << hash;
            } else {
                uint64_t h; ser << h;
                DAS_VERIFYF(h == hash, "expected to walk in the same order");
            }
            serializeUseVariables(ser, f);
            serializeUseFunctions(ser, f);
        });

        generics.foreach_with_hash ([&](smart_ptr<Function> f, uint64_t hash) {
            if ( ser.writing ) {
                ser << hash;
            } else {
                uint64_t h; ser << h;
                DAS_VERIFYF(h == hash, "expected to walk in the same order");
            }
            serializeUseVariables(ser, f);
            serializeUseFunctions(ser, f);
        });

        globals.foreach_with_hash ([&](smart_ptr<Variable> g, uint64_t hash) {
            if ( ser.writing ) {
                ser << hash;
            } else {
                uint64_t h; ser << h;
                DAS_VERIFYF(h == hash, "expected to walk in the same order");
            }
            serializeUseVariables(ser, g);
            serializeUseFunctions(ser, g);
        });

        ser.patch();

        // Now we need to restore the internal state in case this has been a macro module

        if ( ser.writing ) {
            bool is_macro_module = macroContext; // it's a macro module if it has macroContext
            ser << is_macro_module;
        } else {
            bool is_macro_module = false;
            ser << is_macro_module;
            if ( is_macro_module ) {
                reinstantiateMacroModuleState (*ser.moduleLibrary, this);
            }
        }

    }

    class TopSort {
    public:
        TopSort(const vector<Module*> & inputModules) : input(inputModules) {
            for (auto mod : input) {
                visited[mod] = NOT_SEEN;
            }
        }

        vector<Module*> getDependecyOrdered() {
            for ( auto mod : input ) {
                visit(mod);
            }
            return std::move(sorted);
        }

        void visit( Module * mod ) {
            if ( visited[mod] != NOT_SEEN ) return;
            visited[mod] = IN_PROGRESS;
            for ( auto [module, required] : mod->requireModule ) {
                if ( module != mod ) {
                    visit(module);
                }
            }
            visited[mod] = FINISHED;
            sorted.push_back(mod);
        }

    private:
        enum WalkStatus { NOT_SEEN, IN_PROGRESS, FINISHED };
        vector<Module*> sorted;
        const vector<Module*> & input;
        das_hash_map<Module*, WalkStatus> visited;
    };


    AstSerializer & AstSerializer::operator << ( CodeOfPolicies & value ) {
        *this << value.aot
              << value.aot_module
              << value.completion
              << value.export_all
              << value.always_report_candidates_threshold
              << value.stack
              << value.intern_strings
              << value.persistent_heap
              << value.multiple_contexts
              << value.heap_size_hint
              << value.string_heap_size_hint
              << value.solid_context
              << value.macro_context_persistent_heap
              << value.macro_context_collect
              << value.rtti
              << value.no_unsafe
              << value.local_ref_is_unsafe
              << value.no_global_variables
              << value.no_global_variables_at_all
              << value.no_global_heap
              << value.only_fast_aot
              << value.aot_order_side_effects
              << value.no_unused_function_arguments
              << value.no_unused_block_arguments
              << value.smart_pointer_by_value_unsafe
              << value.allow_block_variable_shadowing
              << value.allow_local_variable_shadowing
              << value.allow_shared_lambda
              << value.ignore_shared_modules
              << value.default_module_public
              << value.no_deprecated
              << value.no_aliasing
              << value.strict_smart_pointers
              << value.no_init
              << value.strict_unsafe_delete
              << value.no_optimizations
              << value.fail_on_no_aot
              << value.fail_on_lack_of_aot_export
              << value.debugger
              << value.debug_module
              << value.profiler
              << value.profile_module
              << value.jit
              << value.threadlock_context;
        return *this;
    }

    AstSerializer & AstSerializer::operator << ( tuple<Module *, string, string, bool, LineInfo> & value ) {
        *this << get<0>(value) << get<1>(value) << get<2>(value) << get<3>(value) << get<4>(value);
        return *this;
    }

    void Program::serialize ( AstSerializer & ser ) {
        ser << thisNamespace << thisModuleName;

        ser << totalFunctions << totalVariables << newLambdaIndex;
    // don't serialize errors
        ser << globalInitStackSize << globalStringHeapSize;
        ser << flags;

        ser << options << policies;

    // serialize library
        if ( ser.writing ) {
            ser.moduleLibrary = &library;
            uint64_t size = library.modules.size();
            ser << size;

            TopSort ts(library.modules);
            auto modules = ts.getDependecyOrdered();

            for ( auto & m : modules ) {
                bool builtin = m->builtIn && !m->promoted;
                ser << builtin;
                ser << m->name;
                if ( !builtin )
                    ser << *m; // Serialize the whole module
            }
        } else {
            library.reset();
            thisModule.release();
            ser.moduleLibrary = &library;

            uint64_t size = 0; ser << size;
            for ( uint64_t i = 0; i < size; i++ ) {
                bool builtin; string name;
                ser << builtin << name;
                if ( name == "$" )
                    library.addBuiltInModule();
                else if ( builtin ) {
                    Module * m = Module::require(name);
                    library.addModule(m);
                } else {
                    auto deser = new Module;
                    library.addModule(deser);
                    ser << *deser;
                }
            }
            thisModule.reset(library.modules.back());
        }

        ser << allRequireDecl;

    // for the last module, mark symbols manually
        markExecutableSymbolUse();
        removeUnusedSymbols();
        TextWriter logs;
        allocateStack(logs);
    }

}
