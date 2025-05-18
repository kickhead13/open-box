#ifndef __OPEN_BOX_CRYPTO
#define __OPEN_BOX_CRYPTO
#include <nettle/sha1.h>
#include <gmp.h>
//#include <stdint.h>

/* RSA ALGO */
void gp(mpz_t,int);
void gpp(mpz_t,mpz_t);
void cn(mpz_t, mpz_t, mpz_t);
void phi(mpz_t, mpz_t, mpz_t);
void cp(mpz_t, mpz_t);
void euclid(mpz_t, mpz_t, mpz_t, mpz_t);
void privk(mpz_t, mpz_t, mpz_t);
void mtn(mpz_t,char*);
char *ntm(mpz_t);
void enc(mpz_t, mpz_t, mpz_t, mpz_t);
void enc_t(mpz_t, char *, mpz_t, mpz_t);
void dec(mpz_t, mpz_t, mpz_t, mpz_t);


/* DES ENC */
struct rkg {
  unsigned int c:28;
  unsigned int d:28;
};

uint64_t desk();
uint64_t ctk(char *);
char * ktc(uint64_t);
uint64_t iperm(uint64_t);
uint64_t desk();
char * ktc(uint64_t);
uint64_t ctk(char*);
uint64_t iperm(uint64_t);
uint64_t fperm(uint64_t);
uint64_t as(uint64_t, char);
uint64_t sbperm(uint64_t);
uint64_t sboxer(uint64_t);
uint64_t mangler(uint64_t, uint64_t);
uint64_t des_round(uint64_t, uint64_t);
uint64_t r16(uint64_t, uint64_t*);
uint64_t des_sswap(uint64_t);
uint64_t cperm(uint64_t );
uint64_t dperm(uint64_t );
uint64_t lhperm(uint64_t );
uint64_t rhperm(uint64_t );
uint64_t * des_keygen(uint64_t, char);
uint64_t des_enc(uint64_t , uint64_t );
uint64_t des_enc_rks(uint64_t, uint64_t, uint64_t*);
uint64_t des_dec(uint64_t , uint64_t );
uint64_t des_dec_rks(uint64_t, uint64_t, uint64_t*);


/* AES ENC */
typedef struct u128 {
  uint64_t l;
  uint64_t r;
} uint128_t;


uint128_t aes_enc(uint128_t, uint128_t);
uint128_t aes_dec(uint128_t, uint128_t);

#endif
