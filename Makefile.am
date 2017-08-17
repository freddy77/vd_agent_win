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

# binutils does not take into account entry point when
# -pie is used so we need to provide it manually
ENTRY_PREFIX := $(if $(filter x86_64,$(host_cpu)),,_)

# --dynamicbase to enable ASLR protection
# --nxcompat is to enable NX protection
# --pie as --dynamicbase requires relocations
LDFLAGS_SECURITY_COMMON = \
	-Wl,--dynamicbase -Wl,-pie \
	-Wl,--nxcompat \
	$(NULL)
LDFLAGS_SECURITY_GUI = $(LDFLAGS_SECURITY_COMMON) \
	-Wl,-e,$(ENTRY_PREFIX)WinMainCRTStartup \
	-mwindows \
	$(NULL)
LDFLAGS_SECURITY_CUI = $(LDFLAGS_SECURITY_COMMON) \
	-Wl,-e,$(ENTRY_PREFIX)mainCRTStartup \
	-mconsole \
	$(NULL)

bin_PROGRAMS = vdagent vdservice

vdagent_LDADD = $(LIBPNG_LIBS) $(ZLIB_LIBS) -lwtsapi32 -lgdi32 vdagent_rc.$(OBJEXT)
vdagent_CXXFLAGS = $(AM_CXXFLAGS) $(LIBPNG_CFLAGS)
vdagent_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_SECURITY_GUI)
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
vdservice_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_SECURITY_CUI)
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
imagetest_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_SECURITY_CUI)
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

test_log_win_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS_SECURITY_CUI)
test_log_win_SOURCES =			\
	common/vdcommon.cpp             \
	common/vdcommon.h		\
	common/vdlog.cpp		\
	common/vdlog.h			\
	common/test-log.cpp		\
	$(NULL)

deps.txt:
	$(AM_V_GEN)rpm -qa | grep $(host_os) | sort | unix2dos > $@

MANUFACTURER = The Spice Project

EXTRA_DIST += spice-vdagent.wxs.in
CONFIG_STATUS_DEPENDENCIES = spice-vdagent.wxs.in

spice-vdagent-$(WIXL_ARCH)-$(VERSION)$(BUILDID).msi: spice-vdagent.wxs deps.txt all
	$(AM_V_GEN)DESTDIR=`mktemp -d`&&				\
	make -C $(top_builddir) install DESTDIR=$$DESTDIR >/dev/null &&	\
	MANUFACTURER="$(MANUFACTURER)" wixl -D SourceDir=$(prefix)	\
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
