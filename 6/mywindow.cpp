#include "mywindow.h"
#include <windows.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QProgressDialog>
#include <QProgressBar>

MyWindow::MyWindow(QWidget *parent) : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    problemName = new QLabel(this);
    problemName->setText("Dining students problem");
    problemName->setAlignment(Qt::AlignCenter);
    layout->addWidget(problemName);
    problemName->setStyleSheet("QLabel {"
                               "color: #fe1252;"
                               "font-size: 16px;"
                               "font-weight: bold;"
                               "}");

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);
    textEdit->setStyleSheet("QTextEdit {"
                            "background-color: #090c3f;"
                            "color: #fafafa;"
                            "}");

    runButton = new QPushButton("Run", this);
    layout->addWidget(runButton);
    connect(runButton, &QPushButton::clicked, this, &MyWindow::runCode);
    runButton->setStyleSheet("QPushButton {"
                             "border-radius: 10px;"
                             "padding: 10px 20px;"
                             "background-color: #fe1252;"
                             "color: #090c3f;"
                             "font-size: 16px;"
                             "font-weight: bold;"
                             "}"
                            );

    clearButton = new QPushButton("Clear", this);
    layout->addWidget(clearButton);
    connect(clearButton, &QPushButton::clicked, this, &MyWindow::clearLog);
    clearButton->setStyleSheet("QPushButton {"
                               "border-radius: 10px;"
                               "padding: 10px 20px;"
                               "background-color: #fe1252;"
                               "color: #090c3f;"
                               "font-size: 16px;"
                               "font-weight: bold;"
                               "}"
                             );

    openButton = new QPushButton("Open", this);
    layout->addWidget(openButton);
    connect(openButton, &QPushButton::clicked, this, &MyWindow::openFromFile);
    openButton->setStyleSheet("QPushButton {"
                              "border-radius: 10px;"
                              "padding: 10px 20px;"
                              "background-color: #fe1252;"
                              "color: #090c3f;"
                              "font-size: 16px;"
                              "font-weight: bold;"
                              "}"
                               );

    saveButton = new QPushButton("Save", this);
    layout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, &MyWindow::saveToFile);
    saveButton->setStyleSheet("QPushButton {"
                              "border-radius: 10px;"
                              "padding: 10px 20px;"
                              "background-color: #fe1252;"
                              "color: #090c3f;"
                              "font-size: 16px;"
                              "font-weight: bold;"
                              "}"
                              );

    aboutButton = new QPushButton("About", this);
    layout->addWidget(aboutButton);
    connect(aboutButton, &QPushButton::clicked, this, &MyWindow::aboutProgram);
    aboutButton->setStyleSheet("QPushButton {"
                               "border-radius: 10px;"
                               "padding: 10px 20px;"
                               "background-color: #fe1252;"
                               "color: #090c3f;"
                               "font-size: 16px;"
                               "font-weight: bold;"
                               "}"
                              );
}

void MyWindow::runCode()
{
    setEnabled(false);
    textEdit->append("Process Running... Wait for the end.");

    QProcess *process = new QProcess(this);
    connect(process, &QProcess::finished, this, &MyWindow::stopCode);

    process->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args) {
        args->flags |= CREATE_NO_WINDOW;
    });

    QString executablePath = QDir::currentPath() + "/ServerApplication.exe";

    process->start(executablePath);
}

void MyWindow::stopCode(int exitCode, QProcess::ExitStatus exitStatus)
{
    clearLog();

    setEnabled(true);

    QProcess *process = qobject_cast<QProcess*>(sender());

    QByteArray output = process->readAllStandardOutput();
    textEdit->append(output);

    process->close();
}

void MyWindow::clearLog()
{
    textEdit->clear();
}

void MyWindow::saveToFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save file"), "Lab6Log.txt", tr("Text files (*.txt)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            QMessageBox::information(this, tr("Success"), tr("The data has been successfully saved to a file."));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save the data to a file."));
        }
    }
}

void MyWindow::openFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text files (*.txt)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            textEdit->setPlainText(in.readAll());
            file.close();
            QMessageBox::information(this, tr("Success"), tr("The data has been successfully loaded from a file."));
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Failed to load the data from a file."));
        }
    }
}

void MyWindow::aboutProgram()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle(tr("About This Application"));
    aboutBox.setText(tr("This is an application created with Qt Creator\n(Lab. 6 (GUI for Lab. 5) - Operational Systems course)"));
    aboutBox.setInformativeText(tr("Version: 1.0\nMade by Kuchumov Artem\n(Telegram: @artmkchmv)"));
    aboutBox.setIcon(QMessageBox::Information);
    aboutBox.setWindowIcon(QIcon(":/icon.png"));
    aboutBox.setStyleSheet("QMessageBox {"
                           "background-color: #01020c;"
                           "}"
                           "QLabel {"
                           "color: #fe1252;"
                           "font-size: 16px;"
                           "font-weight: bold;"
                           "}");
    aboutBox.exec();
}
