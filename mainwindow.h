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

    void openFile(QString filename);
    void saveFile(QString filename);
    void run(void);

private:
    Ui::MainWindow *ui;
    QString base_window_title;

public slots:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_open_triggered();
    void on_save_triggered();
    void on_run_triggered();
};

#endif // MAINWINDOW_H
