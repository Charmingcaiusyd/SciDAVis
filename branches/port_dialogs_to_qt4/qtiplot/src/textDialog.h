#ifndef TEXTDLG_H
#define TEXTDLG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>
#include <Q3GridLayout>

class Q3VBoxLayout; 
class Q3HBoxLayout; 
class Q3GridLayout; 
class QComboBox;
class QLabel;
class Q3MultiLineEdit;
class QPushButton;
class Q3ButtonGroup;
class ColorButton;

class TextDialog : public QDialog
{ 
    Q_OBJECT

public:
	enum TextType{TextMarker, AxisTitle};

    TextDialog(TextType type, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~TextDialog();

    ColorButton *colorBox, *backgroundBtn;
    QPushButton* buttonFont;
    QComboBox* backgroundBox;
    QPushButton* buttonOk;
	QPushButton* buttonApply;
    QComboBox* rotateBox;
    QPushButton* buttonCancel;
    QPushButton* buttonIndice;
    QPushButton* buttonExp;
    QPushButton* buttonSym;
    QPushButton* buttonB;
    QPushButton* buttonI;
    QPushButton* buttonU;
	QPushButton* buttonCurve;
    Q3MultiLineEdit* LineEdit;
	Q3ButtonGroup *GroupBox1, *GroupBox2;
	QPushButton* buttonMinGreek, *buttonMajGreek;
	QComboBox* alignementBox;

public slots:

	int alignment();
	void setAlignment(int align);

	void setText(const QString & t);
	QString getText();

	void customFont();
	QFont font(){return f;};
	void setFont(const QFont& fnt){f=fnt;};

	void pickTextColor();
	void setTextColor(QColor c);

	void pickBackgroundColor();
	void setBackgroundColor(QColor c);

	int backgroundType();
	void setBackgroundType(int bkg);

	void setAngle(int angle);
	void accept();
	void apply();

	void addIndex();
	void addExp();
	void addUnderline();
	void addItalic();
	void addBold();
	void addCurve();

	void showMinGreek();
	void showMajGreek();
	void addSymbol(const QString& letter);

signals:
	void values(const QString& text, int, int, const QFont&, const QColor&, const QColor&);
	void changeText(const QString &);
	void changeColor(const QColor &);
	void changeAlignment(int);
	void changeFont(const QFont &);

protected:
	QFont f;
	TextType text_type;
};

#endif // TEXTDLG_H
