#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void closeEvent(QCloseEvent *event);
    void openFile(void);
    void saveFile(void);
    void run(void);

private slots:
    void on_open_triggered();
};

#endif // MAINWINDOW_H
