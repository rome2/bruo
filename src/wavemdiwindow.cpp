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

  // Set splitter as main widget:
  setWidget(m_splitter);
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
