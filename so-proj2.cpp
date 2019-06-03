#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <algorithm>

using namespace std;
mutex mtx;

// Statusy rycerzy
char *W_BAZIE = "w bazie";
char *NA_POLU_BITWY = "na polu bitwy";
char *U_MEDYKA = "u medyka";
char *W_KUCHNI = "w kuchni";

// Statusy rzemieslnika
char *OBIJA_SIE = "obija sie";
char *NAPRAWIA_MIECZ = "naprawia miecz";
char *NAPRAWIA_PANCERZ = "naprawia pancerz";

// Status kucharza
char *SPI = "spi";
char *GOTUJE = "gotuje";

// Ilosci zasobow
const int liczba_rycerzy = 5;
const int liczba_mieczy = 3;
const int liczba_pancerzy = 3;
const int liczba_misek = 3;
const int liczba_kufli = 3;

// Zasoby
int miecz[liczba_mieczy][2];
int pancerz[liczba_pancerzy][2];
int miska[liczba_misek][2];
int kufel[liczba_kufli][2];

int liczba_powtorzen_walki[liczba_rycerzy];
int liczba_powtorzen_jedzenia[liczba_rycerzy];

char *status_rycerza[liczba_rycerzy];
int status_walki[liczba_rycerzy];
int status_jedzenia[liczba_rycerzy];
int status_leczenia[liczba_rycerzy];
int status_oczekiwania[liczba_rycerzy];

int zycie_rycerza[liczba_rycerzy];
int glod_rycerza[liczba_rycerzy];

const int poczatkowe_zycie_rycerza = 8;
const int poczatkowy_glod_rycerza = 8;

char *status_rzemieslnika;
int status_naprawiania;
int status_obijania_sie;

char *status_kucharza;
int status_gotowania;
int status_spania;

bool koniec_programu = false;

int wylosujLiczbe()
{
    return rand() % 1000000 + 300000;
}

void wezMiecz(int miecz_id, int rycerz_id)
{
    miecz[miecz_id][0] = rycerz_id;
}

void odlozMiecz(int miecz_id)
{
    miecz[miecz_id][0] = -1;
    miecz[miecz_id][1] = 0;
}

void wezPancerz(int pancerza_id, int rycerz_id)
{
    pancerz[pancerza_id][0] = rycerz_id;
}

void odlozPancerz(int pancerza_id)
{
    pancerz[pancerza_id][0] = -1;
    pancerz[pancerza_id][1] = 0;
}

void wezKufel(int kufel_id, int rycerz_id)
{
    kufel[kufel_id][0] = rycerz_id;
}

void odlozKufel(int kufel_id)
{
    kufel[kufel_id][0] = -1;
    kufel[kufel_id][1] = 0;
}

void wezMiske(int miska_id, int rycerz_id)
{
    miska[miska_id][0] = rycerz_id;
}

void odlozMiske(int miska_id)
{
    miska[miska_id][0] = -1;
    miska[miska_id][1] = 0;
}

