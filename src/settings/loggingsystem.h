////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    loggingsystem.h
///\ingroup bruo
///\brief   Global logging system definition.
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
#ifndef __LOGGINGSYSTEM_H_INCLUDED__
#define __LOGGINGSYSTEM_H_INCLUDED__

#include "bruo.h"

class LoggingSystem
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::prepare()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialize the logging system.
  ///\remarks Installs callbacks etc. This should be called as the first action
  ///         in your main function.
  //////////////////////////////////////////////////////////////////////////////
  static void prepare();

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::start()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Start the actual logging.
  ///\remarks Creates and prepares the log file.
  //////////////////////////////////////////////////////////////////////////////
  static void start();

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::logFileName()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Access the log file.
  ///\return The name of the log file.
  //////////////////////////////////////////////////////////////////////////////
  static const QString& logFileName();

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::setOutputWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Set the debug output window.
  ///\param [in] textEdit: Target text edit window for the log messages.
  //////////////////////////////////////////////////////////////////////////////
  static void setOutputWindow(QTextEdit* textEdit);

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::logMessage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Write a raw message to the log system.
  ///\param [in] message: The actual log message.
  //////////////////////////////////////////////////////////////////////////////
  static void logMessage(QString& message);

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::pumpAsyncMessages()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Write the cached async messages to the log system.
  ///\remarks Call this function from the main gui thread.
  //////////////////////////////////////////////////////////////////////////////
  static void pumpAsyncMessages();

private:

  //////////////////////////////////////////////////////////////////////////////
  // LoggingSystem::myMessageOutput()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief    Message handler for Qt's debug messages.
  ///\param    [in] type:    The type of the message (debug, warning etc).
  ///\param    [in] context: Call context (source file and line etc).
  ///\param    [in] msg:     The actual message.
  ///\remarks: This function is set via qInstallMessageHandler(). The old
  ///          handler is called before doing anything else.
  //////////////////////////////////////////////////////////////////////////////
  #if QT_VERSION >= 0x050000
  static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
  #else
  static void myMessageOutput(QtMsgType type, const char* msg);
  #endif

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  static QString          m_logFileName;    ///> Name of the target log file.
  #if QT_VERSION >= 0x050000
  static QtMessageHandler m_oldHandler;     ///> Old debug message handler.
  #else
  static QtMsgHandler     m_oldHandler;     ///> Old debug message handler.
  #endif
  static QStringList      m_fileMessages;   ///> Cache for file messages until the system runs.
  static QStringList      m_editMessages;   ///> Cache for editor messages until the system runs.
  static QTextEdit*       m_textEdit;       ///> The target text box.
  static QMutex           m_mutex;          ///> Mutex for exclusive access.
};

#endif // #ifndef __LOGGINGSYSTEM_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
