#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    EnumInfo * DebugInfoHelper::makeEnumDebugInfo ( const Enumeration & en ) {
        auto mangledName = en.getMangledName();
        auto it = emn2e.find(mangledName);
        if ( it!=emn2e.end() ) return it->second;
        EnumInfo * eni = debugInfo->makeNode<EnumInfo>();
        eni->name = debugInfo->allocateName(en.name);
        eni->totalValues = uint32_t(en.list.size());
        eni->values = (EnumValueInfo **) debugInfo->allocate(sizeof(EnumValueInfo *) * eni->totalValues);
        uint32_t i = 0;
        for ( auto & ev : en.list ) {
            eni->values[i] = (EnumValueInfo *) debugInfo->allocate(sizeof(EnumValueInfo));
            eni->values[i]->name = debugInfo->allocateName(ev.first);
            eni->values[i]->value = ev.second;
            i ++;
        }
        eni->hash = hash_value(eni);
        emn2e[mangledName] = eni;
        return eni;
    }

    FuncInfo * DebugInfoHelper::makeFunctionDebugInfo ( const Function & fn ) {
        string mangledName = fn.getMangledName();
        auto it = fmn2f.find(mangledName);
        if ( it!=fmn2f.end() ) return it->second;
        FuncInfo * fni = debugInfo->makeNode<FuncInfo>();
        fni->name = debugInfo->allocateName(fn.name);
        fni->stackSize = fn.totalStackSize;
        fni->argsSize = (uint32_t) fn.arguments.size();
        fni->args = (VarInfo **) debugInfo->allocate(sizeof(VarInfo *) * fni->argsSize);
        for ( uint32_t i=0; i!=fni->argsSize; ++i ) {
            fni->args[i] = makeVariableDebugInfo(*fn.arguments[i]);
        }
        fni->result = makeTypeInfo(nullptr, fn.result);
        fni->hash = hash_value(fni);
        fmn2f[mangledName] = fni;
        return fni;
    }

    StructInfo * DebugInfoHelper::makeStructureDebugInfo ( const Structure & st ) {
        string mangledName = st.getMangledName();
        auto it = smn2s.find(mangledName);
        if ( it!=smn2s.end() ) return it->second;
        StructInfo * sti = debugInfo->makeNode<StructInfo>();
        sti->name = debugInfo->allocateName(st.name);
        sti->fieldsSize = (uint32_t) st.fields.size();
        sti->size = st.getSizeOf();
        sti->fields = (VarInfo **) debugInfo->allocate( sizeof(VarInfo *) * sti->fieldsSize );
        for ( uint32_t i=0; i!=sti->fieldsSize; ++i ) {
            auto & var = st.fields[i];
            VarInfo * vi = makeVariableDebugInfo(st, var);
            sti->fields[i] = vi;
        }
        sti->initializer = -1;
        if ( st.module ) {
            if ( auto fn = st.module->findFunction(st.name) ) {
                sti->initializer = fn->index;
            }
        }
        sti->hash = hash_value(sti);
        smn2s[mangledName] = sti;
        return sti;
    }

    TypeInfo * DebugInfoHelper::makeTypeInfo ( TypeInfo * info, const TypeDeclPtr & type ) {
        if ( info==nullptr ) {
            string mangledName = type->getMangledName();
            auto it = tmn2t.find(mangledName);
            if ( it!=tmn2t.end() ) return it->second;
            info = debugInfo->makeNode<TypeInfo>();
            tmn2t[mangledName] = info;
        }
        info->type = type->baseType;
        info->dimSize = (uint32_t) type->dim.size();
        info->annotation_or_name = type->annotation.get();
        if ( info->dimSize ) {
            info->dim = (uint32_t *) debugInfo->allocate(sizeof(uint32_t) * info->dimSize );
            for ( uint32_t i=0; i != info->dimSize; ++i ) {
                info->dim[i] = type->dim[i];
            }
        }
        if ( type->baseType==Type::tStructure  ) {
            info->structType = makeStructureDebugInfo(*type->structType);
        } else {
            info->structType = nullptr;
        }
        if ( type->baseType==Type::tEnumeration ) {
            info->enumType = makeEnumDebugInfo(*type->enumType);
        } else {
            info->enumType = nullptr;
        }
        info->flags = 0;
        if (type->ref)
            info->flags |= TypeInfo::flag_ref;
        if (type->isRefType())
            info->flags |= TypeInfo::flag_refType;
        if ( type->isRefType() )
            info->flags &= ~TypeInfo::flag_ref;
        if (type->canCopy())
            info->flags |= TypeInfo::flag_canCopy;
        if (type->isPod())
            info->flags |= TypeInfo::flag_isPod;
        if (type->isConst())
            info->flags |= TypeInfo::flag_isConst;
        if (type->isRawPod())
            info->flags |= TypeInfo::flag_isRawPod;
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
        info->hash = hash_value(info);
        debugInfo->lookup[info->hash] = info;
        return info;
    }

    VarInfo * DebugInfoHelper::makeVariableDebugInfo ( const Structure & st, const Structure::FieldDeclaration & var ) {
        string mangledName = st.getMangledName() + " field " + var.name;
        auto it = vmn2v.find(mangledName);
        if ( it!=vmn2v.end() ) return it->second;
        VarInfo * vi = debugInfo->makeNode<VarInfo>();
        makeTypeInfo(vi, var.type);
        vi->name = debugInfo->allocateName(var.name);
        vi->offset = var.offset;
        vmn2v[mangledName] = vi;
        return vi;
    }

    VarInfo * DebugInfoHelper::makeVariableDebugInfo ( const Variable & var ) {
        string mangledName = var.getMangledName();
        auto it = vmn2v.find(mangledName);
        if ( it!=vmn2v.end() ) return it->second;
        VarInfo * vi = debugInfo->makeNode<VarInfo>();
        makeTypeInfo(vi, var.type);
        vi->name = debugInfo->allocateName(var.name);
        vi->offset = 0;
        vmn2v[mangledName] = vi;
        return vi;
    }

}
