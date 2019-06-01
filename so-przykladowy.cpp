#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mtx;
condition_variable cv_field, cv_factoryGrain, cv_cutCereal;

const int numFarmer = 4;
const int numDriver = 2;
const int numProducer = 12;
const int numMiller = 4;
const int numDistiller = 4;
const int numForager = 4;
const int numCustomer = 4;

int fieldStatus = 100;
int cutCereals = 10;
int factoryGrainStatus = 0;

int grainStatus = 0;
int strawStatus = 0;

int counterFarmer[numFarmer];

int flourStatus;
int alcoholStatus;
int forageStatus;

int flourStatusCustomer[numCustomer];
int alcoholStatusCustomer[numCustomer];
int forageStatusCustomer[numCustomer];
int cashCustomer[numCustomer];

int cash;
int growthRate = 1;

void showStatus()
{
    mtx.lock();

    for (int i = 0; i < 150; i++)
    {
        mvprintw(12, 25 + i, "-");
    }
    for (int i = 0; i < 45; i++)
    {
        mvprintw(13 + i, 25, "|");
    }
    for (int i = 0; i < 45; i++)
    {
        mvprintw(12 + 1, 90, "|");
    }

    mvprintw(1, 173, "Ilosc zboza w fabryce : %d", factoryGrainStatus);
    mvprintw(1, 4, "Stan pola : %d", fieldStatus);
    mvprintw(1, 27, "Ilosc scietego zboza : %d", cutCereals);

    mvprintw(19, 173, "Ziarno: %d", grainStatus);
    mvprintw(29, 173, "Sloma: %d", strawStatus);

    mvprintw(13, 130, "Produkcja ");
    mvprintw(15, 130, "Produkcja maki ");
    mvprintw(25, 135, "Produkcja spirytusu ");
    mvprintw(35, 135, "Produkcja paszy");

    mvprintw(13, 95, "Gotowe produkty");
    mvprintw(15, 95, "Maka : %d", flourStatus);
    mvprintw(25, 95, "Spirytus: %d", alcoholStatus);
    mvprintw(35, 95, "Pasza: %d", forageStatus);

    mvprintw(13, 55, "Sprzedane produkty");
    mvprintw(15, 47, "Maka		Spirytus			Pasza");

    mvprintw(45, 35, "Pieniadze : %d", cash);

    mvprintw(46, 35, "Wsploczynnik wzrostu : %d", growthRate);

    refresh();
    mtx.unlock();
}

