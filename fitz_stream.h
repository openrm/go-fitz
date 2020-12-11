#include <mupdf/fitz.h>
#include <stdlib.h>

typedef struct
{
	int key;
	unsigned char buffer[4096];
} fz_reader_state;

int next_reader(fz_context *ctx, fz_stream *stm, size_t n);
fz_reader_state *fz_new_reader_state(fz_context *ctx, int key);
