AM_CXXFLAGS = -Wall -g

MOC_GEN_SRCS = \
	FastImageDisplayWid.moc.cc \
	Player.moc.cc \
	BlueScreenWid.moc.cc \
	ImageButton.moc.cc

lib_LTLIBRARIES = libimrec.la

libimrec_la_CXXFLAGS = @QtCore_CFLAGS@ @QtGui_CFLAGS@

libimrec_la_LDFLAGS =  \
	-version-info @lt_major@:@lt_revision@:@lt_age@ \
	@QtCore_LIBS@ @QtGui_LIBS@

libimrec_la_SOURCES = \
	FileFrameGrabber.cc \
	FastImageDisplayWid.cc \
	ConversionTables.cc \
	HistFeatureVec.cc \
	Sample.cc \
	Player.cc \
	BlueScreenWid.cc\
	ImageButton.cc \
	$(MOC_GEN_SRCS)

include_HEADERS = \
	BasicFrameGrabber.hh \
	BasicDummyFrameGrabber.hh \
	BasicLinuxFrameGrabber.hh \
	BasicLinuxFrameGrabber.icc \
	BasicLinuxFrameGrabber_v4l2.hh \
	BasicLinuxFrameGrabber_v4l2.icc \
	BasicSGIFrameGrabber.hh \
	BasicSGIFrameGrabber.icc \
	BlueScreen.hh \
	BlueScreen.icc \
	BlueScreenWid.hh \
	BsDistance.hh \
	BsDistance.icc \
	CenterOfGravityFeatureVec.hh \
	CenterOfGravityFeatureVec.icc \
	ConstantColor.hh \
	ConstantColor.icc \
	ConversionTables.hh \
	CheckedIter.hh \
	CheckedIter.icc \
	FastImageDisplayWid.hh \
	FastImageDisplayWid.icc \
	FrameGrabberError.hh \
	FileFrameGrabber.hh \
	FileFrameGrabber.icc \
	HistFeatureVec.hh \
	HistFeatureVec.icc \
	Image.hh \
	Image.icc \
	ImageButton.hh \
	ImageOverlayWid.hh \
	MeanColorFeatureVec.hh \
        MinimumDistance.hh \
	MinimumDistance.icc \
	MyError.hh \
	NearestNeighbour.hh \
	NearestNeighbour.icc \
	PeripheryFeatureVec.hh \
	PeripheryFeatureVec.icc \
	PixelCountFeatureVec.hh \
	PixelCountFeatureVec.icc \
	Player.hh \
	Player.icc \
	RecognizeButton.hh \
	RecognizeButton.icc \
	Recognizer.hh \
	Recognizer.icc \
	RgbColor.hh \
	RgbColor.icc \
	Rgb16BitColor.hh \
	Rgb24BitColor.hh \
	Rgb32BitColor.hh \
	Sample.hh \
	Sample.icc \
	Slider.hh \
	Slider.icc \
	SubImage.hh \
	SubImage.icc \
	SubImageIterator.hh \
	SubImageIterator.icc \
	convertColor.hh \
	types.hh

EXTRA_DIST = \
	BasicLinuxFrameGrabber.cc \
	BasicLinuxFrameGrabber_v4l2.cc \
	BasicSGIFrameGrabber.cc

# System-dependent settings
if LINUX
libimrec_la_CXXFLAGS += -DLINUX_GRABBER
libimrec_la_SOURCES += BasicLinuxFrameGrabber_v4l2.cc
endif

if IRIX
libimrec_la_CXXFLAGS += -DSGI_GRABBER
libimrec_la_SOURCES += BasicSGIFrameGrabber.cc
endif

if DARWIN
endif

#  Rules for moc
#
$(MOC_GEN_SRCS): %.moc.cc: %.hh
	$(MOC) -o $@ $<

MOSTLYCLEANFILES = $(MOC_GEN_SRCS)

SUFFIXES: .moc.cc
