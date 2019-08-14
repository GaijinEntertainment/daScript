#pragma once

namespace das {
    vec4f rtti_getTypeInfo ( Context & , SimNode_CallBase *, vec4f * );
    vec4f rtti_contextFunctionInfo ( Context & context, SimNode_CallBase *, vec4f * );
    vec4f rtti_contextVariableInfo ( Context & context, SimNode_CallBase *, vec4f * );
}

