////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    mainframe.h
///\ingroup bruo
///\brief   Main window class definition.
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
#ifndef __MAINFRAME_H_INCLUDED__
#define __MAINFRAME_H_INCLUDED__

#include "documentmanager.h"
#include "controls/wavemdiwindow.h"

////////////////////////////////////////////////////////////////////////////////
///\class MainFrame mainframe.h
///\brief Main window class.
/// This is the main widget of the application. It holds the menu, status bar,
/// and all the views.
////////////////////////////////////////////////////////////////////////////////
class MainFrame :
  public QMainWindow
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::MainFrame()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  MainFrame(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::~MainFrame()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Choose! Choose the form of the Destructor!
  //////////////////////////////////////////////////////////////////////////////
  ~MainFrame();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::manager()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Accessor for the document manager of this application.
  ///\return  The application's document manager.
  ///\remarks The document manager controls the active document, loading etc.
  //////////////////////////////////////////////////////////////////////////////
  DocumentManager* manager();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::manager()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Accessor for the document manager of this application.
  ///\return  The application's document manager.
  ///\remarks The document manager controls the active document, loading etc.
  //////////////////////////////////////////////////////////////////////////////
  const DocumentManager* manager() const;

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::findMDIWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Find the MDI child for a document.
  ///\param   [in] doc: The document to find.
  ///\return  The MDI window or 0 on error.
  //////////////////////////////////////////////////////////////////////////////
  WaveMDIWindow* findMDIWindow(Document* doc);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::loadFile()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create a new document and load a file into it.
  ///\param   [in] fileName: The file to load.
  //////////////////////////////////////////////////////////////////////////////
  void loadFile(QString fileName);

protected:
  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::showEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Message handler for the window show event.
  ///\param   [in] e: Description of the event.
  ///\remarks Initializes the audio engine.
  //////////////////////////////////////////////////////////////////////////////
  void showEvent(QShowEvent* e);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::closeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Message handler for the window close event.
  ///\param   [in] e: Description of the event.
  ///\remarks To allow the window to close, call e->accept().
  //////////////////////////////////////////////////////////////////////////////
  void closeEvent(QCloseEvent* e);

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::idleEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the idle timer signal.
  ///\remarks This one is called whenever there are no more events in the
  ///         message loop.
  //////////////////////////////////////////////////////////////////////////////
  void idleEvent();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::activeDocumentChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document manager's activeDocumentChanged signal.
  ///\remarks This one is called whenever a new document was made active. The
  ///         current document may be null if there is no document open.
  //////////////////////////////////////////////////////////////////////////////
  void activeDocumentChanged();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::documentCreated()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document manager's documentCreated signal.
  ///\remarks This one is called whenever a new document was created. At this
  ///         stage the document is still empty but this handler is good place
  ///         to attach per document signal handlers etc.
  //////////////////////////////////////////////////////////////////////////////
  void documentCreated(Document* doc);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::documentClosed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document's closed signal.
  ///\remarks This is called whenever a document was closed. Closes open views,
  ///         updates related actions etc.
  //////////////////////////////////////////////////////////////////////////////
  void documentClosed();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::documentDirtyChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document's dirty changed signal.
  ///\remarks This is called whenever a document was modified.
  //////////////////////////////////////////////////////////////////////////////
  void documentDirtyChanged();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::subWindowActivated()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the active MDI window changed signal.
  ///\remarks This is called whenever a new MDI child gets activated.
  //////////////////////////////////////////////////////////////////////////////
  void subWindowActivated(QMdiSubWindow* window);

  void selectStartToCursor();
  void selectCursorToEnd();
  void selectCursorToPrevMarker();
  void selectCursorToNextMarker();
  void selectCustom();
  void goToStart();
  void seekBackward();
  void startPlayback();
  void stopPlayback();
  void seekForward();
  void goToEnd();
  void record();
  void loop();
  void goToPreviousMarker();
  void goToNextMarker();

  void selectDocument();
  void showMoreDocuments();

  void configureShortcuts();
  void configureToolbars();
  void toggleThemeDark();
  void toggleThemeDefault();

  void zoomAll();
  void zoomInHorizontally();
  void zoomOutHorizontally();
  void zoomInVertically();
  void zoomOutVertically();

private:

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::updateDocumentMenu()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Update the 'view->document' related actions (and thus the menu).
  //////////////////////////////////////////////////////////////////////////////
  void updateDocumentMenu();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::createActions()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper to create the internal application actions.
  //////////////////////////////////////////////////////////////////////////////
  void createActions();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::createMainMenu()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper to create the main menu of the application window.
  //////////////////////////////////////////////////////////////////////////////
  void createMainMenu();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::createToolbars()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper to create the toolbars of the application.
  //////////////////////////////////////////////////////////////////////////////
  void createToolbars();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::createStatusBar()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper to create the status bar of the application window.
  //////////////////////////////////////////////////////////////////////////////
  void createStatusBar();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::createToolWindows()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper to create the docked tool windows.
  //////////////////////////////////////////////////////////////////////////////
  void createToolWindows();

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QHash<QString, QAction*>  m_actionMap;      ///> All actions.
  DocumentManager*          m_docManager;     ///> The document manager.
  QMdiArea*                 m_mdiArea;        ///> The main edit area.
  QMenu*                    m_toolbarMenu;    ///> The toolbar sub menu.
  QMenu*                    m_toolWindowMenu; ///> The window sub menu.
  QMenu*                    m_themeMenu;      ///> The theme sub menu.
  QTimer*                   m_idleTimer;      ///> Idle timer.
  class AudioSystemQt*      m_audioSystem;    ///> The audio IO system.
};

#endif // __MAINFRAME_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
