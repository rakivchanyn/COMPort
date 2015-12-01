#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_pbRescanPorts_clicked();
	void openSerialPort();
	void closeSerialPort();
	void readData();

	void on_pbClearConsole_clicked();

private:
	Ui::MainWindow *ui;
	QSerialPort *mSerial;
};

#endif // MAINWINDOW_H
