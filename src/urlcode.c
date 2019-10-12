/*
 * URL encoding and decoding.
 *
 * Author: Lasse Karstensen <lasse@isokron.no>
 * Based on urlcode.c from libvmod-urlcode by Rogier "DocWilco" Mulhuijzen.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
static char hexchars[] = "0123456789ABCDEF";
*/
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


/*
uint32_t
urlencode(const char *src, char *dst, int destlen)
{
	char *b, *e;
	unsigned u;
	va_list ap;

	u = WS_Reserve(ctx->ws, 0);
	e = b = ctx->ws->f;
	e += u;
	va_start(ap, str);
	while (b < e && str != vrt_magic_string_end) {
		while (b < e && str && *str) {
			if (visalnum((int) *str) || *str == '-' || *str == '.'
			    || *str ==  '_' || *str ==  '~') { // RFC3986 2.3 
				*b++ = *str++;
			} else if (b + 4 >= e) { // % hex hex NULL
				b = e; // not enough space 
			} else {
				*b++ = '%';
				unsigned char foo = *str;
				*b++ = hexchars[foo >> 4];
				*b++ = hexchars[*str & 15];
				str++;
			}
		}
		str = va_arg(ap, const char *);
	}
	if (b < e)
		*b = '\0';
	b++;
	if (b > e) {
		WS_Release(ctx->ws, 0);
		return (NULL);
	} else {
		e = b;
		b = ctx->ws->f;
		WS_Release(ctx->ws, e - b);
		return (b);
	}
}
*/

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
int main() {
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
#endif
