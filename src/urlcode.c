/*
 * URL encoding and decoding.
 *
 * Author: Lasse Karstensen <lasse@isokron.no>
 * Based on urlcode.c from libvmod-urlcode by Rogier "DocWilco" Mulhuijzen.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char hexchars[] = "0123456789ABCDEF";

#define visalpha(c) \
	((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define visalnum(c) \
	((c >= '0' && c <= '9') || visalpha(c))

static inline int
hex_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (-1);
}


uint32_t
urlencode(char *src, char *dest, int destlen)
{
	char *s = src;  // reading ptr
	char *p = dest;  // writing ptr
	char *end = dest+destlen;

	if (src == NULL || *src == '\0' || dest == NULL || destlen == 0) {
		return(-1);
	}

	while (*s != '\0' && p < end) {
		// RFC3986 2.3
		if (visalnum((int)*s)
			|| *p == '-'
			|| *p == '.'
			|| *p ==  '_'
			|| *p ==  '~') {
			*p++ = *s++;
		} else if (p + 4 >= end) { // % hex hex NULL
			p = end; // not enough space
		} else {
			*p++ = '%';
			unsigned char foo = *p;
			*p++ = hexchars[foo >> 4];
			*p++ = hexchars[*s & 15];
			s++;
		}
	}
	if (p < end)
		*p = '\0';
	p++;
	if (p >= end) {
		memset(dest, '\0', destlen);
		return (-1);
	}
	return (p-dest);

}


uint32_t
urldecode(char * src, char *dest, int destlen)
{
	char *s = src;
	char *p = dest;
	char *end = dest+destlen;

	int percent = 0;
	int highbits, lowbits = 0;

	if (src == NULL || *src == '\0' || dest == NULL || destlen == 0) {
		return(-1);
	}

	while (*s != '\0') {
		if (p >= dest+destlen) {
			break;
		}

		if (percent == 0) {
			switch(*s) {
			case '%':
				percent = 1;
				s++;
				break;
			case '+':
				*p++ = ' ';
				s++;
				break;
			default:
				*p++ = *s++;
				break;
			}
		} else if (percent == 1) {
			highbits = hex_to_int(*s++);
			if (highbits < 0)
				p = end;
			percent = 2;
		} else if (percent == 2) {
			lowbits = hex_to_int(*s++);
			if (lowbits < 0)
				p = end;
			*p++ = (char) ((highbits << 4) | lowbits);
			percent = 0;
		}
	}

	if (p < dest+destlen)
		*p = '\0';
	p++;

	if (p > end) {
		/* don't leave a partial result in dest. */
		memset(dest, '\0', destlen);
		return (-1);
	}
	return(p-dest);
}


#ifndef NDEBUG
void test_enc() {
	char src[] = "foo and bar with a / in it";
	int len;
	int buflen = 100;
	char dst[buflen];
	memset(dst, '\0', buflen);
	len = urlencode((char *)&src, (char *)&dst, buflen);
	printf("len=%i dst=\"%s\"\n", len, dst);
}

void test_dec() {
	int len;
	int buflen = 100;
	// char src[] = "foo%20bar";
//	char src[] = "a-umlaut-%c3%a4";
	char src[] = "http://example.org/\"%<>\\^`{|}";
	char dst[buflen];
	memset(dst, '\0', buflen);
	len = urldecode((char *)&src, (char *)&dst, buflen);
	printf("len=%i dst=\"%s\"\n", len, dst);
}

int main() {
	test_enc();
}
#endif
