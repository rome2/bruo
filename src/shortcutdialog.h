#ifndef SHORTCUTDIALOG_H
#define SHORTCUTDIALOG_H

#include <QDialog>

class ShortcutDialog : public QDialog
{
  Q_OBJECT
public:
  explicit ShortcutDialog(QHash<QString, QAction*>& actionMap, QWidget* parent = 0);

signals:

public slots:

private:
 QHash<QString, QAction*>&  m_actionMap;

};

#endif // SHORTCUTDIALOG_H
