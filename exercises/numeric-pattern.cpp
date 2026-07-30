

#include <iostream>
using namespace std;

int main()

{

int i = 1;

while (i <= 4)

{

      int num = 1;

      for (int j = 1; j <= i; j++)

      {

        cout << num << "bb";

        num *= 3;

      }

      cout << endl;
      i++;

    }

return 0;

  }