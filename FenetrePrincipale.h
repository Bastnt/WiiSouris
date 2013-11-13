#ifndef FEN
#define FEN

#define VERSION "3.6"
#define INTERVALLE 30

#include <QtGui>
#include "wiimote.h"

class FenDemarre : public QWidget
{

    public:
        FenDemarre(QWidget * parent, Qt::WindowFlags f);
        void closeEvent(QCloseEvent *event);

};


class Fenetre : public QWidget
{
    Q_OBJECT

    public:
        Fenetre();
        ~Fenetre();
        void hideEvent ( QHideEvent * event );
        void closeEvent(QCloseEvent *event);


    public slots:
        void FConnection();
        void Lissage(int valeur);
        void continuer();
        void Restorer(QSystemTrayIcon::ActivationReason raseon = QSystemTrayIcon::DoubleClick);
        void about();

    private:
        QGroupBox *group_informations;
        QFormLayout *informations;
        QLabel *batterie;
        QProgressBar *batterie_info;
        QBoxLayout *layout_principal;
        wiimote *remote;
        QTimer *timer;
        FenDemarre *demarrage;
        QSlider *smooth;
        QCheckBox *souris;
        QLabel *precision;
        QPushButton *propos;
        QPushButton *quitter;
        QSystemTrayIcon *menu;
        QAction *strestorer;
        QAction *stquitter;
        QMenu *stmenu;


        int x, y, screen_width, screen_height;
        bool clic_gauche;



};

#endif
