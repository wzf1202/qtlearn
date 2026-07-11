#include "mainwindow.h"

#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QScreen>

MainWindow::MainWindow(QWidget* pParent) : QMainWindow(pParent)
{
    QWidget* pCentralWidget = new QWidget(this);
    setCentralWidget(pCentralWidget);
    const QRect screenGeometry = screen()->availableGeometry();
    resize(qRound(screenGeometry.width() * 0.5), qRound(screenGeometry.height() * 0.5));
    setWindowTitle(tr("MainWindow"));
    QPushButton* pButton = new QPushButton(tr("Click"), pCentralWidget);
    QVBoxLayout* pLayout = new QVBoxLayout(pCentralWidget);
    pLayout->addStretch();
    pLayout->addWidget(pButton, 0, Qt::AlignCenter);
    pLayout->addStretch();

    connect(pButton, &QPushButton::clicked, this, &MainWindow::showHelloDialog);
}

void MainWindow::showHelloDialog()
{
    QMessageBox::information(this, tr("Dialog"), "Hello Wrold!");
}
