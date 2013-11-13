#include "FenetrePrincipale.h"
#include "wiimote.h"

Fenetre::Fenetre()
{
    // =====Démarrage Fenêtre=====
    setMinimumSize(250,350);
    QString titre("WiiSouris par Bastnt v");
    titre += VERSION;
    setWindowTitle(titre);
    setWindowIcon(QPixmap("icone.png"));
    clic_gauche = 0;
    screen_height = GetSystemMetrics(SM_CYSCREEN);
    screen_width = GetSystemMetrics(SM_CXSCREEN);
    // =====Démarrage Fenêtre=====


    // =====Information de la Wiimote=====
    group_informations = new QGroupBox("Informations");
    informations = new QFormLayout();
    batterie = new QLabel("Batterie :");
    QLabel *IRled = new QLabel();
    batterie_info = new QProgressBar;
    batterie_info->setValue(0);

    informations->addRow("&Batterie :", batterie_info);
    informations->addRow("&LED Visible :", IRled);
    group_informations->setLayout(informations);
    // =====Information de la Wiimote=====


    // =====Controle de la Wiimote=====
    QGroupBox *group_controle = new QGroupBox("Contrôle");
    QBoxLayout *controle = new QBoxLayout(QBoxLayout::TopToBottom);
    QLabel *info = new QLabel("<U><B>Boutons :</B></U><br /> - A : Clique (glisser/déposer)<br /> - B : Clique ponctuel<br /> - 1 : Clique droit<br /> - Home : Activer / Désactiver la souris<br /> - Flèches directionnelles<br /><hr />");
    info->setTextFormat(Qt::RichText);
    precision = new QLabel(QString("Lissage : %1").arg(INTERVALLE));
    smooth = new QSlider(Qt::Horizontal);
    smooth->setRange(10,100);
    smooth->setTickPosition(QSlider::TicksAbove);
    smooth->setValue(INTERVALLE);
    souris = new QCheckBox("Contrôle de la souris");
    souris->setCheckState(Qt::Checked);
    controle->addWidget(info);
    controle->addWidget(souris);
    controle->addWidget(precision);
    controle->addWidget(smooth);
    group_controle->setLayout(controle);

    QObject::connect(smooth, SIGNAL(valueChanged(int)), this, SLOT(Lissage(int)));
    // =====Controle de la Wiimote=====


    // ===== Boutons =====
    QBoxLayout *boutons = new QBoxLayout(QBoxLayout::LeftToRight);
    propos = new QPushButton(" A propos");
    quitter = new QPushButton("Quitter");
    boutons->addWidget(propos);
    boutons->addWidget(quitter);

    connect(quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(propos, SIGNAL(clicked()), this, SLOT(about()));
    // ===== Boutons =====



    // =====Initialisation fenêtre=====
    layout_principal = new QBoxLayout(QBoxLayout::TopToBottom);
    layout_principal->addWidget(group_informations);
    layout_principal->addWidget(group_controle);
    layout_principal->addLayout(boutons);

    setLayout(layout_principal);
    setDisabled(1);
    // =====Initialisation fenêtre=====


    // =====Démarrage de la wiimote=====
    demarrage = new FenDemarre(this, Qt::Tool);
        demarrage->show();

        remote = new wiimote();
        timer = new QTimer(this);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(FConnection()));
        timer->start(200);
    // =====Démarrage de la wiimote=====


    // =====Création du systemTray=====
    menu = new QSystemTrayIcon(QIcon(QPixmap("icone.png")), this);

    strestorer = new QAction(tr("&Restorer"), this);
     connect(strestorer, SIGNAL(triggered()), this, SLOT(Restorer()));
    stquitter = new QAction(tr("&Quitter"), this);
     connect(stquitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    stmenu = new QMenu(this);
    stmenu->addAction(strestorer);
    stmenu->addSeparator();
    stmenu->addAction(stquitter);
    menu->setContextMenu(stmenu);
    menu->setToolTip(titre);

    connect(menu, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(Restorer(QSystemTrayIcon::ActivationReason)));
    menu->show();
    // =====Création du systemTray=====

}

void Fenetre::FConnection() // Recher de la wiimote
{
        if(remote->Connect(wiimote::FIRST_AVAILABLE)) // Wiimote trouvé !
        {
            timer->stop();
            Beep(600,500);
            demarrage->hide();
            setDisabled(0);

            if(remote->bExtension)
                remote->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
            else
                remote->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots

            remote->SetLEDs((BYTE)(1<<0));
            remote->RefreshState();
            batterie_info->setValue(remote->BatteryPercent);
            QObject::disconnect(timer, 0, this, 0);
            QObject::connect(timer, SIGNAL(timeout()), this, SLOT(continuer()));
            timer->start(INTERVALLE);
        }
}


void Fenetre::continuer() // Boucle de programme
{
    if(remote->ConnectionLost())
        {
            QMessageBox::warning(this, "Erreur", "La connexion avec la wiimote a été perdu.");
            qApp->quit();
        }

       if(remote->RefreshState() != NO_CHANGE)
    {


    //=============Touches==================
        if (remote->Button.Left())
            keybd_event(VK_LEFT,0,0,0);
            else
            keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);

        if (remote->Button.Right())
            keybd_event(VK_RIGHT,0,0,0);
            else
            keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);


        if (remote->Button.Up())
            keybd_event(VK_UP,0,0,0);
            else
            keybd_event(VK_UP,0,KEYEVENTF_KEYUP,0);


        if (remote->Button.Down())
            keybd_event(VK_DOWN,0,0,0);
            else
            keybd_event(VK_DOWN,0,KEYEVENTF_KEYUP,0);


        if (remote->Button.A()){
            if(!clic_gauche)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                clic_gauche = 1;
            }
            }
            else if(clic_gauche)
            {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            clic_gauche=0;
            }

        if (remote->Button.B())
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(200);

        }

        if (remote->Button.One())
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            Sleep(200);

        }

        if (remote->Button.Home())
        {
            if(souris->isChecked())
            {
                souris->setCheckState(Qt::Unchecked);
                remote->SetLEDs(0);
            }
            else
            {
                souris->setCheckState(Qt::Checked);
                remote->SetLEDs((BYTE)(1<<0));
            }

            Sleep(300);
        }

    //=============Touches==================


    //=============Souris===================
        if(remote->IR.Dot[0].bVisible && souris->isChecked())
        {
            x = (remote->IR.Dot[0].X * screen_width);
            y = (remote->IR.Dot[0].Y * screen_height);
            SetCursorPos(x, y);
        }
    //=============Souris===================
    }

}

