#include "scriptedit.h"
#include "note.h"

#include <qaction.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qsimplerichtext.h>

ScriptEdit::ScriptEdit(ScriptingEnv *env, QWidget *parent, const char *name)
  : QTextEdit(parent, name), scriptEnv(env)
{
  myScript = env->newScript("", this, name);
  connect(myScript, SIGNAL(error(const QString&,const QString&,int)), this, SLOT(insertErrorMsg(const QString&)));
  
  setWordWrap(QTextEdit::NoWrap);
  setTextFormat(PlainText);
  
  actionExecute = new QAction(NULL,"E&xecute",CTRL+Key_J, this, "execute");
  connect(actionExecute, SIGNAL(activated()), this, SLOT(execute()));
  actionExecuteAll = new QAction(NULL, "Execute &All", CTRL+SHIFT+Key_J, this, "executeAll");
  connect(actionExecuteAll, SIGNAL(activated()), this, SLOT(executeAll()));
  actionEval = new QAction(NULL , "&Evaluate Expression", CTRL+Key_Return, this, "evaluate");
  connect(actionEval, SIGNAL(activated()), this, SLOT(evaluate()));
  actionPrint = new QAction(NULL, "&Print", 0, this, "print");
  connect(actionPrint, SIGNAL(activated()), this, SLOT(print()));
  //TODO: CTRL+Key_I -> inspect (currently "Open image file". other shortcut?)

  functionsMenu = new QPopupMenu(this, "functionsMenu");
  Q_CHECK_PTR(functionsMenu);
}

QPopupMenu *ScriptEdit::createPopupMenu (const QPoint & pos)
{
  QPopupMenu *menu = QTextEdit::createPopupMenu(pos);
  Q_CHECK_PTR(menu);

  actionPrint->addTo(menu);
  menu->insertSeparator();

  actionExecute->addTo(menu);
  actionExecuteAll->addTo(menu);
  actionEval->addTo(menu);
  
  if (parent()->isA("Note"))
  {
    Note *sp = (Note*) parent();
    QAction *actionAutoexec = new QAction(0, tr("Auto&exec"), 0, menu);
    actionAutoexec->setToggleAction(true);
    actionAutoexec->setOn(sp->autoexec());
    connect(actionAutoexec, SIGNAL(toggled(bool)), sp, SLOT(setAutoexec(bool)));
    actionAutoexec->addTo(menu);
  }

  functionsMenu->clear();
  functionsMenu->insertTearOffHandle();
  QStringList flist = scriptEnv->mathFunctions();
  for (int i=0; i<flist.size(); i++)
  {
    int id = functionsMenu->insertItem(flist[i], this, SLOT(insertFunction(int)));
    functionsMenu->setItemParameter(id, i);
    functionsMenu->setWhatsThis(id, scriptEnv->mathFunctionDoc(flist[i]));
  }
  menu->insertItem(tr("&Functions"),functionsMenu);
  
  return menu;
}

void ScriptEdit::insertErrorMsg(const QString &message)
{
  QString err = message;
  err.prepend("\n").replace("\n","\n#> ");
  removeSelection();
  insert(err,(uint)QTextEdit::CheckNewLines);
}

void ScriptEdit::insertFunction(const QString &fname)
{
  if (hasSelectedText())
    insert(fname+"("+selectedText()+")");
  else {
    insert(fname + "()");
    int index,para;
    getCursorPosition(&para, &index);
    setCursorPosition(para, index-1);
  }
}

void ScriptEdit::insertFunction(int nr)
{
  insertFunction(scriptEnv->mathFunctions()[nr]);
}

void ScriptEdit::execute()
{
  int paraFrom, indexFrom, paraTo, indexTo;
  QString fname = "<%1:%2>";
  fname = fname.arg(name());
  if (!hasSelectedText())
  {
    moveCursor(QTextEdit::MoveLineStart,false);
    moveCursor(QTextEdit::MoveLineEnd,true);
  }
  getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
  fname = fname.arg(paraFrom+1);
  myScript->setName(fname);
  myScript->setCode(selectedText());
  myScript->exec();
}

void ScriptEdit::executeAll()
{
  QString fname = "<%1>";
  fname = fname.arg(name());
  myScript->setName(fname);
  myScript->setCode(text());
  myScript->exec();
}

void ScriptEdit::evaluate()
{
  int paraFrom, indexFrom, paraTo, indexTo;
  QString fname = "<%1:%2>";
  fname = fname.arg(name());
  if (!hasSelectedText())
  {
    moveCursor(QTextEdit::MoveLineStart,false);
    moveCursor(QTextEdit::MoveLineEnd,true);
  }
  getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
  fname = fname.arg(paraFrom+1);
  myScript->setName(fname);
  myScript->setCode(selectedText());
  myScript->setEmitErrors(false);
  QVariant res = myScript->eval();
  myScript->setEmitErrors(true);
  if (res.isValid() && res.canCast(QVariant::String))
  {
    insert("\n#> "+res.toString()+"\n", (uint)QTextEdit::CheckNewLines);
    removeSelection();
  } else { // statement or invalid
    if (myScript->exec())
    {
      removeSelection();
      insert("\n", (uint)QTextEdit::CheckNewLines);
      removeSelection();
    }
  }
}

ScriptEdit::~ScriptEdit()
{
}

void ScriptEdit::print()
{
QPrinter printer;
printer.setColorMode (QPrinter::GrayScale);
if (printer.setup()) 
	{
    printer.setFullPage( TRUE );
    QPainter painter;
    if ( !painter.begin(&printer ) )
         return;

	QPaintDeviceMetrics metrics( painter.device() );
	int dpiy = metrics.logicalDpiY();
	int margin = (int) ( (1/2.54)*dpiy ); // 1 cm margins
	QRect body( margin, margin, metrics.width() - 2*margin, metrics.height() - 2*margin );
	QSimpleRichText richText(QStyleSheet::convertFromPlainText(text()), QFont(), 
				context(), styleSheet(), mimeSourceFactory(), body.height());
	richText.setWidth( &painter, body.width() );
  	QRect view( body );
	int page = 1;
	do {
	    richText.draw( &painter, body.left(), body.top(), view, colorGroup() );
	    view.moveBy( 0, body.height() );
	    painter.translate( 0 , -body.height() );
	    painter.drawText( view.right() - painter.fontMetrics().width( QString::number( page ) ),
			view.bottom() + painter.fontMetrics().ascent() + 5, QString::number( page ) );
	    if ( view.top()  >= richText.height() )
			break;
	    printer.newPage();
	    page++;
		} 
	while (TRUE);
	}
}

