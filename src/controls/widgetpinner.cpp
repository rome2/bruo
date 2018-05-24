#include <QWidget>
#include "widgetpinner.h"

WidgetPinner::WidgetPinner(QWidget* parent) :
  m_parent(parent)
{
}

void WidgetPinner::addWidget(QWidget* child)
{
  m_children.append(child);
  QRectF rect;
  rect.setLeft(static_cast<qreal>(child->pos().x()) / m_parent->width());
  rect.setTop(static_cast<qreal>(child->pos().y()) / m_parent->height());
  rect.setWidth(static_cast<qreal>(child->width()) / m_parent->width());
  rect.setHeight(static_cast<qreal>(child->height()) / m_parent->height());
  m_childRects.append(rect);
}

void WidgetPinner::resizeWidgets()
{
  double w = m_parent->width();
  double h = m_parent->height();
  for (int i = 0; i < m_children.count(); i++)
  {
    int nx = static_cast<int>(m_childRects[i].x() * w);
    int ny = static_cast<int>(m_childRects[i].y() * h);
    int nw = static_cast<int>(m_childRects[i].width() * w);
    int nh = static_cast<int>(m_childRects[i].height() * h);
    m_children[i]->setGeometry(nx, ny, nw, nh);
  }
}
