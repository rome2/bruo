#ifndef WAVEMDIWINDOW_H
#define WAVEMDIWINDOW_H

#include "documentmanager.h"
#include "waveview.h"
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

protected:
  virtual void closeEvent(QCloseEvent* event);

private:
  Document* m_document;
  QSplitter* m_splitter;
  WaveView* m_topView;
  WaveView* m_mainView;
};

#endif // WAVEMDIWINDOW_H
