from das_binder.config import ConfigBase


class Config(ConfigBase):

    @property
    def das_module_name(self):
        return 'vulkan'

    @property
    def c_header_include(self):
        return 'vulkan/vulkan.h'

    def configure_enum(self, enum):
        #FIXME: these are failing to bind, saying incomplete type
        if enum.name in {
            'VkFormat',
            'VkStructureType',
        }:
            enum.ignore()
