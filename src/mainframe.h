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

protected:
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
  // MainFrame::exitApplication()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the Exit-Application signal.
  ///\remarks Closes the window and quits the application.
  //////////////////////////////////////////////////////////////////////////////
  void exitApplication();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::about()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the Help->About signal.
  ///\remarks Shows an about box with informations about this application.
  //////////////////////////////////////////////////////////////////////////////
  void about();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::goHome()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the Help->Go home signal.
  ///\remarks Opens the system's default browser and navigates to this project's
  ///         Home page.
  //////////////////////////////////////////////////////////////////////////////
  void goHome();

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
  // MainFrame::undo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the undo action triggered signal.
  ///\remarks This one is the handler for the menu and button clicks. It calls
  ///         the undo command of the current document's undo manager and this
  ///         will in turn update the UI (history window, enabled states etc).
  //////////////////////////////////////////////////////////////////////////////
  void undo();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::redo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the undo action triggered signal.
  ///\remarks This one is the handler for the menu and button clicks. It calls
  ///         the redo command of the current document's undo manager and this
  ///         will in turn update the UI (history window, enabled states etc).
  //////////////////////////////////////////////////////////////////////////////
  void redo();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::clearUndo()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the clearUndo action triggered signal.
  ///\remarks This one clears the memory of the current document's undo system.
  //////////////////////////////////////////////////////////////////////////////
  void clearUndo();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::canUndoChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's canUndo state changed signal.
  ///\param   [in] state: New state of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void canUndoChanged(bool state);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::canRedoChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's canRedo state changed signal.
  ///\param   [in] state: New state of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void canRedoChanged(bool state);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::undoTextChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's undo text changed signal.
  ///\param   [in] newString: New text of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void undoTextChanged(QString newString);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::redoTextChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the current document's redo text changed signal.
  ///\param   [in] newString: New text of the property.
  ///\remarks Updates the state of the related UI elements.
  //////////////////////////////////////////////////////////////////////////////
  void redoTextChanged(QString newString);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::documentClosed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the document's closed signal.
  ///\remarks This is called whenever a document was closed. Closes open views,
  ///         updates related actions etc.
  //////////////////////////////////////////////////////////////////////////////
  void documentClosed();

  void documentDirtyChanged();
  void selectionChanged();
  void clipboardChanged(QClipboard::Mode mode);
  void newDocument();
  void newFromClipboard();
  void openDocument();
  void openRecentFile();
  void showMoreRecentFiles();
  void clearRecentFiles();
  void saveDocument();
  void saveDocumentAs();
  void saveAllDocuments();
  void closeDocument();
  void closeAllDocuments();
  void showStats();
  void printStats();
  void printPreview();
  void cut();
  void copy();
  void paste();
  void deleteAction();
  void selectAll();
  void selectNothing();
  void extendSelectionToStart();
  void extendSelectionToEnd();
  void extendSelectionToPreviousMarker();
  void extendSelectionToNextMarker();
  void extendSelectionToCursor();
  void extendSelectionToAllChannels();
  void extendSelectionDoubleLength();
  void extendSelectionHalfLength();
  void selectStartToCursor();
  void selectCursorToEnd();
  void selectCursorToPrevMarker();
  void selectCursorToNextMarker();
  void selectCustom();
  void selectDocument();
  void showMoreDocuments();
  void subWindowActivated(QMdiSubWindow* window);
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
  void configureShortcuts();
  void configureToolbars();
  void toggleThemeDark();
  void toggleThemeDefault();
  void zoomAll();
  void zoomSelection();
  void zoomInHorizontally();
  void zoomOutHorizontally();
  void zoomInVertically();
  void zoomOutVertically();

private:

  WaveMDIWindow* findMDIWindow(Document* doc);

  void loadFile(QString fileName);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::updateUndoState()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Update the undo/redo related actions (and thus the menu).
  //////////////////////////////////////////////////////////////////////////////
  void updateUndoState();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::updateDocumentMenu()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Update the 'view->document' related actions (and thus the menu).
  //////////////////////////////////////////////////////////////////////////////
  void updateDocumentMenu();

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::addRecentFile()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Add a new entry to the recent file list.
  ///\param   [in] fileName: New file to add.
  ///\remarks This will call 'updateRecentFiles()' to update the UI.
  //////////////////////////////////////////////////////////////////////////////
  void addRecentFile(const QString& fileName);

  //////////////////////////////////////////////////////////////////////////////
  // MainFrame::updateRecentFiles()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Update the 'recent files' related actions (and thus the menu).
  //////////////////////////////////////////////////////////////////////////////
  void updateRecentFiles();

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
  QStringList               m_recentFiles;    ///> The recently used files.
  DocumentManager*          m_docManager;     ///> The document manager.
  QMdiArea*                 m_mdiArea;        ///> The main edit area.
  QMenu*                    m_toolbarMenu;    ///> The toolbar sub menu.
  QMenu*                    m_toolWindowMenu; ///> The window sub menu.
};

#endif // __MAINFRAME_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
