#pragma once

#include "daScript/simulate/bind_enum.h"

DAS_BIND_ENUM_CAST(RefMatters);
DAS_BIND_ENUM_CAST(ConstMatters);
DAS_BIND_ENUM_CAST(TemporaryMatters);

DAS_BIND_ENUM_CAST_98(Type);

namespace das {
    vec4f rtti_getTypeInfo ( Context & , SimNode_CallBase *, vec4f * );
    vec4f rtti_contextFunctionInfo ( Context & context, SimNode_CallBase *, vec4f * );
    vec4f rtti_contextVariableInfo ( Context & context, SimNode_CallBase *, vec4f * );

    void builtin_make_arg_names_iterator(Sequence & result, const TypeInfo & info, Context * context);
    void builtin_make_arg_types_iterator(Sequence & result, const TypeInfo & info, Context * context);
}

