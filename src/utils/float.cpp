//
// Created by giacomo on 10/01/2020.
//


#include <fort.hpp>
#include <bitset>

#include <utils/float.h>
#include <cmath>
#include <iomanip>

template <typename T> std::string tostr(const T& t) {
    std::ostringstream os;
    os << std::setprecision(26) << t;
    return os.str();
}

std::ostream &operator<<(std::ostream &os, ieee754_float &classe) {
    /*
     * Checking if the number is normalized as defined by the IEEE754 standard
     */
    bool isNumberNotNormalized = ((classe.parts.exponent == 0) && (classe.parts.mantissa != 0.0));

    /*
     * Using the utility methods from the parts struct to print the final string result and the exponent value
     */
    std::string result;
    std::string exponent;
    if (classe.parts.NaN()) {
        result = "NaN";
    } else if (classe.parts.isZero()) {
        result = "0";
    } else if (classe.parts.isInfinity()) {
        result = "∞";
    } else {
        result = tostr(isNumberNotNormalized ?
                          (classe.parts.mantissa / std::pow(2, 23)) * std::pow(2, -126) :
                          (1.0f + classe.parts.mantissa / std::pow(2, 23)) * std::pow(2, classe.parts.exponent - 127)
                       );
        exponent = (isNumberNotNormalized ? "2^-126" : "2^"+std::to_string(classe.parts.exponent-127));
    }

    /*
     * Bitsets allow to ingest a given variable and to fill a set of bits. Then, the set of bits can be printed to a
     * string using an utility function. Before doing that, we need to typecast the float into a unsigned number. The
     * most effective way to do this is to typecast tye float pointer &classe.f to a unsigned long ponter, and then
     * return the associated value by dereference. Then, we need to tell the class that the final representation will be
     * sizeof(float) bits long.
     */
    std::bitset<sizeof(float)*CHAR_BIT> x(*reinterpret_cast<unsigned long*>(&classe.f));

    // Using LibFort for pretty printing the
    fort::char_table table;
    // Setting the table's borders
    table.set_border_style(FT_SOLID_ROUND_STYLE);
    // Setting the style for the first column
    table.column(0).set_cell_text_style(fort::text_style::bold);

    // First Column  // Second Column   // Line End
    table << "Number" << tostr(classe.f) << fort::endr
          << "IsNormalized" << ((!isNumberNotNormalized) ? "true" : "false") << fort::endr
          << "Sign"   << (classe.parts.sign) << fort::endr
          << "Exponent" << classe.parts.exponent << fort::endr
          << "Mantissa" << classe.parts.mantissa << fort::endr
          << "Binary Representation" << x << fort::endr
          // Bitsets correctly handle unsigned ints, and so we need only to specify how many bits are represented
          << "Binary Sign" << std::bitset<1>(classe.parts.sign) << fort::endr
          // Adding some trailing space to align the binary representation of the exponent to the binary representation
          // of the float
          << "Binary Exponent" << " "+std::bitset<8>(classe.parts.exponent).to_string() << fort::endr
          << "Binary Mantissa" << "         "+std::bitset<23>(classe.parts.mantissa).to_string() << fort::endr
          << "Numeric Exponent" <<  exponent << fort::endr
          << "Result" << (classe.parts.sign ? "-" : "+") + result << fort::endr;

    return os << std::fixed <<   table.to_string() << std::endl;
}
