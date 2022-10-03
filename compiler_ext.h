#ifndef COMPILER_EXT_H
#define COMPILER_EXT_H

#ifdef _MSC_VER
#  define PACKED_STRUCT(name) \
    __pragma(pack(push, 1)) struct alignas(1) name __pragma(pack(pop))
#elif defined(__GNUC__)
#  define PACKED_STRUCT(name) struct __attribute__((packed, aligned(1))) name
#endif

#endif // COMPILER_EXT_H
