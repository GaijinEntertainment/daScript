#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das
{
    TypeDeclPtr makeHandleType(const ModuleLibrary & library, const char * typeName) {
        return library.makeHandleType(typeName);
    }

    // auto or generic type conversion

    bool TypeDecl::isExprType() const {
        for ( auto di : dim ) {
            if ( di==TypeDecl::dimConst ) {
                return true;
            }
        }
        if ( firstType && firstType->isExprType() ) return true;
        if ( secondType && secondType->isExprType() ) return true;
        for ( auto & arg : argTypes ) {
            if ( arg->isExprType() ) {
                return true;
            }
        }
        return false;
    }

    TypeDeclPtr TypeDecl::visit ( Visitor & vis ) {
        for ( size_t i=0; i!=dim.size(); ++i ) {
            if ( dim[i]==TypeDecl::dimConst ) {
                if ( dimExpr[i] ) {
                    dimExpr[i] = dimExpr[i]->visit(vis);
                }
            }
        }
        if ( firstType ) {
            firstType = firstType->visit(vis);
        }
        if ( secondType ) {
            secondType = secondType->visit(vis);
        }
        for ( auto & argType : argTypes ) {
            argType = argType->visit(vis);
        }
        return shared_from_this();
    }

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
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction ||
                   autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple ) {
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
        } else if ( autoT->baseType != Type::autoinfer ) {
            // if we are matching to strong type, with dim only, then it has to match
            if ( autoT->baseType != initT->baseType ) {
                return nullptr;
            }
        }
        // auto & can't be infered from non-ref
        if ( autoT->ref && !initT->ref )
            return nullptr;
        // auto[][][] can't be infered from non-array
        if ( autoT->dim.size() ) {
            if ( autoT->dim.size()!=initT->dim.size() )
                return nullptr;
            for ( size_t di=0; di!=autoT->dim.size(); ++di ) {
                int32_t aDI = autoT->dim[di];
                int32_t iDI = initT->dim[di];
                if ( aDI!=TypeDecl::dimAuto && aDI!=iDI ) {
                    return nullptr;
                }
            }
        }
        // auto? can't be infered from non-pointer
        if ( autoT->isPointer() && (!initT->isPointer() || !initT->firstType) )
            return nullptr;
        // array has to match array
        if ( autoT->baseType==Type::tArray && (initT->baseType!=Type::tArray || !initT->firstType) )
            return nullptr;
        // table has to match table
        if ( autoT->baseType==Type::tTable && (initT->baseType!=Type::tTable || !initT->firstType || !initT->secondType) )
            return nullptr;
        // block has to match block, function to function, lambda to lambda
        if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction
            || autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple ) {
            if ( initT->baseType!=autoT->baseType )
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
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction
                   || autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple ) {
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

    string TypeDecl::describe ( bool extra, bool contracts ) const {
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda ) {
            stream << das_to_string(baseType) << "<";
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
        } else if ( baseType==Type::tTuple ) {
            stream << das_to_string(baseType) << "<";
            if ( argTypes.size() ) {
                for ( const auto & arg : argTypes ) {
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                }
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
        } 
        for ( auto d : dim ) {
            stream << "[" << d << "]";
        }
        if ( ref ) {
            stream << "&";
        } 
        if (contracts) {
            if (removeConstant || removeRef) {
                stream << " delete ";
                if (removeConstant) {
                    stream << "const";
                }
                if (removeRef) {
                    stream << "&";
                }
            }
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
        dimExpr.reserve(decl.dimExpr.size());
        for ( auto & de : decl.dimExpr ) {
            if ( de ) {
                dimExpr.push_back(de->clone());
            } else {
                dimExpr.push_back(nullptr);
            }
        }
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction
                   || baseType==Type::tLambda || baseType==Type::tTuple ) {
            for ( auto & arg : argTypes ) {
                if ( auto att = arg->findAlias(name,allowAuto) ) {
                    return att;
                }
            }
            return firstType ? firstType->findAlias(name,allowAuto) : nullptr;
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
            } else if ( firstType->baseType==Type::tTuple ) {
                return true;
            }else {
                return false;
            }
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            return true;
        } else if ( baseType==Type::tString ) {
            return true;
        } else {
            return false;
        }
    }

    bool TypeDecl::canAot() const {
        set<Structure *> recAot;
        return canAot(recAot);
    }

    bool TypeDecl::canAot( set<Structure *> & recAot) const {
        if ( annotation && !annotation->canAot(recAot) ) return false;
        if ( firstType && !firstType->canAot(recAot) ) return false;
        if ( secondType && !secondType->canAot(recAot) ) return false;
        for ( auto & arg : argTypes ) {
            if ( !arg->canAot(recAot) ) return false;
        }
        if ( structType ) {
            if ( recAot.find(structType)==recAot.end() ) {
                recAot.insert(structType);
                return structType->canAot(recAot);
            }
        }
        return true;
    }

    bool TypeDecl::canMove() const {
        if ( baseType==Type::tHandle )
            return annotation->canMove();
        if ( baseType==Type::tBlock )
            return false;
        return true;
    }

    bool TypeDecl::canClone() const {
        if ( baseType==Type::tHandle )
            return annotation->canClone();
        if ( baseType==Type::tStructure && structType )
            return structType->canClone();
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
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tString
                || baseType==Type::tBlock || baseType==Type::tLambda )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->isPod();
        if ( baseType==Type::tHandle )
            return annotation->isPod();
        if ( baseType==Type::tTuple ) {
            for ( auto & arg : argTypes ) {
                if ( !arg->isPod() ) {
                    return false;
                }
            }
        }
        return true;
    }

    bool TypeDecl::isRawPod() const {
        if ( baseType==Type::tArray || baseType==Type::tTable || baseType==Type::tString
            || baseType==Type::tBlock || baseType==Type::tLambda )
            return false;
        if ( baseType==Type::tStructure && structType )
            return structType->isRawPod();
        if ( baseType==Type::tHandle )
            return annotation->isRawPod();
        if ( baseType==Type::tPointer )
            return false;
        if ( baseType==Type::tTuple ) {
            for ( auto & arg : argTypes ) {
                if ( !arg->isRawPod() ) {
                    return false;
                }
            }
        }
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda || baseType==Type::tTuple ) {
            ss << "#" << das_to_string(baseType);
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

    bool TypeDecl::isLocal() const {
        if ( isHandle() ) {
            return annotation->isLocal();
        } else if ( isStructure() ) {
            return structType ? structType->isLocal() : false;
        } else if ( isPointer() ) {
            return true;
        }
        if ( firstType && !firstType->isLocal() ) {
            return false;
        }
        if ( secondType && !secondType->isLocal() ) {
            return false;
        }
        if ( baseType==Type::tTuple ) {
            for ( const auto & arg : argTypes ) {
                if ( !arg->isLocal() ) {
                    return false;
                }
            }
        }
        return true;
    }

    bool TypeDecl::isConst() const
    {
        return constant;
        /*
        if ( constant )
            return true;
        if ( baseType==Type::tPointer )
            if ( firstType && firstType->constant )
                return true;
        return false;
        */
    }

    bool TypeDecl::isSameType ( const TypeDecl & decl, bool refMatters, bool constMatters, bool topLevel ) const {
        if ( topLevel && !isRef() ) {
            constMatters = false;
        }
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
                && !firstType->isSameType(*decl.firstType,true,true,false) ) {
                return false;
            }
        }
        if ( baseType==Type::tEnumeration ) {
            if ( enumType && decl.enumType && enumType!=decl.enumType ) {
                return false;
            }
        }
        if ( baseType==Type::tArray ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType,true,true,false) ) {
                return false;
            }
        }
        if ( baseType==Type::tTable ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType,true,true,false) ) {
                return false;
            }
            if ( secondType && decl.secondType && !secondType->isSameType(*decl.secondType,true,true,false) ) {
                return false;
            }
        }
        if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda || baseType==Type::tTuple ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType) ) {
                return false;
            }
            if ( firstType || argTypes.size() ) {    // if not any block or any function
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

    bool TypeDecl::isSequencialMask ( const vector<uint8_t> & fields ) {
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
                DAS_ASSERTF(0,
                        "we should not even be here. we are calling getVectorDim on an unsupported baseType."
                        "likely new vector type been added.");
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
                DAS_ASSERTF(0,
                       "we should not even be here. we are calling getVectorBaseType on an unsuppored baseType."
                       "likely new vector type been added.");
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
                default:    DAS_ASSERTF(0, "we should not be herewe are calling getVectorType on an unsuppored baseType."
                                   "likely new vector type been added.");
                            return Type::none;
            }
        } else if ( bt==Type::tInt ) {
            switch ( dim ) {
                case 2:     return Type::tInt2;
                case 3:     return Type::tInt3;
                case 4:     return Type::tInt4;
                default:    DAS_ASSERTF(0, "we should not be here. we are calling getVectorType on an unsuppored baseType."
                                   "likely new vector type been added.");
                    return Type::none;
            }
        } else if ( bt==Type::tUInt ) {
            switch ( dim ) {
                case 2:     return Type::tUInt2;
                case 3:     return Type::tUInt3;
                case 4:     return Type::tUInt4;
                default:    DAS_ASSERTF(0, "we should not be here. we are calling getVectorType on an unsuppored baseType."
                                   "likely new vector type been added.");
                    return Type::none;
            }
        } else {
            DAS_ASSERTF(0, "we should not be here. we are calling getVectorType on an unsuppored baseType."
                   "likely new vector type been added.");
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

    bool TypeDecl::isGoodLambdaType() const {
        return baseType==Type::tLambda && dim.size()==0;
    }

    bool TypeDecl::isGoodArrayType() const {
        return baseType==Type::tArray && dim.size()==0 && firstType;
    }

    bool TypeDecl::isGoodTupleType() const {
        return baseType==Type::tTuple && dim.size()==0;
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
        } else  if ( baseType==Type::tIterator ) {
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda || baseType==Type::tTuple ) {
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
        // also dim[] is aito
        for ( auto di : dim ) {
            if ( di==TypeDecl::dimAuto ) {
                return true;
            }
        }
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda || baseType==Type::tTuple ) {
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
            case Type::tRange:
            case Type::tURange:
                return true;
            default:
                return false;
        }
    }

    bool TypeDecl::isHandle() const {
        return (baseType==Type::tHandle) && (dim.size()==0);
    }

    bool TypeDecl::isStructure() const {
        return (baseType==Type::tStructure) && (dim.size()==0);
    }

    bool TypeDecl::isTuple() const {
        return (baseType==Type::tTuple) && (dim.size()==0);
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
            case Type::tInt8:
            case Type::tUInt8:
            case Type::tInt16:
            case Type::tUInt16:
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

    bool TypeDecl::canInitWithZero() const {
        if ( dim.size() )
            return false;
        if ( isVectorType() )
            return true;
        switch ( baseType ) {
            case Type::tBool:
            case Type::tInt8:
            case Type::tUInt8:
            case Type::tInt16:
            case Type::tUInt16:
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tInt:
            case Type::tUInt:
            case Type::tFloat:
            case Type::tDouble:
            case Type::tString:
            case Type::tPointer:
                return true;
            default:
                return false;
        }
    }

    bool TypeDecl::isPolicyType() const {
        if ( dim.size() )
            return true;
        switch ( baseType ) {
            case Type::tVoid:
            case Type::tEnumeration:
            case Type::tBool:
                /*
            case Type::tInt8:
            case Type::tUInt8:
            case Type::tInt16:
            case Type::tUInt16:
                 */
            case Type::tInt64:
            case Type::tUInt64:
            case Type::tInt:
            case Type::tUInt:
            case Type::tFloat:
            case Type::tDouble:
            case Type::tPointer:
            case Type::tFunction:
                return false;
            default:
                return true;
        }
    }

    bool TypeDecl::isVecPolicyType() const {
        if ( dim.size() )
            return false;
        if ( baseType==Type::tString ) {
            return false;
        }
        return isPolicyType();
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
                DAS_ASSERTF(0, "we should not even be here. we are calling getRangeBaseType on an unsuppored baseType."
                       "likely new range type been added.");
                return Type::none;
        }
    }

    bool TypeDecl::isRange() const
    {
        return (baseType==Type::tRange || baseType==Type::tURange) && dim.size()==0;
    }

    bool TypeDecl::isString() const
    {
        return (baseType==Type::tString) && dim.size()==0;
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
        return baseType==Type::tTuple || baseType==Type::tStructure || baseType==Type::tArray
                || baseType==Type::tTable || baseType==Type::tBlock
                || dim.size();
    }

    bool TypeDecl::isIndex() const {
        return (baseType==Type::tInt || baseType==Type::tUInt) && dim.size()==0;
    }

    int TypeDecl::getTupleFieldOffset ( int index ) const {
        DAS_ASSERT(baseType==Type::tTuple);
        DAS_ASSERT(index>=0 && index<int(argTypes.size()));
        int size = 0, idx = 0;
        for ( const auto & argT : argTypes ) {
            int al = argT->getAlignOf() - 1;
            size = (size + al) & ~al;
            if ( idx==index ) {
                return size;
            }
            size += argT->getSizeOf();
            idx ++;
        }
        DAS_ASSERT(0 && "we should not even be here. field index out of range somehow???");
        return -1;
    }

    int TypeDecl::getTupleSize() const {
        DAS_ASSERT(baseType==Type::tTuple);
        int size = 0;
        for ( const auto & argT : argTypes ) {
            int al = argT->getAlignOf() - 1;
            size = (size + al) & ~al;
            size += argT->getSizeOf();
        }
        int al = getTupleAlign() - 1;
        size = (size + al) & ~al;
        return size;
    }

    int TypeDecl::getTupleAlign() const {
        DAS_ASSERT(baseType==Type::tTuple);
        int align = 1;
        for ( const auto & argT : argTypes ) {
            align = das::max ( argT->getAlignOf(), align );
        }
        return align;
    }

    int TypeDecl::getBaseSizeOf() const {
        if ( baseType==Type::tHandle ) {
            return int(annotation->getSizeOf());
        } else if ( baseType==Type::tStructure ) {
            return structType->getSizeOf();
        } else if ( baseType==Type::tTuple ) {
            return getTupleSize();
        } else {
           return getTypeBaseSize(baseType);
        }
    }

    int TypeDecl::getAlignOf() const {
        if ( baseType==Type::tHandle ) {
            return int(annotation->getAlignOf());
        } else if ( baseType==Type::tStructure ) {
            return structType->getAlignOf();
        } else if ( baseType==Type::tTuple ) {
            return getTupleAlign();
        } else {
            return getTypeBaseAlign(baseType);
        }
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

    bool isCircularType ( const TypeDeclPtr & type, vector<const TypeDecl *> & all ) {
        if ( type->baseType==Type::tPointer ) {
            return false;
        } 
        if ( type->firstType && isCircularType(type->firstType, all) ) return true;
        if ( type->secondType && isCircularType(type->secondType, all) ) return true;
        auto pt = type.get();
        for (auto it : all) {
            if ( it == pt ) return true;
        }
        all.push_back(pt);
        if ( type->baseType==Type::tStructure ) {
            if ( type->structType ) {
                for ( auto & fd : type->structType->fields ) {
                    if ( isCircularType(fd.type, all) ) return true;
                }
            }
        }  else if ( type->baseType==Type::tTuple ) {
            for ( auto & arg : type->argTypes ) {
                if ( isCircularType(arg, all) ) return true;
            }
        }
        all.pop_back();
        return false;
    }

    bool isCircularType ( const TypeDeclPtr & type ) {
        vector<const TypeDecl *> all;
        return isCircularType(type, all);
    }
}
