#pragma once

#include <QMainWindow>

#include <atomic>
#include <thread>

class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent);
    ~MainWindow() override;

private slots:
    void onStartSpam();

private:
    QLineEdit* m_pIntervalEdit = nullptr;
    QLineEdit* m_pContentEdit = nullptr;
    QLineEdit* m_pCountEdit = nullptr;
    QPushButton* m_pStartButton = nullptr;
    std::thread m_spamThread = {};
    std::atomic_bool m_isStopSpam = false;
};
