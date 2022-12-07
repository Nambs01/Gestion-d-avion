#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>


#define ACCESS "DRIVER={Microsoft Access Driver (*.mdb)};"\
    "FIL={MS Access};DBQ=C:\\Users\\Gatien\\Documents\\Nouveau dossier\\FinL1.mdb"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setFixedSize(1021,591);
    ui->setupUi(this);
    allData = QSqlDatabase::addDatabase("QODBC");
    allData.setDatabaseName(ACCESS);

    if(!allData.open()){
        QMessageBox::critical(this, "Error",allData.lastError().text());
        return;
    }

    Avion = new QSqlTableModel(this);
    Avion->setTable("Avion");
    Avion->select();
    ui->listeAvion->setModel(Avion);

    ui->TYPAV_txt->addItem("Avion de ligne");
    ui->TYPAV_txt->addItem("Avion d'affaire");
    ui->TYPAV_txt->addItem("Avion léger");
    ui->TYPAV_txt->addItem("Avion ultra léger");

    avions_a_reviser = new QSqlTableModel(this);
    avions_a_reviser->setTable("avions_a_reviser");
    avions_a_reviser->select();
    ui->listeAvRev->setModel(avions_a_reviser);

    Revision = new QSqlTableModel(this);
    Revision->setTable("Revision");
    Revision->select();
    ui->tableRevision->setModel(Revision);

    tab_nav = new QSqlTableModel(this);
    tab_nav->setTable("tab_nav");
    tab_nav->select();
    ui->tab_nav->setModel(tab_nav);

    tab_non_nav = new QSqlTableModel(this);
    tab_non_nav->setTable("tab_non_nav");
    tab_non_nav->select();
    ui->tab_non_nav->setModel(tab_non_nav);

    Equipage = new QSqlTableModel(this);
    Equipage->setTable("Equipage");
    Equipage->select();
    ui->listEquipage->setModel(Equipage);

    liste = new QSqlTableModel(this);
    liste->setTable("avions_disponible");
    liste->select();
    ui->tableView->setModel(liste);

    vol = new QSqlTableModel(this);
    vol->setTable("vol");
    vol->select();
    ui->listeVol->setModel(vol);

    QSqlQuery j;
        j.exec("SELECT * FROM vol WHERE [JDEP] > Date()");
        avionSearched = new QSqlQueryModel;
        avionSearched->setQuery(j);
    ui->volSearched->setModel(avionSearched);
    ui->volSearched->show();

    QSqlQuery k;
    QString vol1="Oui";
        k.prepare("SELECT * FROM vol INNER JOIN Avion ON Avion.NUMAV = vol.NUMAV WHERE ([JDEP] < Date() AND Avion.[En vol]=:vol1 )");
        k.bindValue(":vol1", vol1);
        k.exec();
    volSUP = new QSqlQueryModel;
    volSUP->setQuery(k);
    ui->VolASupprimer->setModel(volSUP);
    ui->VolASupprimer->show();

    ui->stackedWidget->setCurrentIndex(8);
    ui->stackedWidget_2->setCurrentIndex(0);
    ui->stackedWidget_3->setCurrentIndex(1);
    ui->stackedWidget_4->setCurrentIndex(0);

    QSqlQuery z;
        z.exec("SELECT COUNT(NUMAV) AS Total FROM Avion");
        z.first();
        total_Avion = z.value(0).toString();
    ui->totalAvion->setText(total_Avion);

    QSqlQuery x;
        x.exec("SELECT COUNT([N°]) AS Total FROM tab_nav");
        x.first();
        total_PersonnelNav = x.value(0).toString();
    ui->persNav->setText(total_PersonnelNav);

    QSqlQuery y;
        y.exec("SELECT COUNT([N°]) AS Total FROM tab_non_nav");
        y.first();
        total_PersonnelNonNav = y.value(0).toString();
    ui->persNonNav->setText(total_PersonnelNonNav);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*/////////////////////////////////////////////////////////////
//////////////////////GESTION DES AVIONS///////////////////////
/////////////////////////////////////////////////////////////*/

void MainWindow::on_GestAvion_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_saveAvion_clicked()
{
    QString NUMAV= ui->NUMAV_txt->text();
    QString TYPAV=  ui->TYPAV_txt->currentText();
    QString DATMS= ui->DATMS_txt->text();
    QString vol = "Non";

    QSqlQuery s;
       s.prepare("INSERT INTO Avion(NUMAV,TYPAV,DATMS,DATDREV,[En vol]) VALUES (:NUMAV,:TYPAV,:DATMS,:DATDREV,:vol)");
       s.bindValue(":NUMAV",NUMAV);
       s.bindValue(":TYPAV",TYPAV);
       s.bindValue(":DATMS",DATMS);
       s.bindValue(":DATDREV",DATMS);
       s.bindValue(":vol",vol);

       if(NUMAV=="")
           QMessageBox::critical(this,"Error","Verifier votre formulaire, il y a une information vide");
       else{
           s.exec();
           QMessageBox::information(this, "Rapport", "Avion sauvegarder avec succes");
           Avion->select();
           ui->NUMAV_txt->clear();
       }
       QSqlQuery z;
           z.exec("SELECT COUNT(NUMAV) AS Total FROM Avion");
           z.first();
           total_Avion = z.value(0).toString();
       ui->totalAvion->setText(total_Avion);

}

