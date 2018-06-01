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
  virtual void resizeEvent(QResizeEvent* event);
  virtual void closeEvent(QCloseEvent *event);
  virtual void showEvent(QShowEvent *event);

private slots:

  void idle();

private:
  Document* m_document;
  QList<class RackDeviceGUI*> m_devices;
  int m_deviceMargin;
  QVBoxLayout* m_layout;
  QTimer* m_idleTimer;
};

#endif // RACKWINDOW_H
