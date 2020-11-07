from das_binder.config import ConfigBase


class Config(ConfigBase):

    @property
    def das_module_name(self):
        return 'generatedBindings'

    @property
    def c_header_include(self):
        return 'header_to_bind.h'
