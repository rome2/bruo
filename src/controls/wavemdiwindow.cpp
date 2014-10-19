#include "wavemdiwindow.h"
#include "waveoverview.h"
#include "waveeditview.h"

WaveMDIWindow::WaveMDIWindow(Document* doc, QWidget* parent) :
  QMdiSubWindow(parent),
  m_document(doc)
{
  // We will be deleted on close:
  setAttribute(Qt::WA_DeleteOnClose);

  // Set title and icon:
  setWindowTitle(doc->composeTitle());
  setWindowIcon(QIcon(":images/wave-document.png"));

  // Create tab control:
  QTabWidget* tabs = new QTabWidget(this);
  tabs->setTabPosition(QTabWidget::South);
  tabs->setTabShape(QTabWidget::Triangular);
  tabs->setDocumentMode(false);
  tabs->setFocusPolicy(Qt::NoFocus);
  tabs->setAttribute(Qt::WA_ShowWithoutActivating);

  // Adjust tab appearance:
  QTabBar* tabBar = tabs->findChild<QTabBar*>();
  if (tabBar != 0)
  {
    tabBar->setAttribute(Qt::WA_ShowWithoutActivating);
    tabBar->setExpanding(false);
    tabBar->setMinimumWidth(10);
    tabBar->setFocusPolicy(Qt::NoFocus);
  }

  // Create splitter:
  m_splitter = new QSplitter(this);
  m_splitter->setOrientation(Qt::Vertical);
  m_splitter->setChildrenCollapsible(false);

  // Create views:
  m_mainView = new WaveEditView(doc, m_splitter);
  m_topView = new WaveOverView(doc, static_cast<WaveEditView*>(m_mainView), m_splitter);

  // Fill splitter:
  m_splitter->addWidget(m_topView);
  m_splitter->addWidget(m_mainView);

  // Init size:
  QList<int> sizes;
  sizes.append(8);
  sizes.append(92);
  m_splitter->setSizes(sizes);

  // Add splitter to tabs:
  tabs->addTab(m_splitter, tr("Wave"));

  // Create rack window:
  m_rack = new RackWindow(doc);

  // Add rack to tabs:
  tabs->addTab(m_rack, tr("Rack"));

  // Set tabs as main widget:
  setWidget(tabs);
}

Document* WaveMDIWindow::document()
{
  // Return our document:
  return m_document;
}

const Document* WaveMDIWindow::document() const
{
  // Return our document:
  return m_document;
}

void WaveMDIWindow::setDocument(Document* doc)
{
  // Set new document:
  m_document = doc;

  // Update children:
  m_topView->setDocument(doc);
  m_mainView->setDocument(doc);
}

bool WaveMDIWindow::overviewVisible() const
{
  return true;
}

void WaveMDIWindow::setOverviewVisible(bool /* newState */)
{
}

void WaveMDIWindow::zoomAll()
{
  // Update zoom:
  m_mainView->setViewport(0, m_document->sampleCount());
}

void WaveMDIWindow::zoomSelection()
{
  // Update zoom:
  if (m_document->selectionLength() > 0)
    m_mainView->setViewport(m_document->selectionStart(), m_document->selectionLength());
}

void WaveMDIWindow::zoomIn(bool vertically)
{
  if (vertically)
    m_mainView->btnPlusVPressed();
  else
    m_mainView->btnPlusHPressed();
}

void WaveMDIWindow::zoomOut(bool vertically)
{
  if (vertically)
    m_mainView->btnMinusVPressed();
  else
    m_mainView->btnMinusHPressed();
}

void WaveMDIWindow::idle()
{
  // Update children:
  m_rack->idle();
}

void WaveMDIWindow::closeEvent(QCloseEvent *event)
{
  // Only if we don't have a document anymore then closing is allowed:
  event->setAccepted(m_document == 0);
  if (m_document == 0)
    return;

  // Let the document's manager handle the close:
  if (m_document->manager() != 0)
    event->setAccepted(m_document->manager()->closeDocument(m_document));
}
