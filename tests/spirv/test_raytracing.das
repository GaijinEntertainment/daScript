options gen2
options indenting = 4

// Ray-tracing stage scaffolding (VK_KHR_ray_tracing_pipeline). Each fixture asserts:
//   * the right execution model (RayGenerationKHR 5313 / MissKHR 5317 / ClosestHitKHR 5316)
//   * OpCapability RayTracingKHR (4479) + OpExtension "SPV_KHR_ray_tracing", each exactly once
//   * the SPIR-V 1.4 header (SPV_KHR_ray_tracing requires >= 1.4) and NO execution modes (none of
//     the three stages takes one)
//   * the RT storage classes on the payload / hit-attribute OpVariables + payload Location decorations
//   * OpTypeAccelerationStructureKHR + DescriptorSet/Binding on the TLAS descriptor
//   * OpTraceRayKHR's payload operand == the matching @ray_payload variable id (raygen payload at
//     Location 0; the closest-hit shadow payload at Location 1 -- the multi-payload case)
//   * the reflected acceleration_structure binding + raygen stage flag
//   * spirv-val against vulkan1.2 (the first env with SPV_KHR_ray_tracing promoted extensions).
// AnyHit / Intersection / Callable stages land with their first consumer.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar
require spirv/spirv_reflect
require daslib/strings_boost

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words, "vulkan1.2")
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

// OpExtension's operand words encode the extension-name string (LSB-first bytes, NUL-terminated,
// zero-padded). Reassemble bytes across each OpExtension and substring-search the requested name.
def private has_extension(words : array<uint>; want : string) : bool {
    var found = false
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (found || opcode != uint(SpvOp.Extension)) return
        let name = build_string() $(var wr) {
            for (k in range(n)) {
                let wd = w[s + k]
                for (i in range(4)) {
                    let b = uint8((wd >> uint(i * 8)) & 0xFFu)
                    if (b == 0u8) return
                    wr |> write_char(int(b))
                }
            }
        }
        if (find(name, want) >= 0) {
            found = true
        }
    }
    return found
}

// Result id (operand 1) of the FIRST OpVariable whose storage class (operand 2) is `sc`.
def private variable_id_with_storage(words : array<uint>; sc : SpvStorageClass) : tuple<ok : bool; value : uint> {
    var res = (ok = false, value = 0u)
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (res.ok || opcode != uint(SpvOp.Variable)) return
        if (n > 2 && w[s + 2] == uint(sc)) {
            res = (ok = true, value = w[s + 1])
        }
    }
    return res
}

// The payload operand (the 11th operand, index 10) of the FIRST OpTraceRayKHR.
def private trace_ray_payload_id(words : array<uint>) : tuple<ok : bool; value : uint> {
    var res = (ok = false, value = 0u)
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (res.ok || opcode != uint(SpvOp.TraceRayKHR)) return
        if (n > 10) {
            res = (ok = true, value = w[s + 10])
        }
    }
    return res
}

// The id decorated `Location = loc` (Decorate <id> Location loc), or ok=false.
def private id_with_location(words : array<uint>; loc : uint) : tuple<ok : bool; value : uint> {
    var res = (ok = false, value = 0u)
    foreach_inst(words) $(opcode : uint; w : array<uint>; s, n : int) {
        if (res.ok || opcode != uint(SpvOp.Decorate)) return
        if (n > 2 && w[s + 1] == uint(SpvDecoration.Location) && w[s + 2] == loc) {
            res = (ok = true, value = w[s + 0])
        }
    }
    return res
}

