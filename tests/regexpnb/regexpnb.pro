TEMPLATE = app
TARGET = regexpnb

CONFIG += warn_on release qt

HEADERS = 

SOURCES = regexpnb.cpp

FORMS =


unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
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
