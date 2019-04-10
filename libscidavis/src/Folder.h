/***************************************************************************
    File                 : Folder.h
    Project              : SciDAVis
    --------------------------------------------------------------------
    Copyright            : (C) 2006 by Ion Vasilief, Tilman Benkert
    Email (use @ for *)  : ion_vasilief*yahoo.fr, thzs*gmx.net
    Description          : Folder for the project explorer

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
#ifndef FOLDER_H
#define FOLDER_H

#include <qobject.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QDropEvent>
#include <QMdiArea>

#include "MyWidget.h"
#include "SciDAVisObject.h"

class FolderListItem;
class FolderListView;

#include "Table.h"
#include "Matrix.h"
#include "MultiLayer.h"
#include "Note.h"

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

class Folder;

//! Folder for the project explorer
class Folder : public SciDAVisObject<QObject>
{
  Q_OBJECT

public:
  Folder(const QString &name="");

  PyQtList<MyWidget> windows() const {return lstWindows;};

  void addWindow( MyWidget *w ) {
    w->setFolder(this);
    lstWindows.append( w );
  };
  void removeWindow( MyWidget *w ){
    w->setFolder(0);
    lstWindows.removeAll(w);
    if (w==d_active_window) d_active_window=0;
  };

  int windowCount(bool recursive = false) const {
    int result = lstWindows.size();
    if (recursive)
      foreach (Folder *folder, folders())
        result += folder->windowCount(true);
    return result;
  };

  //! The list of subfolder names, including first generation children only
  QStringList subfolders();

  //! The list of subfolders
  PyQtList<Folder> folders() const;

  void addChild(Folder *f);
  
  //! Pointer to the subfolder called s
  Folder* findSubfolder(const QString& s, bool caseSensitive = true, bool partialMatch = false);
  /// @{
  /** @return folder named \a s.
      @thow if no such folder exists
  */
  Folder& folder(const QString& s, bool caseSensitive, bool partialMatch) {
    if (auto r=findSubfolder(s,caseSensitive,partialMatch))
      return *r;
    throw NoSuchObject();
  }
  Folder& folder(const QString& s, bool c) {return folder(s,c,false);}
  Folder& folder(const QString& s) {return folder(s,true);}
  /// @}
  
  //! Pointer to the first window matching the search criteria
  MyWidget& findWindow(const QString& s, bool windowNames, bool labels,
                       bool caseSensitive, bool partialMatch) const;
  MyWidget& findWindow(const QString& s, bool w, bool l, bool c) const
  {return findWindow(s,w,l,c,true);}
  MyWidget& findWindow(const QString& s, bool w, bool l) const
  {return findWindow(s,w,l,false);}
  MyWidget& findWindow(const QString& s, bool w) const {return findWindow(s,w,true);}
  MyWidget& findWindow(const QString& s) const {return findWindow(s,true);}

  //! get a window by name
  /**
   * Returns the first window with given name that inherits class cls;
   * NULL on failure. If recursive is true, do a depth-first recursive
   * search.
   */
  MyWidget* window(const QString &name, const char *cls="myWidget", bool recursive=false);

  template<class T> T& windowT(const QString& name, bool recursive) const {
    for (auto w: lstWindows)
      if (auto wt=dynamic_cast<T*>(w))
        if (name == QString(w->name().c_str()).mid(0,QString(w->name().c_str()).indexOf("@")))
          return *wt;
    if (recursive)
      for (auto c: children())
        if (auto f=dynamic_cast<Folder*>(c))
          try {
            return f->windowT<T>(name,recursive);
          } catch (const NoSuchObject&) {} //keep going if not found
    throw std::runtime_error((tr("Couldn't find window named ")+name).toStdString());
  }
  
  //! Return table named name or NULL
  Table& table(const QString &name, bool recursive) const
  {return windowT<Table>(name, recursive);}
  Table& table(const QString &name) const {return table(name,false);}
  

  //! Return matrix named name or NULL
  Matrix& matrix(const QString &name, bool recursive) const
  {return windowT<Matrix>(name, recursive);}
  Matrix& matrix(const QString &name) const {return matrix(name, false);}

  //! Return graph named name or NULL
  MultiLayer& graph(const QString &name, bool recursive) const
  {return windowT<MultiLayer>(name, recursive);}
  MultiLayer& graph(const QString &name) const {return graph(name,false);}
  
  //! Return note named name. @throw if nonexistent
  Note& note(const QString &name, bool recursive) const
  {return windowT<Note>(name, recursive); }
  Note& note(const QString &name) const {return note(name,false);}

  //! The complete path of the folder in the project tree
  QString path();

  //! The root of the hierarchy this folder belongs to.
  Folder& rootFolder();

  QString birthDate(){return birthdate;};
  void setBirthDate(const QString& s){birthdate = s;};

  QString modificationDate(){return modifDate;};
  void setModificationDate(const QString& s){modifDate = s;};

  //! Pointer to the corresponding QListViewItem in the main application
  FolderListItem * folderListItem(){return myFolderListItem;};
  void setFolderListItem(FolderListItem *it){myFolderListItem = it;};

  MyWidget *activeWindow(){return d_active_window;};
  void setActiveWindow(MyWidget *w){d_active_window = w;};

  // TODO: move to Aspect
  std::string name(){return objectName().toStdString();}
  void setName(const QString& s){setObjectName(s);}

  /// save contents of folder
  void save(const std::string& fileName);
  
