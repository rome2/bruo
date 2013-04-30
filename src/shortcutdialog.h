#ifndef SHORTCUTDIALOG_H
#define SHORTCUTDIALOG_H

#include <QDialog>
#include <QTableWidget>

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
      m_action(itemAction)
    {
    }

    QAction* action()
    {
      return m_action;
    }

  private:
    QAction* m_action;
  };

  QTableWidget* m_tableWidget;
};

#endif // SHORTCUTDIALOG_H
