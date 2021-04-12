#ifndef ECC_H
#define ECC_H

#include <stdint.h>

typedef unsigned int uint;

/* Define as 1 to enable ECDSA functions, 0 to disable.
 */
#define ECC_ECDSA 1

/* Optimization settings. Define as 1 to enable an optimization, 0 to disable it.
ECC_SQUARE_FUNC - If enabled, this will cause a specific function to be used for (scalar) squaring instead of the generic
                  multiplication function. Improves speed by about 8% .
*/
#define ECC_SQUARE_FUNC 1

/* Inline assembly options.
Currently we do not provide any inline assembly options. In the future we plan to offer
inline assembly for AVR and 8051.

Note: You must choose the appropriate option for your target architecture, or compilation will fail
with strange assembler messages.
*/
#define ecc_asm_none   0
#ifndef ECC_ASM
    #define ECC_ASM ecc_asm_none
#endif

/* Curve selection options. */
#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48

#define ECC_CURVE secp256r1
#ifndef ECC_CURVE
    #define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
    #error "Must define ECC_CURVE to one of the available curves"
#endif

#define NUM_ECC_DIGITS ECC_CURVE

typedef struct EccPoint
{
    uint8_t x[NUM_ECC_DIGITS];
    uint8_t y[NUM_ECC_DIGITS];
} EccPoint;



class ecc
{
public:
    static int make_key(EccPoint *p_publicKey, uint8_t p_privateKey[NUM_ECC_DIGITS], uint8_t p_random[NUM_ECC_DIGITS]);
    static int valid_public_key(EccPoint *p_publicKey);
    static int ecdsa_sign(uint8_t p_privateKey[NUM_ECC_DIGITS], uint8_t p_hash[NUM_ECC_DIGITS], uint8_t p_signature[NUM_ECC_DIGITS*2]);
    static int ecdsa_verify(EccPoint *p_publicKey, uint8_t p_hash[NUM_ECC_DIGITS], uint8_t r[NUM_ECC_DIGITS], uint8_t s[NUM_ECC_DIGITS]);
private:
    static int getRandomNumber(uint8_t *p_vli);
    static void vli_clear(uint8_t *p_vli);
    static int vli_isZero(uint8_t *p_vli);
    static uint8_t vli_testBit(uint8_t *p_vli, uint p_bit);
    static uint vli_numDigits(uint8_t *p_vli);
    static uint vli_numBits(uint8_t *p_vli);
    static void vli_set(uint8_t *p_dest, uint8_t *p_src);
    static int vli_cmp(uint8_t *p_left, uint8_t *p_right);
    static uint8_t vli_lshift(uint8_t *p_result, uint8_t *p_in, uint p_shift);
    static void vli_rshift1(uint8_t *p_vli);
    static uint8_t vli_add(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
    static uint8_t vli_sub(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
    static void vli_mult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
    static void vli_modAdd(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
    static void vli_modSub(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
    static void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product);
    static void vli_modMult_fast(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
    static void vli_square(uint8_t *p_result, uint8_t *p_left);
    static void vli_modSquare_fast(uint8_t *p_result, uint8_t *p_left);
    static void vli_modInv(uint8_t *p_result, uint8_t *p_input, uint8_t *p_mod);
    static int EccPoint_isZero(EccPoint *p_point);
    static void EccPoint_double_jacobian(uint8_t *X1, uint8_t *Y1, uint8_t *Z1);
    static void apply_z(uint8_t *X1, uint8_t *Y1, uint8_t *Z);
    static void XYcZ_initial_double(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2, uint8_t *p_initialZ);
    static void XYcZ_add(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);
    static void XYcZ_addC(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);
    static void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint8_t *p_scalar, uint8_t *p_initialZ);
    static int ecdh_shared_secret(uint8_t p_secret[NUM_ECC_DIGITS], EccPoint *p_publicKey, uint8_t p_privateKey[NUM_ECC_DIGITS], uint8_t p_random[NUM_ECC_DIGITS]);
    static void vli_modMult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
    static void bytes2native(uint8_t p_native[NUM_ECC_DIGITS], uint8_t p_bytes[NUM_ECC_DIGITS*4]);
    static void native2bytes(uint8_t p_bytes[NUM_ECC_DIGITS*4], uint8_t p_native[NUM_ECC_DIGITS]);
};

#endif // ECC_H
