/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PICKER_MACHINE
#define QWT_PICKER_MACHINE 1

#include <q3valuelist.h>
#include "qwt_array.h"
//Added by qt3to4:
#include <QEvent>

class QEvent;
class QwtEventPattern;

/*!
  \brief A state machine for QwtPicker selections

  QwtPickerMachine accepts key and mouse events and translates them
  into selection commands. 

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPickerMachine
{
public:
    enum Command
    {
        Begin,
        Append,
        Move,
        End
    };

    virtual ~QwtPickerMachine();

    //! Transition
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *) = 0;
    void reset(); 

protected:
    QwtPickerMachine();

    int state() const;
    void setState(int);

private:
    int d_state;
};

/*!
  \brief A state machine for point selections

  Pressing QwtEventPattern::MouseSelect1 or 
  QwtEventPattern::KeySelect1 selects a point.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/
class QWT_EXPORT QwtPickerClickPointMachine: public QwtPickerMachine
{
public:
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *);
};

/*!
  \brief A state machine for point selections

  Pressing QwtEventPattern::MouseSelect1 or QwtEventPattern::KeySelect1 
  starts the selection, releasing QwtEventPattern::MouseSelect1 or 
  a second press of QwtEventPattern::KeySelect1 terminates it.
*/
class QWT_EXPORT QwtPickerDragPointMachine: public QwtPickerMachine
{
public:
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *);
};

/*!
  \brief A state machine for rectangle selections

  Pressing QwtEventPattern::MouseSelect1 starts
  the selection, releasing it selects the first point. Pressing it
  again selects the second point and terminates the selection.
  Pressing QwtEventPattern::KeySelect1 also starts the 
  selection, a second press selects the first point. A third one selects 
  the second point and terminates the selection. 

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPickerClickRectMachine: public QwtPickerMachine
{
public:
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *);
};

/*!
  \brief A state machine for rectangle selections

  Pressing QwtEventPattern::MouseSelect1 selects
  the first point, releasing it the second point.
  Pressing QwtEventPattern::KeySelect1 also selects the 
  first point, a second press selects the second point and terminates 
  the selection.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPickerDragRectMachine: public QwtPickerMachine
{
public:
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *);
};

/*!
  \brief A state machine for polygon selections

  Pressing QwtEventPattern::MouseSelect1 or QwtEventPattern::KeySelect1 
  starts the selection and selects the first point, or appends a point. 
  Pressing QwtEventPattern::MouseSelect2 or QwtEventPattern::KeySelect2 
  appends the last point and terminates the selection.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPickerPolygonMachine: public QwtPickerMachine
{
public:
    virtual Q3ValueList<Command> transition(
        const QwtEventPattern &, const QEvent *);
};

#endif
