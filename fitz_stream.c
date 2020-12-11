#include <mupdf/fitz.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "fitz_stream.h"

extern int readBytes(int key, unsigned char *rp, size_t n);

fz_reader_state *fz_new_reader_state(fz_context *ctx, int key)
{
	fz_reader_state *state = fz_malloc_struct(ctx, fz_reader_state);
	state->key = key;
	return state;
}

int next_reader(fz_context *ctx, fz_stream *stm, size_t n)
{
	fz_reader_state *state = stm->state;

	/* n is only a hint, that we can safely ignore */
	n = readBytes(state->key, state->buffer, n);

	if (n < 0)
		fz_throw(ctx, FZ_ERROR_GENERIC, "read error: %s", strerror(errno));

	stm->rp = state->buffer;
	stm->wp = state->buffer + n;
	stm->pos += (int64_t)n;

	if (n == 0)
		return EOF;

	return *stm->rp++;
}
