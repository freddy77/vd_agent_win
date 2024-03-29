NULL =
EXTRA_DIST = mingw-spice-vdagent.spec
MAINTAINERCLEANFILES = mingw-spice-vdagent.spec

DIST_SUBDIRS = spice-protocol

AM_CPPFLAGS = 				\
	-DUNICODE 			\
	-D_UNICODE			\
	-DOLDMSVCRT			\
	-I$(top_srcdir)/common		\
	-I$(top_srcdir)/spice-protocol	\
	$(NULL)

AM_LDFLAGS = -static
if !ENABLE_DEBUG
AM_LDFLAGS += -s
endif

# -lversion is needed for the GetFileVersion* API which is used by vdlog.cpp
LIBS = -lversion

bin_PROGRAMS = vdagent vdservice

vdagent_LDADD = $(LIBPNG_LIBS) $(ZLIB_LIBS) -lwtsapi32 -lgdi32 vdagent_rc.$(OBJEXT)
vdagent_CXXFLAGS = $(AM_CXXFLAGS) $(LIBPNG_CFLAGS)
vdagent_LDFLAGS = $(AM_LDFLAGS) -Wl,--subsystem,windows
vdagent_SOURCES =			\
	common/vdcommon.cpp             \
	common/vdcommon.h		\
	common/vdlog.cpp		\
	common/vdlog.h			\
	vdagent/display_configuration.cpp \
	vdagent/display_configuration.h \
	vdagent/desktop_layout.cpp	\
	vdagent/desktop_layout.h	\
	vdagent/display_setting.cpp	\
	vdagent/display_setting.h	\
	vdagent/file_xfer.cpp		\
	vdagent/file_xfer.h		\
	vdagent/vdagent.cpp		\
	vdagent/as_user.cpp		\
	vdagent/as_user.h		\
	vdagent/image.cpp		\
	vdagent/image.h			\
	vdagent/imagepng.cpp		\
	vdagent/imagepng.h		\
	$(NULL)

vdagent_rc.$(OBJEXT): vdagent/vdagent.rc
	$(AM_V_GEN)$(WINDRES) -I $(top_builddir)/common -i $< -o $@

MAINTAINERCLEANFILES += vdagent_rc.$(OBJEXT)

vdservice_LDADD = -lwtsapi32 vdservice_rc.$(OBJEXT)
vdservice_SOURCES =			\
	common/stdint.h			\
	common/vdcommon.cpp             \
	common/vdcommon.h		\
	common/vdlog.cpp		\
	common/vdlog.h			\
	vdservice/vdservice.cpp		\
	$(NULL)

vdservice_rc.$(OBJEXT): vdservice/vdservice.rc
	$(AM_V_GEN)$(WINDRES) -I $(top_builddir)/common -i $< -o $@

MAINTAINERCLEANFILES += vdservice_rc.$(OBJEXT)

check_PROGRAMS = imagetest

imagetest_LDADD = $(LIBPNG_LIBS) $(ZLIB_LIBS) -lwtsapi32 -lgdi32
imagetest_CXXFLAGS = $(AM_CXXFLAGS) $(LIBPNG_CFLAGS)
imagetest_LDFLAGS = $(AM_LDFLAGS) -Wl,--subsystem,console
imagetest_SOURCES =			\
	common/vdcommon.cpp             \
	common/vdcommon.h		\
	common/vdlog.cpp		\
	common/vdlog.h			\
	vdagent/imagetest.cpp		\
	vdagent/image.cpp		\
	vdagent/image.h			\
	vdagent/imagepng.cpp		\
	vdagent/imagepng.h		\
	$(NULL)

TESTS = test-png
EXTRA_DIST += test-png

check_PROGRAMS += test-log-win
TESTS += test-log
EXTRA_DIST += test-log

test_log_win_LDFLAGS = $(AM_LDFLAGS) -Wl,--subsystem,console
test_log_win_SOURCES =			\
	common/vdcommon.cpp             \
	common/vdcommon.h		\
	common/vdlog.cpp		\
	common/vdlog.h			\
	common/test-log.cpp		\
	$(NULL)

MANUFACTURER = The Spice Project
PRODUCT = Spice agent

EXTRA_DIST += spice-vdagent.wxs.in
CONFIG_STATUS_DEPENDENCIES = spice-vdagent.wxs.in

spice-vdagent-$(WIXL_ARCH)-$(VERSION)$(BUILDID).msi: spice-vdagent.wxs all
	$(AM_V_GEN)DESTDIR=`mktemp -d`&&				\
	make -C $(top_builddir) install DESTDIR=$$DESTDIR >/dev/null &&	\
	  MANUFACTURER="$(MANUFACTURER)" PRODUCT="$(PRODUCT)"           \
	  INSTALLDIRECTORY="$(PRODUCT)" BINDIRECTORY=bin                \
	  wixl -D SourceDir=$(prefix)                                   \
	  -D DESTDIR=$$DESTDIR$(prefix)					\
	  --arch $(WIXL_ARCH)  -o $@ $<

msi: spice-vdagent-$(WIXL_ARCH)-$(VERSION)$(BUILDID).msi

CLEANFILES = spice-vdagent-$(WIXL_ARCH)-$(VERSION)$(BUILDID).msi

.PHONY: msi

EXTRA_DIST +=			\
	$(top_srcdir)/.version	\
	config.h		\
	common/version.rc	\
	tests/clipboard.py	\
	vdagent.sln		\
	vdagent/resource.h	\
	vdagent/vdagent.rc	\
	vdagent/vdagent.vcproj	\
	vdservice/resource.h	\
	vdservice/vdservice.rc	\
	vdservice/vdservice.vcproj

# see git-version-gen
dist-hook:
	echo $(VERSION) > $(distdir)/.tarball-version

BUILT_SOURCES = $(top_srcdir)/.version
$(top_srcdir)/.version:
	echo $(VERSION) > $@-t && mv $@-t $@

MAINTAINERCLEANFILES += $(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL)

-include $(top_srcdir)/git.mk
