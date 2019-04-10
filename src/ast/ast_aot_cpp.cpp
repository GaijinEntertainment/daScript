#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

#include "daScript/misc/enums.h"

namespace das {

    Enum<Type> g_cppCTypeTable = {
        {   Type::autoinfer,    "autoinfer"  },
        {   Type::alias,        "alias"  },
        {   Type::anyArgument,  "anyArgument"  },
        {   Type::tVoid,        "tVoid"  },
        {   Type::tStructure,   "tStructure" },
        {   Type::tPointer,     "tPointer" },
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

    string describeCppType ( const TypeDeclPtr & type, bool substituteRef = false, bool skipRef = false, bool skipConst = false ) {
        TextWriter stream;
        auto baseType = type->baseType;
        if ( !skipConst ) {
            if ( type->constant ) {
                stream << "const ";     // TODO: do we skip it alltogether?
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
                stream << "/*enum*/ " << type->enumType->name;
            } else {
                stream << "/* unspecified enumeration */";
            }
        } else if ( baseType==Type::tIterator ) {
            if ( type->firstType ) {
                stream << "TIterator<" << describeCppType(type->firstType,substituteRef,skipRef,skipConst) << ">";
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
            for ( uint32_t fi=0; fi!=info->fieldsSize; ++fi ) {
                ss << "VarInfo " << structInfoName(info) << "_field_" << fi << " =  { ";
                describeCppVarInfo(ss, info->fields[fi]);
                ss << " };\n";
            }
            ss << "VarInfo * " << structInfoName(info) << "_fields[" << info->fieldsSize << "] =  { ";
            for ( uint32_t fi=0; fi!=info->fieldsSize; ++fi ) {
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
            ss << info->fieldsSize << ", ";
            ss << info->size << ", ";
            ss << info->initializer << ", ";
            ss << "0x" << HEX << info->hash << DEC;
        }

        void describeCppEnumInfoValues ( TextWriter & ss, EnumInfo * einfo ) const {
            for ( uint32_t v=0; v!=einfo->totalValues; ++v ) {
                auto val = einfo->values[v];
                ss << "EnumValueInfo " << enumInfoName(einfo) << "_value_" << v << " = { \""
                << val->name << "\", " << val->value << " };\n";
            }
            ss << "EnumValueInfo * " << enumInfoName(einfo) << "_values [] = { ";
            for ( uint32_t v=0; v!=einfo->totalValues; ++v ) {
                if ( v ) ss << ", ";
                ss << "&" << enumInfoName(einfo) << "_value_" << v;
            }
            ss << " };\n";
        }

        void describeCppEnumInfo ( TextWriter & ss, EnumInfo * info ) const {
            ss << "\"" << info->name << "\", " << enumInfoName(info) << "_values, "
                << info->totalValues << ", 0x" << HEX << info->hash << DEC;
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
                ss << ", DAS_MAKE_ANNOTATION(\"" << info->annotation_or_name->module->name << "::" << info->annotation_or_name->name << "\")";
            } else {
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

    void describeLocalCppType ( TextWriter & ss, const TypeDeclPtr & vtype, bool substituteRef = true ) {
        if ( isLocalVec(vtype) ) {
            if ( vtype->constant ) ss << "const ";
            ss << "vec4f /*" << describeCppType(vtype,substituteRef) << "*/";
        } else {
            ss << describeCppType(vtype,substituteRef,false);
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
            for ( auto & var : expr->iteratorVariables ) {
                renameVariable(var.get());
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
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            if (stack.back()->finalList.size()) {    // only move from the block with finally
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
                variables[block].push_back(var.get());
                moved.insert(var.get());
            }
            renameVariable(var.get());
        }

    public:
        vector<ExprBlock *>                 stack;
        map<ExprBlock *,vector<Variable *>> variables;
    protected:
        map<Variable *,string>              rename;
        set<Variable *>                     moved;
    };

    string describeCppFunc ( Function * fn, BlockVariableCollector * collector, bool needName = true ) {
        TextWriter ss;
        describeLocalCppType(ss,fn->result,false);
        ss << " ";
        if ( needName ) {
            ss << aotFuncName(fn);
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
        CppAot ( const ProgramPtr & prog, BlockVariableCollector & cl ) : program(prog), collector(cl) {}
        string str() const {
            return "\n" + helper.str() + sti.str() + ss.str();
        };
    protected:
        TextWriter                  ss, sti;
        int                         lastNewLine = -1;
        int                         tab = 0;
        int                         debugInfoGlobal = 0;
        AotDebugInfoHelper          helper;
        ProgramPtr                  program;
        BlockVariableCollector &    collector;
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
            ss << "\nstruct " << that->name << " {\n";
        }
        virtual void preVisitStructureField ( Structure * that, Structure::FieldDeclaration & decl, bool last ) override {
            Visitor::preVisitStructureField(that, decl, last);
            ss << "\t" << describeCppType(decl.type) << " " << decl.name << ";";
            if ( decl.parentType ) {
                ss << " /* from " << that->parent->name << " */";
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
            if ( !var->used ) ss << "/* ";
            ss << (var->init ? "das_global" : "das_global_zero");
            ss << "<" << describeCppType(var->type,false,true)
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
            ss << "\n";
            describeLocalCppType(ss,fn->result,false);
            ss << " " << aotFuncName(fn) << " ( Context * __context__";
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
            block->finallyBeforeBody = true;
            block->finallyDisabled = block->inTheLoop;
            ss << "{\n";
            tab ++;
            // pre-declare variables
            auto & vars = collector.variables[block];
            for ( auto & var : vars ) {
                ss << string(tab,'\t');
                describeLocalCppType(ss, var->type);
                ss << " " << collector.getVarName(var) << ";\n";
            }
        }
        virtual void preVisitBlockArgumentInit ( ExprBlock * block, const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitBlockArgumentInit(block, var, init);
            ss << " /* ";
        }
        virtual ExpressionPtr visitBlockArgumentInit ( ExprBlock * block, const VariablePtr & var, Expression * init ) override { 
            ss << " */ ";
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
                describeLocalCppType(ss, var->type);
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
        }
        virtual ExpressionPtr visitLetInit ( ExprLet * let , const VariablePtr & var, Expression * expr ) override {
            if ( expr->type->isPointer() && (!expr->type->firstType || expr->type->firstType->isVoid()) ) {
                ss << ")";
            }
            if ( var->type->ref ) {
                ss << ")";
            }
            return Visitor::visitLetInit(let, var, expr);
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
                    ss << "cast<" << describeCppType(that->type,false,true,true) << ">::to(";
                }
                outPolicy(pt);
                ss << "::" << opPolicyName(that) << "(";
                if ( that->left->type->ref ) {
                    ss << "(char *)&(";
                } else if ( policyArgNeedCast(pt, that->left->type) ) {
                    ss << "cast<" << describeCppType(that->left->type,false,true,true) << ">::from(";
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
                if ( that->left->type->ref ) {
                    ss << ")";
                } else if ( policyArgNeedCast(pt, that->left->type) ) {
                    ss << ")";
                }
                ss << ",";
                if ( policyArgNeedCast(pt, that->right->type) ) {
                    ss << "cast<" << describeCppType(that->right->type,false,true,true) << ">::from(";
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
                if ( policyArgNeedCast(pt, that->right->type) )ss << ")";
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
                ss << "cast_vec_ref<" << describeCppType(argT,false,true,true) << ">::to";
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
                ss << "cast_vec_ref<" << describeCppType(argT,false,true,true) << ">::to";
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
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            if ( var->local && var->variable->type->ref ) {
                ss << "(*" << collector.getVarName(var->variable) << ")";
            } else if ( var->local || var->block || var->argument ) {
                ss << collector.getVarName(var->variable);
            } else {
                ss << "das_global<" << describeCppType(var->variable->type,false,true)
                    << "," << int32_t(var->variable->stackTop) << ">(__context__) /*" << var->name << "*/";
            }
            return Visitor::visit(var);
        }
    // null coaelescing
        virtual void preVisit ( ExprNullCoalescing * nc ) override {
            Visitor::preVisit(nc);
            ss << "das_null_coalescing<" << describeCppType(nc->defaultValue->type) << ">::get(";
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
            if ( field->skipQQ ) ss << "_ptr";
            ss << "<" << describeCppType(field->value->type->firstType) << ",";
            if ( field->skipQQ ) {
                ss << describeCppType(field->type);
            } else {
                ss << describeCppType(field->type->firstType);
            }
            ss << ",&" << field->value->type->firstType->structType->name << "::" << field->name <<  ">::get(";
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
                    ss << "das_swizzle_seq<"
                    << describeCppType(expr->type,false,true) << ","
                    << describeCppType(expr->value->type,false,true) << ","
                    << int32_t(expr->fields[0]) << ">::swizzle(";
                } else {
                    ss << "das_swizzle<"
                        << describeCppType(expr->type,false,true) << ","
                        << describeCppType(expr->value->type,false,true);
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
    // addr
        virtual void preVisit ( ExprAddr * expr ) override {
            if (expr->func) {
                ss << "Func(" << (expr->func->index + 1) << ")";
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
            ss <<  describeCppType(edel->subexpr->type,false,true,true);
            ss << ">::clear(__context__,";
        }
        virtual ExpressionPtr visit ( ExprDelete * edel ) override {
            ss << ")";
            return Visitor::visit(edel);
        }
    // ascend
        virtual void preVisit ( ExprAscend * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->ascType ) {
                ss << "das_ascend<" << describeCppType(expr->type,false,true,true) << "," 
                    << describeCppType(expr->subexpr->type,false,true,true) << ">::make(__context__,";
            } else {
                ss << "das_ascend /* untyped */ ";
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
                ss << describeCppType(enew->type->firstType,false,true,true);
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
                ss << "das_new<" << describeCppType(enew->type->firstType,false,true,true);
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
            if ( block->annotationDataIndex != -1 ) {
                ss << "__context__->annotationData[" << block->annotationDataIndex << "u]";
            } else {
                ss << "0";
            }
            ss << ",[&](";
            int ai = 0;
            for ( auto & arg : block->arguments ) {
                if (ai++) ss << ", ";
                if (isLocalVec(arg->type)) {
                    describeLocalCppType(ss, arg->type);
                } else {
                    ss << describeCppType(arg->type);
                    if ( arg->type->isRefType() && !arg->type->ref ) {
                        ss << " &";
                    }
                }
                ss << " " << collector.getVarName(arg);
            }
            ss << ")->" << describeCppType(block->returnType);
        }
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }

/*
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
 */

    // looks like call
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            if (call->name == "debug" ) {
                auto argType = call->arguments[0]->type;
                TypeInfo * info = helper.makeTypeInfo(nullptr, argType);
                ss << "das_debug(__context__,&" << helper.typeInfoName(info) << ",__FILE__,__LINE__,";
                ss << "cast<" << describeCppType(argType) << ">::from(";
            } else if (call->name == "assert") {
                if ( call->arguments.size()==1 ) ss << "DAS_ASSERT((";
                else ss << "DAS_ASSERTF((";
            } else if (call->name == "erase") {
                ss << "__builtin_table_erase(__context__,";
            } else if (call->name == "find") {
                ss << "__builtin_table_find(__context__,";
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
                    if ( needsArgPass(argType) ) {
                        ss << "das_arg<" << describeCppType(argType,false,true) << ">::pass(";
                    }
                } else if (isVecRef(argType)) {
                    ss << "cast_vec_ref<" << describeCppType(argType,false,true,true) << ">::to(";
                }
            }
        }
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) override {
            if ( call->name=="invoke" ) {
                auto argType = arg->type;
                if ( arg->type->isRefType() ) {
                    if ( needsArgPass(argType) ) {
                        ss << ")";
                    }
                } else if (isVecRef(argType)) {
                    ss << ")";
                }
            }
            if ( !last ) {
                if (call->name == "assert" || call->name=="debug") {
                    ss << "),(";
                } else {
                    ss << ",";
                }
            }
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprLooksLikeCall * call ) override {
            if ( call->name=="assert" || call->name=="debug" ) {
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
            if ( func->builtIn ) return false;
            if ( func->noAot ) return true;
            if ( func->module == program->thisModule.get() ) return false;
            return true;
        }
        bool needsArgPass ( const TypeDeclPtr & argType ) const {
            return !argType->constant;
        }
        void CallFunc_preVisit ( ExprCallFunc * call ) {
            Visitor::preVisit(call);
            if ( call->func->result->aotAlias ) {
                ss << "das_alias<" << call->func->result->alias << ">::from(";
            }
            if ( call->func->builtIn ) {
                auto bif = static_cast<BuiltInFunction *>(call->func);
                if ( !call->arguments.size() && call->type->baseType==Type::tHandle ) {
                    // c-tor?
                    ss << "/*c-tor*/ ";
                } else if ( bif->policyBased ) {
                    outPolicy(call->type);
                    ss << "::";
                }
                if ( bif->interopFn ) {
                    ss << "das_call_interop<";
                    ss << describeCppType(call->func->result);
                    ss << ">::call(&";
                }
                if ( bif->cppName.empty() ) {
                    ss << bif->name;
                } else {
                    ss << bif->cppName;
                }
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
                    if ( call->arguments.size()>1 ) {
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
                        ss << "Func(" << (call->func->index+1) << "/* " << call->name << " */),";
                    } else {
                        ss << "(__context__,";
                        ss << "Func(" << (call->func->index+1) << "/* " << call->name << " */)";
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
                if ( needsArgPass(argType) ) {
                    ss << "das_arg<" << describeCppType(argType,false,true) << ">::pass(";
                }
            } else if (isVecRef(argType)) {
                ss << "cast_vec_ref<" << describeCppType(argType,false,true,true) << ">::to(";
            }
            if ( isPolicyBasedCall(call) && policyArgNeedCast(call->type, argType) ) {
                ss << "cast<" << describeCppType(argType,false,true,true) << ">::from(";
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
                if ( needsArgPass(argType) ) {
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
            ss << string(tab,'\t') << "das_iterator<" << describeCppType(ffor->sources[idx]->type,true,true,true)
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
            ss << string(tab,'\t') << describeCppType(var->type,true,false,true) << " " << collector.getVarName(var) << ";\n";
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
        for ( int i=0; i!=context.totalFunctions; ++i ) {
            if ( fnn[i]->module != thisModule.get() )
                continue;
            if ( fnn[i]->noAot )
                continue;
            SimFunction * fn = context.getFunction(i);
            uint64_t semH = fnn[i]->hash;
            logs << "\t// " << aotFuncName(fnn[i]) << "\n";
            logs << "\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            if ( fnn[i]->copyOnReturn || fnn[i]->moveOnReturn ) {
                logs << "CMRES";
            }
            logs << "<" << describeCppFunc(fnn[i],nullptr,false) << ",";
            logs << aotFuncName(fnn[i]) << ">>();\n\t};\n";
        }
        if ( context.totalVariables ) {
            uint64_t semH = context.getInitSemanticHash();
            logs << "\t// [[ init script ]]\n";
            logs << "\taotLib[0x" << HEX << semH << DEC << "] = [&](Context & ctx){\n\t\treturn ";
            logs << "ctx.code->makeNode<SimNode_Aot";
            logs << "<void (Context *),";
            logs << "__init_script>>();\n\t};\n";
        }
        if ( headers ) {
            logs << "}\n";
        }
    }

    void Program::aotCpp ( Context & context, TextWriter & logs ) {
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
        visit(aotVisitor);
        logs << aotVisitor.str();
    }
}

