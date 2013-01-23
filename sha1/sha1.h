#ifndef SHA1_H_
# define SHA1_H_

typedef unsigned char byte;
typedef unsigned int  uint;

#define SHA1_HASH_SIZE  20
#define SHA1_CHUNK_SIZE 64

struct sha1_ctx
{
  uint h0;
  uint h1;
  uint h2;
  uint h3;
  uint h4;
  byte last_data[SHA1_CHUNK_SIZE];
  uint last_data_idx;
  uint data_size;
};

void sha1_init(struct sha1_ctx *ctx);
void sha1_update(struct sha1_ctx *ctx, void *data, uint size);
void sha1_final(struct sha1_ctx *ctx, byte res[SHA1_HASH_SIZE]);
void sha1(void *data, uint len, byte res[SHA1_HASH_SIZE]);

#endif /* !SHA1_H_ */