void MainWindow::on_delAvion_clicked()
{
    QString numAvionSup = ui->numavSup->text();
    QString vol = "Non";
    QSqlQuery N;
        N.prepare("DELETE FROM Avion WHERE ((NUMAV=:numAvionSup) AND ([En vol] = :vol))");
        N.bindValue(":numAvionSup", numAvionSup);
        N.bindValue(":vol", vol);

    QMessageBox::StandardButton reply;
    if(!(numAvionSup=="")){
        reply = QMessageBox::question(this, "Rapport", "Êtes-vous sure de vouloir supprimer cette avion?",QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes){
            N.exec();
            Avion->select();
            ui->numavSup->clear();
        }
        QSqlQuery z;
            z.exec("SELECT COUNT(NUMAV) AS Total FROM Avion");
            z.first();
            total_Avion = z.value(0).toString();
        ui->totalAvion->setText(total_Avion);

    }
    else
         QMessageBox::critical(this,"Error","Entrer le numero de l'avion à supprimer");
}


void MainWindow::on_actAvion_clicked()
{
    Avion->select();
    QSqlQuery z;
        z.exec("SELECT COUNT(NUMAV) AS Total FROM Avion");
        z.first();
        total_Avion = z.value(0).toString();
    ui->totalAvion->setText(total_Avion);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery o;
        o.prepare("DELETE * FROM avions_a_reviser");
        o.exec();

    QSqlQuery p;
    p.prepare("INSERT INTO avions_a_reviser ( NUMAV, TYPAV, NBHDDREV, mois ) SELECT Avion.NUMAV, Avion.TYPAV, Avion.NBHDDREV, DateDiff(:m,[DATDREV],Date()) AS mois FROM Avion WHERE (((Avion.NBHDDREV)>=1000)) OR (((DateDiff(:m,[DATDREV],Date()))>=6));");
        p.bindValue(":m", "m");
        p.exec();
        avions_a_reviser->select();
   ui->stackedWidget->setCurrentIndex(2);
}

//REVISION
void MainWindow::on_searchAvionRev_clicked()
{
    /*/////////////////////*/
    ui->numAvRev->clear();
    ui->TypAvRev->clear();
    ui->nbrH->clear();
    /*/////////////////////*/

    QString NumAvionRev = ui->searchAvionRev_txt->text();
    QDate datrev;
    QString numavrev;
    QString typavrev;
    QString nbrh;
    QSqlQuery search;
    search.prepare("SELECT * FROM avions_a_reviser WHERE (NUMAV = :NumAvionRev)");
        search.bindValue(":NumAvionRev", NumAvionRev);
    if(search.exec()){
        while(search.next()){
            numavrev = search.value(0).toString();
            typavrev = search.value(1).toString();
            nbrh = search.value(2).toString();
            datrev = QDate::currentDate();
        }
    }

    ui->numAvRev->insert(numavrev);
    ui->TypAvRev->insert(typavrev);
    ui->nbrH->insert(nbrh);
    ui->dateRev->setDate(datrev);

}

void MainWindow::on_saveRev_clicked()
{
    QString numav = ui->numAvRev->text();
    QString texte = ui->texteRev->toPlainText();
    QString datrev = ui->dateRev->text();
    QString nbhrev = ui->nbrH->text();

    QSqlQuery r;
    QSqlQuery m;
    QSqlQuery n;
        r.prepare("INSERT INTO Revision(NUMAV,TEXTE,DATREV,NBHREV) VALUES (:NUMAV,:TEXTE,:DATREV,:NBHREV)");
        r.bindValue(":NUMAV",numav);
        r.bindValue(":TEXTE",texte);
        r.bindValue(":DATREV",datrev);
        r.bindValue(":NBHREV",nbhrev);
        if(numav=="" || texte=="" || datrev=="" || nbhrev==""){
            QMessageBox::critical(this,"Error","Verifier votre formulaire de révision, il y a une information vide");
         }
         else{
            r.exec();
            m.prepare("DELETE FROM avions_a_reviser WHERE NUMAV = :NUMAV");
            m.bindValue(":NUMAV",numav);
            m.exec();

            n.prepare("UPDATE Avion SET NBHDDREV = 0, DATDREV = :DATDREV WHERE NUMAV = :NUMAV");
            n.bindValue(":NUMAV",numav);
            n.bindValue(":DATDREV",datrev);
            n.exec();
            QMessageBox::information(this, "Rapport", "Avion révisé avec succes");
            ui->numAvRev->clear();
            ui->texteRev->clear();
            ui->dateRev->clear();
            ui->nbrH->clear();
            ui->TypAvRev->clear();
        }
    Avion->select();
    avions_a_reviser->select();

}

