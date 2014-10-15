#include "rackwindow.h"

RackWindow::RackWindow(Document* doc, QWidget *parent) :
  QWidget(parent),
  m_document(doc)
{
}

void RackWindow::paintEvent(QPaintEvent* /* event */)
{
  QPainter painter(this);
  painter.fillRect(rect(), QColor(0, 0, 0));
}
