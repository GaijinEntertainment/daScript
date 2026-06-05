options gen2
options no_unused_block_arguments
options no_unused_function_arguments = false
module shader_dsl_boost shared private

require daslib/strings_boost
require daslib/ast_boost
require daslib/templates_boost
require shader_graph_ir_builder
require daslib/flatten
require daslib/fio

// ---------------------------------------------------------------------------
// EdenSpark shader-DSL backend (flatten example harness copy).
//
// Adapted from the engine's shader_dsl_boost. Two changes for this harness:
//   (1) the [shader(stage=,type=)] annotation calls daslib/flatten's
//       `flatten_function` in patch() BEFORE validation, so a shader written
//       with if/else, helper calls or fixed loops is lowered to branchless,
//       call-free `?:`/straight-line form first. flatten does ALL the lowering
//       (inlining + if-conversion + loop unroll); the old UnrollForVisitor is
//       kept only for reference.
//   (2) Phase B (capability tier) adds ExprOp3 -> Select so the backend can
//       consume flatten's `?:` output. The branchless real shaders never emit
//       `?:`, so the regression tier is unaffected.
// ---------------------------------------------------------------------------

def warning_at(message : string, at : LineInfo) {
    print("shader_dsl_boost: warning at {at.describe()}: {message}\n")
}

let DEBUG_AT = false
let PRIMITIVES_MODULE_NAME = "shader_dsl_primitives"

// Per-contract I/O struct names in PRIMITIVES_MODULE_NAME.
let CONTRACT_INPUT_STRUCTS : table<string; string> = {
    "pbr"   => "PbrInput",
    "unlit" => "UnlitInput"
}
let CONTRACT_OUTPUT_STRUCTS : table<string; string> = {
    "pbr"   => "PbrOutput",
    "unlit" => "UnlitOutput"
}

// Leaf primitives flatten must KEEP (not inline). These are the shader DSL's
// [hint] primitives plus the vector constructors the backend lowers directly.
// flatten matches by call name, so list function names (not the hint typeIds).
let HINT_WHITELIST : table<string> <- {
    "tex2d", "noise",
    "abs", "min", "max", "clamp", "saturate", "frac", "floor", "ceil", "mad", "lerp",
    "sqrt", "pow", "sin", "cos", "atan2", "log", "exp", "sign",
    "dot", "cross", "length", "normalize", "fresnel",
    "remap", "one_minus", "step", "smooth_step", "unpack_normal",
    "float", "int",
    "float2", "float3", "float4", "int2", "int3", "int4"
}

let UV = "uv";
let GTime = "gTime";
let WorldPos = "worldPos";
let WorldNormal = "worldNormal";
let ViewDir = "viewDir";
let LocalPos = "localPos";
let LocalNormal = "localNormal";

// ---- Outputs ---------------------------------------------------------------
let OutAlbedo = "out_albedo";
let OutAlpha = "out_alpha";
let OutAlphaCutoff = "out_alphaCutoff";
let OutMetalness = "out_metalness";
let OutRoughness = "out_roughness";
let OutEmission = "out_emission";
let OutNormalMap = "out_normalMap";
let OutEmissionStr = "out_emissionStr";
let OutAO = "out_ao";

// ---- Constants -------------------------------------------------------------
let FloatConst = "floatConst";
let Float2Const = "float2Const";
let Float3Const = "float3Const";
let Float4Const = "float4Const";

// ---- Variables -------------------------------------------------------------
let FloatVar = "floatVar";
let Float2Var = "float2Var";
let Float3Var = "float3Var";
let Float4Var = "float4Var";
let ColorVar = "colorVar";
let TextureVar = "textureVar";

// ---- Ops -------------------------------------------------------------------
let SampleTexture = "sampleTexture";
let PerlinNoise2D = "perlinNoise2D";
let PerlinNoise3D = "perlinNoise3D";

// ---- Generic (AnyFloat) ops ------------------------------------------------
let Add = "add";
let Mul = "mul";
let Sub = "sub";
let Div = "div";
let Neg = "neg";
let Abs = "abs";
let Min = "min";
let Max = "max";
let Clamp = "clamp";
let Sat = "sat";
let Frac = "frac";
let Floor = "floor";
let Ceil = "ceil";
let Mad = "mad";
let Lerp = "lerp";

// ---- Transcendentals -------------------------------------------------------
let Sqrt = "sqrt";
let Pow = "pow";
let Sin = "sin";
let Cos = "cos";
let Atan2 = "atan2";
let Log = "log";
let Exp = "exp";
let Sign = "sign";

// ---- Dot -------------------------------------------------------------------
let DotF2 = "dot_f2";
let DotF3 = "dot_f3";
let DotF4 = "dot_f4";

// ---- Cross -----------------------------------------------------------------
let CrossF3 = "cross_f3";

// ---- Length ----------------------------------------------------------------
let LenF2 = "len_f2";
let LenF3 = "len_f3";
let LenF4 = "len_f4";

// ---- Normalize -------------------------------------------------------------
let NormF2 = "norm_f2";
let NormF3 = "norm_f3";
let NormF4 = "norm_f4";

// ---- Lighting / surface ops ------------------------------------------------
let Fresnel = "fresnel";

// ---- Utility ops -----------------------------------------------------------
let Remap = "remap";
let OneMinus = "oneMinus";
let Step = "step";
let SmoothStep = "smoothStep";
let UnpackNormal = "unpackNormal";

// ---- Select (Phase B: consumes flatten's ?:) -------------------------------
let Select = "select";
let CmpLt = "cmp_lt";
let CmpGt = "cmp_gt";
let CmpLe = "cmp_le";
let CmpGe = "cmp_ge";
let CmpEq = "cmp_eq";
let CmpNe = "cmp_ne";
let LogicAnd = "and";
let LogicOr = "or";
let LogicNot = "not";

// ---- Swizzle / channel ops -------------------------------------------------
let SplatX = "splatX";
let SplatY = "splatY";
let SplatZ = "splatZ";
let SplatW = "splatW";
let CombineFloat2 = "combineFloat2";
let CombineFloat3 = "combineFloat3";
let CombineFloat4 = "combineFloat4";
let CombineFloat4F2F2 = "combineFloat4F2F2";
let CombineFloat4F3F1 = "combineFloat4F3F1";
let SwizzleF2 = "swizzleF2";
let SwizzleF3 = "swizzleF3";
let SwizzleF4 = "swizzleF4";


enum ShaderPinType {
    Float
    Float2
    Float3
    Float4
    AnyFloat
    Texture2D
}


enum ShaderPropertyType {
    Float
    Float2
    Float3
    Float4
    Color
    Texture2D
}


struct IRPin {
    id : int
    _type : ShaderPinType
}


struct ResourceLink {
    name : string
}


struct ShaderGraphProperty {
    name : string
    _type : ShaderPropertyType
    defaultValue : float4
    defaultTexture : ResourceLink
}


enum ShaderNodeKind {
    Input
    Output
    Op
    Const
    Var
}


struct IRLink {
    fromPin : int
    toPin : int
}


struct IRNode {
    id : int = 0
    typeId : string
    kind : ShaderNodeKind
    constValue : float4
    propertyIndex : int

