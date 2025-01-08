#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QDateTime>

#include "qspdlogger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSPDLogger->installMessageHandler();
    QSPDLogger->setLogLevel(QtDebugMsg);
    QString current_date_file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSPDLogger->addFileSink(QDir::currentPath()+"/"+current_date_file_name+"/log.txt",1024*1024*20,1000);

    QMainWindow *mainWindow = new QMainWindow();
    QTextEdit *textEdit = new QTextEdit(mainWindow);
    textEdit->setReadOnly(true);
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    QSPDLogger->addQTextEditSink(textEdit, 4096, true, true);
    QComboBox *comboBox = new QComboBox(mainWindow);
    comboBox->addItem("QtDebugMsg");
    comboBox->addItem("QtInfoMsg");
    comboBox->addItem("QtWarningMsg");
    comboBox->addItem("QtCriticalMsg");
    comboBox->addItem("QtFatalMsg");
    QObject::connect(comboBox, &QComboBox::currentTextChanged, [textEdit](const QString &str){
        static const QMap<QString, QtMsgType> level_map = {
            {"QtDebugMsg"   , QtDebugMsg},
            {"QtInfoMsg"    , QtInfoMsg},
            {"QtWarningMsg" , QtWarningMsg},
            {"QtCriticalMsg", QtCriticalMsg},
            {"QtFatalMsg"   , QtFatalMsg},
        };
        QSPDLogger->setQTextEditSinkLogLevel(textEdit, level_map[str]);
    });
    QPushButton *sendLog = new QPushButton(mainWindow);
    sendLog->setText("Send Log");
    QObject::connect(sendLog, &QPushButton::clicked, [mainWindow](){
        static int lastlv = 0;
        static QString laststr;
        QStringList list = {"QtDebugMsg", "QtInfoMsg", "QtWarningMsg", "QtCriticalMsg", "QtFatalMsg"};
        bool isOk = false;
        QString lvl = QInputDialog::getItem(mainWindow, "Send Log", "Log Level", list, lastlv, false, &isOk);
        if (!isOk) return;
        QString str = QInputDialog::getText(mainWindow, "Send Log", "Log Message", QLineEdit::Normal, laststr, &isOk);
        if (!isOk) return;
        if (str.isEmpty()) return;
        switch (list.indexOf(lvl)){
            case 0: qDebug().noquote() << str; break;
            case 1: qInfo().noquote() << str; break;
            case 2: qWarning().noquote() << str; break;
            case 3: qCritical().noquote() << str; break;
            case 4: qFatal().noquote() << str; break;
            default: break;
        }
        lastlv = list.indexOf(lvl);
        laststr = str;
    });
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(comboBox);
    hLayout->addWidget(sendLog);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(textEdit);
    vLayout->addLayout(hLayout);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(vLayout);
    mainWindow->setCentralWidget(centralWidget);
    mainWindow->resize(1280, 720);
    mainWindow->show();

    // test
    qDebug().noquote() << "Hello World!";
    qInfo().noquote() << "Hello World!";
    qWarning().noquote() << "Hello World!";
    qCritical().noquote() << "Hello World!";
    //qFatal().noquote() << "Hello World!";
 
    int ret = app.exec();
    QSPDLogger->uninstallMessageHandler();
    return ret;
}
