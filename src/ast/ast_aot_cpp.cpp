#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

#include "daScript/misc/enums.h"

namespace das {

    Enum<Type> g_cppCTypeTable = {
        {   Type::autoinfer,    "autoinfer"  },
        {   Type::alias,        "alias"  },
        {   Type::anyArgument,  "anyArgument"  },
        {   Type::tVoid,        "tVoid"  },
        {   Type::tBool,        "tBool"  },
        {   Type::tInt64,       "tInt64"  },
        {   Type::tUInt64,      "tUInt64"  },
        {   Type::tString,      "tString" },
        {   Type::tPointer,     "tPointer" },
        {   Type::tEnumeration, "tEnumeration" },
        {   Type::tIterator,    "tIterator" },
        {   Type::tArray,       "tArray" },
        {   Type::tTable,       "tTable" },
        {   Type::tInt,         "tInt"   },
        {   Type::tInt2,        "tInt2"  },
        {   Type::tInt3,        "tInt3"  },
        {   Type::tInt4,        "tInt4"  },
        {   Type::tUInt,        "tUInt"  },
        {   Type::tUInt2,       "tUInt2" },
        {   Type::tUInt3,       "tUInt3" },
        {   Type::tUInt4,       "tUInt4" },
        {   Type::tFloat,       "tFloat" },
        {   Type::tFloat2,      "tFloat2"},
        {   Type::tFloat3,      "tFloat3"},
        {   Type::tFloat4,      "tFloat4"},
        {   Type::tDouble,      "tDouble" },
        {   Type::tRange,       "tRange" },
        {   Type::tURange,      "tURange"},
        {   Type::tBlock,       "tBlock"},
        {   Type::tFunction,    "tFunction"},
        {   Type::tLambda,      "tLambda"}
    };

    Enum<Type> g_cppTypeTable = {
        {   Type::tVoid,        "void"     },
        {   Type::tBool,        "bool"     },
        {   Type::tInt64,       "int64_t"  },
        {   Type::tUInt64,      "uint64_t" },
        {   Type::tString,      "char *"   },
        {   Type::tInt,         "int32_t"  },
        {   Type::tInt2,        "int2"     },
        {   Type::tInt3,        "int3"     },
        {   Type::tInt4,        "int4"     },
        {   Type::tUInt,        "uint32_t" },
        {   Type::tUInt2,       "uint2"    },
        {   Type::tUInt3,       "uint3"    },
        {   Type::tUInt4,       "uint4"    },
        {   Type::tFloat,       "float"    },
        {   Type::tFloat2,      "float2"   },
        {   Type::tFloat3,      "float3"   },
        {   Type::tFloat4,      "float4"   },
        {   Type::tDouble,      "double"   },
        {   Type::tRange,       "range"    },
        {   Type::tURange,      "urange"   },
        {   Type::tBlock,       "Block"    },
        {   Type::tFunction,    "Func"     },
        {   Type::tLambda,      "Lambda"   }
    };

    string das_to_cppString ( Type t ) {
        return g_cppTypeTable.find(t);
    }

    string das_to_cppCTypeString ( Type t ) {
        return g_cppCTypeTable.find(t);
    }