int znajdzGotowyMiecz(int tab[liczba_mieczy][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id && tab[i][1] == 1)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzGotowyPancerz(int tab[liczba_pancerzy][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id && tab[i][1] == 1)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzGotowaMiske(int tab[liczba_misek][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id && tab[i][1] == 1)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzGotowyKufel(int tab[liczba_kufli][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id && tab[i][1] == 1)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzMieczRzemieslnika(int tab[liczba_mieczy][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzPancerzRzemieslnika(int tab[liczba_pancerzy][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzMiskeKucharza(int tab[liczba_misek][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzKufelKucharza(int tab[liczba_kufli][2], int ilosc, int id)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == id)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int znajdzZepsutyPrzedmiot(int tab[liczba_mieczy][2], int ilosc)
{
    int index_miecza = ilosc;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == -1 && tab[i][1] == 0)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int zwrocPostepRycerza(string status, int id)
{
    if (status == U_MEDYKA)
    {
        return status_leczenia[id];
    }
    else if (status == W_KUCHNI)
    {
        return status_jedzenia[id];
    }
    else if (status == W_BAZIE)
    {
        return status_oczekiwania[id];
    }
    else if (status == NA_POLU_BITWY)
    {
        return status_walki[id];
    }
}

int zwrocPostepRzemieslnika()
{
    if (status_rzemieslnika == OBIJA_SIE)
    {
        return status_obijania_sie;
    }
    else if (status_rzemieslnika == NAPRAWIA_MIECZ || status_rzemieslnika == NAPRAWIA_PANCERZ)
    {
        return status_naprawiania;
    }
}

int zwrocPostepKucharza()
{
    if (status_kucharza == SPI)
    {
        return status_spania;
    }
    else if (status_kucharza == GOTUJE)
    {
        return status_gotowania;
    }
}

int zwrocPrzesuniecie(string status)
{
    if (status == U_MEDYKA)
    {
        return 0;
    }
    else if (status == W_KUCHNI)
    {
        return 14;
    }
    else if (status == W_BAZIE)
    {
        return 28;
    }
    else if (status == NA_POLU_BITWY)
    {
        return 42;
    }
}

void pokazStatusRycerza(int i)
{
    int przesuniecie = 14;
    int odstep = (25 * i) + 3;
    int przes = zwrocPrzesuniecie(status_rycerza[i]);
    mvprintw(2 + przes, odstep, "Rycerz %d", i);
    mvprintw(3 + przes, odstep, "----------------");

    // Wyswietlenie zycia rycerza
    mvprintw(4 + przes, odstep, "zycie: [");

    int color = zycie_rycerza[i] > 2 ? 2 : 1;
    attron(COLOR_PAIR(color));
    for (int j = 0; j < poczatkowe_zycie_rycerza; j++)
    {

        mvprintw(4 + przes, odstep + 8 + j, zycie_rycerza[i] > j ? "=" : " ");
    }
    attroff(COLOR_PAIR(color));
    mvprintw(4 + przes, odstep + 8 + poczatkowe_zycie_rycerza, "]");

    // Wyswietlenie glody rycerza
    mvprintw(5 + przes, odstep, "glod:  [");
    attron(COLOR_PAIR(3));
    for (int j = 0; j < poczatkowy_glod_rycerza; j++)
    {
        mvprintw(5 + przes, odstep + 8 + j, glod_rycerza[i] > j ? "=" : " ");
    }
    attroff(COLOR_PAIR(3));
    mvprintw(5 + przes, odstep + 8 + poczatkowy_glod_rycerza, "]");

    mvprintw(6 + przes, odstep, "status: %s", status_rycerza[i]);
    mvprintw(7 + przes, odstep, "postep: %d/100", zwrocPostepRycerza(status_rycerza[i], i));

    mvprintw(8 + przes, odstep, "uzywa: ");
    // int index_pancerza = distance(pancerz, find(pancerz, pancerz + liczba_pancerzy, i));
    int index_pancerza = znajdzGotowyPancerz(pancerz, sizeof(pancerz) / sizeof(pancerz[0]), i);
    if (index_pancerza != liczba_pancerzy)
    {
        mvprintw(9 + przes, odstep, "- pancerz %d", index_pancerza);
    }

    // int index_miecza = distance(miecz, find(miecz, miecz + liczba_mieczy, i));
    int index_miecza = znajdzGotowyMiecz(miecz, sizeof(miecz) / sizeof(miecz[0]), i);
    if (index_miecza != liczba_mieczy)
    {
        mvprintw(10 + przes, odstep, "- miecz %d", index_miecza);
    }

    // int index_kufla = distance(kufel, find(kufel, kufel + liczba_kufli, i));
    int index_kufla = znajdzGotowaMiske(kufel, sizeof(kufel) / sizeof(kufel[0]), i);
    if (index_kufla != liczba_kufli)
    {
        mvprintw(9 + przes, odstep, "- kufel %d", index_kufla);
    }

    // int index_miski = distance(miska, find(miska, miska + liczba_misek, i));
    int index_miski = znajdzGotowaMiske(miska, sizeof(miska) / sizeof(miska[0]), i);
    if (index_miski != liczba_misek)
    {
        mvprintw(10 + przes, odstep, "- miska %d", index_miski);
    }
}

void pokazStatusRzemieslnika()
{
    mvprintw(0, 128, "- - - - - - - - - - - - - RZEMIESLNIK - - - - - - - - - - - - - -");
    mvprintw(2, 130, "Rzemieslnik");
    mvprintw(3, 130, "----------------");
    mvprintw(4, 130, "status: %s", status_rzemieslnika);
    mvprintw(5, 130, "postep: %d/100", zwrocPostepRzemieslnika());
    mvprintw(6, 130, "uzywa: ");
    int index_miecza = znajdzMieczRzemieslnika(miecz, sizeof(miecz) / sizeof(miecz[0]), liczba_rycerzy + 1);
    if (index_miecza != liczba_mieczy)
    {
        mvprintw(7, 130, "- miecz %d", index_miecza);
    }
    int index_pancerza = znajdzPancerzRzemieslnika(pancerz, sizeof(pancerz) / sizeof(pancerz[0]), liczba_rycerzy + 1);
    if (index_pancerza != liczba_pancerzy)
    {
        mvprintw(7, 130, "- pancerz %d", index_pancerza);
    }
}

void pokazStatusKucharza()
{
    mvprintw(14, 128, "- - - - - - - - - - - - - KUCHARZ - - - - - - - - - - - - - -");
    mvprintw(15, 130, "Kucharz");
    mvprintw(16, 130, "----------------");
    mvprintw(17, 130, "status: %s", status_kucharza);
    mvprintw(18, 130, "postep: %d/100", zwrocPostepKucharza());
    mvprintw(19, 130, "uzywa: ");
    int index_kufla = znajdzKufelKucharza(kufel, sizeof(kufel) / sizeof(kufel[0]), liczba_rycerzy + 2);
    if (index_kufla != liczba_kufli)
    {
        mvprintw(20, 130, "- kufel %d", index_kufla);
    }
    int index_miski = znajdzMiskeKucharza(miska, sizeof(miska) / sizeof(miska[0]), liczba_rycerzy + 2);
    if (index_miski != liczba_misek)
    {
        mvprintw(21, 130, "- miska %d", index_miski);
    }
    mvprintw(28, 128, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
}

void pokazStatus()
{
    mtx.lock();
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        int odstep = 25 * i;

        mvprintw(0, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - MEDYK - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(14, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - KUCHNIA - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(28, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ZAMEK - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        attron(COLOR_PAIR(1));
        mvprintw(42, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - POLE BITWY - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(56, 0, " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        attroff(COLOR_PAIR(1));
        for (int i = 0; i < 56; i++)
        {
            mvprintw(i, 127, "|");
            mvprintw(i, 0, "|");
        }

        pokazStatusRycerza(i);

        pokazStatusRzemieslnika();

        pokazStatusKucharza();

        mvprintw(41, 140, " przedmiot|");
        mvprintw(42, 140, "----------|");
        for (int i = 0; i < liczba_mieczy; i++)
        {
            mvprintw(41, 151 + i * 9, "   %d   ", i);
            mvprintw(42, 151 + i * 9, "---------");
        }
        mvprintw(43, 140, "   miecz  |");
        for (int i = 0; i < liczba_mieczy; i++)
        {
            mvprintw(43, 151 + i * 9, "  %s  ", miecz[i][1] == 1 ? "gotowy" : "zepsuty");
        }
        mvprintw(44, 140, "          |");
        mvprintw(45, 140, "  pancerz |");
        for (int i = 0; i < liczba_pancerzy; i++)
        {
            mvprintw(45, 151 + i * 9, "  %s  ", pancerz[i][1] == 1 ? "gotowy" : "zepsuty");
        }
        mvprintw(46, 140, "          |");
        mvprintw(47, 140, "   kufel  |");
        for (int i = 0; i < liczba_kufli; i++)
        {
            mvprintw(47, 151 + i * 9, "  %s  ", kufel[i][1] == 1 ? "pelny" : "pusty");
        }
        mvprintw(48, 140, "          |");
        mvprintw(49, 140, "   miska  |");
        for (int i = 0; i < liczba_misek; i++)
        {
            mvprintw(49, 151 + i * 9, "  %s  ", miska[i][1] == 1 ? "pelna" : "pusta");
        }
        refresh();
    }
    mtx.unlock();
}

void wystartujWatekRycerza(int id)
{
    zycie_rycerza[id] = poczatkowe_zycie_rycerza;
    glod_rycerza[id] = poczatkowy_glod_rycerza;

    while (!koniec_programu)
    {
        if (getch() == 27)
        {
            koniec_programu = true;
            break;
        }

        mtx.lock();
        // int index_miecza = distance(miecz, find(miecz, miecz + liczba_mieczy, -1));
        // int index_pancerza = distance(pancerz, find(pancerz, pancerz + liczba_pancerzy, -1));
        int index_miecza = znajdzGotowyMiecz(miecz, sizeof(miecz) / sizeof(miecz[0]), -1);
        int index_pancerza = znajdzGotowyPancerz(pancerz, sizeof(pancerz) / sizeof(pancerz[0]), -1);

        bool liczba_powtorzen_rycerza_poprzedniego = liczba_powtorzen_walki[id] <= liczba_powtorzen_walki[(2 * liczba_rycerzy + id - 1) % liczba_rycerzy];
        bool liczba_powtorzen_rycerza_kolejnego = liczba_powtorzen_walki[id] <= liczba_powtorzen_walki[(2 * liczba_rycerzy + id + 1) % liczba_rycerzy];

        // Rycerz udaje sie na pole bitwy
        if (zycie_rycerza[id] > 3 && glod_rycerza[id] > 3 && index_miecza < liczba_mieczy && index_pancerza < liczba_pancerzy && liczba_powtorzen_rycerza_poprzedniego && liczba_powtorzen_rycerza_kolejnego)
        {
            wezMiecz(index_miecza, id);
            wezPancerz(index_pancerza, id);
            status_rycerza[id] = NA_POLU_BITWY;
            liczba_powtorzen_walki[id]++;
        }
        mtx.unlock();

        status_walki[id] = 0;
        for (int i = 1; i <= 10; i++)
        {
            if (status_rycerza[id] == NA_POLU_BITWY)
            {
                usleep(wylosujLiczbe());
                status_walki[id] += 10;

                // Utrata zycia przez rycerza (randomowo)
                if (zycie_rycerza[id] > 0 && rand() % 10 > 5)
                {
                    zycie_rycerza[id] -= 1;
                }

                // Utrata glodu przez rycerza (stale)
                if (i % 3 == 0)
                {
                    glod_rycerza[id] -= 1;
                }

                if (zycie_rycerza[id] <= 1 || glod_rycerza[id] <= 1)
                {
                    break;
                    status_walki[id] = 0;
                }

                clear();
                pokazStatus();
            }
            else
            {
                break;
            }
        }
        status_walki[id] = 0;
        clear();
        pokazStatus();

        if (status_rycerza[id] == NA_POLU_BITWY)
        {
            mtx.lock();
            odlozMiecz(index_miecza);
            odlozPancerz(index_pancerza);
            status_rycerza[id] = W_BAZIE;
            mtx.unlock();
        }

        // Rycerz posila sie
        mtx.lock();
        // int index_kufla = distance(kufel, find(kufel, kufel + liczba_kufli, -1));
        int index_kufla = znajdzGotowyKufel(kufel, sizeof(kufel) / sizeof(kufel[0]), -1);
        // int index_miski = distance(miska, find(miska, miska + liczba_misek, -1));
        int index_miski = znajdzGotowaMiske(miska, sizeof(miska) / sizeof(miska[0]), -1);

        bool liczba_powtorzen_jedzenia_rycerza_poprzedniego = liczba_powtorzen_jedzenia[id] <= liczba_powtorzen_jedzenia[(2 * liczba_rycerzy + id - 1) % liczba_rycerzy];
        bool liczba_powtorzen_jedzenia_rycerza_kolejnego = liczba_powtorzen_jedzenia[id] <= liczba_powtorzen_jedzenia[(2 * liczba_rycerzy + id + 1) % liczba_rycerzy];

        // Rycerz udaje sie na pole bitwy
        if (glod_rycerza[id] < poczatkowy_glod_rycerza && index_kufla < liczba_kufli && index_miski < liczba_misek && liczba_powtorzen_jedzenia_rycerza_poprzedniego && liczba_powtorzen_jedzenia_rycerza_kolejnego)
        {
            wezKufel(index_kufla, id);
            wezMiske(index_miski, id);
            status_rycerza[id] = W_KUCHNI;
            liczba_powtorzen_jedzenia[id]++;
        }
        mtx.unlock();

        if (status_rycerza[id] == W_KUCHNI)
        {
            status_jedzenia[id] = 0;
            for (int i = 1; i <= 10; i++)
            {
                usleep(wylosujLiczbe());
                status_jedzenia[id] += 10;
                if (glod_rycerza[id] < poczatkowy_glod_rycerza && i % 2 == 0)
                {
                    glod_rycerza[id] += 1;
                }
                if (glod_rycerza[id] == poczatkowy_glod_rycerza)
                {
                    break;
                    status_jedzenia[id] = 0;
                }
                clear();
                pokazStatus();
            }
            status_jedzenia[id] = 0;
            clear();
            pokazStatus();

            mtx.lock();
            odlozKufel(index_kufla);
            odlozMiske(index_miski);
            mtx.unlock();
            status_rycerza[id] = W_BAZIE;
        }
        else if (zycie_rycerza[id] < poczatkowe_zycie_rycerza)
        {
            // Lub rycerz udaje sie do medyka
            status_rycerza[id] = U_MEDYKA;
            status_leczenia[id] = 0;
            for (int i = 1; i <= 10; i++)
            {
                usleep(wylosujLiczbe());
                status_leczenia[id] += 10;
                if (zycie_rycerza[id] < poczatkowe_zycie_rycerza && i % 2 == 0)
                {
                    zycie_rycerza[id] += 1;
                }

                if (zycie_rycerza[id] == poczatkowe_zycie_rycerza)
                {
                    break;
                    status_leczenia[id] = 0;
                }
                clear();
                pokazStatus();
            }
            status_leczenia[id] = 0;
            clear();
            pokazStatus();
            status_rycerza[id] = W_BAZIE;
        }
        else
        {
            // W innym przypadku rycerz oczekuje w bazie
            status_rycerza[id] = W_BAZIE;
            status_oczekiwania[id] = 0;
            for (int i = 1; i <= 10; i++)
            {
                usleep(wylosujLiczbe() / 2);
                status_oczekiwania[id] += 10;
                clear();
                pokazStatus();
            }
            status_oczekiwania[id] = 0;
            clear();
            pokazStatus();
        }
    }
}

void wystartujWatekRzemieslnika(int id)
{
    while (!koniec_programu)
    {
        status_rzemieslnika = OBIJA_SIE;
        mtx.lock();
        int index_miecza = znajdzZepsutyPrzedmiot(miecz, sizeof(miecz) / sizeof(miecz[0]));
        int index_pancerza = znajdzZepsutyPrzedmiot(pancerz, sizeof(pancerz) / sizeof(pancerz[0]));

        if (index_miecza != liczba_mieczy)
        {
            wezMiecz(index_miecza, id);
            status_rzemieslnika = NAPRAWIA_MIECZ;
        }
        else if (index_pancerza != liczba_pancerzy)
        {
            wezPancerz(index_pancerza, id);
            status_rzemieslnika = NAPRAWIA_PANCERZ;
        }

        mtx.unlock();

        status_naprawiania = 0;
        for (int i = 1; i <= 10; i++)
        {
            if (status_rzemieslnika == NAPRAWIA_MIECZ)
            {
                usleep(wylosujLiczbe());
                status_naprawiania += 10;

                clear();
                pokazStatus();
            }
            else if (status_rzemieslnika == NAPRAWIA_PANCERZ)
            {
                usleep(wylosujLiczbe());
                status_naprawiania += 10;

                clear();
                pokazStatus();
            }
        }

        status_naprawiania = 0;

        clear();
        pokazStatus();

        if (status_rzemieslnika == NAPRAWIA_MIECZ)
        {
            mtx.lock();
            odlozMiecz(index_miecza);
            miecz[index_miecza][1] = 1;
            status_rzemieslnika = OBIJA_SIE;
            mtx.unlock();
        }
        else if (status_rzemieslnika == NAPRAWIA_PANCERZ)
        {
            mtx.lock();
            odlozPancerz(index_pancerza);
            pancerz[index_pancerza][1] = 1;
            status_rzemieslnika = OBIJA_SIE;
            mtx.unlock();
        }

        status_rzemieslnika = OBIJA_SIE;
        status_obijania_sie = 0;
        for (int i = 1; i <= 10; i++)
        {
            usleep(wylosujLiczbe());
            status_obijania_sie += 10;

            clear();
            pokazStatus();
        }
        status_obijania_sie = 0;
    }
}

void wystartujWatekKucharza(int id)
{
    while (!koniec_programu)
    {
        status_kucharza = SPI;
        mtx.lock();
        int index_kufla = znajdzZepsutyPrzedmiot(kufel, sizeof(kufel) / sizeof(kufel[0]));
        int index_miski = znajdzZepsutyPrzedmiot(miska, sizeof(miska) / sizeof(miska[0]));

        if (index_kufla != liczba_kufli && index_miski != liczba_misek)
        {
            wezKufel(index_kufla, id);
            wezMiske(index_miski, id);
            status_kucharza = GOTUJE;
        }

        mtx.unlock();

        status_gotowania = 0;
        for (int i = 1; i <= 10; i++)
        {
            if (status_kucharza == GOTUJE)
            {
                usleep(wylosujLiczbe());
                status_gotowania += 10;

                clear();
                pokazStatus();
            }
        }

        status_gotowania = 0;

        clear();
        pokazStatus();

        if (status_kucharza == GOTUJE)
        {
            mtx.lock();
            odlozMiske(index_miski);
            odlozKufel(index_kufla);
            miska[index_miski][1] = 1;
            kufel[index_kufla][1] = 1;
            status_kucharza = SPI;
            mtx.unlock();
        }

        status_kucharza = SPI;
        status_spania = 0;
        for (int i = 1; i <= 10; i++)
        {
            usleep(wylosujLiczbe());
            status_spania += 10;

            clear();
            pokazStatus();
        }
        status_spania = 0;
    }
}

int main()
{
    srand(time(NULL));
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        status_rycerza[i] = W_BAZIE;
    }

    for (int i = 0; i < liczba_mieczy; i++)
    {
        miecz[i][0] = -1;
        miecz[i][1] = 1;
    }

    for (int i = 0; i < liczba_pancerzy; i++)
    {
        pancerz[i][0] = -1;
        pancerz[i][1] = 1;
    }

    for (int i = 0; i < liczba_misek; i++)
    {
        miska[i][0] = -1;
        miska[i][1] = 1;
    }

    for (int i = 0; i < liczba_kufli; i++)
    {
        kufel[i][0] = -1;
        kufel[i][1] = 1;
    }

    int tab[liczba_rycerzy];
    thread watek_rycerza[liczba_rycerzy];
    thread watek_rzemieslnika;
    thread watek_kucharza;
    initscr();
    use_default_colors();
    start_color();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_BLUE, -1);
    nodelay(stdscr, TRUE);
    refresh();

    for (int i = 0; i < liczba_rycerzy; i++)
    {
        watek_rycerza[i] = thread(wystartujWatekRycerza, i);
    }
    watek_rzemieslnika = thread(wystartujWatekRzemieslnika, liczba_rycerzy + 1);

    watek_kucharza = thread(wystartujWatekKucharza, liczba_rycerzy + 2);

    for (int i = 0; i < liczba_rycerzy; i++)
    {
        watek_rycerza[i].join();
    }

    watek_rzemieslnika.join();
    watek_kucharza.join();

    endwin();
    return 0;
}
