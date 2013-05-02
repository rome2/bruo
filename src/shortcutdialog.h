#ifndef SHORTCUTDIALOG_H
#define SHORTCUTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QAction>

class ShortcutDialog : public QDialog
{
  Q_OBJECT
public:
  explicit ShortcutDialog(QHash<QString, QAction*>& actionMap, QWidget* parent = 0);

signals:

public slots:

  void itemSelectionChanged();
  void searchTextChanged(QString);

private:

  class CustomItem :
    public QTableWidgetItem
  {
  public:
    CustomItem(const QString& text, QAction* itemAction) :
      QTableWidgetItem(text, QTableWidgetItem::UserType + 1),
      m_action(itemAction),
      m_modified(false)
    {
      m_shortCut = m_action->shortcut();
    }

    QAction* action()
    {
      return m_action;
    }

    bool modified() const
    {
      return m_modified;
    }

    const QKeySequence& shortcut() const
    {
      return m_shortCut;
    }

    void setShortcut(const QKeySequence& sc)
    {
      if (sc != m_shortCut)
      {
        m_shortCut = sc;
        m_modified = true;
      }
    }

  private:
    QAction* m_action;
    bool m_modified;
    QKeySequence m_shortCut;
  };

  QTableWidget* m_tableWidget;
  class ShortcutEdit* m_scEdit;
  QPushButton* m_assignBtn;
  QPushButton* m_restoreBtn;
};

#endif // SHORTCUTDIALOG_H
