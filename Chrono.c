/* Auteur : MAHESWARAN Theepak
 * Création : 11-02-20
 * Modification : 24-02-20 */

#define _DEFAULT_SOURCE

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <assert.h>

typedef struct chrono {
    int etat;
    int temps;
    int avertissement;
    int dernier_tour;
    int nbr_tour;
    int tableau[3];
} Chronometre;

/* initialser_chronometre:                                                     *
 * Ne prend pas de paramètres.                                                  *
 * Renvoie une structure "Chronometre".                                        *
 * Le but est d'initialiser une structure avec toutes les informations à zéro. */
Chronometre initialser_chronometre() {
    Chronometre chrono;
    chrono.etat = 0;
    chrono.temps = 0;
    chrono.avertissement = 25000;
    chrono.dernier_tour = 0;
    chrono.nbr_tour = 0;
    chrono.tableau[0] = 0;
    chrono.tableau[1] = 0;
    chrono.tableau[2] = 0;
    return chrono;
}

/* intervalle_ms:                                                                  *
 * Prend en paramètre, deux structure qui représente le temps.                     *
 * Renvoie un entier qui représente des millisecondes.                             *
 * Le but est de convertir un intervalle de temps de microseconde en milliseconde. */
int intervalle_ms(struct timeval debut, struct timeval fin){
    return ((fin.tv_sec - debut.tv_sec) * 1000 + (fin.tv_usec - debut.tv_usec ) / 1000) ;
}

/* nb_ms_vers_centiemes:                                             *
 * Prend en paramètre un entier qui représente des millisecondes.    *
 * Renvoie un entier qui représente des centiemes de secondes.       *
 * Le but est de convertir des milliseconde en centiemes de seconde. */
int nb_ms_vers_centiemes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 10) % 100;
}

/* nb_ms_vers_secondes:                                           *
 * Prend en paramètre un entier qui représente des millisecondes. *
 * Renvoie un entier qui représente des secondes.                 *
 * Le but est de convertir des milliseconde en seconde.           */
int nb_ms_vers_secondes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / 1000) % 60;
}

/* nb_ms_vers_minutes:                                            *
 * Prend en paramètre un entier qui représente des millisecondes. *
 * Renvoie un entier qui représente des minutes.                  *
 * Le but est de convertir des milliseconde en minutes.           */
int nb_ms_vers_minutes(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / (1000 * 60)) % 60;
}

/* nb_ms_vers_heures:                                             *
 * Prend en paramètre un entier qui représente des millisecondes. *
 * Renvoie un entier qui représente des heures.                   *
 * Le but est de convertir des milliseconde en heures.            */
int nb_ms_vers_heures(int nb_ms) {
    assert(nb_ms >= 0);
    return (nb_ms / (1000 * 3600));
}

/* afficher_duree *
 * Prend un paramètre une coordonnées d'entier "x" et "y" ainsi qu'un nombre de milliseconde "nb_ms". *
 * Ne renvoie rien.                                                                                   *
 * Le but est d'afficher le nombre "nb_ms" en coordonnée (x,y) dans un affichage tout aligné.         */
void afficher_duree(int y, int x, int nb_ms) {
    assert(nb_ms >= 0);
    mvprintw(y, x, "%2d : %2d : %2d : %2d\n", nb_ms_vers_heures(nb_ms), nb_ms_vers_minutes(nb_ms), nb_ms_vers_secondes(nb_ms), nb_ms_vers_centiemes(nb_ms));
}

/* affiche_comptour:                                               *
 * Ne prend rien en paramètre.                                     *
 * Ne renvoie rien.                                                *
 * Le but est d'afficher au milieu de la page un rectangle de "*". */
void affiche_comptour(void) {
    int i;

    for (i = 10; i <= COLS - 10; i ++) 
        mvprintw(5, i, "*");
    for (i = 10; i <= COLS - 10; i ++) 
        mvprintw(LINES - 15, i, "*");
    for (i = 5; i <= LINES - 15; i ++) 
        mvprintw(i, 10, "*");
    for (i = 5; i <= LINES - 15; i ++) 
        mvprintw(i, COLS - 10, "*");

    mvprintw(5, COLS * 0.4, " Chronometre ");

    mvprintw(LINES - 7, 0, "Espace : Lancer / Mettre en pause");
    mvprintw(LINES - 6, 0, "r      : Reinitialiser");
    mvprintw(LINES - 5, 0, "t      : Marquer tour");
    mvprintw(LINES - 4, 0, "F1/F2  : Incrementer / decrementer heure avertissement");
    mvprintw(LINES - 3, 0, "F3/F4  : Incrementer / decrementer minute avertissement");
    mvprintw(LINES - 2, 0, "F5/F6  : Incrementer / decrementer seconde avertissement");
    mvprintw(LINES - 1, 0, "q      : Quitter");
}

