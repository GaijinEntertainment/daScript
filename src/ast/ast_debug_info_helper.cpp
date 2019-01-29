#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    FuncInfo * DebugInfoHelper::makeFunctionDebugInfo ( const Function & fn ) {
        string mangledName = fn.getMangledName();
        auto it = fmn2f.find(mangledName);
        if ( it!=fmn2f.end() ) return it->second;
        FuncInfo * fni = debugInfo.makeNode<FuncInfo>();
        fni->name = debugInfo.allocateName(fn.name);
        fni->stackSize = fn.totalStackSize;
        fni->argsSize = (uint32_t) fn.arguments.size();
        fni->args = (VarInfo **) debugInfo.allocate(sizeof(VarInfo *) * fni->argsSize);
        for ( uint32_t i=0; i!=fni->argsSize; ++i ) {
            fni->args[i] = makeVariableDebugInfo(*fn.arguments[i]);
        }
        fmn2f[mangledName] = fni;
        return fni;
    }

    StructInfo * DebugInfoHelper::makeStructureDebugInfo ( const Structure & st ) {
        string mangledName = st.getMangledName();
        auto it = smn2s.find(mangledName);
        if ( it!=smn2s.end() ) return it->second;
        StructInfo * sti = debugInfo.makeNode<StructInfo>();
        sti->name = debugInfo.allocateName(st.name);
        sti->fieldsSize = (uint32_t) st.fields.size();
        sti->size = st.getSizeOf();
        sti->fields = (VarInfo **) debugInfo.allocate( sizeof(VarInfo *) * sti->fieldsSize );
        for ( uint32_t i=0; i!=sti->fieldsSize; ++i ) {
            auto & var = st.fields[i];
            VarInfo * vi = debugInfo.makeNode<VarInfo>();
            makeTypeInfo(vi, var.type);
            vi->name = debugInfo.allocateName(var.name);
            vi->offset = st.fields[i].offset;
            sti->fields[i] = vi;
        }
        smn2s[mangledName] = sti;
        return sti;
    }

    TypeInfo * DebugInfoHelper::makeTypeInfo ( TypeInfo * info, const TypeDeclPtr & type ) {
        if ( info==nullptr ) {
            string mangledName = type->getMangledName();
            auto it = tmn2t.find(mangledName);
            if ( it!=tmn2t.end() ) return it->second;
            info = debugInfo.makeNode<TypeInfo>();
            tmn2t[mangledName] = info;
        }
        info->type = type->baseType;
        info->dimSize = (uint32_t) type->dim.size();
        info->annotation = type->annotation.get();
        if ( info->dimSize ) {
            info->dim = (uint32_t *) debugInfo.allocate(sizeof(uint32_t) * info->dimSize );
            for ( uint32_t i=0; i != info->dimSize; ++i ) {
                info->dim[i] = type->dim[i];
            }
        }
        if ( type->baseType==Type::tStructure  ) {
            info->structType = makeStructureDebugInfo(*type->structType);
        }
        info->ref = type->ref;
        info->refType = type->isRefType();
        if ( type->isRefType() )
            info->ref = false;
        info->canCopy = type->canCopy();
        info->isPod = type->isPod();
        if ( type->firstType ) {
            info->firstType = makeTypeInfo(nullptr, type->firstType);
        } else {
            info->firstType = nullptr;
        }
        if ( type->secondType ) {
            info->secondType = makeTypeInfo(nullptr, type->secondType);
        } else {
            info->secondType = nullptr;
        }
        return info;
    }

    VarInfo * DebugInfoHelper::makeVariableDebugInfo ( const Variable & var ) {
        string mangledName = var.getMangledName();
        auto it = vmn2v.find(mangledName);
        if ( it!=vmn2v.end() ) return it->second;
        VarInfo * vi = debugInfo.makeNode<VarInfo>();
        makeTypeInfo(vi, var.type);
        vi->name = debugInfo.allocateName(var.name);
        vi->offset = 0;
        vmn2v[mangledName] = vi;
        return vi;
    }

}
