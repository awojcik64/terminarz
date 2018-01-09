#ifndef CALCULATEWEEKDAY_H
#define CALCULATEWEEKDAY_H

#include <QDialog>

namespace Ui {
class calculateWeekday;
}

class calculateWeekday : public QDialog
{
    Q_OBJECT

public:
    explicit calculateWeekday(QWidget *parent = 0);
    ~calculateWeekday();

private slots:
    void on_wybor_daty_dateChanged(const QDate &date);

private:
    Ui::calculateWeekday *ui;
};

#endif // CALCULATEWEEKDAY_H
