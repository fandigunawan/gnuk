/*
cc -c -g t-x448.c 
cc -c -g ../src/p448.c
cc -c -g ../src/ecc-x488.c
 */

#include <stdint.h>
#include <string.h>

static const uint8_t *k0
= (uint8_t *)
  "\x3d\x26\x2f\xdd\xf9\xec\x8e\x88"
  "\x49\x52\x66\xfe\xa1\x9a\x34\xd2"
  "\x88\x82\xac\xef\x04\x51\x04\xd0"
  "\xd1\xaa\xe1\x21\x70\x0a\x77\x9c"
  "\x98\x4c\x24\xf8\xcd\xd7\x8f\xbf"
  "\xf4\x49\x43\xeb\xa3\x68\xf5\x4b"
  "\x29\x25\x9a\x4f\x1c\x60\x0a\xd3";

static const uint8_t *u0
= (uint8_t *)
  "\x06\xfc\xe6\x40\xfa\x34\x87\xbf"
  "\xda\x5f\x6c\xf2\xd5\x26\x3f\x8a"
  "\xad\x88\x33\x4c\xbd\x07\x43\x7f"
  "\x02\x0f\x08\xf9\x81\x4d\xc0\x31"
  "\xdd\xbd\xc3\x8c\x19\xc6\xda\x25"
  "\x83\xfa\x54\x29\xdb\x94\xad\xa1"
  "\x8a\xa7\xa7\xfb\x4e\xf8\xa0\x86";

static const uint8_t *r0
= (uint8_t *)
  "\xce\x3e\x4f\xf9\x5a\x60\xdc\x66"
  "\x97\xda\x1d\xb1\xd8\x5e\x6a\xfb"
  "\xdf\x79\xb5\x0a\x24\x12\xd7\x54"
  "\x6d\x5f\x23\x9f\xe1\x4f\xba\xad"
  "\xeb\x44\x5f\xc6\x6a\x01\xb0\x77"
  "\x9d\x98\x22\x39\x61\x11\x1e\x21"
  "\x76\x62\x82\xf7\x3d\xd9\x6b\x6f";

static const uint8_t g_x[56] = { 5, 0, };

static const uint8_t *r3
= (uint8_t *)
  "\x3f\x48\x2c\x8a\x9f\x19\xb0\x1e"
  "\x6c\x46\xee\x97\x11\xd9\xdc\x14"
  "\xfd\x4b\xf6\x7a\xf3\x07\x65\xc2"
  "\xae\x2b\x84\x6a\x4d\x23\xa8\xcd"
  "\x0d\xb8\x97\x08\x62\x39\x49\x2c"
  "\xaf\x35\x0b\x51\xf8\x33\x86\x8b"
  "\x9b\xc2\xb3\xbc\xa9\xcf\x41\x13";

static const uint8_t *r5
= (uint8_t *)
  "\xaa\x3b\x47\x49\xd5\x5b\x9d\xaf"
  "\x1e\x5b\x00\x28\x88\x26\xc4\x67"
  "\x27\x4c\xe3\xeb\xbd\xd5\xc1\x7b"
  "\x97\x5e\x09\xd4\xaf\x6c\x67\xcf"
  "\x10\xd0\x87\x20\x2d\xb8\x82\x86"
  "\xe2\xb7\x9f\xce\xea\x3e\xc3\x53"
  "\xef\x54\xfa\xa2\x6e\x21\x9f\x38";

extern int
ecdh_decrypt_x448 (uint8_t *output, const uint8_t *input, 
		   const uint8_t *key_data);

#define N_LIMBS 14

static void
iteration (uint8_t *r, const uint8_t *k_in, int iterations)
{
  uint8_t k0[56];
  uint8_t k1[56];
  int i, j;

  memcpy (k0, k_in, 56);
  memcpy (k1, k0, 56);
  for (i = 0; i < iterations; i++)
    {
      ecdh_decrypt_x448 (r, k1, k0);
      memcpy (k1, k0, 56);
      memcpy (k0, r, 56);
    }
}

int
main (int artc, const char *argv[])
{
  uint8_t r[56];

  ecdh_decrypt_x448 (r, u0, k0);
  if (memcmp (r, r0, 56) != 0)
    return 1;

  ecdh_decrypt_x448 (r, g_x, g_x);
  if (memcmp (r, r3, 56) != 0)
    return 1;

  iteration (r, g_x, 1);
  if (memcmp (r, r3, 56) != 0)
    return 1;

  iteration (r, g_x, 1000);
  if (memcmp (r, r5, 56) != 0)
    return 1;

  return 0;
}
