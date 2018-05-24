#ifndef RACKDEVICEGUI_H
#define RACKDEVICEGUI_H

#include "bruo.h"
#include "../audio/samplebuffer.h"
#include "rackdevice.h"
#include "../controls/widgetpinner.h"

class RackDeviceGUI : public QWidget
{
  Q_OBJECT

public:
  RackDeviceGUI(RackDevice* device, QWidget* parent);

  RackDevice* device();
  const RackDevice* device() const;

  virtual void idle();
  virtual void parameterChanged(const int index, const double value);

  virtual int heightForWidth(int w) const;
  virtual QSize sizeHint() const;

  double aspectRatio();
  void setAspectRatio(double ratio);

signals:

public slots:

protected:
  void addPinnedChild(QWidget* child);
  virtual void resizeEvent(QResizeEvent* event);

private:

  RackDevice* m_device;
  double m_aspectRatio;
  WidgetPinner m_widgetPinner;
};

#endif // RACKDEVICEGUI_H
