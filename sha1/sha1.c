#include "sha1.h"

#define H0_INIT 0x67452301
#define H1_INIT 0xEFCDAB89
#define H2_INIT 0x98BADCFE
#define H3_INIT 0x10325476
#define H4_INIT 0xC3D2E1F0

/* Rotate left. */
static inline uint rol(uint value, int shift)
{
  if ((shift &= 31) == 0)
    return value;

  return ((value << shift) | (value >> (32 - shift)));
}

/* Endian convert. */
static inline uint econv(uint value)
{
  uint tmp = (value << 16) | (value >> 16);

  return (((tmp & 0xFF00FF00) >> 8) | ((tmp & 0x00FF00FF) << 8));
}

void sha1_init(struct sha1_ctx *ctx)
{
  ctx->h0 = H0_INIT;
  ctx->h1 = H1_INIT;
  ctx->h2 = H2_INIT;
  ctx->h3 = H3_INIT;
  ctx->h4 = H4_INIT;
  ctx->last_data_idx = 0;
  ctx->data_size = 0;
}

static void sha1_block(struct sha1_ctx *ctx)
{
  uint a = ctx->h0;
  uint b = ctx->h1;
  uint c = ctx->h2;
  uint d = ctx->h3;
  uint e = ctx->h4;
  uint w[80];

  for (uint i = 0; i < 16; ++i)
    w[i] = econv(((uint*) ctx->last_data)[i]);

  for (uint i = 16; i < 80; ++i)
    w[i] = rol((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i-16]), 1);

  for (uint i = 0; i < 20; ++i)
  {
    uint f;
    uint k;
    uint tmp;

    f = (b & c) | ((~b) & d);
    k = 0x5A827999;

    tmp = rol(a, 5) + f + e + k + w[i];
    e = d;
    d = c;
    c = rol(b, 30);
    b = a;
    a = tmp;
  }

  for (uint i = 20; i < 40; ++i)
  {
    uint f;
    uint k;
    uint tmp;

    f = b ^ c ^ d;
    k = 0x6ED9EBA1;

    tmp = rol(a, 5) + f + e + k + w[i];
    e = d;
    d = c;
    c = rol(b, 30);
    b = a;
    a = tmp;
  }

  for (uint i = 40; i < 60; ++i)
  {
    uint f;
    uint k;
    uint tmp;

    f = (b & c) | (b & d) | (c & d);
    k = 0x8F1BBCDC;

    tmp = rol(a, 5) + f + e + k + w[i];
    e = d;
    d = c;
    c = rol(b, 30);
    b = a;
    a = tmp;
  }

  for (uint i = 60; i < 80; ++i)
  {
    uint f;
    uint k;
    uint tmp;

    f = b ^ c ^ d;
    k = 0xCA62C1D6;

    tmp = rol(a, 5) + f + e + k + w[i];
    e = d;
    d = c;
    c = rol(b, 30);
    b = a;
    a = tmp;
  }

  ctx->h0 += a;
  ctx->h1 += b;
  ctx->h2 += c;
  ctx->h3 += d;
  ctx->h4 += e;
}

void sha1_update(struct sha1_ctx *ctx, void *data, uint size)
{
  byte *bdata = data;

  ctx->data_size += size * 8;

  while (size)
  {
    while (ctx->last_data_idx < 16 * sizeof (uint) && size)
    {
      ctx->last_data[ctx->last_data_idx] = *bdata;
      ++bdata;
      --size;
      ++ctx->last_data_idx;
    }

    if (ctx->last_data_idx == 16 * sizeof (uint))
    {
      sha1_block(ctx);
      ctx->last_data_idx = 0;
    }
  }
}

void sha1_final(struct sha1_ctx *ctx, byte res[SHA1_HASH_SIZE])
{
  /* Append the bit '1'. */
  ctx->last_data[ctx->last_data_idx++] = 0x80;
  if (ctx->last_data_idx == SHA1_CHUNK_SIZE)
    sha1_block(ctx);

  /* Skip current block if there is not enough place for the size. */
  if (ctx->last_data_idx > SHA1_CHUNK_SIZE - sizeof (uint))
  {
    while (ctx->last_data_idx < SHA1_CHUNK_SIZE)
      ctx->last_data[ctx->last_data_idx++] = 0;
    sha1_block(ctx);
    ctx->last_data_idx = 0;
  }

  /* Add zeroes until there is just enough place for the size. */
  while (ctx->last_data_idx < (SHA1_CHUNK_SIZE - sizeof (ctx->data_size)))
    ctx->last_data[ctx->last_data_idx++] = 0;

  for (uint i = sizeof (ctx->data_size); i > 0; --i)
    ctx->last_data[ctx->last_data_idx++] = ((byte*) &ctx->data_size)[i - 1];

  sha1_block(ctx);

  ((uint*) res)[0] = econv(ctx->h0);
  ((uint*) res)[1] = econv(ctx->h1);
  ((uint*) res)[2] = econv(ctx->h2);
  ((uint*) res)[3] = econv(ctx->h3);
  ((uint*) res)[4] = econv(ctx->h4);
}

void sha1(void *data, uint len, byte res[SHA1_HASH_SIZE])
{
  struct sha1_ctx ctx;

  sha1_init(&ctx);
  sha1_update(&ctx, data, len);
  sha1_final(&ctx, res);
}
