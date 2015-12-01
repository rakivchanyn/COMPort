#include <QtWidgets/QComboBox>
#include <QtSerialPort/QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mSerial = new QSerialPort(this);
	ui->cbBoundRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
	ui->cbBoundRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
	ui->cbBoundRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
	ui->cbBoundRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
	ui->cbBoundRate->addItem(tr("Custom"));

	ui->cbDataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
	ui->cbDataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
	ui->cbDataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
	ui->cbDataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
	ui->cbDataBits->setCurrentIndex(3);

	ui->cbParity->addItem(tr("None"), QSerialPort::NoParity);
	ui->cbParity->addItem(tr("Even"), QSerialPort::EvenParity);
	ui->cbParity->addItem(tr("Odd"), QSerialPort::OddParity);
	ui->cbParity->addItem(tr("Mark"), QSerialPort::MarkParity);
	ui->cbParity->addItem(tr("Space"), QSerialPort::SpaceParity);

	ui->cbStopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
	ui->cbStopBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
	ui->cbStopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

	ui->cbFlowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
	ui->cbFlowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
	ui->cbFlowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

	ui->statusBar->showMessage("Ready to work");

	on_pbRescanPorts_clicked();
	connect(ui->pbConnect, SIGNAL(clicked()), this, SLOT(openSerialPort()));
	connect(ui->pbDisconnect, SIGNAL(clicked()), this, SLOT(closeSerialPort()));
	connect(mSerial, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(ui->pbClearConsole, SIGNAL(clicked()), this, SLOT(on_pbClearConsole_clicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pbRescanPorts_clicked()
{
	ui->cbSerialPort->clear();
	QString description;
	QString manufacturer;
	QString serialNumber;
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		QStringList list;
		description = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
			 << (!description.isEmpty() ? description : blankString)
			 << (!manufacturer.isEmpty() ? manufacturer : blankString)
			 << (!serialNumber.isEmpty() ? serialNumber : blankString)
			 << info.systemLocation()
			 << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
			 << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

		ui->cbSerialPort->addItem(list.first(), list);
	}

	ui->cbSerialPort->addItem(tr("Custom"));
}

void MainWindow::openSerialPort()
{
	mSerial->setPortName(ui->cbSerialPort->currentText());
	mSerial->setBaudRate(ui->cbBoundRate->currentText().toInt());
	mSerial->setDataBits(static_cast<QSerialPort::DataBits>(ui->cbDataBits->currentText().toInt()));
	mSerial->setParity(static_cast<QSerialPort::Parity>(ui->cbParity->currentText().toInt()));
	mSerial->setStopBits(static_cast<QSerialPort::StopBits>(ui->cbStopBits->currentText().toInt()));
	mSerial->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->cbFlowControl->currentText().toInt()));
	if (mSerial->open(QIODevice::ReadWrite))
	{
		ui->statusBar->showMessage("Connected");
	}

//		console->setEnabled(true);
//		console->setLocalEchoEnabled(p.localEchoEnabled);
//		ui->actionConnect->setEnabled(false);
//		ui->actionDisconnect->setEnabled(true);
//		ui->actionConfigure->setEnabled(false);
//		showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
//						  .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
//						  .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
//	} else {
//		QMessageBox::critical(this, tr("Error"), serial->errorString());

//		showStatusMessage(tr("Open error"));
//	}
}

void MainWindow::closeSerialPort()
{
	if (mSerial->isOpen())
		mSerial->close();
//	console->setEnabled(false);
//	ui->actionConnect->setEnabled(true);
//	ui->actionDisconnect->setEnabled(false);
//	ui->actionConfigure->setEnabled(true);
	ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::readData()
{
	QByteArray data = mSerial->readAll();
	ui->teConsole->append(QString(data));
}

void MainWindow::on_pbClearConsole_clicked()
{
	ui->teConsole->clear();
}
