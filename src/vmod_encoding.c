#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "cache/cache.h"

#include "vcc_encoding_if.h"

int v_matchproto_(vmod_event_f)
vmod_event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	return(0);
}


VCL_STRING
vmod_urlencode(VRT_CTX, VCL_STRING s)
{
	(void)ctx;
	return(NULL);
}

VCL_STRING
vmod_urldecode(VRT_CTX, VCL_STRING s)
{
	(void)ctx;
	return(NULL);
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
