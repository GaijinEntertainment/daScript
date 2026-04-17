#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"


namespace das {

    // function

    AnnotationList cloneAnnotationList ( const AnnotationList & list ) {
        AnnotationList clist;
        for ( auto & ann : list ) {
            auto decl = new AnnotationDeclaration();
            decl->annotation = ann->annotation;
            decl->arguments = ann->arguments;
            decl->at = ann->at;
            clist.push_back(decl);
        }
        return clist;
    }

    FunctionPtr Function::clone() const {
        auto cfun = new Function();
        cfun->name = name;
        for ( const auto & arg : arguments ) {
            cfun->arguments.push_back(arg->clone());
        }
        cfun->annotations = cloneAnnotationList(annotations);
        cfun->result = new TypeDecl(*result);
        cfun->body = body->clone();
        cfun->index = -1;
        cfun->totalStackSize = 0;
        cfun->totalGenLabel = totalGenLabel;
        cfun->at = at;
        cfun->atDecl = atDecl;
        cfun->module = nullptr;
        cfun->flags = flags;
        cfun->moreFlags = moreFlags;
        cfun->sideEffectFlags = sideEffectFlags;
        cfun->inferStack = inferStack;
        cfun->classParent = classParent;
        // copy aliasing info
        cfun->resultAliases = resultAliases;
        cfun->argumentAliases = argumentAliases;
        cfun->resultAliasesGlobals = resultAliasesGlobals;
        return cfun;
    }

    LineInfo Function::getConceptLocation(const LineInfo & atL) const {
        return inferStack.size() ? inferStack[0].at : atL;
    }

    string Function::getLocationExtra() const {
        if ( !inferStack.size() ) {
            return "";
        }
        TextWriter ss;
        ss << "\nwhile compiling " << describe() << "\n";
        for ( const auto & ih : inferStack ) {
            ss << "instanced from " << ih.func->describe() << " at " << ih.at.describe() << "\n";
        }
        return ss.str();
    }

    string Function::describeName(DescribeModule moduleName) const {
        TextWriter ss;
        if ( moduleName==DescribeModule::yes && module && !module->name.empty() ) {
            ss << module->name << "::";
        }
        if ( !isalpha(name[0]) && name[0]!='_' && name[0]!='`' ) {
            ss << "operator ";
        }
        ss << name;
        return ss.str();
    }

    string Function::describe(DescribeModule moduleName, DescribeExtra extra) const {
        TextWriter ss;
        if ( moduleName==DescribeModule::yes && module && !module->name.empty() ) {
            ss << module->name << "::";
        }
        if ( !isalpha(name[0]) && name[0]!='_' && name[0]!='`' ) {
            ss << "operator ";
        }
        ss << name;
        if ( arguments.size() ) {
            ss << "(";
            for ( auto & arg : arguments ) {
                ss << arg->name << ": " << *arg->type;
                if ( extra==DescribeExtra::yes && arg->init ) {
                    ss << " = " << *arg->init;
                }
                if ( arg != arguments.back() ) ss << "; ";
            }
            ss << ")";
        }
        if ( result ) {
            ss << ": " << result->describe();
        }
        return ss.str();
    }

    string Function::getMangledName() const {
        TextWriter ss;
        getMangledName(ss);
        return ss.str();
    }

    void Function::getMangledName(TextWriter & ss) const {
        if ( module && !module->name.empty() ) {
            ss << "@" << module->name << "::";
        }
        ss << name;
        for ( auto & arg : arguments ) {
            ss << " ";
            arg->type->getMangledName(ss);
        }
        for ( auto & ann : annotations ) {
            if (ann->annotation && ann->annotation->rtti_isFunctionAnnotation() ) {
                auto fna = static_cast<FunctionAnnotation*>(ann->annotation);
                string mname;
                fna->appendToMangledName((Function *)this, *ann, mname);
                if ( !mname.empty() ) {
                    ss << " %<" << mname << ">";
                }
            }
        }
    }

    uint64_t Function::getMangledNameHash() const {
        auto mangledName = getMangledName();
        return hash_blockz64((uint8_t *)mangledName.c_str());
    }

