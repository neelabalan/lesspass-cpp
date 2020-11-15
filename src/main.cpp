#include <iostream>
#include <unordered_map>
#include <string_view>
#include <utility>
#include <vector>

#include "argparse.hpp"
#include "pbkdf2_hmac.h"
#include "util.h"
#include "BigInt.hpp"

enum SUBSET {
    lowercase,
    uppercase,
    digits,
    symbols
};

static const std::unordered_map< SUBSET, std::string > CHARACTER_MAP = {
    { SUBSET::lowercase,  "abcdefghijklmnopqrstuvwxyz"        },
    { SUBSET::uppercase,  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"        },
    { SUBSET::digits,     "0123456789"                        },
    { SUBSET::symbols,    "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"},
};
// );


// add cli arguments
void _add_arguments( argparse::ArgumentParser & );

// get values required to generate salt
std::unordered_map<std::string, std::string> create_salt_profile( argparse::ArgumentParser ); 

// get salt value from site, login and counter hex value 
std::string _get_some_salt( std::unordered_map<std::string, std::string> ); 

// calculate entropy with password and salt
BigInt calculate_entropy( std::string, std::string );

// get rules like uppercase? lowercase? ...
std::vector< SUBSET > _get_rules( argparse::ArgumentParser );

// get characters based on the rules
std::string _get_set_of_characters( std::vector< SUBSET > );

// consume entropy generated and give back entropy and pass
std::pair<std::string, BigInt> _consume_entropy( std::string, BigInt, std::string, unsigned int ); 

// get one character from one rule
std::pair< BigInt, std::string > _get_one_char_per_rule( BigInt, std::vector< SUBSET > );

// insert character in the password obtained "randomly"
std::string _insert_string_pseudo_randomly( BigInt, std::string, std::string );

int main(int argc, char *argv[])
{
    argparse::ArgumentParser args("lesspass-cpp");
    try 
    {
        _add_arguments( args );
        args.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << args;
        exit(0);
    }
    auto profile = create_salt_profile( args );
    auto salt = _get_some_salt( profile );
    BigInt entropy = calculate_entropy( salt, profile[ "pass" ] );

    std::string password {};
    BigInt password_entropy = std::string( "0" );

    auto rules = _get_rules( args );
    auto set_of_characters = _get_set_of_characters( rules );
    auto password_length = args.get< int >( "--length" );

    std::tie( password, password_entropy ) = _consume_entropy( 
        std::string(""),
        entropy,
        set_of_characters,
        ( password_length -  rules.size() )
    );

    BigInt character_entropy = std::string( "0" );
    std::string chars {};

    std::tie( character_entropy, chars )= _get_one_char_per_rule( password_entropy, rules );

    auto final_password = _insert_string_pseudo_randomly( character_entropy, password, chars );

    std::cout<< final_password;

}

std::string _insert_string_pseudo_randomly( BigInt character_entropy, std::string generated_password, std::string chars )
{
    for( auto& letter : chars ) {
        BigInt remainder = generated_password.length();
        std::tie( character_entropy, remainder ) = util::divmod( character_entropy, remainder );
        generated_password.insert( remainder.to_int(), 1, letter ); 
    }
    return generated_password;
}
std::pair< BigInt, std::string > _get_one_char_per_rule( BigInt password_entropy, std::vector< SUBSET > rules ) {
    std::string one_char_per_rule {}, value {};

    for( auto& rule : rules ) {
        std::tie( value, password_entropy ) = _consume_entropy( 
            std::string(""),
            password_entropy,
            CHARACTER_MAP.at( rule ),
            1
        );
        one_char_per_rule += value;
    }
    return std::make_pair( password_entropy, one_char_per_rule );
}


std::string _get_set_of_characters( std::vector< SUBSET > rules )
{
    std::string set_of_characters {};
    
    for ( auto& rule : rules ) {
        if ( rule == SUBSET::lowercase ) { set_of_characters += CHARACTER_MAP.at( SUBSET::lowercase );  }
        if ( rule == SUBSET::uppercase ) { set_of_characters += CHARACTER_MAP.at( SUBSET::uppercase );  }
        if ( rule == SUBSET::digits )    { set_of_characters += CHARACTER_MAP.at( SUBSET::digits );     }
        if ( rule == SUBSET::symbols )   { set_of_characters += CHARACTER_MAP.at( SUBSET::symbols );    }
    }
    return set_of_characters;
}
std::pair<std::string, BigInt> _consume_entropy( std::string gen_pass, BigInt quotient, std::string chars, unsigned int len ) 
{
    BigInt remainder;
    if ( gen_pass.length() >= len ) {
        return std::make_pair( gen_pass, quotient );
    }
    std::tie( quotient, remainder ) = util::divmod( quotient, chars.length() );
    gen_pass += chars[ remainder.to_int() ];
    return _consume_entropy( gen_pass, quotient, chars, len );

}
BigInt calculate_entropy( std::string salt, std::string pass ) 
{
    return util::hexstream_to_bigint( 
        PBKDF2_HMAC_SHA_256( reinterpret_cast<const unsigned char *>(salt.c_str()), pass.c_str() )
    );
}

std::string _get_some_salt( std::unordered_map<std::string, std::string> profile ) 
{
    return (profile[ "site" ] + profile[ "login" ] + util::int_to_hexstr( profile[ "counter" ] )); 
}

std::unordered_map<std::string, std::string> create_salt_profile( argparse::ArgumentParser args ) 
{
    std::unordered_map<std::string, std::string> profile;
    profile.insert( { "site",       args.get<std::string>( "--site" )     });
    profile.insert( { "login",      args.get<std::string>( "--login" )    });
    profile.insert( { "counter",    args.get<std::string>( "--counter" )  });
    profile.insert( { "pass",       args.get<std::string>( "--pass" )     });
    return profile;
}
std::vector<SUBSET> _get_rules( argparse::ArgumentParser args ) 
{
    std::vector< SUBSET > rules;
    if ( args.get<bool>( "-l" ) ) { rules.push_back( SUBSET::lowercase ); }
    if ( args.get<bool>( "-u" ) ) { rules.push_back( SUBSET::uppercase ); }
    if ( args.get<bool>( "-d" ) ) { rules.push_back( SUBSET::digits );    }
    if ( args.get<bool>( "-s" ) ) { rules.push_back( SUBSET::symbols );   }
    
    return rules;
}
void _add_arguments( argparse::ArgumentParser & args )
{
    args.add_argument( "--site" )
        .help( "site" )
        .default_value( std::string(" ") )
        .required();

    args.add_argument( "--login" )
        .help( "login" )
        .default_value( std::string("") )
        .required();

    args.add_argument("--pass")
        .help( "password" )
        .default_value( std::string("") )
        .required();

    args.add_argument( "--counter" )
        .help( "counter (default is 5)" )
        .default_value( std::string("5") );

    args.add_argument( "-l", "--no-lower" )
        .help( "exclude lowecase characters in password" )
        .default_value( true )
        .implicit_value( false );

    args.add_argument( "-u", "--no-upper" )
        .help("exclude uppercase characters in password")
        .default_value( true )
        .implicit_value( false );

    args.add_argument( "-d", "--no-digits" )
        .help("exclude digits in password")
        .default_value( true )
        .implicit_value( false );

    args.add_argument( "-s", "--no-symbols" )
        .help("exclude symbols in password")
        .default_value( true )
        .implicit_value( false );

    args.add_argument("--length")
        .help("length of the password string (default is 10)")
        .default_value( 10 )
        .action([](const std::string &value) { return std::stoi(value); });

}
