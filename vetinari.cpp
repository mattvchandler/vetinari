#include <array>
#include <iostream>
#include <vector>

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
    unsigned short rows = 0, cols = 0;
    termios t;
    bool resized = true;
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

void draw(); // TODO remove?
void resize(int)
{
    winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    term_info.rows = ws.ws_row;
    term_info.cols = ws.ws_col;
    term_info.resized = true;
    draw(); // TODO remove
}

void draw()
{
    // we have a cols x rows grid of chars
    // each char can be divided into quadrants, where
    // each quadrant is a 1x2 pixel
    // using quad with as our unit, we have 2*cols x 4*rows grid

    int width  = term_info.cols * 2;
    int height = term_info.rows * 4;

    auto img = std::vector(height, std::vector<unsigned char>(width, 0));

    auto center_x = width / 2;
    auto center_y = height / 2;

    const unsigned int thickness = 2;
    auto radius = std::min(width, height) / 2 - thickness;
    auto inner_ring2 = (radius - thickness)*(radius - thickness);
    auto outer_ring2 = (radius + thickness)*(radius + thickness);

    // draw outer ring
    for(auto row = 0; row < height; ++row)
    {
        for(auto col = 0; col < width; ++col)
        {
            int y = center_y - row;
            int x = center_x - col;
            auto r2 = x*x + y*y;
            img[row][col] = r2 >= inner_ring2 && r2 < outer_ring2;
        }
    }

    std::cout<<CLS;

    for(auto row = 0; row < height; row += 4)
    {
        for(auto col = 0; col < width; col += 2)
        {
            auto cell =
                img[row    ][col    ] << 3 |
                img[row    ][col + 1] << 2 |
                img[row + 1][col    ] << 1 |
                img[row + 1][col + 1];

            static const std::array<std::string, 16> disp
            {
                " ", "▗", "▖", "▄",
                "▝", "▐", "▞", "▟",
                "▘", "▚", "▌", "▙",
                "▀", "▜", "▛", "█"
            };

            std::cout<<disp[cell];
        }
        std::cout<<'\n';
    }

    std::cout.flush();
    term_info.resized = false;
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

    resize(0);

    while(true)
    {
        draw();
        int c = std::cin.get();
        if(c == 'q' || c == 'Q')
            break;
    }

    return 0;
}
