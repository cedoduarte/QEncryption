/**
 * @author Carlos Enrique Duarte Ortiz (carlosduarte.1@hotmail.com)
 * @date November 18th 2022
 * @version 1.0
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qencryption.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QEncryption::saveRandomMatrix("salida.txt");

    QEncryptionInput input;
    input.setInput("hola mundo");
    input.setPassword("mypassword");
    input.setMatrix(QEncryption::loadMatrix("salida.txt"));
    QString encrypted = QEncryption::encrypt(input);

    QEncryptionInput output;
    output.setInput(encrypted);
    output.setPassword("mypassword");
    output.setMatrix(QEncryption::loadMatrix("salida.txt"));
    QString decrypted = QEncryption::decrypt(output);

    qDebug() << encrypted;
    qDebug() << decrypted;
}

MainWindow::~MainWindow()
{
    delete ui;
}
