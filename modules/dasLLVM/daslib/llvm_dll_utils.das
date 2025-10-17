options gen2
module llvm_dll_utils shared private

require jit

require rtti
require daslib/strings_boost
require ast

// Wrapped name for dll.
struct public DllName {
    name : string

    def DllName(s : string) {
        name = s |> replace_multiple([("@", "`At`")])
    }

    def const impl() {
        return "{name} implementation"
    }

    def const publ() {
        return "{name}"
    }

    def const id() {
        return "{name} hash"
    }
}


// Sometimes we need unique id for some nodes. This visitor generates such ids.
class public UidGen : AstVisitor {
    uid : uint64
    id_map : table<void?, uint64>
    namesp : string
    @do_not_delete thisFunc : Function?

    def UidGen(prog : Program?) {
        namesp = prog.thisNamespace |> string()
    }

    def const get_namesp_prefix() {
        return namesp
    }

    def private get_base(ptr : void?; hint : string) {
        if (id_map |> key_exists(ptr)) {
            let id = unsafe(id_map[ptr])
            return DllName("{get_namesp_prefix()}`{get_mangled_name(thisFunc)}_{id}_{hint}")
        } else {
            panic("Key not found for {hint}")
            return DllName()
        }
    }

    def private add(ptr : void?) {
        id_map |> insert(ptr, uid)
        uid++
    }

    def get_addr(expr : smart_ptr<ExprAddr>) : DllName {
        return get_base(get_ptr(expr), "addr_{get_mangled_name(expr.func)}")
    }

    def override preVisitFunction(expr : FunctionPtr) {
        uid = 0 |> uint64()
    }

    def override preVisitExprAddr(expr : smart_ptr<ExprAddr>) {
        add(get_ptr(expr))
    }

    def get_new(expr : TypeDeclPtr) : DllName {
        return get_base(get_ptr(expr), "new`handle`{get_mangled_name(expr)}")
    }

    def override preVisitExprNew(expr : smart_ptr<ExprNew>) {
        add(get_ptr(expr))
        add(get_ptr(expr.typeexpr))
    }

    def get_ascend_new(expr : smart_ptr<ExprAscend>) {
        return get_base(get_ptr(expr), "ascend_new`handle`{get_mangled_name(expr.subexpr._type)}")
    }

    def override preVisitExprAscend(expr : smart_ptr<ExprAscend>) {
        add(get_ptr(expr))
        add(get_ptr(expr.subexpr._type))
    }

    def get_delete_type(t : TypeDeclPtr) : DllName {
        return get_base(get_ptr(t), "delete`handle`{get_mangled_name(t)}")
    }

    def override preVisitExprDelete(expr : smart_ptr<ExprDelete>) {
        add(get_ptr(expr))
        add(get_ptr(expr.subexpr._type.firstType))
    }

    def get_tinfo(expr : smart_ptr<ExprTypeInfo>) {
        return get_base(get_ptr(expr), "tinfo_{expr.typeexpr.get_mnh}")
    }

    def override preVisitExprTypeInfo(expr : smart_ptr<ExprTypeInfo>) {
        add(get_ptr(expr))
    }

    def get_ascend_tinfo(expr : smart_ptr<ExprAscend>) {
        assert(expr.ascendFlags.needTypeInfo)
        return get_base(get_ptr(expr), "tinfo_{expr.subexpr._type.get_mnh}")
    }

    def get_ctor(expr : smart_ptr<ExprMakeStruct>) {
        return get_base(get_ptr(expr), "ctor_{expr.constructor}")
    }

    def override preVisitExprMakeStruct(expr : smart_ptr<ExprMakeStruct>) {
        add(get_ptr(expr))
    }

    def get_call(expr : smart_ptr<ExprCallFunc>) : DllName {
        return get_base(get_ptr(expr), "call_{get_mangled_name(expr.func)}")
    }


    def override preVisitExprCall(expr : smart_ptr<ExprCall>) {
        add(get_ptr(expr))
    }

    def override preVisitExprOp2(var expr : smart_ptr<ExprOp2>) {
        add(get_ptr(expr))
    }

    def override preVisitExprOp1(var expr : smart_ptr<ExprOp1>) {
        add(get_ptr(expr))
    }

    def get_string_builder(expr : smart_ptr<ExprStringBuilder>) {
        return get_base(get_ptr(expr), "string_builder")
    }

    def override preVisitExprStringBuilder(expr : smart_ptr<ExprStringBuilder>) {
        add(get_ptr(expr))
    }

    def public get_block_name(blk : smart_ptr<ExprBlock>) : DllName {
        return get_base(get_ptr(blk), "block_{get_mangled_name(blk)}")
    }

    def override preVisitExprBlock(expr : smart_ptr<ExprBlock>) {
        add(get_ptr(expr))
    }

    def reset(var func : Function?) {
        thisFunc = func
    }

    def const get_dll_fn_name_ptr(fn : Function?) : DllName {
        return DllName("{get_namesp_prefix()}`{fn |> get_mangled_name()}")
    }

    def const get_dll_fn_name(fn : FunctionPtr) : DllName {
        return DllName("{get_namesp_prefix()}`{fn |> get_mangled_name()}")
    }

}


// All dll operations pass through this class 
struct public DLLHandle {
    handle : void?
    created_fileinfos : table<FileInfo?; FileInfo?>

    def reset() {
        if (handle != null) {
            handle |> close_dynamic_library()
            handle = null
        }
    }

    // def has_function(fn : FunctionPtr) : bool {
    //     if (handle == null) {
    //         return false
    //     }
    //     var written_hash = get_function_hash(get_dll_fn_name(fn))
    //     let is_ok = (written_hash != null && get_function_aot_hash(fn |> get_ptr) == *written_hash)
    //     return is_ok;
    // }

    def get_function_address(fn : DllName) {
        assert(handle != null, "dynamic_lib_handle is null")
        return handle |> get_function_address(fn.publ());
    }

    def get_function_impl(fn : DllName) {
        assert(handle != null, "dynamic_lib_handle is null")
        return handle |> get_function_address(fn.impl());
    }

    def get_function_hash(fn : DllName) {
        return unsafe(reinterpret<uint64?>(handle |> get_function_address(fn.id())))
    }


    def set_function_address(fn : DllName, ptr : void?) {
        assert(handle != null, "dynamic_lib_handle is null")
        var fn_addr = unsafe(reinterpret<void ??>(handle |> get_function_address(fn.publ())))
        // assert(fn_addr != null)
        if (fn_addr == null) {
            return false
        }
        print("{fn.publ()}\n")
        *fn_addr = ptr;
        return true
    }


    def create_fileinfo(fi : FileInfo?) {
        var res : FileInfo? = null;
        if (!created_fileinfos |> key_exists(fi)) {
            let fi_ptr = unsafe(reinterpret<void?>(fi))
            var sym = get_function_address(DllName("fileinfo_at_{fi_ptr}"))
            var sym_name = get_function_address(DllName("fileinfo_at_{fi_ptr}_name"))
            var name = unsafe(reinterpret<string>(sym_name))

            let src_fi = unsafe(reinterpret<FileInfo?>(sym))
            recreate_fileinfo_name(src_fi, name)
            return src_fi
        } else {
            return unsafe(created_fileinfos[fi])
        }
    }

}