    inputs : array<IRPin?>
    outputs : array<IRPin?>

    at : string
}

typedef IRPinId = int

[macro]
class ValidateShaderVisitor : AstVisitor {
    irPinId : int = 0
    irNodeId : int = 0

    moduleName : string

    links : array<IRLink>
    props : array<ShaderGraphProperty>
    nodes : array<IRNode>

    errors : array<tuple<string, LineInfo const?>>

    ignoreNext : array<bool>

    propsIdx : int = 0

    @do_not_delete pins : table<IRPinId, IRPin?>
    @do_not_delete nodeExitPins : table<Expression const?, IRPinId>
    @do_not_delete exprLinks : array<tuple<IRPinId, Expression const?>>
    @do_not_delete variableLinks : array<tuple<IRPinId, Variable const?>>

    @do_not_delete variables : table<Variable const?, Expression const?>
    @do_not_delete variablePins : table<Variable const?, IRPinId>
    @do_not_delete aliasTargets : table<Variable const?, Variable const?>
    @do_not_delete outPins : table<Variable const?, IRPinId>
    @do_not_delete accumVars : table<Variable const?, bool>
    @do_not_delete accumCurPin : table<Variable const?, IRPinId>
    @do_not_delete inputFieldPins : table<string, IRPinId>
    @do_not_delete makeStructStates : array<int>

    contract : string

    def is_input_contract_struct(st : StructurePtr) : bool {
        if (st == null || st._module == null || st._module.name != PRIMITIVES_MODULE_NAME) { return false; }
        let expectedName = CONTRACT_INPUT_STRUCTS?[contract] ?? ""
        return !empty(expectedName) && st.name == expectedName
    }

    def is_output_contract_struct(st : StructurePtr) : bool {
        if (st == null || st._module == null || st._module.name != PRIMITIVES_MODULE_NAME) { return false; }
        let expectedName = CONTRACT_OUTPUT_STRUCTS?[contract] ?? ""
        return !empty(expectedName) && st.name == expectedName
    }

    static unsupportedExpr = {
        ("ExprStringBuilder" => "string interpolation is not supported in shader code"),
        ("ExprNew" => "heap allocations are not supported in shader code"),
        ("ExprLooksLikeCall" => "function calls without function identifiers are not supported in shader code"),
        ("ExprNullCoalescing" => "null coalescing operator (??) is not supported in shader code"),
        ("ExprAt" => "the at operator is not supported in shader code"),
        ("ExprSafeAt" => "safe at operator is not supported in shader code"),
        ("ExprIs" => "is operator is not supported in shader code"),
        ("ExprMove" => "move semantics are not supported in shader code"),
        ("ExprClone" => "clone operator is not supported in shader code"),
        ("ExprWith" => "the with operator is not supported in shader code"),
        ("ExprWhile" => "while loops are not supported in shader code"),
        ("ExprTryCatch" => "try-catch blocks are not supported in shader code"),
        ("ExprIfThenElse" => "if expressions are not supported in shader code"),
        ("ExprFor" => "for loops are not supported in shader code"),
        ("ExprMakeVariant" => "make variant expressions are not supported in shader code"),
        ("ExprMakeArray" => "make array expressions are not supported in shader code"),
        ("ExprMakeTuple" => "make tuple expressions are not supported in shader code"),
        ("ExprArrayComprehension" => "array comprehensions are not supported in shader code"),
        ("ExprTypeInfo" => "type info expressions are not supported in shader code"),
        ("ExprPtr2Ref" => "pointer to reference conversions are not supported in shader code"),
        ("ExprLabel" => "labels are not supported in shader code"),
        ("ExprGoto" => "goto statements are not supported in shader code"),
        ("ExprRef2Ptr" => "reference to pointer conversions are not supported in shader code"),
        ("ExprAddr" => "function address retrieval is not supported in shader code"),
        ("ExprAssert" => "assert statements are not supported in shader code"),
        ("ExprQuote" => "quote expressions are not supported in shader code"),
        ("ExprDebug" => "debug expressions are not supported in shader code"),
        ("ExprInvoke" => "function invocations are not supported in shader code"),
        ("ExprErase" => "erase function is not supported in shader code"),
        ("ExprSetInsert" => "set/insert function is not supported in shader code"),
        ("ExprFind" => "find function is not supported in shader code"),
        ("ExprKeyExists" => "key exists function is not supported in shader code"),
        ("ExprAscend" => "ascend function is not supported in shader code"),
        ("ExprCast" => "cast function is not supported in shader code"),
        ("ExprDelete" => "delete function is not supported in shader code"),
        ("ExprSafeField" => "safe field access is not supported in shader code"),
        ("ExprIsVariant" => "is variant function is not supported in shader code"),
        ("ExprAsVariant" => "as variant function is not supported in shader code"),
        ("ExprSafeAsVariant" => "safe as variant function is not supported in shader code"),
        ("ExprYield" => "yield statements are not supported in shader code"),
        ("ExprBreak" => "break statements are not supported in shader code"),
        ("ExprContinue" => "continue statements are not supported in shader code"),
        ("ExprMakeBlock" => "make block expressions are not supported in shader code"),
        ("ExprMakeGenerator" => "make generator expressions are not supported in shader code"),
        ("ExprMemZero" => "mem zero expressions are not supported in shader code"),
        ("ExprConstEnumeration" => "const enumeration expressions are not supported in shader code"),
        ("ExprConstBitfield" => "const bitfield expressions are not supported in shader code"),
        ("ExprConstInt8" => "int8 expressions are not supported in shader code"),
        ("ExprConstInt16" => "int16 expressions are not supported in shader code"),
        ("ExprConstInt64" => "int64 expressions are not supported in shader code"),
        ("ExprConstInt2" => "int2 expressions are not supported in shader code"),
        ("ExprConstInt3" => "int3 expressions are not supported in shader code"),
        ("ExprConstInt4" => "int4 expressions are not supported in shader code"),
        ("ExprConstUInt8" => "uint8 expressions are not supported in shader code"),
        ("ExprConstUInt16" => "uint16 expressions are not supported in shader code"),
        ("ExprConstUInt64" => "uint64 expressions are not supported in shader code"),
        ("ExprConstUInt" => "uint expressions are not supported in shader code"),
        ("ExprConstUInt2" => "uint2 expressions are not supported in shader code"),
        ("ExprConstUInt3" => "uint3 expressions are not supported in shader code"),
        ("ExprConstUInt4" => "uint4 expressions are not supported in shader code"),
        ("ExprConstRange" => "range expressions are not supported in shader code"),
        ("ExprConstURange" => "urange expressions are not supported in shader code"),
        ("ExprConstRange64" => "range64 expressions are not supported in shader code"),
        ("ExprConstURange64" => "urange64 expressions are not supported in shader code"),
        ("ExprConstString" => "string expressions are not supported in shader code"),
        ("ExprConstDouble" => "double expressions are not supported in shader code"),
        ("ExprReader" => "reader expressions are not supported in shader code"),
        ("ExprUnsafe" => "unsafe expressions are not supported in shader code"),
        ("ExprCallMacro" => "call macro expressions are not supported in shader code")
    }

