////////////////////////////////////////////////////////////////////////////////
// (c) 2013 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    mainframe.cpp
///\ingroup bruo
///\brief   Main window class implementation.
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
#include "mainframe.h"
#include "toolwindows/historytoolwindow.h"
#include "toolwindows/debugtoolwindow.h"
#include "toolwindows/browsertoolwindow.h"
#include "controls/stringselectdialog.h"
#include "settings/shortcutdialog.h"
#include "commands/selectcommand.h"
#include "commands/clearselectioncommand.h"
#include "audio/audiosystemqt.h"
#include "audio/audiosystem.h"
#include "settings/loggingsystem.h"
#include "actions/actions.h"

////////////////////////////////////////////////////////////////////////////////
// MainFrame::MainFrame()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
MainFrame::MainFrame(QWidget* parent) :
  QMainWindow(parent),
  m_docManager(0),
  m_audioSystem(0)
{
  // Create document manager:
  m_docManager = new DocumentManager(this);
  connect(m_docManager, SIGNAL(activeDocumentChanged()), this, SLOT(activeDocumentChanged()));
  connect(m_docManager, SIGNAL(documentCreated(Document*)), this, SLOT(documentCreated(Document*)));

  // Init title and icon:
  setWindowTitle(tr("bruo"));
  setWindowIcon(QIcon(":/images/icon128.png"));
  setWindowIconText(tr("bruo"));

  // Set minimum size:
  setMinimumSize(QSize(640, 480));

  // Setup basic UI:
  createActions();
  createMainMenu();
  createToolbars();
  createStatusBar();
  createToolWindows();

  // Create the main wave view:
  m_mdiArea = new QMdiArea(this);
  m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_mdiArea->setViewMode(QMdiArea::TabbedView);
  m_mdiArea->setTabsClosable(true);
  m_mdiArea->setTabsMovable(true);

  // Adjust tab appearance:
  QTabBar* tabBar = m_mdiArea->findChild<QTabBar*>();
  if (tabBar != 0)
  {
    tabBar->setExpanding(false);
    tabBar->setMinimumWidth(10);
  }

  // Set MDI area as main widget:
  setCentralWidget(m_mdiArea);
  connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowActivated(QMdiSubWindow*)));
  connect(m_actionMap["selectNextDocument"], SIGNAL(triggered()), m_mdiArea, SLOT(activateNextSubWindow()));
  connect(m_actionMap["selectPreviousDocument"], SIGNAL(triggered()), m_mdiArea, SLOT(activatePreviousSubWindow()));

  // Load settings:
  QSettings settings;

  // Load window position:
  if (settings.contains("mainwindow/geometry"))
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
  else
  {
    // Init size and position (screen center):
    int w = 800;
    int h = 600;
    int x = (QApplication::desktop()->width()  / 2) - (w / 2);
    int y = (QApplication::desktop()->height() / 2) - (h / 2);

    // Place window:
    setGeometry(x, y, w, h);
  }

  // Restore toolbars etc:
  if (settings.contains("mainwindow/windowstate"))
    restoreState(settings.value("mainwindow/windowstate").toByteArray());

  // Init recent files:
  m_docManager->emitRecentFilesChanged();

  // Init window menu:
  updateDocumentMenu();

  // Init active states:
  m_docManager->emitActiveDocumentChanged();

  // Load user keys:
  if (!settings.fileName().isEmpty())
  {
    // Compose key file name:
    QString fileName = getSettingsPath() + "user.keymap";
    if (QFile::exists(fileName))
    {
      // Load the file:
      Keymap keys;
      if (keys.load(fileName))
      {
        // Assign keys:
        for (QHash<QString, QAction*>::iterator i = m_actionMap.begin(); i != m_actionMap.end(); i++)
        {
          if (keys.hasKey(i.key()))
            (*i)->setShortcut(keys.key(i.key()));
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::~MainFrame()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Choose! Choose the form of the Destructor!
////////////////////////////////////////////////////////////////////////////////
MainFrame::~MainFrame()
{
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::manager()
////////////////////////////////////////////////////////////////////////////////
///\brief   Accessor for the document manager of this application.
///\return  The application's document manager.
///\remarks The document manager controls the active document, loading etc.
////////////////////////////////////////////////////////////////////////////////
DocumentManager* MainFrame::manager()
{
  // Return our manager:
  return m_docManager;
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::manager()
////////////////////////////////////////////////////////////////////////////////
///\brief   Accessor for the document manager of this application.
///\return  The application's document manager.
///\remarks The document manager controls the active document, loading etc.
////////////////////////////////////////////////////////////////////////////////
const DocumentManager* MainFrame::manager() const
{
  // Return our manager:
  return m_docManager;
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::findMDIWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Find the MDI child for a document.
///\param   [in] doc: The document to find.
///\return  The MDI window or 0 on error.
////////////////////////////////////////////////////////////////////////////////
WaveMDIWindow* MainFrame::findMDIWindow(Document* doc)
{
  // Loop through the MDI view's sub windows:
  QList<QMdiSubWindow*> subWindows = m_mdiArea->subWindowList();
  for (int i = 0; i < subWindows.length(); i++)
  {
    // Get embedded wave view:
    WaveMDIWindow* view = qobject_cast<WaveMDIWindow*>(subWindows.at(i));
    if (view != 0)
    {
      // Is this the requested view?
      if (view->document() == doc)
        return view;
    }
  }

  // Nothing found:
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::loadFile()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create a new document and load a file into it.
///\param   [in] fileName: The file to load.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::loadFile(QString fileName)
{
  AudioSuspenderQt suspender(m_audioSystem);

  // Let's see if we already have loaded this file:
  for (int i = 0; i < m_docManager->documents().length(); i++)
  {
    // Compare file names:
    if (fileName == m_docManager->documents().at(i)->fileName())
    {
      // Just activate the file:
      m_docManager->setActiveDocument(i);
      return;
    }
  }

  // Create new document:
  Document* doc = m_docManager->newDocument();

  // Load the file:
  if (doc->loadFile(fileName))
  {
    // Create a new mdi view for this document:
    WaveMDIWindow* subWindow = new WaveMDIWindow(doc);
    m_mdiArea->addSubWindow(subWindow);

    // Show it (this three calls are needed):
    subWindow->show();
    subWindow->activateWindow();
    subWindow->raise();

    // Set active:
    m_docManager->setActiveDocument(doc);

    // Force an activation event on the first document:
    if (m_docManager->documents().length() == 1)
      m_docManager->emitActiveDocumentChanged();

    // Move file to the top of the recent file list:
    m_docManager->addRecentFile(fileName);

    // Activate rack:
    doc->rack().resume();
  }
  else
  {
    // Show the failure reason:
    QMessageBox::critical(this, tr("File open error"), doc->lastError());

    // Cleanup document:
    doc->close();
    doc->emitClosed();
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::showEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the window show event.
///\param   [in] e: Description of the event.
///\remarks Initializes the audio engine.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::showEvent(QShowEvent* /*e*/)
{
  // Create idle timer:
  m_idleTimer = new QTimer(this);
  m_idleTimer->setSingleShot(false);
  connect(m_idleTimer, SIGNAL(timeout()), this, SLOT(idleEvent()));
  m_idleTimer->start(100);

  // Init audio system:
  m_audioSystem->initialize(m_docManager);
  //while (!m_audioSystem->probeCurrentDevice())
  {
    // Show dialog
  }
  m_audioSystem->start();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::closeEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the window close event.
///\param   [in] e: Description of the event.
///\remarks To allow the window to close, call e->accept().
////////////////////////////////////////////////////////////////////////////////
void MainFrame::closeEvent(QCloseEvent* e)
{
  // Check for unsaved documents etc:
  if (!m_docManager->closeAllDocuments())
    return;

  // Stop audio:
  m_audioSystem->stop();
  m_audioSystem->finalize();
  delete m_audioSystem;
  m_audioSystem = 0;

  // Stop idle timer:
  m_idleTimer->stop();

  // Save window position:
  QSettings settings;
  settings.setValue("mainwindow/geometry", saveGeometry());

  // Save toolbars etc:
  settings.setValue("mainwindow/windowstate", saveState());

  // Manager cleanup:
  m_docManager->cleanup();

  // Detach log window:
  LoggingSystem::setOutputWindow(0);

  // allow closing:
  e->accept();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::idleEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the idle timer signal.
///\remarks This one is called whenever there are no more events in the
///         message loop.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::idleEvent()
{
  // Enabled?
  if (!isEnabled())
      return;

  // Update logs:
  LoggingSystem::pumpAsyncMessages();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::activeDocumentChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's activeDocumentChanged signal.
///\remarks This one is called whenever a new document was made active. The
///         current document may be null if there is no document open.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::activeDocumentChanged()
{
  // Find currently active mdi window:
  QMdiSubWindow* activeSubWindow = m_mdiArea->activeSubWindow();

  // Find the document's MDI view:
  QMdiSubWindow* subWindow = findMDIWindow(m_docManager->activeDocument());
  if (subWindow != activeSubWindow)
    m_mdiArea->setActiveSubWindow(subWindow);

  // Update window and undo menu:
  updateDocumentMenu();

  // Get document and selection state:
  Document* doc = m_docManager->activeDocument();

  m_actionMap["selectAll"]->setEnabled(doc != 0);
  
  m_actionMap["selectStartToCursor"]->setEnabled(doc != 0);
  m_actionMap["selectCursorToEnd"]->setEnabled(doc != 0);
  m_actionMap["selectCursorToPrevMarker"]->setEnabled(doc != 0);
  m_actionMap["selectCursorToNextMarker"]->setEnabled(doc != 0);
  m_actionMap["selectCustom"]->setEnabled(doc != 0);
  m_actionMap["goToStart"]->setEnabled(doc != 0);
  m_actionMap["seekBackward"]->setEnabled(doc != 0);
  m_actionMap["startPlayback"]->setEnabled(doc != 0);
  m_actionMap["stopPlayback"]->setEnabled(doc != 0);
  m_actionMap["seekForward"]->setEnabled(doc != 0);
  m_actionMap["goToEnd"]->setEnabled(doc != 0);
  m_actionMap["loop"]->setEnabled(doc != 0);
  m_actionMap["goToPreviousMarker"]->setEnabled(doc != 0);
  m_actionMap["goToNextMarker"]->setEnabled(doc != 0);

  m_actionMap["zoomAll"]->setEnabled(doc != 0);
  m_actionMap["zoomInHorizontally"]->setEnabled(doc != 0);
  m_actionMap["zoomOutHorizontally"]->setEnabled(doc != 0);
  m_actionMap["zoomInVertically"]->setEnabled(doc != 0);
  m_actionMap["zoomOutVertically"]->setEnabled(doc != 0);

  m_actionMap["loop"]->setChecked(doc != 0 && doc->looping());
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::documentCreated()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document manager's documentCreated signal.
///\remarks This one is called whenever a new document was created. At this
///         stage the document is still empty but this handler is good place to
///         attach per document signal handlers etc.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::documentCreated(Document* doc)
{
  // Connect document signal handler:
  connect(doc, SIGNAL(closed()),       this, SLOT(documentClosed()));
  connect(doc, SIGNAL(dirtyChanged()), this, SLOT(documentDirtyChanged()));
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::documentClosed()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document's closed signal.
///\remarks This is called whenever a document was closed. Closes open views,
///         updates related actions etc.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::documentClosed()
{
  // Get document:
  Document* doc = qobject_cast<Document*>(sender());
  if (doc == 0)
    return;

  // Detach and close the view:
  WaveMDIWindow* view = findMDIWindow(doc);
  if (view != 0)
  {
    view->setDocument(0);
    view->close();
  }

  // Update the menu:
  updateDocumentMenu();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::documentDirtyChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the document's dirty changed signal.
///\remarks This is called whenever a document was modified.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::documentDirtyChanged()
{
  // Get document:
  Document* doc = qobject_cast<Document*>(sender());
  if (doc == 0)
    return;

  // Update window title:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow != 0)
    subWindow->setWindowTitle(doc->composeTitle());

  // Update the view menu:
  updateDocumentMenu();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::subWindowActivated()
///////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the active MDI window changed signal.
///\remarks This is called whenever a new MDI child gets activated.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::subWindowActivated(QMdiSubWindow* window)
{
  // Got a window?
  if (window == 0)
    return;

  // Get active wave view:
  WaveMDIWindow* view = qobject_cast<WaveMDIWindow*>(window);
  if (view == 0)
    return;

  // Maximize this window:
  view->setWindowState(view->windowState() | Qt::WindowMaximized);

  // Set new active document if needed:
  if (view->document() != m_docManager->activeDocument())
  {
    // Stop playing of old document:
    if (m_docManager->activeDocument())
      m_docManager->activeDocument()->setPlaying(false);

    AudioSuspenderQt suspender(m_audioSystem);
    m_docManager->setActiveDocument(view->document());
  }
}

void MainFrame::selectStartToCursor()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->cursorPosition() == 0)
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, 0, doc->cursorPosition());
  doc->undoStack()->push(cmd);
}

void MainFrame::selectCursorToEnd()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->cursorPosition() >= doc->sampleCount())
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->cursorPosition(), doc->sampleCount() - doc->cursorPosition());
  doc->undoStack()->push(cmd);
}

void MainFrame::selectCursorToPrevMarker()
{
}

void MainFrame::selectCursorToNextMarker()
{
}

void MainFrame::selectCustom()
{
}

void MainFrame::selectDocument()
{
  // Get source action:
  QAction* action = qobject_cast<QAction*>(sender());
  if (action != 0)
  {
    // Select the document:
    m_docManager->setActiveDocument(action->data().toInt());
  }
}

void MainFrame::showMoreDocuments()
{
  // Build a string list with the title of all open documents:
  QStringList list;
  for (int i = 0; i < m_docManager->documents().length(); i++)
    list.append(m_docManager->documents().at(i)->composeTitle());

  // Create and init the dialog:
  StringSelectDialog dialog(list, this);
  dialog.setWindowTitle(tr("Select File"));

  // Show it:
  if (dialog.exec() == QDialog::Accepted)
  {
    // Activate the selected document:
    int index = dialog.selectedItem();
    if (index >= 0 && index < m_docManager->documents().length())
      m_docManager->setActiveDocument(index);
  }
}

void MainFrame::goToStart()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Update position:
  doc->setCursorPosition(0);
}

void MainFrame::seekBackward()
{
}

void MainFrame::startPlayback()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Update play state:
  doc->setPlaying(true);
}

void MainFrame::stopPlayback()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Update play state:
  doc->setPlaying(false);
}

void MainFrame::seekForward()
{
}

void MainFrame::goToEnd()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Update play state:
  doc->setCursorPosition(doc->sampleCount());
}

void MainFrame::record()
{
}

void MainFrame::loop()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Update loop state:
  doc->setLooping(!doc->looping());

  // Update UI:
  m_actionMap["loop"]->setChecked(doc->looping());
}

void MainFrame::goToPreviousMarker()
{
}

void MainFrame::goToNextMarker()
{
}

void MainFrame::configureShortcuts()
{
  // Create and show the dialog:
  ShortcutDialog dlg(m_actionMap, this);
  if (dlg.exec() == QDialog::Accepted)
  {
    // Create key map:
    Keymap keys;
    keys.setName("Usermap");
    for (QHash<QString, QAction*>::iterator i = m_actionMap.begin(); i != m_actionMap.end(); i++)
      keys.setKey(i.key(), (*i)->shortcut());

    // Compose key file name:
    QString fileName = getSettingsPath() + "user.keymap";

    // Save the file:
    keys.save(fileName);
  }
}

void MainFrame::configureToolbars()
{
}

void MainFrame::toggleThemeDark()
{
  // Update UI:
  m_actionMap["defaultTheme"]->setChecked(false);
  m_actionMap["darkTheme"]->setChecked(true);

  // Anything to do?
  if (QSettings().value("darkTheme", QVariant(false)).toBool())
    return;

  // Switch theme:
  toggleDarkTheme(true);

  // Update value:
  QSettings().setValue("darkTheme", QVariant(true));
}

void MainFrame::toggleThemeDefault()
{
  // Update UI:
  m_actionMap["defaultTheme"]->setChecked(true);
  m_actionMap["darkTheme"]->setChecked(false);

  // Anything to do?
  if (!QSettings().value("darkTheme", QVariant(false)).toBool())
    return;

  // Switch theme:
  toggleDarkTheme(false);

  // Update value:
  QSettings().setValue("darkTheme", QVariant(false));
}

void MainFrame::zoomAll()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Get matching mdi window:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow == 0)
    return;

  // Update the zoom state:
  subWindow->zoomAll();
}

void MainFrame::zoomInHorizontally()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Get matching mdi window:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow == 0)
    return;

  // Update the zoom state:
  subWindow->zoomIn(false);
}

void MainFrame::zoomOutHorizontally()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Get matching mdi window:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow == 0)
    return;

  // Update the zoom state:
  subWindow->zoomOut(false);
}