/* afficher_flash:                                                                         *
 * Ne prend rien en paramètre.                                                             *
 * Ne renvoie rien.                                                                        *
 * Le but est d'afficher les comptours en "*" grace a la fonction "affiche_comptour"       *
 * et en alternant la couleur de l'affichage de trois manière différente pendant un temps. */
void afficher_flash(void) {
    int compteur;

    for (compteur = 0; compteur <= 10; compteur++) {

        attron(COLOR_PAIR(1));
        affiche_comptour();
        attroff(COLOR_PAIR(1));

        refresh();
        usleep(100000);

        attron(COLOR_PAIR(2));
        affiche_comptour();
        attroff(COLOR_PAIR(2));

        refresh();
        usleep(100000);

        affiche_comptour();

        refresh();
        usleep(100000);
    }
}

/* afficher_interface:                                                         *
 * Prend en paramètre une structure "Chronometre".                             *
 * Ne renvoie rien.                                                            *
 * Le but est d'afficher au milieu de la page tout l'affichage du chronometre. */
void afficher_interface(Chronometre chrono) {
    int ordonne, abscisse;
    int i;

    assert(&chrono != NULL);

    ordonne = (LINES/3);
    abscisse = (COLS/3) - 2;

    mvprintw(ordonne + 1, abscisse, "Avertissement : ");
    afficher_duree(ordonne + 1, abscisse + 16, chrono.avertissement);
    afficher_duree(ordonne, abscisse + 16, chrono.temps);

    if (chrono.nbr_tour > 0){
        for(i = 0; i <= chrono.dernier_tour; i++) {
            mvprintw(ordonne - 1 - i, abscisse, "     Tour %3d : ", chrono.nbr_tour - i);
            afficher_duree(ordonne - 1 - i, abscisse + 16, chrono.tableau[ chrono.dernier_tour - i]);
        }
    }

    affiche_comptour();
}

/* ajouter_tour:                                                             *
 * Prend en paramètre l'adresse d'une structure "Chronometre".               *
 * Ne renvoie rien.                                                          *
 * Le but est d'ajouter ou de modifier le tableau "tableau" de la structure. */
void ajouter_tour(Chronometre *chrono) {
    assert(chrono != NULL);
    if (chrono->dernier_tour < 2) {
        chrono->dernier_tour += 1;
        chrono->tableau[ chrono->dernier_tour ]  = chrono->temps;
    }
    else {
        chrono->tableau[0] = chrono->tableau[1];
        chrono->tableau[1] = chrono->tableau[2];
        chrono->tableau[2] = chrono->temps;
    }
    chrono->nbr_tour += 1;
}

/* main:                                             *
 * Le but est d'afficher un Chronometre fonctionnel. */
int main(int argc, char const *argv[]){ 
    Chronometre chrono = initialser_chronometre();
    struct timeval temps_debut, temps_fin;
    int touche;
    int tmp_avert;

    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    while(1){
        touche = getch();
        if (touche == 32){
            if (chrono.etat == 0){
                gettimeofday(&temps_debut, NULL);
                chrono.etat = 1;
            }
            else
                chrono.etat = 0;
        }
        if (chrono.etat == 1){
            gettimeofday(&temps_fin, NULL);
            chrono.temps += intervalle_ms(temps_debut, temps_fin);
            temps_debut = temps_fin;
        }
        if (touche != ERR) {
            if (touche == 't')
                ajouter_tour(&chrono);
            if (touche == 'q')
                break;
            if (touche == 'r'){
                tmp_avert = chrono.avertissement;
                chrono = initialser_chronometre();
                chrono.avertissement = tmp_avert;
            }
            if (touche >= 265 && touche <= 270){
                tmp_avert = chrono.avertissement;
                if (touche == 265)
                    chrono.avertissement += 3600000;
                if (touche == 266)
                    chrono.avertissement -= 3600000;
                if (touche == 267)
                    chrono.avertissement += 60000;
                if (touche == 268)
                    chrono.avertissement -= 60000;
                if (touche == 269)
                    chrono.avertissement += 1000;
                if (touche == 270)
                    chrono.avertissement -= 1000;
                if (chrono.avertissement < 1000 || chrono.avertissement > 359999999)
                    chrono.avertissement = tmp_avert;
            }
        }
        if (chrono.avertissement <= chrono.temps && chrono.avertissement + 1000 >= chrono.temps)
            afficher_flash();
        if (LINES < 14 || COLS < 58)
            break;
        if (chrono.temps >= 359999999)
            chrono.temps = 0;
        afficher_interface(chrono);
        refresh();
        usleep(70000);
        clear();
    }

    getch();
    endwin();
    return 0;
}
