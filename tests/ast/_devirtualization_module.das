options gen2

module _devirtualization_module public

class sealed ModuleSealed {
    n : int = 3
    def read : int => n * 2
}

class ModulePlain {
    n : int = 4
    def read : int => n * 2
}
