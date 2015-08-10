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
  m_docManager(0)
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
  if (settings.contains("document/recentFiles"))
    m_recentFiles = settings.value("document/recentFiles").toStringList();
  updateRecentFiles();

  // Init window menu:
  updateDocumentMenu();

  // Init active states:
  activeDocumentChanged();
  selectionChanged();

  // Init clipboard:
  QClipboard* clip = QApplication::clipboard();
  connect(clip, SIGNAL(changed(QClipboard::Mode)), SLOT(clipboardChanged(QClipboard::Mode)));
  clipboardChanged(QClipboard::Selection);

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

  // Create idle timer:
  QTimer* idleTimer = new QTimer(this);
  idleTimer->setSingleShot(false);
  connect(idleTimer,SIGNAL(timeout()), this, SLOT(idleEvent()));
  idleTimer->start(100);

  AudioSystem::initialize(m_docManager);
  AudioSystem::start();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::~MainFrame()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Choose! Choose the form of the Destructor!
////////////////////////////////////////////////////////////////////////////////
MainFrame::~MainFrame()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::canPaste()
////////////////////////////////////////////////////////////////////////////////
///\brief   Check if the clipboard content is an audio file.
///\return  true if the contents of the clipboard can be pasted.
///\remarks Usually only uncompressed PCM wave data can be pasted.
////////////////////////////////////////////////////////////////////////////////
bool MainFrame::canPaste() const
{
  // Check for a target document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return false;

  // Get clipboard contents:
  const QClipboard* clipboard = QApplication::clipboard();
  const QMimeData* mimeData = clipboard->mimeData();

  // Check if it's the right type:
  return mimeData->hasFormat("audio/wav") ||
         mimeData->hasFormat("audio/x-wav") ||
         mimeData->hasFormat("audio/x-aiff");
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
  AudioSystem::stop();
  AudioSystem::finalize();

  // Save window position:
  QSettings settings;

  // Save window position:
  settings.setValue("mainwindow/geometry", saveGeometry());

  // Save toolbars etc:
  settings.setValue("mainwindow/windowstate", saveState());

  // Save recent files:
  settings.setValue("document/recentFiles", m_recentFiles);

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
  bool selected = doc != 0 && doc->selectionLength() > 0;
  bool dirty = doc != 0 && doc->dirty() > 0;

  // Update actions:
  m_actionMap["saveDocument"]->setEnabled(dirty);
  m_actionMap["saveDocumentAs"]->setEnabled(doc != 0);
  m_actionMap["saveAllDocuments"]->setEnabled(m_docManager->documents().length() > 0);
  m_actionMap["closeDocument"]->setEnabled(doc != 0);
  m_actionMap["closeAllDocuments"]->setEnabled(m_docManager->documents().length() > 0);
  m_actionMap["showStats"]->setEnabled(doc != 0);
  m_actionMap["printStats"]->setEnabled(doc != 0);
  m_actionMap["printPreview"]->setEnabled(doc != 0);
  m_actionMap["cut"]->setEnabled(selected);
  m_actionMap["copy"]->setEnabled(selected);
  m_actionMap["delete"]->setEnabled(selected);
  m_actionMap["selectAll"]->setEnabled(doc != 0);
  m_actionMap["selectNothing"]->setEnabled(selected);
  m_actionMap["extendSelectionToStart"]->setEnabled(selected);
  m_actionMap["extendSelectionToEnd"]->setEnabled(selected);
  m_actionMap["extendSelectionToPreviousMarker"]->setEnabled(selected);
  m_actionMap["extendSelectionToNextMarker"]->setEnabled(selected);
  m_actionMap["extendSelectionToCursor"]->setEnabled(selected);
  m_actionMap["extendSelectionToAllChannels"]->setEnabled(selected && doc->selectedChannel() >= 0);
  m_actionMap["extendSelectionDoubleLength"]->setEnabled(selected);
  m_actionMap["extendSelectionHalfLength"]->setEnabled(selected);
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
  m_actionMap["ZoomAll"]->setEnabled(doc != 0);
  m_actionMap["ZoomSelection"]->setEnabled(selected);
  m_actionMap["zoomInHorizontally"]->setEnabled(doc != 0);
  m_actionMap["zoomOutHorizontally"]->setEnabled(doc != 0);
  m_actionMap["zoomInVertically"]->setEnabled(doc != 0);
  m_actionMap["zoomOutVertically"]->setEnabled(doc != 0);
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
  connect(doc, SIGNAL(closed()),           this, SLOT(documentClosed()));
  connect(doc, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
  connect(doc, SIGNAL(dirtyChanged()),     this, SLOT(documentDirtyChanged()));
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

void MainFrame::selectionChanged()
{
  // Get document and selection state:
  Document* doc = m_docManager->activeDocument();
  bool selected = doc != 0 && doc->selectionLength() > 0;

  // Update actions:
  m_actionMap["cut"]->setEnabled(selected);
  m_actionMap["copy"]->setEnabled(selected);
  m_actionMap["delete"]->setEnabled(selected);
  m_actionMap["selectNothing"]->setEnabled(selected);
  m_actionMap["extendSelectionToStart"]->setEnabled(selected);
  m_actionMap["extendSelectionToEnd"]->setEnabled(selected);
  m_actionMap["extendSelectionToPreviousMarker"]->setEnabled(selected);
  m_actionMap["extendSelectionToNextMarker"]->setEnabled(selected);
  m_actionMap["extendSelectionToCursor"]->setEnabled(selected);
  m_actionMap["extendSelectionToAllChannels"]->setEnabled(selected && doc->selectedChannel() >= 0);
  m_actionMap["extendSelectionDoubleLength"]->setEnabled(selected);
  m_actionMap["extendSelectionHalfLength"]->setEnabled(selected);
  m_actionMap["ZoomSelection"]->setEnabled(selected);
}

void MainFrame::clipboardChanged(QClipboard::Mode /* mode */)
{
  // Check if it's the right type:
  bool formatOK = canPaste();

  // Update related actions:
  m_actionMap["paste"]->setEnabled(formatOK);
  m_actionMap["newFromClipboard"]->setEnabled(formatOK);
}

void MainFrame::newDocument()
{
}

void MainFrame::newFromClipboard()
{
}

void MainFrame::openDocument()
{
  QStringList filters;
  filters << tr("Microsoft (*.wav)")
          << tr("SGI / Apple (*.aiff *.aif *.aifc)")
          << tr("Sun / DEC / NeXT (*.au *.snd)")
          << tr("Headerless (*raw)")
          << tr("Paris Audio File (*.paf)")
          << tr("Commodore Amiga (*.iff *.svx)")
          << tr("Sphere Nist (*.wav *.nist *.sph)")
          << tr("IRCAM (*.sf)")
          << tr("Creative (*.voc)")
          << tr("Soundforge (*.w64)")
          << tr("GNU Octave 2.0 (*.mat4)")
          << tr("GNU Octave 2.1 (*.mat5)")
          << tr("Portable Voice Format (*.pvf)")
          << tr("Fasttracker 2 (*.xi)")
          << tr("HMM Tool Kit (*.htk)")
          << tr("Apple (*.caf)")
          << tr("Sound Designer II (*.sd2)")
          << tr("Free Lossless Audio Codec (*.flac)")
          << tr("Ogg / Vorbis (*.ogg *.mogg)")
          << tr("All files (*.*)");
  QFileDialog dialog;
  dialog.setNameFilters(filters);
  if (dialog.exec())
  {
    // Load the file:
    loadFile(dialog.selectedFiles()[0]);
  }
}

void MainFrame::openRecentFile()
{
  // Get source action:
  QAction* action = qobject_cast<QAction*>(sender());
  if (action != 0)
  {
    // Load the file:
    loadFile(action->data().toString());
  }
}

void MainFrame::showMoreRecentFiles()
{
  // Create and init the dialog:
  StringSelectDialog dialog(m_recentFiles, this);
  dialog.setWindowTitle(tr("Select File"));

  // Show it:
  if (dialog.exec() == QDialog::Accepted)
  {
    // Load the seected file:
    int index = dialog.selectedItem();
    if (index >= 0 && index < m_recentFiles.length())
      loadFile(m_recentFiles[index]);
  }
}

void MainFrame::clearRecentFiles()
{
  // Clear the list:
  m_recentFiles.clear();

  // Update UI:
  updateRecentFiles();
}

void MainFrame::saveDocument()
{
}

void MainFrame::saveDocumentAs()
{
}

void MainFrame::saveAllDocuments()
{
}

void MainFrame::closeDocument()
{
  // Close the document:
  m_docManager->closeDocument(m_docManager->activeDocument());
}

void MainFrame::closeAllDocuments()
{
  // Close all documents:
  m_docManager->closeAllDocuments();
}

void MainFrame::showStats()
{
}

void MainFrame::printStats()
{
}

void MainFrame::printPreview()
{
}

void MainFrame::cut()
{
}

void MainFrame::copy()
{
}

void MainFrame::paste()
{
}

void MainFrame::deleteAction()
{
}

void MainFrame::selectAll()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionStart() == 0 && doc->selectionLength() == doc->sampleCount())
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, 0, doc->sampleCount());
  cmd->setText(tr("Select all"));
  doc->undoStack()->push(cmd);
}

