#include <iostream>

#include <csignal>
#include <cstdlib>

#include <termios.h>
#include <unistd.h>

#define ESC "\x1B"
#define CSI ESC "["
#define ENABLED "h"
#define DISABLED "l"
#define ALT_BUFF CSI "?1049"
#define CURSOR CSI "?25"
#define CLS CSI "2J"

static struct
{
    unsigned short rows = 0, cols =0;
    termios t;
} term_info;

void cleanup()
{
    std::cout<<ALT_BUFF ENABLED<<std::flush;
    std::cout<<CLS ALT_BUFF DISABLED CURSOR ENABLED<<std::flush;
    tcsetattr(STDIN_FILENO, TCSANOW, &term_info.t);
}

void cleanup_exit(int)
{
    cleanup();
    std::exit(1);
}

void resize(int)
{
    winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    term_info.rows = ws.ws_row;
    term_info.cols = ws.ws_col;
    std::cout<<term_info.rows<<' '<<term_info.cols<<'\n';
}

int main(int argc, char * argv[])
{
    std::atexit(cleanup);
    std::signal(SIGINT, cleanup_exit);
    std::signal(SIGTERM, cleanup_exit);
    std::signal(SIGWINCH, resize);

    std::cout<<ALT_BUFF ENABLED CLS CURSOR DISABLED<<std::flush;
    tcgetattr(STDIN_FILENO, &term_info.t); // save old term attrs
    auto newt = term_info.t;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while(true)
    {
        int c = std::cin.get();
        if(c == 'q' || c == 'Q')
            break;
    }

    return 0;
}
