TEMPLATE = app
CONFIG += qt warn_on release

TARGET = rubicalc

SOURCES =	globals.cpp \
		main.cpp \
		correlation.cpp \
		scanning.cpp \
		scanningthread.cpp \
		voxerncalcthread.cpp \
		propagation.cpp \
		calculvoxern.cpp

HEADERS =	globals.h \
		fonctions.h \
		scanningthread.h \
		voxerncalcthread.h

INCLUDEPATH = ../librubic

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
