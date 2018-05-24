#ifndef WIDGETPINNER_H
#define WIDGETPINNER_H

class WidgetPinner
{
public:
  WidgetPinner(QWidget* parent);
  void addWidget(QWidget* child);
  void resizeWidgets();

private:
  QWidget* m_parent;
  QList<QWidget*> m_children;
  QList<QRectF> m_childRects;
};

#endif // WIDGETPINNER_H