    def add_in_link(from : ExpressionPtr&, to : IRPin?) {
        if (from is ExprVar) {
            let v : Variable const? = (from as ExprVar).variable
            if (accumVars.key_exists(v)) {
                let readId : Expression const? = from
                nodeExitPins[readId] = accumCurPin?[v] ?? -1
                exprLinks.push((to.id, readId))
            } else {
                variableLinks.push((to.id, v))
            }
        } else {
            let it : Expression const? = from
            exprLinks.push((to.id, it))
        }
    }

    def register_node(x : ExpressionPtr&, var irNode : IRNode&) {
        let id : Expression const? = x
        verify(irNode.outputs.length() <= 1)
        if (irNode.outputs.length() == 1) {
            nodeExitPins[id] = irNode.outputs[0].id
        }
        nodes.emplace(irNode)
        irNodeId++
    }

    def add_variable(x : VariablePtr&) {
        let id : Variable const? = x
        variables[id] = x.init
    }

    def pin_type_of(t : TypeDecl?) : ShaderPinType {
        if (t.baseType == Type.tFloat) { return ShaderPinType.Float; }
        if (t.baseType == Type.tFloat2) { return ShaderPinType.Float2; }
        if (t.baseType == Type.tFloat3) { return ShaderPinType.Float3; }
        if (t.baseType == Type.tFloat4) { return ShaderPinType.Float4; }
        if (t.baseType == Type.tStructure) {
            var st = t.structType
            if (st != null && st.name == "Sampler2D" && st._module != null && st._module.name == PRIMITIVES_MODULE_NAME) {
                return ShaderPinType.Texture2D
            }
        }
        return ShaderPinType.Float
    }

    def add_pin(expr : ExpressionPtr) : IRPin? {
        var pin = new IRPin(id = irPinId, _type = ShaderPinType.Float)
        pins[irPinId] = pin
        if (expr._type.baseType == Type.tFloat) {
            pin._type = ShaderPinType.Float
        } elif (expr._type.baseType == Type.tFloat2) {
            pin._type = ShaderPinType.Float2
        } elif (expr._type.baseType == Type.tFloat3) {
            pin._type = ShaderPinType.Float3
        } elif (expr._type.baseType == Type.tFloat4) {
            pin._type = ShaderPinType.Float4
        } elif (expr._type.baseType == Type.tBool) {
            // Phase B: a `?:` condition (or comparison result) is a bool mask.
            // The branchless backend treats it as a float 0/1 selector.
            pin._type = ShaderPinType.Float
        } elif (expr._type.baseType == Type.tStructure) {
            var st = expr._type.structType
            if (st.name == "Sampler2D" && st._module != null && st._module.name == PRIMITIVES_MODULE_NAME) {
                pin._type = ShaderPinType.Texture2D
            } else {
                errors.push(("unsupported expression type {describe(expr._type)} for pin creation", unsafe(addr(expr.at))))
            }
        }
        irPinId++
        return pin
    }

    def begin_op(expr : ExpressionPtr; typeId : string; kind : ShaderNodeKind = ShaderNodeKind.Op) : IRNode {
        var node = IRNode(id = irNodeId, typeId = typeId, kind = kind)
        if (DEBUG_AT) {
            node.at = describe(expr)
        }
        return <- node
    }

    def add_arg(var irNode : IRNode&; arg : ExpressionPtr) {
        var pin = add_pin(arg)
        irNode.inputs.push(pin)
        add_in_link(arg, pin)
    }

    def emit_noop(from : ExpressionPtr; to : ExpressionPtr) {
        var node = begin_op(to, "<noop>")
        add_arg(node, from)
        node.outputs.push(add_pin(to))
        register_node(to, node)
    }

    def emit_const_node(expr : ExpressionPtr; typeId : string; constValue : float4) {
        var node = begin_op(expr, typeId, ShaderNodeKind.Const)
        node.constValue = constValue
        node.outputs.push(add_pin(expr))
        register_node(expr, node)
    }

    def emit_vector_ctor(expr : ExprCall?; dims : int; combineTypeId : string) {
        let argc = expr.arguments.length()
        if (argc == 1 && expr.arguments[0]._type.baseType == Type.tFloat) {
            emit_noop(expr.arguments[0], expr)
            return
        }
        var node = begin_op(expr, combineTypeId)
        if (argc == 1) {
            for (_ in range(dims)) {
                add_arg(node, expr.arguments[0])
            }
        } else {
            for (arg in expr.arguments) {
                add_arg(node, arg)
            }
        }
        node.outputs.push(add_pin(expr))
        register_node(expr, node)
    }


    def override canVisitFunctionArgumentInit(fun : Function?; arg : VariablePtr; value : ExpressionPtr) : bool {
        return false
    }