void MainFrame::selectNothing()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionStart() == 0 && doc->selectionLength() == 0)
    return;

  // Create selection command:
  ClearSelectionCommand* cmd = new ClearSelectionCommand(doc);
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionToStart()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionStart() == 0)
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, 0, doc->selectionStart() + doc->selectionLength(), doc->selectedChannel());
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionToEnd()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionLength() == doc->sampleCount())
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->selectionStart(), doc->sampleCount() - doc->selectionStart(), doc->selectedChannel());
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionToPreviousMarker()
{
}

void MainFrame::extendSelectionToNextMarker()
{
}

void MainFrame::extendSelectionToCursor()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->cursorPosition() >= doc->selectionStart() && doc->cursorPosition() <= (doc->selectionStart() + doc->selectionLength()))
    return;

  // Calc new length and position:
  qint64 pos, len;
  if (doc->cursorPosition() < doc->selectionStart())
  {
    pos = doc->cursorPosition();
    len = (doc->selectionStart() + doc->selectionLength()) - pos;
  }
  else
  {
    pos = doc->selectionStart();
    len = doc->cursorPosition() - pos;
  }

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, pos, len, doc->selectedChannel());
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionToAllChannels()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectedChannel() < 0)
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->selectionStart(), doc->selectionLength(), -1);
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionDoubleLength()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionLength() == doc->sampleCount())
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->selectionStart(), doc->selectionLength() * 2, doc->selectedChannel());
  cmd->setText(tr("Extend selection"));
  doc->undoStack()->push(cmd);
}

