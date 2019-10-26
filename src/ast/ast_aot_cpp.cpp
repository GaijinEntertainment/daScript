#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

#include "daScript/misc/enums.h"
#include "daScript/simulate/hash.h"

namespace das {

    Enum<Type> g_cppCTypeTable = {
        {   Type::autoinfer,    "autoinfer"  },
        {   Type::alias,        "alias"  },
        {   Type::anyArgument,  "anyArgument"  },
        {   Type::tVoid,        "tVoid"  },
        {   Type::tStructure,   "tStructure" },
        {   Type::tPointer,     "tPointer" },
        {   Type::tBool,        "tBool"  },
        {   Type::tInt8,        "tInt8"  },
        {   Type::tUInt8,       "tUInt8"  },
        {   Type::tInt16,       "tInt16"  },
        {   Type::tUInt16,      "tUInt16"  },
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
        {   Type::tTuple,       "tTuple"},
        {   Type::tHandle,      "tHandle"}
    };

    Enum<Type> g_cppTypeTable = {
        {   Type::tVoid,        "void"     },
        {   Type::tBool,        "bool"     },
        {   Type::tInt8,        "int8_t"   },
        {   Type::tUInt8,       "uint8_t"  },
        {   Type::tInt16,       "int16_t"  },
        {   Type::tUInt16,      "uint16_t" },
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
        {   Type::tLambda,      "Lambda"   },
        {   Type::tTuple,       "Tuple"    }
    };

    string aotModuleName ( Module * pm  ) {
        if ( pm->name.empty() ) {
            return "";
        } else if ( pm->name=="$" ) {
            return "_builtin_";
        } else {
            return pm->name;
        }
    }

    string das_to_cppString ( Type t ) {
        return g_cppTypeTable.find(t);
    }

    string das_to_cppCTypeString ( Type t ) {
        return g_cppCTypeTable.find(t);
    }

    bool isConstRedundantForCpp ( const TypeDeclPtr & type ) {
        if ( type->dim.size() ) return false;
        if ( type->isVectorType() ) return true;
        switch ( type->baseType ) {
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
                return true;
            default:
                return false;
        }
    }

