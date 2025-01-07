# QSpdLogger

```cpp
QTextEdit *textEdit = new QTextEdit(mainWindow);
textEdit->setReadOnly(true);
textEdit->setWordWrapMode(QTextOption::NoWrap);

QSpdLogger::Instance()->installMessageHandler();
QSpdLogger::Instance()->setLogLevel(QtDebugMsg);
QSpdLogger::Instance()->addFileSink("log.txt",1024*1024*20,1000);
QSpdLogger::Instance()->addQTextEditSink(textEdit, 4096, true, true);
QSpdLogger::Instance()->addUdpSink("127.0.0.1",12345);
QSpdLogger::Instance()->addTcpSink("127.0.0.1",12346);
```
