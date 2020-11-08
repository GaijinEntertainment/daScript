from das.binder.config import ConfigBase


class Config(ConfigBase):

    @property
    def das_module_name(self):
        return 'vulkan'

    @property
    def c_header_include(self):
        return 'vulkan/vulkan.h'

    def configure_struct(self, struct):
        #FIXME: make it work for all structs
        for field in struct.fields:
            if (field.is_array
                or field.name.startswith('pfn')
                or field.is_bit_field
            ):
                struct.ignore()
                return
            for kw in [
                '*',
                'unsigned long',

                # the following are unions:
                'VkClearValue',
                'VkPerformanceValueDataINTEL',
            ]:
                if kw in field.type:
                    struct.ignore()
                    return
