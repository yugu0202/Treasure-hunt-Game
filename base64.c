#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

int DecodeBase64_6(int c)
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

void DecodeBase64(char *dst, char *src)
{
	unsigned int o[4];
	char *p = dst;
	size_t i;

	for (i = 0; src[i]; i += 4) {
		o[0] = DecodeBase64_6(src[i]);
		o[1] = DecodeBase64_6(src[i + 1]);
		o[2] = DecodeBase64_6(src[i + 2]);
		o[3] = DecodeBase64_6(src[i + 3]);

		*p++ = (o[0] << 2) | ((o[1] & 0x30) >> 4);
		*p++ = ((o[1] & 0xf) << 4) | ((o[2] & 0x3c) >> 2);
		*p++ = ((o[2] & 0x3) << 6) | (o[3] & 0x3f);
	}

	*p = '\0';
}

void EncodeBase64(char *dst, char *src)
{
	size_t i;
	unsigned int o[4];
	unsigned int x;
	const char table[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	size_t len = strlen(src);
	size_t mod = len % 3;
	size_t mod_len = len - mod;
	char *p = dst;

	for (i = 0; i < mod_len; i += 3) {
		x = ((unsigned int) src[i] << 16)
			+ ((unsigned int) src[i + 1] << 8)
			+ (unsigned int) src[i + 2];
		o[0] = (x >> 18) & 0x3f;
		o[1] = (x >> 12) & 0x3f;
		o[2] = (x >> 6) & 0x3f;
		o[3] = x & 0x3f;
		*p++ = table[o[0]];
		*p++ = table[o[1]];
		*p++ = table[o[2]];
		*p++ = table[o[3]];
	}

	/* do nothing if mod == 0. */
	if (mod == 1) {
		x = (unsigned int) src[i] << 16;
		o[0] = (x >> 18) & 0x3f;
		o[1] = (x >> 12) & 0x3f;
		*p++ = table[o[0]];
		*p++ = table[o[1]];
		*p++ = '=';
		*p++ = '=';
	} else if (mod == 2) {
		x = ((unsigned int) src[i] << 16) + ((unsigned int) src[i + 1] << 8);
		o[0] = (x >> 18) & 0x3f;
		o[1] = (x >> 12) & 0x3f;
		o[2] = (x >> 6) & 0x3f;
		*p++ = table[o[0]];
		*p++ = table[o[1]];
		*p++ = table[o[2]];
		*p++ = '=';
	}

	*p = '\0';
}