void Fenetre::Restorer(QSystemTrayIcon::ActivationReason raseon)
{
    if(raseon == QSystemTrayIcon::DoubleClick && isHidden())
    {
        setWindowFlags(Qt::Window);
        showNormal();
    }
}

void Fenetre::about()
{
    QMessageBox::information(this, "A propos de Wiisouris", "Wiisouris vous permet de contrôler votre souris avec une Wiimote.<br />Vous devez connecter votre manette par bluetooth.<br />Informations :<br /><ul><li>Wiisouris v3.6 license GPL</li><li>Codeur : Bastnt</li><li>langage : C++</li><li>Librairie Tierce : Qt</li></ul><br /><a href='http://bastnt.free.fr/'>bastnt.free.fr</a>(bastien45@gmail.com)");
}

void Fenetre::hideEvent ( QHideEvent * event ) //Notification lors de la réduction
{
    setWindowFlags(Qt::Tool);
    hide();
}



void Fenetre::Lissage(int valeur) //Changement de lissage
{
    QString resultat;
    resultat = QString("Lissage : %1").arg(valeur);
    precision->setText(resultat);
    timer->setInterval(valeur);

}


void Fenetre::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}


Fenetre::~Fenetre() //Destruction wiimote
{
    remote->Disconnect();
    delete remote;

}




























 FenDemarre::FenDemarre(QWidget *parent=0, Qt::WindowFlags f=0) : QWidget(parent, f) //Création fenetre de Démarrage
 {
    setWindowModality(Qt::WindowModal);
    setWindowTitle("Démarrage ...");
    QLabel *lancement = new QLabel("Recherche d'une wiimote ...");
    QPushButton *annuler = new QPushButton("Annuler");
    QBoxLayout *group_demarrage = new QBoxLayout(QBoxLayout::TopToBottom);
    group_demarrage->addWidget(lancement);
    group_demarrage->addWidget(annuler);
    setLayout(group_demarrage);


    QObject::connect(annuler, SIGNAL(clicked()), qApp, SLOT(quit()));
 }

 void FenDemarre::closeEvent(QCloseEvent *event) // Empêcher la fermeture
 {
    event->ignore();
 }
