#include "crypto.h"
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define BSIZE 64
#define B32 32
#define B28 28
#define B24 24
#define ENC_MODE 1

const int bmap1[64] = {
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17, 9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7
};

const int bmapf[64] = {
  40, 8, 48, 16, 56, 24, 64, 32,
  39, 7, 47, 15, 55, 23, 63, 31, 
  38, 6, 46, 14, 54, 22, 62, 30, 
  37, 5, 45, 13, 53, 21, 61, 29,
  36, 4, 44, 12, 52, 20, 60, 28,
  35, 3, 43, 11, 51, 19, 59, 27,
  34, 2, 42, 10, 50, 18, 58, 26,
  33, 1, 41,  9, 49, 17, 57, 25,
};

const int lhbmap[24] = {
  14, 17, 11, 24, 1, 5,
  3, 28, 15, 6, 21, 10,
  23, 19, 12, 4, 26, 8,
  16, 7, 27, 20, 13, 2
};

const static size_t rhbmap[24] = {
  41, 52, 31, 37, 47, 55,
  30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53,
  46, 42, 50, 36, 29, 32
};

const static size_t cbmap[28] = {
  57, 49, 41, 33, 25, 17, 9,
  1, 58, 50, 42, 34, 26, 18,
  10, 2, 59, 51, 43, 35, 27,
  19, 11, 3, 60, 52, 44, 36
}; 

const static size_t dbmap[28] = {
  63, 55, 47, 39, 31, 23, 15,
  7, 62, 54, 46, 38, 30, 22,
  14, 6, 61, 53, 45, 37, 29,
  21, 13, 5, 28, 20, 12, 4
};

const static size_t sbpbmap[32] = {
  16, 7, 20, 21, 29, 12, 28, 17,
  1, 15, 23, 26, 5, 18, 31, 10,
  2, 8, 24, 14, 32, 27, 3, 9,
  19, 13, 30, 6, 22, 11, 4, 25
};

const static uint64_t sbox1[4] = {
  0xe4d12fb83a6c5907,
  0x0f74e2d1a6cb9538,
  0x41e8d62bfc973a50,
  0xfc8249175b3ea06d,
};

const static uint64_t sbox2[4] = {
  0x3af084d169e2bc75,
  0x123f9eca6078b4d5,
  0xd9ecf063a1b25487,
  0x5feba019d4c28736,
};

/* Creates random 64 bit DES key.
 * */
uint64_t desk() {
  srand(time(NULL));
  uint64_t k = (uint64_t)rand();
  k <<= 32;
  k += (uint64_t)rand();
  return k;
}

/* Returns ascii representation of uint64_t value.
 * */
char * ktc(uint64_t k) {
  char * ck = (char*)malloc(sizeof(char) * 9);
  uint64_t t = k;
  char count = 7;
  for(;t;t>>=8) {
    ck[count] = (char)(t & 0xff);
    count--;
  }
  ck[8] = '\0';
  return ck;
}

uint64_t ctk(char *ck) {
  char * t = ck;
  uint64_t k = 0;
  for(;*t;t++) {
    k |= *t;
    if(t+1 && *(t+1))k <<= 8;
  }
  return k; 
}

uint64_t iperm(uint64_t k) {
  uint64_t ok = 0;
  for(size_t i = 0; i < BSIZE; i++) {
    ok += (k >> (BSIZE - bmap1[i])) & 0x01;
    if(i < BSIZE - 1)ok <<= 1;
  }
  return ok;
}

uint64_t fperm(uint64_t k) {
  uint64_t ok = 0;
  for(size_t i = 0; i < BSIZE; i++) {
    ok += (k >> (BSIZE - bmapf[i])) & 0x01;
    if(i < BSIZE - 1) ok <<= 1;
  }
  return ok;
}


uint64_t as(uint64_t v, char s) {
  uint64_t ro = v & 1;
  ro |= (v & 0x20) >> 5;
  uint64_t co = (v << 1) >> 2;
  return (s == 1 ? ((sbox1[ro] << (co-1)) >> 15) : ((sbox2[ro] << (co-1)) >> 15));
}

uint64_t sbperm(uint64_t v) {
  uint64_t res = 0;
  for(size_t i = 0; i < B32; i++) {
    res += (v >> (B32 - sbpbmap[i])) & 0x01;
    if(i < B32 - 1) res <<= 1;
  }
  return res;
}

uint64_t sboxer(uint64_t in) {
  uint64_t res = 0;
  res |= (as(in >> 42, 1) << 28);
  res |= (as((in << 6) >> 36, 2) << 24);
  res |= (as((in << 12) >> 36, 1) << 20);
  res |= (as((in << 18) >> 36, 2) << 16);
  res |= (as((in << 24) >> 36, 1) << 12);
  res |= (as((in << 30) >> 36, 2) << 8);
  res |= (as((in << 36) >> 36, 1) << 4);
  res |= as((in << 42) >> 36, 2);
  return sbperm(res);
}

