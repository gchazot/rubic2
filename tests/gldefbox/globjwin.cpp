/****************************************************************************
** $Id: globjwin.cpp,v 1.1 2006/05/09 08:25:07 gchazot Exp $
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <q3frame.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <qapplication.h>
#include <qnamespace.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include "globjwin.h"
#include "gldefbox.h"

#include "dmatrix.h"
#include "dvect.h"
#include "lvect.h"

using namespace Qt;

GLObjectWindow::GLObjectWindow( QWidget* parent, const char* name )
    : QWidget( parent, name )
{

    // Create a menu
    Q3PopupMenu *file = new Q3PopupMenu( this );
    file->insertItem( "Exit",  qApp, SLOT(quit()), CTRL+Key_Q );

    // Create a menu bar
    QMenuBar *m = new QMenuBar( this );
    m->setSeparator( QMenuBar::InWindowsStyle );
    m->insertItem("&File", file );

    // Create a nice frame to put around the OpenGL widget
    Q3Frame* f = new Q3Frame( this, "frame" );
    f->setFrameStyle( Q3Frame::Sunken | Q3Frame::Panel );
    f->setLineWidth( 2 );
    Q3Frame* f2 = new Q3Frame( this, "frame2" );
    f2->setFrameStyle( Q3Frame::Sunken | Q3Frame::Panel );
    f2->setLineWidth( 2 );

    // Create our OpenGL widget
    GLDefBox* c = new GLDefBox( f, "glbox");
    GLDefBox* c2 = new GLDefBox( f2, "glbox2");

    // Create the three sliders; one for each rotation axis
    QSlider* x = new QSlider ( 0, 360, 60, 0, Vertical, this, "xsl" );
    x->setTickmarks( QSlider::Left );
    QObject::connect( x, SIGNAL(valueChanged(int)),c,SLOT(setXRotation(int)) );
    QObject::connect( x, SIGNAL(valueChanged(int)),c2,SLOT(setXRotation(int)) );
    QObject::connect( c, SIGNAL(xRotationChanged(int)), x, SLOT(setValue(int)) );
    QObject::connect( c2, SIGNAL(xRotationChanged(int)), x, SLOT(setValue(int)) );

    QSlider* y = new QSlider ( 0, 360, 60, 0, Vertical, this, "ysl" );
    y->setTickmarks( QSlider::Left );
    QObject::connect( y, SIGNAL(valueChanged(int)),c,SLOT(setYRotation(int)) );
    QObject::connect( y, SIGNAL(valueChanged(int)),c2,SLOT(setYRotation(int)) );
    QObject::connect( c, SIGNAL(yRotationChanged(int)), y, SLOT(setValue(int)) );
    QObject::connect( c2, SIGNAL(yRotationChanged(int)), y, SLOT(setValue(int)) );

    QSlider* z = new QSlider ( 0, 360, 60, 0, Vertical, this, "zsl" );
    z->setTickmarks( QSlider::Left );
    QObject::connect( z, SIGNAL(valueChanged(int)),c,SLOT(setZRotation(int)) );
    QObject::connect( z, SIGNAL(valueChanged(int)),c2,SLOT(setZRotation(int)) );

    // Now that we have all the widgets, put them into a nice layout

    // Put the sliders on top of each other
    Q3VBoxLayout* vlayout = new Q3VBoxLayout( 20, "vlayout");
    vlayout->addWidget( x );
    vlayout->addWidget( y );
    vlayout->addWidget( z );

    // Put the GL widget inside the frame
    Q3HBoxLayout* flayout = new Q3HBoxLayout( f, 2, 2, "flayout");
    flayout->addWidget( c, 1 );
    
    Q3HBoxLayout* f2layout = new Q3HBoxLayout( f2, 2, 2, "f2layout");
    f2layout->addWidget( c2, 1 );

    // Top level layout, puts the sliders to the left of the frame/GL widget
    Q3HBoxLayout* hlayout = new Q3HBoxLayout( this, 20, 20, "hlayout");
    hlayout->setMenuBar( m );
    hlayout->addLayout( vlayout );
    hlayout->addWidget( f, 1 );
    hlayout->addWidget( f2, 1 );
    
    
    c2->setObjSize(LVect(100,50,20));
    
    DMatrix def = DMatrix::Identite();
    DVect trans(0.2,0.2,0.2);
    
    def[0][0] = 0.5;

    c2->setDefMatrix(def);
    c2->setTransVect(trans);
 
}