    def override preVisitExprField(expr : ExprField?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        let isContractArg = ((expr.value is ExprVar)
            && (expr.value as ExprVar).varFlags.argument
            && (expr.value as ExprVar).variable._type.baseType == Type.tStructure
            && is_input_contract_struct((expr.value as ExprVar).variable._type.structType))
        if (!isContractArg) {
            errors.push(("field access is not supported in shader code", unsafe(addr(expr.at))))
            ignoreNext.push(true)
            return
        }
        let fieldName = "{expr.name}"
        let structType = (expr.value as ExprVar).variable._type.structType
        var typeId = ""
        for (field in structType.fields) {
            if (field.name == fieldName) {
                for (ann in field.annotation) {
                    if (ann.name == "alias") {
                        typeId = ann.sValue.string()
                        break
                    }
                }
                break
            }
        }
        if (typeId == "") {
            errors.push(("field '{fieldName}' on '{structType.name}' has no @alias annotation - cannot map to shader input", unsafe(addr(expr.at))))
            ignoreNext.push(true)
            return
        }
        if (inputFieldPins.key_exists(fieldName)) {
            let id : Expression const? = expr
            nodeExitPins[id] = inputFieldPins[fieldName]
            ignoreNext.push(true)
            return
        }
        var irNode = begin_op(expr, typeId, ShaderNodeKind.Input)
        var pin = add_pin(expr)
        irNode.outputs.push(pin)
        register_node(expr, irNode)
        inputFieldPins[fieldName] = pin.id
        ignoreNext.push(true)
    }

    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        if (!empty(ignoreNext)) {
            ignoreNext.pop()
        }
        return <- expr
    }

    def override preVisitExprReturn(expr : ExprReturn?) : void {
        if (!empty(ignoreNext)) { return ; }
        let allowSubexpr = (expr.subexpr != null
            && (expr.subexpr is ExprMakeStruct)
            && (expr.subexpr as ExprMakeStruct)._type.baseType == Type.tStructure
            && is_output_contract_struct((expr.subexpr as ExprMakeStruct)._type.structType))
        if (!allowSubexpr) {
            ignoreNext.push(true)
        }
    }

    def override visitExprReturn(var expr : ExprReturn?) : ExpressionPtr {
        if (!empty(ignoreNext)) {
            ignoreNext.pop()
        }
        return <- expr
    }

    def override preVisitExprMakeStruct(expr : ExprMakeStruct?) : void {
        if (!empty(ignoreNext)) {
            makeStructStates.push(2)
            return
        }
        if (expr._type.baseType != Type.tStructure || !is_output_contract_struct(expr._type.structType)) {
            errors.push(("make struct expressions are not supported in shader code", unsafe(addr(expr.at))))
            makeStructStates.push(1)
            ignoreNext.push(true)
            return
        }
        makeStructStates.push(0)
    }

    def override preVisitExprMakeStructField(expr : ExprMakeStruct?; index : int; decl : MakeFieldDeclPtr; last : bool) : void {
        if (!empty(ignoreNext)) { return ; }
        let fieldName = string(decl.name)
        let structType = expr._type.structType
        var typeId = ""
        for (sf in structType.fields) {
            if (sf.name == fieldName) {
                for (ann in sf.annotation) {
                    if (ann.name == "alias") {
                        typeId = ann.sValue.string()
                        break
                    }
                }
                break
            }
        }
        if (typeId == "") {
            errors.push(("output struct field '{fieldName}' has no @alias annotation - cannot map to shader output", unsafe(addr(expr.at))))
            return
        }
        var irNode = begin_op(decl.value, typeId, ShaderNodeKind.Output)
        add_arg(irNode, decl.value)
        register_node(decl.value, irNode)
    }

    def override visitExprMakeStructField(expr : ExprMakeStruct?; index : int; var decl : MakeFieldDeclPtr; last : bool) : MakeFieldDeclPtr {
        return <- decl
    }

    def override visitExprMakeStruct(var expr : ExprMakeStruct?) : ExpressionPtr {
        if (!empty(makeStructStates)) {
            let state = makeStructStates[makeStructStates.length() - 1]
            makeStructStates.pop()
            if (state == 1) {
                ignoreNext.pop()
            }
        }
        return <- expr
    }


    def override preVisitExpression(expr : ExpressionPtr) : void {
        if (is_in_completion() && expr.__rtti == "ExprRef2Value") {
            return
        }
        unsupportedExpr.get(expr.__rtti) $(message) {
            if (empty(ignoreNext)) {
                errors.push((message, unsafe(addr(expr.at))))
            }
            ignoreNext.push(true)
        }
    }

    def override visitExpression(var expr : ExpressionPtr) : ExpressionPtr {
        if (is_in_completion() && expr.__rtti == "ExprRef2Value") {
            return <- expr
        }
        if (unsupportedExpr.key_exists(expr.__rtti)) {
            ignoreNext.pop()
        }
        return <- expr
    }

    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        if (expr.left is ExprVar) {
            let v : Variable const? = (expr.left as ExprVar).variable
            if (accumVars.key_exists(v)) {
                let rhsId : Expression const? = expr.right
                let pin = nodeExitPins?[rhsId] ?? -1
                if (pin == -1) {
                    errors.push(("failed to resolve right-hand side of assignment to accumulator '{v.name}'", unsafe(addr(expr.at))))
                } else {
                    accumCurPin[v] = pin
                }
                return <- expr
            }
            if (outPins.key_exists(v)) {
                let outPin = pins[outPins[v]]
                if (pins.key_exists(outPin.id)) {
                    add_in_link(expr.right, outPin)
                } else {
                    let at : LineInfo const? = unsafe(addr(expr.at))
                    errors.push(("failed to resolve output pin for variable {v.name}", at))
                }
            } else {
                let at : LineInfo const? = unsafe(addr(expr.at))
                errors.push(("assignment to variable {v.name} is not supported in shader code", at))
            }
        }
        return <- expr
    }

    def override preVisitExprLet(expr : ExprLet?) : void {
        if (!empty(ignoreNext)) {
            return
        }

        for (v in expr.variables) {
            if (v.init == null) {
                errors.push(("shader variables must be initialized with a constant expression", unsafe(addr(v.at))))
                continue
            }
            if (v._type.isPointer) {
                errors.push(("shader code doesn't support pointer types", unsafe(addr(v.at))))
            }
            if (!empty(v._type.dim)) {
                errors.push(("shader code doesn't support vector literals; use float2/3/4 types instead", unsafe(addr(v.at))))
            }
            let isAccum = !v._type.isConst
            if (v._type.baseType == Type.tFloat || v._type.baseType == Type.tFloat2 || v._type.baseType == Type.tFloat3 || v._type.baseType == Type.tFloat4) {
                add_variable(v)
                if (isAccum) {
                    let accId : Variable const? = v
                    accumVars[accId] = true
                    if (v.init is ExprVar) {
                        errors.push(("a mutable shader accumulator must be initialized with a value expression, not aliased to another variable", unsafe(addr(v.at))))
                    }
                    continue
                }
                if (v.init is ExprVar) {
                    let aliasId : Variable const? = v
                    var target : Variable const? = (v.init as ExprVar).variable
                    if (aliasTargets.key_exists(target)) {
                        target = aliasTargets[target]
                    }
                    aliasTargets[aliasId] = target
                }
            } else {
                errors.push(("unsupported variable type {describe(v._type)}", unsafe(addr(v.at))))
            }
        }
    }

    def override visitExprLet(var expr : ExprLet?) : ExpressionPtr {
        if (empty(ignoreNext)) {
            for (v in expr.variables) {
                let accId : Variable const? = v
                if (accumVars.key_exists(accId) && v.init != null) {
                    let initId : Expression const? = v.init
                    let pin = nodeExitPins?[initId] ?? -1
                    if (pin == -1) {
                        errors.push(("shader accumulator '{v.name}' must be initialized with a constant or computed value", unsafe(addr(v.at))))
                    } else {
                        accumCurPin[accId] = pin
                    }
                }
            }
        }
        return <- expr
    }

    def override preVisitExprCall(expr : ExprCall?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        var kind = ShaderNodeKind.Op
        for (ann in expr.func.annotations) {
            if (ann.annotation.name == "marker") {
                for (arg in ann.arguments) {
                    if (arg.name.string() == "output") {
                        kind = ShaderNodeKind.Output
                    }
                }
                break
            }
        }
        for (ann in expr.func.annotations) {
            if (ann.annotation.name == "hint") {
                if (length(ann.arguments) != 1) {
                    errors.push(("hint annotation must have exactly one argument", unsafe(addr(expr.at))))
                    return
                }
                let typeId = ann.arguments[0].name.string()
                var irNode = begin_op(expr, typeId, kind)
                for (arg in expr.arguments) {
                    add_arg(irNode, arg)
                }
                if (!expr.func.result.isVoid) {
                    irNode.outputs.push(add_pin(expr))
                }
                register_node(expr, irNode)
                return
            }
        }
        if (expr._type.baseType == Type.tFloat && expr.arguments.length() == 1 && expr.func.name.string().find("float") >= 0
            && (expr.arguments[0]._type.baseType == Type.tFloat || expr.arguments[0]._type.baseType == Type.tInt)) {
            emit_noop(expr.arguments[0], expr)
            return
        }

        if (expr._type.baseType == Type.tFloat2 && expr.func.name.string().find("float2") >= 0) {
            if (expr.arguments.length() != 2 && expr.arguments.length() != 1) {
                errors.push(("float2 constructor requires 1 or 2 arguments", unsafe(addr(expr.at))))
                return
            }
            emit_vector_ctor(expr, 2, CombineFloat2)
            return
        }

        if (expr._type.baseType == Type.tFloat3 && expr.func.name.string().find("float3") >= 0) {
            let argc = expr.arguments.length()
            if (argc != 1 && argc != 3) {
                errors.push(("float3 constructor requires 1 or 3 arguments", unsafe(addr(expr.at))))
                return
            }
            emit_vector_ctor(expr, 3, CombineFloat3)
            return
        }

        if (expr._type.baseType == Type.tFloat4 && expr.func.name.string().find("float4") >= 0) {
            let argc = expr.arguments.length()
            if (argc == 1 && expr.arguments[0]._type.baseType == Type.tFloat) {
                emit_noop(expr.arguments[0], expr)
                return
            }
            if (argc == 4 && expr.arguments[0]._type.baseType == Type.tFloat
                          && expr.arguments[1]._type.baseType == Type.tFloat
                          && expr.arguments[2]._type.baseType == Type.tFloat
                          && expr.arguments[3]._type.baseType == Type.tFloat) {
                var irNode = begin_op(expr, CombineFloat4, kind)
                for (arg in expr.arguments) { add_arg(irNode, arg); }
                irNode.outputs.push(add_pin(expr))
                register_node(expr, irNode)
                return
            }
            if (argc == 2 && expr.arguments[0]._type.baseType == Type.tFloat2
                          && expr.arguments[1]._type.baseType == Type.tFloat2) {
                var irNode = begin_op(expr, CombineFloat4F2F2, kind)
                for (arg in expr.arguments) { add_arg(irNode, arg); }
                irNode.outputs.push(add_pin(expr))
                register_node(expr, irNode)
                return
            }
            if (argc == 2 && expr.arguments[0]._type.baseType == Type.tFloat3
                          && expr.arguments[1]._type.baseType == Type.tFloat) {
                var irNode = begin_op(expr, CombineFloat4F3F1, kind)
                for (arg in expr.arguments) { add_arg(irNode, arg); }
                irNode.outputs.push(add_pin(expr))
                register_node(expr, irNode)
                return
            }
            errors.push(("float4 constructor in shader code supports: float4(f), float4(f,f,f,f), float4(f2,f2), float4(f3,f)", unsafe(addr(expr.at))))
            return
        }

        if (is_in_completion()) {
            let name = expr.name.string()
            if (name.starts_with("__::shader_dsl_primitives")) {
                return
            }
        }
        errors.push(("unsupported function call in shader code: {expr.name}", unsafe(addr(expr.at))))
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        let id : Variable const? = expr.variable

        if (accumVars.key_exists(id)) {
            return
        }

        var kind = ShaderNodeKind.Input
        for (ann in expr.variable.annotation) {
            if (ann.name == "dir") {
                let kindStr = ann.sValue.string()
                if (kindStr == "input") {
                    kind = ShaderNodeKind.Input
                } elif (kindStr == "output") {
                    kind = ShaderNodeKind.Output
                } else {
                    errors.push(("unsupported variable annotation dir={kindStr}; expected input or output", unsafe(addr(expr.at))))
                    return
                }
            }
        }
        for (ann in expr.variable.annotation) {
            if (ann.name == "alias") {
                if (kind == ShaderNodeKind.Input) {
                    if (expr.varFlags.write) {
                        errors.push(("shader input variable {expr.variable.name} must be read-only", unsafe(addr(expr.at))))
                    }
                } else {
                    if (!expr.varFlags.write) {
                        errors.push(("shader output variable {expr.variable.name} must be written to in the shader code and cannot be read", unsafe(addr(expr.at))))
                    }
                }
                break
            }
        }

        if (variablePins.key_exists(id) || variables.key_exists(id) || outPins.key_exists(id)) {
            return
        }

        for (ann in expr.variable.annotation) {
            if (ann.name == "alias") {
                let typeId = ann.sValue.string()
                var irNode = begin_op(expr, typeId, kind)
                if (kind == ShaderNodeKind.Input) {
                    var pin = add_pin(expr)
                    irNode.outputs.push(pin)
                    variablePins[id] = pin.id
                } else {
                    var pin = add_pin(expr)
                    irNode.inputs.push(pin)
                    outPins[id] = pin.id
                }
                register_node(expr, irNode)
                return
            }
        }

        if (expr.varFlags.argument) {
            if (expr.variable._type.baseType == Type.tStructure) {
                var st = expr.variable._type.structType
                if (st != null && st._module != null && st._module.name == PRIMITIVES_MODULE_NAME && st.name != "Sampler2D") {
                    return
                }
            }
            errors.push(("shader function argument {expr.variable.name} is not supported in this context", unsafe(addr(expr.at))))
            return
        }

        if (!expr.varFlags.local) {
            assume arg = expr.variable
            let varModName = expr.variable._module != null ? expr.variable._module.name.string() : ""
            if (varModName != moduleName) {
                errors.push(("variable {arg.name} is defined in module {varModName}, but only variables defined in the same module as the shader function ({moduleName}) can be used as shader properties", unsafe(addr(arg.at))))
                return
            }
            if (arg._type.isPointer) {
                errors.push(("shader properties can't be pointers", unsafe(addr(arg.at))))
            }
            if (!empty(arg._type.dim)) {
                errors.push(("shader properties can't be vectors; use float2/3/4 types instead", unsafe(addr(arg.at))))
            }
            var prop : ShaderGraphProperty
            prop.name = arg.name.string()
            var valid = false
            if (arg._type.baseType == Type.tFloat) {
                prop._type = ShaderPropertyType.Float
                valid = true
                if (arg.init != null) {
                    if (!extract_float(arg.init, prop.defaultValue.x, null)) {
                        errors.push(("{arg.init.describe()} unsupported default value expression for float property. Only constant values are supported.", unsafe(addr(arg.at))))
                    }
                }
            } elif (arg._type.baseType == Type.tFloat2) {
                prop._type = ShaderPropertyType.Float2
                valid = true
                if (arg.init != null) {
                    if (!extract_float2(arg.init, prop.defaultValue.xy, null)) {
                        errors.push(("{arg.init.describe()} unsupported default value expression for float2 property. Only constant literals are supported, e.g. `a : float2 = float2(1.0, 2.0)`", unsafe(addr(arg.at))))
                    }
                }
            } elif (arg._type.baseType == Type.tFloat3) {
                prop._type = arg.annotation.find_arg("color") is nothing ? ShaderPropertyType.Float3 : ShaderPropertyType.Color
                valid = true
                if (arg.init != null) {
                    if (!extract_float3(arg.init, prop.defaultValue.xyz, null)) {
                        errors.push(("unsupported default value expression for float3 property. Only constant literals are supported, e.g. `a : float3 = float3(1.0, 2.0, 3.0)`", unsafe(addr(arg.at))))
                    }
                }
            } elif (arg._type.baseType == Type.tFloat4) {
                prop._type = ShaderPropertyType.Float4
                valid = true
                if (arg.init != null) {
                    if (!extract_float4(arg.init, prop.defaultValue, null)) {
                        errors.push(("{arg.init.describe()} unsupported default value expression for float4 property. Only constant literals are supported, e.g. `a : float4 = float4(1.0, 2.0, 3.0, 4.0)`", unsafe(addr(arg.at))))
                    }
                }
            } elif (arg._type.baseType == Type.tStructure) {
                var st = arg._type.structType
                if (st.name == "Sampler2D" && st._module != null && st._module.name == PRIMITIVES_MODULE_NAME) {
                    prop._type = ShaderPropertyType.Texture2D
                    valid = true
                    if (arg.init != null) {
                        if (!extract_sampler2d(arg.init, prop.defaultTexture.name, null)) {
                            errors.push(("{arg.init.describe()} unsupported default value expression for Texture2D property. Only constant literals are supported, e.g. `a : Sampler2D = Sampler2D(\"foo.png\")`", unsafe(addr(arg.at))))
                        }
                    }
                } else {
                    errors.push(("unsupported shader property type {describe(arg._type)}", unsafe(addr(arg.at))))
                }
            } else {
                errors.push(("unsupported shader property type {describe(arg._type)}", unsafe(addr(arg.at))))
            }

            if (!valid) {
                errors.push(("unsupported shader property type {describe(arg._type)}", unsafe(addr(arg.at))))
                return
            }

            let irNodeTypeId = (prop._type == ShaderPropertyType.Color ? ColorVar
                              : prop._type == ShaderPropertyType.Float ? FloatVar
                              : prop._type == ShaderPropertyType.Float2 ? Float2Var
                              : prop._type == ShaderPropertyType.Float3 ? Float3Var
                              : prop._type == ShaderPropertyType.Float4 ? Float4Var
                              : TextureVar)
            var irNode = IRNode(id = irNodeId++, typeId = irNodeTypeId, kind = ShaderNodeKind.Var, propertyIndex = propsIdx++)
            if (DEBUG_AT) {
                irNode.at = describe(expr.variable)
            }
            let pinType = (prop._type == ShaderPropertyType.Color ? ShaderPinType.Float3
                        : prop._type == ShaderPropertyType.Float ? ShaderPinType.Float
                        : prop._type == ShaderPropertyType.Float2 ? ShaderPinType.Float2
                        : prop._type == ShaderPropertyType.Float3 ? ShaderPinType.Float3
                        : prop._type == ShaderPropertyType.Float4 ? ShaderPinType.Float4
                        : ShaderPinType.Texture2D)
            irNode.outputs.push(new IRPin(id = irPinId++, _type = pinType))
            pins[irNode.outputs[0].id] = irNode.outputs[0]

            variablePins[id] = irNode.outputs[0].id
            register_node(expr, irNode)
            props.emplace(prop)
            return
        }

        errors.push(("undefined variable {expr.variable.name} {expr.varFlags} referenced in shader code", unsafe(addr(expr.at))))
    }

    def override preVisitExprOp1(expr : ExprOp1?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        if (expr.op == "+") {
            emit_noop(expr.subexpr, expr)
            return
        } elif (expr.op == "-") {
            var irNode = begin_op(expr, Neg)
            add_arg(irNode, expr.subexpr)
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        } elif (expr.op == "!") {
            // Phase B: logical NOT of a mask (from flatten's `!cond`).
            var irNode = begin_op(expr, LogicNot)
            add_arg(irNode, expr.subexpr)
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        }
        errors.push(("unsupported unary operator '{expr.op}' in shader code", unsafe(addr(expr.at))))
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        if (expr.op == "+" || expr.op == "-" || expr.op == "*" || expr.op == "/") {
            let typeId = expr.op == "+" ? Add : expr.op == "-" ? Sub : expr.op == "*" ? Mul : Div
            var irNode = begin_op(expr, typeId)
            add_arg(irNode, expr.left)
            add_arg(irNode, expr.right)
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        }
        // Phase B: comparison / logical ops produce the bool mask feeding a select.
        if (expr.op == "<" || expr.op == ">" || expr.op == "<=" || expr.op == ">=" || expr.op == "==" || expr.op == "!="
            || expr.op == "&&" || expr.op == "||") {
            let typeId = (expr.op == "<" ? CmpLt : expr.op == ">" ? CmpGt
                       : expr.op == "<=" ? CmpLe : expr.op == ">=" ? CmpGe
                       : expr.op == "==" ? CmpEq : expr.op == "!=" ? CmpNe
                       : expr.op == "&&" ? LogicAnd : LogicOr)
            var irNode = begin_op(expr, typeId)
            add_arg(irNode, expr.left)
            add_arg(irNode, expr.right)
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        }
        if (expr.op == "+=" || expr.op == "-=" || expr.op == "*=" || expr.op == "/=") {
            if (!(expr.left is ExprVar) || !accumVars.key_exists((expr.left as ExprVar).variable)) {
                errors.push(("compound assignment '{expr.op}' is only supported on a mutable accumulator (`var acc = ...`)", unsafe(addr(expr.at))))
                return
            }
            let typeId = expr.op == "+=" ? Add : expr.op == "-=" ? Sub : expr.op == "*=" ? Mul : Div
            var irNode = begin_op(expr, typeId)
            add_arg(irNode, expr.left)
            add_arg(irNode, expr.right)
            var outPin = new IRPin(id = irPinId, _type = pin_type_of(expr.left._type))
            pins[irPinId] = outPin
            irPinId++
            irNode.outputs.push(outPin)
            register_node(expr, irNode)
            return
        }
        errors.push(("unsupported binary operator {expr.op} in shader code", unsafe(addr(expr.at))))
    }

    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (empty(ignoreNext)
            && (expr.op == "+=" || expr.op == "-=" || expr.op == "*=" || expr.op == "/=")
            && (expr.left is ExprVar)) {
            let v : Variable const? = (expr.left as ExprVar).variable
            if (accumVars.key_exists(v)) {
                let opId : Expression const? = expr
                let pin = nodeExitPins?[opId] ?? -1
                if (pin != -1) {
                    accumCurPin[v] = pin
                }
            }
        }
        return <- expr
    }

    // Phase B: `cond ? a : b` -> a Select node (consumes flatten's `?:`).
    def override preVisitExprOp3(expr : ExprOp3?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        var irNode = begin_op(expr, Select)
        add_arg(irNode, expr.subexpr)   // condition (mask)
        add_arg(irNode, expr.left)      // if-true
        add_arg(irNode, expr.right)     // if-false
        irNode.outputs.push(add_pin(expr))
        register_node(expr, irNode)
    }

    def swizzle_component_typeId(ch : string) : string {
        if (ch == "x" || ch == "r") { return SplatX; }
        if (ch == "y" || ch == "g") { return SplatY; }
        if (ch == "z" || ch == "b") { return SplatZ; }
        return SplatW
    }

    def swizzle_pack_mask(mask : string) : int {
        var packed = 0
        for (i in range(length(mask))) {
            let ch = slice(mask, i, i + 1)
            let idx = (ch == "y" || ch == "g") ? 1 : ((ch == "z" || ch == "b") ? 2 : ((ch == "w" || ch == "a") ? 3 : 0))
            packed = packed | (idx << (i * 2))
        }
        return packed
    }

    def override preVisitExprSwizzle(expr : ExprSwizzle?) : void {
        if (!empty(ignoreNext)) {
            return
        }
        let mask = "{expr.mask}"

        if (mask == "xyzw" || mask == "xyz" || mask == "xy" || mask == "rgba" || mask == "rgb" || mask == "rg") {
            emit_noop(expr.value, expr)
            return
        }

        if (length(mask) == 1) {
            var irNode = begin_op(expr, swizzle_component_typeId(mask))
            add_arg(irNode, expr.value)
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        }

        if ((mask == "xyxy" || mask == "rgrg") && expr.value._type.baseType == Type.tFloat2) {
            var irNode = begin_op(expr, CombineFloat4F2F2)
            for (_ in range(2)) { add_arg(irNode, expr.value); }
            irNode.outputs.push(add_pin(expr))
            register_node(expr, irNode)
            return
        }

        let maskLen = length(mask)
        if (maskLen == 2 || maskLen == 3 || maskLen == 4) {
            let typeId = maskLen == 2 ? SwizzleF2 : (maskLen == 3 ? SwizzleF3 : SwizzleF4)
            var swizzleNode = begin_op(expr, typeId)
            add_arg(swizzleNode, expr.value)
            swizzleNode.constValue.x = float(swizzle_pack_mask(mask))
            swizzleNode.outputs.push(add_pin(expr))
            register_node(expr, swizzleNode)
            return
        }

        errors.push(("unsupported swizzle mask {expr.mask} in shader code", unsafe(addr(expr.at))))
    }

    def override preVisitExprConstInt(expr : ExprConstInt?) : void {
        if (!empty(ignoreNext)) { return ; }
        emit_const_node(expr, FloatConst, float4(float(expr.value), 0., 0., 0.))
    }

    def override preVisitExprConstFloat(expr : ExprConstFloat?) : void {
        if (!empty(ignoreNext)) { return ; }
        emit_const_node(expr, FloatConst, float4(expr.value, 0., 0., 0.))
    }

    def override preVisitExprConstFloat2(expr : ExprConstFloat2?) : void {
        if (!empty(ignoreNext)) { return ; }
        emit_const_node(expr, Float2Const, float4(expr.value, 0., 0.))
    }

    def override preVisitExprConstFloat3(expr : ExprConstFloat3?) : void {
        if (!empty(ignoreNext)) { return ; }
        emit_const_node(expr, Float3Const, float4(expr.value, 0.))
    }

    def override preVisitExprConstFloat4(expr : ExprConstFloat4?) : void {
        if (!empty(ignoreNext)) { return ; }
        emit_const_node(expr, Float4Const, expr.value)
    }
}