void MainWindow::on_actualiserRev_clicked()
{
    avions_a_reviser->select();
    ui->numAvRev->clear();
    ui->TypAvRev->clear();
    ui->nbrH->clear();
    ui->searchAvionRev_txt->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


//OUVRIR REVISION FAITES
void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    Revision->select();
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}



/*/////////////////////////////////////////////////
/////////////GESTION DES PERSONNELS////////////////
/////////////////////////////////////////////////*/
void MainWindow::on_GestPers_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    int mois_en_cours = QDate::currentDate().month();
    QSqlQuery m;
    m.prepare("UPDATE mois, tab_nav SET tab_nav.NBMHV = 0, mois.Mois = :mois_en_cours WHERE (((mois.Mois)<>:mois_en_cours));");
    m.bindValue(":mois_en_cours", mois_en_cours);
    m.exec();

    tab_nav->select();
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1=="Navigants"){
        ui->stackedWidget_4->setCurrentIndex(0);
        ui->stackedWidget_2->setCurrentIndex(0);
    }else{
        ui->stackedWidget_4->setCurrentIndex(1);
        ui->stackedWidget_2->setCurrentIndex(1);
    }
}

void MainWindow::on_sauv_1_clicked()
{
    QString nom = ui->nom_2->text();
    QString prenom = ui->prenom_2->text();
    QString adresse = ui->adresse_2->text();
    QString tel = ui->tel_4->text();
    QString fonction = ui->fonction_2->currentText();
    QString salaire = ui->salaire_2->text();
    QString date = ui->date_2->text();

    QSqlQuery z;
    z.prepare("INSERT INTO tab_non_nav(Nom,Prenom,Adresse,Telephone,Salaire,Fonction,DATEMB) VALUES (:nom,:prenom,:adresse,:tel,:salaire,:fonction,:date)");
    z.bindValue(":nom",nom);
    z.bindValue(":prenom",prenom);
    z.bindValue(":adresse",adresse);
    z.bindValue(":tel",tel);
    z.bindValue(":fonction",fonction);
    z.bindValue(":date",date);
    z.bindValue(":salaire",salaire);

    if(nom=="" || prenom=="" || adresse=="" || tel=="" || fonction=="" || salaire=="0" || date==""){
        QMessageBox::critical(this,"Error","Verifier votre formulaire, il y a une information vide");
    }
    else {
       z.exec();
       QMessageBox::information(this,"Rapport","Peronnel enregistrer avec succes");
       ui->nom_2->clear();
       ui->prenom_2->clear();
       ui->adresse_2->clear();
       ui->tel_4->clear();
       ui->salaire_2->clear();
       ui->date_2->clear();
       ui->fonction_2->setCurrentIndex(0);
    }

    tab_non_nav->select();

    QSqlQuery y;
        y.exec("SELECT COUNT([N°]) AS Total FROM tab_non_nav");
        y.first();
        total_PersonnelNonNav = y.value(0).toString();
    ui->persNonNav->setText(total_PersonnelNonNav);
}

void MainWindow::on_sauv_clicked()
{
    QString numPerso = ui->numPerso->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString adresse = ui->adresse->text();
    QString tel = ui->tel->text();
    QString fonction = ui->fonction->currentText();
    QString salaire = ui->salaire->text();
    QString date = ui->date->text();

    QSqlQuery a;
    a.prepare("INSERT INTO tab_nav(N°,Nom,Prenom,Adresse,Telephone,Salaire,Fonction,DATEMB) VALUES (:numPerso,:nom,:prenom,:adresse,:tel,:salaire,:fonction,:date)");
    a.bindValue(":numPerso", numPerso);
    a.bindValue(":nom",nom);
    a.bindValue(":prenom",prenom);
    a.bindValue(":adresse",adresse);
    a.bindValue(":tel",tel);
    a.bindValue(":fonction",fonction);
    a.bindValue(":date",date);
    a.bindValue(":salaire",salaire);

    if(numPerso=="" || nom=="" || prenom=="" || adresse=="" || tel=="" || fonction=="" || salaire=="0" || date==""){
        QMessageBox::critical(this,"Error","Verifier votre formulaire, il y a une information vide");
    }
    else {
        a.exec();
        QMessageBox::information(this,"Rapport","Peronnel enregistrer avec succes");
        ui->numPerso->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->adresse->clear();
        ui->tel->clear();
        ui->salaire->clear();
        ui->date->clear();
        ui->fonction->setCurrentIndex(0);
        tab_nav->select();
    }

    QSqlQuery x;
        x.exec("SELECT COUNT([N°]) AS Total FROM tab_nav");
        x.first();
        total_PersonnelNav = x.value(0).toString();
    ui->persNav->setText(total_PersonnelNav);
}

