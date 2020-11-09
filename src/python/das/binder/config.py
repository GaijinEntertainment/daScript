class ConfigBase(object):

    @property
    def das_module_name(self):
        '''Module name as to be seen by das "require".'''
        raise NotImplementedError()

    @property
    def c_header_include(self):
        '''C header for which to generate bindings, but in the form suitable for #include in generated das::Module cpp file.'''
        raise NotImplementedError()

    def configure_enum(self, enum):
        '''This function is called for each encountered enum.'''
        pass

    def configure_struct(self, struct):
        '''This function is called for each encountered struct.'''
        pass

    def configure_struct_field(self, field):
        '''This function is called for each field in each struct.'''
        pass