[macro_function]
def resolve_contract(struct_name : das_string, map : table<string, string>) : string {
    for (contractName, structName in keys(map), values(map)) {
        if (structName == struct_name) {
            return contractName
        }
    }
    return ""
}

[macro_function, unused_argument(group)]
def shader_build_ir(var func : FunctionPtr; var group : ModuleGroup; var errors : das_string; shaderStage : string) : bool {

    if (func._module != null) {
        var secondFn : string
        func._module.for_each_function("") $(fn) {
            if (secondFn == "" && fn != func) {
                for (it in fn.annotations) {
                    if (it.annotation.name == "shader" || it.annotation.name == "pixel_shader") {
                        secondFn = "{fn.name}"
                        break
                    }
                }
            }
        }
        if (secondFn != "") {
            errors := "module {func._module.name} contains multiple functions ({func.name} and {secondFn}); shader code must be defined in a module with exactly one function"
            return false
        }
    }

    let shaderContract = func.result.baseType == Type.tStructure && func.result.structType != null ? resolve_contract(func.result.structType.name, CONTRACT_OUTPUT_STRUCTS) : ""
    if (shaderContract == "") {
        errors := "shader function must return a struct with an output, e.g. `PbrOutput` or `UnlitOutput`"
        return false
    }

    var astVisitor = new ValidateShaderVisitor(
        moduleName = func._module != null ? func._module.name.string() : "",
        contract = shaderContract)

    for (arg in func.arguments) {
        if (arg._type.baseType == Type.tStructure && arg._type.structType != null && astVisitor.is_input_contract_struct(arg._type.structType)) {
            // ok, input contract struct argument
        } else {
            let inputType = CONTRACT_INPUT_STRUCTS?[shaderContract] ?? ""
            errors := "shader function arguments must be `{inputType}`"
            return false
        }
    }

    if (func.arguments.length() != 1) {
        errors := "shader function must have exactly one argument (the input struct)"
        return false
    }

    var res = true

    make_visitor(*astVisitor) $(adapter) {
        visit(func, adapter)

        if (!empty(astVisitor.errors)) {
            for (it in astVisitor.errors) {
                macro_error(compiling_program(), it._1 != null ? *it._1 : func.at, it._0)
            }
            res = false
            return
        }

        if (!is_in_completion()) {

            for (link in astVisitor.exprLinks) {
                let fromPin = astVisitor.pins[link._0].id
                let toPin = astVisitor.nodeExitPins?[link._1] ?? -1
                if (toPin == -1) {
                    macro_error(compiling_program(), func.at, "internal error: failed to resolve expression link in shader code. This is a bug in the shader compiler, please report it.")
                    continue
                }
                astVisitor.links.emplace(IRLink(fromPin = toPin, toPin = fromPin))
            }

            for (link in astVisitor.variableLinks) {
                let fromPin = astVisitor.pins[link._0].id
                var targetVar = link._1
                if (astVisitor.aliasTargets.key_exists(targetVar)) {
                    targetVar = astVisitor.aliasTargets[targetVar]
                }
                var toPin = astVisitor.variablePins?[targetVar] ?? -1
                if (toPin == -1) {
                    for (it, initExpr in keys(astVisitor.variables), values(astVisitor.variables)) {
                        if (initExpr == null) {
                            continue
                        }
                        let x : void? = targetVar
                        var y : void? = it
                        if (x == y) {
                            for (it2, outPin in keys(astVisitor.nodeExitPins), values(astVisitor.nodeExitPins)) {
                                let x2 : void? = initExpr
                                var y2 : void? = it2
                                if (x2 == y2) {
                                    toPin = outPin
                                    break
                                }
                            }
                            break
                        }
                    }
                }
                if (toPin == -1) {
                    macro_error(compiling_program(), func.at, "internal error: failed to resolve variable {unsafe(reinterpret<VariablePtr>(link._1)).describe()} link in shader code. This is a bug in the shader compiler, please report it.")
                    continue
                }
                astVisitor.links.emplace(IRLink(fromPin = toPin, toPin = fromPin))
            }
        }

        // remove <noop> nodes, rewire links
        var idx = astVisitor.nodes.length() - 1
        while (idx >= 0) {
            assume node = astVisitor.nodes[idx]
            if (node.typeId == "<noop>") {
                verify(node.inputs.length() == 1 && node.outputs.length() == 1)
                let inPin = node.inputs[0]
                let outPin = node.outputs[0]
                var enters : array<IRPin?>
                var exits : array<IRPin?>
                for (link in astVisitor.links) {
                    if (link.toPin == inPin.id) {
                        enters.push(astVisitor.pins[link.fromPin])
                    }
                    if (link.fromPin == outPin.id) {
                        exits.push(astVisitor.pins[link.toPin])
                    }
                }
                for (enterPin in enters) {
                    for (exitPin in exits) {
                        astVisitor.links.emplace(IRLink(fromPin = enterPin.id, toPin = exitPin.id))
                    }
                }
                var linkIdx = astVisitor.links.length() - 1
                while (linkIdx >= 0) {
                    let l = astVisitor.links[linkIdx]
                    if (l.fromPin == outPin.id || l.toPin == inPin.id) {
                        astVisitor.links.erase(linkIdx)
                    }
                    linkIdx--
                }
                astVisitor.pins.erase(inPin.id)
                astVisitor.pins.erase(outPin.id)
                astVisitor.nodes.erase(idx)
            }
            idx--
        }

        if (!is_in_completion()) {
            sg_ir_reset()
            sg_ir_set_contract(shaderStage, shaderContract)
            for (prop in astVisitor.props) {
                sg_ir_add_property(prop.name, int(prop._type), prop.defaultValue, prop.defaultTexture.name)
            }
            for (node in astVisitor.nodes) {
                sg_ir_begin_node(node.id, node.typeId, int(node.kind), node.constValue, node.propertyIndex)
                for (pin in node.inputs) {
                    if (pin != null) {
                        sg_ir_add_input_pin(pin.id, int(pin._type))
                    }
                }
                for (pin in node.outputs) {
                    if (pin != null) {
                        sg_ir_add_output_pin(pin.id, int(pin._type))
                    }
                }
            }
            for (link in astVisitor.links) {
                sg_ir_add_link(link.fromPin, link.toPin)
            }
        }
    }
    unsafe {
        delete astVisitor
    }
    return res
}