    VariablePtr Function::findArgument(const string & na) {
        for ( auto & arg : arguments ) {
            if ( arg->name==na ) {
                return arg;
            }
        }
        return nullptr;
    }

    FunctionPtr Function::visit(Visitor & vis) {
        vis.preVisit(this);
        for ( auto & arg : arguments ) {
            vis.preVisitArgument(this, arg, arg==arguments.back() );
            if ( arg->type ) {
                vis.preVisit(arg->type);
                arg->type = arg->type->visit(vis);
                arg->type = vis.visit(arg->type);
            }
            if ( arg->init && vis.canVisitArgumentInit(this,arg,arg->init) ) {
                vis.preVisitArgumentInit(this, arg, arg->init);
                arg->init = arg->init->visit(vis);
                if ( arg->init ) {
                    arg->init = vis.visitArgumentInit(this, arg, arg->init);
                }
            }
            arg = vis.visitArgument(this, arg, arg==arguments.back() );
        }
        if ( result ) {
            vis.preVisit(result);
            result = result->visit(vis);
            result = vis.visit(result);
        }
        if ( body ) {
            vis.preVisitFunctionBody(this, body);
            if ( body ) body = body->visit(vis);
            if ( body ) body = vis.visitFunctionBody(this, body);
        }
        return vis.visit(this);
    }

    bool Function::isGeneric() const {
        for ( const auto & ann : annotations ) {
            if (ann->annotation) {
                auto fna = static_cast<FunctionAnnotation*>(ann->annotation);
                if (fna->isGeneric()) {
                    return true;
                }
            }
        }
        for ( auto & arg : arguments ) {
            if ( arg->type->isAuto() && !arg->init ) {
                return true;
            }
        }
        return false;
    }

    string Function::getAotArgumentPrefix(ExprCallFunc * call, int argIndex) const {
        for ( auto & ann : annotations ) {
            if ( ann->annotation->rtti_isFunctionAnnotation() ) {
                auto pAnn = static_cast<FunctionAnnotation*>(ann->annotation);
                return pAnn->aotArgumentPrefix(call, argIndex);
            }
        }
        return "";
    }

    string Function::getAotArgumentSuffix(ExprCallFunc * call, int argIndex) const {
        for ( auto & ann : annotations ) {
            if ( ann->annotation->rtti_isFunctionAnnotation() ) {
                auto pAnn = static_cast<FunctionAnnotation*>(ann->annotation);
                return pAnn->aotArgumentSuffix(call, argIndex);
            }
        }
        return "";
    }

    string Function::getAotName(ExprCallFunc * call) const {
        for ( auto & ann : annotations ) {
            if ( ann->annotation->rtti_isFunctionAnnotation() ) {
                auto pAnn = static_cast<FunctionAnnotation*>(ann->annotation);
                return pAnn->aotName(call);
            }
        }
        return call->func->getAotBasicName();
    }

    FunctionPtr Function::setSideEffects ( SideEffects seFlags ) {
        sideEffectFlags = uint32_t(seFlags) & ~uint32_t(SideEffects::unsafe);
        if ( uint32_t(seFlags) & uint32_t(SideEffects::unsafe) ) {
            unsafeOperation = true;
        }
        return this;
    }

    FunctionPtr Function::setCustomProperty() {
        isCustomProperty = true;
        return this;
    }

    FunctionPtr Function::setAotTemplate() {
        aotTemplate = true;
        return this;
    }

    FunctionPtr Function::setAnyTemplate() {
        anyTemplate = true;
        return this;
    }

    FunctionPtr Function::setTempResult() {
        result->temporary = true;
        return this;
    }

    FunctionPtr Function::setCaptureString() {
        captureString = true;
        return this;
    }

    FunctionPtr Function::setNoDiscard() {
        nodiscard = true;
        return this;
    }

