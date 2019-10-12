=============
vmod-encoding
=============

SYNOPSIS
========

import encoding;

DESCRIPTION
===========

This vmod implements string encoding functions for Varnish VCL.

Currently implemented:
* URL encoding (based off the abandoned libvmod-urlcode)
* base64 (adapted from postgresql)

See `src/vmod_encoding.vcc` for usage documentation.

Main goal: simple and non-complicate use in VCL.

Non-goals:
* comprehensive set of encodings. A bare minimum of what is needed for
semi-complicated to advanced Varnish configurations is enough.


SAMPLE VCL
==========

::
    import encoding;

    sub vcl_recv {
        set req.http.enc = encoding.urlencode(req.http.Referer);
        set req.http.foo = encoding.b64decode(req.http.Authorization);

        if (req.url ~ "/login") {
            return(synth(1302));
        }
    }

    sub vcl_synth {
        if (resp.status == 1302) {
            set resp.status = 302;
            set req.http.x-tmp = "https://" + req.http.host + req.url;
            set resp.http.Location = "https://accounts.google.com/authorize?scope=openid&redirect_uri=" + encoding.urlencode(req.http.x-tmp);
            return(deliver);
        }
    }

The main use case is for cleaner OAuth2 in Varnish.

LICENSE
=======

The combined vmod and documentation is licensed under GNU GPLv2.

Original code in `src/base64.c` is licensed under the PostgreSQL license.
(BSD/MIT similar)

Original code in `src/urlcode.c` is licensed under 2-clause BSD.


See LICENSE file for more information.

AUTHOR
======

Authored by Lasse Karstensen <lasse@isokron.no>.
