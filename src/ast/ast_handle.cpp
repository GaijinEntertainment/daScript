#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

namespace das {

    void BasicStructureAnnotation::seal( Module * m ) {
        TypeAnnotation::seal(m);
        mlib = nullptr;
    }

    TypeDeclPtr BasicStructureAnnotation::makeFieldType ( const string & na ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() ) {
            auto t = make_smart<TypeDecl>(*it->second.decl);
            if ( it->second.offset != -1U ) {
                t->ref = true;
            }
            return t;
        } else {
            return nullptr;
        }
    }

    TypeDeclPtr BasicStructureAnnotation::makeSafeFieldType ( const string & na ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() && it->second.offset!=-1U ) {
            return make_smart<TypeDecl>(*it->second.decl);
        } else {
            return nullptr;
        }
    }

    SimNode * BasicStructureAnnotation::simulateGetField ( const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() ) {
            if ( value->type->isPointer() ) {
                return it->second.factory(FactoryNodeType::getFieldPtr,context,at,value);
            } else {
                return it->second.factory(FactoryNodeType::getField,context,at,value);
            }
        } else {
            return nullptr;
        }
    }

    SimNode * BasicStructureAnnotation::simulateGetFieldR2V ( const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() ) {
            auto itT = it->second.decl;
            if ( value->type->isPointer() ) {
                return it->second.factory(FactoryNodeType::getFieldPtrR2V,context,at,value);
            } else {
                return it->second.factory(FactoryNodeType::getFieldR2V,context,at,value);
            }
        } else {
            return nullptr;
        }
    }

    SimNode * BasicStructureAnnotation::simulateSafeGetField ( const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() ) {
            return it->second.factory(FactoryNodeType::safeGetField,context,at,value);
        } else {
            return nullptr;
        }
    }

    SimNode * BasicStructureAnnotation::simulateSafeGetFieldPtr ( const string & na, Context & context,
            const LineInfo & at, const ExpressionPtr & value ) const {
        auto it = fields.find(na);
        if ( it!=fields.end() ) {
            return it->second.factory(FactoryNodeType::safeGetFieldPtr,context,at,value);
        } else {
            return nullptr;
        }
    }

    void BasicStructureAnnotation::aotVisitGetField ( TextWriter & ss, const string & fieldName ) {
        auto it = fields.find(fieldName);
        if (it != fields.end()) {
            ss << "." << it->second.cppName;
        } else {
            ss << "." << fieldName << " /*undefined */";
        }
    }

    void BasicStructureAnnotation::aotVisitGetFieldPtr ( TextWriter & ss, const string & fieldName ) {
        auto it = fields.find(fieldName);
        if (it != fields.end()) {
            ss << "->" << it->second.cppName;
        } else {
            ss << "->" << fieldName << " /*undefined */";
        }
    }

    void BasicStructureAnnotation::addFieldEx ( const string & na, const string & cppNa, off_t offset, TypeDeclPtr pT ) {
        auto & field = fields[na];
        if ( field.decl ) {
            DAS_FATAL_LOG("structure field %s already exist in structure %s\n", na.c_str(), name.c_str() );
            DAS_FATAL_ERROR;
        }
        field.cppName = cppNa;
        field.decl = pT;
        field.offset = offset;
        auto baseType = field.decl->baseType;
        field.factory = [offset,baseType](FactoryNodeType nt,Context & context,const LineInfo & at, const ExpressionPtr & value) -> SimNode * {
            if ( !value->type->isPointer() ) {
                if ( nt==FactoryNodeType::getField || nt==FactoryNodeType::getFieldR2V ) {
                    auto r2vType = (nt==FactoryNodeType::getField) ? Type::none : baseType;
                    auto tnode = value->trySimulate(context, offset, r2vType);
                    if ( tnode ) {
                        return tnode;
                    }
                }
            }
            auto simV = value->simulate(context);
            switch ( nt ) {
            case FactoryNodeType::getField:
                return context.code->makeNode<SimNode_FieldDeref>(at,simV,offset);
            case FactoryNodeType::getFieldR2V:
                return context.code->makeValueNode<SimNode_FieldDerefR2V>(baseType,at,simV,offset);
            case FactoryNodeType::getFieldPtr:
                return context.code->makeNode<SimNode_PtrFieldDeref>(at,simV,offset);
            case FactoryNodeType::getFieldPtrR2V:
                return context.code->makeValueNode<SimNode_PtrFieldDerefR2V>(baseType,at,simV,offset);
            case FactoryNodeType::safeGetField:
                return context.code->makeNode<SimNode_SafeFieldDeref>(at,simV,offset);
            case FactoryNodeType::safeGetFieldPtr:
                return context.code->makeNode<SimNode_SafeFieldDerefPtr>(at,simV,offset);
            default:
                return nullptr;
            }
        };
    }

    void BasicStructureAnnotation::walk ( DataWalker & walker, void * data ) {
        if ( !sti ) {
            auto debugInfo = helpA.debugInfo;
            sti = debugInfo->template makeNode<StructInfo>();
            sti->name = debugInfo->allocateName(name);
            // count fields
            sti->count = 0;
            for ( auto & fi : fields ) {
                auto & var = fi.second;
                if ( var.offset != -1U ) {
                    sti->count ++;
                }
            }
            // and allocate
            sti->size = (uint32_t) getSizeOf();
            sti->fields = (VarInfo **) debugInfo->allocate( sizeof(VarInfo *) * sti->count );
            int i = 0;
            for ( auto & fi : fields ) {
                auto & var = fi.second;
                if ( var.offset != -1U ) {
                    VarInfo * vi = debugInfo->template makeNode<VarInfo>();
                    helpA.makeTypeInfo(vi, var.decl);
                    vi->name = debugInfo->allocateName(fi.first);
                    vi->offset = var.offset;
                    sti->fields[i++] = vi;
                }
            }
        }
        walker.walk_struct((char *)data, sti);
    }
}
