#include <stdint.h>
#include <libmsp/mem.h>
#define KEY_SIZE_BITS	64
#define DIGIT_BITS       8 // arithmetic ops take 8-bit args produce 16-bit result
#define DIGIT_MASK       0x00ff
#define NUM_DIGITS       (KEY_SIZE_BITS / DIGIT_BITS)

#if NUM_DIGITS < 2
#error The modular reduction implementation requires at least 2 digits
#endif

#define PRINT_HEX_ASCII_COLS 8

#define PLAINTEXT_SIZE 257

#define NUM_PLAINTEXT_BLOCKS (PLAINTEXT_SIZE / (NUM_DIGITS - NUM_PAD_DIGITS) + 1)
#define CYPHERTEXT_SIZE (NUM_PLAINTEXT_BLOCKS * NUM_DIGITS)
#define NUM_PAD_DIGITS 1

/** @brief Type large enough to store a product of two digits */
typedef uint16_t digit_t;

/** @brief Multi-digit integer */
typedef unsigned bigint_t[NUM_DIGITS * 2];

typedef struct {
	bigint_t n; // modulus
	digit_t e;  // exponent
} pubkey_t;


void encrypt_rsa(uint8_t *cyphertext, unsigned *cyphertext_len,
const uint8_t *message, unsigned message_length, const pubkey_t *k);


extern __nv pubkey_t pubkey;
extern __nv char PLAINTEXT[];
extern __nv volatile uint8_t CYPHERTEXT[CYPHERTEXT_SIZE];
extern __nv unsigned CYPHERTEXT_LEN;