void MainWindow::on_sup_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Rapport", "Êtes-vous sure de vouloir supprimer cet personnel?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){
      QMessageBox::information(this,"Rapport","Peronnel supprimer avec succes");
      tab_nav->removeRow(ui->tab_nav->currentIndex().row());
    }

    QSqlQuery x;
        x.exec("SELECT COUNT([N°]) AS Total FROM tab_nav");
        x.first();
        total_PersonnelNav = x.value(0).toString();
    ui->persNav->setText(total_PersonnelNav);
    tab_nav->select();
}

void MainWindow::on_sup_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Rapport", "Êtes-vous sure de vouloir supprimer cet personnel?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){
      QMessageBox::information(this,"Rapport","Peronnel supprimer avec succes");
      tab_non_nav->removeRow(ui->tab_non_nav->currentIndex().row());
    }

    QSqlQuery y;
        y.exec("SELECT COUNT([N°]) AS Total FROM tab_non_nav");
        y.first();
        total_PersonnelNonNav = y.value(0).toString();
    ui->persNonNav->setText(total_PersonnelNonNav);
    tab_non_nav->select();
}

void MainWindow::on_actu_clicked()
{
    ui->nom->clear();
    ui->prenom->clear();
    ui->adresse->clear();
    ui->tel->clear();
    ui->salaire->clear();
    ui->date->clear();

    tab_nav->select();

    QSqlQuery x;
        x.exec("SELECT COUNT([N°]) AS Total FROM tab_nav");
        x.first();
        total_PersonnelNav = x.value(0).toString();
    ui->persNav->setText(total_PersonnelNav);
}

void MainWindow::on_actu_1_clicked()
{
    ui->nom_2->clear();
    ui->prenom_2->clear();
    ui->adresse_2->clear();
    ui->tel_4->clear();
    ui->salaire_2->clear();
    ui->date_2->clear();

    tab_non_nav->select();

    QSqlQuery y;
        y.exec("SELECT COUNT([N°]) AS Total FROM tab_non_nav");
        y.first();
        total_PersonnelNonNav = y.value(0).toString();
    ui->persNonNav->setText(total_PersonnelNonNav);
}


void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


/*
    Sauvegarder un nouveau Equipage
*/
void MainWindow::on_saveEquipage_clicked()
{
    QString pilote1 = ui->pilote1->text();
    QString pilote2 = ui->pilote2->text();
    QString hotesse1 = ui->hotesse1->text();
    QString hotesse2 = ui->hotesse2->text();
    QString hotesse3 = ui->hotesse3->text();
    QString hotesse4 = ui->hotesse4->text();
    QString steward1 = ui->steward1->text();
    QString steward2 = ui->steward2->text();
    QString vol ="Non";

    QSqlQuery a;
    a.prepare("INSERT INTO Equipage([ID Pilote1], [ID Pilote2], [ID Hôtesse1], [ID Hôtesse2], [ID Hôtesse3], [ID Hôtesse4], [ID Steward1], [ID Steward2], [En vol]) VALUES (:pilote1, :pilote2, :hotesse1, :hotesse2, :hotesse3, :hotesse4, :steward1, :steward2, :vol);");
    a.bindValue(":pilote1", pilote1);
    a.bindValue(":pilote2", pilote2);
    a.bindValue(":hotesse1", hotesse1);
    a.bindValue(":hotesse2", hotesse2);
    a.bindValue(":hotesse3", hotesse3);
    a.bindValue(":hotesse4", hotesse4);
    a.bindValue(":steward1", steward1);
    a.bindValue(":steward2", steward2);
    a.bindValue(":vol", vol);

    if(pilote1=="" || pilote2=="" || hotesse1=="" || hotesse2=="" || hotesse3=="" || hotesse4=="" || steward1=="" || steward2=="")
        QMessageBox::critical(this,"Error","Equipage incomplet");
    else{
       a.exec();
       QMessageBox::information(this,"Rapport","Equipage enregistrer avec succes");
       ui->pilote1->clear();
       ui->pilote2->clear();
       ui->hotesse1->clear();
       ui->hotesse2->clear();
       ui->hotesse3->clear();
       ui->hotesse4->clear();
       ui->steward1->clear();
       ui->steward2->clear();
    }

    Equipage->select();

}


void MainWindow::on_sup_3_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Rapport", "Êtes-vous sure de vouloir supprimer cet equipage?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){
      Equipage->removeRow(ui->listEquipage->currentIndex().row());
      QMessageBox::information(this,"Rapport","Equipage supprimer avec succes");
    }
    Equipage->select();
}

/*/////////////////////////////////////////////////
////////////////GESTION DES VOLS///////////////////
/////////////////////////////////////////////////*/
void MainWindow::on_GestVol_clicked()
{
 ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_comboBox_2_highlighted(const QString &arg1)
{

    if (arg1=="Avion") {
        liste = new QSqlTableModel(this);
        liste->setTable("avions_disponible");
        liste->select();
        ui->tableView->setModel(liste);
    }else{
        liste = new QSqlTableModel(this);
        liste->setTable("equipage_dispo");
        liste->select();
        ui->tableView->setModel(liste);
    }
}

void MainWindow::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    if(arg1=="0"){
        ui->stackedWidget_3->setCurrentIndex(1);
    }
    else{
        ui->stackedWidget_3->setCurrentIndex(0);
    }
}


