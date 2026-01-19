module macro3 public

require daslib/typemacro_boost

struct template TemplateCounter {
    counter : CounterType
    def TemplateCounter {
        counter = CounterType($v(initialValue))
    }
    def next {
        counter ++
    }
    def value {
        return counter
    }
}

def template print_counter(c : $t(resType)) {
    print("Counter value: {c.value()}\n")
}

[typemacro_template_function(TemplateCounter)]
def makeTemplateCounter(macroArgument, passArgument : TypeDeclPtr; CounterType : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
    return <- default<TypeDeclPtr>
}

[typemacro_function]
def TemplateCounter(macroArgument, passArgument : TypeDeclPtr; CounterType : TypeDeclPtr; initialValue : int = 123) : TypeDeclPtr {
    var inscope resType <- makeTemplateCounter(macroArgument, passArgument, CounterType, initialValue)
    if (resType != null && passArgument == null) {
        if (!is_custom_work_done(resType.structType)) {
            mark_custom_work_done(resType.structType)
            var inscope print_fn <- qmacro_template_function(@@print_counter)
            add_function(compiling_module(), print_fn)
        }
    }
    return <- resType
}

