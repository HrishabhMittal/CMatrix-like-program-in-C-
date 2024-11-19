#include <iostream>
#include <thread>
#include <random>
#include <string>
#include <vector>
using namespace std;
int height,width;
vector<vector<char>> arr(500,vector(500,'\0'));
vector<vector<bool>> bb(500,vector(500,false));
vector<bool> b(500,false);
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
vector<int> getConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {rows,columns};
}
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
#include <sys/ioctl.h>
void getConsoleSize(int& h,int& w) {
    struct winsize wi;
    if (ioctl(1, TIOCGWINSZ, &wi) == -1) {
        perror("ioctl");
    }
    h=wi.ws_row;
    w=wi.ws_col;
}
void update() {
    for (int j=0;j<width;j++) {
        for (int i=height-1;i>0;i--) {
            if (!bb[i][j] && bb[i-1][j]) {
                arr[i][j]=(int)(rrandom()*93)+33;
            }
            bb[i][j]=bb[i-1][j];
        }
        if (randbool(0.1)) bb[0][j]=!bb[0][j];
    }
}
int main() {
    for (int i=0;i<500;i++) {
        b[i]=randbool();
    }
    for (int i=0;i<500;i++) for (int j=0;j<500;j++) arr[i][j]=(int)(rrandom()*93)+33;
    enableEscapeSquences();
    cout << "\033[H\033[2J";
    while (true) {
        getConsoleSize(height,width);
        if (height>500 || width>500) return 1;
        update();
        cout << "\033[?25l";
        cout << "\033[H";
        string s;
        string prevcol="";
        for (int i=0;i<height;i++) {
            for (int j=0;j<width;j++) {
                string col;
                if (bb[i][j]) {
                    if (i+1<height && !bb[i+1][j]) {
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
