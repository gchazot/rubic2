TEMPLATE = app
TARGET = lvectqueuetest

CONFIG += warn_on debug qt threads

HEADERS = 

SOURCES = lvectqueuetest.cpp

FORMS =

INCLUDEPATH = ../../librubic

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  LIBS += -L../../librubic/ -lrubic
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
