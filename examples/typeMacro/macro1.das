module macro1 public

require daslib/typemacro_boost

[template_structure(CounterType, initialValue=1)]
struct template TemplateCounter {
    private counter : CounterType
    def TemplateCounter {
        counter = CounterType($v(initialValue))
    }
    def next {
        counter ++
    }
    def const value {
        return counter
    }
}

