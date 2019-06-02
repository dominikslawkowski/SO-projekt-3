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

// Ilosci zasobow
const int liczba_rycerzy = 5;
const int liczba_mieczy = 3;
const int liczba_pancerzy = 3;
const int liczba_misek = 3;
const int liczba_kufli = 3;

// Zasoby
int miecz[liczba_mieczy];
int pancerz[liczba_pancerzy];
int miski[liczba_misek];
int kufle[liczba_kufli];

int liczba_powtorzen[liczba_rycerzy];

char *status_rycerza[liczba_rycerzy];
int status_walki[liczba_rycerzy];
int status_jedzenia[liczba_rycerzy];
int status_oczekiwania[liczba_rycerzy];

int zycie_rycerza[liczba_rycerzy];
int glod_rycerza[liczba_rycerzy];

const int poczatkowe_zycie_rycerza = 5;
const int poczatkowy_glod_rycerza = 5;

bool koniec_programu = false;

void wezMiecz(int miecz_id, int rycerz_id)
{
    miecz[miecz_id] = rycerz_id;
}

void odlozMiecz(int miecz_id)
{
    miecz[miecz_id] = -1;
}

void wezPancerz(int pancerza_id, int rycerz_id)
{
    pancerz[pancerza_id] = rycerz_id;
}

void odlozPancerz(int pancerza_id)
{
    pancerz[pancerza_id] = -1;
}

