#pragma once
namespace das
{
    extern intptr_t ModuleKarma;
    class Module;
};

#define NEED_MODULE(ClassName) \
    extern das::Module * register_##ClassName (); \
    das::ModuleKarma += intptr_t(register_##ClassName());
