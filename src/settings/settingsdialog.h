#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QStackedLayout>

class SettingsDialog : public QDialog
{
  Q_OBJECT
public:
  explicit SettingsDialog(class MainFrame* parent = 0);

signals:

public slots:
  void apply();

 private:
  QLineEdit* m_filterLineEdit;
  QListWidget* m_categoryList;
  QLabel* m_headerLabel;
  QStackedLayout* m_stackedLayout;
};

#endif // SETTINGSDIALOG_H