def private assert_rt_module_shape(t : T?; words : array<uint>; lbl : string; model : SpvExecutionModel) {
    t |> success(words[1] == SPV_VERSION_1_4, "{lbl}: SPIR-V header version is 1.4")
    t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.RayTracingKHR)),
        "{lbl}: OpCapability RayTracingKHR")
    let ncap = count_op_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.RayTracingKHR))
    t |> success(ncap == 1, "{lbl}: exactly one RayTracingKHR cap (got {ncap})")
    t |> success(has_extension(words, "SPV_KHR_ray_tracing"), "{lbl}: OpExtension SPV_KHR_ray_tracing")
    t |> success(count_op(words, SpvOp.Extension) == 1,
        "{lbl}: exactly one OpExtension (got {count_op(words, SpvOp.Extension)})")
    t |> success(op_has_operand_at(words, SpvOp.EntryPoint, 0, uint(model)),
        "{lbl}: OpEntryPoint execution model")
    // none of the three RT stages takes an execution mode (no LocalSize / OriginUpperLeft / ...)
    t |> success(count_op(words, SpvOp.ExecutionMode) == 0,
        "{lbl}: no execution modes (got {count_op(words, SpvOp.ExecutionMode)})")
}

[test]
def test_raygen_stage(t : T?) {
    t |> run("[raygen_shader]: model + cap + ext + payload + TLAS + OpTraceRayKHR payload operand") <| @(t : T?) {
        var words <- rt_raygen_words()
        assert_rt_module_shape(t, words, "rt_raygen", SpvExecutionModel.RayGenerationKHR)
        // the launch-grid builtins
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.LaunchIdKHR)),
            "rt_raygen: gl_LaunchIDEXT -> BuiltIn LaunchIdKHR")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.LaunchSizeKHR)),
            "rt_raygen: gl_LaunchSizeEXT -> BuiltIn LaunchSizeKHR")
        // @ray_payload -> a RayPayloadKHR OpVariable, Location 0
        let pvar = variable_id_with_storage(words, SpvStorageClass.RayPayloadKHR)
        t |> success(pvar.ok, "rt_raygen: @ray_payload -> RayPayloadKHR OpVariable")
        let ploc = id_with_location(words, 0u)
        t |> success(ploc.ok && ploc.value == pvar.value,
            "rt_raygen: the payload variable carries Location 0 (loc-id={ploc.value} var={pvar.value})")
        // the TLAS descriptor: OpTypeAccelerationStructureKHR + set/binding decorations
        t |> success(has_op(words, SpvOp.TypeAccelerationStructureKHR),
            "rt_raygen: OpTypeAccelerationStructureKHR")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.DescriptorSet)),
            "rt_raygen: TLAS has a DescriptorSet decoration")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Binding)),
            "rt_raygen: TLAS has a Binding decoration")
        // OpTraceRayKHR: present once, its payload operand names the @ray_payload variable
        t |> success(count_op(words, SpvOp.TraceRayKHR) == 1,
            "rt_raygen: exactly one OpTraceRayKHR (got {count_op(words, SpvOp.TraceRayKHR)})")
        let tpay = trace_ray_payload_id(words)
        t |> success(tpay.ok && tpay.value == pvar.value,
            "rt_raygen: OpTraceRayKHR payload operand == the payload variable id (op={tpay.value} var={pvar.value})")
        validate(t, words, "rt_raygen")
        delete words
        // reflection: the TLAS binding surfaces as acceleration_structure with the raygen stage flag
        var rwords <- rt_raygen_reflect_words()
        var refl <- decode_reflection(rwords)
        t |> success(refl.stage.raygen, "rt_raygen: reflection stage flag is raygen")
        var accel_found = false
        for (b in refl.bindings) {
            if (b.kind == SpirvDescriptorKind.acceleration_structure) {
                accel_found = true
                t |> equal(b.set, 0)
                t |> equal(b.binding, 0)
                t |> success(b.stages.raygen, "rt_raygen: the TLAS binding carries the raygen stage bit")
            }
        }
        t |> success(accel_found, "rt_raygen: reflection carries an acceleration_structure binding")
        finalize(refl)
        delete rwords
    }
}

