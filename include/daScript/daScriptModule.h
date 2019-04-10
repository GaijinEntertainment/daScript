#pragma once
namespace das
{
    extern unsigned ModuleKarma;
    class Module;
};

#define NEED_MODULE(ClassName) \
    extern das::Module * register_##ClassName (); \
    das::ModuleKarma += unsigned(intptr_t(register_##ClassName()));