    string describeCppType ( const TypeDeclPtr & type ) {
        TextWriter stream;
        auto baseType = type->baseType;
        if ( type->constant ) {
            stream << "const ";     // TODO: do we skip it alltogether?
        }
        if ( type->dim.size() ) {
            stream << "TDim< ";
        }
        if ( baseType==Type::alias ) {
            stream << "/* alias */";
        } else if ( baseType==Type::autoinfer ) {
            stream << "/* auto";
            if ( !type->alias.empty() ) {
                stream << " (" << type->alias << ")";
            }
            stream << " */";
        } else if ( baseType==Type::tHandle ) {
            stream << "/* handled */ " << type->annotation->name;
        } else if ( baseType==Type::tArray ) {
            if ( type->firstType ) {
                stream << "TArray<" << describeCppType(type->firstType) << ">";
            } else {
                stream << "Array";
            }
        } else if ( baseType==Type::tTable ) {
            if ( type->firstType && type->secondType ) {
                stream << "TTable<" << describeCppType(type->firstType) << "," << describeCppType(type->secondType) << ">";
            } else {
                stream << "Table";
            }
        } else if ( baseType==Type::tStructure ) {
            if ( type->structType ) {
                stream << "struct " << type->structType->name;
            } else {
                stream << "/* unspecified structure */";
            }
        } else if ( baseType==Type::tPointer ) {
            if ( type->firstType ) {
                stream << describeCppType(type->firstType) << " *";
            } else {
                stream << "void *";
            }
        } else if ( baseType==Type::tEnumeration ) {
            if ( type->enumType ) {
                stream << "enum class " << type->enumType->name;
            } else {
                stream << "/* unspecified enumeration */";
            }
        } else if ( baseType==Type::tIterator ) {
            if ( type->firstType ) {
                stream << "TIterator<" << describeCppType(type->firstType) << ">";
            } else {
                stream << "Iterator";
            }
        } else if ( baseType==Type::tBlock || baseType==Type::tFunction || baseType==Type::tLambda ) {
            stream << "T" << das_to_cppString(baseType) << "<";
            if ( type->firstType ) {
                stream << describeCppType(type->firstType);
            } else {
                stream << "void";
            }
            if ( type->argTypes.size() ) {
                for ( const auto & arg : type->argTypes ) {
                    stream << "," << describeCppType(arg);
                }
            }
            stream << ">";
        }  else {
            stream << das_to_cppString(baseType);
        }
        if ( type->dim.size() ) {
            for ( auto d : type->dim ) {
                stream << "," << d;
            }
            stream << " >";
        }
        if ( type->ref ) {
            stream << " &";
        }
        return stream.str();
    }

    void describeCppStructInfo ( TextWriter & ss, StructInfo * info ) {
    }

    void describeCppEnumInfo ( TextWriter & ss, EnumInfo * info ) {
    }

    void describeCppTypeInfo ( TextWriter & ss, TypeInfo * info ) {
        ss << "{ Type::" << das_to_cppCTypeString(info->type) << ", ";
        if ( info->structType ) {
            describeCppStructInfo(ss, info->structType);
        } else {
            ss << "nullptr";
        }
        ss << ", ";
        if ( info->enumType ) {
            describeCppEnumInfo(ss, info->enumType);
        } else {
            ss << "nullptr";
        }
        ss << ", /*annotation*/ nullptr";
        ss << ", ";
        if ( info->firstType ) {
            describeCppTypeInfo(ss, info->firstType);
        } else {
            ss << "nullptr";
        }
        ss << ", ";
        if ( info->secondType ) {
            describeCppTypeInfo(ss, info->secondType);
        } else {
            ss << "nullptr";
        }
        ss << ", " << info->dimSize;
        ss << ", ";
        if ( info->dimSize ) {
            ss << "{";
            for ( uint32_t i=0; i!=info->dimSize; ++i ) {
                if ( i ) ss << ",";
                ss << info->dim[i];
            }
            ss << "}";
        } else {
            ss << "nullptr";
        }
        ss << ", " << info->flags;
        ss << ", 0x" << HEX << info->hash << DEC;
        ss << " }";
    }

    string describeCppTypeInfo ( TypeInfo * info ) {
        TextWriter ss;
        describeCppTypeInfo(ss,info);
        return ss.str();
    }

