options gen2
require daslib/templates
require dastest/testing_boost public

[template(a), sideeffects]
def boo(x : int; a : auto(TT)) => "{x}_{typeinfo typename(type<TT>)}"

[test(description = "a [template] argument binds a type or decltype so the function sees it as a const type name")]
def test_template(t : T?) {
    let a = boo(123, type<float>)
    t |> equal(a, "123_float const")
    let b = boo(456, decltype(2 + 2))
    t |> equal(b, "456_int const")
}

struct TemplateHolder {
    data : void?
}

def template_raw_data(h : auto(HH)) => h.data

[template(t), unused_argument(t)]
def template_find(h : TemplateHolder?; t : type<auto(TT)>) : bool {
    let p = unsafe(reinterpret<TT -const? -const>(template_raw_data(h)))
    return p != null
}

[test(description = "the body of a [template] function still names the alias of its erased type argument")]
def test_template_alias_in_body(t : T?) {
    var x = 42
    var h = TemplateHolder(data = unsafe(addr(x)))
    t |> equal(template_find(unsafe(addr(h)), type<int>), true)
    h.data = null
    t |> equal(template_find(unsafe(addr(h)), type<int>), false)
}

typedef TemplateClash = float

[template(t), unused_argument(t)]
def template_clash(t : auto(TemplateClash)) => 1

[test(description = "a [template] argument alias that shares a module type name still erases without a name clash")]
def test_template_alias_clashes_with_module_type(t : T?) {
    t |> equal(template_clash(type<int>), 1)
}

enum TemplateClashEnum {
    first
    second
}

[template(t), unused_argument(t)]
def template_clash_enum(t : auto(TemplateClashEnum)) => 1

[test(description = "a [template] argument alias that shares an enum name still erases without a name clash")]
def test_template_alias_clashes_with_enum(t : T?) {
    t |> equal(template_clash_enum(type<int>), 1)
}

[unused_argument(a)]
def template_plain_typename(h : TemplateHolder?; a : auto(TT)[]) : string {
    let p = unsafe(reinterpret<TT?>(template_raw_data(h)))
    return typeinfo typename(p)
}

[template(a), unused_argument(a)]
def template_erased_typename(h : TemplateHolder?; a : auto(TT)[]) : string {
    let p = unsafe(reinterpret<TT?>(template_raw_data(h)))
    return typeinfo typename(p)
}

[test(description = "an erased fixed-array [template] argument binds its alias const, as an ordinary generic does")]
def test_template_alias_const_under_dim(t : T?) {
    var x = 1
    var h = TemplateHolder(data = unsafe(addr(x)))
    let arr : int[4]
    let erased = template_erased_typename(unsafe(addr(h)), type<int[4]>)
    t |> equal(erased, template_plain_typename(unsafe(addr(h)), arr))
    t |> equal(erased, "int const? const")
}

[inline, template(t), unused_argument(t)]
def template_inlined(x : int; t : auto(TT)) => x + 1

[template(a, b)]
def template_twice(a : auto(TT); b : auto(TT)) => 1

[test(description = "an erased alias does not block inlining and may repeat across erased arguments")]
def test_template_alias_inline_and_repeat(t : T?) {
    t |> equal(template_inlined(1, type<int>), 2)
    t |> equal(template_twice(type<int>, type<float>), 1)
}

[template(t), unused_argument(t)]
def template_repeat_erased_last(h : TemplateHolder?; x : auto(TT); t : auto(TT)) : string {
    let p = unsafe(reinterpret<TT?>(template_raw_data(h)))
    return typeinfo typename(p)
}

[template(t), unused_argument(t)]
def template_repeat_erased_first(h : TemplateHolder?; t : auto(TT); x : auto(TT)) : string {
    let p = unsafe(reinterpret<TT?>(template_raw_data(h)))
    return typeinfo typename(p)
}

[test(description = "a repeated alias keeps the binding of the first argument, erased or retained")]
def test_template_repeated_alias_first_wins(t : T?) {
    var x = 1
    var h = TemplateHolder(data = unsafe(addr(x)))
    t |> equal(template_repeat_erased_last(unsafe(addr(h)), 1, type<float>), "int const? const")
    t |> equal(template_repeat_erased_first(unsafe(addr(h)), type<float>, 1), "float const? const")
}
