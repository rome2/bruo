#ifndef GRABLINEEDIT_H
#define GRABLINEEDIT_H

#include <QLineEdit>

class ShortcutEdit : public QLineEdit
{
  Q_OBJECT

public:

  explicit ShortcutEdit(QWidget* parent = 0);

signals:

public slots:

protected:

  void keyPressEvent(QKeyEvent* event);
};

#endif // GRABLINEEDIT_H
