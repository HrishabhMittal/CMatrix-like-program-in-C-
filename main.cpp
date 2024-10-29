#include <iostream>
#include <thread>
#include <random>
#include <string>
using namespace std;
#define WIDTH 100
#define HEIGHT 40
char arr[HEIGHT][WIDTH];
bool b[WIDTH];
bool bb[HEIGHT][WIDTH];
default_random_engine generator;
uniform_int_distribution<int> distribution(0,10000);
double rrandom() {
    double x=(double)distribution(generator);
    return x/10000.;
}
bool randbool(double trueProb=0.5) {
    return rrandom()<trueProb;
}
struct vec2 {
    int x,y;
    vec2(int x,int y): x(x),y(y) {};
};
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
int enableEscapeSquences() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return GetLastError();
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return GetLastError();
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return GetLastError();
    return 0;
}
#else
int enableEscapeSquences() {return 0;}
#endif
void update() {
    for (int j=0;j<WIDTH;j++) {
        for (int i=HEIGHT-1;i>0;i--) {
            bb[i][j]=bb[i-1][j];
        }
        if (randbool(0.2)) bb[0][j]=!bb[0][j];
    }
}
int main() {
    for (int i=0;i<WIDTH;i++) {
        b[i]=randbool();
    }
    for (int i=0;i<HEIGHT;i++) {
        for (int j=0;j<WIDTH;j++) {
            arr[i][j]=(int)(rrandom()*93)+33;
            bb[i][j]=false;
        }
    }
    enableEscapeSquences();
    cout << "\033[H\033[2J";
    while (true) {
        cout << ("\033[8;"+to_string(HEIGHT)+';'+to_string(WIDTH)+"t");
        update();
        cout << "\033[?25l";
        cout << "\033[H";
        string s;
        string prevcol="";
        for (int i=0;i<HEIGHT;i++) {
            for (int j=0;j<WIDTH;j++) {
                string col;
                if (bb[i][j]) {
                    if (i+1<HEIGHT && !bb[i+1][j]) {
                        col="\033[38;2;75;220;255m";
                    } else col="\033[38;2;30;125;200m";
                } else {
                    col="\033[38;2;50;50;50m";
                }
                if (prevcol==col) s+=arr[i][j];
                else {
                    s+=col+arr[i][j];
                    prevcol=col;
                }
            }
            s+='\n';
        }
        s.pop_back();
        cout << s;
        this_thread::sleep_for(50ms);
    }
    cout << "\033[?25h";
}
