#include "rackwindow.h"
#include "rackdevice.h"
#include "rackdevicegui.h"

RackWindow::RackWindow(Document* doc, QWidget *parent) :
  QWidget(parent),
  m_document(doc)
{
  for (int i = 0, y = 0; i < m_document->rack().devices().count(); i++)
  {
    RackDevice* device = m_document->rack().devices().at(i);
    if (device == 0)
      continue;

    QWidget* deviceGui = device->createGUI(this);
    if (deviceGui == 0)
      continue;

    deviceGui->move(0, y);
    y += deviceGui->height();
  }
}

void RackWindow::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), QColor(0, 0, 0));
}
