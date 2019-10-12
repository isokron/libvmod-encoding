#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "cache/cache.h"

#include "vcc_encoding_if.h"
#include "urlcode.h"

int v_matchproto_(vmod_event_f)
vmod_event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	return(0);
}


VCL_STRING
vmod_urlencode(VRT_CTX, VCL_STRING s)
{
	if (s == NULL) {
		return(NULL);
	}
	int destlen = WS_ReserveAll(ctx->ws);
	AN(destlen);

	int n = urlencode(s, ctx->ws->f, destlen-1);

	if (n < 0) {
		WS_Release(ctx->ws, 0);
		return(NULL);
	}

	char *value = ctx->ws->f;
	WS_Release(ctx->ws, n);
	return(value);
}

VCL_STRING
vmod_urldecode(VRT_CTX, VCL_STRING s)
{
	if (s == NULL) {
		return(NULL);
	}
	int destlen = WS_ReserveAll(ctx->ws);
	AN(destlen);

	int n = urldecode(s, ctx->ws->f, destlen-1);

	if (n < 0) {
		WS_Release(ctx->ws, 0);
		return(NULL);
	}

	char *value = ctx->ws->f;
	WS_Release(ctx->ws, n);
	return(value);
}

VCL_STRING
vmod_b64encode(VRT_CTX, VCL_STRING s)
{
	(void)ctx;
	return(NULL);
}

VCL_STRING
vmod_b64decode(VRT_CTX, VCL_STRING s)
{
	(void)ctx;
	return(NULL);
}
