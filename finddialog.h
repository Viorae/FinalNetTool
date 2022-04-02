#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();
public slots:
    void on_searchbtn_clicked();

signals:
    void search(QString searchstr);
    void reset();
private slots:
    void on_FindDialog_rejected();

private:
    Ui::FindDialog *ui;

};

#endif // FINDDIALOG_H
