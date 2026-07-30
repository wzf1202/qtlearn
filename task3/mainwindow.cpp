#include "mainwindow.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <windows.h>

MainWindow::MainWindow(QWidget* pParent) : QMainWindow(pParent)
{
    QWidget* pCentralWidget = new QWidget(this);
    setCentralWidget(pCentralWidget);

    const QRect screenGeometry = screen()->availableGeometry();
    resize(qRound(screenGeometry.width() * 0.5), qRound(screenGeometry.height() * 0.5));
    setWindowTitle(tr("Spam Tool"));

    m_pIntervalEdit = new QLineEdit(pCentralWidget);
    m_pIntervalEdit->setPlaceholderText(tr("Interval in ms"));
    m_pIntervalEdit->setText(QStringLiteral("100"));
    m_pIntervalEdit->setMaximumWidth(220);

    m_pContentEdit = new QLineEdit(pCentralWidget);
    m_pContentEdit->setPlaceholderText(tr("Spam content"));
    m_pContentEdit->setMaximumWidth(220);

    m_pCountEdit = new QLineEdit(pCentralWidget);
    m_pCountEdit->setPlaceholderText(tr("Repeat count"));
    m_pCountEdit->setText(QStringLiteral("10"));
    m_pCountEdit->setMaximumWidth(220);

    QPushButton* pStartButton = new QPushButton(tr("Start"), pCentralWidget);

    QFormLayout* pFormLayout = new QFormLayout();
    pFormLayout->addRow(tr("Interval (ms)"), m_pIntervalEdit);
    pFormLayout->addRow(tr("Content"), m_pContentEdit);
    pFormLayout->addRow(tr("Count"), m_pCountEdit);

    QWidget* pFormContainer = new QWidget(pCentralWidget);
    pFormContainer->setLayout(pFormLayout);
    pFormContainer->setMaximumWidth(360);

    QVBoxLayout* pLayout = new QVBoxLayout(pCentralWidget);
    pLayout->addStretch();
    pLayout->addWidget(pFormContainer, 0, Qt::AlignHCenter);
    pLayout->addWidget(pStartButton, 0, Qt::AlignHCenter);
    pLayout->addStretch();

    connect(pStartButton, &QPushButton::clicked, this, &MainWindow::onStartSpam);
}

void MainWindow::onStartSpam()
{
    const int intervalMs = m_pIntervalEdit->text().trimmed().toInt();
    const int count = m_pCountEdit->text().trimmed().toInt();

    if (intervalMs <= 0)
    {
        QMessageBox::warning(this, tr("Invalid input"), tr("Interval must be a positive integer (ms)."));
        return;
    }
    if (count <= 0)
    {
        QMessageBox::warning(this, tr("Invalid input"), tr("Count must be a positive integer."));
        return;
    }
    if (m_pContentEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Invalid input"), tr("Content must not be empty."));
        return;
    }

    QMessageBox::information(this, tr("Ready"), tr("Click OK, then switch to the target input box within 3 seconds."));
    QThread::msleep(3000);
    for (int i = 0; i < count; ++i)
    {
        for (QChar ch : m_pContentEdit->text())
        {
            INPUT inputs[2] = {};
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = 0;
            inputs[0].ki.wScan = ch.unicode();
            inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;
            inputs[1] = inputs[0];
            inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            SendInput(2, inputs, sizeof(INPUT));
        }

        INPUT enterInputs[2] = {};
        enterInputs[0].type = INPUT_KEYBOARD;
        enterInputs[0].ki.wVk = VK_RETURN;
        enterInputs[1].type = INPUT_KEYBOARD;
        enterInputs[1].ki.wVk = VK_RETURN;
        enterInputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(2, enterInputs, sizeof(INPUT));
        QThread::msleep(intervalMs);
    }
}