void MainFrame::zoomInVertically()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Get matching mdi window:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow == 0)
    return;

  // Update the zoom state:
  subWindow->zoomIn(true);
}

void MainFrame::zoomOutVertically()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Get matching mdi window:
  WaveMDIWindow* subWindow = findMDIWindow(doc);
  if (subWindow == 0)
    return;

  // Update the zoom state:
  subWindow->zoomOut(true);
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::updateDocumentMenu()
////////////////////////////////////////////////////////////////////////////////
///\brief Update the 'view->document' related actions (and thus the menu).
////////////////////////////////////////////////////////////////////////////////
void MainFrame::updateDocumentMenu()
{
  // Loop through menu entries:
  for (int i = 0; i < 10; ++i)
  {
    // Get action:
    QAction* action = m_actionMap[QString("selectDocument%1").arg(i)];

    // Is this a valid entry?:
    if (i >= m_docManager->documents().length())
    {
      // No entry for this action, deactivate it:
      action->setVisible(false);
    }
    else
    {
      // Show the item:
      action->setVisible(true);

      // Get short file name:
      QString shortPath = m_docManager->documents().at(i)->composeTitle();

      // Update action text:
      QString text = i == 9 ? tr("1&0: %1") : tr("&%1: ").arg(i + 1);
      text.append(shortPath);
      action->setText(text);

      // Update status bar entry:
      action->setStatusTip(tr("Switch to %1").arg(shortPath));
    }
  }

  // Enable/disable related items:
  m_actionMap["showMoreDocuments"]->setEnabled(m_docManager->documents().count() > 10);
  m_actionMap["selectNextDocument"]->setEnabled(m_docManager->documents().count() > 1);
  m_actionMap["selectPreviousDocument"]->setEnabled(m_docManager->documents().count() > 1);
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::createActions()
////////////////////////////////////////////////////////////////////////////////
///\brief   Internal helper to create the internal application actions.
///\remarks The theme icons will only work under Linux but it is good practice
///         to use them. This will provide a consistant user experience across
///         different applications. The icon names used here can be fopenRecentFileound at:
///         http://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html
////////////////////////////////////////////////////////////////////////////////
void MainFrame::createActions()
{
  QAction* action;

  m_actionMap["newDocument"] = new NewDocumentAction(this);
  m_actionMap["newFromClipboard"] = new NewFromClipboardAction(this);
  m_actionMap["openDocument"] = new OpenDocumentAction(this);
  for (int i = 0; i < 10; ++i)
    m_actionMap[QString("openRecentFile%1").arg(i)] = new OpenRecentFileAction(this, i);
  m_actionMap["noRecentFiles"] = new NoRecentFilesAction(this);
  m_actionMap["showMoreRecentFiles"] = new ShowMoreRecentFilesAction(this);
  m_actionMap["clearRecentFiles"] = new ClearRecentFilesAction(this);
  m_actionMap["saveDocument"] = new SaveDocumentAction(this);
  m_actionMap["saveDocumentAs"] = new SaveDocumentAsAction(this);
  m_actionMap["saveAllDocuments"] = new SaveAllDocumentsAction(this);
  m_actionMap["closeDocument"] = new CloseDocumentAction(this);
  m_actionMap["closeAllDocuments"] = new CloseAllDocumentsAction(this);
  m_actionMap["showStats"] = new ShowStatsAction(this);
  m_actionMap["printStats"] = new PrintStatsAction(this);
  m_actionMap["printPreview"] = new PrintPreviewAction(this);
  m_actionMap["exit"] = new ExitAction(this);
  m_actionMap["undo"] = new UndoAction(this);
  m_actionMap["redo"] = new RedoAction(this);
  m_actionMap["clearUndo"] = new ClearUndoAction(this);
  m_actionMap["cut"] = new CutAction(this);
  m_actionMap["copy"] = new CopyAction(this);
  m_actionMap["paste"] = new PasteAction(this);
  m_actionMap["delete"] = new DeleteAction(this);
  m_actionMap["selectAll"] = new SelectAllAction(this);
  m_actionMap["selectNothing"] = new SelectNothingAction(this);
  m_actionMap["extendSelectionToStart"] = new ExtendSelectionToStartAction(this);
  m_actionMap["extendSelectionToEnd"] = new ExtendSelectionToEndAction(this);
  m_actionMap["extendSelectionToCursor"] = new ExtendSelectionToCursorAction(this);
  m_actionMap["extendSelectionToPreviousMarker"] = new ExtendSelectionToPreviousMarkerAction(this);
  m_actionMap["extendSelectionToNextMarker"] = new ExtendSelectionToNextMarkerAction(this);
  m_actionMap["extendSelectionToAllChannels"] = new ExtendSelectionToAllChannelsAction(this);
  m_actionMap["extendSelectionDoubleLength"] = new ExtendSelectionDoubleLengthAction(this);
  m_actionMap["shrinkSelectionHalfLength"] = new ShrinkSelectionHalfLengthAction(this);

  // Edit->select from start to cursor:
  action = new QAction(QIcon(":/images/edit-select-cursor-start.png"), tr("Select from start to cursor"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Home));
  action->setStatusTip(tr("Select from start to the play cursor"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectStartToCursor()));
  m_actionMap["selectStartToCursor"] = action;

  // Edit->select from cursor to end:
  action = new QAction(QIcon(":/images/edit-select-cursor-end.png"), tr("Select from cursor to end"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_End));
  action->setStatusTip(tr("Select from play cursor to the end"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectCursorToEnd()));
  m_actionMap["selectCursorToEnd"] = action;

  // Edit->select from cursor to previous marker:
  action = new QAction(QIcon(":/images/edit-select-cursor-prev-marker.png"), tr("Select from cursor to previous marker"), this);
  action->setStatusTip(tr("Select from the play cursor to the previous marker"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectCursorToPrevMarker()));
  m_actionMap["selectCursorToPrevMarker"] = action;

  // Edit->select from cursor to next marker:
  action = new QAction(QIcon(":/images/edit-select-cursor-next-marker.png"), tr("Select from cursor to next marker"), this);
  action->setStatusTip(tr("Select from the play cursor to the next marker"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectCursorToNextMarker()));
  m_actionMap["selectCursorToNextMarker"] = action;

  // Edit->custom selection:
  action = new QAction(QIcon(":/images/edit-select-custom.png"), tr("Custom..."), this);
  action->setStatusTip(tr("Select by entering parameters"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectCustom()));
  m_actionMap["selectCustom"] = action;

  m_actionMap["settings"] = new SettingsAction(this);

  // Transport->Go to start:
  action = new QAction(QIcon(":/images/media-skip-backward.png"), tr("&Go to start"), this);
  action->setShortcut(QKeySequence(Qt::Key_Home));
  action->setStatusTip(tr("Go to start of the file"));
  connect(action, SIGNAL(triggered()), this, SLOT(goToStart()));
  m_actionMap["goToStart"] = action;

  // Transport->Seek backward:
  action = new QAction(QIcon(":/images/media-seek-backward.png"), tr("&Seek backwards"), this);
  action->setStatusTip(tr("Seek backward"));
  connect(action, SIGNAL(triggered()), this, SLOT(seekBackward()));
  m_actionMap["seekBackward"] = action;

  // Transport->Play:
  action = new QAction(QIcon(":/images/media-playback-start.png"), tr("&Play"), this);
  action->setStatusTip(tr("Start playback"));
  connect(action, SIGNAL(triggered()), this, SLOT(startPlayback()));
  m_actionMap["startPlayback"] = action;

  // Transport->Stop:
  action = new QAction(QIcon(":/images/media-playback-stop.png"), tr("S&top"), this);
  action->setStatusTip(tr("Stop playback"));
  connect(action, SIGNAL(triggered()), this, SLOT(stopPlayback()));
  m_actionMap["stopPlayback"] = action;

  // Transport->Seek forward:
  action = new QAction(QIcon(":/images/media-seek-forward.png"), tr("Seek &forward"), this);
  action->setStatusTip(tr("Seek forward"));
  connect(action, SIGNAL(triggered()), this, SLOT(seekForward()));
  m_actionMap["seekForward"] = action;

  // Transport->Go to end:
  action = new QAction(QIcon(":/images/media-skip-forward.png"), tr("Go to &end"), this);
  action->setShortcut(QKeySequence(Qt::Key_End));
  action->setStatusTip(tr("Go to the end of the file"));
  connect(action, SIGNAL(triggered()), this, SLOT(goToEnd()));
  m_actionMap["goToEnd"] = action;

  // Transport->Record:
  action = new QAction(QIcon(":/images/media-record.png"), tr("&Record"), this);
  action->setStatusTip(tr("Record a file"));
  connect(action, SIGNAL(triggered()), this, SLOT(record()));
  m_actionMap["record"] = action;

  // Transport->Loop:
  action = new QAction(QIcon(":/images/media-loop.png"), tr("&Loop"), this);
  action->setStatusTip(tr("Loop on/off"));
  action->setCheckable(true);
  connect(action, SIGNAL(triggered()), this, SLOT(loop()));
  m_actionMap["loop"] = action;

  // Transport->Go to previous marker:
  action = new QAction(QIcon(":/images/go-to-prev-marker.png"), tr("Go to previous mar&ker"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
  action->setStatusTip(tr("Go to the previous marker."));
  connect(action, SIGNAL(triggered()), this, SLOT(goToPreviousMarker()));
  m_actionMap["goToPreviousMarker"] = action;

  // Transport->Go to next marker:
  action = new QAction(QIcon(":/images/go-to-next-marker.png"), tr("Go to next &marker"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
  action->setStatusTip(tr("Go to the previous marker."));
  connect(action, SIGNAL(triggered()), this, SLOT(goToNextMarker()));
  m_actionMap["goToNextMarker"] = action;

  // Tools->Configure shortcuts:
  action = new QAction(QIcon(":/images/configure-shortcuts.png"), tr("&Configure shortcuts..."), this);
  action->setStatusTip(tr("Configure keyboard shortcuts"));
  connect(action, SIGNAL(triggered()), this, SLOT(configureShortcuts()));
  m_actionMap["configureShortcuts"] = action;

  // Tools->Configure toolbars:
  action = new QAction(QIcon(":/images/configure-toolbars.png"), tr("Confi&gure toolbars..."), this);
  action->setStatusTip(tr("Configure toolbars"));
  connect(action, SIGNAL(triggered()), this, SLOT(configureToolbars()));
  m_actionMap["configureToolbars"] = action;

  // View->Theme->Default:
  action = new QAction(QIcon(":/images/theme-default.png"), tr("Default"), this);
  action->setStatusTip(tr("Switch to default theme"));
  action->setCheckable(true);
  action->setChecked(!QSettings().value("darkTheme", QVariant(false)).toBool());
  connect(action, SIGNAL(triggered()), this, SLOT(toggleThemeDefault()));
  m_actionMap["defaultTheme"] = action;

  // View->Theme->Dark:
  action = new QAction(QIcon(":/images/theme-dark.png"), tr("Dark"), this);
  action->setStatusTip(tr("Switch to dark theme"));
  action->setCheckable(true);
  action->setChecked(QSettings().value("darkTheme", QVariant(false)).toBool());
  connect(action, SIGNAL(triggered()), this, SLOT(toggleThemeDark()));
  m_actionMap["darkTheme"] = action;

  // View->Next document:
  action = new QAction(QIcon(":/images/arrow-right.png"), tr("&Next"), this);
  action->setShortcuts(QKeySequence::NextChild);
  action->setStatusTip(tr("Got to next document"));
  m_actionMap["selectNextDocument"] = action;

  // View->Previous document:
  action = new QAction(QIcon(":/images/arrow-left.png"), tr("&Previous"), this);
  action->setShortcuts(QKeySequence::PreviousChild);
  action->setStatusTip(tr("Got to previos document"));
  m_actionMap["selectPreviousDocument"] = action;

  // View->Zoom all:
  action = new QAction(QIcon(":/images/zoom-draw.png"), tr("&Zoom all"), this);
  action->setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_0));
  action->setStatusTip(tr("Show the entire document"));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomAll()));
  m_actionMap["zoomAll"] = action;

  // View->Zoom selection:
  m_actionMap["zoomSelection"] = new ZoomSelectionAction(this);

  // View->Zoom in horizontally:
  action = new QAction(QIcon(":/images/zoom-in-h.png"), tr("Zoom &in horizontally"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
  action->setStatusTip(tr("Zoom horizontally into the current document."));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomInHorizontally()));
  m_actionMap["zoomInHorizontally"] = action;

  // View->Zoom out horizontally:
  action = new QAction(QIcon(":/images/zoom-out-h.png"), tr("Zoom out &horizontally"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  action->setStatusTip(tr("Zoom horizontally out of the current document."));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomOutHorizontally()));
  m_actionMap["zoomOutHorizontally"] = action;

  // View->Zoom in vertically:
  action = new QAction(QIcon(":/images/zoom-in-v.png"), tr("Zoom in verti&cally"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Plus));
  action->setStatusTip(tr("Zoom vertically into the current document."));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomInVertically()));
  m_actionMap["zoomInVertically"] = action;

  // View->Zoom out vertically:
  action = new QAction(QIcon(":/images/zoom-out-v.png"), tr("Zoom out &vertically"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_Minus));
  action->setStatusTip(tr("Zoom vertically out of the current document."));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomOutVertically()));
  m_actionMap["zoomOutVertically"] = action;

  // View->Document->1...10:
  for (int i = 0; i < 10; ++i)
  {
    QString text = i == 9 ? tr("1&0: Doc #10") : tr("&%1: Doc #%2").arg(i + 1).arg(i + 1);
    action = new QAction(QIcon(":images/wave-document.png"), text, this);
    action->setStatusTip(tr("Document #%2").arg(i + 1));
    action->setData(QVariant(i));
    connect(action, SIGNAL(triggered()), this, SLOT(selectDocument()));
    m_actionMap[QString("selectDocument%1").arg(i)] = action;
  }

  // View->More...:
  action = new QAction(QIcon(":/images/view-choose.png"), tr("&More..."), this);
  action->setStatusTip(tr("Show full list of open files"));
  connect(action, SIGNAL(triggered()), this, SLOT(showMoreDocuments()));
  m_actionMap["showMoreDocuments"] = action;

  // Help
  m_actionMap["goHome"] = new GoHomeAction(this);
  m_actionMap["about"] = new AboutAction(this);
  m_actionMap["aboutQt"] = new AboutQtAction(this);
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::createMainMenu()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper to create the main menu of the application window.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::createMainMenu()
{
  // File menu:
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(m_actionMap["newDocument"]);
  fileMenu->addAction(m_actionMap["newFromClipboard"]);
  fileMenu->addSeparator();
  fileMenu->addAction(m_actionMap["openDocument"]);
  QMenu* recents = fileMenu->addMenu(tr("Recen&t files"));
  recents->setStatusTip(tr("Open recent files"));
  for (int i = 0; i < 10; i++)
    recents->addAction(m_actionMap[QString("openRecentFile%1").arg(i)]);
  recents->addAction(m_actionMap["noRecentFiles"]);
  recents->addSeparator();
  recents->addAction(m_actionMap["showMoreRecentFiles"]);
  recents->addAction(m_actionMap["clearRecentFiles"]);
  fileMenu->addAction(m_actionMap["saveDocument"]);
  fileMenu->addAction(m_actionMap["saveDocumentAs"]);
  fileMenu->addAction(m_actionMap["saveAllDocuments"]);
  fileMenu->addSeparator();
  fileMenu->addAction(m_actionMap["closeDocument"]);
  fileMenu->addAction(m_actionMap["closeAllDocuments"]);
  fileMenu->addSeparator();
  fileMenu->addAction(m_actionMap["showStats"]);
  fileMenu->addAction(m_actionMap["printStats"]);
  fileMenu->addAction(m_actionMap["printPreview"]);
  fileMenu->addSeparator();
  fileMenu->addAction(m_actionMap["exit"]);

  // Edit menu:
  QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(m_actionMap["undo"]);
  editMenu->addAction(m_actionMap["redo"]);
  editMenu->addAction(m_actionMap["clearUndo"]);
  editMenu->addSeparator();
  editMenu->addAction(m_actionMap["cut"]);
  editMenu->addAction(m_actionMap["copy"]);
  editMenu->addAction(m_actionMap["paste"]);
  editMenu->addAction(m_actionMap["delete"]);
  editMenu->addSeparator();
  QMenu* selectMenu = editMenu->addMenu(tr("&Selection"));
  selectMenu->addAction(m_actionMap["selectAll"]);
  selectMenu->addAction(m_actionMap["selectNothing"]);
  selectMenu->addSeparator();
  selectMenu->addAction(m_actionMap["extendSelectionToStart"]);
  selectMenu->addAction(m_actionMap["extendSelectionToEnd"]);
  selectMenu->addAction(m_actionMap["extendSelectionToPreviousMarker"]);
  selectMenu->addAction(m_actionMap["extendSelectionToNextMarker"]);
  selectMenu->addAction(m_actionMap["extendSelectionToCursor"]);
  selectMenu->addAction(m_actionMap["extendSelectionToAllChannels"]);
  selectMenu->addAction(m_actionMap["extendSelectionDoubleLength"]);
  selectMenu->addAction(m_actionMap["shrinkSelectionHalfLength"]);
  selectMenu->addSeparator();
  selectMenu->addAction(m_actionMap["selectStartToCursor"]);
  selectMenu->addAction(m_actionMap["selectCursorToEnd"]);
  selectMenu->addAction(m_actionMap["selectCursorToPrevMarker"]);
  selectMenu->addAction(m_actionMap["selectCursorToNextMarker"]);
  selectMenu->addSeparator();
  selectMenu->addAction(m_actionMap["selectCustom"]);
  editMenu->addSeparator();
  editMenu->addAction(m_actionMap["settings"]);

  // Tools menu:
  QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
  toolsMenu->addAction(m_actionMap["configureShortcuts"]);
  toolsMenu->addAction(m_actionMap["configureToolbars"]);

  // View menu:
  QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
  m_toolbarMenu = viewMenu->addMenu(tr("Toolbars"));
  m_toolWindowMenu = viewMenu->addMenu(tr("Panels"));
  m_themeMenu = viewMenu->addMenu(tr("T&heme"));
  m_themeMenu->addAction(m_actionMap["defaultTheme"]);
  m_themeMenu->addAction(m_actionMap["darkTheme"]);
  viewMenu->addSeparator();
  viewMenu->addAction(m_actionMap["zoomAll"]);
  viewMenu->addAction(m_actionMap["zoomSelection"]);
  viewMenu->addAction(m_actionMap["zoomInHorizontally"]);
  viewMenu->addAction(m_actionMap["zoomOutHorizontally"]);
  viewMenu->addAction(m_actionMap["zoomInVertically"]);
  viewMenu->addAction(m_actionMap["zoomOutVertically"]);
  viewMenu->addSeparator();
  viewMenu->addAction(m_actionMap["selectNextDocument"]);
  viewMenu->addAction(m_actionMap["selectPreviousDocument"]);
  viewMenu->addSeparator();
  for (int i = 0; i < 10; i++)
    viewMenu->addAction(m_actionMap[QString("selectDocument%1").arg(i)]);
  viewMenu->addAction(m_actionMap["showMoreDocuments"]);

  // Help menu:
  QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(m_actionMap["goHome"]);
  helpMenu->addSeparator();
  helpMenu->addAction(m_actionMap["about"]);
  helpMenu->addAction(m_actionMap["aboutQt"]);
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::createToolbars()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper to create the toolbars of the application.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::createToolbars()
{
  // File toolbar:
  QToolBar* toolBar = addToolBar(tr("File"));
  toolBar->setObjectName("fileToolbar");
  toolBar->addAction(m_actionMap["newDocument"]);

  // The open button has a recent files menu attached:
  QMenu* menu = new QMenu(tr("Open"));
  for (int i = 0; i < 10; i++)
    menu->addAction(m_actionMap[QString("openRecentFile%1").arg(i)]);
  menu->addAction(m_actionMap["noRecentFiles"]);
  menu->addSeparator();
  menu->addAction(m_actionMap["showMoreRecentFiles"]);
  menu->addAction(m_actionMap["clearRecentFiles"]);
  OpenDocumentAction* oda = static_cast<OpenDocumentAction*>(m_actionMap["openDocument"]);
  menu->menuAction()->setStatusTip(oda->statusTip());
  menu->setIcon(oda->icon());
  connect(menu->menuAction(), SIGNAL(triggered()), oda, SLOT(fired()));
  toolBar->addAction(menu->menuAction());
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["saveDocument"]);
  toolBar->addAction(m_actionMap["saveDocumentAs"]);
  toolBar->addAction(m_actionMap["saveAllDocuments"]);
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["showStats"]);
  toolBar->addAction(m_actionMap["printStats"]);
  toolBar->toggleViewAction()->setStatusTip(tr("Show/hide file toolbar"));
  m_toolbarMenu->addAction(toolBar->toggleViewAction());

  // Edit toolbar:
  toolBar = addToolBar(tr("Edit"));
  toolBar->setObjectName("editToolbar");
  toolBar->addAction(m_actionMap["undo"]);
  toolBar->addAction(m_actionMap["redo"]);
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["cut"]);
  toolBar->addAction(m_actionMap["copy"]);
  toolBar->addAction(m_actionMap["paste"]);
  toolBar->addAction(m_actionMap["delete"]);
  toolBar->toggleViewAction()->setStatusTip(tr("Show/hide edit toolbar"));
  m_toolbarMenu->addAction(toolBar->toggleViewAction());

  // Transport toolbar:
  toolBar = addToolBar(tr("Tranport"));
  toolBar->setObjectName("transportToolbar");
  toolBar->addAction(m_actionMap["goToStart"]);
  toolBar->addAction(m_actionMap["seekBackward"]);
  toolBar->addAction(m_actionMap["seekForward"]);
  toolBar->addAction(m_actionMap["goToEnd"]);
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["startPlayback"]);
  toolBar->addAction(m_actionMap["stopPlayback"]);
  toolBar->addAction(m_actionMap["loop"]);
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["record"]);
  toolBar->addSeparator();
  toolBar->addAction(m_actionMap["goToPreviousMarker"]);
  toolBar->addAction(m_actionMap["goToNextMarker"]);
  toolBar->toggleViewAction()->setStatusTip(tr("Show/hide transport toolbar"));
  m_toolbarMenu->addAction(toolBar->toggleViewAction());
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::createStatusBar()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper to create the status bar of the application window.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::createStatusBar()
{
  // Initialize status bar text:
  statusBar()->showMessage(tr("Ready"));
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::createToolWindows()
////////////////////////////////////////////////////////////////////////////////
///\brief Internal helper to create the docked tool windows.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::createToolWindows()
{
  // History window:
  QDockWidget* dock = new HistoryToolWindow(m_docManager, this);
  addDockWidget(Qt::RightDockWidgetArea, dock);
  dock->toggleViewAction()->setStatusTip(tr("Show/hide the history window"));
  m_toolWindowMenu->addAction(dock->toggleViewAction());

  // Debug window:
  dock = new DebugToolWindow(this);
  addDockWidget(Qt::BottomDockWidgetArea, dock);
  dock->toggleViewAction()->setStatusTip(tr("Show/hide the debug window"));
  m_toolWindowMenu->addAction(dock->toggleViewAction());

  // Explorer window:
  dock = new BrowserToolWindow(this);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
  dock->toggleViewAction()->setStatusTip(tr("Show/hide the file selector window"));
  m_toolWindowMenu->addAction(dock->toggleViewAction());
}

///////////////////////////////// End of File //////////////////////////////////
