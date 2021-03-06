////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    loggingsystem.cpp
///\ingroup bruo
///\brief   Global logging system implementation.
///\author  Rolf Meyerhoff (badlantic@gmail.com)
///\version 1.0
/// This file is part of the bruo audio editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\par
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///\par
/// You should have received a copy of the GNU General Public License along with
/// this program; see the file COPYING. If not, see http://www.gnu.org/licenses/
/// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
/// Floor, Boston, MA 02110-1301, USA.
////////////////////////////////////////////////////////////////////////////////
#include "loggingsystem.h"

///////////////////////////////////////////////////////////////////////////////
// Static members:
QString          LoggingSystem::m_logFileName;
#if QT_VERSION >= 0x050000
QtMessageHandler LoggingSystem::m_oldHandler = 0;
#else
QtMsgHandler     LoggingSystem::m_oldHandler = 0;
#endif
QStringList      LoggingSystem::m_fileMessages;
QStringList      LoggingSystem::m_editMessages;
QTextEdit*       LoggingSystem::m_textEdit = 0;
QMutex           LoggingSystem::m_mutex;

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::prepare()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialize the logging system.
///\remarks Installs callbacks etc. This should be called as the first action
///         in your main function.
////////////////////////////////////////////////////////////////////////////////
void LoggingSystem::prepare()
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Install debug handler:
  #if QT_VERSION >= 0x050000
  m_oldHandler = qInstallMessageHandler(myMessageOutput);
  #else
  m_oldHandler = qInstallMsgHandler(myMessageOutput);
  #endif
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::start()
////////////////////////////////////////////////////////////////////////////////
///\brief   Start the actual logging.
///\remarks Creates and prepares the log file.
////////////////////////////////////////////////////////////////////////////////
void LoggingSystem::start()
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Get log file name:
  m_logFileName = getSettingsPath() + "debug.log";

  // Add start entries:
  QFile logFile(m_logFileName);
  logFile.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream log(&logFile);
  log << "--------------------------------------------------------------------------------" << endl;
  log << "Logging started on: " << QDateTime::currentDateTime().toString("MMMM dd yyyy, hh:mm:ss") << endl;
  log << "--------------------------------------------------------------------------------" << endl;

  // Add cached messages:
  for (int i = 0; i < m_fileMessages.count(); i++)
    log << m_fileMessages[i] << endl;
  m_fileMessages.clear();

  // Done with this file for now:
  logFile.close();
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::logFileName()
////////////////////////////////////////////////////////////////////////////////
///\brief  Access the log file.
///\return The name of the log file.
////////////////////////////////////////////////////////////////////////////////
const QString& LoggingSystem::logFileName()
{
  // Return our file name:
  return m_logFileName;
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::setOutputWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief Set the debug output window.
///\param [in] textEdit: Target text edit window for the log messages.
////////////////////////////////////////////////////////////////////////////////
void LoggingSystem::setOutputWindow(QTextEdit* textEdit)
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Store window:
  m_textEdit = textEdit;

  // Init with existing data:
  if (m_textEdit != 0)
  {
    try
    {
      for (int i = 0; i < m_editMessages.count(); i++)
        textEdit->append(m_editMessages[i]);
      m_editMessages.clear();
    }
    catch (...) {}
  }
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::logMessage()
////////////////////////////////////////////////////////////////////////////////
///\brief Write a raw message to the log system.
///\param [in] message: The actual log message.
///\remarks This message is written when pumpAsyncMessage() is called.
////////////////////////////////////////////////////////////////////////////////
void LoggingSystem::logMessage(QString& message)
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Cache message if we've got no log file yet:
  if (m_logFileName.isEmpty())
    m_fileMessages.append(message);
  else
  {
    // Add message to file:
    try
    {
      QFile logFile(m_logFileName);
      logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
      QTextStream log(&logFile);
      log << message << endl;
      log.flush();
      logFile.close();
    }
    catch (...) { }
  }

  // Add message to queue:
  m_editMessages.append(message);
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::pumpAsyncMessages()
////////////////////////////////////////////////////////////////////////////////
///\brief Write the cached async messages to the log system.
///\remarks Call this function from the main gui thread.
////////////////////////////////////////////////////////////////////////////////
void LoggingSystem::pumpAsyncMessages()
{
  // Lock system:
  QMutexLocker locker(&m_mutex);

  // Anything to do?
  if (m_editMessages.empty())
    return;

  // Add to the output window:
  if (m_textEdit != 0)
  {
    try
    {
      for (int i = 0; i < m_editMessages.count(); i++)
        m_textEdit->append(m_editMessages[i]);
    }
    catch (...) { }
  }

  // Clear cache:
  m_editMessages.clear();
}

////////////////////////////////////////////////////////////////////////////////
// LoggingSystem::myMessageOutput()
////////////////////////////////////////////////////////////////////////////////
///\brief    Message handler for Qt's debug messages.
///\param    [in] type:    The type of the message (debug, warning etc).
///\param    [in] context: Call context (source file and line etc).
///\param    [in] msg:     The actual message.
///\remarks: This function is set via qInstallMessageHandler(). The old
///          handler is called before doing anything else.
////////////////////////////////////////////////////////////////////////////////
#if QT_VERSION >= 0x050000
void LoggingSystem::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
#else
void LoggingSystem::myMessageOutput(QtMsgType type, const char* msg)
#endif
{
  // Call default handler (if any):
  if (m_oldHandler)
    #if QT_VERSION >= 0x050000
    m_oldHandler(type, context, msg);
    #else
    m_oldHandler(type, msg);
    #endif

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
  #if QT_VERSION >= 0x050500
  case QtInfoMsg:
    typeString = "Info";
    break;
  #endif
  }

  // Compose final message:
  QString message;
  #if SHOW_CONTEXT && QT_VERSION >= 0x050000
  QTextStream(&message) << typeString << ": " << msg << " (" << context.file << ":" << context.line << ", " << context.function;
  #else
  QTextStream(&message) << typeString << ": " << msg;
  #endif

  // Write to log:
  logMessage(message);
}

///////////////////////////////// End of File //////////////////////////////////
