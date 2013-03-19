#include "loggingsystem.h"

QString LoggingSystem::m_logFileName;
QtMessageHandler LoggingSystem::m_oldHandler = 0;
QStringList LoggingSystem::m_cachedMessages;
QTextEdit* LoggingSystem::m_textEdit = 0;
QMutex LoggingSystem::m_mutex;

void LoggingSystem::prepare()
{
  // Install debug handler:
  m_oldHandler = qInstallMessageHandler(myMessageOutput);
}

void LoggingSystem::start()
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Get log file name:
  QSettings settings;
  QString fileName = settings.fileName();
  if (!fileName.isEmpty())
    m_logFileName = QFileInfo(fileName).path() + QDir::separator() + QFileInfo(fileName).baseName() + ".log";

  // Add start entries:
  QFile logFile(m_logFileName);
  logFile.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream log(&logFile);
  log << "--------------------------------------------------------------------------------" << endl;
  log << "Logging started on: " << QDateTime::currentDateTime().toString("MMMM dd yyyy, hh:mm:ss") << endl;
  log << "--------------------------------------------------------------------------------" << endl;

  // Add cached messages:
  for (int i = 0; i < m_cachedMessages.count(); i++)
    log << m_cachedMessages[i] << endl;

  // Done with this file for now:
  logFile.close();
}

const QString& LoggingSystem::logFileName()
{
  // Return our file name:
  return m_logFileName;
}

void LoggingSystem::setOutputWindow(QTextEdit* textEdit)
{
  // Store window:
  m_textEdit = textEdit;

  // Init with existing data:
  if (m_textEdit != 0)
  {
    for (int i = 0; i < m_cachedMessages.count(); i++)
      textEdit->append(m_cachedMessages[i]);
  }
}

void LoggingSystem::logMessage(QString& message)
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // If we don't have a target yet cache the text:
  if (m_logFileName.isEmpty() || m_textEdit == 0)
  {
    m_cachedMessages.append(message);
    if (m_logFileName.isEmpty())
      return;
  }

  // Add message to the log file:
  QFile logFile(m_logFileName);
  logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
  QTextStream log(&logFile);
  log << message << endl;
  log.flush();
  logFile.close();

  // Add to the output window:
  if (m_textEdit != 0)
    m_textEdit->append(message);
}

void LoggingSystem::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
  // Call default handler (if any):
  if (m_oldHandler)
    m_oldHandler(type, context, msg);

  // Convert type to string:
  QString typeString;
  switch (type)
  {
  case QtDebugMsg:
    typeString = "Debug";
    break;
  case QtWarningMsg:
    typeString = "Warning";
    break;
  case QtCriticalMsg:
    typeString = "Critical";
    break;
  case QtFatalMsg:
    typeString = "Fatal";
    break;
  }

  // Compose final message:
  QString message;
  QTextStream(&message) << typeString << ": " << msg << " (" << context.file << ":" << context.line << ", " << context.function;

  // Write to log:
  logMessage(message);
}
