#include "rackwindow.h"
#include "rackdevice.h"
#include "rackdevicegui.h"

RackWindow::RackWindow(Document* doc, QWidget *parent) :
  QWidget(parent),
  m_document(doc),
  m_deviceMargin(5)
{
  m_layout = new QVBoxLayout();
  for (int i = 0; i < m_document->rack().devices().count(); i++)
  {
    RackDevice* device = m_document->rack().devices().at(i);
    if (device == 0)
      continue;

    RackDeviceGUI* deviceGui = device->createGUI(this);
    if (deviceGui == 0)
      continue;

    m_devices.append(deviceGui);
    m_layout->addWidget(deviceGui, 0);
  }
  m_layout->addStretch(1);
  setLayout(m_layout);

  // Create idle timer:
  QTimer* idleTimer = new QTimer(this);
  idleTimer->setSingleShot(false);
  connect(idleTimer,SIGNAL(timeout()), this, SLOT(idle()));
  idleTimer->start(100);
}

void RackWindow::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), QColor(42, 50, 55));
}

void RackWindow::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
}

void RackWindow::idle()
{
  // Update children:
  for (int i = 0; i < m_document->rack().devices().count(); i++)
  {
    RackDevice* device = m_document->rack().devices().at(i);
    if (device == 0)
      continue;

    RackDeviceGUI* deviceGui = device->gui();
    if (deviceGui != 0)
      deviceGui->idle();
  }
}
