#include "crypto.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PD
#define PD 211
#endif

#ifndef QD
#define QD 263
#endif

#ifndef CPD
#define CPD 233
#endif

/*Generate n digit prime.
 * int n       -- number of digits
 * mpz_t prime -- stores prime
 * return void -- NTR
 * */
void gp(mpz_t prime, int digits) {
    gmp_randstate_t state;
    mpz_t min, max;
    
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    
    mpz_init(min);
    mpz_init(max);
    mpz_ui_pow_ui(min, 10, digits - 1);
    mpz_ui_pow_ui(max, 10, digits);
    mpz_sub_ui(max, max, 1);
    
    mpz_urandomm(prime, state, max);
    mpz_add(prime, prime, min);
    
    mpz_nextprime(prime, prime);
    
    mpz_clear(min);
    mpz_clear(max);
    gmp_randclear(state);
}


/* Generate prime pair.
 * mpz_t p
 * mpz_t q
 * return void -- NTR
 * */
void gpp(mpz_t p, mpz_t q) {
  gp(p, PD);
  gp(q, QD);
}

/* Computes phi(p,q): (p-1)(q-1).
 * mpz_t ret   -- phi
 * mpz_t p 
 * mpz_t q
 * return void -- NTR
 * */
void phi(mpz_t ret, mpz_t p, mpz_t q) {
  mpz_t q1;
  mpz_init(q1);
  mpz_sub_ui(ret, p, 1);
  mpz_sub_ui(q1, q, 1);
  mpz_mul(ret, ret, q1);
  mpz_clear(q1);
}

/* Computes coprime of p.
 *  
 * */
void cp(mpz_t rez, mpz_t p) {
  gp(rez, CPD);
}

void euclid(mpz_t a , mpz_t b , mpz_t x, mpz_t y)
{
    if(mpz_cmp_ui(b, 0) == 0)
    {
        mpz_set_ui(x, 1);
        mpz_set_ui(y, 1);
    }
    else
    {
        mpz_t x1;
        mpz_init(x1);
        mpz_t y1;
        mpz_init(y1);
        mpz_t mod;
        mpz_init(mod);
        mpz_mod(mod, a, b);
        euclid(b , mod, x1 , y1);
        mpz_set(x, y1);
        mpz_t div;
        mpz_init(div);
        mpz_fdiv_q(div, a, b);
        mpz_t mul;
        mpz_init(mul);
        mpz_mul(mul, div, y1);
        mpz_sub(y, x1, mul);
        mpz_clear(div);
        mpz_clear(mul);
        mpz_clear(mod);
        mpz_clear(x1);
        mpz_clear(y1);
    }
}

void privk(mpz_t d, mpz_t e, mpz_t n) {
  mpz_t y;
  mpz_init(y);
  euclid(e, n, d, y);
  while(mpz_cmp_ui(d, 0) < 0) mpz_add(d, d, n);
  mpz_clear(y);
}

void mtn(mpz_t n, char *m) {
  if(m == NULL) return;
  char *c = m;
  for(;*c;c++) {
    mpz_mul_ui(n, n, 1000);
    mpz_add_ui(n, n, *c);
  }
}

char * ntm(mpz_t n) {
  size_t len = mpz_sizeinbase(n, 10) / 3;
  mpz_t rem;
  mpz_init(rem);
  mpz_t t;
  mpz_init(t);
  mpz_set_ui(t, 1000);
  char * m = (char*)malloc(sizeof(char) * (len + 1));
  

  m[len] = '\0';
  len--;
  for(;mpz_cmp_ui(n, 0) > 0;len--) {
    mpz_mod_ui(rem, n, 1000);
    m[len] = (char)mpz_get_ui(rem);
    mpz_fdiv_q(n, n, t);
  }
  mpz_clear(t);
  mpz_clear(rem);
  return m;
}

void cn(mpz_t n, mpz_t p, mpz_t q) {
  mpz_mul(n, p, q);
}

void enc(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
  mpz_powm(c, m, e, n);
}

void dec(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
  mpz_powm(m, c, d, n);
}


void enc_t(mpz_t cypher, char *message, mpz_t pubk, mpz_t n) {
  mpz_t m;
  mpz_init(m);

  mtn(m, message);

  enc(cypher, m, pubk, n);
}
