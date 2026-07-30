#pragma once

#include <QMainWindow>

class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent);
    ~MainWindow() override = default;

private slots:
    void onStartSpam();

private:
    QLineEdit* m_pIntervalEdit = nullptr;
    QLineEdit* m_pContentEdit = nullptr;
    QLineEdit* m_pCountEdit = nullptr;
};
