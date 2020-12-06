#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das
{
    TypeDeclPtr makeHandleType(const ModuleLibrary & library, const char * typeName) {
        return library.makeHandleType(typeName);
    }

    // auto or generic type conversion

    TypeDecl::TypeDecl(const EnumerationPtr & ep)
        : baseType(ep->getEnumType()), enumType(ep.get())
    {
    }

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
        return this;
    }

    void TypeDecl::applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT ) {
        if ( !autoT->isAuto() ) return;
        TT->ref = (TT->ref | autoT->ref) && !autoT->removeRef && !TT->removeRef;
        TT->constant = (TT->constant | autoT->constant) && !autoT->removeConstant && !TT->removeConstant;
        TT->temporary = (TT->temporary | autoT->temporary) && !autoT->removeTemporary && !TT->removeTemporary;
        if ( (autoT->removeDim || TT->removeDim) && TT->dim.size() ) TT->dim.erase(TT->dim.begin());
        TT->removeConstant = false;
        TT->removeDim = false;
        TT->removeRef = false;
        if ( autoT->isPointer() ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tIterator ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tArray ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tTable ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
            applyAutoContracts(TT->secondType, autoT->secondType);
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction ||
                   autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple ||
                   autoT->baseType==Type::tVariant ) {
            if ( TT->firstType ) {
                applyAutoContracts(TT->firstType, autoT->firstType);
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                applyAutoContracts(TT->argTypes[i], autoT->argTypes[i]);
            }
        }
    }

    void TypeDecl::updateAliasMap ( const TypeDeclPtr & decl, const TypeDeclPtr & pass, AliasMap & aliases ) {
        if ( !pass ) {
            return;
        } else if ( decl->baseType==Type::autoinfer ) {
            if ( !decl->alias.empty() && aliases.find(decl->alias)==aliases.end() ) {
                auto TT = make_smart<TypeDecl>(*pass);
                TT->alias = decl->alias;
                TT->dim.clear();
                TT->constant = false;
                TT->temporary = false;
                TT->ref = false;
                aliases[decl->alias] = TT;
            }
        } else if ( pass->baseType==Type::autoinfer ) {
            if ( !pass->alias.empty() && aliases.find(pass->alias)==aliases.end() ) {
                auto TT = make_smart<TypeDecl>(*decl);
                TT->alias = pass->alias;
                TT->dim.clear();
                TT->constant = false;
                TT->temporary = false;
                TT->ref = false;
                aliases[pass->alias] = TT;
            }
        } else {
            if ( decl->firstType ) updateAliasMap(decl->firstType, pass->firstType, aliases);
            if ( decl->secondType ) updateAliasMap(decl->secondType, pass->secondType, aliases);
            for ( size_t iA=0; iA!=decl->argTypes.size(); ++iA ) {
                updateAliasMap(decl->argTypes[iA], pass->argTypes[iA], aliases);
            }
        }
    }

    TypeDeclPtr TypeDecl::inferGenericInitType ( TypeDeclPtr autoT, TypeDeclPtr initT, AliasMap * aliases ) {
        if ( autoT->ref ) {
            autoT->ref = false;
            auto resT = inferGenericType(autoT, initT, aliases);
            if ( resT ) resT->ref = true;
            autoT->ref = true;
            return resT;
        } else {
            return inferGenericType(autoT, initT,aliases);
        }
    }

    TypeDeclPtr TypeDecl::inferGenericType ( TypeDeclPtr autoT, TypeDeclPtr initT, AliasMap * /* aliases */ ) {
        // can't infer from the type, which is already 'auto'
        if (initT->isAuto()) {
            if (!autoT->isAuto()) {
                return make_smart<TypeDecl>(*autoT);
            } else if (autoT->baseType == Type::tBlock || autoT->baseType == Type::tFunction
                || autoT->baseType == Type::tLambda || autoT->baseType == Type::tTuple
                || autoT->baseType == Type::tVariant ) {
                /* two-sided infer */
            } else {
                /*
                if ( aliases && !autoT->alias.empty() ) {
                    auto it = aliases->find(autoT->alias);
                    if ( it != aliases->end() ) {
                        return make_smart<TypeDecl>(*it->second);
                    }
                }
                */
                return nullptr;
            }
        }
        // if its not an auto type, return as is
        if ( !autoT->isAuto() ) {
            if ( autoT->isSameType(*initT, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes) ) {
                return make_smart<TypeDecl>(*autoT);
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
        if ( autoT->ref && !initT->isRef() )
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
        // non-implicit temp can't be infered from non-temp, and non-temp from temp
        if ( autoT->baseType!=autoinfer && !autoT->implicit && !initT->implicit ) {
            if ( autoT->temporary != initT->temporary )
                return nullptr;
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
            || autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple
            || autoT->baseType==Type::tVariant ) {
            if ( initT->baseType!=autoT->baseType )
                return nullptr;
            if ( (autoT->firstType!=nullptr) != (initT->firstType!=nullptr) )   // both do or don't have return type
                return nullptr;
            if ( autoT->argTypes.size() != initT->argTypes.size() )             // both have same number of arguments
                return nullptr;
        }
        // now, lets make the type
        auto TT = make_smart<TypeDecl>(*initT);
        TT->at = autoT->at;
        TT->aotAlias = autoT->aotAlias;
        TT->alias = autoT->alias;
        TT->removeRef = false;
        TT->removeConstant = false;
        TT->removeTemporary = false;
        TT->removeDim = false;
        if ( autoT->isPointer() ) {
            // if it's a pointer, infer pointer-to separately
            TT->firstType = inferGenericType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tIterator ) {
            // if it's a iterator, infer iterator-ofo separately
            TT->firstType = inferGenericType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tArray ) {
            // if it's an array, infer array type separately
            TT->firstType = inferGenericType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tTable ) {
            // if it's a table, infer table keys and values types separately
            TT->firstType = inferGenericType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
            if ( !TT->firstType->isWorkhorseType() ) return nullptr;            // table key has to be hashable too
            TT->secondType = inferGenericType(autoT->secondType, initT->secondType);
            if ( !TT->secondType ) return nullptr;
        } else if ( autoT->baseType==Type::tBlock || autoT->baseType==Type::tFunction
                   || autoT->baseType==Type::tLambda || autoT->baseType==Type::tTuple
                   || autoT->baseType==Type::tVariant ) {
            // if it's a block or function, infer argument and return types
            if ( autoT->firstType ) {
                TT->firstType = inferGenericType(autoT->firstType, initT->firstType);
                if ( !TT->firstType ) return nullptr;
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                TT->argTypes[i] = inferGenericType(autoT->argTypes[i], initT->argTypes[i]);
                if ( !TT->argTypes[i] ) return nullptr;
            }
            if ( TT->argNames.size()==0 && !autoT->argNames.empty() ) {
                TT->argNames = autoT->argNames;
            }
        }
        return TT;
    }


    // TypeDecl

    string TypeDecl::describe ( DescribeExtra extra, DescribeContracts contracts, DescribeModule dmodule ) const {
        TextWriter stream;
        if ( baseType==Type::alias ) {
            stream << alias;
        } else if ( baseType==Type::autoinfer ) {
            stream << "auto";
            if ( !alias.empty() ) {
                stream << "(" << alias << ")";
            }
        } else if ( baseType==Type::tHandle ) {
            if ( annotation ) {
                if (dmodule == DescribeModule::yes && annotation->module && !annotation->module->name.empty()) {
                    stream << annotation->module->name << "::";
                }
                stream << annotation->name;
            } else {
                stream << "unspecified annotation";
            }
        } else if ( baseType==Type::tArray ) {
            if ( firstType ) {
                stream << "array<" << firstType->describe(extra) << ">";
            } else {
                stream << "array";
            }
        } else if ( baseType==Type::tTable ) {
            if ( firstType && secondType ) {
                stream << "table<" << firstType->describe(extra) << ";" << secondType->describe(extra) << ">";
            } else {
                stream << "table";
            }
        } else if ( baseType==Type::tStructure ) {
            if ( structType ) {
                if (dmodule == DescribeModule::yes && structType->module && !structType->module->name.empty()) {
                    stream << structType->module->name << "::";
                }
                stream << structType->name;
            } else {
                stream << "unspecified structure";
            }
        } else if ( baseType==Type::tPointer ) {
            if ( smartPtr ) stream << "smart_ptr<";
            if ( firstType ) {
                stream << firstType->describe(extra);
            } else {
                stream << "void";
            }
            stream << (smartPtr ? ">" : "?");
        } else if ( isEnumT() ) {
            if ( enumType ) {
                if (dmodule == DescribeModule::yes && enumType->module && !enumType->module->name.empty()) {
                    stream << enumType->module->name << "::";
                }
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
                for ( size_t ai=0; ai!=argTypes.size(); ++ ai ) {
                    if ( ai!=0 ) stream << ";";
                    if ( argNames.size()==argTypes.size() ) {
                        stream << argNames[ai] << ":";
                    }
                    stream << argTypes[ai]->describe(extra);
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
            if ( argNames.size() && argNames.size()!=argTypes.size() ) {
                stream << " DAS_COMMENT(malformed) ";
            }
        } else if ( baseType==Type::tTuple ) {
            stream << das_to_string(baseType) << "<";
            if ( argTypes.size() ) {
                int ai = 0;
                for ( const auto & arg : argTypes ) {
                    if ( argNames.size()==argTypes.size() ) {
                        const auto & argName = argNames[ai];
                        if ( !argName.empty() ) stream << argName << ":";
                    }
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                    ai ++;
                }
            }
            stream << ">";
        } else if ( baseType==Type::tVariant ) {
            stream << das_to_string(baseType) << "<";
            if ( argTypes.size() ) {
                int ai = 0;
                for ( const auto & arg : argTypes ) {
                    if ( argNames.size()==argTypes.size() ) {
                        const auto & argName = argNames[ai];
                        if ( !argName.empty() ) stream << argName << ":";
                    }
                    stream << arg->describe(extra);
                    if ( arg != argTypes.back() ) {
                        stream << ";";
                    }
                    ai ++;
                }
            }
            stream << ">";
        } else if ( baseType==Type::tBitfield ) {
            stream << das_to_string(baseType);
            if ( argNames.size() ) {
                stream << "<";
                int ai = 0;
                for ( const auto & arg : argNames ) {
                    if ( ai !=0 ) stream << ";";
                    stream << arg;
                    ai ++;
                }
                stream << ">";
            }
        }else {
            stream << das_to_string(baseType);
        }
        if ( extra==DescribeExtra::yes && baseType!=Type::autoinfer && baseType!=Type::alias && !alias.empty() ) {
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
        if ( temporary ) {
            stream << "#";
        }
        if ( implicit ) {
            stream << " implicit";
        }
        if ( contracts==DescribeContracts::yes && isExplicit ) {
            stream << " explicit";
        }
        if ( explicitConst ) {
            stream << " =const";
        }
        if (contracts == DescribeContracts::yes) {
            if (removeConstant || removeRef || removeDim || removeTemporary) {
                if (removeConstant) {
                    stream << " -const";
                }
                if (removeRef) {
                    stream << " -&";
                }
                if (removeDim ) {
                    stream << " -[]";
                }
                if (removeTemporary) {
                    stream << " -#";
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
            firstType = make_smart<TypeDecl>(*decl.firstType);
        if ( decl.secondType )
            secondType = make_smart<TypeDecl>(*decl.secondType);
        for ( const auto & arg : decl.argTypes ) {
            argTypes.push_back(make_smart<TypeDecl>(*arg) );
        }
        argNames = decl.argNames;
    }

    TypeDecl * TypeDecl::findAlias ( const string & name, bool allowAuto ) {
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
        } else if ( baseType==Type::tIterator ) {
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
                   || baseType==Type::tLambda || baseType==Type::tTuple
                   || baseType==Type::tVariant ) {
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
            } else if (firstType->baseType==Type::tHandle ) {
                return firstType->annotation->canDeletePtr();
            } else if ( firstType->baseType==Type::tStructure ) {
                return true;
            } else if ( firstType->baseType==Type::tTuple ) {
                return true;
            } else if ( firstType->baseType==Type::tVariant ) {
                return true;
            } else {
                return false;
            }
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            return true;
        } else if ( baseType==Type::tString ) {
            return false;
        } else if ( baseType==Type::tStructure ) {
            return true;
        } else if ( baseType==Type::tLambda ) {
            return true;
        } else if ( baseType==Type::tIterator ) {
            return true;
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            return true;
        } else {
            return false;
        }
    }

    bool TypeDecl::needDelete() const {
        if ( baseType==Type::tHandle ) {
            return annotation->needDelete();
        } else if ( baseType==Type::tPointer ) {
            return canDelete();
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            return true;
        } else if ( baseType==Type::tString ) {
            return false;
        } else if ( baseType==Type::tStructure ) {
            return true;
        } else if ( baseType==Type::tLambda ) {
            return true;
        } else if ( baseType==Type::tIterator ) {
            return true;
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            return true;
        } else {
            return false;
        }
    }

    bool TypeDecl::canAot() const {
        das_set<Structure *> recAot;
        return canAot(recAot);
    }

    bool TypeDecl::canAot( das_set<Structure *> & recAot) const {
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
        if (baseType == Type::tHandle) {
            return annotation->canMove();
        } else if (baseType == Type::tBlock) {
            return false;
        } else if (baseType == Type::tStructure && structType) {
            return structType->canMove();
        } else if (baseType == Type::tTuple || baseType == Type::tVariant) {
            for (const auto & arg : argTypes) {
                if (!arg->canMove()) return false;
            }
            return true;
        } else {
            return true;
        }
    }

    bool TypeDecl::canClone() const {
        if (baseType == Type::tHandle) {
            return annotation->canClone();
        } else if (baseType == Type::tStructure && structType) {
            return structType->canClone();
        } else if (baseType == Type::tTuple || baseType == Type::tVariant) {
            for (const auto & arg : argTypes) {
                if (!arg->canClone()) return false;
            }
            return true;
        } else if (baseType == Type::tBlock) {
            return false;
        } else if (baseType == Type::tIterator) {
            return false;
        } else if (baseType == Type::tPointer) {
            return true;
        } else {
            return true;
        }
    }

    bool TypeDecl::canCopy() const {
        if ( baseType == Type::tHandle ) {
            return annotation->canCopy();
        } else if (baseType == Type::tArray || baseType == Type::tTable || baseType == Type::tBlock || baseType == Type::tIterator) {
            return false;
        } else if (baseType == Type::tStructure && structType) {
            return structType->canCopy();
        } else if (baseType == Type::tTuple || baseType == Type::tVariant) {
            for (const auto & arg : argTypes) {
                if (!arg->canCopy()) return false;
            }
            return true;
        } else if (baseType == Type::tPointer) {
            return !smartPtr;
        } if ( baseType == Type::tLambda ) {
            return false;
        } else {
            return true;
        }
    }

        bool TypeDecl::isNoHeapType() const {
            if ( baseType==Type::tArray || baseType==Type::tTable
                    || baseType==Type::tBlock || baseType==Type::tLambda )
                return false;
            if ( baseType==Type::tStructure && structType )
                return structType->isNoHeapType();
            if ( baseType==Type::tHandle )
                return annotation->isPod();
            if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
                for ( auto & arg : argTypes ) {
                    if ( !arg->isNoHeapType() ) {
                        return false;
                    }
                }
            }
            if ( baseType==Type::tPointer ) return false;   // pointer can point to heap
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
        if ( baseType==Type::tPointer )
            return !smartPtr;
        if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
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
        if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
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
            ss << (smartPtr ? "#smart_ptr" : "#ptr");
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tEnumeration ) {
            ss << "#enum";
            if ( enumType ) {
                ss << "#" << enumType->getMangledName();
            }
        } else if ( baseType==Type::tEnumeration8 ) {
            ss << "#enum8";
            if ( enumType ) {
                ss << "#" << enumType->getMangledName();
            }
        } else if ( baseType==Type::tEnumeration16 ) {
            ss << "#enum16";
            if ( enumType ) {
                ss << "#" << enumType->getMangledName();
            }
        } else if ( baseType==Type::tIterator ) {
            ss << "#iterator";
            if ( firstType ) {
                ss << "#" << firstType->getMangledName();
            }
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            ss << "#" << das_to_string(baseType);
            for ( auto & arg : argTypes ) {
                ss << "#" << arg->getMangledName();
            }
            for ( auto & arg : argNames ) {
                ss << "#`" << arg;
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
        } else if ( baseType==Type::tBitfield ) {
            ss << "#" << das_to_string(baseType);
            for ( auto & arg : argNames ) {
                ss << "#`" << arg;
            }
        } else {
            ss << das_to_string(baseType);
        }
        if ( constant )     ss << "#const";
        if ( ref )          ss << "#ref";
        if ( temporary )    ss << "#temporary";
        if ( implicit )     ss << "#implicit";
        if ( explicitConst )ss << "#explicitconst";
        if ( isExplicit )   ss << "#explicit";
        if ( dim.size() ) {
            for ( auto d : dim ) {
                ss << "#" << d;
            }
        }
        return ss.str();
    }

    bool TypeDecl::canBePlacedInContainer() const {
        das_set<Structure *> dep;
        return canBePlacedInContainer(dep);
    }

    bool TypeDecl::canBePlacedInContainer(das_set<Structure *> & dep) const {
        if ( baseType==Type::tHandle ) {
            return annotation->isLocal() && !annotation->hasNonTrivialCtor();
        } else if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return true;
                dep.insert(structType);
                return structType->canBePlacedInContainer(dep);
            }
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            for ( const auto & arg : argTypes ) {
                if ( !arg->canBePlacedInContainer(dep) ) {
                    return false;
                }
            }
            return true;
        } else if ( baseType==Type::tBlock ) {
            return false;
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            if ( firstType && !firstType->canBePlacedInContainer(dep) ) return false;
            if ( secondType && !secondType->canBePlacedInContainer(dep) ) return false;
        }
        return true;
    }

    bool TypeDecl::hasNonTrivialCtor() const {
        das_set<Structure *> dep;
        return hasNonTrivialCtor(dep);
    }

    bool TypeDecl::hasNonTrivialCtor(das_set<Structure *> & dep) const {
        if ( baseType==Type::tHandle ) {
            return annotation->hasNonTrivialCtor();
        } else if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return false;
                dep.insert(structType);
                return structType->hasNonTrivialCtor(dep);
            }
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            for ( const auto & arg : argTypes ) {
                if ( arg->hasNonTrivialCtor(dep) ) {
                    return true;
                }
            }
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            if ( firstType && firstType->hasNonTrivialCtor(dep) ) return true;
            if ( secondType && secondType->hasNonTrivialCtor(dep) ) return true;
        }
        return false;
    }

    bool TypeDecl::hasClasses() const {
        das_set<Structure *> dep;
        return hasClasses(dep);
    }

    bool TypeDecl::hasClasses(das_set<Structure *> & dep) const {
        if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return false;
                dep.insert(structType);
                return structType->hasClasses(dep);
            }
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            for ( const auto & arg : argTypes ) {
                if ( arg->hasClasses(dep) ) {
                    return true;
                }
            }
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            if ( firstType && firstType->hasClasses(dep) ) return true;
            if ( secondType && secondType->hasClasses(dep) ) return true;
        }
        return false;
    }

    bool TypeDecl::isLocal() const {
        das_set<Structure *> dep;
        return isLocal(dep);
    }

    bool TypeDecl::isLocal(das_set<Structure *> & dep) const {
        if ( baseType==Type::tHandle ) {
            return annotation->isLocal();
        } else  if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return true;
                dep.insert(structType);
                return structType->isLocal(dep);
            } else {
                return true;
            }
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            for ( const auto & arg : argTypes ) {
                if ( !arg->isLocal(dep) ) {
                    return false;
                }
            }
            return true;
        } else if ( baseType==Type::tBlock ) {
            return false;
        } else if ( baseType==Type::tArray || baseType==Type::tTable ) {
            if ( firstType && !firstType->isLocal(dep) ) return false;
            if ( secondType && !secondType->isLocal(dep) ) return false;
        }
        return true;
    }

    bool TypeDecl::isConst() const {
        return constant;
    }

    void TypeDecl::sanitize ( ) {
        isExplicit = false;
        if ( firstType ) firstType->sanitize();
        if ( secondType ) secondType->sanitize();
        for ( auto & argT : argTypes ) argT->sanitize();
    }

    bool TypeDecl::isSameType ( const TypeDecl & decl,
             RefMatters refMatters,
             ConstMatters constMatters,
             TemporaryMatters temporaryMatters,
             AllowSubstitute allowSubstitute,
             bool topLevel,
             bool isPassType ) const {
        if ( topLevel && !isRef() && !isPointer() ) {
            constMatters = ConstMatters::no;
        }
        if ( topLevel && !isTempType() ) {
            temporaryMatters = TemporaryMatters::no;
        }
        if ( baseType!=decl.baseType ) {
            return false;
        }
        if ( baseType==Type::tHandle && annotation!=decl.annotation ) {
            if ( !isExplicit && (allowSubstitute == AllowSubstitute::yes) ) {
                if ( annotation->canSubstitute(decl.annotation) ) {
                    return true;
                } else if ( decl.annotation->canBeSubstituted(annotation) ) {
                    return true;
                }
            }
            return false;
        }
        if ( baseType==Type::tStructure && structType!=decl.structType ) {
            if ( !isExplicit && (allowSubstitute == AllowSubstitute::yes) ) {
                if ( structType && decl.structType && structType->isCompatibleCast(*(decl.structType)) ){
                    return true;
                }
            }
            return false;
        }
        if ( baseType==Type::tPointer || baseType==Type::tIterator ) {
            if ( smartPtr != decl.smartPtr ) {
                return false;
            }
            bool iAmVoid = !firstType || firstType->isVoid();
            bool heIsVoid = !decl.firstType || decl.firstType->isVoid();
            TemporaryMatters tpm = implicit ? TemporaryMatters::no : TemporaryMatters::yes;
            if ( topLevel ) {
                ConstMatters pcm = ConstMatters::yes;
                if ( isPassType && firstType && firstType->constant ) {
                    pcm = ConstMatters::no;
                }
                if ( !iAmVoid && !heIsVoid &&
                        !firstType->isSameType(*decl.firstType,RefMatters::yes,pcm,
                            tpm, isExplicit ? AllowSubstitute::no : allowSubstitute,false) ) {
                    return false;
                }
            } else {
                if ( !iAmVoid && !heIsVoid ) {
                    if ( !firstType->isSameType(*decl.firstType,RefMatters::yes,ConstMatters::yes,
                                tpm, isExplicit ? AllowSubstitute::no : allowSubstitute,false) ) {
                        return false;
                    }
                } else {
                    return iAmVoid == heIsVoid;
                }
            }
        }
        if ( isEnumT() ) {
            if ( baseType != decl.baseType ) {
                return false;
            }
            if ( enumType && decl.enumType && enumType!=decl.enumType ) {
                return false;
            }
        }
        if ( baseType==Type::tArray ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType,RefMatters::yes,ConstMatters::yes,
                    TemporaryMatters::yes,AllowSubstitute::no,false) ) {
                return false;
            }
        }
        if ( baseType==Type::tTable ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType,RefMatters::yes,ConstMatters::yes,
                    TemporaryMatters::yes,AllowSubstitute::no,false) ) {
                return false;
            }
            if ( secondType && decl.secondType && !secondType->isSameType(*decl.secondType,RefMatters::yes,ConstMatters::yes,
                    TemporaryMatters::yes,AllowSubstitute::no,false) ) {
                return false;
            }
        }
        if ( baseType==Type::tBlock || baseType==Type::tFunction ||
            baseType==Type::tLambda || baseType==Type::tTuple ||
            baseType==Type::tVariant ) {
            if ( firstType && decl.firstType && !firstType->isSameType(*decl.firstType,RefMatters::yes,ConstMatters::yes,
                    TemporaryMatters::yes,AllowSubstitute::no,true) ) {
                return false;
            }
            if ( firstType || argTypes.size() ) {    // if not any block or any function
                if ( argTypes.size() != decl.argTypes.size() ) {
                    return false;
                }
                if (baseType == Type::tVariant) {
                    if (argNames.size() != decl.argNames.size()) {
                        return false;
                    }
                    for ( size_t i=0; i != argNames.size(); ++i ) {
                        const auto & arg = argNames[i];
                        const auto & declArg = decl.argNames[i];
                        if ( arg != declArg ) {
                            return false;
                        }
                    }
                }
                for ( size_t i=0; i != argTypes.size(); ++i ) {
                    const auto & arg = argTypes[i];
                    const auto & declArg = decl.argTypes[i];
                    if ( !arg->isSameType(*declArg, RefMatters::yes, ConstMatters::yes,
                            TemporaryMatters::yes,AllowSubstitute::no,true ) ) {
                        return false;
                    }
                }
            }
        }
        if ( dim!=decl.dim ) {
            return false;
        }
        if ( refMatters == RefMatters::yes ) {
            if ( ref!=decl.ref ) {
                return false;
            }
        }
        if ( constMatters == ConstMatters::yes ) {
            if ( constant!=decl.constant ) {
                return false;
            }
        }
        if ( temporaryMatters == TemporaryMatters::yes ) {
            if ( temporary != decl.temporary ) {
                return false;
            }
        }
        return true;
    }

    bool TypeDecl::canWrite() const {
        bool cw = isRef() || baseType==Type::tPointer || baseType==Type::anyArgument;
        if ( baseType!=Type::tPointer ) {
            cw &= !constant;
        } else if ( firstType ) {
            cw &= !firstType->constant;
        }
        return cw;
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

    bool TypeDecl::isGoodVariantType() const {
        return baseType==Type::tVariant && dim.size()==0;
    }

    bool TypeDecl::isGoodTableType() const {
        return baseType==Type::tTable && dim.size()==0 && firstType && secondType;
    }

    bool TypeDecl::isVoid() const {
        return (baseType==Type::tVoid) && (dim.size()==0);
    }

    bool TypeDecl::isPointer() const {
        return (baseType==Type::tPointer) && (dim.size()==0);
    }

    bool TypeDecl::isVoidPointer() const {
        return isPointer() && (!firstType || firstType->isVoid());
    }

    bool TypeDecl::isBitfield() const {
        return (baseType==Type::tBitfield) && (dim.size()==0);
    }

    bool TypeDecl::isIterator() const {
        return (baseType==Type::tIterator) && (dim.size()==0);
    }

    bool TypeDecl::isEnumT() const {
        return (baseType==Type::tEnumeration) || (baseType==Type::tEnumeration8)
            || (baseType==Type::tEnumeration16);
    }

    bool TypeDecl::isEnum() const {
        return isEnumT() && (dim.size()==0);
    }

    void TypeDecl::collectAliasList(vector<string> & aliases) const {
        // auto is auto.... or auto....?
        if ( baseType==Type::alias ) {
            aliases.push_back(alias);
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                firstType->collectAliasList(aliases);
        } else  if ( baseType==Type::tIterator ) {
            if ( firstType )
                firstType->collectAliasList(aliases);
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                firstType->collectAliasList(aliases);
        } else if ( baseType==Type::tTable ) {
            if ( firstType )
                firstType->collectAliasList(aliases);
            if ( secondType )
                secondType->collectAliasList(aliases);
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            if ( firstType )
                firstType->collectAliasList(aliases);
            for ( auto & arg : argTypes )
                arg->collectAliasList(aliases);
        }
    }

    bool TypeDecl::isAotAlias () const {
        if ( aotAlias ) {
            return true;
        } else  if ( baseType==Type::tPointer ) {
            if ( firstType )
                return firstType->isAotAlias();
        } else  if ( baseType==Type::tIterator ) {
            if ( firstType )
                return firstType->isAotAlias();
        } else if ( baseType==Type::tArray ) {
            if ( firstType )
                return firstType->isAotAlias();
        } else if ( baseType==Type::tTable ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAotAlias();
            if ( secondType )
                any |= secondType->isAotAlias();
            return any;
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAotAlias();
            for ( auto & arg : argTypes )
                any |= arg->isAotAlias();
            return any;
        }
        return false;
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAlias();
            for ( auto & arg : argTypes )
                any |= arg->isAlias();
            return any;
        }
        return false;
    }

    bool TypeDecl::isAutoArrayResolved() const {
        for ( auto di : dim ) {
            if ( di==TypeDecl::dimAuto ) {
                return false;
            }
        }
        return true;
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
        } else  if ( baseType==Type::tIterator ) {
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
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            bool any = false;
            if ( firstType )
                any |= firstType->isAuto();
            for ( auto & arg : argTypes )
                any |= arg->isAuto();
            return any;
        }
        return false;
    }

    bool TypeDecl::isAutoOrAlias() const {
        // auto is auto.... or auto....?
        // also dim[] is aito
        for (auto di : dim) {
            if (di == TypeDecl::dimAuto) {
                return true;
            }
        }
        if (baseType == Type::autoinfer) {
            return true;
        } if (baseType == Type::alias) {
            return true;
        } else  if (baseType == Type::tPointer) {
            if (firstType)
                return firstType->isAutoOrAlias();
        } else  if (baseType == Type::tIterator) {
            if (firstType)
                return firstType->isAutoOrAlias();
        } else if (baseType == Type::tArray) {
            if (firstType)
                return firstType->isAutoOrAlias();
        } else if (baseType == Type::tTable) {
            bool any = false;
            if (firstType)
                any |= firstType->isAutoOrAlias();
            if (secondType)
                any |= secondType->isAutoOrAlias();
            return any;
        } else if (baseType == Type::tBlock || baseType == Type::tFunction ||
            baseType == Type::tLambda || baseType == Type::tTuple ||
            baseType == Type::tVariant) {
            bool any = false;
            if (firstType)
                any |= firstType->isAutoOrAlias();
            for (auto& arg : argTypes)
                any |= arg->isAutoOrAlias();
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
            case Type::tBitfield:
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
            case Type::tEnumeration8:
            case Type::tEnumeration16:
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

    bool TypeDecl::isClass() const {
        return isStructure() && structType && structType->isClass;
    }

    bool TypeDecl::isTuple() const {
        return (baseType==Type::tTuple) && (dim.size()==0);
    }

    bool TypeDecl::isFunction() const {
        return (baseType==Type::tFunction) && (dim.size()==0);
    }

    bool TypeDecl::isVariant() const {
        return (baseType==Type::tVariant) && (dim.size()==0);
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
            case Type::tBitfield:
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
            case Type::tBitfield:
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
                return true;
            case Type::tPointer:
                return !smartPtr;
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
            case Type::tBitfield:
            case Type::tFloat:
            case Type::tDouble:
            case Type::tString:
                return true;
            case Type::tPointer:
                return !smartPtr;
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
            case Type::tEnumeration8:
            case Type::tEnumeration16:
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
            case Type::tBitfield:
            case Type::tFloat:
            case Type::tDouble:
            case Type::tPointer:
            case Type::tFunction:
            case Type::tLambda:
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

    Type TypeDecl::getRangeBaseType() const {
        switch ( baseType ) {
            case Type::tRange:  return Type::tInt;
            case Type::tURange: return Type::tUInt;
            default:
                DAS_ASSERTF(0, "we should not even be here. we are calling getRangeBaseType on an unsuppored baseType."
                       "likely new range type been added.");
                return Type::none;
        }
    }

    bool TypeDecl::isRange() const {
        return (baseType==Type::tRange || baseType==Type::tURange) && dim.size()==0;
    }

    bool TypeDecl::isString() const {
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
        return baseType==Type::tTuple || baseType==Type::tVariant ||
                baseType==Type::tStructure || baseType==Type::tArray ||
                baseType==Type::tTable || baseType==Type::tBlock ||
                baseType==Type::tIterator
                || dim.size();
    }

    bool TypeDecl::isTempType(bool refMatters) const {
        return (ref && refMatters) || isRefType() || isPointer() || isString() || baseType==Type::tIterator;
    }

    bool TypeDecl::isTemp( bool topLevel, bool refMatters ) const {
        das_set<Structure *> dep;
        return isTemp(topLevel, refMatters, dep);
    }

    bool TypeDecl::isTemp( bool topLevel, bool refMatters, das_set<Structure *> & dep ) const {
        if ( topLevel && !isTempType(refMatters) ) {
            return false;
        } else if ( temporary ) {
            return true;
        } else if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return false;
                dep.insert(structType);
                return structType->isTemp(dep);
            } else {
                return false;
            }
        } else if ( baseType==Type::tPointer || baseType==Type::tIterator ) {
            return firstType ? firstType->isTemp(false, true, dep) : false;
        } else if ( baseType==Type::tArray ) {
            return firstType ? firstType->isTemp(false, true, dep) : false;
        } else if ( baseType==Type::tTable ) {
            if ( firstType && firstType->isTemp(false, true, dep) ) {
                return true;
            } else if ( secondType && secondType->isTemp(false, true, dep) ) {
                return true;
            }
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction ||
                   baseType==Type::tLambda || baseType==Type::tTuple ||
                   baseType==Type::tVariant ) {
            if ( firstType && firstType->isTemp(true, true, dep) ) {
                return true;
            }
            for ( const auto & argT : argTypes ) {
                if ( argT->isTemp(true, true, dep) ) {
                    return true;
                }
            }
        }
        return false;
    }

    bool TypeDecl::isShareable() const {
        das_set<Structure *> dep;
        return isShareable(dep);
    }

    bool TypeDecl::isShareable( das_set<Structure *> & dep ) const {
        if ( baseType==Type::tIterator || baseType==Type::tBlock || baseType==Type::tLambda ) {
            return false;
        } else if ( baseType==Type::tHandle ) {
            return annotation ? annotation->isShareable() : true;
        } else if ( baseType==Type::tStructure ) {
            if (structType) {
                if (dep.find(structType) != dep.end()) return true;
                dep.insert(structType);
                return structType->isShareable(dep);
            } else {
                return true;
            }
        } else if ( baseType==Type::tArray ) {
            return firstType ? firstType->isShareable(dep) : true;
        } else if ( baseType==Type::tTable ) {
            if ( firstType && !firstType->isShareable(dep) ) {
                return false;
            } else if ( secondType && !secondType->isShareable(dep) ) {
                return false;
            } else {
                return true;
            }
        } else if ( baseType==Type::tTuple || baseType==Type::tVariant ) {
            for ( const auto & argT : argTypes ) {
                if ( !argT->isShareable(dep) ) {
                    return false;
                }
            }
            return true;
        } else {
            return true;
        }
    }

    bool TypeDecl::isInteger() const {
        if (dim.size() != 0) return false;
        switch (baseType) {
        case Type::tInt:
        case Type::tUInt:
        case Type::tBitfield:
        case Type::tInt8:
        case Type::tUInt8:
        case Type::tInt16:
        case Type::tUInt16:
        case Type::tInt64:
        case Type::tUInt64:
            return true;
        default:;
        }
        return false;
    }


    bool TypeDecl::isNumeric() const {
        if (dim.size() != 0) return false;
        switch (baseType) {
        case Type::tInt:
        case Type::tUInt:
        case Type::tBitfield:
        case Type::tInt8:
        case Type::tUInt8:
        case Type::tInt16:
        case Type::tUInt16:
        case Type::tInt64:
        case Type::tUInt64:
        case Type::tFloat:
        case Type::tDouble:
            return true;
        default:;
        }
        return false;
    }

    bool TypeDecl::isNumericComparable() const {
        if (dim.size() != 0) return false;
        switch (baseType) {
        case Type::tInt:
        case Type::tUInt:
        case Type::tBitfield:
        case Type::tInt64:
        case Type::tUInt64:
        case Type::tFloat:
        case Type::tDouble:
            return true;
        default:;
        }
        return false;
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

    int TypeDecl::getVariantFieldOffset ( int index ) const {
        DAS_ASSERT(baseType==Type::tVariant);
        DAS_ASSERT(index>=0 && index<int(argTypes.size()));
        int al = getVariantAlign() - 1;
        int offset = (getTypeBaseSize(Type::tInt) + al) & ~al;
        return offset;
    }

    int TypeDecl::getVariantUniqueFieldIndex ( const TypeDeclPtr & uniqueType ) const {
        int index = -1;
        int idx = 0;
        for ( const auto & argT : argTypes ) {
            if ( argT->isSameType(*uniqueType, RefMatters::no, ConstMatters::no, TemporaryMatters::no, AllowSubstitute::yes) ) {
                if ( index != -1 ) {
                    return -1;
                } else {
                    index = idx;
                }
            }
            idx ++;
        }
        return index;
    }

    int TypeDecl::getVariantSize() const {
        DAS_ASSERT(baseType==Type::tVariant);
        int maxSize = 0;
        int al = getVariantAlign() - 1;
        for ( const auto & argT : argTypes ) {
            int size = (getTypeBaseSize(Type::tInt) + al) & ~al;
            size += argT->getSizeOf();
            maxSize = das::max(size, maxSize);
        }
        maxSize = (maxSize + al) & ~al;
        return maxSize;
    }

    int TypeDecl::getVariantAlign() const {
        DAS_ASSERT(baseType==Type::tVariant);
        int align = getTypeBaseAlign(Type::tInt);
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
        } else if ( baseType==Type::tVariant ) {
            return getVariantSize();
        } else if ( isEnumT() ) {
            return enumType ? getTypeBaseSize(enumType->baseType) : getTypeBaseSize(Type::tInt);
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
        } else if ( baseType==Type::tVariant ) {
            return getVariantAlign();
        } else if ( isEnumT() ) {
            return enumType ? getTypeBaseAlign(enumType->baseType) : getTypeBaseAlign(Type::tInt);
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
            for ( size_t i=1; i!=dim.size(); ++i )
                size *= dim[i];
        }
        return getBaseSizeOf() * size;
    }

    int TypeDecl::findArgumentIndex( const string & name ) const {
        for (int index = 0; index != int(argNames.size()); ++index) {
            if (argNames[index] == name) return index;
        }
        return -1;
    }

    string TypeDecl::findBitfieldName ( uint32_t val ) const {
       if ( argNames.size() ) {
            if ( val && (val & (val-1))==0 ) {  // if bit is set, and only one bit
                int index = 31 - __builtin_clz(val);
                if ( index < int(argNames.size()) ) {
                    return argNames[index];
                }
            }
        }
        return "";
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
        }  else if ( type->baseType==Type::tTuple || type->baseType==Type::tVariant ) {
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

    void TypeDecl::addVariant(const string & name, const TypeDeclPtr & tt) {
        DAS_ASSERT(find(argNames.begin(), argNames.end(), name) == argNames.end() && "duplicate variant");
        argNames.push_back(name);
        argTypes.push_back(tt);
    }
}
