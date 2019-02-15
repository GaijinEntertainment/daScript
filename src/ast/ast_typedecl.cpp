#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das
{
    // auto or generic type conversion

    void TypeDecl::applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT ) {
        if ( !autoT->isAuto() ) return;
        TT->ref = (TT->ref | autoT->ref) && !autoT->removeRef;
        TT->constant = (TT->constant | autoT->constant) && !autoT->removeConstant;
        if ( autoT->isPointer() ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tArray ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tTable ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
            applyAutoContracts(TT->secondType, autoT->secondType);
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction ) {
            if ( TT->firstType ) {
                applyAutoContracts(TT->firstType, autoT->firstType);
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                applyAutoContracts(TT->argTypes[i], autoT->argTypes[i]);
            }
        }
    }

    TypeDeclPtr TypeDecl::inferAutoType ( TypeDeclPtr autoT, TypeDeclPtr initT ) {
        // can't infer from the type, which is already 'auto'
        if ( initT->isAuto() )
            return nullptr;
        // if its not an auto type, return as is
        if ( !autoT->isAuto() ) {
            if ( autoT->isSameType(*initT) ) {
                return make_shared<TypeDecl>(*autoT);
            } else {
                return nullptr;
            }
        }
        // auto & can't be infered from non-ref
        if ( autoT->ref && !initT->ref )
            return nullptr;
        // auto[][][] can't be infered from non-array
        if ( autoT->dim.size() && autoT->dim!=initT->dim )
            return nullptr;
        // auto? can't be infered from non-pointer
        if ( autoT->isPointer() && (!initT->isPointer() || !initT->firstType) )
            return nullptr;
        // array has to match array
        if ( autoT->baseType==Type::tArray && (initT->baseType!=Type::tArray || !initT->firstType) )
            return nullptr;
        // table has to match table
        if ( autoT->baseType==Type::tTable && (initT->baseType!=Type::tTable || !initT->firstType || !initT->secondType) )
            return nullptr;
        // block has to match block
        if ( autoT->baseType==Type::tBlock ) {
            if ( initT->baseType!=Type::tBlock )
                return nullptr;
            if ( (autoT->firstType!=nullptr) != (initT->firstType!=nullptr) )   // both do or don't have return type
                return nullptr;
            if ( autoT->argTypes.size() != initT->argTypes.size() )             // both have same number of arguments
                return nullptr;
        }
        // function has to match function
        if ( autoT->baseType==Type::tFunction ) {
            if ( initT->baseType!=Type::tFunction )
                return nullptr;
            if ( (autoT->firstType!=nullptr) != (initT->firstType!=nullptr) )   // both do or don't have return type
                return nullptr;
            if ( autoT->argTypes.size() != initT->argTypes.size() )             // both have same number of arguments
                return nullptr;
        }
        // now, lets make the type
        auto TT = make_shared<TypeDecl>(*initT);
        TT->at = autoT->at;
        TT->alias = autoT->alias;
        if ( autoT->isPointer() ) {
            // if it's a pointer, infer pointer-to separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tArray ) {
            // if it's an array, infer array type separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tTable ) {
            // if it's a table, infer table keys and values types separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
            if ( !TT->firstType->isWorkhorseType() ) return nullptr;            // table key has to be hashable too
            TT->secondType = inferAutoType(autoT->secondType, initT->secondType);
            if ( !TT->secondType ) return nullptr;
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction ) {
            // if it's a block or function, infer argument and return types
            if ( autoT->firstType ) {
                TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
                if ( !TT->firstType ) return nullptr;
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                TT->argTypes[i] = inferAutoType(autoT->argTypes[i], initT->argTypes[i]);
                if ( !TT->argTypes[i] ) return nullptr;
            }
        }
        return TT;
    }


    // TypeDecl

    string TypeDecl::describe ( bool extra ) const {
        TextWriter stream;
        if ( baseType==Type::alias ) {
            stream << alias;
        } else if ( baseType==Type::autoinfer ) {
            stream << "auto";
            if ( !alias.empty() ) {
                stream << "(" << alias << ")";
            }
        } else if ( baseType==Type::tHandle ) {
            stream << annotation->name;
        } else if ( baseType==Type::tArray ) {
            if ( firstType ) {
                stream << "array<" << firstType->describe(extra) << ">";
            } else {
                stream << "array";
            }
        } else if ( baseType==Type::tTable ) {
            if ( firstType && secondType ) {
                stream << "table<" << firstType->describe(extra) << "," << secondType->describe(extra) << ">";
            } else {
                stream << "table";
            }
        } else if ( baseType==Type::tStructure ) {
            if ( structType ) {
                stream << structType->name;
            } else {
                stream << "unspecified structure";
            }
        } else if ( baseType==Type::tPointer ) {
            if ( firstType ) {
                stream << firstType->describe(extra) << "?";
            } else {
                stream << "void ?";
            }
        } else if ( baseType==Type::tEnumeration ) {
            if ( enumType ) {
                stream << enumType->describe();
            } else {
                stream << "unspecified enumeration";
            }
        } else if ( baseType==Type::tIterator ) {
            if ( firstType ) {
                stream << "iterator<" << firstType->describe(extra) << ">";
            } else {
                stream << "iterator";
            }
        } else if ( baseType==Type::tBlock ) {
            stream << "block<";
            if ( argTypes.size() ) {
                stream << "(";
                for ( const auto & arg : argTypes ) {
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                }
                stream << ")";
            }
            if ( firstType ) {
                if ( argTypes.size() ) {
                    stream << ":";
                }
                stream << firstType->describe(extra);
            }
            stream << ">";
        } else if ( baseType==Type::tFunction ) {
            stream << "function<";
            if ( argTypes.size() ) {
                stream << "(";
                for ( const auto & arg : argTypes ) {
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                }
                stream << ")";
            }
            if ( firstType ) {
                if ( argTypes.size() ) {
                    stream << ":";
                }
                stream << firstType->describe(extra);
            }
            stream << ">";
        } else {
            stream << das_to_string(baseType);
        }
        if ( extra && baseType!=Type::autoinfer && baseType!=Type::alias && !alias.empty() ) {
            stream << " aka " << alias;
        }
        if ( constant ) {
            stream << " const";
        } else if ( removeConstant ) {
            stream << " !const";
        }
        for ( auto d : dim ) {
            stream << "[" << d << "]";
        }
        if ( ref ) {
            stream << "&";
        } else if ( removeRef ) {
            stream << "!&";
        }
        return stream.str();
    }

    TextWriter& operator<< (TextWriter& stream, const TypeDecl & decl) {
        stream << decl.describe();
        return stream;
    }

    TypeDecl::TypeDecl(const TypeDecl & decl) {
        baseType = decl.baseType;
        structType = decl.structType;
        enumType = decl.enumType;
        annotation = decl.annotation;
        dim = decl.dim;
        flags = decl.flags;
        alias = decl.alias;
        at = decl.at;
        module = decl.module;
        if ( decl.firstType )
            firstType = make_shared<TypeDecl>(*decl.firstType);
        if ( decl.secondType )
            secondType = make_shared<TypeDecl>(*decl.secondType);
        for ( const auto & arg : decl.argTypes ) {
            argTypes.push_back(make_shared<TypeDecl>(*arg) );
        }
    }

    const TypeDecl * TypeDecl::findAlias ( const string & name, bool allowAuto ) const {
        if (baseType == Type::alias) {
            return nullptr; // if it is another alias, can't find it
        } else if (baseType == Type::autoinfer && !allowAuto) {
            return nullptr; // if it has not been infered yet, can't find it
        }
        else if (alias == name) {
            return this;
        }
        if ( baseType==Type::tPointer ) {
            return firstType ? firstType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tArray ) {
            return firstType ? firstType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tTable ) {
            if ( firstType ) {
                if ( auto k = firstType->findAlias(name,allowAuto) ) {
                    return k;
                }
            }
            return secondType ? secondType->findAlias(name,allowAuto) : nullptr;
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ) {
            for ( auto & arg : argTypes ) {
                if ( auto att = arg->findAlias(name,allowAuto) ) {
                    return att;
                }
            }
            return firstType->findAlias(name,allowAuto);
        } else {
            return nullptr;
        }
    }
    
    bool TypeDecl::canDelete() const {
        if ( baseType==Type::tHandle ) {
            return annotation->canDelete();
        } else if ( baseType==Type::tPointer ) {
            if ( !firstType ) {
                return false;
            } else if ( firstType->dim.size() ) {
                return false;
            } else if (firstType->baseType==Type::tHandle ) {
                return firstType->annotation->canDeletePtr();
            } else if ( firstType->baseType==Type::tStructure ) {
                return true;
            } else {
                return false;
            }
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            return true;
        } else {
            return false;
        }
    }

    bool TypeDecl::canMove() const {
        if ( baseType==Type::tHandle )
            return annotation->canMove();
        if ( baseType==Type::tBlock )
            return false;
        return true;
    }

    bool TypeDecl::canCopy() const {
        if ( baseType==Type::tHandle )
            return annotation->canCopy();
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tBlock )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->canCopy();
        return true;
    }

    bool TypeDecl::isPod() const {
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tString || baseType==Type::tBlock )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->isPod();
        if ( baseType==Type::tHandle )
            return annotation->isPod();
        return true;
    }

    string TypeDecl::getMangledName() const {
        TextWriter ss;
        if ( constant ) {
            ss << "#const";
        }
        if (baseType == Type::autoinfer) {
            ss << "#auto";
        } else if (baseType == Type::alias) {
            ss << "#alias#" << alias;
        } else if ( baseType==Type::tHandle ) {
            ss << "#handle#" << annotation->name;
        } else if ( baseType==Type::tArray ) {
            ss << "#array";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tTable ) {
            ss << "#table";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
            if ( secondType ) {
                ss << "#" << secondType->getMangledName();
            }
        } else if ( baseType==Type::tPointer ) {
            ss << "#ptr";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tEnumeration ) {
            ss << "#enum";
            if ( enumType ) {
                ss << "#" << enumType->getMangledName();
            }
        } else if ( baseType==Type::tIterator ) {
            ss << "#iterator";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tBlock ) {
            ss << "#block";
            for ( auto & arg : argTypes ) {
                ss << "#" << arg->getMangledName();
            }
            if ( firstType ) {
                ss << "#:" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tFunction ) {
            ss << "#function";
            for ( auto & arg : argTypes ) {
                ss << "#" << arg->getMangledName();
            }
            if ( firstType ) {
                ss << "#:" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tStructure ) {
            if ( structType ) {
                ss << structType->name;
            } else {
                ss << "structue?";
            }
        } else {
            ss << das_to_string(baseType);
        }
        if ( ref ) {
            ss << "#ref";
        }
        if ( dim.size() ) {
            for ( auto d : dim ) {
                ss << "#" << d;
            }
        }
        return ss.str();
    }

    bool TypeDecl::isConst() const
    {
        if ( constant )
            return true;
        if ( baseType==Type::tPointer )
            if ( firstType && firstType->constant )
                return true;
        return false;
    }

    bool TypeDecl::isSameType ( const TypeDecl & decl, bool refMatters, bool constMatters ) const {
        if ( baseType!=decl.baseType ) {
            return false;
        }
        if ( baseType==Type::tHandle && annotation!=decl.annotation ) {
            return false;
        }
        if ( baseType==Type::tStructure && structType!=decl.structType ) {
            return false;
        }
        if ( baseType==Type::tPointer || baseType==Type::tIterator ) {
            if ( (firstType && !firstType->isVoid())
                && (decl.firstType && !decl.firstType->isVoid())
                && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
        }
        if ( baseType==Type::tEnumeration ) {
            if ( enumType && decl.enumType && enumType!=decl.enumType ) {
                return false;
            }
        }
        if ( baseType==Type::tArray ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
        }
        if ( baseType==Type::tTable ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
            if ( secondType && decl.secondType && !secondType->isSameType(*decl.secondType) ) {
                return false;
            }
        }
        if ( baseType==Type::tBlock || baseType==Type::tFunction ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
            if ( firstType && argTypes.size()==0 ) {    // if not any block or any function
                if ( argTypes.size() != decl.argTypes.size() ) {
                    return false;
                }
                for ( size_t i=0; i != argTypes.size(); ++i ) {
                    const auto & arg = argTypes[i];
                    const auto & declArg = decl.argTypes[i];
                    if ( !arg->isSameType(*declArg) ) {
                        return false;
                    }
                }
            }
        }
        if ( dim!=decl.dim ) {
            return false;
        }
        if ( refMatters ) {
            if ( ref!=decl.ref ) {
                return false;
            }
        }
        if ( constMatters ) {
            if ( constant!=decl.constant ) {
                return false;
            }
        }
        return true;
    }

    // validate swizzle mask and build mask type

    bool TypeDecl::isSequencialMask ( vector<uint8_t> & fields ) {
        for ( size_t i=1; i<fields.size(); ++i ) {
            if ( (fields[i-1]+1)!=fields[i] ) {
                return false;
            }
        }
        return true;
    }

    int TypeDecl::getMaskFieldIndex ( char ch ) {
        switch ( ch ) {
            case 'x':
            case 'X':
                return 0;
            case 'y':
            case 'Y':
                return 1;
            case 'z':
            case 'Z':
                return 2;
            case 'w':
            case 'W':
                return 3;
            default:
                return -1;
        }
    }

    bool TypeDecl::buildSwizzleMask ( const string & mask, int dim, vector<uint8_t> & fields ) {
        fields.clear();
        for ( auto ch : mask ) {
            int field = getMaskFieldIndex(ch);
            if ( field==-1 || field>=dim ) {
                return false;
            }
            fields.push_back(uint8_t(field));
        }
        return fields.size()>=1 && fields.size()<=4;
    }

    bool TypeDecl::isVectorType() const {
        if ( dim.size() ) return false;
        switch (baseType) {
            case tInt2:
            case tInt3:
            case tInt4:
            case tUInt2:
            case tUInt3:
            case tUInt4:
            case tFloat2:
            case tFloat3:
            case tFloat4:
            case tRange:
            case tURange:
                return true;
            default:
                return false;
        }
    }

    int TypeDecl::getVectorDim() const {
        switch (baseType) {
            case tInt2:
            case tUInt2:
            case tFloat2:
            case tRange:
            case tURange:
                return 2;
            case tInt3:
            case tUInt3:
            case tFloat3:
                return 3;
            case tInt4:
            case tUInt4:
            case tFloat4:
                return 4;
            default:
                assert(0 && "we should not even be here");
                return 0;
        }
    }

    Type TypeDecl::getVectorBaseType() const {
        switch (baseType) {
            case tInt2:
            case tInt3:
            case tInt4:     return Type::tInt;
            case tUInt2:
            case tUInt3:
            case tUInt4:    return Type::tUInt;
            case tFloat2:
            case tFloat3:
            case tFloat4:   return Type::tFloat;
            case tRange:    return Type::tInt;
            case tURange:   return Type::tUInt;
            default:
                assert(0 && "we should not even be here");
                return Type::none;
        }
    }

    Type TypeDecl::getVectorType ( Type bt, int dim ) {
        if ( dim==1 ) return bt;
        if ( bt==Type::tFloat ) {
            switch ( dim ) {
                case 2:     return Type::tFloat2;
                case 3:     return Type::tFloat3;
                case 4:     return Type::tFloat4;
                default:    assert(0 && "we should not be here"); return Type::none;
            }
        } else if ( bt==Type::tInt ) {
            switch ( dim ) {
                case 2:     return Type::tInt2;
                case 3:     return Type::tInt3;
                case 4:     return Type::tInt4;
                default:    assert(0 && "we should not be here"); return Type::none;
            }
        } else if ( bt==Type::tUInt ) {
            switch ( dim ) {
                case 2:     return Type::tUInt2;
                case 3:     return Type::tUInt3;
                case 4:     return Type::tUInt4;
                default:    assert(0 && "we should not be here"); return Type::none;
            }
        } else {
            assert(0 && "we should not be here");
            return Type::none;
        }
    }

    bool TypeDecl::isGoodIteratorType() const {
        return baseType==Type::tIterator && dim.size()==0 && firstType;
    }

    bool TypeDecl::isGoodBlockType() const {
        return baseType==Type::tBlock && dim.size()==0;
    }

    bool TypeDecl::isGoodFunctionType() const {
        return baseType==Type::tFunction && dim.size()==0;
    }

    bool TypeDecl::isGoodArrayType() const {
        return baseType==Type::tArray && dim.size()==0 && firstType;
    }

    bool TypeDecl::isGoodTableType() const {
        return baseType==Type::tTable && dim.size()==0 && firstType && secondType;
    }

    bool TypeDecl::isIteratorType ( const TypeDecl & decl ) const {
        if ( baseType!=decl.baseType )
            return false;
        if ( baseType==Type::tStructure && structType!=decl.structType )
            return false;
        if ( decl.dim.size() )
            return false;
        if ( !decl.isRef() )
            return false;
        return true;
    }

    bool TypeDecl::isVoid() const {
        return (baseType==Type::tVoid) && (dim.size()==0);
    }

    bool TypeDecl::isPointer() const {
        return (baseType==Type::tPointer) && (dim.size()==0);
    }

    bool TypeDecl::isEnum() const {
        return (baseType==Type::tEnumeration) && (dim.size()==0);
    }

    bool TypeDecl::isAlias() const {
        // auto is auto.... or auto....?
        if ( baseType==Type::alias ) {
            return true;
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                return firstType->isAlias();
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                return firstType->isAlias();
        } else if ( baseType==Type::tTable ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAlias();
            if ( secondType )
                any |= secondType->isAlias();
            return any;
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAlias();
            for ( auto & arg : argTypes )
                any |= arg->isAlias();
            return any;
        }
        return false;
    }

    bool TypeDecl::isAuto() const {
        // auto is auto.... or auto....?
        if ( baseType==Type::autoinfer ) {
            return true;
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                return firstType->isAuto();
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                return firstType->isAuto();
        } else if ( baseType==Type::tTable ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAuto();
            if ( secondType )
                any |= secondType->isAuto();
            return any;
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAuto();
            for ( auto & arg : argTypes )
                any |= arg->isAuto();
            return any;
        }
        return false;
    }

    bool TypeDecl::isFoldable() const {
        if ( dim.size() || ref )
            return false;
        switch ( baseType ) {
            case Type::tBool:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tInt64:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tUInt64:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
            case Type::tString:
            case Type::tDouble:
            case Type::tEnumeration:
                return true;
            default:
                return false;
        }
    }

    bool TypeDecl::isHandle() const {
        return (baseType==Type::tHandle) && (dim.size()==0);
    }

    bool TypeDecl::isSimpleType() const {
        if (    baseType==Type::none
            ||  baseType==Type::tVoid
            ||  baseType==Type::tStructure
            ||  baseType==Type::tPointer
            ||  baseType==Type::tEnumeration )
            return false;
        if ( dim.size() )
            return false;
        return true;
    }

    bool TypeDecl::isCtorType() const {
        if ( dim.size() )
            return false;
        switch ( baseType ) {
            // case Type::tBool:
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
            case Type::tRange:
            case Type::tURange:
            case Type::tString:
            case Type::tDouble:
            // case Type::tPointer:
                return true;
            default:
                return false;
        }
    }

    bool TypeDecl::isWorkhorseType() const {
        if ( dim.size() )
            return false;
        switch ( baseType ) {
            case Type::tBool:
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tEnumeration:
            case Type::tInt:
            case Type::tInt2:
            case Type::tInt3:
            case Type::tInt4:
            case Type::tUInt:
            case Type::tUInt2:
            case Type::tUInt3:
            case Type::tUInt4:
            case Type::tFloat:
            case Type::tFloat2:
            case Type::tFloat3:
            case Type::tFloat4:
            case Type::tRange:
            case Type::tURange:
            case Type::tString:
            case Type::tPointer:
            case Type::tDouble:
                return true;
            default:
                return false;
        }
    }

    bool TypeDecl::isReturnType() const {
        if ( isGoodBlockType() ) {
            return false;
        }
        return true;
    }

    Type TypeDecl::getRangeBaseType() const
    {
        switch ( baseType ) {
            case Type::tRange:  return Type::tInt;
            case Type::tURange: return Type::tUInt;
            default:
                assert(0 && "we should not even be here");
                return Type::none;
        }
    }

    bool TypeDecl::isRange() const
    {
        return (baseType==Type::tRange || baseType==Type::tURange) && dim.size()==0;
    }

    bool TypeDecl::isSimpleType(Type typ) const {
        return baseType==typ && isSimpleType();
    }

    bool TypeDecl::isArray() const {
        return (bool) dim.size();
    }

    bool TypeDecl::isRef() const {
        return ref || isRefType();
    }

    bool TypeDecl::isRefType() const {
        if ( baseType==Type::tHandle ) {
            return annotation->isRefType();
        }
        return baseType==Type::tStructure || baseType==Type::tArray || baseType==Type::tTable || dim.size();
    }

    bool TypeDecl::isIndex() const {
        return (baseType==Type::tInt || baseType==Type::tUInt) && dim.size()==0;
    }

    int TypeDecl::getBaseSizeOf() const {
        if ( baseType==Type::tHandle ) {
            return int(annotation->getSizeOf());
        }
        return baseType==Type::tStructure ? structType->getSizeOf() : getTypeBaseSize(baseType);
    }

    int TypeDecl::getAlignOf() const {
        if ( baseType==Type::tHandle ) {
            return int(annotation->getAlignOf());
        }
        return baseType==Type::tStructure ? structType->getAlignOf() : getTypeBaseAlign(baseType);
    }

    int TypeDecl::getCountOf() const {
        int size = 1;
        for ( auto i : dim )
            size *= i;
        return size;
    }
    
    int TypeDecl::getSizeOf() const {
        return getBaseSizeOf() * getCountOf();
    }

    int TypeDecl::getStride() const {
        int size = 1;
        if ( dim.size() > 1 ) {
            for ( size_t i=0; i!=dim.size()-1; ++i )
                size *= dim[i];
        }
        return getBaseSizeOf() * size;
    }
}
