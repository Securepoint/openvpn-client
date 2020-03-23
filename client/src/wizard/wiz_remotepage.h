#ifndef WIZ_REMOTEPAGE_H
#define WIZ_REMOTEPAGE_H

#include <QtWidgets/QWizardPage>
#include <QtCore>

namespace Ui {
    class RemotePage;
}

class RemotePage : public QWizardPage {
    Q_OBJECT

public:
    RemotePage(QWidget *parent = 0);
    ~RemotePage();

    Q_PROPERTY(int rowCount READ getRowCount WRITE setRowCount NOTIFY rowCountChanged)

  
    int getRowCount() const;
    void setRowCount(int rowCount);

    QStringList remoteList();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject* object, QEvent* event);

private:
    Ui::RemotePage *m_ui;

    int _rowCount;

    void initializePage();
      

signals:
    void rowCountChanged();

private slots:
    void on_cmdAdd_clicked();
    void on_cmdRemove_clicked();
public slots:
    void on_tbvRowCountChanged(const QModelIndex &parent, int first, int last);

};

#endif // WIZ_REMOTEPAGE_H