void MainWindow::on_saveVolDirect_clicked()
{
    QString vildep = ui->vilDep->text();
    QString vilarr = ui->vilArr->text();
    QString jdep = ui->jdepTxt->text();
    QString hdep = ui->hDep->text();
    QString durvol = ui->durVol->text();
    QString numav = ui->numav->text();
    QString equipage = ui->numEq->text();

    QSqlQuery a;
    QSqlQuery b1;
    QSqlQuery b;
    QSqlQuery c;
    QSqlQuery d;
    QSqlQuery e;
    QSqlQuery f;
    QSqlQuery g;
    QSqlQuery h;
    QSqlQuery i;
    QSqlQuery j;


    a.prepare("INSERT INTO vol(VILDEP, VILARR, JDEP, HDEP, DURVOL, NUMAV, Equipage) VALUES (:vildep, :vilarr, :jdep, :hdep, :durvol, :numav, :equipage);");
    a.bindValue(":vildep",vildep);
    a.bindValue(":vilarr",vilarr);
    a.bindValue(":jdep", jdep);
    a.bindValue(":hdep",hdep);
    a.bindValue(":durvol",durvol);
    a.bindValue(":numav",numav);
    a.bindValue(":equipage",equipage);

    if(vildep=="" || vilarr=="" || durvol=="0" || numav=="" || equipage=="")
        QMessageBox::critical(this,"Error","Verifier votre formulaire, il y a une information vide");

    else{
        a.exec();

        vol->select();

        QString vol = "Oui";
        b.prepare("UPDATE Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV SET Avion.NBHDDREV=[DURVOL]+[NBHDDREV], Avion.[En vol]=:vol  WHERE (((Avion.NUMAV)=:numav));");
        b.bindValue(":numav", numav);
        b.bindValue(":vol", vol);
        b.exec();

        b1.prepare("UPDATE Equipage INNER JOIN vol ON Equipage.[N° Equipage] = vol.Equipage SET Equipage.[En vol]=:vol  WHERE ((Equipage.[N° Equipage]) = :equipage) ;");
        b1.bindValue(":equipage", equipage);
        b1.bindValue(":vol", vol);
        b1.exec();

        c.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Pilote1]) AND ((vol.NUMAV)=:numav));");
        c.bindValue(":numav", numav);
        c.exec();

        d.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Pilote2]) AND ((vol.NUMAV)=:numav));");
        d.bindValue(":numav", numav);
        d.exec();

        e.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse1]) AND ((vol.NUMAV)=:numav));");
        e.bindValue(":numav",numav);
        e.exec();

        f.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse2]) AND ((vol.NUMAV)=:numav));");
        f.bindValue(":numav",numav);
        f.exec();

        g.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse3]) AND ((vol.NUMAV)=:numav));");
        g.bindValue(":numav",numav);
        g.exec();

        h.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse4]) AND ((vol.NUMAV)=:numav));");
        h.bindValue(":numav",numav);
        h.exec();

        i.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Steward1]) AND ((vol.NUMAV)=:numav));");
        i.bindValue(":numav",numav);
        i.exec();

        j.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Steward2]) AND ((vol.NUMAV)=:numav));");
        j.bindValue(":numav",numav);
        j.exec();

        tab_nav->select();

        QMessageBox::information(this,"Rapport","Vol enregistré avec succes");
        ui->vilDep->clear();
        ui->vilArr->clear();
        ui->jdepTxt->text();
        ui->hDep->text();
        ui->durVol->clear();
        ui->numav->clear();
        ui->numEq->clear();
    }
    liste->select();
}