// ---- Shader annotation (flatten-integrated) --------------------------------
//
// [shader(stage="pixel", type="pbr")] is the surface the real EdenSpark shaders
// use. patch() flattens the shader (inlining + if-conversion + loop unroll) via
// daslib/flatten, then re-infers; finish() builds the shader graph from the now
// branchless body. A "flattened" annotation arg guards against re-flattening on
// the post-astChanged re-infer cycle.

def private shader_annotation_arg(func : FunctionPtr; argName : string) : bool {
    for (ann in func.annotations) {
        if (ann.annotation.name == "shader") {
            for (a in ann.arguments) {
                if ("{a.name}" == argName && a.basicType == Type.tBool) {
                    return a.bValue
                }
            }
        }
    }
    return false
}

def private set_shader_annotation_flag(var func : FunctionPtr; argName : string) {
    for (ann in func.annotations) {
        if (ann.annotation.name != "shader") {
            continue
        }
        var found = false
        for (a in ann.arguments) {
            if ("{a.name}" == argName) {
                a.bValue = true
                a.basicType = Type.tBool
                found = true
            }
        }
        if (!found) {
            ann.arguments |> add_annotation_argument(argName, true)
        }
    }
}

def private shader_stage_arg(args : AnnotationArgumentList) : string {
    let sv = find_arg(args, "stage")
    if (sv is tString) {
        return sv as tString
    }
    return "pixel"
}

