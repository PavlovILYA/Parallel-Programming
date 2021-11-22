#include <iostream>
#include <math.h>

using namespace std;

double f(double x)
{
    return sin(x)*x+cos(x*x)*x*x;
}

int main()
{
    double x = 0;
    double cur_min = f(x);
    double cur_max = f(x);
    x += 0.0001;

    while(x < 100)
    {
        double cur_f = f(x);

        if (cur_f < cur_min)
            cur_min = cur_f;

        if (cur_f > cur_max)
            cur_max = cur_f;

        x += 0.0001;
    }

    cout << "f = sin(x)*x+cos(x*x)*x*x" << endl;
    cout << "min(f) = " << cur_min << endl;
    cout << "max(f) = " << cur_max << endl;
    return 0;
}
