TEMPLATE = app
TARGET = jacobi

CONFIG += warn_on release
CONFIG -= qt

HEADERS = 

SOURCES = jacobi.cpp

FORMS =

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
#The following line was inserted by qt3to4
QT +=  opengl qt3support 
