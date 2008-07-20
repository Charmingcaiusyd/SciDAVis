/***************************************************************************
    File                 : Table.h
    Project              : SciDAVis
    Description          : Table worksheet class
    --------------------------------------------------------------------
    Copyright            : (C) 2006-2008 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2006-2008 Knut Franke (knut.franke*gmx.de)
    Copyright            : (C) 2006-2007 Ion Vasilief (ion_vasilief*yahoo.fr)
                           (replace * with @ in the email addresses) 

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
#ifndef TABLE_H
#define TABLE_H

#include <q3table.h>
#include <q3header.h>
#include <Q3ValueList>
#include <QVarLengthArray>

#include "Graph.h"
#include "MyWidget.h"
#include "ScriptingEnv.h"
#include "Script.h"
#include "future/table/future_Table.h"
#include "future/table/TableView.h"
#include "globals.h"

/*!\brief MDI window providing a spreadsheet table with column logic.
 *
 * \section future_plans Future Plans
 * Port to the Model/View approach used in Qt4 and get rid of the Qt3Support dependancy.
 * [ assigned to thzs ]
 */
class Table: public TableView, public scripted
{
    Q_OBJECT

public:
	future::Table *d_future_table;

// TODO: remove obsolete enum
//	enum PlotDesignation{All = -1, None = 0, X = 1, Y = 2, Z = 3, xErr = 4, yErr = 5};
	enum ColType{Numeric = 0, Text = 1, Date = 2, Time = 3, Month = 4, Day = 5};

   	Table(ScriptingEnv *env, const QString &fname,const QString &sep, int ignoredLines, bool renameCols,
		 bool stripSpaces, bool simplifySpaces, const QString &label,
		 QWidget* parent=0, const char* name=0, Qt::WFlags f=0);
	Table(ScriptingEnv *env, int r,int c, const QString &label, QWidget* parent=0, const char* name=0, Qt::WFlags f=0);

	//! Sets the number of significant digits
	void setNumericPrecision(int prec);

public slots:
// TODO: remove this
#if 0
	MyTable* table(){return d_table;};
#endif
	void copy(Table *m);
	int numRows();
	int numCols();
	void setNumRows(int rows);
	void setNumCols(int cols);
	void resizeRows(int);
	void resizeCols(int);
	void handleChange();

	//! Return column number 'index'
	Column* column(int index) const { return d_future_table->column(index); }
	//! Return the column determined by the given name
	/**
	 * This method should not be used unless absolutely necessary. 
	 * Columns should be addressed by their index. 
	 * This method is mainly meant to be used in scripts.
	 */
	Column* column(const QString & name) const { return d_future_table->column(name); }

	//! Return the value of the cell as a double
	double cell(int row, int col);
	void setCell(int row, int col, double val);

	QString text(int row, int col);
	QStringList columnsList();
	QStringList colNames();
	QString colName(int col);
	QString colLabel(int col);
	int colIndex(const QString& name);

	SciDAVis::PlotDesignation colPlotDesignation(int col);
	void setColPlotDesignation(int col, SciDAVis::PlotDesignation d);
	void setPlotDesignation(SciDAVis::PlotDesignation pd);
	QList<int> plotDesignations();

	void setColName(int col,const QString& text);
	void setHeader(QStringList header);
	void loadHeader(QStringList header);
	void setHeaderColType();
	void setText(int row,int col,const QString & text);

/////	void cellEdited(int,int col);
	void moveCurrentCell();
	void clearCell(int row, int col);

	void print();
	void print(const QString& fileName);
	void exportPDF(const QString& fileName);
    void customEvent(QEvent* e);

	//! \name Column Operations
	//@{
	void removeCol();
	void removeCol(const QStringList& list);
	void clearCol();
	void insertCol();
	void insertCols(int start, int count);
	void addCol(SciDAVis::PlotDesignation pd = SciDAVis::Y);
	void addColumns(int c);
	//@}

	int firstXCol();
	bool noXColumn();
	bool noYColumn();
	int colX(int col);
	int colY(int col);

