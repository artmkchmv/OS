#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QProcess>

class MyWindow : public QWidget
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);

private slots:
    void runCode();
    void stopCode(int exitCode, QProcess::ExitStatus exitStatus);
    void clearLog();
    void openFromFile();
    void saveToFile();
    void aboutProgram();

private:
    QLabel *problemName;
    QTextEdit *textEdit;
    QPushButton *runButton;
    QPushButton *clearButton;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *aboutButton;
};

#endif // MYWINDOW_H
