#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ossl.h"

int decode_base64_to_6bit(int c)
{
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	} else if (c >= 'a' && c <= 'z') {
		return c - 'a' + 26;
	} else if (c >= '0' && c <= '9') {
		return c - '0' + 52;
	} else if (c == '+') {
		return 62;
	} else if (c == '/') {
		return 63;
	} else if (c == '=') {
		return 0;
	} else {
		fprintf(stderr, "Error: unknown data %d\n", c);
		exit(1);
	}
}

void decode_base64(char *dst, char *src)
{
	unsigned int o[4];
	char *p = dst;
	size_t i;

	for (i = 0; src[i]; i += 4) {
		o[0] = decode_base64_to_6bit(src[i]);
		o[1] = decode_base64_to_6bit(src[i + 1]);
		o[2] = decode_base64_to_6bit(src[i + 2]);
		o[3] = decode_base64_to_6bit(src[i + 3]);

		*p++ = (o[0] << 2) | ((o[1] & 0x30) >> 4);
		*p++ = ((o[1] & 0xf) << 4) | ((o[2] & 0x3c) >> 2);
		*p++ = ((o[2] & 0x3) << 6) | (o[3] & 0x3f);
	}

	*p = '\0';
}