	QStringList getCommands();
	//! Set all column formulae.
	void setCommands(const QStringList& com);
	//! Set all column formulae.
	void setCommands(const QString& com);
	//! Set formula for column col.
	void setCommand(int col, const QString& com);
	//! Compute specified cells from column formula.
	bool calculate(int col, int startRow, int endRow);
	//! Compute selected cells from column formulae; use current cell if there's no selection.
	bool calculate();

	//! \name Row Operations
	//@{
	void deleteSelectedRows();
	void insertRow();
	//@}

	//! Selection Operations
	//@{
	void cutSelection();
	void copySelection();
	void clearSelection();
	void pasteSelection();
	void selectAllTable();
	void deselect();
	void clear();
	//@}

	void init(int rows, int cols);
	QStringList selectedColumns();
	QStringList selectedYColumns();
	QStringList selectedErrColumns();
	QStringList selectedYLabels();
	QStringList drawableColumnSelection();
	QStringList YColumns();
	int selectedColsNumber();
	void changeColName(const QString& text);

	void changeColWidth(int width, bool allCols);
	void changeColWidth(int width, int col);
	int columnWidth(int col);
	void setColWidths(const QStringList& widths);

#if 0 // remove
	void setSelectedCol(int col){selectedCol = col;};
	int selectedColumn(){return selectedCol;};
#endif
	int numSelectedRows();

	void columnNumericFormat(int col, char *f, int *precision);
	void columnNumericFormat(int col, int *f, int *precision);
	int columnType(int col);

	QList<int> columnTypes();
	void setColumnTypes(QList<int> ctl);
	void setColumnTypes(const QStringList& ctl);
	void setColumnType(int col, SciDAVis::ColumnMode mode);

    void saveToMemory(double **cells);
	void saveToMemory();
	void freeMemory();

	QString columnFormat(int col);
	QStringList getColumnsFormat();
	void setColumnsFormat(const QStringList& lst);

	void setTextFormat(int col);
	void setColNumericFormat(int f, int prec, int col, bool updateCells = true);
	bool setDateFormat(const QString& format, int col, bool updateCells = true);
	bool setTimeFormat(const QString& format, int col, bool updateCells = true);
	void setMonthFormat(const QString& format, int col, bool updateCells = true);
	void setDayFormat(const QString& format, int col, bool updateCells = true);

	bool exportASCII(const QString& fname, const QString& separator,
					bool withLabels = false, bool exportSelection = false);
	void importASCII(const QString &fname, const QString &sep, int ignoredLines,
						bool renameCols, bool stripSpaces, bool simplifySpaces, bool newTable);
	void importMultipleASCIIFiles(const QString &fname, const QString &sep, int ignoredLines,
					bool renameCols, bool stripSpaces, bool simplifySpaces, int importFileAs);

	//! \name Saving and Restoring
	//@{
	virtual QString saveToString(const QString& geometry);
	QString saveHeader();
	QString saveComments();
	QString saveCommands();
	QString saveColumnWidths();
	QString saveColumnTypes();
	//@}

	void setBackgroundColor(const QColor& col);
	void setTextColor(const QColor& col);
	void setHeaderColor(const QColor& col);
	void setTextFont(const QFont& fnt);
	void setHeaderFont(const QFont& fnt);

	int verticalHeaderWidth();

	QString colComment(int col);
	void setColComment(int col, const QString& s);
	QStringList colComments();
	void setColComments(const QStringList& lst);
	bool commentsEnabled();

	QString saveAsTemplate(const QString& geometryInfo);
	void restore(const QStringList& lst);

	//! This slot notifies the main application that the table has been modified. Triggers the update of 2D plots.
	void notifyChanges();

	//! Notifies the main application that the width of a table column has been modified by the user.
////	void colWidthModified(int, int, int);

signals:
	void changedColHeader(const QString&, const QString&);
	void removedCol(const QString&);
	void modifiedData(Table *, const QString&);
	void resizedTable(QWidget*);
	void showContextMenu(bool selection);
	void createTable(const QString&,int,int,const QString&);

private:
	double **d_saved_cells;

	//! Internal function to change the column header
	void setColumnHeader(int index, const QString& label);
};

#endif