protected:
  QString birthdate, modifDate;
  PyQtList<MyWidget> lstWindows;
  FolderListItem *myFolderListItem;

  //! Pointer to the active window in the folder
  MyWidget *d_active_window;
};

/*****************************************************************************
 *
 * Class WindowListItem
 *
 *****************************************************************************/
//! Windows list item class
class WindowListItem : public QTreeWidgetItem
{
public:
    WindowListItem( QTreeWidget *parent, MyWidget *w );
    static const int WindowType = QTreeWidgetItem::UserType+2;
    void setData(int, int, const QVariant&);

    MyWidget *window() { return myWindow; };

    FolderListView* folderListView() { return (FolderListView*)treeWidget(); };

protected:
    MyWidget *myWindow;
};

/*****************************************************************************
 *
 * Class FolderListItem
 *
 *****************************************************************************/
//! Folders list item class
class FolderListItem : public QTreeWidgetItem
{
public:
    FolderListItem( QTreeWidget *parent, Folder *f );
    FolderListItem( FolderListItem *parent, Folder *f );

	static const int FolderType = QTreeWidgetItem::UserType+1;

	void setActive( bool o );
	void setData(int, int, const QVariant&);

    Folder *folder() { return myFolder; };
	int depth();

	FolderListView* folderListView() { return (FolderListView*)treeWidget(); };


	//! Checks weather the folder item is a grandchild of the source folder
	/**
	 * \param src source folder item
	 */
	bool isChildOf(FolderListItem *src);

protected:
    Folder *myFolder;
};

/*****************************************************************************
 *
 * Class FolderListView
 *
 *****************************************************************************/
//! Folder list view class
class FolderListView : public SciDAVisObject<QTreeWidget>
{
    Q_OBJECT

	friend class FolderListItem;
	friend class WindowListItem;

public:
	FolderListView(const QString& name = QString() );

public slots:
	void adjustColumns();
	bool isRenaming() { return state() == QAbstractItemView::EditingState; };

protected:
	void startDrag();

    void dropEvent( QDropEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void mousePressEvent( QMouseEvent *e );
	void mouseDoubleClickEvent( QMouseEvent* e );
	void keyPressEvent ( QKeyEvent * e );
    void mouseReleaseEvent( QMouseEvent *){mousePressed = false;};
	void enterEvent(QEvent *){mousePressed = false;};

signals:
	void dragItems(QList<QTreeWidgetItem *> items);
	void dropItems(QTreeWidgetItem *dest);
	void renameItem(QTreeWidgetItem *item, int);
	void itemRenamed(QTreeWidgetItem *item, int, const QString&);
	void addFolderItem();
	void deleteSelection();

private:
	bool mousePressed;
	QPoint presspos;
};

#endif
