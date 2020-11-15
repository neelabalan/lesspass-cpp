#include <string>
#include <sstream>
#include "BigInt.hpp"

namespace util 
{
    BigInt hexchar_to_int(char ch)
    {
        return ( ch >= 'a') ? ( ch - 'a' + 10) : ( ch - '0');
    }

    auto pow( BigInt base, BigInt power ) 
    {
        BigInt value = 1;
        for ( unsigned int i = 0; i < power; i++ ) {
            value = value * base;
        }
        return value;
    }

    auto hexstream_to_bigint( std::string str ) 
    {
        BigInt val = 0;
        auto len = str.length();
        for ( unsigned int i = 0; i < len; i++ ) {
            val = val +  util::hexchar_to_int( str[i] ) * util::pow( 16, len - 1 - i ); 
        }
        return val;
    }

    auto int_to_hexstr( std::string val ) 
    {
        std::stringstream stream;
        stream << std::hex << std::atoi( val.c_str() );
        return stream.str();
    }

    std::pair<BigInt, BigInt> divmod( BigInt dividend, BigInt divisor ) {
        return std::make_pair(dividend / divisor, dividend % divisor );
    }
}