    class CppAot : public Visitor {
    public:
        CppAot () {}
        string str() const { return sti.str() + ss.str(); };
    protected:
        TextWriter ss, sti;
        int        lastNewLine = -1;
        int        tab = 0;
        int        debugInfoGlobal = 0;
    protected:
        void newLine () {
            auto nlPos = ss.tellp();
            if ( nlPos != lastNewLine ) {
                ss << "\n";
                lastNewLine = ss.tellp();
            }
        }
        __forceinline static bool noBracket ( Expression * expr ) {
            return expr->topLevel || expr->bottomLevel || expr->argLevel;
        }
    protected:
    // function
        virtual void preVisit ( Function * fn) override {
            Visitor::preVisit(fn);
            ss << "\n" << describeCppType(fn->result) << " " << fn->name << " ( Context * __context__";
        }
        virtual void preVisitFunctionBody ( Function * fn,Expression * expr ) override {
            Visitor::preVisitFunctionBody(fn,expr);
            ss << " )\n";
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & arg, bool last ) override {
            Visitor::preVisitArgument(fn,arg,last);
            ss << ", " << describeCppType(arg->type) << " " << arg->name;
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & arg, Expression * expr ) override {
            Visitor::preVisitArgumentInit(fn,arg,expr);
            ss << " = ";
        }
        virtual VariablePtr visitArgument ( Function * fn, const VariablePtr & that, bool last ) override {
            // if ( !last ) ss << "; ";
            return Visitor::visitArgument(fn, that, last);
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            ss << "\n";
            return Visitor::visit(fn);
        }
    // block
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            ss << "{\n";
            tab ++;
        }
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockExpression(block, expr);
            ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockExpression ( ExprBlock * block, Expression * that ) override {
            ss << ";"; newLine();
            return Visitor::visitBlockExpression(block, that);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            tab --;
            ss << string(tab,'\t') << "}";
            return Visitor::visit(block);
        }
        virtual void preVisitBlockFinal ( ExprBlock * block ) override {
            Visitor::preVisitBlockFinal(block);
            ss << string(tab-1,'\t') << "/* finally */\n";
        }
        virtual void preVisitBlockFinalExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockFinalExpression(block, expr);
            ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockFinalExpression ( ExprBlock * block, Expression * that ) override {
            ss << ";"; newLine();
            return Visitor::visitBlockFinalExpression(block, that);
        }
    // let
        virtual void preVisit ( ExprLet * let ) override {
            Visitor::preVisit(let);
            if ( let->subexpr ) {
                ss << "{ /* let */\n";
                tab ++;
            }
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            ss << describeCppType(var->type) << " " << var->name;
        }
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( let->subexpr ) {
                tab --;
                ss << string(tab,'\t') << "}";
            }
            return Visitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * expr ) override {
            Visitor::preVisitLetInit(let,var,expr);
            ss << " = ";
        }
    // copy
        virtual void preVisitRight ( ExprCopy * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << " = ";
        }
    // move
        virtual void preVisit ( ExprMove * that ) override {
            Visitor::preVisit(that);
            ss << "das_move(";
        }
        virtual void preVisitRight ( ExprMove * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << ",";
        }
        virtual ExpressionPtr visit ( ExprMove * that ) override {
            ss << ")";
            return Visitor::visit(that);
        }
    // op1
        virtual void preVisit ( ExprOp1 * that ) override {
            Visitor::preVisit(that);
            if ( that->op!="+++" && that->op!="---" ) {
                ss << that->op;
            }
            if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << "(";
        }
        virtual ExpressionPtr visit ( ExprOp1 * that ) override {
            if ( that->op=="+++" || that->op=="---" ) {
                ss << that->op[0] << that->op[1];
            }
            if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << ")";
            return Visitor::visit(that);
        }
    // op2
        virtual void preVisit ( ExprOp2 * that ) override {
            Visitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitRight ( ExprOp2 * op2, Expression * right ) override {
            Visitor::preVisitRight(op2,right);
            ss << " " << op2->op << " ";
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return Visitor::visit(that);
        }
    // op3
        virtual void preVisit ( ExprOp3 * that ) override {
            Visitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitLeft  ( ExprOp3 * that, Expression * left ) override {
            Visitor::preVisitLeft(that,left);
            ss << " ? ";
        }
        virtual void preVisitRight ( ExprOp3 * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << " : ";
        }
        virtual ExpressionPtr visit ( ExprOp3 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return Visitor::visit(that);
        }
    // return
        virtual void preVisit ( ExprReturn * expr ) override {
            Visitor::preVisit(expr);
            ss << "return ";
            if ( expr->moveSemantics ) ss << "/* <- */ ";
        }
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            ss << var->name;
            return Visitor::visit(var);
        }
    // const
        virtual ExpressionPtr visit(ExprFakeContext * c) override {
            ss << "__context__";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstPtr * c ) override {
            if ( c->getValue() ) {
                ss << "((void *) 0x" << HEX << intptr_t(c->getValue()) << DEC << ")";
            } else {
                ss << "null";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstEnumeration * c ) override {
            auto value = c->getValue();
            ss << c->enumType->name << "::" << c->enumType->find(value,to_string(value));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt * c ) override {
            ss << c->getValue();
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt64 * c ) override {
            ss << c->getValue() << "ll";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt64 * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC << "ull";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC << "u";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstBool * c ) override {
            ss << (c->getValue() ? "true" : "false");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstDouble * c ) override {
            ss << c->getValue();
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat * c ) override {
            ss << c->getValue() << "f";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstString * c ) override {
            ss << "\"" << escapeString(c->text) << "\"";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt2 * c ) override {
            auto val = c->getValue();
            ss << "int2(" << val.x << "," << val.y << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt3 * c ) override {
            auto val = c->getValue();
            ss << "int3(" << val.x << "," << val.y << "," << val.z << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt4 * c ) override {
            auto val = c->getValue();
            ss << "int4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt2 * c ) override {
            auto val = c->getValue();
            ss << "uint2(" << val.x << "," << val.y << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt3 * c ) override {
            auto val = c->getValue();
            ss << "uint3(" << val.x << "," << val.y << "," << val.z << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt4 * c ) override {
            auto val = c->getValue();
            ss << "uint4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat2 * c ) override {
            auto val = c->getValue();
            ss << "float2(" << val.x << "f," << val.y << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            ss << "float3(" << val.x << "f," << val.y << "f," << val.z << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            ss << "float4(" << val.x << "f," << val.y << "f," << val.z << "f," << val.w << "f)";
            return Visitor::visit(c);
        }
        // if then else
        virtual void preVisit ( ExprIfThenElse * ifte ) override {
            Visitor::preVisit(ifte);
            ss << "if ( ";
        }
        virtual void preVisitIfBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitIfBlock(ifte,block);
            ss << " )\n";
        }
        virtual void preVisitElseBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitElseBlock(ifte, block);
            ss << string(tab,'\t') << "else\n";
        }
    // string builder
        virtual void preVisit ( ExprStringBuilder * expr ) override {
            Visitor::preVisit(expr);
            uint32_t nArgs = uint32_t(expr->elements.size());
            ss << "das_string_builder(__context__,SimNode_AotInterop(";
            if ( nArgs ) {
                DebugInfoHelper helper;
                string debug_info_name = "__type_info__" + to_string(debugInfoGlobal++);
                sti << "\nTypeInfo " << debug_info_name << "[" << nArgs << "] = {\n";
                for ( uint32_t i=0; i!=nArgs; ++i ) {
                    auto & el = expr->elements[i];
                    TypeInfo * info = helper.makeTypeInfo(nullptr, el->type);
                    sti << "\t";
                    describeCppTypeInfo(sti, info);
                    sti << ",\n";
                }
                sti << "};\n";
                ss << nArgs << ", " << debug_info_name << ", ";
            } else {
                ss << "0, nullptr";
            }
        }
        virtual void preVisitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            Visitor::preVisitStringBuilderElement(sb, expr, last);
            ss << "cast<" << describeCppType(expr->type) << ">::from(";

        }
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            ss << ")";
            if ( !last ) ss << ", ";
            return Visitor::visitStringBuilderElement(sb, expr, last);
        }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) override {
            ss << "))";
            return Visitor::visit(expr);
        }
    // ExprMakeBlock
        virtual void preVisit ( ExprMakeBlock * expr ) override {
            Visitor::preVisit(expr);
            auto block = static_pointer_cast<ExprBlock>(expr->block);
            ss << "das_make_block<";
            ss << describeCppType(block->returnType);
            for ( auto & arg : block->arguments ) {
                ss << "," << describeCppType(arg->type);
            }
            ss << ">(__context__,[&](";
            for ( auto & arg : block->arguments ) {
                ss << describeCppType(arg->type) << " " << arg->name;
            }
            ss << ")->" << describeCppType(block->returnType);
        }
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // call
        virtual void preVisit ( ExprCall * call ) override {
            Visitor::preVisit(call);
            if ( call->func->builtIn ) {
                auto bif = static_cast<BuiltInFunction *>(call->func);
                if ( bif->cppName.empty() ) {
                    ss << bif->name << "(";
                } else {
                    ss << bif->cppName << "(";
                }
            } else {
                // TODO: support 'hybrid' calls here
                ss << call->name << "(__context__,";
            }
        }
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprCall * c ) override {
            ss << ")";
            return Visitor::visit(c);
        }
    // for
        string forSrcName ( const string & varName ) const {
            return "__" + varName + "_iterator";
        }
        virtual void preVisit ( ExprFor * ffor ) override {
            Visitor::preVisit(ffor);
            ss << "{\n";
            tab ++;
            ss << string(tab,'\t') << "bool __need_loop = true;\n";
        }
        virtual void preVisitForBody ( ExprFor * ffor, Expression * body ) override {
            Visitor::preVisitForBody(ffor, body);
            ss << string(tab,'\t') << "for ( ; __need_loop ; __need_loop &= __need_loop";
            for ( auto & var : ffor->iteratorVariables ) {
                ss << " && " << forSrcName(var->name) << ".next(__context__," << var->name << ")";
            }
            ss << " )\n";
            ss << string(tab,'\t');
        }
        virtual void preVisitForSource ( ExprFor * ffor, Expression * that, bool last ) override {
            Visitor::preVisitForSource(ffor, that, last);
            size_t idx;
            for ( idx=0; idx!=ffor->sources.size(); ++idx ) {
                if ( ffor->sources[idx].get()==that ) {
                    break;
                }
            }
            auto & var = ffor->iteratorVariables[idx];
            ss << string(tab,'\t') << "das_iterator<" << describeCppType(ffor->sources[idx]->type)
                << "> " << forSrcName(var->name) << "(";
        }
        virtual ExpressionPtr visitForSource ( ExprFor * ffor, Expression * that , bool last ) override {
            size_t idx;
            for ( idx=0; idx!=ffor->sources.size(); ++idx ) {
                if ( ffor->sources[idx].get()==that ) {
                    break;
                }
            }
            ss << ");\n";
            auto & var = ffor->iteratorVariables[idx];
            ss << string(tab,'\t') << describeCppType(var->type) << " " << var->name << ";\n";
            ss << string(tab,'\t') << "__need_loop = " << forSrcName(var->name)
                << ".first(__context__," << var->name << ") && __need_loop;\n";
            return Visitor::visitForSource(ffor, that, last);
        }
        virtual ExpressionPtr visit ( ExprFor * ffor ) override {
            ss << "\n";
            for ( auto & var : ffor->iteratorVariables ) {
                ss << string(tab,'\t') << forSrcName(var->name) << ".close(__context__," << var->name << ");\n";
            }
            tab --;
            ss << string(tab,'\t') << "}";
            return Visitor::visit(ffor);
        }
    };

    void Program::aotCpp ( TextWriter & logs ) {
        setPrintFlags();
        /*
        bool any = false;
        program.library.foreach([&](Module * pm) {
            if ( !pm->name.empty() && pm->name!="$" ) {
                stream << "require " << pm->name << "\n";
                any = true;
            }
            return true;
        }, "*");
        if (any) stream << "\n";
        */
        CppAot aotVisitor;
        visit(aotVisitor);
        logs << aotVisitor.str();
    }
}

