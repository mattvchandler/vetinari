#include <array>
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <ctime>

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

void draw_line(std::vector<std::vector<unsigned char>> & img, int start_x, int start_y, int end_x, int end_y, float wt)
{
    auto draw = [&img](int x, int y)
    {
        if(y >= 0 && y < static_cast<int>(std::size(img)) && x >= 0 && x < static_cast<int>(std::size(img[y])))
            img[y][x] = 1;
    };

    if(start_x == end_x)
    {
        if(start_y > end_y)
            std::swap(start_y, end_y);

        for(int y = start_y; y <= end_y; ++y)
        {
            for(int x = start_x - static_cast<int>(std::floor(wt / 2.0f)); x < start_x + static_cast<int>(std::ceil(wt / 2.0f)); ++x)
            {
                draw(x, y);
            }
        }
        return;
    }
    else if(start_y == end_y)
    {
        if(start_x > end_x)
            std::swap(start_x, end_x);

        for(int x = start_x; x <= end_x; ++x)
        {
            for(int y = start_y - static_cast<int>(std::floor(wt / 2.0f)); y < start_y + static_cast<int>(std::ceil(wt / 2.0f)); ++y)
            {
                draw(x, y);
            }
        }
        return;
    }

    // modified Bresenham algorithm
    auto draw_x = start_x, draw_y = start_y;

    int dx = std::abs(end_x - draw_x);
    int dy = std::abs(end_y - draw_y);
    int sx = draw_x < end_x ? 1 : -1;
    int sy = draw_y < end_y ? 1 : -1;

    int err = dx - dy; // error value e_xy
    float ed = dx + dy == 0 ? 1.0f
                            : std::sqrt(static_cast<float>(dx) * static_cast<float>(dx)
                                      + static_cast<float>(dy) * static_cast<float>(dy));

    float wd = wt / 2.0f;
    while(true)
    {
        draw(draw_x, draw_y);

        int e2 = err;
        int x2 = draw_x;
        int y2 = draw_y;

        if(e2 * 2 >= -dx)  // x step
        {
            e2 += dy;
            while(static_cast<float>(e2) < ed * wd && (end_y != y2 || dx > dy))
            {
                y2 += sy;
                draw(draw_x, y2);
                e2 += dx;
            }
            if(draw_x == end_x)
                break;
            e2 = err;
            err -= dy;
            draw_x += sx;
        }
        if(e2 * 2 <= dy)  // y step
        {
            e2 = dx - e2;
            while(static_cast<float>(e2) < ed * wd && (end_x != x2 || dx < dy))
            {
                x2 += sx;
                draw(x2, draw_y);
                e2 += dy;
            }
            if(draw_y == end_y)
                break;
            err += dx;
            draw_y += sy;
        }
    }
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

    const unsigned int ring_thickness = 2;
    auto radius = std::min(width, height) / 2 - ring_thickness;
    auto inner_ring2 = (radius - ring_thickness)*(radius - ring_thickness);
    auto outer_ring2 = (radius + ring_thickness)*(radius + ring_thickness);

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

    auto now = std::time(nullptr);
    auto clock = std::localtime(&now);

    auto hour = clock->tm_hour % 12; // we just need an angle, so no worries about am/pm/noon/midnight 0-11 is perfect
    auto min  = clock->tm_min;
    auto sec  = clock->tm_sec;

    auto hour_angle = M_PI / 180.0f * (hour * 30.0f + min / 10.0f + sec / 600.0f);
    auto min_angle  = M_PI / 180.0f * (min  *  6.0f + sec / 10.0f);
    auto sec_angle  = M_PI / 180.0f * (sec  *  6.0f);

    auto hour_len = radius * 0.50f;
    auto min_len  = radius * 0.75f;
    auto sec_len  = radius * 0.90f;

    auto hour_x = std::sin(hour_angle) *  hour_len + center_x;
    auto hour_y = std::cos(hour_angle) * -hour_len + center_y;
    auto min_x  = std::sin(min_angle)  *  min_len  + center_x;
    auto min_y  = std::cos(min_angle)  * -min_len  + center_y;
    auto sec_x  = std::sin(sec_angle)  *  sec_len  + center_x;
    auto sec_y  = std::cos(sec_angle)  * -sec_len  + center_y;

    draw_line(img, center_x, center_y, hour_x, hour_y, 4.0f);
    draw_line(img, center_x, center_y, min_x,  min_y,  3.0f);
    draw_line(img, center_x, center_y, sec_x,  sec_y,  2.0f);

    // render to terminal
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

        if(row + 1 < height)
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
        sleep(1);
        // int c = std::cin.get();
        // if(c == 'q' || c == 'Q')
        //     break;
    }

    return 0;
}
