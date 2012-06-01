#include "frmsaveddata.h"
#include "ui_frmsaveddata.h"
#include "preferences.h"

FrmSavedData::FrmSavedData(QStringList configs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmSavedData),
    data (configs)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
}

FrmSavedData::~FrmSavedData()
{
    delete ui;
}

void FrmSavedData::showEvent(QShowEvent *e)
{
    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left = Preferences::instance()->geometry().x();
    left = left + (Preferences::instance()->geometry().width() - winW) / 2;

    // Nun die neuen setzen
    this->setGeometry(left, (qApp->desktop()->height() / 2) - (winH / 2), winW, winH);
    //
    // Nun füllen
    this->refreshTree();
}

void FrmSavedData::on_pushButton_clicked()
{
    this->close();
}

void FrmSavedData::refreshTree()
{
    ui->trvSavedData->clear();
    QStringList  header;
    header << QObject::tr("ID") << QObject::tr("Name") << QObject::tr("User") << QObject::tr("Password") << QObject::tr("Pkcs12") << QObject::tr("HTTP User") << QObject::tr("HTTP Password");
    ui->trvSavedData->setColumnCount(7);
    ui->trvSavedData->setHeaderLabels(header);
    ui->trvSavedData->header()->setMovable(false);
    ui->trvSavedData->header()->hideSection(0);
    ui->trvSavedData->header()->show();

    // Nun die Liste durchlaufen und anfügen
    foreach(QString line, this->data) {
        QStringList fields (line.split(";"));
        if (fields.size() == 7) {
            // Append
            QTreeWidgetItem *item = new QTreeWidgetItem (ui->trvSavedData->invisibleRootItem());
            item->setText(0, fields.at(0));
            item->setText(1, fields.at(1));
            // Nun die Speicherdaten
            item->setText(2, (fields.at(2) == QLatin1String("1") ? QObject::tr("yes") : QObject::tr("no")));
            item->setText(3, (fields.at(3) == QLatin1String("1") ? QObject::tr("yes") : QObject::tr("no")));
            item->setText(4, (fields.at(4) == QLatin1String("1") ? QObject::tr("yes") : QObject::tr("no")));

            item->setText(5, (fields.at(5) == QLatin1String("1") ? QObject::tr("yes") : QObject::tr("no")));
            item->setText(6, (fields.at(6) == QLatin1String("1") ? QObject::tr("yes") : QObject::tr("no")));

            ui->trvSavedData->invisibleRootItem()->addChild(item);
        }
    }


    ui->trvSavedData->resizeColumnToContents(1);
    ui->trvSavedData->resizeColumnToContents(2);
    ui->trvSavedData->resizeColumnToContents(3);
    ui->trvSavedData->resizeColumnToContents(4);
    ui->trvSavedData->resizeColumnToContents(5);
    ui->trvSavedData->resizeColumnToContents(6);

}

