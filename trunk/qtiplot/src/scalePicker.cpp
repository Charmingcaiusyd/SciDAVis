/***************************************************************************
    File                 : scalePicker.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Hoener zu Siederdissen
    Email                : ion_vasilief@yahoo.fr, thzs@gmx.net
    Description          : Scale picker
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#include "scalePicker.h"
#include <qpainter.h>
#include <qwt_plot.h>
//FIXME: QwtScale is now QwtScaleWidget
#include <qwt_scale_widget.h>

#include <qmessagebox.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QLabel>
#include <QKeyEvent>
#include <QEvent>

//FIXME: All functionality disabled for now (needs port to Qwt5)

ScalePicker::ScalePicker(QwtPlot *plot):
    QObject(plot)
{
#if false
	movedGraph=false;
	
    for ( uint i = 0; i < QwtPlot::axisCnt; i++ )
		{
        QwtScale *scale = (QwtScale *)plot->axis(i);
        if ( scale )
            scale->installEventFilter(this);
		}
#endif
}

bool ScalePicker::eventFilter(QObject *object, QEvent *e)
{  
#if false
	if ( object->inherits("QwtScale") && e->type() == QEvent::MouseButtonDblClick)
    	{
		mouseDblClicked((const QwtScale *)object, ((QMouseEvent *)e)->pos());
        return true;
    	}

	if ( object->inherits("QwtScale") && e->type() == QEvent::MouseButtonPress)
    	{
		const QMouseEvent *me = (const QMouseEvent *)e;	
		if (me->button()==Qt::LeftButton)
			{
			emit clicked();	

			if (plot()->margin() < 2 && plot()->lineWidth() < 2)
				{
				QRect r = ((const QwtScale *)object)->rect();
				r.addCoords(2, 2, -2, -2);
				if (!r.contains(me->pos()))
					emit highlightGraph();
				}
			return true;
			}
		else if (me->button() == Qt::RightButton)
			{
			mouseRightClicked((const QwtScale *)object, me->pos());
			return true;
			}
    	}
	
	if ( object->inherits("QwtScale") && e->type() == QEvent::MouseMove)
    	{	
		const QMouseEvent *me = (const QMouseEvent *)e;			

		movedGraph=true;
		emit moveGraph(me->pos());

        return true;
   	 }
	
	if ( object->inherits("QwtScale") && e->type() == QEvent::MouseButtonRelease)
    	{
		if (movedGraph)
			{
			emit releasedGraph();
			movedGraph=false;
			}
				
        return true;
    	}
		
return QObject::eventFilter(object, e);
#endif
}

void ScalePicker::mouseDblClicked(const QwtScale *scale, const QPoint &pos) 
{
#if false
QRect rect = scaleRect(scale);

int margin = 2; // pixels tolerance
rect.setRect(rect.x() - margin, rect.y() - margin, rect.width() + 2 * margin, rect.height() +  2 * margin);

if ( rect.contains(pos) ) 
	{
	emit axisDblClicked(scale->position());
	}
else
	{// Click on the title
    switch(scale->position())   
        {
        case QwtScale::Left:
            {
			emit yAxisTitleDblClicked();
            break;
            }
        case QwtScale::Right:
            {
			emit rightAxisTitleDblClicked();
            break;
            }
        case QwtScale::Bottom:
            {
			emit xAxisTitleDblClicked();
            break;
            }
        case QwtScale::Top:
            {
			emit topAxisTitleDblClicked();
            break;
            }
		}
	}
#endif
}

void ScalePicker::mouseRightClicked(const QwtScale *scale, const QPoint &pos) 
{
#if false
QRect rect = scaleRect(scale);

int margin = 2; // pixels tolerance
rect.setRect(rect.x() - margin, rect.y() - margin, rect.width() + 2 * margin, rect.height() +  2 * margin);

if (rect.contains(pos)) 
	{
	if (scale->position() == QwtScale::Left || scale->position() == QwtScale::Right)
			emit axisRightClicked(scale->position());
	else if (scale->position() == QwtScale::Top)
		emit axisRightClicked(QwtScale::Bottom);
	else if (scale->position() == QwtScale::Bottom)
		emit axisRightClicked(QwtScale::Top);
	}
else
	{
	if (scale->position() == QwtScale::Left || scale->position() == QwtScale::Right)
		emit axisTitleRightClicked(scale->position());
	else if (scale->position() == QwtScale::Top)
		emit axisTitleRightClicked(QwtScale::Bottom);
	else if (scale->position() == QwtScale::Bottom)
		emit axisTitleRightClicked(QwtScale::Top);
	}
#endif
}

// The rect of a scale without the title
QRect ScalePicker::scaleRect(const QwtScale *scale) const
{
#if false
    const int bld = scale->baseLineDist();
    const int mjt = scale->scaleDraw()->majTickLength();
    const int sbd = scale->startBorderDist();
    const int ebd = scale->endBorderDist();
	
	int mlw, mlh;

    QRect rect;
    switch(scale->position())   
    {
        case QwtScale::Left:
        {
			mlw=maxLabelWidth(scale);
			
			rect.setRect(scale->width() - bld - mjt-mlw, sbd,
                mjt+mlw, scale->height() - sbd - ebd);
			
            break;
        }
        case QwtScale::Right:
        {
			mlw=maxLabelWidth(scale);
			rect.setRect(bld, sbd,
                 mjt+mlw, scale->height() - sbd - ebd);
            break;
        }
        case QwtScale::Bottom:
        {
			mlh=maxLabelHeight(scale);
			rect.setRect(sbd, bld, 
                scale->width() - sbd - ebd, mjt+mlh);
	        break;
        }
        case QwtScale::Top:
        {
			mlh=maxLabelHeight(scale);
			rect.setRect(sbd, scale->height() - bld - mjt-mlh, 
                scale->width() - sbd - ebd, mjt+mlh);
            break;
        }
    }
    return rect;
#endif
}

int ScalePicker::maxLabelWidth(const QwtScale *scale) const
{
#if false
	QFontMetrics fm(scale->font());
	const QwtScaleDraw * scaleDraw=scale->scaleDraw ();
	const QwtScaleDiv 	scaleDiv=scaleDraw->scaleDiv();
	const double step_eps = 1.0e-6;
    int maxWidth = 0;

    for (uint i = 0; i < scaleDiv.majCnt(); i++)
    {
        double val = scaleDiv.majMark(i);

        // correct rounding errors if val = 0
        if ((!scaleDiv.logScale()) 
            && (qwtAbs(val) < step_eps * qwtAbs(scaleDiv.majStep())))
        {
            val = 0.0;
        }

        const int w = fm.boundingRect(scaleDraw->label(val)).width();
        if ( w > maxWidth )
            maxWidth = w;
    }

    return maxWidth;
#endif
}

int ScalePicker::maxLabelHeight(const QwtScale *scale) const
{
#if false
	QFontMetrics fm(scale->font());
	const QwtScaleDraw * scaleDraw=scale->scaleDraw ();
	const QwtScaleDiv 	scaleDiv=scaleDraw->scaleDiv();
   
	int maxHeight = 0;

    for (uint i = 0; i < scaleDiv.majCnt(); i++)
    {
        double val = scaleDiv.majMark(i);

        const int w = fm.boundingRect(scaleDraw->label(val)).height();
        if ( w > maxHeight )
            maxHeight = w;
    }
	
    return maxHeight;
#endif
}

void ScalePicker::refresh()
{	
#if false
    for ( uint i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScale *scale = (QwtScale *)plot()->axis(i);
        if ( scale )
            scale->installEventFilter(this);
    }
#endif
}

TitlePicker::TitlePicker(QwtPlot *plot):
    QObject(plot)
{
#if false
movedGraph=false;

title = (QLabel *)plot->titleLabel();
title->setFocusPolicy(Qt::StrongFocus);
if (title)
	title->installEventFilter(this);
#endif
}

bool TitlePicker::eventFilter(QObject *object, QEvent *e)
{
#if false
	if (object != (QObject *)title)
		return false;
	
    if ( object->inherits("QLabel") && e->type() == QEvent::MouseButtonDblClick)
		{
		emit doubleClicked();
        return true;
		}

	if ( object->inherits("QLabel") &&  e->type() == QEvent::MouseButtonPress )
		{
		emit clicked();

		const QMouseEvent *me = (const QMouseEvent *)e;	
		if (me->button()==Qt::RightButton)
			emit showTitleMenu();

		QwtPlot *plot = (QwtPlot *)title->parent();
		if (plot->margin() < 2 && plot->lineWidth() < 2)
			{
			QRect r = title->rect();
			r.addCoords(2, 2, -2, -2);
			if (!r.contains(me->pos()))
				emit highlightGraph();
			}

		return true;
		}

	if ( object->inherits("QLabel") &&  e->type() == QEvent::MouseMove)
		{	
		const QMouseEvent *me = (const QMouseEvent *)e;			
		movedGraph=true;
		emit moveGraph(me->pos());

        return true;
		}
	
	if ( object->inherits("QLabel") && e->type() == QEvent::MouseButtonRelease)
		{
		const QMouseEvent *me = (const QMouseEvent *)e;
		if (me->button()== Qt::LeftButton)
			{
			emit clicked();
			if (movedGraph)
				{
				emit releasedGraph();
				movedGraph=false;
				}
        	return true;
			}
		}

	if ( object->inherits("QLabel") && 
        e->type() == QEvent::KeyPress)
		{
		switch (((const QKeyEvent *)e)->key()) 
			{
			case Qt::Key_Delete: 
			emit removeTitle();	
            return true;
			}
		}

    return QObject::eventFilter(object, e);
#endif
}
