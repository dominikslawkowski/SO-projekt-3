#include <iostream>
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end

using namespace std;

char *W_BAZIE = "w bazie";
char *NA_POLU_BITWY = "na polu bitwy";

const int liczbaMieczy = 3;

int znajdzWolnyPrzedmiot(int tab[liczbaMieczy][2], int ilosc)
{
    int index_miecza = -1;
    for (int i = 0; i < ilosc; i++)
    {
        if (tab[i][0] == -1)
        {
            index_miecza = i;
            break;
        }
    }
    return index_miecza;
}

int main()
{
    int miecz[5][2] = {{1, 1}, {1, 0}, {-1, 1}};

    cout << "wolny miecz: " << znajdzWolnyPrzedmiot(miecz, sizeof(miecz) / sizeof(miecz[0])) << endl;

    return 0;
}