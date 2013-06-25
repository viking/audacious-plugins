PLUGIN = scoring${PLUGIN_SUFFIX}

SRCS = scoring.c

include ../../buildsys.mk
include ../../extra.mk

PACKAGE_NAME = audacious

plugindir := ${plugindir}/${GENERAL_PLUGIN_DIR}

CFLAGS += ${PLUGIN_CFLAGS} -Wall
CPPFLAGS += ${PLUGIN_CPPFLAGS} ${GLIB_CFLAGS} -I../..
LIBS += ${GLIB_LIBS}
