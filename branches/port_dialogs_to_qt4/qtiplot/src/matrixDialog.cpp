#include "matrixDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <q3buttongroup.h>
#include <qspinbox.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

matrixDialog::matrixDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "matrixDialog" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMouseTracking( false );

	Q3ButtonGroup *GroupBox1 = new Q3ButtonGroup(3, Qt::Horizontal,QString::null,this,"GroupBox1" );
	GroupBox1->setFlat(TRUE);
// FIXME: replace this with Qt4 equivalent
//X	GroupBox1->setLineWidth(0);

	new QLabel( tr( "Cell Width" ), GroupBox1, "TextLabel2",0 );
    boxColWidth = new QSpinBox(0,1000,10, GroupBox1, "boxColWidth" );
	new QLabel( "", GroupBox1, "TextLabel22",0 );

	new QLabel( tr( "Data Format" ), GroupBox1, "TextLabel2",0 );
    boxFormat = new QComboBox(GroupBox1, "boxFormat" );
	new QLabel( "", GroupBox1, "TextLabel23",0 );

	new QLabel( tr( "Numeric Display" ), GroupBox1, "TextLabel2",0 );
    boxNumericDisplay = new QComboBox(GroupBox1, "boxNumericDisplay");
	boxPrecision = new QSpinBox(0,100,1, GroupBox1, "boxPrecision");
	boxPrecision->hide();

	Q3ButtonGroup *GroupBox2 = new Q3ButtonGroup( 3,Qt::Horizontal,QString::null,this,"GroupBox2" );
	GroupBox2->setFlat(TRUE);
// FIXME: replace this with Qt4 equivalent
//X	GroupBox2->setLineWidth(0);
	
	buttonApply = new QPushButton(GroupBox2, "buttonApply" );
    buttonApply->setAutoDefault( TRUE );

	buttonOk = new QPushButton(GroupBox2, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
   
    buttonCancel = new QPushButton(GroupBox2, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
	
	Q3VBoxLayout* hlayout = new Q3VBoxLayout(this, 5, 5, "hlayout");
    hlayout->addWidget(GroupBox1);
	hlayout->addWidget(GroupBox2);

    languageChange();
   
    // signals and slots connections
	connect( buttonApply, SIGNAL( clicked() ), this, SLOT( apply() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( boxNumericDisplay, SIGNAL( activated(int) ), this, SLOT( showPrecisionBox(int) ) );
	connect( boxColWidth, SIGNAL( valueChanged(int) ), this, SIGNAL( changeColumnsWidth(int) ) );
	connect( boxPrecision, SIGNAL( valueChanged(int) ), this, SLOT( changePrecision(int) ) );
}

matrixDialog::~matrixDialog()
{
}

void matrixDialog::changePrecision(int precision)
{
if (boxFormat->currentItem())
	emit changeTextFormat('e', precision);
else
	emit changeTextFormat('f', precision);
}

void matrixDialog::setTextFormat(const QString& format, int precision)
{
if (format == "f")
	boxFormat->setCurrentItem(0);
else
	boxFormat->setCurrentItem(1);

boxPrecision->setValue(precision);
if (precision != 6)
	{
	boxPrecision->show();
	boxNumericDisplay->setCurrentItem(1);
	}
}

void matrixDialog::showPrecisionBox(int item)
{
if (item)
	boxPrecision->show();
else
	{
	boxPrecision->setValue(6);
	boxPrecision->hide();
	}
}

void matrixDialog::setColumnsWidth(int width)
{
boxColWidth->setValue(width);
}

void matrixDialog::languageChange()
{
    setCaption( tr( "QtiPlot - Matrix Properties" ) );
    buttonOk->setText( tr( "&OK" ) );
	buttonCancel->setText( tr( "&Cancel" ) );
	buttonApply->setText( tr( "&Apply" ) );

	boxFormat->clear();
	boxFormat->insertItem( tr( "Decimal: 1000" ) );
    boxFormat->insertItem( tr( "Scientific: 1E3" ) );

	boxNumericDisplay->clear();
    boxNumericDisplay->insertItem( tr( "Default Decimal Digits" ) );
    boxNumericDisplay->insertItem( tr( "Significant Digits=" ) );
}

void matrixDialog::apply()
{
emit changeColumnsWidth(boxColWidth->value());
if (boxFormat->currentItem())
	emit changeTextFormat('e', boxPrecision->value());
else
	emit changeTextFormat('f', boxPrecision->value());
}

void matrixDialog::accept()
{
apply();
close();
}
