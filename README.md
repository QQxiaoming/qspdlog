# QSpdLogger

QSpdLogger是对Spdlog的封装，可以直接把QtDebug的log重定位到Spdlog输出。

并提供API接口动态设置log等级、动态添加log输出文件或通过UDP/TCP发送、以及输出到QTextEdit。

QSpdLogger以单例形式提供API，通过 QSPDLogger-> 访问。

## API

| API                                  | 描述                          |
| ------------------------------------ | ----------------------------- |
| QSPDLogger->installMessageHandler    | 初始化并安装到QtDebug系统     |
| QSPDLogger->uninstallMessageHandler  | 反初始化取消安装到QtDebug系统 |
| QSPDLogger->setGlobalLogPattern      | 设置全局日志格式              |
| QSPDLogger->clearGlobalLogPattern    | 清理全局日志格式              |
| QSPDLogger->setLogLevel              | 设置全局日志级别              |
| QSPDLogger->setStdLogLevel           | 设置标准输出日志级别          |
| QSPDLogger->addFileSink              | 添加文件日志输出              |
| QSPDLogger->removeFileSink           | 移除文件日志输出              |
| QSPDLogger->setFileSinkLogLevel      | 设置文件日志输出级别          |
| QSPDLogger->addFileSink              | 添加 UDP 日志输出             |
| QSPDLogger->removeUdpSink            | 移除 UDP 日志输出             |
| QSPDLogger->setUdpSinkLogLevel       | 设置 UDP 日志输出级别         |
| QSPDLogger->TySpdLogAddTcpSink       | 添加 TCP 日志输出             |
| QSPDLogger->removeTcpSink            | 移除 TCP 日志输出             |
| QSPDLogger->setTcpSinkLogLevel       | 设置 TCP 日志输出级别         |
| QSPDLogger->addQTextEditSink         | 添加 QTextEdit 日志输出       |
| QSPDLogger->setQTextEditSinkLogLevel | 移除 QTextEdit 日志输出       |
| QSPDLogger->removeQTextEditSink      | 设置 QTextEdit 日志输出级别   |

## 示例

```cpp
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QTextEdit>
#include <QTimer>

#include "qspdlogger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextEdit *textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setWordWrapMode(QTextOption::NoWrap);

    QSPDLogger->installMessageHandler();
    QSPDLogger->setLogLevel(QtDebugMsg);
    QSPDLogger->addFileSink("log.txt",1024*1024*20,1000);
    QSPDLogger->addQTextEditSink(textEdit, 4096, true, true);
    QSPDLogger->addUdpSink("127.0.0.1",12345);
    QSPDLogger->addTcpSink("127.0.0.1",12346);

    textEdit->resize(800,600);
    textEdit->show();

    QTimer *timer = new QTimer(textEdit);
    QObject::connect(timer, &QTimer::timeout, [](){
        qDebug().noquote() << "Hello World!";
        qInfo().noquote() << "Hello World!";
        qWarning().noquote() << "Hello World!";
        qCritical().noquote() << "Hello World!";
    });
    timer->start(1000);

    int ret = app.exec();
    QSPDLogger->uninstallMessageHandler();
    return ret;
}
```
