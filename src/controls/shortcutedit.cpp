#include "../bruo.h"
#include "shortcutedit.h"

ShortcutEdit::ShortcutEdit(QWidget* parent) :
  QLineEdit(parent)
{
}

void ShortcutEdit::keyPressEvent(QKeyEvent* event)
{
  int val = event->key();
  if (val == Qt::Key_unknown || val == Qt::Key_Shift || val == Qt::Key_Control || val == Qt::Key_Alt)
    val = 0;
  int modifiers = event->modifiers();
  if(modifiers & Qt::ControlModifier)
    val += Qt::CTRL;
  if(modifiers & Qt::ShiftModifier)
    val += Qt::SHIFT;
  if(modifiers & Qt::AltModifier)
    val += Qt::ALT;
  QKeySequence seq(val);
  setText(seq.toString());
}