[test]
def test_miss_stage(t : T?) {
    t |> run("[miss_shader]: model + incoming payload + world-ray builtin") <| @(t : T?) {
        var words <- rt_miss_words()
        assert_rt_module_shape(t, words, "rt_miss", SpvExecutionModel.MissKHR)
        // @incoming_ray_payload -> an IncomingRayPayloadKHR OpVariable, Location 0
        let pvar = variable_id_with_storage(words, SpvStorageClass.IncomingRayPayloadKHR)
        t |> success(pvar.ok, "rt_miss: @incoming_ray_payload -> IncomingRayPayloadKHR OpVariable")
        let ploc = id_with_location(words, 0u)
        t |> success(ploc.ok && ploc.value == pvar.value,
            "rt_miss: the incoming payload carries Location 0")
        // the world-ray builtin read by the sky gradient
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.WorldRayDirectionKHR)),
            "rt_miss: gl_WorldRayDirectionEXT -> BuiltIn WorldRayDirectionKHR")
        // a miss shader that doesn't trace has no OpTraceRayKHR
        t |> success(count_op(words, SpvOp.TraceRayKHR) == 0, "rt_miss: no OpTraceRayKHR")
        validate(t, words, "rt_miss")
        delete words
    }
}

[test]
def test_closest_hit_stage(t : T?) {
    t |> run("[closest_hit_shader]: hit attribute + hit builtins + second payload + shadow trace") <| @(t : T?) {
        var words <- rt_chit_words()
        assert_rt_module_shape(t, words, "rt_chit", SpvExecutionModel.ClosestHitKHR)
        // @hit_attribute -> a HitAttributeKHR OpVariable (NO Location decoration on it)
        let hvar = variable_id_with_storage(words, SpvStorageClass.HitAttributeKHR)
        t |> success(hvar.ok, "rt_chit: @hit_attribute -> HitAttributeKHR OpVariable")
        // both payloads: the incoming one (written result) + the chit's OWN outgoing shadow payload
        let ivar = variable_id_with_storage(words, SpvStorageClass.IncomingRayPayloadKHR)
        t |> success(ivar.ok, "rt_chit: @incoming_ray_payload -> IncomingRayPayloadKHR OpVariable")
        let svar = variable_id_with_storage(words, SpvStorageClass.RayPayloadKHR)
        t |> success(svar.ok, "rt_chit: the shadow @ray_payload -> RayPayloadKHR OpVariable")
        let sloc = id_with_location(words, 1u)
        t |> success(sloc.ok && sloc.value == svar.value,
            "rt_chit: the shadow payload carries @location = 1")
        // the hit-state builtins
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.RayTmaxKHR)),
            "rt_chit: gl_HitTEXT -> BuiltIn RayTmaxKHR")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.WorldRayOriginKHR)),
            "rt_chit: gl_WorldRayOriginEXT -> BuiltIn WorldRayOriginKHR")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.InstanceCustomIndexKHR)),
            "rt_chit: gl_InstanceCustomIndexEXT -> BuiltIn InstanceCustomIndexKHR")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.PrimitiveId)),
            "rt_chit: gl_PrimitiveID -> BuiltIn PrimitiveId")
        // gl_PrimitiveID must NOT be Flat here (that's the fragment-interface rule, not closest-hit)
        t |> success(!op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Flat)),
            "rt_chit: no Flat decoration (fragment-only rule)")
        // ... and no MeshShadingEXT capability leaked in via the gl_PrimitiveID path
        t |> success(!op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.MeshShadingEXT)),
            "rt_chit: no MeshShadingEXT capability")
        // the shadow trace: OpTraceRayKHR whose payload operand is the SHADOW payload (Location 1)
        t |> success(count_op(words, SpvOp.TraceRayKHR) == 1,
            "rt_chit: exactly one OpTraceRayKHR (got {count_op(words, SpvOp.TraceRayKHR)})")
        let tpay = trace_ray_payload_id(words)
        t |> success(tpay.ok && tpay.value == svar.value,
            "rt_chit: OpTraceRayKHR payload operand == the shadow payload id (op={tpay.value} var={svar.value})")
        validate(t, words, "rt_chit")
        delete words
    }
}
