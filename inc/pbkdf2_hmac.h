// from Anti-weakpasswords transformed to modern C++ syntax
// https://github.com/Anti-weakpasswords/PBKDF2-GCC-OpenSSL-library/blob/master/pbkdf2_openssl.c
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

const unsigned int iterations = 100000;
const unsigned int outputBytes = 32;

auto PBKDF2_HMAC_SHA_256(const unsigned char *salt, const char *pass)
{
    std::stringstream hstream;
    unsigned char digest[outputBytes];

    PKCS5_PBKDF2_HMAC(
        pass, std::strlen(pass), salt, std::strlen((char *)salt), iterations,
        EVP_sha256(), outputBytes, digest
    );

    for (unsigned int i = 0; i < sizeof(digest); i++)
    {
        hstream << std::hex << std::setfill('0') << std::setw(2) << std::right
                << (255 & digest[i]);
    }
    return hstream.str();
}
