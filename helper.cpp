#include <iostream>
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end

using namespace std;

char *W_BAZIE = "w bazie";
char *NA_POLU_BITWY = "na polu bitwy";

int zwrocPrzesuniecie(int status)
{
    switch (status)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 14;
        break;
    }
}

int main()
{
    cout << "w bazie " << zwrocPrzesuniecie(1) << endl;

    return 0;
}