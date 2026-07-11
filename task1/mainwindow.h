#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* pParent);
    ~MainWindow() override = default;

private slots:
    void showHelloDialog();
};
