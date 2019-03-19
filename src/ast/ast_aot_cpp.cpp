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
        {   Type::tLambda,      "tLambda"},
        {   Type::tHandle,      "tHandle"}
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

    string describeCppType ( const TypeDeclPtr & type, bool substituteRef = false, bool skipRef = false ) {
        TextWriter stream;
        auto baseType = type->baseType;
        if ( type->constant ) {
            stream << "const ";     // TODO: do we skip it alltogether?
        }
        if ( type->dim.size() ) {
            for ( size_t d=0; d!=type->dim.size(); ++d ) {
                stream << "TDim<";
            }
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
            if ( !type->constant && type->baseType==Type::tBlock ) {
                stream << "const ";
            }
            stream << das_to_cppString(baseType) << " /*";
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
            stream << "*/";
        }  else {
            stream << das_to_cppString(baseType);
        }
        if ( type->dim.size() ) {
            for ( auto d : type->dim ) {
                stream << "," << d << ">";
            }
        }
        if ( type->ref && !skipRef ) {
            if ( !substituteRef ) {
                stream << " &";
            } else {
                stream << " *";
            }
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
        if ( info->annotation_or_name ) {
            ss << ", DAS_MAKE_ANNOTATION(\"" << info->annotation_or_name->module->name << "::" << info->annotation_or_name->name << "\")";
        } else {
            ss << ", nullptr";
        }
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

    string describeCppFunc ( Function * fn, bool needName = true ) {
        TextWriter ss;
        ss << describeCppType(fn->result) << " ";
        if ( needName ) {
            ss << fn->name;
        }
        ss << " ( Context * __context__";
        for ( auto & var : fn->arguments ) {
            ss << ", " << describeCppType(var->type) << " ";
            if ( var->type->isRefType() ) {
                ss << "& ";
            }
            ss << var->name;
        }
        ss << " )";
        return ss.str();
    }

    class CppAot : public Visitor {
    public:
        CppAot () {}
        string str() const { return sti.str() + ss.str(); };
    protected:
        TextWriter      ss, sti;
        int             lastNewLine = -1;
        int             tab = 0;
        int             debugInfoGlobal = 0;
        set<uint32_t>   tinfo;
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
        // strcuture
        virtual void preVisit ( Structure * that ) override {
            Visitor::preVisit(that);
            ss << "\nstruct " << that->name << " {\n";
        }
        virtual void preVisitStructureField ( Structure * that, Structure::FieldDeclaration & decl, bool last ) override {
            Visitor::preVisitStructureField(that, decl, last);
            ss << "\t" << describeCppType(decl.type) << " " << decl.name << ";";
            if ( decl.parentType ) {
                ss << " /* from " << that->parent->name << " */";
            }
            if ( decl.init ) {
                ss << " /* = ";
            }
        }
        virtual void visitStructureField ( Structure * var, Structure::FieldDeclaration & decl, bool last ) override {
            if ( decl.init ) {
                ss << " */\n";
            } else {
                ss << "\n";
            }
            Visitor::visitStructureField(var, decl, last);
        }
        virtual StructurePtr visit ( Structure * that ) override {
            ss << "};\n";
            return Visitor::visit(that);
        }
    // program body
        virtual void preVisitProgramBody ( Program * prog ) override {
            // functions
            ss << "\n";
            for ( auto & fnI : prog->thisModule->functions ) {
                auto & fn = fnI.second;
                if ( !fn->builtIn ) {
                    ss << describeCppFunc(fn.get()) << ";\n";
                }
            }
            ss << "\n";
        }
    // global let body
        virtual void preVisitGlobalLetBody ( Program * prog ) override {
            Visitor::preVisitGlobalLetBody(prog);
            ss << "void __init_script ( Context * __context__ )\n{\n";
            tab ++;
        }
        virtual void visitGlobalLetBody ( Program * prog ) override {
            tab --;
            ss << "}\n";
            Visitor::visitGlobalLetBody(prog);
        }
    // global
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            ss << string(tab,'\t');
            ss << (var->init ? "das_global" : "das_global_zero");
            ss << "<" << describeCppType(var->type,false,true)
                << "," << int32_t(var->stackTop) << ">(__context__) /*"  << var->name << "*/";
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            ss << ";\n";
            return Visitor::visitGlobalLet(var);
        }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitGlobalLetInit(var, init);
            ss << " = ";
        }
    // function
        virtual void preVisit ( Function * fn) override {
            Visitor::preVisit(fn);
            ss << "\n" << describeCppType(fn->result) << " " << fn->name << " ( Context * __context__";
        }
        virtual void preVisitFunctionBody ( Function * fn,Expression * expr ) override {
            Visitor::preVisitFunctionBody(fn,expr);
            if ( fn->hasMakeBlock ) {
                ss << " ) { das_stack_prologue __prologue(__context__," << fn->totalStackSize << ",__LINE__);\n";
            } else {
                ss << " )\n";
            }
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & arg, bool last ) override {
            Visitor::preVisitArgument(fn,arg,last);
            // arg
            ss << ", " << describeCppType(arg->type);
            if ( arg->type->isRefType() ) {
                ss << " & ";
            }
            ss << " " << arg->name;
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & arg, Expression * expr ) override {
            Visitor::preVisitArgumentInit(fn,arg,expr);
            ss << " = ";
        }
        virtual VariablePtr visitArgument ( Function * fn, const VariablePtr & that, bool last ) override {
            return Visitor::visitArgument(fn, that, last);
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            if ( fn->hasMakeBlock ) {
                ss << "}\n";
            } else {
                ss << "\n";
            }
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
                ss << string(tab,'\t');
            }
        }
        virtual ExpressionPtr visit ( ExprLet * let ) override {
            if ( let->subexpr ) {
                tab --;
                ss << "\n" << string(tab,'\t') << "}";
            }
            return Visitor::visit(let);
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            ss << describeCppType(var->type,true) << " " << var->name;
            if ( !var->init && var->type->canInitWithZero() ) {
                ss << " = 0";
            } else if ( !var->init && !var->type->canInitWithZero() ) {
                ss << "; das_zero(" << var->name << ")";
            }
        }
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( let->scoped ) ss << "; ";
            return Visitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * expr ) override {
            Visitor::preVisitLetInit(let,var,expr);
            ss << " = ";
            if ( var->type->ref ) {
                ss << "&(";
            }
        }
        virtual ExpressionPtr visitLetInit ( ExprLet * let , const VariablePtr & var, Expression * that ) override {
            if ( var->type->ref ) {
                ss << ")";
            }
            return Visitor::visitLetInit(let, var, that);
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
        bool isOpPolicy ( ExprOp * that, const ExpressionPtr & left ) const {
            return left->type->isPolicyType() || isalpha(that->op[0]);
        }
        string opPolicyName ( ExprOp * that ) {
            auto bfn = static_cast<BuiltInFunction *>(that->func);
            return bfn->cppName.empty() ? bfn->name : bfn->cppName;
        }
        virtual void preVisit ( ExprOp2 * that ) override {
            Visitor::preVisit(that);
             if ( !noBracket(that) ) ss << "(";
            if ( isOpPolicy(that, that->left) ) {
                ss << "SimPolicy<" << das_to_cppString(that->left->type->baseType) << ">::" << opPolicyName(that) << "(";
                if ( that->left->type->ref ) ss << "(char *)&(";
                if ( policyArgNeedCast(that->type, that->left->type) ) {
                    ss << "cast<" << describeCppType(that->left->type,false,true) << ">::from(";
                }
            }
        }
        virtual void preVisitRight ( ExprOp2 * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            if ( isOpPolicy(that, that->left) ) {
                if ( policyArgNeedCast(that->type, that->left->type) )ss << ")";
                if ( that->left->type->ref ) ss << ")";
                ss << ",";
                if ( policyArgNeedCast(that->type, that->right->type) ) {
                    ss << "cast<" << describeCppType(that->right->type,false,true) << ">::from(";
                }
            } else {
                ss << " " << that->op << " ";
            }
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( isOpPolicy(that, that->left) ) {
                if ( policyArgNeedCast(that->type, that->right->type) )ss << ")";
                ss << ",*__context__)";
            }
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
    // break
        virtual void preVisit ( ExprBreak * that ) override {
            Visitor::preVisit(that);
            ss << "break";
        }
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            if ( var->local && var->variable->type->ref ) {
                ss << "(*" << var->name << ")";
            } else if ( var->block || var->local || var->argument) {
                ss << var->name;
            } else {
                ss << "das_global<" << describeCppType(var->variable->type,false,true)
                    << "," << int32_t(var->variable->stackTop) << ">(__context__) /*" << var->name << "*/";
            }
            return Visitor::visit(var);
        }
    // field
        virtual void preVisit ( ExprField * field ) override {
            Visitor::preVisit(field);
            if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotPreVisitGetField(ss, field->name);
            }
        }
        virtual ExpressionPtr visit ( ExprField * field ) override {
            if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotVisitGetField(ss, field->name);
                ss << " /*" << field->name << "*/";
            } else if ( field->value->type->baseType==Type::tPointer ) {
                 ss << "->" << field->name;
            } else {
                ss << "." << field->name;
            }
            return Visitor::visit(field);
        }
    // at
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            if ( !(expr->subexpr->type->dim.size() || expr->subexpr->type->isGoodArrayType() || expr->subexpr->type->isGoodTableType()) ) {
                ss << "das_index<" << describeCppType(expr->subexpr->type,false,true) << ">::at(";
            }
        }
        virtual void preVisitAtIndex ( ExprAt * expr, Expression * index ) override {
            Visitor::preVisitAtIndex(expr, index);
            if ( expr->subexpr->type->dim.size() || expr->subexpr->type->isGoodArrayType() || expr->subexpr->type->isGoodTableType() ) {
                ss << "(";
            } else {
                ss << ",";
            }

        }
        virtual ExpressionPtr visit ( ExprAt * that ) override {
            ss << ",__context__)";
            return Visitor::visit(that);
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
            ss << to_string_ex(c->getValue()) << "f";
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
            ss << "float2(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            ss << "float3(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f," << to_string_ex(val.z) << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            ss << "float4(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f," << to_string_ex(val.z) << "f," << to_string_ex(val.w) << "f)";
            return Visitor::visit(c);
        }
    // ExprWhile
        virtual void preVisit ( ExprWhile * wh ) override {
            Visitor::preVisit(wh);
            ss << "while ( ";
        }
        virtual void preVisitWhileBody ( ExprWhile * wh, Expression * body ) override {
            Visitor::preVisitWhileBody(wh,body);
            ss << " )\n";
            ss << string(tab,'\t');
        }
    // if then else
        virtual void preVisit ( ExprIfThenElse * ifte ) override {
            Visitor::preVisit(ifte);
            ss << "if ( ";
        }
        virtual void preVisitIfBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitIfBlock(ifte,block);
            ss << " )\n";
            ss << string(tab,'\t');
        }
        virtual void preVisitElseBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitElseBlock(ifte, block);
            ss << string(tab,'\t') << "else\n";
        }
    // swizzle
        virtual void preVisit ( ExprSwizzle * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->type->ref ) {
                ss << "das_swizzle_ref<"
                    << describeCppType(expr->type,false,true) << ","
                    << describeCppType(expr->value->type,false,true) << ","
                    << int32_t(expr->fields[0]) << ">::swizzle(";
            } else {
                if ( expr->fields.size()==1 ) {
                    const char * mask = "xyzw";
                    ss << "v_extract_" << mask[expr->fields[0]];
                    if ( expr->type->baseType!=Type::tFloat ) ss << "i";
                    ss << "(";
                } else if ( TypeDecl::isSequencialMask(expr->fields) ) {
                    ss << "das_swizzle_seq(";
                } else {
                    ss << "das_swizzle(";
                }
            }
        }
        virtual ExpressionPtr visit ( ExprSwizzle * expr ) override {
            if ( expr->type->ref ) {
                ss << ")";
            } else {
                if ( expr->fields.size()==1 ) {
                    ss << ")";
                } else if ( TypeDecl::isSequencialMask(expr->fields) ) {
                    ss << ")";
                } else {
                    ss << ")";
                }
            }
            ss << " /*";
            for ( auto f : expr->fields ) {
                switch ( f ) {
                    case 0:     ss << "x"; break;
                    case 1:     ss << "y"; break;
                    case 2:     ss << "z"; break;
                    case 3:     ss << "w"; break;
                    default:    ss << "?"; break;
                }
            }
            ss << "*/";
            return Visitor::visit(expr);
        }
    // string builder
        virtual void preVisit ( ExprStringBuilder * expr ) override {
            Visitor::preVisit(expr);
            uint32_t nArgs = uint32_t(expr->elements.size());
            ss << "das_string_builder(__context__,SimNode_AotInterop<" << nArgs << ">(";
            if ( nArgs ) {
                DebugInfoHelper helper;
                vector<TypeInfo*> elInfo;
                elInfo.reserve(expr->elements.size());
                for ( auto & el : expr->elements ) {
                    TypeInfo * info = helper.makeTypeInfo(nullptr, el->type);
                    if ( tinfo.find(info->hash)==tinfo.end() ) {
                        tinfo.insert(info->hash);
                        sti << "\nTypeInfo __type_info__" << HEX << info->hash << DEC << " = ";
                        describeCppTypeInfo(sti, info);
                        sti << ";";
                    }
                    elInfo.push_back(info);
                }
                string debug_info_name = "__tinfo_" + to_string(debugInfoGlobal++);
                sti << "\nTypeInfo * " << debug_info_name << "[" << nArgs << "] = { ";
                for ( size_t i=0; i!=elInfo.size(); ++i ) {
                    auto info = elInfo[i];
                    if ( i ) sti << ", ";
                    sti << "&__type_info__" << HEX << info->hash << DEC;
                }
                sti << " };\n";
                ss << debug_info_name << ", ";
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
    // try-catch
        virtual void preVisit ( ExprTryCatch * tc ) override {
            Visitor::preVisit(tc);
            ss << "das_try_recover(__context__, [&]()\n";
            ss << string(tab,'\t');
        }
        virtual void preVisitCatch ( ExprTryCatch * tc, Expression * block ) override {
            Visitor::preVisitCatch(tc, block);
            ss << ", [&]()\n";
            ss << string(tab,'\t');
        }
        virtual ExpressionPtr visit ( ExprTryCatch * tc ) override {
            ss << ")";
            return Visitor::visit(tc);
        }
    // make structure
        string mksName ( ExprMakeStructure * expr ) const {
            return "__mks_" + to_string(expr->at.line);
        }
        virtual void preVisit ( ExprMakeStructure * expr ) override {
            Visitor::preVisit(expr);
            ss << "(([&]() -> " << describeCppType(expr->type,false,true) << " {\n";
            tab ++;
            ss << string(tab,'\t')<< describeCppType(expr->type,false,true) << " " << mksName(expr)
                << "; das_zero(" << mksName(expr) << ");\n";

        }
        virtual void preVisitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool last ) override {
            Visitor::preVisitMakeStructureField(expr,index,decl,last);
            ss << string(tab,'\t') << mksName(expr);
            if ( expr->structs.size()!=1 ) ss << "(" << index << ",__context__)";
            ss << "." << decl->name << " = ";
        }
        virtual MakeFieldDeclPtr visitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool last ) override {
            ss << ";\n";
            return Visitor::visitMakeStructureField(expr,index,decl,last);
        }
        virtual ExpressionPtr visit ( ExprMakeStructure * expr ) override {
            ss << string(tab,'\t') << "return " << mksName(expr)<< ";\n";
            tab --;
            ss << string(tab,'\t') << "})())";
            return Visitor::visit(expr);
        }
    // make array
        string mkaName ( ExprMakeArray * expr ) const {
            return "__mka_" + to_string(expr->at.line);
        }
        virtual void preVisit ( ExprMakeArray * expr ) override {
            Visitor::preVisit(expr);
            ss << "(([&]() -> " << describeCppType(expr->type,false,true) << " {\n";
            tab ++;
            ss << string(tab,'\t')<< describeCppType(expr->type,false,true) << " " << mkaName(expr)
                << "; das_zero(" << mkaName(expr) << ");\n";
        }
        virtual void preVisitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool lastField ) override {
            Visitor::preVisitMakeArrayIndex(expr, index, init, lastField);
            ss << string(tab,'\t') << mkaName(expr) << "(" << index << ",__context__) = ";
        }
        virtual ExpressionPtr visitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool lastField ) override {
            ss << ";\n";
            return Visitor::visitMakeArrayIndex(expr, index, init, lastField);
        }
        virtual ExpressionPtr visit ( ExprMakeArray * expr ) override {
            ss << string(tab,'\t') << "return " << mkaName(expr)<< ";\n";
            tab --;
            ss << string(tab,'\t') << "})())";
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
            ss << ">(__context__," << block->stackTop << ",[&](";
            for ( auto & arg : block->arguments ) {
                ss << describeCppType(arg->type) << " " << arg->name;
            }
            ss << ")->" << describeCppType(block->returnType);
        }
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // looks like call
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            if ( call->name=="assert" ) {
                if ( call->arguments.size()==1 ) ss << "DAS_ASSERT((";
                else ss << "DAS_ASSERTF((";
            } else if ( call->name=="invoke" ) {
                ss << "das_invoke<" << describeCppType(call->type) << ">::invoke";
                if ( call->arguments.size()>1 ) {
                    ss << "<";
                    for ( const auto & arg : call->arguments ) {
                        if ( arg!=call->arguments.front() ) {
                            ss << describeCppType(arg->type);
                            if ( arg!=call->arguments.back() ) {
                                ss << ",";
                            }
                        }
                    }
                    ss << ">";
                }
                ss << "(__context__,";
            } else ss << call->name << "(";
        }
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprLooksLikeCall * call ) override {
            if ( call->name=="assert" ) {
                ss << "))"; // ts macro
            } else {
                ss << ")";
            }
            return Visitor::visit(call);
        }
    // call
        bool isPolicyBasedCall ( ExprCall * call ) const {
            auto bif = static_cast<BuiltInFunction *>(call->func);
            if ( !call->arguments.size() && call->type->baseType==Type::tHandle ) {
                // c-tor?
                return false;
            } else if ( bif->policyBased ) {
                return true;
            } else {
                return false;
            }
        }
        bool policyArgNeedCast ( const TypeDeclPtr & polType, const TypeDeclPtr & argType ) {
            if ( argType->isVectorType() ) {
                return false;
            }
            if ( !polType->isPolicyType() && !argType->isPolicyType() ) {
                return false;
            }
            return true;
        }
        virtual void preVisit ( ExprCall * call ) override {
            Visitor::preVisit(call);
            if ( call->func->builtIn ) {
                auto bif = static_cast<BuiltInFunction *>(call->func);
                if ( !call->arguments.size() && call->type->baseType==Type::tHandle ) {
                    // c-tor?
                    ss << "/*c-tor*/ ";
                } else if ( bif->policyBased ) {
                    ss << "SimPolicy<" << das_to_cppString(call->type->baseType) << ">::";
                }
                if ( bif->cppName.empty() ) {
                    ss << bif->name << "(";
                } else {
                    ss << bif->cppName << "(";
                }
            } else {
                // TODO: support 'hybrid' calls here
                ss << call->name << "(__context__";
                if  ( call->arguments.size() ) ss << ",";
            }
        }
        bool needsArgPass ( const TypeDeclPtr & argType ) const {
            return !argType->constant;
        }
        virtual void preVisitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            Visitor::preVisitCallArg(call, arg, last);
            if ( arg->type->isRefType() ) {
                auto it = find_if(call->arguments.begin(), call->arguments.end(), [&](const ExpressionPtr & a) {
                    return a.get() == arg;
                });
                DAS_ASSERT(it != call->arguments.end());
                auto argType = (*it)->type;
                if ( needsArgPass(argType) ) {
                    ss << "das_arg<" << describeCppType(argType,false,true) << ">::pass(";
                }
            }
        }
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            if ( arg->type->isRefType() ) {
                auto it = find_if(call->arguments.begin(), call->arguments.end(), [&](const ExpressionPtr & a) {
                    return a.get() == arg;
                });
                DAS_ASSERT(it != call->arguments.end());
                if ( needsArgPass((*it)->type) ) {
                    ss << ")";
                }
            }
            if ( !last ) ss << ",";
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprCall * call ) override {
            if ( isPolicyBasedCall(call) ) {
                ss << ",*__context__";
            }
            ss << ")";
            return Visitor::visit(call);
        }
    // for
        string forSrcName ( const string & varName ) const {
            return "__" + varName + "_iterator";
        }
        string needLoopName ( ExprFor * ffor ) const {
            return "__need_loop_" + to_string(ffor->at.line);
        }
        virtual void preVisit ( ExprFor * ffor ) override {
            Visitor::preVisit(ffor);
            ss << "{\n";
            tab ++;
            auto nl = needLoopName(ffor);
            ss << string(tab,'\t') << "bool " << nl << " = true;\n";
        }
        virtual void preVisitForBody ( ExprFor * ffor, Expression * body ) override {
            Visitor::preVisitForBody(ffor, body);
            auto nl = needLoopName(ffor);
            ss << string(tab,'\t') << "for ( ; " << nl << " ; " << nl << " = ";
            for ( auto & var : ffor->iteratorVariables ) {
                if (var != ffor->iteratorVariables.front()) {
                    ss << " && ";
                }
                ss << forSrcName(var->name) << ".next(__context__," << var->name << ")";
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
            ss << string(tab,'\t') << "das_iterator<" << describeCppType(ffor->sources[idx]->type,false,true)
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
            // source
            ss << string(tab,'\t') << describeCppType(var->type,true) << " " << var->name << ";\n";
            // loop
            auto nl = needLoopName(ffor);
            ss << string(tab,'\t') << nl << " = " << forSrcName(var->name)
                << ".first(__context__," << var->name << ") && " << nl << ";\n";
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

    uint64_t Context::getInitSemanticHash() {
        const uint64_t fnv_prime = 1099511628211ul;
        uint64_t hash = globalsSize;
        for ( int i=0; i!=totalVariables; ++i ) {
            hash = (hash ^ globalVariables[i].offset) * fnv_prime;
            hash = (hash ^ globalVariables[i].size) * fnv_prime;
            if ( globalVariables[i].init ) {
                hash = (hash ^ getSemanticHash(globalVariables[i].init)) * fnv_prime;
            }
        }
        return hash;
    }

    void Program::registerAotCpp ( TextWriter & logs, Context & context ) {
        vector<Function *> fnn; fnn.reserve(totalFunctions);
        for (auto & pm : library.modules) {
            for (auto & it : pm->functions) {
                auto pfun = it.second;
                if (pfun->index < 0 || !pfun->used)
                    continue;
                fnn.push_back(pfun.get());
            }
        }
        logs << "\nvoid registerAot ( AotLibrary & aotLib )\n{\n";
        for ( int i=0; i!=context.totalFunctions; ++i ) {
            SimFunction * fn = context.getFunction(i);
            uint64_t semH = getSemanticHash(fn->code);
            logs << "\t// " << fn->name << "\n";
            logs << "\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            if ( fnn[i]->copyOnReturn || fnn[i]->moveOnReturn ) {
                logs << "CMRES";
            }
            logs << "<" << describeCppFunc(fnn[i],false) << "," << fn->name << ">>();\n\t};\n";
        }
        if ( context.totalVariables ) {
            uint64_t semH = context.getInitSemanticHash();
            logs << "\t// [[ init script ]]\n";
            logs << "\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            logs << "<void (Context *), __init_script>>();\n\t};\n";
        }
        logs << "}\n";
    }

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

