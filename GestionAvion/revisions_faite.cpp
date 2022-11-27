#include "revisions_faite.h"
#include "ui_revisions_faite.h"


revisions_faite::revisions_faite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::revisions_faite)
{
    ui->setupUi(this);
    /*Data = QSqlDatabase::addDatabase("QODBC");
    Data.setDatabaseName(ACCESS);

    if(!Data.open()){
        QMessageBox::critical(this, "Error",Data.lastError().text());
        return;
    }*/

    revision = new QSqlTableModel(this);
    revision->setTable("Revision");
    revision->select();
    ui->tableView->setModel(revision);
}

revisions_faite::~revisions_faite()
{
    delete ui;
}

void revisions_faite::on_pushButton_clicked()
{

}