    FunctionPtr Function::addToModule ( Module & mod, SideEffects seFlags ) {
        setSideEffects(seFlags);
        if (!mod.addFunction(this)) {
            DAS_FATAL_ERROR("addExtern(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
        }
        return this;
    }

    Function * Function::getOriginPtr() const {
        if ( fromGeneric ) {
            auto origin = fromGeneric;
            while ( origin->fromGeneric ) {
                origin = origin->fromGeneric;
            }
            return origin;
        } else {
            return nullptr;
        }
    }

    FunctionPtr Function::getOrigin() const {
        if ( fromGeneric ) {
            auto origin = fromGeneric;
            while ( origin->fromGeneric ) {
                origin = origin->fromGeneric;
            }
            return origin;
        } else {
            return nullptr;
        }
    }

    // built-in function

    BuiltInFunction::BuiltInFunction ( const char * fn, const char * fnCpp ) {
        builtIn = true;
        name = fn;
        cppName = fnCpp ? fnCpp : "";
    }

    void BuiltInFunction::construct (const vector<TypeDeclPtr> & args ) {
        this->totalStackSize = sizeof(Prologue);
        for ( size_t argi=1; argi != args.size(); ++argi ) {
            auto arg = new Variable();
            arg->name = "arg" + to_string(argi-1);
            arg->type = args[argi];
            if ( arg->type->baseType==Type::fakeContext ) {
                arg->init = new ExprFakeContext(at);
                arg->init->generated = true;
            } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                arg->init = new ExprFakeLineInfo(at);
                arg->init->generated = true;
            }
            if ( arg->type->isTempType() ) {
                arg->type->implicit = true;
            }
            this->arguments.push_back(arg);
        }
        result = args[0];
        if ( result->isRefType() && !result->ref ) {
            if ( result->canCopy() ) {
                copyOnReturn = true;
                moveOnReturn = false;
            } else if ( result->canMove() ) {
                copyOnReturn = false;
                moveOnReturn = true;
            } else if ( result->ref ) {
                // its ref, so its fine
            } else if ( result->hasNonTrivialCtor() ) {
                // we can initialize it locally
            } else {
                DAS_FATAL_ERROR("BuiltInFn %s can't be bound. It returns values which can't be copied or moved\n", name.c_str());
            }
        } else {
            copyOnReturn = false;
            moveOnReturn = false;
        }
    }

    void BuiltInFunction::constructExternal (const vector<TypeDeclPtr> & args ) {
        this->totalStackSize = sizeof(Prologue);
        for ( size_t argi=1; argi != args.size(); ++argi ) {
            auto arg = new Variable();
            arg->name = "arg" + to_string(argi-1);
            arg->type = args[argi];
            if ( arg->type->baseType==Type::fakeContext ) {
                arg->init = new ExprFakeContext(at);
                arg->init->generated = true;
            } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                arg->init = new ExprFakeLineInfo(at);
                arg->init->generated = true;
            }
            this->arguments.push_back(arg);
        }
        result = args[0];
        if ( result->isRefType() && !result->ref ) {
            if ( result->canCopy() ) {
                copyOnReturn = true;
                moveOnReturn = false;
            } else if ( result->canMove() ) {
                copyOnReturn = false;
                moveOnReturn = true;
            } else if ( !result->ref ) {
                DAS_FATAL_ERROR("ExternalFn %s can't be bound. It returns values which can't be copied or moved\n", name.c_str());
            }
        } else {
            copyOnReturn = false;
            moveOnReturn = false;
        }
    }

    void BuiltInFunction::constructInterop (const vector<TypeDeclPtr> & args ) {
        this->totalStackSize = sizeof(Prologue);
        for ( size_t argi=1; argi!=args.size(); ++argi ) {
            auto arg = new Variable();
            arg->name = "arg" + to_string(argi-1);
            arg->type = args[argi];
            if ( arg->type->baseType==Type::fakeContext ) {
                arg->init = new ExprFakeContext(at);
                arg->init->generated = true;
            } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                arg->init = new ExprFakeLineInfo(at);
                arg->init->generated = true;
            }
            this->arguments.push_back(arg);
        }
        result = args[0];
        if ( result->isRefType() && !result->ref ) {
            if ( result->canCopy() ) {
                copyOnReturn = true;
                moveOnReturn = false;
            } else if ( result->canMove() ) {
                copyOnReturn = false;
                moveOnReturn = true;
            } else if ( !result->ref ) {
                DAS_FATAL_ERROR("InteropFn %s can't be bound. It returns values which can't be copied or moved\n", name.c_str());
            }
        } else {
            copyOnReturn = false;
            moveOnReturn = false;
        }
    }

}