#ifndef PLOT_H
#define PLOT_H

#include <qobject.h>

#include <qwt_plot.h>

class Plot: public QwtPlot
{	
    Q_OBJECT

public:	
	Plot(QWidget *parent = 0, const char *name = 0);
	
	enum TicksType{None = 0, Out = 1, Both = 2, In = 3};
	
	QValueList <int> getMajorTicksType(){return majorTicksType;};
	void setMajorTicksType(int axis, int type){majorTicksType[axis]=type;}

	QValueList <int> getMinorTicksType(){return minorTicksType;};
	void setMinorTicksType(int axis, int type){minorTicksType[axis]=type;}

	int minorTickLength(){return minTickLength;};
	int majorTickLength(){return majTickLength;};
	void setTickLength (int minLength, int majLength);

	void setTicksLineWidth(int width);
	void printFrame(QPainter *painter, const QRect &rect) const;

	QColor frameColor();

	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );

	void drawPixmap(QPainter *painter, const QRect &rect);
	virtual void print(QPainter *, const QRect &rect,
        const QwtPlotPrintFilter & = QwtPlotPrintFilter()) const;
	
protected:
	void printCanvas(QPainter *painter, const QRect &canvasRect,
   			 const QwtArray<QwtDiMap> &map, const QwtPlotPrintFilter &pfilter) const;

	virtual void drawCanvasItems (QPainter *painter, const QRect &rect,
			const QwtArray< QwtDiMap > &map, const QwtPlotPrintFilter &pfilter) const;

	void drawInwardTicks(QPainter *painter, const QRect &rect, 
							const QwtDiMap&map, int axis) const;

	void drawInwardMinorTicks(QPainter *painter, const QRect &rect, 
							const QwtDiMap &map, int axis) const;

	void drawInwardMajorTicks(QPainter *painter, const QRect &rect, 
							const QwtDiMap &map, int axis) const;
signals:
	void selectPlot();
	void moveGraph(const QPoint&);
	void releasedGraph();
	void resizeGraph(const QPoint&);
	void resizedGraph();

protected:
	int minTickLength, majTickLength, d_lineWidth;
	QValueList <int> minorTicksType;
	QValueList <int> majorTicksType;
	bool movedGraph, ShiftButton, graphToResize;
	QPoint presspos;
};

#endif