uint64_t mangler(uint64_t in, uint64_t rk) {
  uint64_t res = 0;
  res = (in & 1) < 47;
  res |= ((in >> 27) << 43);
  res |= (((in << 3) >> 26) << 36);
  res |= (((in << 7) >> 26) << 30);
  res |= (((in <<11) >> 26) << 24);
  res |= (((in <<15) >> 26) << 18);
  res |= (((in <<19) >> 26) << 12);
  res |= (((in <<23) >> 26) << 6);
  res |= ((in <<27) >> 26);
  res |= ((in & 0x80000000) >> 31);
  return sboxer(res ^ rk);
}

uint64_t des_round(uint64_t in, uint64_t rk) {
  uint64_t left = (in >> B32);
  uint64_t right = (in << B32) >> B32;
  uint64_t man = mangler(right, rk);
  uint64_t nl = left ^ man;
  return (right << B32) + nl;
}

uint64_t r16(uint64_t in, uint64_t *rks) {
  size_t iter = 0;
  for(;iter < 16; iter++) {
    in = des_round(in, rks[iter]);
  }
  return in;
}

/* Swaps left 32 bits and right 32 bits arround:
 * 000...000 111...111
 *          |
 *          v
 * 111...111 000...000
 *
 *  uint64_t v      -- value to be swaped
 *  return uint64_T -- swapped value
 * */
uint64_t des_sswap(uint64_t v) {
  return (v << B32) + (v >> B32);
}

uint64_t cperm(uint64_t k) { 
  uint64_t res = 0;
  for(size_t i = 0; i < B28; i++) {
    res += (k >> (BSIZE - cbmap[i])) & 0x01;
    if(i < B28 - 1) res <<= 1;
  }
  return res;
}

uint64_t dperm(uint64_t k) { 
  uint64_t res = 0;
  for(size_t i = 0; i < B28; i++) {
    res += (k >> (BSIZE - dbmap[i])) & 0x01;
    if(i < B28 - 1) res <<= 1;
  }
  return res;
}

uint64_t lhperm(uint64_t c) { 
  uint64_t res = 0;
  for(size_t i = 0; i < B24; i++) {
    res += (c >> (BSIZE - lhbmap[i])) & 0x01;
    if(i < B24 - 1) res <<= 1;
  }
  return res;
}

uint64_t rhperm(uint64_t d) { 
  uint64_t res = 0;
  for(size_t i = 0; i < B24; i++) {
    res += (d >> (BSIZE - rhbmap[i])) & 0x01;
    if(i < B24 - 1) res <<= 1;
  }
  return res;
}

uint64_t * des_keygen(uint64_t k, char mode) {
  uint64_t c = cperm(k);
  uint64_t d = dperm(k);
  uint64_t * rks = (uint64_t*)malloc(sizeof(uint64_t) * 20);
  ssize_t rnd = 0;
  if(mode == ENC_MODE) {
    for(;rnd < 16; rnd++) {
      c <<= 1;
      d <<= 1;
      rks[rnd] = ((lhperm(c) << 6) + rhperm(d));
    }
  } else {
    rnd = 15;
    for(;rnd >= 0; rnd--) {
      c <<= 1;
      d <<= 1;
      rks[rnd] = ((lhperm(c) << 6) + rhperm(d));
    }
  }
  return rks;
}

/* DES encryption.
 *  uint64_t m       -- message (plain)
 *  uint64_t k       -- key
 *  return uint64_t  -- message (cyphertext)
 * */
uint64_t des_enc(uint64_t m, uint64_t k) {
  uint64_t * rks = des_keygen(k, 1);
  uint64_t ct = iperm(m);
  ct = r16(ct, rks);
  free(rks);
  return fperm(des_sswap(ct));
}

/* DES encryption using already generated Round Keys.
 * ! IMPORTANT: rks MUST be generated with mode = 1 to work
 * properly.
 *
 *  uint64_t m      -- message (plain)
 *  uint64_t k      -- key
 *  uint64_t *rks   -- round keys
 *  return uint64_t -- message (cyphertext)
 * */
uint64_t des_enc_rks(uint64_t m, uint64_t k, uint64_t * rks) {
  return fperm(des_sswap(r16(iperm(m), rks)));
}

/* DES decryption.
 *  uint64_t m      -- message (plain)
 *  uint64_t k      -- key
 *  return uint64_t -- message (cyphertext)
 * */
uint64_t des_dec(uint64_t m, uint64_t k) {
  uint64_t * rks = des_keygen(k, 0);
  uint64_t ct = iperm(m);
  ct = r16(ct, rks);
  free(rks);
  return fperm(des_sswap(ct));
}

/* DES decryption with Round Keys already generated,
 * ! IMPORTANT: rks must be generated with mode = 0 before hand, to work 
 * properly.
 *  
 *  uint64_t m      -- message (cyphertext)
 *  uint64_t k      -- key
 *  uint64_t rks    -- round keys
 *  return uint64_t -- message (plain)
 * */
uint64_t des_dec_rks(uint64_t m, uint64_t k, uint64_t * rks) {
  return fperm(des_sswap(r16(iperm(m), rks)));
}
