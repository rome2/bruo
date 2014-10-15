#ifndef WAVEMDIWINDOW_H
#define WAVEMDIWINDOW_H

#include "documentmanager.h"
#include "waveview.h"
#include "waveoverview.h"
#include "waveeditview.h"
#include "rackwindow.h"
#include <QMdiSubWindow>

class WaveMDIWindow : public QMdiSubWindow
{
  Q_OBJECT // Qt magic...

public:
  WaveMDIWindow(Document* doc, QWidget* parent = 0);

  Document* document();
  const Document* document() const;
  void setDocument(Document* doc);

  bool overviewVisible() const;
  void setOverviewVisible(bool newState);

  void zoomAll();
  void zoomSelection();
  void zoomIn(bool vertically);
  void zoomOut(bool vertically);

protected:
  virtual void closeEvent(QCloseEvent* event);

private:
  Document* m_document;
  QSplitter* m_splitter;
  WaveOverView* m_topView;
  WaveEditView* m_mainView;
  RackWindow* m_rack;
};

#endif // WAVEMDIWINDOW_H
