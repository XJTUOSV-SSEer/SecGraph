#ifndef UTILS_H
#define UTILS_H

#include "../pbcwrapper/Pairing.h"
#include "../common/data_type.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

// class HashResultZr{
//     public:
//         element_t element;

//         HashResultZr(pairing_t& pairing, std::vector<uint8_t> m) {
//             element_init_Zr(element, pairing);
//             element_from_hash(element, m.data(), m.size());
//         }
// }; 


int enc_aes_gcm(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key,
                unsigned char *ciphertext);//加密

int dec_aes_gcm(unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *key,
                unsigned char *plaintext);//解密




// HashResultZr HashToZr(pairing_t& pairing, std::vector<uint8_t>& m) {
//     return HashResultZr(pairing, m);
// }
#endif