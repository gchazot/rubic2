TEMPLATE = lib
TARGET = rubic
VERSION = 1.0

CONFIG += warn_on qt thread opengl release

HEADERS = 	constants.h \
		dvect.h \
		lvect.h \
		paramitem.h \
		paramgroup.h \
		params.h \
		rubicfiles.h \
		dmatrix_utils.h \
		dmatrix.h \
		rubicimage.h \
		rubicparams.h \
		rubicparams_defaults.h \
		rubicparams_names.h \
		logutil.h \
		gldefbox.h \
		dvect24.h \
		rubicresults_point.h \
		rubicresults.h \
		lvectqueueelt.h \
		lvectuniquequeue.h \
		visupropagl.h

SOURCES =	dvect.cpp \
		lvect.cpp \
		paramitem.cpp \
		paramgroup.cpp \
		params.cpp \
		rubicfiles.cpp \
		dmatrix.cpp \
		dmatrix_utils.cpp \
		rubicimage.cpp \
		rubicparams.cpp \
		logutil.cpp \
		gldefbox.cpp \
		dvect24.cpp \
		rubicresults_point.cpp \
		rubicresults.cpp \
		interpolation.cpp \
		lvectqueueelt.cpp \
		lvectuniquequeue.cpp \
		visupropagl.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  # Pour que les fonctions inline soient tout de même
  # stockées dans la librairie
  QMAKE_CXXFLAGS += -fkeep-inline-functions

  target.path=$$(HOME)/programs/lib
  INSTALLS += target
}

win32-g++ {
  # Pour que les fonctions inline soient tout de même
  # stockées dans la librairie
  QMAKE_CXXFLAGS += -fkeep-inline-functions
}

#The following line was inserted by qt3to4
QT +=  opengl qt3support 