[function_macro(name="shader")]
class ShaderMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        func.flags.exports = true
        return true
    }
    def override patch(var func : FunctionPtr; var group : ModuleGroup;
                       args, progArgs : AnnotationArgumentList; var errors : das_string;
                       var astChanged : bool&) : bool {
        // Skip during lint (the twin is irrelevant to linting source).
        if (is_in_lint_check() || shader_annotation_arg(func, "folded")) {
            return true
        }
        if (shader_annotation_arg(func, "flattened")) {
            // Cycle 2 (re-inferred after flattening): collapse const-condition selects
            // and drop the resulting pure self-assigns (`acc = acc`) — without this a
            // loop-var-gated accumulate leaves a self-referential accumulator the IR
            // builder cannot resolve. Re-infer once more if anything changed.
            let folded = flatten_fold(func)
            set_shader_annotation_flag(func, "folded")
            if (folded) {
                astChanged = true
            }
            return true
        }
        // Cycle 1: flatten does ALL lowering — inline helper calls, if/else -> ?:,
        // unroll fixed loops. The [hint] primitives are kept (whitelist precedence).
        var ctx = flatten_function(func, HINT_WHITELIST)
        if (!ctx.ok) {
            errors := ctx.err
            return false
        }
        set_shader_annotation_flag(func, "flattened")
        astChanged = true
        return true
    }
    def override finish(var func : FunctionPtr; var group : ModuleGroup;
                        args, progArgs : AnnotationArgumentList; var errors : das_string) : bool {
        // Optional: dump the flattened daslang source alongside the opcodes, so the
        // branchless rewrite (inlined calls, if/else -> ?:, unrolled loops) is legible.
        if (!is_in_completion() && !empty(get_env_variable("FLATTEN_DUMP_DAS"))) {
            print(";; ---- flattened daslang ----\n{describe(func)}\n")
        }
        return shader_build_ir(func, group, errors, shader_stage_arg(args))
    }
}