void MainWindow::on_saveVolEsc_clicked()
{
    QString vildep = ui->vilDep->text();
    QString vilarr = ui->vilArr->text();
    QString jdep = ui->jdepTxt->text();
    QString hdep = ui->hDep->text();
    QString durvol = ui->durVol->text();
    QString numav = ui->numav->text();
    QString equipage = ui->numEq->text();

    QString villeEsc1 = ui->villeEsc1->text();
    QString numav2 = ui->numAvEsc1->text();
    QString durvol2 = ui->durvolEsc1->text();
    QString equipage2 = ui->equipEsc1->text();

    QSqlQuery a;
        a.prepare("INSERT INTO vol(VILDEP, VILARR, JDEP, HDEP, DURVOL, NUMAV, Equipage, VILLESC, DURVOL2, NUMAV2, Equipage2) VALUES (:vildep, :vilarr, :jdep, :hdep, :durvol, :numav, :equipage, :villesc, :durvol2,:numav2, :equipage2);");
        a.bindValue(":vildep", vildep);
        a.bindValue(":vilarr", vilarr);
        a.bindValue(":jdep", jdep);
        a.bindValue(":hdep", hdep);
        a.bindValue(":durvol", durvol);
        a.bindValue(":numav", numav);
        a.bindValue(":equipage", equipage);
        a.bindValue(":villesc", villeEsc1);
        a.bindValue(":numav2", numav2);
        a.bindValue(":durvol2", durvol2);
        a.bindValue(":equipage2", equipage2);

        if(vildep=="" || vilarr=="" || durvol=="0" || numav=="" || jdep=="" || equipage=="" || villeEsc1=="" || numav2=="" || durvol2=="0" || equipage2=="")
            QMessageBox::critical(this,"Error","Verifier votre formulaire, il y a une information vide");
        else{
            a.exec();

            vol->select();

        /*
            Mise a jour heure des deux avions
        */
            QString vol = "Oui";
            QSqlQuery A;
                A.prepare("UPDATE Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV SET Avion.NBHDDREV = [DURVOL]+[NBHDDREV], Avion.[En vol]=:vol WHERE (((Avion.NUMAV)=:numav));");
                A.bindValue(":numav", numav);
                A.bindValue(":vol", vol);
                A.exec();
            QSqlQuery A1;
            A1.prepare("UPDATE Equipage INNER JOIN vol ON Equipage.[N° Equipage] = vol.Equipage SET Equipage.[En vol]=:vol  WHERE (Equipage.[N° Equipage])=:equipage;");
                A1.bindValue(":vol", vol);
                A1.bindValue(":equipage", equipage);
                A1.exec();


            QSqlQuery B;
                B.prepare("UPDATE Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2 SET Avion.NBHDDREV = [DURVOL2]+[NBHDDREV], Avion.[En vol]=:vol WHERE (((Avion.NUMAV)=:numav));");
                B.bindValue(":numav", numav2);
                B.bindValue(":vol", vol);
                B.exec();
            QSqlQuery B1;
            B1.prepare("UPDATE Equipage INNER JOIN vol ON Equipage.[N° Equipage] = vol.Equipage2 SET Equipage.[En vol]=:vol  WHERE (Equipage[N° Equipage])=:equipage;");
                B1.bindValue(":vol", vol);
                B1.bindValue(":equipage", equipage2);
                B1.exec();


        /*
            Mise a jour heure des personnel dans le premier avion
        */
            QSqlQuery c;
                c.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Pilote1]) AND ((vol.NUMAV)=:numav));");
                c.bindValue(":numav", numav);
                c.exec();

            QSqlQuery d;
                d.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Pilote2]) AND ((vol.NUMAV)=:numav));");
                d.bindValue(":numav", numav);
                d.exec();

            QSqlQuery e;
                e.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse1]) AND ((vol.NUMAV)=:numav));");
                e.bindValue(":numav",numav);
                e.exec();

            QSqlQuery f;
                f.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse2]) AND ((vol.NUMAV)=:numav));");
                f.bindValue(":numav",numav);
                f.exec();

            QSqlQuery g;
                g.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse3]) AND ((vol.NUMAV)=:numav));");
                g.bindValue(":numav",numav);
                g.exec();

            QSqlQuery h;
                h.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Hôtesse4]) AND ((vol.NUMAV)=:numav));");
                h.bindValue(":nummav",numav);
                h.exec();

            QSqlQuery i;
                i.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Steward1]) AND ((vol.NUMAV)=:numav));");
                i.bindValue(":numav",numav);
                i.exec();


            QSqlQuery j;
                j.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV) ON Equipage.[N° Equiapage] = vol.Equipage SET tab_nav.NBMHV = [NBMHV]+[DURVOL], tab_nav.NBTHV = [NBTHV]+[DURVOL] WHERE (((tab_nav.N°)=[ID Steward2]) AND ((vol.NUMAV)=:numav));");
                j.bindValue(":numav",numav);
                j.exec();


        /*
            Mise a jour heure des personnel dans la deuxième avion
        */
            QSqlQuery O;
                O.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Pilote1]) AND ((vol.NUMAV2)=:numav));");
                O.bindValue(":numav",numav2);
                O.exec();

            QSqlQuery P;
                P.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Pilote2]) AND ((vol.NUMAV2)=:numav));");
                P.bindValue(":numav",numav2);
                P.exec();

            QSqlQuery Q;
                Q.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Steward1]) AND ((vol.NUMAV2)=:numav));");
                Q.bindValue(":numav",numav2);
                Q.exec();

            QSqlQuery R;
                R.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Steward2]) AND ((vol.NUMAV2)=:numav));");
                R.bindValue(":numav",numav2);
                R.exec();

            QSqlQuery S;
                S.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Hôtesse1]) AND ((vol.NUMAV2)=:numav));");
                S.bindValue(":numav",numav2);
                S.exec();

            QSqlQuery T;
                T.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Hôtesse2]) AND ((vol.NUMAV2)=:numav));");
                T.bindValue(":numav",numav2);
                T.exec();

            QSqlQuery U;
                U.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Hôtesse3]) AND ((vol.NUMAV2)=:numav));");
                U.bindValue(":numav",numav2);
                U.exec();

            QSqlQuery V;
                V.prepare("UPDATE tab_nav, Equipage INNER JOIN (Avion INNER JOIN vol ON Avion.NUMAV = vol.NUMAV2) ON Equipage.[N° Equiapage] = vol.Equipage2 SET tab_nav.NBMHV = [NBMHV]+[DURVOL2], tab_nav.NBTHV = [NBTHV]+[DURVOL2] WHERE (((tab_nav.N°)=[ID Hôtesse4]) AND ((vol.NUMAV2)=:numav));");
                V.bindValue(":numav",numav2);
                V.exec();

            liste->select();
            tab_nav->select();
            QMessageBox::information(this,"Rapport","Vol enregistré avec succes");
            ui->vilDep->clear();
            ui->vilArr->clear();
            ui->jdepTxt->text();
            ui->hDep->text();
            ui->durVol->clear();
            ui->numav->clear();
            ui->numEq->clear();

            ui->villeEsc1->clear();
            ui->numAvEsc1->clear();
            ui->durvolEsc1->clear();
            ui->equipEsc1->clear();
        }

}

