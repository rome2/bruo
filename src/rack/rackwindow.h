#ifndef RACKWINDOW_H
#define RACKWINDOW_H

#include "documentmanager.h"

class RackWindow : public QWidget
{
  Q_OBJECT
public:
  explicit RackWindow(Document* doc, QWidget *parent = 0);

signals:

public slots:

protected:

  virtual void paintEvent(QPaintEvent* event);


private slots:

  void idle();

private:
  Document* m_document;
  QPixmap m_backpic;
};

#endif // RACKWINDOW_H