[macro_function]
def extract_float(expr : Expression?&, var res : float&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a float literal, got nothing"
        }
        return false
    }
    if (expr is ExprConstFloat) {
        res = (expr as ExprConstFloat).value
        return true
    }
    if (expr is ExprConstInt) {
        res = float((expr as ExprConstInt).value)
        return true
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_float(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a float literal"
    }
    return false
}


[macro_function]
def extract_float2(expr : Expression?&, var res : float2&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a float2 literal, got nothing"
        }
        return false
    }
    if (expr is ExprConstFloat2) {
        res = (expr as ExprConstFloat2).value
        return true
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_float2(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a float2 literal"
    }
    return false
}


[macro_function]
def extract_float3(expr : Expression?&, var res : float3&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a float3 literal, got nothing"
        }
        return false
    }
    if (expr is ExprConstFloat3) {
        res = (expr as ExprConstFloat3).value
        return true
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_float3(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a float3 literal"
    }
    return false
}


[macro_function]
def extract_float4(expr : Expression?&, var res : float4&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a float4 literal, got nothing"
        }
        return false
    }
    if (expr is ExprConstFloat4) {
        res = (expr as ExprConstFloat4).value
        return true
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_float4(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a float4 literal"
    }
    return false
}


[macro_function]
def extract_string(expr : Expression?&, var res : string&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a string literal, got nothing"
        }
        return false
    }
    if (expr is ExprConstString) {
        res = (expr as ExprConstString).value.string()
        return true
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_string(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a string literal"
    }
    return false
}


[macro_function]
def extract_sampler2d(expr : Expression?&, var res : string&, var errors : string?) : bool {
    if (expr == null) {
        if (errors != null) {
            *errors = "expected a texture path literal, got nothing"
        }
        return false
    }
    if (expr is ExprCall) {
        let e = expr as ExprCall
        if (e.name == "Sampler2D" && e.arguments.length() == 1 && e.func._module != null && e.func._module.name == PRIMITIVES_MODULE_NAME) {
            return extract_string(e.arguments[0], res, errors)
        }
    }
    if (expr is ExprVar) {
        let v = expr as ExprVar
        if (v.variable != null) {
            return extract_sampler2d(v.variable.init, res, errors)
        }
    }
    if (errors != null) {
        *errors = "expected a texture path literal, e.g. `Sampler2D(\"foo.png\")`"
    }
    return false
}