void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

//VOL
void MainWindow::on_searchVol_clicked()
{
    QString numvol = ui->searchNumvol->text();
    QString villearr = ui->villeArrTxt->text();
    QString villedep = ui->villeDepTxt->text();
    if(numvol!=""){
        QSqlQuery T;
            T.prepare("SELECT NUMVOL, VILDEP, VILARR, HDEP, DURVOL, NUMAV, VILLESC, DURVOL2, NUMAV2 FROM vol WHERE ( [JDEP] > Date() AND NUMVOL =:numvol);");
            T.bindValue(":numvol", numvol);
            T.exec();

        avionSearched = new QSqlQueryModel;
        avionSearched->setQuery(T);
        ui->volSearched->setModel(avionSearched);
        ui->volSearched->show();
    };
    if(villearr!=""){
        QSqlQuery M;
            M.prepare("SELECT NUMVOL, VILDEP, VILARR, JDEP, HDEP, DURVOL, NUMAV, VILLESC, DURVOL2, NUMAV2 FROM vol WHERE ( [JDEP] > Date() AND VILARR =:villearr);");
            M.bindValue(":villearr", villearr);
            M.exec();

        avionSearched = new QSqlQueryModel;
        avionSearched->setQuery(M);
        ui->volSearched->setModel(avionSearched);
        ui->volSearched->show();
    }

    if(villedep!=""){
        QSqlQuery Z;
            Z.prepare("SELECT NUMVOL, VILDEP, VILARR, JDEP, HDEP, DURVOL, NUMAV, VILLESC, DURVOL2, NUMAV2 FROM vol WHERE ( [JDEP] > Date() AND VILDEP =:vildep);");
            Z.bindValue(":vildep", villedep);
            Z.exec();

        avionSearched = new QSqlQueryModel;
        avionSearched->setQuery(Z);
        ui->volSearched->setModel(avionSearched);
        ui->volSearched->show();
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    QSqlQuery j;
        j.exec("SELECT * FROM vol WHERE [JDEP] > Date()");
        avionSearched = new QSqlQueryModel;
        avionSearched->setQuery(j);
    ui->volSearched->setModel(avionSearched);
    ui->volSearched->show();

}

void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_pushButton_16_clicked()
{
    QString volSup = ui->numVolSup->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Rapport", "Êtes-vous sure de vouloir supprimer cet vol?",QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes){
            QSqlQuery p;
                p.prepare("SELECT NUMAV FROM vol WHERE NUMVOL=:volSup");
                p.bindValue(":volSup", volSup);
                p.exec();
                p.first();
                QString numav = p.value(0).toString();

            QSqlQuery q;
                q.prepare("SELECT NUMAV2 FROM vol WHERE NUMVOL=:volSup");
                q.bindValue(":volSup", volSup);
                q.exec();
                q.first();
                QString numav2 = q.value(0).toString();

            QSqlQuery p1;
                p1.prepare("SELECT Equipage FROM vol WHERE NUMVOL=:volSup");
                p1.bindValue(":volSup", volSup);
                p1.exec();
                p1.first();
                QString equip = p1.value(0).toString();

            QSqlQuery p2;
                p2.prepare("SELECT Equipage2 FROM vol WHERE NUMVOL=:volSup");
                p2.bindValue(":volSup", volSup);
                p2.exec();
                p2.first();
                QString equip2 = p2.value(0).toString();

            QString enVol = "Non";
            QSqlQuery r;
                r.prepare("UPDATE Avion SET Avion.[En vol]=:enVol WHERE (((Avion.NUMAV)=:numav));");
                r.bindValue(":enVol", enVol);
                r.bindValue(":numav", numav);
                r.exec();

            QSqlQuery s;
                s.prepare("UPDATE Avion SET Avion.[En vol]=:enVol WHERE (((Avion.NUMAV)=:numav2));");
                s.bindValue(":enVol", enVol);
                s.bindValue(":numav2", numav2);
                s.exec();

            QSqlQuery r1;
            r1.prepare("UPDATE Equipage SET Equipage.[En vol]=:enVol WHERE (((Equipage.[N° Equipage])=:equip));");
                r1.bindValue(":enVol", enVol);
                r1.bindValue(":equip", equip);
                r1.exec();

            QSqlQuery r2;
                r2.prepare("UPDATE Equipage SET Equipage.[En vol]=:enVol WHERE (((Equipage.[N° Equipage])=:equip2));");
                r2.bindValue(":enVol", enVol);
                r2.bindValue(":equip2", equip2);
                r2.exec();

            QSqlQuery w;
                w.prepare("DELETE FROM vol WHERE [NUMVOL]=:volSup");
                w.bindValue(":volSup", volSup);
                w.exec();

            vol->select();
            QSqlQuery k;
            QString vol1="Oui";
                k.prepare("SELECT * FROM vol INNER JOIN Avion ON Avion.NUMAV = vol.NUMAV WHERE ([JDEP] < Date() AND Avion.[En vol]=:vol1 )");
                k.bindValue(":vol1", vol1);
                k.exec();
            volSUP = new QSqlQueryModel;
            volSUP->setQuery(k);
            ui->VolASupprimer->setModel(volSUP);
            ui->VolASupprimer->show();
        }
}

