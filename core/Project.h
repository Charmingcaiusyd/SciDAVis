/***************************************************************************
    File                 : Project.h
    Project              : SciDAVis
    Description          : Represents a SciDAVis project.
    --------------------------------------------------------------------
    Copyright            : (C) 2007 Tilman Hoener zu Siederdissen (thzs*gmx.net)
    Copyright            : (C) 2007 Knut Franke (knut.franke*gmx.de)
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
#ifndef PROJECT_H
#define PROJECT_H

#include "Folder.h"

class QString;
class ProjectWindow;
class QAction;
class AbstractScriptingEngine;

//! Represents a SciDAVis project.
/**
 * Project manages an undo stack and is responsible for creating ProjectWindow instances
 * as views on itself.
 */
class Project : public Folder
{
	Q_OBJECT

	public:
		enum MdiWindowVisibility
		{
			folderOnly,
			folderAndSubfolders,
			manual
		};

	public:
		Project();
		~Project();

		//!\name Reimplemented from AbstractAspect
		//@{
		virtual Project *project() const { return const_cast<Project*>(this); }
		virtual QUndoStack *undoStack() const;
		virtual QString path() const { return name(); }
		virtual ProjectWindow *view();
		virtual QMenu *createContextMenu() const;
		//@}

		AbstractScriptingEngine * scriptingEngine() const;

		void setMdiWindowVisibility(MdiWindowVisibility visibility);
		MdiWindowVisibility mdiWindowVisibility() const;
	
	private slots:
		void setMdiWindowVisibility(QAction * action);

	private:
		class Private;
		Private *d;
};

#endif // ifndef PROJECT_H