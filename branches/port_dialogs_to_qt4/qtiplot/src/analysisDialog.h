#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class Q3ButtonGroup;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;
class Q3HButtonGroup;
	
class analysisDialog : public QDialog
{
    Q_OBJECT

public:
    analysisDialog( QWidget* parent = 0, const QString& text="Curve", const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~analysisDialog();

    QPushButton* buttonOk;
	QPushButton* buttonCancel;
    Q3ButtonGroup* GroupBox1;
    Q3HButtonGroup* GroupBox2;
    QCheckBox* boxShowFormula;
	QComboBox* boxName;

public slots:
    virtual void languageChange();
	void accept();
	void setCurveNames(const QStringList& names);
	void setOperationType(const QString& s){operation=s;};

signals:
	void options(const QString&);
	void analyse(const QString&, const QString&);

private:
	QString operation;
};

#endif 