void MainFrame::extendSelectionHalfLength()
{
  // Get document:
  Document* doc = m_docManager->activeDocument();
  if (doc == 0)
    return;

  // Anything to do?
  if (doc->selectionLength() < 2)
    return;

  // Create selection command:
  SelectCommand* cmd = new SelectCommand(doc, doc->selectionStart(), doc->selectionLength() / 2, doc->selectedChannel());
  cmd->setText(tr("Shrink selection"));
  doc->undoStack()->push(cmd);
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
    // Activate the seected document:
    int index = dialog.selectedItem();
    if (index >= 0 && index < m_recentFiles.length())
      m_docManager->setActiveDocument(index);
  }
}

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
    AudioSuspender suspender;
    m_docManager->setActiveDocument(view->document());
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

void MainFrame::zoomSelection()
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
  subWindow->zoomSelection();
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

void MainFrame::loadFile(QString fileName)
{
  AudioSuspender suspender;

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
    addRecentFile(fileName);

    // Activate rack:
    doc->rack().activate();
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
// MainFrame::addRecentFile()
////////////////////////////////////////////////////////////////////////////////
///\brief   Add a new entry to the recent file list.
///\param   [in] fileName: New file to add.
///\remarks This will call 'updateRecentFiles()' to update the UI.
////////////////////////////////////////////////////////////////////////////////
void MainFrame::addRecentFile(const QString& fileName)
{
  // Remove it if it's already in the list:
  if (m_recentFiles.contains(fileName))
    m_recentFiles.removeOne(fileName);

  // Add to the back:
  m_recentFiles.append(fileName);

  // Clip number of entries:
  int maxEntries = QSettings().value("document/maxRecentFiles", 50).toInt();
  while (m_recentFiles.length() > maxEntries)
    m_recentFiles.removeFirst();

  // Update UI:
  updateRecentFiles();
}

////////////////////////////////////////////////////////////////////////////////
// MainFrame::updateRecentFiles()
////////////////////////////////////////////////////////////////////////////////
///\brief Update the 'recent files' related actions (and thus the menu).
////////////////////////////////////////////////////////////////////////////////
void MainFrame::updateRecentFiles()
{
  // Show full paths?
  bool fullPaths = QSettings().value("document/recentFilesFullPath", true).toBool();

  // Loop through menu entries:
  for (int i = 0; i < 10; ++i)
  {
    // Get action:
    QAction* action = m_actionMap[QString("openRecentFile%1").arg(i)];

    // Get recent entry:
    int entry = m_recentFiles.length() - 1 - i;
    if (entry < 0)
    {
      // No entry for this action, deactivate it:
      action->setVisible(false);
    }
    else
    {
      // Show the item:
      action->setVisible(true);

      // Set file name as payload:
      action->setData(QVariant(m_recentFiles[entry]));

      // Get short file name:
      QString shortPath = QFileInfo(m_recentFiles[entry]).fileName();

      // Update action text:
      QString text = i == 9 ? tr("1&0: %1") : tr("&%1: ").arg(i + 1);
      if (fullPaths)
        text.append(m_recentFiles[entry]);
      else
        text.append(shortPath);
      action->setText(text);

      // Update status bar entry:
      action->setStatusTip(tr("Open %1").arg(shortPath));
    }
  }

  // Show/hide placeholder:
  m_actionMap["noRecentFiles"]->setVisible(m_recentFiles.length() <= 0);

  // Enable/disable related items:
  m_actionMap["showMoreRecentFiles"]->setEnabled(m_recentFiles.length() > 10);
  m_actionMap["clearRecentFiles"]->setEnabled(m_recentFiles.length() > 0);
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

  // File->New:
  action = new QAction(QIcon(":/images/document-new.png"), tr("&New..."), this);
  action->setShortcuts(QKeySequence::New);
  action->setStatusTip(tr("Create a new empty document"));
  connect(action, SIGNAL(triggered()), this, SLOT(newDocument()));
  m_actionMap["newDocument"] = action;

  // File->New from clipboard:
  action = new QAction(QIcon(":/images/new_clip.png"), tr("Ne&w from clipboard"), this);
  action->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_N));
  action->setStatusTip(tr("Create a new document from clipboard"));
  connect(action, SIGNAL(triggered()), this, SLOT(newFromClipboard()));
  m_actionMap["newFromClipboard"] = action;

  // File->Open:
  action = new QAction(QIcon(":/images/document-open.png"), tr("&Open..."), this);
  action->setShortcuts(QKeySequence::Open);
  action->setStatusTip(tr("Open an existing document"));
  connect(action, SIGNAL(triggered()), this, SLOT(openDocument()));
  m_actionMap["openDocument"] = action;

  // File->Open recent->1...10:
  for (int i = 0; i < 10; ++i)
  {
    QString text = i == 9 ? tr("1&0: Recent file #10") : tr("&%1: Recent file #%2").arg(i + 1).arg(i + 1);
    action = new QAction(QIcon(":images/wave-document.png"), text, this);
    action->setStatusTip(tr("Open recent file #%2").arg(i + 1));
    connect(action, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    m_actionMap[QString("openRecentFile%1").arg(i)] = action;
  }

  // File->Open recent->No recent files:
  action = new QAction(tr("No recent files"), this);
  action->setStatusTip(tr("No recent files"));
  action->setEnabled(false);
  m_actionMap["noRecentFiles"] = action;

  // File->Open recent->More...:
  action = new QAction(QIcon(":/images/document-open-recent.png"), tr("&More..."), this);
  action->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_O));
  action->setStatusTip(tr("Show full list of recent files"));
  connect(action, SIGNAL(triggered()), this, SLOT(showMoreRecentFiles()));
  m_actionMap["showMoreRecentFiles"] = action;

  // File->Open recent->Clear list:
  action = new QAction(QIcon(":/images/edit-clear-list.png"), tr("&Clear list"), this);
  action->setStatusTip(tr("Clears the list of recent files"));
  connect(action, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
  m_actionMap["clearRecentFiles"] = action;

  // File->Save:
  action = new QAction(QIcon(":/images/document-save.png"), tr("&Save"), this);
  action->setShortcuts(QKeySequence::Save);
  action->setStatusTip(tr("Save the current document"));
  connect(action, SIGNAL(triggered()), this, SLOT(saveDocument()));
  m_actionMap["saveDocument"] = action;

  // File->Save as:
  action = new QAction(QIcon(":/images/document-save-as.png"), tr("Save &as..."), this);
  action->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
  action->setStatusTip(tr("Save the current document under a new name"));
  connect(action, SIGNAL(triggered()), this, SLOT(saveDocumentAs()));
  m_actionMap["saveDocumentAs"] = action;

  // File->Save all:
  action = new QAction(QIcon(":/images/document-save-all.png"), tr("Sa&ve all"), this);
  action->setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_S));
  action->setStatusTip(tr("Save all open documents"));
  connect(action, SIGNAL(triggered()), this, SLOT(saveAllDocuments()));
  m_actionMap["saveAllDocuments"] = action;

  // File->Close:
  action = new QAction(QIcon(":images/document-close.png"), tr("&Close"), this);
  action->setShortcut(QKeySequence::Close);
  action->setStatusTip(tr("Save all open documents"));
  connect(action, SIGNAL(triggered()), this, SLOT(closeDocument()));
  m_actionMap["closeDocument"] = action;

  // File->Close all:
  action = new QAction(QIcon(":images/document-close-all.png"), tr("Close a&ll"), this);
  action->setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_F4));
  action->setStatusTip(tr("Close all open documents"));
  connect(action, SIGNAL(triggered()), this, SLOT(closeAllDocuments()));
  m_actionMap["closeAllDocuments"] = action;

  // File->Properties:
  action = new QAction(QIcon(":/images/document-properties.png"), tr("&Properties..."), this);
  action->setStatusTip(tr("Show stats for the current document"));
  connect(action, SIGNAL(triggered()), this, SLOT(showStats()));
  m_actionMap["showStats"] = action;

  // File->Print stats:
  action = new QAction(QIcon(":/images/document-print.png"), tr("&Print stats..."), this);
  action->setShortcuts(QKeySequence::Print);
  action->setStatusTip(tr("Print stats for the current document"));
  connect(action, SIGNAL(triggered()), this, SLOT(printStats()));
  m_actionMap["printStats"] = action;

  // File->Print preview:
  action = new QAction(QIcon(":images/document-print-preview.png"), tr("P&rint preview..."), this);
  action->setShortcut(QKeySequence(Qt::ALT + Qt::CTRL + Qt::Key_P));
  action->setStatusTip(tr("Preview the printer output"));
  connect(action, SIGNAL(triggered()), this, SLOT(printPreview()));
  m_actionMap["printPreview"] = action;

  // File->Quit:
  m_actionMap["exitApplication"] = new ExitAction(this);
  m_actionMap["undo"] = new UndoAction(this);
  m_actionMap["redo"] = new RedoAction(this);
  m_actionMap["clearUndo"] = new ClearUndoAction(this);

  // Edit->cut:
  action = new QAction(QIcon(":/images/edit-cut.png"), tr("&Cut"), this);
  action->setShortcuts(QKeySequence::Cut);
  action->setStatusTip(tr("Cut the current selection and put it to the clipboard"));
  connect(action, SIGNAL(triggered()), this, SLOT(cut()));
  m_actionMap["cut"] = action;

  // Edit->copy:
  action = new QAction(QIcon(":/images/edit-copy.png"), tr("Co&py"), this);
  action->setShortcuts(QKeySequence::Copy);
  action->setStatusTip(tr("Copy the current selection to the clipboard"));
  connect(action, SIGNAL(triggered()), this, SLOT(copy()));
  m_actionMap["copy"] = action;

  // Edit->paste:
  action = new QAction(QIcon(":/images/edit-paste.png"), tr("&Paste"), this);
  action->setShortcuts(QKeySequence::Paste);
  action->setStatusTip(tr("Insert the clipboard contents at the current position."));
  connect(action, SIGNAL(triggered()), this, SLOT(paste()));
  m_actionMap["paste"] = action;

  // Edit->delete:
  action = new QAction(QIcon(":/images/edit-delete.png"), tr("&Delete"), this);
  action->setShortcuts(QKeySequence::Delete);
  action->setStatusTip(tr("Remove the current selection"));
  connect(action, SIGNAL(triggered()), this, SLOT(deleteAction()));
  m_actionMap["delete"] = action;

  // Edit->select all:
  action = new QAction(QIcon(":/images/edit-select-all.png"), tr("&Select all"), this);
  action->setShortcuts(QKeySequence::SelectAll);
  action->setStatusTip(tr("Select the entire document"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectAll()));
  m_actionMap["selectAll"] = action;

  // Edit->select nothing:
  action = new QAction(QIcon(":/images/edit-select-nothing.png"), tr("Select &nothing"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));
  action->setStatusTip(tr("Deselect all"));
  connect(action, SIGNAL(triggered()), this, SLOT(selectNothing()));
  m_actionMap["selectNothing"] = action;

  // Edit->extend to start:
  action = new QAction(QIcon(":/images/edit-select-to-start.png"), tr("E&xtend to start"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Left));
  action->setStatusTip(tr("Extend selection to the start of the document"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToStart()));
  m_actionMap["extendSelectionToStart"] = action;

  // Edit->extend to end:
  action = new QAction(QIcon(":/images/edit-select-to-end.png"), tr("Ex&tend to end"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Right));
  action->setStatusTip(tr("Extend selection to the end of the document"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToEnd()));
  m_actionMap["extendSelectionToEnd"] = action;

  // Edit->extend to previous marker:
  action = new QAction(QIcon(":/images/edit-select-to-prev-marker.png"), tr("Extend to &previous marker"), this);
  action->setStatusTip(tr("Extend selection to the next marker"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToPreviousMarker()));
  m_actionMap["extendSelectionToPreviousMarker"] = action;

  // Edit->extend to next marker:
  action = new QAction(QIcon(":/images/edit-select-to-next-marker.png"), tr("Extend to next &marker"), this);
  action->setStatusTip(tr("Extend selection to the next marker"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToNextMarker()));
  m_actionMap["extendSelectionToNextMarker"] = action;

  // Edit->extend to cursor:
  action = new QAction(QIcon(":/images/edit-select-to-cursor.png"), tr("Extend to &cursor"), this);
  action->setStatusTip(tr("Extend selection to the play cursor"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToCursor()));
  m_actionMap["extendSelectionToCursor"] = action;

  // Edit->extend to all channels:
  action = new QAction(QIcon(":/images/edit-select-all-channels.png"), tr("Extend to all c&hannels"), this);
  action->setStatusTip(tr("Extend selection to all channels"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionToAllChannels()));
  m_actionMap["extendSelectionToAllChannels"] = action;

  // Edit->extend to double length:
  action = new QAction(QIcon(":/images/edit-select-double-length.png"), tr("Extend to &double length"), this);
  action->setStatusTip(tr("Extend selection to twice the length"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionDoubleLength()));
  m_actionMap["extendSelectionDoubleLength"] = action;

  // Edit->shrink to half length:
  action = new QAction(QIcon(":/images/edit-select-half-length.png"), tr("Shrink to &half length"), this);
  action->setStatusTip(tr("Extend selection to half the length"));
  connect(action, SIGNAL(triggered()), this, SLOT(extendSelectionHalfLength()));
  m_actionMap["extendSelectionHalfLength"] = action;

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
  m_actionMap["ZoomAll"] = action;

  // View->Zoom selection:
  action = new QAction(QIcon(":/images/zoom-select.png"), tr("Z&oom to selection"), this);
  action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
  action->setStatusTip(tr("Zoom to selection"));
  connect(action, SIGNAL(triggered()), this, SLOT(zoomSelection()));
  m_actionMap["ZoomSelection"] = action;

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
  fileMenu->addAction(m_actionMap["exitApplication"]);

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
  selectMenu->addAction(m_actionMap["extendSelectionHalfLength"]);
  selectMenu->addSeparator();
  selectMenu->addAction(m_actionMap["selectStartToCursor"]);
  selectMenu->addAction(m_actionMap["selectCursorToEnd"]);
  selectMenu->addAction(m_actionMap["selectCursorToPrevMarker"]);
  selectMenu->addAction(m_actionMap["selectCursorToNextMarker"]);
  selectMenu->addSeparator();
  selectMenu->addAction(m_actionMap["selectCustom"]);

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
  viewMenu->addAction(m_actionMap["ZoomAll"]);
  viewMenu->addAction(m_actionMap["ZoomSelection"]);
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
  menu->menuAction()->setStatusTip(m_actionMap["openDocument"]->statusTip());
  menu->setIcon(m_actionMap["openDocument"]->icon());
  connect(menu->menuAction(), SIGNAL(triggered()), this, SLOT(openDocument()));
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
