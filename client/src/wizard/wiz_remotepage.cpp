#include "wiz_remotepage.h"
#include "ui_wiz_remotepage.h"

#include <QStringListModel>
#include <QStandardItemModel>
#include <QKeyEvent>

float windowsDpiScale();

RemotePage::RemotePage(QWidget *parent) :
    QWizardPage(parent),
    m_ui(new Ui::RemotePage)
{
    _rowCount = 0;
    m_ui->setupUi(this);

    auto geom = this->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom.setSize(size);
    this->setGeometry(geom);

    // fill Combo
    m_ui->cmbRemoteProtocol->insertItem(0, QLatin1String("TCP"));
    m_ui->cmbRemoteProtocol->insertItem(1, QLatin1String("UDP"));

    this->registerField(QLatin1String("txtRemoteProtocol"), m_ui->cmbRemoteProtocol);

    this->m_ui->lvRemote->setModel(new QStandardItemModel(0, 2));

    this->m_ui->lvRemote->model()->setHeaderData( 0, Qt::Horizontal, "IP");
    this->m_ui->lvRemote->model()->setHeaderData( 1, Qt::Horizontal, "Port");

    this->m_ui->lvRemote->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

   // this->m_ui->lvRemote->installEventFilter(this);
    this->registerField(QLatin1String("txtRemoteCount*"), this, "rowCount", SIGNAL(rowCountChanged()));

    QObject::connect(m_ui->lvRemote->model(), SIGNAL(rowsInserted(QModelIndex,int,int)),  this, SLOT(on_tbvRowCountChanged(QModelIndex,int,int)));
    QObject::connect(m_ui->lvRemote->model(), SIGNAL(rowsRemoved(QModelIndex,int,int)),  this, SLOT(on_tbvRowCountChanged(QModelIndex,int,int)));

    this->setTitle("\n\n" + this->title());

}

void RemotePage::on_tbvRowCountChanged(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    setRowCount(this->m_ui->lvRemote->model()->rowCount());
    
}

int RemotePage::getRowCount() const
{
    return this->_rowCount;
}

void RemotePage::setRowCount(int rowCount)
{
    this->_rowCount = rowCount;
    emit rowCountChanged();
}

RemotePage::~RemotePage()
{
    delete m_ui;
}

void RemotePage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RemotePage::showEvent(QShowEvent *event)
{
    // Reenable the next button if we already got at least one remote
    // This happens if you are going back to the name page and come again
    // later.
    if (this->m_ui->lvRemote->model()->rowCount() > 0) {
        // Call the slot to enable the next-button
        on_tbvRowCountChanged(QModelIndex(), 0, 0);
    }

    event->accept();
}

bool RemotePage::eventFilter(QObject* object, QEvent* event)
{
    if (event->type()==QEvent::KeyPress) {
        QKeyEvent* pKeyEvent=static_cast<QKeyEvent*>(event);
        //
        if (pKeyEvent->key() == Qt::Key_Delete) {
            if(this->m_ui->lvRemote->hasFocus()) {
                on_cmdRemove_clicked();
                //
                return true;
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void RemotePage::initializePage() {
    //
    if (m_ui->txtRemotePort->text().isEmpty()) {
        m_ui->txtRemotePort->setText(QLatin1String("1194"));
    }
    //
    m_ui->cmbRemoteProtocol->setCurrentIndex(1);
    m_ui->txtRemoteIP->clear();
}

void RemotePage::on_cmdAdd_clicked()
{
    if(m_ui->txtRemoteIP->text().isEmpty() || m_ui->txtRemotePort->text().isEmpty()) {
        //
        return;
    }

    // With more than 1 column we need to append a list to the model
    QList<QStandardItem*> newRow;

    // Add the columns
    newRow.append(new QStandardItem(m_ui->txtRemoteIP->text()));
    newRow.append(new QStandardItem(m_ui->txtRemotePort->text()));

    // Get the model
    QStandardItemModel *m = (QStandardItemModel*)this->m_ui->lvRemote->model();
    m->appendRow(newRow);

    // Reinit page
    initializePage();
}

void RemotePage::on_cmdRemove_clicked()
{
    QModelIndexList indexes = this->m_ui->lvRemote->selectionModel()->selectedRows();
    //
    while (!indexes.isEmpty()) {
        (QStandardItemModel*)this->m_ui->lvRemote->model()->removeRows(indexes.last().row(), 1);
        //
        indexes.removeLast();
    }
}

QStringList RemotePage::remoteList()
{
    QStringList list;
    QStandardItemModel *m = (QStandardItemModel*)this->m_ui->lvRemote->model();
    //
    for(int row = 0; row < m->rowCount(); ++row) {
        list.append(m->item(row, 0)->text() + QLatin1String(" ") +  m->item(row , 1)->text());
    }

    return list;
}
