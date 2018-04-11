TEMPLATE	= app
TARGET		= visupropa

CONFIG		+= qt opengl warn_on release
CONFIG		-= dlopen_opengl

#The following line was commented out by qt3to4
#REQUIRES        = opengl

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3		=	visupropawinui.ui

HEADERS		=	visupropawin.h

SOURCES		=	main.cpp \
			visupropawin.cpp

INCLUDEPATH += ../librubic

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
#The following line was inserted by qt3to4
CONFIG += uic3

