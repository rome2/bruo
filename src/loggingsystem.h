#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H

#include "bruo.h"

class LoggingSystem
{
public:

  static void prepare();
  static void start();
  static const QString& logFileName();
  static void setOutputWindow(QTextEdit* textEdit);
  static void logMessage(QString& message);

private:

  static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

  static QString m_logFileName;
  static QtMessageHandler m_oldHandler;
  static QStringList m_cachedMessages;
  static QTextEdit* m_textEdit;
  static QMutex m_mutex;
};

#endif // LOGGINGSYSTEM_H