    string describeCppType ( const TypeDeclPtr & type,
                            CpptSubstitureRef substituteRef,
                            CpptSkipRef skipRef,
                            CpptSkipConst skipConst,
                            CpptRedundantConst redundantConst ) {

        TextWriter stream;
        auto baseType = type->baseType;
        if ( isConstRedundantForCpp(type) && redundantConst==CpptRedundantConst::yes ) {
            skipConst = CpptSkipConst::yes;
        }
        if ( baseType != Type::tPointer ) {
            if ( skipConst==CpptSkipConst::no ) {
                if ( type->constant ) {
                    stream << "const ";
                }
            }
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
            if ( type->annotation->cppName.empty() ) {
                stream << type->annotation->name;
            } else {
                stream << type->annotation->cppName;
            }
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
        } else if ( baseType==Type::tTuple ) {
            stream << "TTuple<" << int(type->getTupleSize());
            for ( const auto & arg : type->argTypes ) {
                stream << "," << describeCppType(arg);
            }
            stream << ">";
        } else if ( baseType==Type::tStructure ) {
            if ( type->structType ) {
                if ( type->structType->module->name.empty() ) {
                    stream << "struct " << type->structType->name;
                } else {
                    stream << "struct " << aotModuleName(type->structType->module) << "::" << type->structType->name;
                }
            } else {
                stream << "/* unspecified structure */";
            }
        } else if ( baseType==Type::tPointer ) {
            if ( type->firstType ) {
                stream << describeCppType(type->firstType,CpptSubstitureRef::no,CpptSkipRef::no,CpptSkipConst::no,CpptRedundantConst::no) << " *";
            } else {
                stream << "void *";
            }
        } else if ( baseType==Type::tEnumeration ) {
            if ( type->enumType ) {
                if ( type->enumType->module->name.empty() ) {
                    stream << "/*enum*/ " << type->enumType->name;
                } else {
                    stream << "/*enum*/ " << aotModuleName(type->enumType->module) << "::" << type->enumType->name;
                }
            } else {
                stream << "/* unspecified enumeration */";
            }
        } else if ( baseType==Type::tIterator ) {
            if ( type->firstType ) {
                stream << "Iterator * /*" << describeCppType(type->firstType,substituteRef,skipRef,skipConst) << "*/";
            } else {
                stream << "Iterator *";
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
        } else {
            stream << das_to_cppString(baseType);
        }
        if ( type->dim.size() ) {
            for ( auto d : type->dim ) {
                stream << "," << d << ">";
            }
        }
        if ( type->ref && skipRef==CpptSkipRef::no ) {
            if ( substituteRef==CpptSubstitureRef::no ) {
                stream << " &";
            } else {
                stream << " *";
            }
        }
        if ( baseType == Type::tPointer ) {
            if ( skipConst==CpptSkipConst::no ) {
                if ( type->constant ) {
                    stream << "const ";
                }
            }
        }
        return stream.str();
    }

    class NoAotMarker : public Visitor {
    public:
        NoAotMarker() {}
    protected:
        Function * func = nullptr;
    protected:
        // type
        virtual void preVisit ( TypeDecl * type ) override {
            if ( func && !type->canAot() ) func->noAot = true;
        }
        // function
        virtual void preVisit ( Function * f ) override {
            func = f;
            Visitor::preVisit(f);
        }
        virtual FunctionPtr visit ( Function * that ) override {
            auto res = Visitor::visit(that);
            func = nullptr;
            return res;
        }
        // any expression
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            if ( func && expr->type && !expr->type->canAot() ) func->noAot = true;
        }
        // looks like call
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            if ( call->name=="invoke" ) {
                if ( call->arguments.size() && call->arguments[0]->rtti_isMakeBlock() ) {
                    auto mkb = static_pointer_cast<ExprMakeBlock>(call->arguments[0]);
                    auto blk = static_pointer_cast<ExprBlock>(mkb->block);
                    blk->aotSkipMakeBlock = true;
                }
            }
        }
    };

    class PrologueMarker : public Visitor {
    public:
        PrologueMarker() {}
    protected:
        Function * func = nullptr;
    protected:
        // function
        virtual void preVisit ( Function * f ) override {
            func = f;
            Visitor::preVisit(f);
        }
        virtual FunctionPtr visit ( Function * that ) override {
            auto res = Visitor::visit(that);
            func = nullptr;
            return res;
        }
        // ExprMakeBlock
        virtual void preVisit ( ExprMakeBlock * expr ) override {
            Visitor::preVisit(expr);
            if ( func && func->hasMakeBlock ) {
                auto block = static_pointer_cast<ExprBlock>(expr->block);
                if ( !block->aotSkipMakeBlock ) {
                    func->aotNeedPrologue = true;
                }
            }
        }
    };

    class AotDebugInfoHelper : public DebugInfoHelper {
    public:
        void writeDim ( TextWriter & ss, TypeInfo * info ) const {
            if ( info->dimSize ) {
                ss << "uint32_t " << typeInfoName(info) << "_dim[" << info->dimSize << "] = {";
                for ( uint32_t i=0; i!=info->dimSize; ++i ) {
                    if ( i ) ss << ",";
                    ss << info->dim[i];
                }
                ss << "};\n";
            }
        }
        string str() const {
            TextWriter ss;
            // extern declarations
            for ( auto & ti : smn2s ) {
                ss << "extern StructInfo " << structInfoName(ti.second) << ";\n";
            }
            for ( auto & ti : tmn2t ) {
                ss << "extern TypeInfo " << typeInfoName(ti.second) << ";\n";
            }
            for ( auto & ti : vmn2v ) {
                ss << "extern VarInfo " << varInfoName(ti.second) << ";\n";
                writeDim(ss, ti.second);
            }
            for ( auto & ti : fmn2f ) {
                ss << "extern FuncInfo " << funcInfoName(ti.second) << ";\n";
            }
            for ( auto & ti : emn2e ) {
                ss << "extern EnumInfo " << enumInfoName(ti.second) << ";\n";
            }
            ss << "\n";
            for ( auto & ti : emn2e ) {
                describeCppEnumInfoValues(ss, ti.second);
                ss << "EnumInfo " << enumInfoName(ti.second) << " = { ";
                describeCppEnumInfo(ss, ti.second);
                ss << " };\n";
            }
            for ( auto & ti : smn2s ) {
                describeCppStructInfoFields(ss, ti.second);
                ss << "StructInfo " << structInfoName(ti.second) << " = {";
                describeCppStructInfo(ss, ti.second);
                ss << " };\n";
            }
            for ( auto & ti : tmn2t ) {
                ss << "TypeInfo " << typeInfoName(ti.second) << " = { ";
                describeCppTypeInfo(ss, ti.second);
                ss << " };\n";
                writeDim(ss, ti.second);
            }
            ss << "\n";
            return ss.str();
        }

        string enumInfoName ( EnumInfo * info ) const {
            TextWriter ss;
            ss << "__enum_info__" << HEX << info->hash << DEC;
            return ss.str();
        }

        string funcInfoName ( FuncInfo * info ) const {
            TextWriter ss;
            ss << "__func_info__" << HEX << info->hash << DEC;
            return ss.str();
        }

        string varInfoName ( VarInfo * info ) const {
            TextWriter ss;
            ss << "__var_info__" << HEX << info->hash << DEC;
            return ss.str();
        }

        string structInfoName ( StructInfo * info ) const {
            TextWriter ss;
            ss << "__struct_info__" << HEX << info->hash << DEC;
            return ss.str();
        }

        string typeInfoName ( TypeInfo * info ) const {
            TextWriter ss;
            ss << "__type_info__" << HEX << info->hash << DEC;
            return ss.str();
        }

    protected:
        void describeCppVarInfo ( TextWriter & ss, VarInfo * info ) const {
            describeCppTypeInfo(ss, info);
            ss << ", \"" << info->name << "\", ";
            ss << info->offset;

        }
        void describeCppStructInfoFields ( TextWriter & ss, StructInfo * info ) const {
            if ( !info->fields ) return;
            for ( uint32_t fi=0; fi!=info->count; ++fi ) {
                ss << "VarInfo " << structInfoName(info) << "_field_" << fi << " =  { ";
                describeCppVarInfo(ss, info->fields[fi]);
                ss << " };\n";
            }
            ss << "VarInfo * " << structInfoName(info) << "_fields[" << info->count << "] =  { ";
            for ( uint32_t fi=0; fi!=info->count; ++fi ) {
                if ( fi ) ss << ", ";
                ss << "&" << structInfoName(info) << "_field_" << fi;
            }
            ss << " };\n";
        }
        void describeCppStructInfo ( TextWriter & ss, StructInfo * info ) const {
            ss << "\"" << info->name << "\", ";
            if ( info->fields ) {
                ss << structInfoName(info) << "_fields, ";
            } else {
                ss << "nullptr, ";
            }
            ss << info->count << ", ";
            ss << info->size << ", ";
            ss << info->initializer << ", ";
            ss << "nullptr, ";  // annotation list
            ss << "0x" << HEX << info->hash << DEC;
        }

        void describeCppEnumInfoValues ( TextWriter & ss, EnumInfo * einfo ) const {
            for ( uint32_t v=0; v!=einfo->count; ++v ) {
                auto val = einfo->fields[v];
                ss << "EnumValueInfo " << enumInfoName(einfo) << "_value_" << v << " = { \""
                << val->name << "\", " << val->value << " };\n";
            }
            ss << "EnumValueInfo * " << enumInfoName(einfo) << "_values [] = { ";
            for ( uint32_t v=0; v!=einfo->count; ++v ) {
                if ( v ) ss << ", ";
                ss << "&" << enumInfoName(einfo) << "_value_" << v;
            }
            ss << " };\n";
        }

        void describeCppEnumInfo ( TextWriter & ss, EnumInfo * info ) const {
            ss << "\"" << info->name << "\", " << enumInfoName(info) << "_values, "
                << info->count << ", 0x" << HEX << info->hash << DEC;
        }

        void describeCppTypeInfo ( TextWriter & ss, TypeInfo * info ) const {
            ss << "Type::" << das_to_cppCTypeString(info->type) << ", ";
            if ( info->structType ) {
                ss << "&" << structInfoName(info->structType);
            } else {
                ss << "nullptr";
            }
            ss << ", ";
            if ( info->enumType ) {
                ss << "&" << enumInfoName(info->enumType);
            } else {
                ss << "nullptr";
            }
            if ( info->annotation_or_name ) {
                ss << ", DAS_MAKE_ANNOTATION(L\"" << info->annotation_or_name->module->name << "::" << info->annotation_or_name->name << "\")";
            } else {
                DAS_ASSERT(info->type!=Type::tHandle);
                ss << ", nullptr";
            }
            ss << ", ";
            if ( info->firstType ) {
                ss << "&" << typeInfoName(info->firstType);
            } else {
                ss << "nullptr";
            }
            ss << ", ";
            if ( info->secondType ) {
                ss << "&" << typeInfoName(info->secondType);
            } else {
                ss << "nullptr";
            }

            // TODO: implement typeinfo 'argTypes'
            ss << ", nullptr";
            ss << ", " << info->argCount;

            ss << ", " << info->dimSize;
            ss << ", ";
            if ( info->dimSize ) {
                ss << typeInfoName(info) << "_dim";
            } else {
                ss << "nullptr";
            }
            ss << ", " << info->flags;
            ss << ", 0x" << HEX << info->hash << DEC;
        }

    };

    bool isLocalVec ( const TypeDeclPtr & vtype ) {
        return vtype->dim.size()==0 && vtype->isVectorType() && !vtype->ref;
    }

    bool isVecRef ( const TypeDeclPtr & vtype ) {
        return vtype->dim.size()==0 && vtype->isVectorType() && vtype->ref;
    }

    void describeLocalCppType ( TextWriter & ss, const TypeDeclPtr & vtype, CpptSubstitureRef substituteRef = CpptSubstitureRef::yes ) {
        if ( isLocalVec(vtype) ) {
            if ( vtype->constant ) ss << "const ";
            ss << "vec4f /*" << describeCppType(vtype,substituteRef) << "*/";
        } else {
            ss << describeCppType(vtype,substituteRef,CpptSkipRef::no);
        }
    }

    void describeVarLocalCppType ( TextWriter & ss, const TypeDeclPtr & vtype, CpptSubstitureRef substituteRef = CpptSubstitureRef::yes ) {
        if ( isLocalVec(vtype) ) {
            if ( vtype->constant ) ss << "const ";
            ss << "vec4f /*" << describeCppType(vtype,substituteRef) << "*/";
        } else {
            ss << describeCppType(vtype,substituteRef,CpptSkipRef::no,CpptSkipConst::yes);
        }
    }

    string aotFuncNameEx ( const string & funcName ) {
        string name;
        bool prefix = false;
        for ( char ch : funcName ) {
            if ( isalnum(ch) || ch=='_' ) {
                name += ch;
            } else {
                prefix = true;
                switch ( ch ) {
                    case '=':   name += "Equ"; break;
                    case '+':   name += "Add"; break;
                    case '-':   name += "Sub"; break;
                    case '*':   name += "Mul"; break;
                    case '/':   name += "Div"; break;
                    case '%':   name += "Mod"; break;
                    case '&':   name += "And"; break;
                    case '|':   name += "Or"; break;
                    case '^':   name += "Xor"; break;
                    case '?':   name += "Qmark"; break;
                    case '~':   name += "Tilda"; break;
                    case '!':   name += "Excl"; break;
                    case '>':   name += "Greater"; break;
                    case '<':   name += "Less"; break;
                    default:
                        name += "_0x";
                        name += '0' + (ch>>4);
                        name += '0' + (ch & 0x0f);
                        name += "_";
                        break;
                }
            }
        }
        return prefix ? ("_Func" + name) : name;
    }

    string aotFuncName ( Function * func ) {
        if ( func->hash ) {
            return aotFuncNameEx(func->name) + "_" + to_string(func->hash);
        } else {
            return aotFuncNameEx(func->name);
        }
    }

    class BlockVariableCollector : public Visitor {
    public:
        BlockVariableCollector() {}
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            stack.push_back(block);
        }
        string getVarName ( Variable * var ) const {
            auto it = rename.find(var);
            return it==rename.end() ? var->name : it->second;
        }
        string getVarName ( const VariablePtr & var ) const {
            return getVarName(var.get());
        }
        __forceinline bool isMoved(const VariablePtr & var) const {
            return moved.find(var.get()) != moved.end();
        }
    protected:
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            stack.pop_back();
            return Visitor::visit(block);
        }
        bool needRenaming ( Variable * ) const {
            // TODO: check if it indeed needs renaming
            return true;
        }
        void renameVariable ( Variable * var ) {
            if ( needRenaming(var) ) {
                string newName = "__" + var->name + "_rename_at_" + to_string(var->at.line);
                rename[var] = newName;
            }
        }
    // for loop
        virtual void preVisitFor ( ExprFor * expr, const VariablePtr & var, bool last ) override {
            Visitor::preVisitFor(expr,var,last);
            for ( auto & varr : expr->iteratorVariables ) {
                renameVariable(varr.get());
            }
        }
    // block argument
        virtual void preVisitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitBlockArgument(block, var, lastArg);
            renameVariable(var.get());
        }
    // functon argument
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitArgument(fn, var, lastArg);
            renameVariable(var.get());
        }
    // let
        ExprBlock * getCurrentBlock() const {
            ExprBlock * block = nullptr;
            for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
                ExprBlock * pb = *it;
                if (pb->isClosure) {
                    block = pb;
                    break;
                }
                if (!(pb->inTheLoop && pb->finalList.size())) {
                    block = pb;
                    break;
                }
            }
            return block;
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            if (stack.back()->finalList.size()) {    // only move from the block with finally
                ExprBlock * block = getCurrentBlock();
                DAS_ASSERT(block && "internal error. let expression without block");
                variables[block].push_back(var.get());
                moved.insert(var.get());
            }
            renameVariable(var.get());
        }
    // make array
        virtual void preVisit ( ExprMakeArray * expr ) override {
            auto block = getCurrentBlock();
            if ( !expr->doesNotNeedSp && expr->stackTop ) {
                localTemps[block].push_back(expr);
            }
        }
    // make tuple
        virtual void preVisit ( ExprMakeTuple * expr ) override {
            auto block = getCurrentBlock();
            if ( !expr->doesNotNeedSp && expr->stackTop ) {
                localTemps[block].push_back(expr);
            }
        }
    // make structure
        virtual void preVisit ( ExprMakeStructure * expr ) override {
            auto block = getCurrentBlock();
            if ( !expr->doesNotNeedSp && expr->stackTop ) {
                localTemps[block].push_back(expr);
            }
        }
    // call with CMRES
        virtual void preVisit ( ExprCall * expr ) override {
            auto block = getCurrentBlock();
            if ( !expr->doesNotNeedSp && expr->stackTop ) {
                localTemps[block].push_back(expr);
            }
        }
    public:
        vector<ExprBlock *>                     stack;
        map<ExprBlock *,vector<Variable *>>     variables;
        map<ExprBlock *,vector<Expression *>>   localTemps;
    protected:
        map<Variable *,string>              rename;
        set<Variable *>                     moved;
    };

    string describeCppFunc ( Function * fn, BlockVariableCollector * collector, bool needName = true, bool needInline = true ) {
        TextWriter ss;
        if ( needInline ) {
            ss << "inline ";
        }
        describeLocalCppType(ss,fn->result,CpptSubstitureRef::no);
        ss << " ";
        if ( needName ) {
            ss << aotFuncName(fn);
        } else {
            ss << "(*)";
        }
        ss << " ( Context * __context__";
        for ( auto & var : fn->arguments ) {
            ss << ", ";
            if (isLocalVec(var->type)) {
                describeLocalCppType(ss, var->type);
            } else {
                ss << describeCppType(var->type);
            }
            ss << " ";
            if ( var->type->isRefType() ) {
                ss << "& ";
            }
            if ( collector ) {
                ss << collector->getVarName(var);
            }
        }
        ss << " )";
        return ss.str();
    }

    class CppAot : public Visitor {
    public:
        CppAot ( const ProgramPtr & prog, BlockVariableCollector & cl ) : program(prog), collector(cl) {
            helper.rtti = program->options.getBoolOption("rtti",false);
        }
        string str() const {
            return "\n" + helper.str() + sti.str()  + stg.str() + ss.str();
        };
    public:
        TextWriter                  ss, sti, stg;
    protected:
        int                         lastNewLine = -1;
        int                         tab = 0;
        int                         debugInfoGlobal = 0;
        AotDebugInfoHelper          helper;
        ProgramPtr                  program;
        BlockVariableCollector &    collector;
        set<string>                 aotPrefix;
        vector<ExprBlock *>         scopes;
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
    public:
    // enumeration
        virtual void preVisit ( Enumeration * enu ) override {
            Visitor::preVisit(enu);
            ss << "\nenum class " << enu->name << " {\n";
        }
        virtual void preVisitEnumerationValue ( Enumeration * enu, const string & name, int value, bool last ) override {
            Visitor::preVisitEnumerationValue(enu, name, value, last);
            ss << "\t" << name << " = " << value;
            if ( !last ) ss << ",";
            ss << "\n";
        }
        virtual EnumerationPtr visit ( Enumeration * enu ) override {
            ss << "};\n";
            return Visitor::visit(enu);
        }
    // strcuture
        virtual bool canVisitStructureFieldInit ( Structure * ) override {
            return false;
        }
        virtual void preVisit ( Structure * that ) override {
            Visitor::preVisit(that);
            if ( that->cppLayout ) {
                ss << "\n#if 0 // skipping structure " << that->name << " declaration due to CPP layout";
            }
            ss << "\nstruct " << that->name;
            if (that->cppLayout && that->parent) {
                ss << " : " << that->parent->name;
            }
            ss << " {\n";
        }
        virtual void preVisitStructureField ( Structure * that, Structure::FieldDeclaration & decl, bool last ) override {
            Visitor::preVisitStructureField(that, decl, last);
            auto from = that->findFieldParent(decl.name);
            if ( that->cppLayout && from!=that ) {
                ss << "\t/* skipping " << decl.name << ", from " << from->name << " */";
                return;
            }
            ss << "\t" << describeCppType(decl.type) << " " << decl.name << ";";
            if ( decl.parentType ) {
                ss << " /* from " << from->name << " */";
            }
        }
        virtual void visitStructureField ( Structure * var, Structure::FieldDeclaration & decl, bool last ) override {
            ss << "\n";
            Visitor::visitStructureField(var, decl, last);
        }
        virtual StructurePtr visit ( Structure * that ) override {
            ss << "};\n";
            ss << "static_assert(sizeof(" << that->name << ")==" << that->getSizeOf() << ",\"structure size mismatch with DAS\");\n";
            for ( auto & tf : that->fields ) {
                ss << "static_assert(offsetof(" << that->name << "," << tf.name << ")=="
                    << tf.offset << ",\"structure field offset mismatch with DAS\");\n";
            }
            if ( that->cppLayout ) {
                ss << "#endif // end of skipping structure " << that->name << " declaration due to CPP layout\n";
            }
            return Visitor::visit(that);
        }
    // program body
        virtual void preVisitProgramBody ( Program * prog ) override {
            // functions
            ss << "\n";
            for ( auto & fnI : prog->thisModule->functions ) {
                auto & fn = fnI.second;
                if ( !fn->builtIn && !fn->noAot ) {
                    ss << describeCppFunc(fn.get(),&collector) << ";\n";
                }
            }
            ss << "\n";
        }
    // global let body
        virtual void preVisitGlobalLetBody ( Program * prog ) override {
            Visitor::preVisitGlobalLetBody(prog);
            ss << "void __init_script ( Context * __context__ )\n{\n";
            tab ++;
            // pre-declare locals
            auto & temps = collector.localTemps[nullptr];
            for ( auto & tmp : temps ) {
                ss << string(tab,'\t');
                describeVarLocalCppType(ss, tmp->type);
                ss << " " << makeLocalTempName(tmp) << ";\n";
            }
        }
        virtual void visitGlobalLetBody ( Program * prog ) override {
            for ( auto & fnI : program->thisModule->functions ) {
                auto & fn = fnI.second;
                if ( fn->init ) {
                    ss << string(tab,'\t');
                    if ( fn->noAot ) {
                        auto mangledName = fn->getMangledName();
                        uint32_t hash = hash_blockz32((uint8_t *)mangledName.c_str());
                        ss << "das_invoke_function<void>::invoke(__context__,Func(__context__->fnIdxByMangledName(/*"
                            << mangledName << "*/ " << hash << "u)));\n";
                    } else {
                        ss << aotFuncName(fn.get()) << "(__context__);\n";
                    }
                }
            }
            tab --;
            ss << "}\n";
            Visitor::visitGlobalLetBody(prog);
        }
    // global
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            ss << string(tab,'\t');
            if ( !var->used ) ss << "/* ";
            ss << (var->init ? "das_global" : "das_global_zero");
            ss << "<" << describeCppType(var->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes)
                << "," << int32_t(var->stackTop) << ">(__context__)";
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            if ( !var->used ) ss << " */";
            ss << "; /*" << var->name << "*/\n";

            return Visitor::visitGlobalLet(var);
        }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitGlobalLetInit(var, init);
            ss << " = ";
        }
    // function
        virtual bool canVisitFunction ( Function * fun ) override {
            if ( fun->noAot ) return false;
            return true;
        }
        virtual void preVisit ( Function * fn) override {
            Visitor::preVisit(fn);
            ss << "\ninline ";
            describeLocalCppType(ss,fn->result,CpptSubstitureRef::no);
            ss << " " << aotFuncName(fn) << " ( Context * __context__";
        }
        virtual void preVisitFunctionBody ( Function * fn,Expression * expr ) override {
            Visitor::preVisitFunctionBody(fn,expr);
            if ( fn->aotNeedPrologue ) {
                ss << " ) { das_stack_prologue __prologue(__context__," << fn->totalStackSize << ",__LINE__);\n";
            } else {
                ss << " )\n";
            }
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & arg, bool last ) override {
            Visitor::preVisitArgument(fn,arg,last);
            // arg
            ss << ", ";
            if (isLocalVec(arg->type)) {
                describeLocalCppType(ss, arg->type);
            } else {
                ss << describeCppType(arg->type);
            }
            if (arg->type->isRefType()) {
                ss << " & ";
            }
            ss << " " << collector.getVarName(arg);
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & arg, Expression * expr ) override {
            Visitor::preVisitArgumentInit(fn,arg,expr);
            ss << " = ";
        }
        virtual VariablePtr visitArgument ( Function * fn, const VariablePtr & that, bool last ) override {
            return Visitor::visitArgument(fn, that, last);
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            if ( fn->aotNeedPrologue ) {
                ss << "}\n";
            } else {
                ss << "\n";
            }
            return Visitor::visit(fn);
        }
    // block
        string makeLocalTempName ( Expression * expr ) const {
            uint32_t stackTop = 0;
            if ( expr->rtti_isMakeLocal() ) {
                stackTop = ((ExprMakeLocal *)expr)->stackTop;
            } else if ( expr->rtti_isCall() ) {
                stackTop = ((ExprCall *)expr)->stackTop;
            } else {
                DAS_ASSERT(0 && "we should not be here. we need stacktop for the name");
                stackTop = (expr->at.line<<16) + expr->at.column;
            }
            return "_temp_make_local_" + to_string(expr->at.line) + "_" + to_string(stackTop);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            scopes.push_back(block);
            block->finallyBeforeBody = true;
            block->finallyDisabled = block->inTheLoop;
            ss << "{\n";
            tab ++;
            // pre-declare variables
            auto & vars = collector.variables[block];
            for ( auto & var : vars ) {
                ss << string(tab,'\t');
                describeVarLocalCppType(ss, var->type);
                ss << " " << collector.getVarName(var) << ";\n";
            }
            // pre-declare locals
            auto & temps = collector.localTemps[block];
            for ( auto & tmp : temps ) {
                ss << string(tab,'\t');
                describeVarLocalCppType(ss, tmp->type);
                ss << " " << makeLocalTempName(tmp) << ";\n";
            }
        }
        virtual void preVisitBlockArgumentInit ( ExprBlock * block, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitBlockArgumentInit(block, var, init);
            ss << "\n#if 0\n";
        }
        virtual ExpressionPtr visitBlockArgumentInit ( ExprBlock * block, const VariablePtr & var, Expression * init ) override {
            ss << "\n#endif\n";
            return Visitor::visitBlockArgumentInit(block, var, init);
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
            block->finallyBeforeBody = false;
            block->finallyDisabled = false;
            scopes.pop_back();
            return Visitor::visit(block);
        }
        string finallyName ( ExprBlock * block ) const {
            return "__finally_" + to_string(block->at.line);
        }
        virtual void preVisitBlockFinal ( ExprBlock * block ) override {
            Visitor::preVisitBlockFinal(block);
            ss << string(tab-1,'\t') << "/* finally */ auto " << finallyName(block) << "= das_finally([&](){\n";
        }
        virtual void preVisitBlockFinalExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockFinalExpression(block, expr);
            ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockFinalExpression ( ExprBlock * block, Expression * that ) override {
            ss << ";"; newLine();
            return Visitor::visitBlockFinalExpression(block, that);
        }
        virtual void visitBlockFinal ( ExprBlock * block ) override {
            ss << string(tab-1,'\t') << "/* end finally */ });\n";
            Visitor::visitBlockFinal(block);
        }
    // let
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            if ( !collector.isMoved(var) ) {
                describeVarLocalCppType(ss, var->type);
                ss << " ";
            }
            ss << collector.getVarName(var);
            if ( !var->init && var->type->canInitWithZero() ) {
                if ( isLocalVec(var->type) ) {
                    ss << " = v_zero()";
                } else {
                    ss << " = 0";
                }
            } else if ( !var->init && !var->type->canInitWithZero() ) {
                ss << "; das_zero(" << collector.getVarName(var) << ")";
            }
        }
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !last ) ss << "; ";
            return Visitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * expr ) override {
            Visitor::preVisitLetInit(let,var,expr);
            ss << " = ";
            if ( var->type->ref ) {
                ss << "&(";
            }
            if ( expr->type->isPointer() && (!expr->type->firstType || expr->type->firstType->isVoid()) ) {
                ss << "(" << describeCppType(var->type) << ")(";
            }
            if ( expr->type->isString() ) {
                ss << "(char *)(";
            }
        }
        virtual ExpressionPtr visitLetInit ( ExprLet * let , const VariablePtr & var, Expression * expr ) override {
            if ( expr->type->isString() ) {
                ss << ")";
            }
            if ( expr->type->isPointer() && (!expr->type->firstType || expr->type->firstType->isVoid()) ) {
                ss << ")";
            }
            if ( var->type->ref ) {
                ss << ")";
            }
            return Visitor::visitLetInit(let, var, expr);
        }
    // label
        virtual void preVisit ( ExprLabel * that ) override {
            Visitor::preVisit(that);
            ss << "label_" << that->label << ":;";
        }
    // goto
        virtual void preVisit ( ExprGoto * that ) override {
            Visitor::preVisit(that);
            if ( !that->subexpr ) {
                ss << "goto label_" << that->label;
            } else {
                ss << "switch (";
            }
        }
        virtual ExpressionPtr visit(ExprGoto *that) override {
            if ( that->subexpr ) {
                ss << ") {\n";
                for ( auto it = scopes.rbegin(); it!=scopes.rend(); ++it ) {
                    auto blk = *it;
                    for ( const auto & ex : blk->list ) {
                        if ( ex->rtti_isLabel() ) {
                            auto lab = static_pointer_cast<ExprLabel>(ex);
                            ss << string(tab,'\t') << "case " << lab->label <<": goto label_" << lab->label << ";\n";
                        }
                    }
                }
                ss << string(tab,'\t') << "default: __context__->throw_error(\"invalid label\");\n";
                ss << string(tab,'\t') << "}";
            }
            return Visitor::visit(that);
        }
    // copy
        virtual void preVisit ( ExprCopy * that ) override {
            Visitor::preVisit(that);
            ss << "das_copy(";
        }
        virtual void preVisitRight ( ExprCopy * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << ",";
        }
        virtual ExpressionPtr visit ( ExprCopy * that ) override {
            ss << ")";
            return Visitor::visit(that);
        }
    // clone
        virtual void preVisit ( ExprClone * that ) override {
            Visitor::preVisit(that);
            ss << "das_clone(";
        }
        virtual void preVisitRight ( ExprClone * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << ",";
        }
        virtual ExpressionPtr visit ( ExprClone * that ) override {
            ss << ")";
            return Visitor::visit(that);
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
        void outPolicy ( const TypeDeclPtr & decl ) {
            if ( decl->baseType!=Type::tHandle ){
                ss << "SimPolicy<" << das_to_cppString(decl->baseType) << ">";
            } else {
                auto pname = decl->annotation->cppName.empty() ? decl->annotation->name : decl->annotation->cppName;
                ss << "SimPolicy<" << pname << ">";
            }
        }
        bool isOpPolicy ( ExprOp1 * that ) const {
            if ( isalpha(that->op[0]) ) return true;
            return that->subexpr->type->isPolicyType();
        }
        virtual void preVisit ( ExprOp1 * that ) override {
            Visitor::preVisit(that);
            if ( !that->func->builtIn || that->func->callBased ) {
                that->arguments.clear();
                that->arguments.push_back(that->subexpr);
                CallFunc_preVisit(that);
                CallFunc_preVisitCallArg(that, that->subexpr.get(), true);
            } else if ( isOpPolicy(that) ){
                outPolicy(that->subexpr->type);
                ss << "::" << opPolicyName(that) << "(";
            } else {
                if ( that->op!="+++" && that->op!="---" ) {
                    ss << that->op;
                }
                if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << "(";
            }
        }
        virtual ExpressionPtr visit ( ExprOp1 * that ) override {
            if ( !that->func->builtIn || that->func->callBased ) {
                CallFunc_visitCallArg(that, that->subexpr.get(), true);
                CallFunc_visit(that);
                that->arguments.clear();
            } else if ( isOpPolicy(that) ){
                ss << ",*__context__)";
            } else {
                if ( that->op=="+++" || that->op=="---" ) {
                    ss << that->op[0] << that->op[1];
                }
                if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << ")";
            }
            return Visitor::visit(that);
        }
    // op2
        bool isOpPolicy ( ExprOp2 * that ) const {
            if ( isalpha(that->op[0]) ) return true;
            if ( that->op=="/" || that->op=="%" ) return true;
            if ( that->op=="<<<" || that->op==">>>" || that->op=="<<<=" || that->op==">>>=" ) return true;
            return that->type->isPolicyType() || that->left->type->isPolicyType() || that->right->type->isPolicyType();
        }
        const TypeDeclPtr & opPolicyBase ( ExprOp2 * that ) const {
            if ( that->type->isPolicyType() ) return that->type;
            else if ( that->left->type->isPolicyType() ) return that->left->type;
            else return that->right->type;
        }
        string opPolicyName ( ExprOp * that ) {
            if ( that->func->builtIn ) {
                auto bfn = static_cast<BuiltInFunction *>(that->func);
                return bfn->cppName.empty() ? bfn->name : bfn->cppName;
            } else {
                return "/* NotAPolicy */";
            }
        }
        bool isRefPolicyOp(ExprOp2 * that) {
            return
                // math
                   (that->op == "+=")
                || (that->op == "-=")
                || (that->op == "*=")
                || (that->op == "/=")
                || (that->op == "%=")
                // bin
                || (that->op == "&=")
                || (that->op == "|=")
                || (that->op == "^=")
                // rotational
                || (that->op == "<<=")
                || (that->op == ">>=")
                || (that->op == "<<<=")
                || (that->op == ">>>=")
                ;
        }
        virtual void preVisit ( ExprOp2 * that ) override {
            Visitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
            if ( !that->func->builtIn || that->func->callBased ) {
                that->arguments.clear();
                that->arguments.push_back(that->left);
                that->arguments.push_back(that->right);
                CallFunc_preVisit(that);
                CallFunc_preVisitCallArg(that, that->left.get(), false);
            } else if ( isOpPolicy(that) ) {
                auto pt = opPolicyBase(that);
                if ( policyResultNeedCast(pt, that->type) ) {
                    ss << "cast<" << describeCppType(that->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::to(";
                }
                outPolicy(pt);
                ss << "::" << opPolicyName(that) << "(";
                if ( isRefPolicyOp(that) ) {
                    ss << "(char *)&(";
                } else if ( policyArgNeedCast(pt, that->left->type) ) {
                    if (that->left->type->isRefType()) {
                        ss << "cast<" << describeCppType(that->left->type, CpptSubstitureRef::no, CpptSkipRef::yes, CpptSkipConst::yes) << "*>::from(&(";
                    } else {
                        ss << "cast<" << describeCppType(that->left->type, CpptSubstitureRef::no, CpptSkipRef::yes, CpptSkipConst::yes) << ">::from(";
                    }
                }
            }
        }
        virtual void preVisitRight ( ExprOp2 * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            if ( !that->func->builtIn || that->func->callBased ) {
                CallFunc_visitCallArg(that, that->left.get(), false);
                CallFunc_preVisitCallArg(that, that->right.get(), true);
            } else if ( isOpPolicy(that) ) {
                auto pt = opPolicyBase(that);
                if ( isRefPolicyOp(that) ) {
                    ss << ")";
                } else if ( policyArgNeedCast(pt, that->left->type) ) {
                    if (that->left->type->isRefType()) {
                        ss << "))";
                    } else {
                        ss << ")";
                    }
                }
                ss << ",";
                if ( policyArgNeedCast(pt, that->right->type) ) {
                    if (that->right->type->isRefType()) {
                        ss << "cast<" << describeCppType(that->right->type, CpptSubstitureRef::no, CpptSkipRef::yes, CpptSkipConst::yes) << " *>::from(&(";
                    } else {
                        ss << "cast<" << describeCppType(that->right->type, CpptSubstitureRef::no, CpptSkipRef::yes, CpptSkipConst::yes) << ">::from(";
                    }
                }
            } else {
                ss << " " << that->op << " ";
            }
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( !that->func->builtIn || that->func->callBased ) {
                CallFunc_visitCallArg(that, that->right.get(), true);
                CallFunc_visit(that);
                that->arguments.clear();
            } else if ( isOpPolicy(that) ) {
                auto pt = opPolicyBase(that);
                if (policyArgNeedCast(pt, that->right->type)) {
                    if (that->right->type->isRefType()) {
                        ss << "))";
                    } else {
                        ss << ")";
                    }
                }
                ss << ",*__context__)";
                if ( policyResultNeedCast(pt, that->type) ) {
                    ss << ")";
                }
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
            auto argT = left->type;
            if ( isLocalVec(argT) ) {
                ss << "(vec4f)";
            } else if ( isVecRef(argT) ) {
                ss << "cast_vec_ref<" << describeCppType(argT,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::to";
            }
            ss << "(";
        }
        virtual void preVisitRight ( ExprOp3 * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << ") : ";
            auto argT = right->type;
            if ( isLocalVec(argT) ) {
                ss << "(vec4f)";
            } else if ( isVecRef(argT) ) {
                ss << "cast_vec_ref<" << describeCppType(argT,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::to";
            }
            ss << "(";
        }
        virtual ExpressionPtr visit ( ExprOp3 * that ) override {
            ss << ")";
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
    // continue
        virtual void preVisit ( ExprContinue * that ) override {
            Visitor::preVisit(that);
            ss << "continue";
        }
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            if ( var->local && var->variable->type->ref ) {
                ss << "(*" << collector.getVarName(var->variable) << ")";
            } else if ( var->local || var->block || var->argument ) {
                ss << collector.getVarName(var->variable);
            } else {
                ss << "das_global<" << describeCppType(var->variable->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes)
                    << "," << int32_t(var->variable->stackTop) << ">(__context__) /*" << var->name << "*/";
            }
            return Visitor::visit(var);
        }
    // null coaelescing
        virtual void preVisit ( ExprNullCoalescing * nc ) override {
            Visitor::preVisit(nc);
            ss << "das_null_coalescing<" << describeCppType(nc->defaultValue->type,CpptSubstitureRef::no,CpptSkipRef::no,CpptSkipConst::no)
                << ">::get(";
        }
        virtual void preVisitNullCoaelescingDefault ( ExprNullCoalescing * nc, Expression * expr ) override {
            Visitor::preVisitNullCoaelescingDefault(nc,expr);
            ss << ",";
        }
        virtual ExpressionPtr visit ( ExprNullCoalescing * nc ) override {
            ss << ")";
            return Visitor::visit(nc);
        }
    // safe field
        virtual void preVisit ( ExprSafeField * field ) override {
            Visitor::preVisit(field);
            ss << "das_safe_navigation";
            auto vtype = field->value->type->firstType;
            if ( vtype->isGoodTupleType() ) ss << "_tuple";
            if ( field->skipQQ ) ss << "_ptr";
            ss << "<";
            if ( !vtype->isGoodTupleType() ) {
                ss << describeCppType(field->value->type->firstType) << ",";
            }
            if ( field->skipQQ ) {
                ss << describeCppType(field->type);
            } else {
                ss << describeCppType(field->type->firstType);
            }
            if ( vtype->isHandle() ) {
                ss << ",&";
                if ( vtype->annotation->cppName.empty() ) {
                    ss << vtype->annotation->name;
                } else {
                    ss << vtype->annotation->cppName;
                }
            } else if ( vtype->isGoodTupleType() ) {
                ss << ", " << vtype->getTupleFieldOffset(field->tupleIndex) <<  ">::get(";
            } else {
                ss << ",&" << vtype->structType->name;
            }
            if ( !vtype->isGoodTupleType() ) {
                ss << "::" << field->name <<  ">::get(";
            }
            /*
            if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotPreVisitGetField(ss, field->name);
            }
            */
        }
        virtual ExpressionPtr visit ( ExprSafeField * field ) override {
            /*
            if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotVisitGetField(ss, field->name);
            } */
            ss << ")";
            return Visitor::visit(field);
        }
    // field
        virtual void preVisit ( ExprField * field ) override {
            Visitor::preVisit(field);
            if ( field->value->type->isTuple() ) {
                ss << "das_get_tuple_field<"
                    << describeCppType(field->value->type->argTypes[field->tupleIndex])
                    << ","
                    << field->value->type->getTupleFieldOffset(field->tupleIndex)
                    << ">::get(";
            } else if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotPreVisitGetField(ss, field->name);
            } else if ( field->value->type->baseType==Type::tPointer ) {
                if ( field->value->type->firstType->isHandle() ) {
                    field->value->type->firstType->annotation->aotPreVisitGetFieldPtr(ss, field->name);
                } else if ( field->value->type->firstType->isTuple() ) {
                    ss << "das_get_tuple_field_ptr<"
                        << describeCppType(field->value->type->firstType->argTypes[field->tupleIndex])
                        << ","
                        << field->value->type->firstType->getTupleFieldOffset(field->tupleIndex)
                        << ">::get(";
                }
            }
        }
        virtual ExpressionPtr visit ( ExprField * field ) override {
            if ( field->value->type->isTuple() ) {
                ss << ")";
            } else if ( field->value->type->isHandle() ) {
                field->value->type->annotation->aotVisitGetField(ss, field->name);
                ss << " /*" << field->name << "*/";
            } else if ( field->value->type->baseType==Type::tPointer ) {
                if ( field->value->type->firstType->isHandle() ) {
                    field->value->type->firstType->annotation->aotVisitGetFieldPtr(ss, field->name);
                    ss << " /*" << field->name << "*/";
                } else if ( field->value->type->firstType->isTuple() ) {
                    ss << ")";
                } else {
                    ss << "->" << field->name;
                }
            } else {
                ss << "." << field->name;
            }
            return Visitor::visit(field);
        }
    // at
        virtual void preVisit ( ExprAt * expr ) override {
            Visitor::preVisit(expr);
            if ( !(expr->subexpr->type->dim.size() || expr->subexpr->type->isGoodArrayType() || expr->subexpr->type->isGoodTableType()) ) {
                ss << "das_index<" << describeCppType(expr->subexpr->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes)
                    << ">::at(";
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
                ss << "nullptr";
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
        virtual ExpressionPtr visit ( ExprConstRange * c ) override {
            auto val = c->getValue();
            ss << "range(" << val.from << "," << val.to << ")";
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
        virtual ExpressionPtr visit ( ExprConstURange * c ) override {
            auto val = c->getValue();
            ss << "urange(" << val.from << "," << val.to << ")";
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
            if (val.x == 0.0f && val.y == 0.0f ) {
                ss << "v_zero()";
            } else if (val.x == val.y ) {
                ss << "v_splats(" << to_string_ex(val.x) << "f)";
            } else {
                ss << "v_make_vec4f(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f,0.f,0.f)";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            if (val.x == 0.0f && val.y == 0.0f && val.z == 0.0f) {
                ss << "v_zero()";
            } else if (val.x == val.y && val.x == val.z) {
                ss << "v_splats(" << to_string_ex(val.x) << "f)";
            } else {
                ss << "v_make_vec4f(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f," << to_string_ex(val.z) << "f,0.f)";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            if (val.x == 0.0f && val.y == 0.0f && val.z == 0.0f && val.w == 0.0f ) {
                ss << "v_zero()";
            } else if (val.x == val.y && val.x == val.z && val.x == val.w ) {
                ss << "v_splats(" << to_string_ex(val.x) << "f)";
            } else {
                ss << "v_make_vec4f(" << to_string_ex(val.x) << "f," << to_string_ex(val.y) << "f," << to_string_ex(val.z) << "f," << to_string_ex(val.w) << "f)";
            }
            return Visitor::visit(c);
        }
    // ExprWhile
        virtual void preVisit ( ExprWhile * wh ) override {
            Visitor::preVisit(wh);
            if ( wh->body->rtti_isBlock() ) {
                auto * block = static_cast<ExprBlock *>(wh->body.get());
                if ( !block->finalList.empty() ) {
                    ss << "{\n";
                    tab ++;
                    block->visitFinally(*this);
                    ss << string(tab,'\t');
                }
            }
            ss << "while ( ";
        }
        virtual void preVisitWhileBody ( ExprWhile * wh, Expression * body ) override {
            Visitor::preVisitWhileBody(wh,body);
            ss << " )\n";
            ss << string(tab,'\t');
        }
        virtual ExpressionPtr visit ( ExprWhile * wh ) override {
            if ( wh->body->rtti_isBlock() ) {
                auto * block = static_cast<ExprBlock *>(wh->body.get());
                if ( !block->finalList.empty() ) {
                    tab --;
                    ss << "\n" << string(tab,'\t') << "}";
                }
            }
            return Visitor::visit(wh);
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
            ss << " else ";
        }
    // swizzle
        virtual void preVisit ( ExprSwizzle * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->type->ref ) {
                ss << "das_swizzle_ref<"
                    << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes) << ","
                    << describeCppType(expr->value->type,CpptSubstitureRef::no,CpptSkipRef::yes) << ","
                    << int32_t(expr->fields[0]) << ">::swizzle(";
            } else {
                if ( expr->fields.size()==1 ) {
                    const char * mask = "xyzw";
                    ss << "v_extract_" << mask[expr->fields[0]];
                    if ( expr->type->baseType!=Type::tFloat ) ss << "i";
                    ss << "(";
                    if (expr->type->baseType != Type::tFloat) ss << "v_cast_vec4i(";
                } else if ( TypeDecl::isSequencialMask(expr->fields) ) {
                    ss << "das_swizzle_seq<"
                    << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes) << ","
                    << describeCppType(expr->value->type,CpptSubstitureRef::no,CpptSkipRef::yes) << ","
                    << int32_t(expr->fields[0]) << ">::swizzle(";
                } else {
                    ss << "das_swizzle<"
                        << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes) << ","
                        << describeCppType(expr->value->type,CpptSubstitureRef::no,CpptSkipRef::yes);
                    for ( size_t i=0; i!=expr->fields.size(); ++i ) {
                        ss << ",";
                        ss << int32_t(expr->fields[i]);
                    }
                    ss << ">::swizzle(";
                }
            }
        }
        virtual ExpressionPtr visit ( ExprSwizzle * expr ) override {
            if ( expr->type->ref ) {
                ss << ")";
            } else {
                if ( expr->fields.size()==1 ) {
                    if (expr->type->baseType != Type::tFloat) ss << ")";
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
        string outputCallTypeInfo ( uint32_t nArgs, vector<ExpressionPtr> & elements ) {
            vector<TypeInfo*> elInfo;
            elInfo.reserve(elements.size());
            for ( auto & el : elements ) {
                TypeInfo * info = helper.makeTypeInfo(nullptr, el->type);
                elInfo.push_back(info);
            }
            string debug_info_name = "__tinfo_" + to_string(debugInfoGlobal++);
            sti << "TypeInfo * " << debug_info_name << "[" << nArgs << "] = { ";
            for ( size_t i=0; i!=elInfo.size(); ++i ) {
                auto info = elInfo[i];
                if ( i ) sti << ", ";
                sti << "&" << helper.typeInfoName(info);
            }
            sti << " };\n";
            return debug_info_name;
        }
        virtual void preVisit ( ExprStringBuilder * expr ) override {
            Visitor::preVisit(expr);
            uint32_t nArgs = uint32_t(expr->elements.size());
            ss << "das_string_builder(__context__,SimNode_AotInterop<" << nArgs << ">(";
            if ( nArgs ) {
                auto debug_info_name = outputCallTypeInfo(nArgs, expr->elements);
                ss << debug_info_name << ", ";
            }
        }
        virtual void preVisitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            Visitor::preVisitStringBuilderElement(sb, expr, last);
            ss << "cast<" << describeCppType(expr->type);
            if ( expr->type->isRefType() && !expr->type->ref ) {
                ss << " &";
            }
            ss << ">::from(";
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
    // ptr2ref
        virtual void preVisit ( ExprPtr2Ref * ptr2ref ) override {
            Visitor::preVisit(ptr2ref);
            ss << "das_deref(__context__,";
        }
        virtual ExpressionPtr visit ( ExprPtr2Ref * ptr2ref ) override {
            ss << ")";
            return Visitor::visit(ptr2ref);
        }
     // ref2ptr
        virtual void preVisit ( ExprRef2Ptr * ref2ptr ) override {
            Visitor::preVisit(ref2ptr);
            ss << "das_ref(__context__,";
        }
        virtual ExpressionPtr visit ( ExprRef2Ptr * ref2ptr ) override {
            ss << ")";
            return Visitor::visit(ref2ptr);
        }
    // addr
        virtual void preVisit ( ExprAddr * expr ) override {
            if (expr->func) {
                auto mangledName = expr->func->getMangledName();
                uint32_t hash = hash_blockz32((uint8_t *)mangledName.c_str());
                ss << "Func(__context__->fnIdxByMangledName(/*" << mangledName << "*/ " << hash << "u))";
            } else {
                ss << "Func(0 /*nullptr*/)";
            }
        }
    // cast
        virtual void preVisit ( ExprCast * expr ) override {
            Visitor::preVisit(expr);
            ss << (expr->upcast ? "das_upcast" : "das_cast")
                << "<" << describeCppType(expr->castType) << ">::cast(";
        }
        virtual ExpressionPtr visit ( ExprCast * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // delete
        virtual void preVisit ( ExprDelete * edel ) override {
            Visitor::preVisit(edel);
            ss << "das_delete<";
            ss <<  describeCppType(edel->subexpr->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes);
            ss << ">::clear(__context__,";
        }
        virtual ExpressionPtr visit ( ExprDelete * edel ) override {
            ss << ")";
            return Visitor::visit(edel);
        }
    // ascend
        virtual void preVisit ( ExprAscend * expr ) override {
            Visitor::preVisit(expr);
            TypeInfo * info = nullptr;
            if ( expr->needTypeInfo ) {
                info = helper.makeTypeInfo(nullptr, expr->subexpr->type);
            }
            ss << "das_ascend<" << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ","
            << describeCppType(expr->subexpr->type,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::make(__context__,";
            if ( info ) {
                ss << "&" << helper.typeInfoName(info) << ",";
            } else {
                ss << "nullptr,";
            }
        }
        virtual ExpressionPtr visit ( ExprAscend * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // new
        virtual void preVisit ( ExprNew * enew ) override {
            Visitor::preVisit(enew);
            if ( enew->type->dim.size() ) {
                ss << "das_new_dim<";
                ss << describeCppType(enew->type->firstType,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes);
                for ( auto dd : enew->type->dim ) {
                    ss << "," << uint32_t(dd);
                }
                if ( enew->initializer ) {
                    ss << ">::make_and_init(__context__,[&](){ return ";
                    CallFunc_preVisit(enew);
                } else {
                    ss << ">::make(__context__";
                }
            } else {
                ss << "das_new<" << describeCppType(enew->type->firstType,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes);
                if ( enew->initializer ) {
                    ss << ">::make_and_init(__context__,[&](){ return ";
                    CallFunc_preVisit(enew);
                } else {
                    ss << ">::make(__context__";
                }
            }
        }
        virtual void preVisitNewArg ( ExprNew * enew, Expression * arg, bool last ) override {
            Visitor::preVisitNewArg(enew, arg, last);
            if ( enew->initializer ) {
                CallFunc_preVisitCallArg(enew, arg, last);
            }
        }
        virtual ExpressionPtr visitNewArg ( ExprNew * enew, Expression * arg, bool last ) override {
            if ( enew->initializer ) {
                CallFunc_visitCallArg(enew, arg, last);
            } else {
                DAS_ASSERT(0 && "we should not even be here. we are visiting arguments of a new, but it has no initializer???");
                ss << ",";
            }
            return Visitor::visitNewArg(enew, arg, last);
        }
        virtual ExpressionPtr visit ( ExprNew * enew ) override {
            if ( enew->initializer ) {
                CallFunc_visit(enew);
                ss << "; })";
            } else {
                ss << ")";
            }
            return Visitor::visit(enew);
        }
    // make structure
        bool needTempSrc ( ExprMakeLocal * expr ) const {
            return !expr->doesNotNeedSp && expr->stackTop;
        }
        string mksName ( ExprMakeStructure * expr ) const {
            if ( needTempSrc(expr) ) {
                return makeLocalTempName(expr);
            } else {
                return "__mks_" + to_string(expr->at.line);
            }
        }
        virtual void preVisit ( ExprMakeStructure * expr ) override {
            Visitor::preVisit(expr);
            ss << "(([&]() -> " << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                << (needTempSrc(expr) ? "&" : "") << " {\n";
            tab ++;
            if ( !needTempSrc(expr) ) {
                ss << string(tab,'\t') << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                    << " " << mksName(expr) << ";\n";
            }
            if ( !expr->initAllFields ) {
                ss << string(tab,'\t') << "das_zero(" << mksName(expr) << ");\n";
            }

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
            if ( !needTempSrc(expr) ) {
                return "__mka_" + to_string(expr->at.line);
            } else {
                return makeLocalTempName(expr);
            }
        }
        virtual void preVisit ( ExprMakeArray * expr ) override {
            Visitor::preVisit(expr);
            ss << "(([&]() -> " << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                << (needTempSrc(expr) ? "&" : "") << " {\n";
            tab ++;
            if ( !needTempSrc(expr) ) {
                ss << string(tab,'\t') << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                    << " " << mkaName(expr) << ";\n";
            }
            if ( !expr->initAllFields ) {
                ss << string(tab,'\t') << "das_zero(" << mkaName(expr) << ");\n";
            }
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
   // make tuple
        string mktName ( ExprMakeTuple * expr ) const {
            if ( !needTempSrc(expr) ) {
                return "__mkt_" + to_string(expr->at.line);
            } else {
                return makeLocalTempName(expr);
            }
        }
        virtual void preVisit ( ExprMakeTuple * expr ) override {
            Visitor::preVisit(expr);
            ss << "(([&]() -> " << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                << (needTempSrc(expr) ? "&" : "") << " {\n";
            tab ++;
            if ( !needTempSrc(expr) ) {
                ss << string(tab,'\t') << describeCppType(expr->type,CpptSubstitureRef::no,CpptSkipRef::yes)
                    << " " << mktName(expr) << ";\n";
            }
            if ( !expr->initAllFields ) {
                ss << string(tab,'\t') << "das_zero(" << mktName(expr) << ");\n";
            }
        }
        virtual void preVisitMakeTupleIndex ( ExprMakeTuple * expr, int index, Expression * init, bool lastField ) override {
            Visitor::preVisitMakeTupleIndex(expr, index, init, lastField);
            ss << string(tab,'\t') << "das_get_tuple_field<"
                << describeCppType(expr->makeType->argTypes[index])
                << ","
                << expr->makeType->getTupleFieldOffset(index)
                << ">::get(" << mktName(expr) << ") = ";
        }
        virtual ExpressionPtr visitMakeTupleIndex ( ExprMakeTuple * expr, int index, Expression * init, bool lastField ) override {
            ss << ";\n";
            return Visitor::visitMakeTupleIndex(expr, index, init, lastField);
        }
        virtual ExpressionPtr visit ( ExprMakeTuple * expr ) override {
            ss << string(tab,'\t') << "return " << mktName(expr)<< ";\n";
            tab --;
            ss << string(tab,'\t') << "})())";
            return Visitor::visit(expr);
        }
    // ExprMakeBlock
        virtual void preVisit ( ExprMakeBlock * expr ) override {
            Visitor::preVisit(expr);
            auto block = static_pointer_cast<ExprBlock>(expr->block);
            if ( !block->aotSkipMakeBlock ) {
                ss << "das_make_block";
                if ( block->returnType->isRefType() && !block->returnType->ref ) {
                    ss << "_cmres";
                }
                ss << "<" << describeCppType(block->returnType);
                for ( auto & arg : block->arguments ) {
                    ss << "," << describeCppType(arg->type);
                    if ( arg->type->isRefType() && !arg->type->ref ) {
                        ss << " &";
                    }
                }
                ss << ">(__context__," << block->stackTop << ",";
            }
            if ( !block->aotSkipMakeBlock || block->aotDoNotSkipAnnotationData ) {
                if ( block->annotationDataSid != 0 ) {
                    ss << "__context__->adBySid(" << block->annotationDataSid << "u)";
                } else {
                    ss << "0";
                }
                ss << ",";
            }
            ss << "[&](";
            int ai = 0;
            for ( auto & arg : block->arguments ) {
                if (ai++) ss << ", ";
                if (isLocalVec(arg->type)) {
                    describeLocalCppType(ss, arg->type);
                } else {
                    ss << describeCppType(arg->type,CpptSubstitureRef::no,CpptSkipRef::no,CpptSkipConst::no,CpptRedundantConst::no);
                    if ( arg->type->isRefType() && !arg->type->ref ) {
                        ss << " &";
                    }
                }
                ss << " " << collector.getVarName(arg);
            }
            ss << ")->" << describeCppType(block->returnType);
        }
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override {
            auto block = static_pointer_cast<ExprBlock>(expr->block);
            if ( !block->aotSkipMakeBlock ) {
                ss << ")";
            }
            return Visitor::visit(expr);
        }

    // looks like call
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            if (call->name == "debug" ) {
                auto argType = call->arguments[0]->type;
                TypeInfo * info = helper.makeTypeInfo(nullptr, argType);
                ss << "das_debug(__context__,&" << helper.typeInfoName(info) << ",__FILE__,__LINE__,";
                ss << "cast<" << describeCppType(argType) << ">::from(";
            } else if (call->name == "assert" || call->name=="verify") {
                auto ea = static_cast<ExprAssert *>(call);
                if ( call->arguments.size()==1 ) {
                    ss << (ea->isVerify ? "DAS_VERIFY" : "DAS_ASSERT") << "((";
                } else {
                    ss << (ea->isVerify ? "DAS_VERIFYF" : "DAS_ASSERTF") << "((";
                }
            } else if (call->name == "erase") {
                ss << "__builtin_table_erase(__context__,";
            } else if (call->name == "find") {
                ss << "__builtin_table_find(__context__,";
            } else if (call->name == "key_exists") {
                ss << "__builtin_table_key_exists(__context__,";
            } else if (call->name == "keys") {
                ss << "__builtin_table_keys(__context__,";
            } else if (call->name == "values") {
                ss << "__builtin_table_values(__context__,";
            } else if ( call->name=="invoke" ) {
                auto bt = call->arguments[0]->type->baseType;
                if (bt == Type::tBlock) ss << "das_invoke";
                else if (bt == Type::tLambda) ss << "das_invoke_lambda";
                else if (bt == Type::tFunction) ss << "das_invoke_function";
                else ss << "das_invoke /*unknown*/";
                ExprInvoke * einv = static_cast<ExprInvoke *>(call);
                ss << "<" << describeCppType(call->type) << ">::invoke";
                if ( einv->isCopyOrMove() ) ss << "_cmres";
                if ( call->arguments.size()>1 ) {
                    ss << "<";
                    for ( const auto & arg : call->arguments ) {
                        if ( arg!=call->arguments.front() ) {
                            ss << describeCppType(arg->type);
                            if ( arg->type->isRefType() && !arg->type->ref ) {
                                ss << " &";
                            }
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
        virtual void preVisitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) override {
            Visitor::preVisitLooksLikeCallArg(call, arg, last);
            if ( call->name=="invoke" ) {
                auto argType = arg->type;
                if ( arg->type->isRefType() ) {
                    if ( needsArgPass(arg) ) {
                        ss << "das_arg<" << describeCppType(argType,CpptSubstitureRef::no,CpptSkipRef::yes) << ">::pass(";
                    }
                } else if (isVecRef(argType)) {
                    ss << "cast_vec_ref<" << describeCppType(argType,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::to(";
                }
            }
        }
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) override {
            if ( call->name=="invoke" ) {
                auto argType = arg->type;
                if ( arg->type->isRefType() ) {
                    if ( needsArgPass(arg) ) {
                        ss << ")";
                    }
                } else if (isVecRef(argType)) {
                    ss << ")";
                }
            }
            if ( !last ) {
                if (call->name == "assert" || call->name=="verify" || call->name=="debug") {
                    ss << "),(";
                } else {
                    ss << ",";
                }
            }
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprLooksLikeCall * call ) override {
            if ( call->name=="assert" || call->name=="verify" || call->name=="debug" ) {
                ss << "))";
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
            if ( !polType->isHandle() ) {
                if ( polType->isVecPolicyType() && argType->isVecPolicyType() ) {
                    return false;
                }
            }
            if ( !polType->isPolicyType() ) {
                return false;
            }
            return true;
        }
        bool policyResultNeedCast ( const TypeDeclPtr & polType, const TypeDeclPtr & resType ) {
            if ( resType->isVoid() ) {
                return false;
            }
            if ( !resType->isPolicyType() ) {
                return false;
            }
            return policyArgNeedCast(polType, resType);
        }
        bool isPolicyBasedCall ( ExprCallFunc * call ) {
            if ( call->func->builtIn ) {
                auto bif = static_cast<BuiltInFunction *>(call->func);
                if ( bif->policyBased ) {
                    return true;
                }
            }
            return false;
        }
        bool isHybridCall ( Function * func ) {
            if ( func->builtIn ) {
                auto bif = (BuiltInFunction *) func;
                DAS_VERIFYF(!func->policyBased, "we should not be here. policy based calls are handled elsewhere");
                DAS_VERIFYF(!func->callBased, "we should not be here. call-based calls handled elsewhere");
                if ( bif->cppName.empty() ) {
                    return true;
                }
                return false;
            }
            if ( func->noAot ) return true;
            if ( func->aotHybrid ) return true;
            if ( func->module == program->thisModule.get() ) return false;
            return true;
        }
        bool needsArgPass ( const TypeDeclPtr & argType ) const {
            return !argType->constant;
        }
        bool needsArgPass ( Expression * expr ) const {
            if ( expr->rtti_isMakeBlock() ) {
                auto mkblock = static_cast<ExprMakeBlock *>(expr);
                auto block = static_pointer_cast<ExprBlock>(mkblock->block);
                if ( block->aotSkipMakeBlock ) {
                    return false;
                }
            }
            return needsArgPass(expr->type);
        }
        bool isCallWithTemp ( ExprCallFunc * call ) {
            if ( call->rtti_isCall() ) {
                auto expr = (ExprCall *) call;
                return !expr->doesNotNeedSp && expr->stackTop;
            }
            return false;
        }
        void CallFunc_preVisit ( ExprCallFunc * call ) {
            Visitor::preVisit(call);
            string aotName = call->func->getAotName(call);
            for ( auto & ann : call->func->annotations ) {
                if ( ann->annotation->rtti_isFunctionAnnotation() ) {
                    auto pAnn = static_pointer_cast<FunctionAnnotation>(ann->annotation);
                    if ( aotPrefix.find(aotName)==aotPrefix.end() ) {
                        pAnn->aotPrefix(stg, call);
                        aotPrefix.insert(aotName);
                    }
                }
            }
            if ( call->func->result->aotAlias ) {
                ss << "das_alias<" << call->func->result->alias << ">::from(";
            }
            if ( isCallWithTemp(call) ) {
                ss << "(" << makeLocalTempName(call) << " = (";
            }
            if ( call->func->builtIn ) {
                auto bif = static_cast<BuiltInFunction *>(call->func);
                if ( !call->arguments.size() && call->type->baseType==Type::tHandle ) {
                    // c-tor?
                    ss << "/*c-tor*/ ";
                } else if ( bif->policyBased ) {
                    outPolicy(call->arguments[0]->type);
                    ss << "::";
                }
                if ( bif->interopFn ) {
                    ss << "das_call_interop<";
                    ss << describeCppType(call->func->result);
                    ss << ">::call(&";
                }
                ss << aotName;
                if ( bif->interopFn ) {
                    uint32_t nArgs = (uint32_t) call->arguments.size();
                    ss << ",__context__,SimNode_AotInterop<" << nArgs << ">(";
                    if ( nArgs ) {
                        auto debug_info_name = outputCallTypeInfo(nArgs, call->arguments);
                        ss << debug_info_name << ",";
                    }
                } else {
                    ss << "(";
                }
            } else {
                if ( isHybridCall(call->func) ) {
                    ss << "das_invoke_function<" << describeCppType(call->type) << ">::invoke";
                    if ( call->func->result->isRefType() && !call->func->result->ref ) {
                        ss << "_cmres";
                    }
                    auto mangledName = call->func->getMangledName();
                    uint32_t hash = hash_blockz32((uint8_t *)mangledName.c_str());
                    if ( call->arguments.size()>=1 ) {
                        ss << "<";
                        for ( const auto & arg : call->arguments ) {
                            ss << describeCppType(arg->type);
                            if ( arg->type->isRefType() && !arg->type->ref ) {
                                ss << " &";
                            }
                            if ( arg!=call->arguments.back() ) {
                                ss << ",";
                            }
                        }
                        ss << ">(__context__,";
                        ss << "Func(__context__->fnIdxByMangledName(/*" << mangledName << "*/ " << hash << "u)),";
                    } else {
                        ss << "(__context__,";
                        ss << "Func(__context__->fnIdxByMangledName(/*" << mangledName << "*/ " << hash << "u))";
                    }
                } else {
                    ss << aotFuncName(call->func) << "(__context__";
                    if  ( call->arguments.size() ) ss << ",";
                }
            }
        }
        void CallFunc_preVisitCallArg ( ExprCallFunc * call, Expression * arg, bool ) {
            auto it = find_if(call->arguments.begin(), call->arguments.end(), [&](const ExpressionPtr & a) {
                return a.get() == arg;
            });
            DAS_ASSERT(it != call->arguments.end());
            auto argType = (*it)->type;
            auto funArgType = call->func->arguments[it-call->arguments.begin()]->type;
            if ( funArgType->aotAlias ) {
                ss << "das_alias<" << funArgType->alias << ">::to(";
            }
            if ( call->func->interopFn ) {
                ss << "cast<" << describeCppType(argType);
                if ( argType->isRefType() && !argType->ref ) {
                    ss << " &";
                }
                ss << ">::from(";
            } else if ( arg->type->isRefType() ) {
                if ( needsArgPass(arg) ) {
                    ss << "das_arg<" << describeCppType(argType,CpptSubstitureRef::no,CpptSkipRef::yes) << ">::pass(";
                }
            } else if (isVecRef(argType)) {
                ss << "cast_vec_ref<" << describeCppType(argType,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::to(";
            }
            if ( isPolicyBasedCall(call) && policyArgNeedCast(call->type, argType) ) {
                ss << "cast<" << describeCppType(argType,CpptSubstitureRef::no,CpptSkipRef::yes,CpptSkipConst::yes) << ">::from(";
            }
        }
        void CallFunc_visitCallArg ( ExprCallFunc * call, Expression * arg, bool last ) {
            auto it = find_if(call->arguments.begin(), call->arguments.end(), [&](const ExpressionPtr & a) {
                return a.get() == arg;
            });
            DAS_ASSERT(it != call->arguments.end());
            auto argType = (*it)->type;
            if ( isPolicyBasedCall(call) && policyArgNeedCast(call->type, argType) ) {
                ss << ")";
            }
            if ( call->func->interopFn ) {
                ss << ")";
            } else if ( arg->type->isRefType() ) {
                if ( needsArgPass(arg) ) {
                    ss << ")";
                }
            } else if (isVecRef(argType)) {
                ss << ")";
            }
            auto funArgType = call->func->arguments[it-call->arguments.begin()]->type;
            if ( funArgType->aotAlias ) ss << ")";
            if ( !last ) ss << ",";
        }
        void CallFunc_visit ( ExprCallFunc * call ) {
            if ( call->func->interopFn ) {
                ss << ")";
            }
            if ( !call->arguments.size() && call->type->baseType==Type::tHandle ) {
                // c-tor?
                ss << "/*end-c-tor*/";
            } else if ( isPolicyBasedCall(call) ) {
                ss << ",*__context__";
            }
            ss << ")";
            if ( isCallWithTemp(call) ) {
                ss << "))";
            }
            if ( call->func->result->aotAlias ) {
                ss << ")";
            }
        }
        virtual void preVisit ( ExprCall * call ) override {
            Visitor::preVisit(call);
            CallFunc_preVisit(call);
        }
        virtual void preVisitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            Visitor::preVisitCallArg(call, arg, last);
            CallFunc_preVisitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            CallFunc_visitCallArg(call, arg, last);
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprCall * call ) override {
            CallFunc_visit(call);
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
            if ( body->rtti_isBlock() ) {
                auto * block = static_cast<ExprBlock *>(body);
                if ( !block->finalList.empty() ) {
                    block->visitFinally(*this);
                }
            }
            ss << string(tab,'\t') << "for ( ; " << nl << " ; " << nl << " = ";
            for ( auto & var : ffor->iteratorVariables ) {
                if (var != ffor->iteratorVariables.front()) {
                    ss << " && ";
                }
                ss << forSrcName(var->name) << ".next(__context__," << collector.getVarName(var) << ")";
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
            ss << string(tab,'\t') << "das_iterator<"
                << describeCppType(ffor->sources[idx]->type,CpptSubstitureRef::yes,CpptSkipRef::yes,CpptSkipConst::yes)
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
            ss << string(tab,'\t') << describeCppType(var->type,CpptSubstitureRef::yes,CpptSkipRef::no,CpptSkipConst::yes)
                << " " << collector.getVarName(var) << ";\n";
            // loop
            auto nl = needLoopName(ffor);
            ss << string(tab,'\t') << nl << " = " << forSrcName(var->name)
                << ".first(__context__," << collector.getVarName(var) << ") && " << nl << ";\n";
            return Visitor::visitForSource(ffor, that, last);
        }
        virtual ExpressionPtr visit ( ExprFor * ffor ) override {
            ss << "\n";
            for ( auto & var : ffor->iteratorVariables ) {
                ss << string(tab,'\t') << forSrcName(var->name) << ".close(__context__," << collector.getVarName(var) << ");\n";
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

    void Program::registerAotCpp ( TextWriter & logs, Context & context, bool headers ) {
        vector<Function *> fnn; fnn.reserve(totalFunctions);
        for (auto & pm : library.modules) {
            for (auto & it : pm->functions) {
                auto pfun = it.second;
                if (pfun->index < 0 || !pfun->used)
                    continue;
                fnn.push_back(pfun.get());
            }
        }
        if ( headers ) {
            logs << "\nvoid registerAot ( AotLibrary & aotLib )\n{\n";
        }
        bool funInit = false;
        for ( int i=0; i!=context.totalFunctions; ++i ) {
            if ( fnn[i]->module != thisModule.get() )
                continue;
            if ( fnn[i]->init ) {
                funInit = true;
            }
            if ( fnn[i]->noAot )
                continue;
            // SimFunction * fn = context.getFunction(i);
            uint64_t semH = fnn[i]->hash;
            logs << "\t\t// " << aotFuncName(fnn[i]) << "\n";
            logs << "\t\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            if ( fnn[i]->copyOnReturn || fnn[i]->moveOnReturn ) {
                logs << "CMRES";
            }
            logs << "<" << describeCppFunc(fnn[i],nullptr,false,false) << ",";
            logs << "&" << aotFuncName(fnn[i]) << ">>();\n\t};\n";
        }
        if ( context.totalVariables || funInit ) {
            uint64_t semH = context.getInitSemanticHash();
            logs << "\t\t// [[ init script ]]\n";
            logs << "\t\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            logs << "<void (*)(Context *),";
            logs << "&__init_script>>();\n\t\t};\n";
        }
        if ( headers ) {
            logs << "}\n";
        }
    }

    void Program::aotCpp ( Context & context, TextWriter & logs ) {
        // run no-aot marker
        NoAotMarker marker;
        visit(marker);
        // mark prologue
        PrologueMarker pmarker;
        visit(pmarker);
        // compute semantic hash for each used function
        int fni = 0;
        for (auto & pm : library.modules) {
            for (auto & it : pm->functions) {
                auto pfun = it.second;
                if (pfun->index < 0 || !pfun->used)
                    continue;
                SimFunction * fn = context.getFunction(fni);
                pfun->hash = getFunctionHash(pfun.get(), fn->code);
                fni++;
            }
        }
        setPrintFlags();
        BlockVariableCollector collector;
        visit(collector);
        CppAot aotVisitor(shared_from_this(),collector);
        // pre visit all enumerations and structures for each dependency
        for ( auto & pm : library.modules ) {
            if ( pm == thisModule.get() ) {
                continue;
            }
            aotVisitor.ss << "namespace " << aotModuleName(pm) << " {\n";
            for ( auto & ite : pm->enumerations ) {
                visitEnumeration(aotVisitor, ite.second.get());
            }
            for ( auto & its : pm->structures ) {
                visitStructure(aotVisitor, its.second.get());
            }
            aotVisitor.ss << "\n}; // " << pm->name << "\n";
        }
        // now to the main body
        visit(aotVisitor);
        logs << aotVisitor.str();
    }
}

