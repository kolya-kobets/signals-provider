#ifndef CONVERTER_H
#define CONVERTER_H

#include "base_types.h"

class Converter
{
public:
    static Signal convert(const Frame& frame);
};

#endif // CONVERTER_H
