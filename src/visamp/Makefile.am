AM_CXXFLAGS = -Wall -g

MOC_GEN_SRCS = \
	VisAmpWid.moc.cc \
	MyRecognizer.moc.cc

bin_PROGRAMS = visamp

visamp_DEPENDENCIES = $(top_builddir)/src/libimrec/libimrec.la

visamp_CXXFLAGS = -I$(top_srcdir)/src/libimrec @QtGui_CFLAGS@

visamp_LDFLAGS = -L$(top_builddir)/src/libimrec -limrec @QtGui_LIBS@

visamp_SOURCES = \
	visamp.cc \
	VisAmpWid.cc \
	MyImageButtons.cc \
	MySlider.cc \
	MyRecognizer.cc \
	$(MOC_GEN_SRCS)

noinst_HEADERS = \
	VisAmpWid.hh \
	MyImageButtons.hh \
	MySlider.hh \
	MyRecognizer.hh

if LINUX
visamp_CXXFLAGS += -DLINUX_GRABBER
endif

if IRIX
visamp_CXXFLAGS += -DSGI_GRABBER
visamp_LDFLAGS += -lvl -ldmedia
endif

#  Rules for moc
#
$(MOC_GEN_SRCS): %.moc.cc: %.hh
	$(MOC) -o $@ $<

MOSTLYCLEANFILES = $(MOC_GEN_SRCS)

SUFFIXES: .moc.cc
