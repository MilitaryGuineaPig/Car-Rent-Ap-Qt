#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QDialog>

namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QWidget *parent = nullptr);
    ~ApplicationWindow();

private:
    Ui::ApplicationWindow *ui;
};

#endif // APPLICATIONWINDOW_H
