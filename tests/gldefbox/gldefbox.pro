TEMPLATE	= app
TARGET		= gldefbox

CONFIG		+= qt opengl warn_on release
CONFIG		-= dlopen_opengl

DEPENDPATH	= 

#The following line was commented out by qt3to4
#REQUIRES        = opengl



HEADERS		= globjwin.h
SOURCES		= globjwin.cpp \
		  main.cpp
		  
INCLUDEPATH += ../../librubic

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += -L../../librubic -lrubic
}

win32-g++ {
	debug{
		LIBS += -L../../librubic/debug
	}
	
	release{
		LIBS += -L../../librubic/release
	}
	
	LIBS += -lrubic
}

#The following line was inserted by qt3to4
QT +=  opengl qt3support 
