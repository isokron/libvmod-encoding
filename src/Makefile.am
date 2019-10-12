AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_encoding.la

libvmod_encoding_la_SOURCES = \
	vmod_encoding.c \
	urlcode.c \
	urlcode.h \
	base64.h \
	base64.c

nodist_libvmod_encoding_la_SOURCES = \
	vcc_encoding_if.c \
	vcc_encoding_if.h

dist_man_MANS = vmod_encoding.3

@BUILD_VMOD_ENCODING@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_encoding="$(VMOD_ENCODING)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_encoding.vcc \
	$(VMOD_TESTS)