void showStatusFactoryGrain()
{
    mtx.lock();
    int counter = factoryGrainStatus;
    mvprintw(1, 173, "Ilosc zboza w fabryce : ");
    mvprintw(1, 173, "Ilosc zboza w fabryce :  %d", factoryGrainStatus);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter > 0)
            {
                mvprintw(i + 2, j + 182, "*");
                counter--;
            }
            else
            {
                mvprintw(i + 2, j + 182, " ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusField()
{
    mtx.lock();
    int counter = fieldStatus;
    counter *= 2;
    mvprintw(1, 4, "Stan pola: ");
    mvprintw(1, 4, "Stan pola:  %d", fieldStatus);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (counter > 0)
            {
                attron(COLOR_PAIR(3));
                mvprintw(i + 2, j + 2, " ");
                attroff(COLOR_PAIR(3));
                counter--;
            }
            else
            {
                attron(COLOR_PAIR(2));
                mvprintw(i + 2, j + 2, " ");
                attroff(COLOR_PAIR(2));
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusFarmer(int tID, bool work)
{
    mtx.lock();
    if (work)
    {
        attron(COLOR_PAIR(2));
        mvprintw(13 + tID, 2, "Rolnik %d : %d", tID, counterFarmer[tID]);
        attroff(COLOR_PAIR(2));
    }
    else
    {
        mvprintw(13 + tID, 2, "Rolnik %d : %d", tID, counterFarmer[tID]);
    }
    refresh();
    mtx.unlock();
}

void showStatusCutCereals()
{
    mtx.lock();
    int counter = cutCereals;
    mvprintw(1, 27, "Ilosc zboza: ");
    mvprintw(1, 27, "Ilosc zboza:  %d", cutCereals);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter > 0)
            {
                mvprintw(i + 2, j + 29, "*");
                counter--;
            }
            else
            {
                mvprintw(i + 2, j + 29, " ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusFactory()
{
    mtx.lock();
    int counter1 = grainStatus;
    int counter2 = strawStatus;
    mvprintw(19, 173, "Ziarno: ");
    mvprintw(19, 173, "Ziarno:  %d", grainStatus);

    mvprintw(29, 173, "Sloma: ");
    mvprintw(29, 173, "Sloma:  %d", strawStatus);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter1 > 0)
            {
                attron(COLOR_PAIR(2));
                mvprintw(i + 20, j + 175, "z");
                attroff(COLOR_PAIR(2));
                counter1--;
            }
            else
            {
                attron(COLOR_PAIR(2));
                mvprintw(i + 20, j + 175, " ");
                attroff(COLOR_PAIR(2));
            }
            if (counter2 > 0)
            {
                attron(COLOR_PAIR(2));
                mvprintw(i + 30, j + 175, "s");
                attroff(COLOR_PAIR(2));
                counter2--;
            }
            else
            {
                attron(COLOR_PAIR(2));
                mvprintw(i + 30, j + 175, " ");
                attroff(COLOR_PAIR(2));
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusDriver(int tID, int position)
{
    mtx.lock();
    if (position == 0)
    {
        mvprintw(2 + tID, 45, "#");
        refresh();
    }
    else
    {
        for (int i = 45; i < 95; i++)
        {
            for (int j = 2; j <= 6; j++)
            {
                mvprintw(j + tID * 5, i - 1, " ");
            }
            mvprintw(2 + tID * 5, i, "#");
            refresh();
            usleep(20000);
        }
    }
    mtx.unlock();
}

void showStatusDriver(int tID)
{
    mtx.lock();
    for (int i = 95; i > 45; i--)
    {
        for (int j = 2; j <= 6; j++)
        {
            mvprintw(j + tID * 5, i - 1, " ");
        }
        mvprintw(2 + tID * 5, i, "#");
        refresh();
        usleep(20000);
    }
    for (int j = 2; j <= 6; j++)
    {
        mvprintw(j + tID * 5, 45, " ");
    }
    mtx.unlock();
}

void showStatusMiller(int tID, int position)
{
    mtx.lock();
    mvprintw(17 + tID, 125, "Mlynarz %d: ", tID);
    for (int i = 0; i < 10; i++)
    {
        mvprintw(17 + tID, 136 + i, " ");
    }
    for (int i = 0; i < position; i++)
    {
        attron(COLOR_PAIR(1));
        mvprintw(17 + tID, 136 + i, " ");
        attroff(COLOR_PAIR(1));
    }
    char percent = 37;
    mvprintw(17 + tID, 146, "      ");
    mvprintw(17 + tID, 146, "%d  %c", position * 10, percent);
    refresh();
    mtx.unlock();
}

void showStatusDistiller(int tID, int position)
{
    mtx.lock();
    mvprintw(27 + tID, 125, "Gorzelnik %d: ", tID);
    for (int i = 0; i < 10; i++)
    {
        mvprintw(27 + tID, 136 + i, " ");
    }
    for (int i = 0; i < position; i++)
    {
        attron(COLOR_PAIR(3));
        mvprintw(27 + tID, 136 + i, " ");
        attroff(COLOR_PAIR(3));
    }
    char percent = 37;
    mvprintw(27 + tID, 146, "      ");
    mvprintw(27 + tID, 146, "%d  %c", position * 10, percent);
    refresh();
    mtx.unlock();
}

void showStatusForager(int tID, int position)
{
    mtx.lock();
    mvprintw(37 + tID, 125, "Paszarz %d: ", tID);
    for (int i = 0; i < 10; i++)
    {
        mvprintw(37 + tID, 136 + i, " ");
    }
    for (int i = 0; i < position; i++)
    {
        attron(COLOR_PAIR(4));
        mvprintw(37 + tID, 136 + i, " ");
        attroff(COLOR_PAIR(4));
    }
    char percent = 47;
    mvprintw(37 + tID, 146, "      ");
    mvprintw(37 + tID, 146, "%d  %c", position * 10, percent);
    refresh();
    mtx.unlock();
}

void showStatusFlour()
{
    mtx.lock();
    int counter = flourStatus;
    mvprintw(15, 95, "Maka : ");
    mvprintw(15, 95, "Maka : %d", flourStatus);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter > 0)
            {
                attron(COLOR_PAIR(1));
                mvprintw(i + 17, j + 95, "M");
                attroff(COLOR_PAIR(1));
                counter--;
            }
            else
            {
                mvprintw(i + 17, j + 95, " ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusAlcohol()
{
    mtx.lock();
    int counter = alcoholStatus;
    mvprintw(25, 95, "Alkohol : ");
    mvprintw(25, 95, "Alkohol : %d", alcoholStatus);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter > 0)
            {
                attron(COLOR_PAIR(3));
                mvprintw(i + 27, j + 95, "A");
                attroff(COLOR_PAIR(3));
                counter--;
            }
            else
            {
                mvprintw(i + 27, j + 95, " ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusForage()
{
    mtx.lock();
    int counter = forageStatus;
    mvprintw(35, 95, "Pasza : ");
    mvprintw(35, 95, "Pasza : %d", forageStatus);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (counter > 0)
            {
                attron(COLOR_PAIR(4));
                mvprintw(i + 37, j + 95, "P");
                attroff(COLOR_PAIR(4));
                counter--;
            }
            else
            {
                mvprintw(i + 37, j + 95, " ");
            }
        }
    }
    refresh();
    mtx.unlock();
}

void showStatusCustomer(int tID)
{
    mtx.lock();
    mvprintw(17 + tID, 35, "Klient %d:		%d		%d		%d		%d",
             tID, flourStatusCustomer[tID], alcoholStatusCustomer[tID], forageStatusCustomer[tID], cashCustomer[tID]);
    refresh();
    mtx.unlock();
}

void showStatusCash()
{
    mtx.lock();
    mvprintw(45, 35, "Pieniadze : ");
    mvprintw(45, 35, "Pieniadze : %d", cash);
    mvprintw(45, 35, "Wsploczynnik wzrostu : ");
    mvprintw(45, 35, "Wsploczynnik wzrostu : %d", growthRate);
    refresh();
    mtx.unlock();
}

//------------------------------------------------------------------------------------------------------------------------

void startTreadField()
{
    while (1)
    {
        showStatusField();
        while (fieldStatus > 0)
        {
            usleep(rand() % 10000 + (int)(10000 / growthRate));
            unique_lock<mutex> lck(mtx);
            fieldStatus++;
            lck.unlock();
            cv_field.notify_all();
            showStatusField();
        }
    }
}

void startTreadFarmer(int tID)
{
    while (1)
    {
        bool work = false;
        showStatusFarmer(tID, work);
        usleep(1000000);
        if (fieldStatus < 20)
        {
            unique_lock<mutex> lck(mtx);
            while (fieldStatus < 50)
                cv_field.wait(lck);
            lck.unlock();
        }
        mtx.lock();
        if ((counterFarmer[tID] <= counterFarmer[(2 * numFarmer + tID - 1) % numFarmer]) &&
            (counterFarmer[tID] <= counterFarmer[(2 * numFarmer + tID + 1) % numFarmer]))
        {
            if (fieldStatus > 0 && cutCereals < 100)
            {
                counterFarmer[tID]++;
                fieldStatus--;
                cutCereals++;
                work = true;
            }
        }
        mtx.unlock();
        cv_cutCereal.notify_one();
        showStatusField();
        showStatusCutCereals();
        if (work)
        {
            for (int i = 0; i < 10; i++)
            {
                showStatusFarmer(tID, work);
                usleep(rand() % 100000 + 100000);
            }
        }
    }
}

void startTreadDriver(int tID)
{
    while (1)
    {
        showStatusDriver(tID, 0);
        unique_lock<mutex> lck(mtx);
        while (cutCereals < 20)
            cv_cutCereal.wait(lck);
        lck.unlock();

        cutCereals -= 20;
        showStatusCutCereals();
        showStatusDriver(tID, 1);
        usleep(500000);

        lck.lock();
        while (factoryGrainStatus > 80)
            cv_factoryGrain.wait(lck);
        lck.unlock();
        factoryGrainStatus += 20;
        showStatusFactoryGrain();
        showStatusDriver(tID);
    }
}

void startTreadFactory(int tID)
{
    bool make;
    while (1)
    {
        make = false;
        mtx.lock();
        if (factoryGrainStatus > 0 && flourStatus <= 50 && forageStatus <= 50)
        {
            factoryGrainStatus--;
            make = true;
        }
        mtx.unlock();
        cv_factoryGrain.notify_one();
        showStatusFactoryGrain();

        if (make)
        {
            for (int i = 0; i < 10; i++)
            {
                usleep(rand() % 1000000 + 1000000);
                grainStatus++;
                showStatusFactory();
                if (i % 2)
                {
                    usleep(rand() % 1000000 + 1000000);
                    strawStatus++;
                    showStatusFactory();
                }
            }
        }
        if (cash >= 100 && growthRate < 100000)
        {
            cash -= 100;
            growthRate *= 2;
            showStatusCash();
        }
    }
}

void startTreadMiller(int tID)
{
    bool make;
    while (1)
    {
        make = false;
        mtx.lock();
        if ((grainStatus >= 7) && (flourStatus <= alcoholStatus) && (flourStatus <= forageStatus))
        {
            grainStatus -= 7;
            make = true;
        }
        mtx.unlock();
        showStatusFactory();
        if (make)
        {
            for (int i = 0; i <= 10; i++)
            {
                usleep(rand() % 100000 + 100000);
                showStatusMiller(tID, 1);
            }
            flourStatus++;
            showStatusFlour();
        }
    }
}

void startTreadDistiller(int tID)
{
    bool make;
    while (1)
    {
        make = false;
        mtx.lock();
        if ((grainStatus >= 7) && (alcoholStatus <= flourStatus) && (alcoholStatus <= forageStatus))
        {
            grainStatus -= 7;
            make = true;
        }
        mtx.unlock();
        showStatusFactory();
        if (make)
        {
            for (int i = 0; i <= 10; i++)
            {
                usleep(rand() % 100000 + 100000);
                showStatusDistiller(tID, 1);
            }
            alcoholStatus++;
            showStatusAlcohol();
        }
    }
}

void startTreadForager(int tID)
{
    bool make;
    while (1)
    {
        make = false;
        mtx.lock();
        if ((grainStatus >= 7 && strawStatus >= 14) && (forageStatus <= flourStatus) && (forageStatus <= alcoholStatus))
        {
            grainStatus -= 7;
            strawStatus -= 14;
            make = true;
        }
        mtx.unlock();
        showStatusFactory();
        if (make)
        {
            for (int i = 0; i <= 10; i++)
            {
                usleep(rand() % 100000 + 100000);
                showStatusForager(tID, 1);
            }
            forageStatus++;
            showStatusForage();
        }
    }
}

void startTreadCustomer(int tID)
{
    while (1)
    {
        int choice = rand() % 3 + 1;
        mtx.lock();
        switch (choice)
        {
        case 1:
            if (flourStatus > 0)
            {
                flourStatus--;
                cash += 10;
                flourStatusCustomer[tID]++;
                cashCustomer[tID] += 10;
            }
            break;
        case 2:
            if (alcoholStatus > 0)
            {
                alcoholStatus--;
                cash += 10;
                alcoholStatusCustomer[tID]++;
                cashCustomer[tID] += 10;
            }
            break;
        case 3:
            if (forageStatus > 0)
            {
                forageStatus--;
                cash += 10;
                forageStatusCustomer[tID]++;
                cashCustomer[tID] += 10;
            }
            break;
        default:
            break;
        }
        mtx.unlock();
        showStatusFlour();
        showStatusAlcohol();
        showStatusForage();
        showStatusCustomer(tID);
        showStatusCash();
        usleep(5000000);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

int main()
{
    initscr();
    nodelay(stdscr, TRUE);
    refresh();
    srand(time(NULL));
    clear();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_BLACK, COLOR_CYAN);
    curs_set(0);

    //--------------deklaracja watkow--------------
    thread field;
    thread farmer[numFarmer];
    thread driver[numDriver];
    thread producer[numProducer];
    thread miller[numMiller];
    thread distiller[numDistiller];
    thread forager[numForager];
    thread customer[numCustomer];

    showStatus();

    //--------------uruchamianie watkow------------
    field = thread(startTreadField);
    for (int i = 0; i < numFarmer; i++)
    {
        farmer[i] = thread(startTreadFarmer, i);
        counterFarmer[i] = 0;
    }
    for (int i = 0; i < numDriver; i++)
    {
        driver[i] = thread(startTreadDriver, i);
    }
    for (int i = 0; i < numProducer; i++)
    {
        producer[i] = thread(startTreadFactory, i);
    }
    for (int i = 0; i < numMiller; i++)
    {
        miller[i] = thread(startTreadMiller, i);
    }
    for (int i = 0; i < numDistiller; i++)
    {
        distiller[i] = thread(startTreadDistiller, i);
    }
    for (int i = 0; i < numForager; i++)
    {
        forager[i] = thread(startTreadForager, i);
    }
    for (int i = 0; i < numCustomer; i++)
    {
        customer[i] = thread(startTreadCustomer, i);
    }
    //--------------join watkow------------
    field.join();
    for (int i = 0; i < numFarmer; i++)
    {
        farmer[i].join();
    }
    for (int i = 0; i < numDriver; i++)
    {
        driver[i].join();
    }
    for (int i = 0; i < numProducer; i++)
    {
        producer[i].join();
    }
    for (int i = 0; i < numMiller; i++)
    {
        miller[i].join();
    }
    for (int i = 0; i < numDistiller; i++)
    {
        distiller[i].join();
    }
    for (int i = 0; i < numForager; i++)
    {
        forager[i].join();
    }
    for (int i = 0; i < numCustomer; i++)
    {
        customer[i].join();
    }

    endwin();
    return 0;
}
