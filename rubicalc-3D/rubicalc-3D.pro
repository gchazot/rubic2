TEMPLATE = app
CONFIG += qt warn_on release

TARGET = rubicalc-3D

SOURCES =	../rubicalc/globals.cpp \
		../rubicalc/main.cpp \
		../rubicalc/correlation.cpp \
		../rubicalc/scanning.cpp \
		../rubicalc/scanningthread.cpp \
		../rubicalc/voxerncalcthread.cpp \
		../rubicalc/propagation.cpp \
		calculvoxern-3D.cpp

HEADERS =	../rubicalc/globals.h \
		../rubicalc/fonctions.h \
		../rubicalc/scanningthread.h \
		../rubicalc/voxerncalcthread.h

INCLUDEPATH = ../librubic ../rubicalc

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += -L../librubic -lrubic

  target.path=$$(HOME)/programs/bin
  INSTALLS += target
}

win32-g++ {
	debug{
		LIBS += -L../librubic/debug
	}
	
	release{
		LIBS += -L../librubic/release
	}
	
	LIBS += -lrubic
}

#The following line was inserted by qt3to4
QT +=  opengl qt3support 
