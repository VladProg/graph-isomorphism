#include "algorithms.h"
#include "test.h"
#include <iostream>

int main()
{
    for(int i = 3; i <= 11; i++)
        test(i, {
                     NAMED_ALGORITHM(mckay),
                     NAMED_ALGORITHM(ullmann),
                     NAMED_ALGORITHM(naive)
            }, 5000);

    test(12, {
                 NAMED_ALGORITHM(mckay),
                 NAMED_ALGORITHM(ullmann),
                 NAMED_ALGORITHM(naive)
        }, 500);

    for(int i = 20; i <= 70; i += 10)
        test(i, {
                     NAMED_ALGORITHM(mckay),
                     NAMED_ALGORITHM(ullmann),
            }, 5000);

    test(80, {
                 NAMED_ALGORITHM(mckay),
                 NAMED_ALGORITHM(ullmann),
        }, 500);

    for(int i = 100; i <= 300; i += 100)
        test(i, {
                     NAMED_ALGORITHM(mckay)
            }, 5000);

    test(400, {
                 NAMED_ALGORITHM(mckay)
        }, 500);
}
