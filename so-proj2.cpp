#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;

const int liczba_watkow = 5;
bool miecz[3];
int status_rycerza[liczba_watkow];
int liczba_powtorzen[liczba_watkow];
int status_walki[liczba_watkow];
int status_jedzenia[liczba_watkow];
bool koniec_programu = false;

void wezMiecz(int numer_watku)
{
    miecz[numer_watku] = false;
    miecz[((2 * liczba_watkow + numer_watku - 1) % liczba_watkow)] = false;
}

void odlozMiecz(int numer_watku)
{
    miecz[numer_watku] = true;
    miecz[((2 * liczba_watkow + numer_watku - 1) % liczba_watkow)] = true;
}

void pokazStatus()
{
    mtx.lock();
    for (int i = 0; i < liczba_watkow; i++)
    {
        mvprintw(1, i * 5 + 3 + 15, "%d", status_rycerza[i]);

        if (i < 3)
        {
            string tekst = miecz[i] == 1 ? "wolny" : "zajety";
            mvprintw(2, i * 5 + 3 + 15, tekst);
        }

        mvprintw(i + 4, 0, "Rycerz %d walczyl %d razy", i, liczba_powtorzen[i]);
        mvprintw(i + 4, 35, "Walka %d/100", status_walki[i]);
        mvprintw(i + 4, 65, "Jedzenie %d/100", status_jedzenia[i]);
        mvprintw(1, 0, "(rycerze)");
        mvprintw(2, 0, "(miecze)");
        refresh();
    }
    mtx.unlock();
}

void wystartujWatek(int numer_watku)
{
    while (!koniec_programu)
    {
        if (getch() == 27)
        {
            koniec_programu = true;
            break;
        }
        mtx.lock();

        bool dostepny_miecz = miecz[numer_watku];
        bool liczba_powtorzen_rycerza_poprzedniego = liczba_powtorzen[numer_watku] <= liczba_powtorzen[(2 * liczba_watkow + numer_watku - 1) % liczba_watkow];
        bool liczba_powtorzen_rycerza_kolejnego = liczba_powtorzen[numer_watku] <= liczba_powtorzen[(2 * liczba_watkow + numer_watku + 1) % liczba_watkow];

        if (dostepny_miecz && liczba_powtorzen_rycerza_poprzedniego && liczba_powtorzen_rycerza_kolejnego)
        {
            wezMiecz(numer_watku);
            status_rycerza[numer_watku] = 1;
            liczba_powtorzen[numer_watku]++;
        }
        mtx.unlock();
        status_walki[numer_watku] = 0;
        for (int i = 1; i <= 10; i++)
        {
            if (status_rycerza[numer_watku] == 1)
            {
                usleep(rand() % 300000 + 300000);
                status_walki[numer_watku] += 10;
                pokazStatus();
            }
            else
            {
                break;
            }
        }
        status_walki[numer_watku] = 0;
        clear();
        pokazStatus();

        mtx.lock();
        if (status_rycerza[numer_watku] == 1)
        {
            odlozMiecz(numer_watku);
            status_rycerza[numer_watku] = 0;
        }
        mtx.unlock();

        status_jedzenia[numer_watku] = 0;
        for (int i = 1; i <= 10; i++)
        {
            usleep(rand() % 300000 + 200000);
            status_jedzenia[numer_watku] += 10;
            pokazStatus();
        }
        status_jedzenia[numer_watku] = 0;
        clear();
        pokazStatus();
    }
}
int main()
{
    srand(time(NULL));
    for (int i = 0; i < liczba_watkow; i++)
    {
        miecz[i] = true;
        status_rycerza[i] = 0;
    }
    int tab[liczba_watkow];
    thread t[liczba_watkow];
    initscr();
    nodelay(stdscr, TRUE);
    refresh();
    for (int i = 0; i < liczba_watkow; i++)
    {
        t[i] = thread(wystartujWatek, i);
    }
    for (int i = 0; i < liczba_watkow; i++)
    {
        t[i].join();
    }
    endwin();
    return 0;
}