void MainWindow::on_pushButton_17_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButton_18_clicked()
{
    QString username=ui->username->text();
    QString passwords=ui->passwords->text();
    if(username=="admin" && passwords=="admin@2022"){
        ui->username->clear();
        ui->passwords->clear();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
        ui->notification->setText("Nom d'utilisateur ou mots de passes incorrecte");
}

void MainWindow::on_pushButton_19_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->notification->setText("");
    ui->username->clear();
    ui->passwords->clear();
}

void MainWindow::on_username_editingFinished()
{
    ui->notification->setText("");
}

void MainWindow::on_passwords_editingFinished()
{
    ui->notification->setText("");
}

void MainWindow::on_pushButton_20_clicked()
{
    QString volSup = ui->numVolSup->text();
    QSqlQuery p;
        p.prepare("SELECT NUMAV FROM vol WHERE NUMVOL=:volSup");
        p.bindValue(":volSup", volSup);
        p.exec();
        p.first();
        QString numav = p.value(0).toString();

    QSqlQuery q;
        q.prepare("SELECT NUMAV2 FROM vol WHERE NUMVOL=:volSup");
        q.bindValue(":volSup", volSup);
        q.exec();
        q.first();
        QString numav2 = q.value(0).toString();

    QSqlQuery p1;
        p1.prepare("SELECT Equipage FROM vol WHERE NUMVOL=:volSup");
        p1.bindValue(":volSup", volSup);
        p1.exec();
        p1.first();
        QString equip = p1.value(0).toString();

    QSqlQuery p2;
        p2.prepare("SELECT Equipage2 FROM vol WHERE NUMVOL=:volSup");
        p2.bindValue(":volSup", volSup);
        p2.exec();
        p2.first();
        QString equip2 = p2.value(0).toString();

    QString enVol = "Non";
    QSqlQuery r;
        r.prepare("UPDATE Avion SET Avion.[En vol]=:enVol WHERE ((Avion.NUMAV)=:numav);");

        r.bindValue(":enVol", enVol);
        r.bindValue(":numav", numav);
        r.exec();

    QSqlQuery s;
        s.prepare("UPDATE Avion SET Avion.[En vol]=:enVol WHERE ((Avion.NUMAV)=:numav2);");
        s.bindValue(":enVol", enVol);
        s.bindValue(":numav2", numav2);
        s.exec();

    QSqlQuery r1;
    r1.prepare("UPDATE Equipage SET Equipage.[En vol]=:enVol WHERE ((Equipage.[N° Equipage])=:equip);");
        r1.bindValue(":enVol", enVol);
        r1.bindValue(":equip", equip);
        r1.exec();

    QSqlQuery r2;
        r2.prepare("UPDATE Equipage SET Equipage.[En vol]=:enVol WHERE ((Equipage.[N° Equipage])=:equip2);");
        r2.bindValue(":enVol", enVol);
        r2.bindValue(":equip2", equip2);
        r2.exec();

    QSqlQuery k;
    QString vol1="Oui";
    k.prepare("SELECT * FROM vol INNER JOIN Avion ON Avion.NUMAV = vol.NUMAV WHERE ([JDEP] < Date() AND Avion.[En vol]=:vol1)");
        k.bindValue(":vol1", vol1);
        k.exec();
    volSUP = new QSqlQueryModel;
    volSUP->setQuery(k);
    ui->VolASupprimer->setModel(volSUP);
    ui->VolASupprimer->show();

    QMessageBox::information(this,"Rapport","Vol liberer avec succes");
}