int zwrocPostepRycerza(string status, int id)
{
    if (status == U_MEDYKA)
    {
        return 0;
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

int wylosujLiczbe()
{
    return rand() % 1000000 + 300000;
}

void pokazStatusRycerza(int i)
{
    int przesuniecie = 14;
    int odstep = 25 * i;
    int przes = zwrocPrzesuniecie(status_rycerza[i]);

    mvprintw(2 + przes, odstep, "Rycerz %d", i);
    mvprintw(3 + przes, odstep, "--------------");

    // Wyswietlenie zycia rycerza
    mvprintw(4 + przes, odstep, "zycie: [");
    for (int j = 0; j < poczatkowe_zycie_rycerza; j++)
    {
        mvprintw(4 + przes, odstep + 8 + j, zycie_rycerza[i] > j ? "=" : " ");
    }
    mvprintw(4 + przes, odstep + 8 + 5, "]");

    // Wyswietlenie glody rycerza
    mvprintw(5 + przes, odstep, "glod:  [");
    for (int j = 0; j < poczatkowy_glod_rycerza; j++)
    {
        mvprintw(5 + przes, odstep + 8 + j, glod_rycerza[i] > j ? "=" : " ");
    }
    mvprintw(5 + przes, odstep + 8 + 5, "]");

    mvprintw(6 + przes, odstep, "status: %s", status_rycerza[i]);
    mvprintw(7 + przes, odstep, "postep: %d/100", zwrocPostepRycerza(status_rycerza[i], i));

    mvprintw(8 + przes, odstep, "uzywa: ");
    int index_pancerza = distance(pancerz, find(pancerz, pancerz + liczba_pancerzy, i));
    if (index_pancerza != liczba_pancerzy)
    {
        mvprintw(9 + przes, odstep, "- pancerz %d", index_pancerza);
    }

    int index_miecza = distance(miecz, find(miecz, miecz + liczba_mieczy, i));
    if (index_miecza != liczba_mieczy)
    {
        mvprintw(10 + przes, odstep, "- miecz %d", index_miecza);
    }

    mvprintw(11 + przes, odstep, "status: %s", status_rycerza[i]);
}

void pokazStatus()
{
    mtx.lock();
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        int odstep = 25 * i;

        refresh();
        mvprintw(0, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - MEDYK - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(14, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - KUCHNIA - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(28, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ZAMEK - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        mvprintw(42, 0, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - POLE BITWY - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

        pokazStatusRycerza(i);

        // if (i < liczba_pancerzy)
        // {
        //     mvprintw(2, 150 + odstep, "%d", i);
        // }

        // mvprintw(3, 150, "pancerze");
        // mvprintw(4, 150, "rycerz %d", pancerz[0]);
        // mvprintw(4, 150 + 25, "rycerz %d", pancerz[1]);
        // mvprintw(4, 150 + 50, "rycerz %d", pancerz[2]);

        // mvprintw(6, 150, "miecze");
        // mvprintw(7, 150, "rycerz %d", miecz[0]);
        // mvprintw(7, 150 + 25, "rycerz %d", miecz[1]);
        // mvprintw(7, 150 + 50, "rycerz %d", miecz[2]);

        //-----------------

        // mvprintw(1, 0, "(rycerze)");
        // mvprintw(2, 0, "(miecze)");

        // const char *tekst = status_rycerza[i] == 0 ? W_BAZIE : "walczy";
        // mvprintw(1, i * 10 + 3 + 15, "%s", tekst);

        // if (i < liczba_mieczy)
        // {
        //     const char *tekst = miecz[i] == 1 ? "wolny" : "zajety";
        //     mvprintw(2, i * 10 + 3 + 15, "%s", tekst);
        // }

        // mvprintw(i + 4, 0, "Rycerz %d walczyl %d razy", i, liczba_powtorzen[i]);
        // mvprintw(i + 4, 35, "Walka %d/100", status_walki[i]);
        // mvprintw(i + 4, 65, "Jedzenie %d/100", status_jedzenia[i]);
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
        int index_miecza = distance(miecz, find(miecz, miecz + liczba_mieczy, -1));
        int index_pancerza = distance(pancerz, find(pancerz, pancerz + liczba_pancerzy, -1));

        bool liczba_powtorzen_rycerza_poprzedniego = liczba_powtorzen[id] <= liczba_powtorzen[(2 * liczba_rycerzy + id - 1) % liczba_rycerzy];
        bool liczba_powtorzen_rycerza_kolejnego = liczba_powtorzen[id] <= liczba_powtorzen[(2 * liczba_rycerzy + id + 1) % liczba_rycerzy];

        // Rycerz udaje sie na pole bitwy
        if (zycie_rycerza[id] > 3 && index_miecza < liczba_mieczy && index_pancerza < liczba_pancerzy && liczba_powtorzen_rycerza_poprzedniego && liczba_powtorzen_rycerza_kolejnego)
        {
            wezMiecz(index_miecza, id);
            wezPancerz(index_pancerza, id);
            status_rycerza[id] = NA_POLU_BITWY;
            liczba_powtorzen[id]++;
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
        if (glod_rycerza[id] < poczatkowy_glod_rycerza)
        {
            status_rycerza[id] = W_KUCHNI;
            status_jedzenia[id] = 0;
            for (int i = 1; i <= 10; i++)
            {
                usleep(wylosujLiczbe());
                status_jedzenia[id] += 10;
                if (glod_rycerza[id] < poczatkowy_glod_rycerza && rand() % 10 > 7)
                {
                    glod_rycerza[id] += 1;
                }
                clear();
                pokazStatus();
            }
            status_jedzenia[id] = 0;
            clear();
            pokazStatus();
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
int main()
{
    srand(time(NULL));
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        status_rycerza[i] = W_BAZIE;
    }

    for (int i = 0; i < liczba_mieczy; i++)
    {
        miecz[i] = -1;
    }

    for (int i = 0; i < liczba_pancerzy; i++)
    {
        pancerz[i] = -1;
    }

    int tab[liczba_rycerzy];
    thread t[liczba_rycerzy];
    initscr();
    nodelay(stdscr, TRUE);
    refresh();
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        t[i] = thread(wystartujWatekRycerza, i);
    }
    for (int i = 0; i < liczba_rycerzy; i++)
    {
        t[i].join();
    }
    endwin();
    return 0;
}
