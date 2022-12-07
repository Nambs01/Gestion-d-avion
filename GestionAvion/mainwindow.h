#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_saveAvion_clicked();
    void on_delAvion_clicked();
    void on_actAvion_clicked();

    void on_GestAvion_clicked();

    void on_GestPers_clicked();

    void on_GestVol_clicked();

    void on_pushButton_clicked();

    void on_searchAvionRev_clicked();

    void on_saveRev_clicked();

    void on_pushButton_2_clicked();

    void on_actualiserRev_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_sauv_1_clicked();

    void on_sauv_clicked();

    void on_sup_clicked();

    void on_sup_2_clicked();

    void on_actu_clicked();

    void on_actu_1_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_comboBox_2_highlighted(const QString &arg1);

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_saveVolDirect_clicked();

    void on_saveVolEsc_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_saveEquipage_clicked();

    void on_pushButton_12_clicked();

    void on_searchVol_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_username_editingFinished();

    void on_passwords_editingFinished();

    void on_sup_3_clicked();

    void on_pushButton_20_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase allData;
    QSqlTableModel *Avion;
    QSqlTableModel *avions_a_reviser;
    QSqlTableModel *Revision;
    QSqlTableModel *tab_nav;
    QSqlTableModel *tab_non_nav;
    QSqlTableModel *liste;
    QSqlTableModel *Equipage;
    QSqlTableModel *vol;
    QString total_Avion;
    QString total_PersonnelNav;
    QString total_PersonnelNonNav;
    QSqlQueryModel *avionSearched;
    QSqlQueryModel *volSUP;
};
#endif // MAINWINDOW_H
