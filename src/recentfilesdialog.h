#ifndef RECENTFILESDIALOG_H
#define RECENTFILESDIALOG_H

#include <QDialog>
#include <QListWidget>

class RecentFilesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RecentFilesDialog(const QStringList& files, QWidget* parent = 0);

  int selectedItem() const;

public slots:

  void listBoxSelectionChanged();

  void listBoxItemDoubleClicked(QListWidgetItem* item);

private:
  int m_selectedItem;
};

#endif // RECENTFILESDIALOG_H
