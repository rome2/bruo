#include "rackwindow.h"
#include "rackdevice.h"
#include "rackdevicegui.h"

RackWindow::RackWindow(Document* doc, QWidget *parent) :
  QWidget(parent),
  m_document(doc),
  m_backpic(":/images/rack/rack_back.png")
{
  for (int i = 0, y = 0; i < m_document->rack().devices().count(); i++)
  {
    RackDevice* device = m_document->rack().devices().at(i);
    if (device == 0)
      continue;

    RackDeviceGUI* deviceGui = device->createGUI(this);
    if (deviceGui == 0)
      continue;

    deviceGui->move(0, y);
    y += deviceGui->height();
  }
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

void RackWindow::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), QColor(0, 0, 0));

  for (int y = 0; y < height(); y += m_backpic.height())
    painter.drawPixmap(0, y, m_backpic.width(), m_backpic.height(), m_backpic);
}
