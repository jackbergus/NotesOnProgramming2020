//
// Created by giacomo on 10/01/2020.
//


#include <fort.hpp>
#include <bitset>

#include <utils/float.h>
#include <cmath>

std::ostream &operator<<(std::ostream &os, ieee754_float &classe) {
    std::bitset<sizeof(float)*CHAR_BIT> x(*reinterpret_cast<unsigned long*>(&classe.f));
    fort::char_table table;
    table.set_border_style(FT_SOLID_ROUND_STYLE);
    table << "Number" << classe.f << fort::endr
          << "Sign"   << classe.parts.sign << fort::endr
          << "Exponent" << classe.parts.exponent << fort::endr
          << "Mantissa" << classe.parts.mantissa << fort::endr
          << "Binary Representation" << x << fort::endr
          << "Binary Sign" << std::bitset<1>(classe.parts.sign) << fort::endr
          << "Binary Exponent" << " "+std::bitset<8>(classe.parts.exponent).to_string() << fort::endr
          << "Binary Mantissa" << "         "+std::bitset<23>(classe.parts.mantissa).to_string() << fort::endr
          << "Numeric Exponent" << "2^"+std::to_string(classe.parts.exponent-127) << fort::endr
          << "Numeric Mantissa" << classe.parts.mantissa / std::pow(2,23) << fort::endr
          << "Result" << (classe.parts.sign ? -1 : 1)*(1.0f+classe.parts.mantissa / std::pow(2,23))*std::pow(2, classe.parts.exponent-127 ) << fort::endr;

    return os << table.to_string() << std::endl;
}
