options gen2
require dastest/testing_boost public
require opengl/gl_binding_cache

[test]
def test_uniform_payload_and_epoch(t : T?){
    var entry = GlUniformBinding()
    var values = fixed_array(float4(1.0), float4(2.0))
    let data = unsafe(addr<void?>(values))
    let size = typeinfo sizeof(values)
    t |> success(gl_binding_value_changed(entry, 1ul, data, size), "first upload")
    t |> success(!gl_binding_value_changed(entry, 1ul, data, size), "identical array skipped")
    values[1].w = 3.0
    t |> success(gl_binding_value_changed(entry, 1ul, data, size), "last component compared")
    t |> success(gl_binding_value_changed(entry, 2ul, data, size), "new ownership interval uploads unchanged payload")
    t |> success(gl_binding_value_changed(entry, 2ul, data, 16), "size changes invalidate payload")
    t |> success(!gl_binding_value_changed(entry, 2ul, data, 16), "new size retained")
    delete entry
}
