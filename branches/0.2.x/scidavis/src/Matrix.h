/***************************************************************************
    File                 : Matrix.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief,
                           Tilman Benkert
                           Knut Franke
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net,
                           knut.franke*gmx.de
    Description          : Matrix worksheet class

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
#ifndef MATRIX_H
#define MATRIX_H

#include <QTableWidget>
#include <QContextMenuEvent>
#include <QEvent>
#include <QHeaderView>
#include <QDateTime>
#include "MyWidget.h"
#include "ScriptingEnv.h"
#include "Script.h"
#include <qwt_double_rect.h>
#include "future/matrix/future_Matrix.h"
#include "future/matrix/MatrixView.h"

// (maximum) initial matrix size
#define _Matrix_initial_rows_ 10
#define _Matrix_initial_columns_ 3

//! Matrix worksheet class
class Matrix: public MatrixView, public scripted
{
    Q_OBJECT

public:
	future::Matrix *d_future_matrix;

	/*!
	 * \brief Constructor
	 *
	 * \param env scripting interpreter
	 * \param r initial number of rows
	 * \param c initial number of columns
	 * \param label window label
	 * \param parent parent object
	 * \param name
	 * \param name window name
	 * \param f window flags
	 */
	Matrix(ScriptingEnv *env, int r, int c, const QString& label, QWidget* parent=0, const char* name=0, Qt::WFlags f=0);
	~Matrix() { delete d_future_matrix; };

	//! Return the number of rows
	int numRows();
	void setNumRows(int rows);

	//! Return the number of columns
	int numCols();
	void setNumCols(int cols);

	//! Returns whether the row is empty or not
	// TODO: is this used anywhere at all?
//	bool isEmptyRow(int row);

	//event handlers
	/*!
	 * \brief Event filter
	 *
	 * Currently only reacts to events of the
	 * title bar.
	 */
	bool eventFilter(QObject *object, QEvent *e);
	//! Custom event handler
	/**
	 * Currently handles SCRIPTING_CHANGE_EVENT only.
	 */
	void customEvent(QEvent *e);

	void updateDecimalSeparators();

	virtual QWidget *view() {return d_future_matrix->view(); }


public slots:
	void exportPDF(const QString& fileName);
	//! Print the Matrix
	void print();
	//! Print the Matrix to fileName
	void print(const QString& fileName);
#if 0
	//! Called if any cell value was changed
	void cellEdited(int,int);
#endif
	void handleChange();

#if 0
	//! Return the width of all columns
	int columnsWidth();
#endif
	//! Set the width of all columns (obsolete, only for OPJ import)
	/**
	 * Don't uses this. Use view()->setColumnWidth(column, width) instead.
	 */
	void setColumnsWidth(int width);

	//! Set the Matrix size
	void setDimensions(int rows, int cols);
	//! Transpose the matrix
	void transpose();
	//! Invert the matrix
	void invert();
	//! Calculate the determinant of the matrix
	double determinant();

	//! Calculate matrix values using the formula
	bool calculate(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1);

	//! Return the content of the cell as a string
	QString text(int row, int col);
	//! Set the content of the cell as a string
	void setText(int row, int col, const QString & new_text );
	//! Return the value of the cell as a double
	double cell(int row, int col);
	//! Set the value of the cell
	void setCell(int row, int col, double value );

	/*!
	 * \brief Return the text format code ('e', 'f', ...)
	 *
	 * \sa setNumerFormat(), setTextFormat()
	 */
	QChar textFormat() {return d_future_matrix->numericFormat();};
	/*!
	 * \brief Return the number precision digits
	 *
	 * See arguments of setNumericFormat().
	 * \sa setNumericFormat(), setTextFormat()
	 */
	int precision() {return d_future_matrix->displayedDigits();};
	/*!
	 * \brief Set the number of significant digits
	 *
	 * \sa precision(), setNumericFormat(), setTextFormat()
	 */
	void setNumericPrecision(int prec) { d_future_matrix->setDisplayedDigits(prec);};

	/*!
	 * \brief Set the number format for the cells
	 *
	 * This method should only be called before any user
	 * interaction was done. Use setTextFormat() if you
	 * want to change it from a dialog.
	 * \sa setTextFormat()
	 */
	void setTextFormat(const QChar &format, int precision);
	/*!
	  \brief Set the number format for the cells

	  You must call saveCellsToMemory() before and
	  forgetSavedCells() after calling this!
	  Format character and precision have the same meaning as for
	  sprintf().
	  \param f format character 'e', 'f', 'g'
	  \param prec
	  - for 'e', 'f': the number of digits after the radix character (point)
	  - for 'g': the maximum number of significant digits

	  \sa saveCellsToMemory(), forgetSavedCells(), dMatrix
	  */
	void setNumericFormat(const QChar & f, int prec);

	//! Return the matrix formula
	QString formula();
	//! Set the matrix forumla
	void setFormula(const QString &s);

	//! Format the matrix format in a string to save it in a template file
	QString saveAsTemplate(const QString &info);

	//! Return a string to save the matrix in a project file (\<matrix\> section)
	QString saveToString(const QString &info);
	//! Return a string conaining the data of the matrix (\<data\> section)
	QString saveText();

	// selection operations
	//! Standard cut operation
	void cutSelection();
	//! Standard copy operation
	void copySelection();
	//! Clear cells
	void clearSelection();
	//! Standard paste operation
	void pasteSelection();

	//! Insert a row before the current cell
	void insertRow();
	//! Return whether any rows are fully selected
	bool rowsSelected();
	//! Delete the selected rows
	void deleteSelectedRows();
	//! Return the number of selected rows
	int numSelectedRows();

	//! Insert a column before the current cell
	void insertColumn();
	//! Return whether any columns are fully selected
	bool columnsSelected();
	//! Delte the selected columns
	void deleteSelectedColumns();
	//! Return the number of selected columns
	int numSelectedColumns();
	//! Returns whether a column contains selected cells (full=false) or is fully selected (full=true)
	bool isColumnSelected(int col, bool full=false);
	//! Returns whether a row contains selected cells (full=false) or is fully selected (full=true)
	bool isRowSelected(int row, bool full=false);
	//! Return the first fully selected column or -1 if no column is fully selected
	int firstSelectedColumn();

	/*!
	 * \brief Temporally save the cell values to memory
	 *
	 * \sa setNumericFormat(), forgetSavedCells(), dMatrix
	 */
	void saveCellsToMemory();
	/*!
	 * \ brief Free memory of saved cells
	 *
	 * \sa setNumericFormat(), saveCellsToMemory(), dMatrix
	 */
	void forgetSavedCells();

	//! Return the X value corresponding to column 1
	double xStart(){return d_future_matrix->xStart();};
	//! Return the X value corresponding to the last column
	double xEnd(){return d_future_matrix->xEnd();};
	//! Return the Y value corresponding to row 1
	double yStart(){return d_future_matrix->yStart();};
	//! Return the Y value corresponding to the last row
	double yEnd(){return d_future_matrix->yEnd();};

	//! Returns the bounding rect of the matrix coordinates
  	QwtDoubleRect boundingRect() {return QwtDoubleRect(xStart(), yStart(), xEnd()-xStart(), yEnd()-yStart()).normalized();};
	//! Set the X and Y coordinate intervals
	void setCoordinates(double xs, double xe, double ys, double ye);

	 //! Min and max values of the matrix.
  	void range(double *min, double *max);

	//! Scroll to cell
	void goToCell(int row, int col);

	//! Allocate memory for a matrix buffer
	static double** allocateMatrixData(int rows, int columns);
	//! Free memory used for a matrix buffer
	static void freeMatrixData(double **data, int rows);

	static Matrix * fromImage(const QImage & image);
    void copy(Matrix *m);
	
	//! Return the creation date
	virtual QString birthDate(){return d_future_matrix->creationTime().toString(Qt::LocalDate); };

signals:
	//! Show the context menu
	void showContextMenu();

private:
	//! Initialize the matrix
	void init(int rows, int cols);

	MatrixView *d_view;
	//! Stores the matrix data only before the user opens the matrix dialog in order to avoid data loses during number format changes.
	double **dMatrix;
	double x_start, //!< X value corresponding to column 1
	x_end,  //!< X value corresponding to the last column
	y_start,  //!< Y value corresponding to row 1
	y_end;  //!< Y value corresponding to the last row

};

#endif
