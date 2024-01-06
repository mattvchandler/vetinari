#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#define _USE_MATH_DEFINES
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

#ifdef TERMUX_VIBRATE
#include <list>
#elif defined(PULSEAUDIO_FOUND)
#include <pulse/simple.h>
#include <pulse/error.h>
#include "ticktock.h"
#endif

#define ESC "\x1B"
#define CSI ESC "["
#define ENABLED "h"
#define DISABLED "l"
#define ALT_BUFF CSI "?1049"
#define CURSOR CSI "?25"
#define CLS CSI "2J"

void move_cusor(int row, int col)
{
    std::cout << CSI << row + 1 << ';' << col + 1 << 'H';
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

static struct
{
    unsigned short rows = 0, cols = 0;
    termios t;
    bool resized = true;
} term_info;

void draw()
{
    // we have a cols x rows grid of chars
    // each char can be divided into quadrants, where
    // each quadrant is a 1x2 pixel
    // using quad witdh as our unit, we have 2*cols x 4*rows grid

    int width  = term_info.cols * 2;
    int height = term_info.rows * 4;
    auto square = std::min(width, height);

    auto img = std::vector(square, std::vector<unsigned char>(square, 0));

    auto center = square / 2;

    const int ring_thickness = 2;
    auto radius = square / 2 - ring_thickness;
    auto inner_ring2 = (radius - ring_thickness)*(radius - ring_thickness);
    auto outer_ring2 = (radius + ring_thickness)*(radius + ring_thickness);

    // draw outer ring
    for(auto row = 0; row < square; ++row)
    {
        for(auto col = 0; col < square; ++col)
        {
            int y = center - row;
            int x = center - col;
            auto r2 = x*x + y*y;
            img[row][col] = r2 >= inner_ring2 && r2 < outer_ring2;
        }
    }

    // draw hour markers
    for(int i = 0; i < 12; ++i)
    {
        auto angle = M_PI / 6.0f * i;
        draw_line(img, center + std::sin(angle) * radius * 0.8f,
                       center - std::cos(angle) * radius * 0.8f,
                       center + std::sin(angle) * radius,
                       center - std::cos(angle) * radius,
                       ring_thickness * 1.5f);
    }

    // draw hands
    auto now = std::time(nullptr);
    auto clock = std::localtime(&now);

    auto hour = clock->tm_hour % 12; // we just need an angle, so no worries about am/pm/noon/midnight 0-11 is perfect
    auto min  = clock->tm_min;
    auto sec  = clock->tm_sec;

    auto hour_angle = M_PI / 180.0f * ((static_cast<float>(hour) + min / 60.0f + sec / 3600.0f) * 30.0f);
    auto min_angle  = M_PI / 180.0f * ((static_cast<float>(min)  + sec / 60.0f)                 *  6.0f);
    auto sec_angle  = M_PI / 180.0f *  (static_cast<float>(sec)  *  6.0f);

    auto hour_len = radius * 0.50f;
    auto min_len  = radius * 0.75f;
    auto sec_len  = radius * 0.90f;

    auto hour_x = std::sin(hour_angle) *  hour_len + center;
    auto hour_y = std::cos(hour_angle) * -hour_len + center;
    auto min_x  = std::sin(min_angle)  *  min_len  + center;
    auto min_y  = std::cos(min_angle)  * -min_len  + center;
    auto sec_x  = std::sin(sec_angle)  *  sec_len  + center;
    auto sec_y  = std::cos(sec_angle)  * -sec_len  + center;

    draw_line(img, center, center, hour_x, hour_y, 4.0f);
    draw_line(img, center, center, min_x,  min_y,  3.0f);
    draw_line(img, center, center, sec_x,  sec_y,  2.0f);

    // render to terminal
    std::cout<<CLS;

    for(auto row = 0; row < square; row += 4)
    {
        move_cusor(row / 4 + (height - square) / 8, (width - square) / 4);
        for(auto col = 0; col < square; col += 2)
        {
            auto cell =
                img[row    ][col    ] << 3 |
                img[row    ][col + 1] << 2 |
                img[row + 1][col    ] << 1 |
                img[row + 1][col + 1];

            static const std::array<std::string, 16> disp {
                " ", "▗", "▖", "▄",
                "▝", "▐", "▞", "▟",
                "▘", "▚", "▌", "▙",
                "▀", "▜", "▛", "█"
            };
            // static const std::array<std::string, 16> disp {
            //     " ", ".", ",", "_",
            //     "'", "]", "/", "d",
            //     "`", "\\", "[", "L",
            //     "-", "7", "P", "M"
            // };

            std::cout<<disp[cell];
        }
    }

    std::cout.flush();
    term_info.resized = false;
}

#ifdef TERMUX_VIBRATE
// cpp silliness follows
#define STRX(x) #x
#define STR(x) STRX(x)
#define TERMUX_VIBRATE_EXE STR(TERMUX_VIBRATE)
class Audio
{
public:
    ~Audio()
    {
        for(auto && t: thread_pool)
            t.join();
    }
    void tick()
    {
        cleanup();
        if(!errored)
            thread_pool.emplace_back(&Audio::vibrate, this, true);
    }
    void tock()
    {
        cleanup();
        if(!errored)
            thread_pool.emplace_back(&Audio::vibrate, this, false);
    }
private:

    void vibrate(bool tick)
    {
        std::this_thread::sleep_for(sleep_delay);
        if(!errored)
        {
            auto cmd = tick ? TERMUX_VIBRATE_EXE " -fd 50" : TERMUX_VIBRATE_EXE " -fd 100";
            if(std::system(cmd) != 0)
                errored = true;
        }
    }

    // it takes a while for the os to launch and execute the termux-vibrate command, so add a delay to catch the next tick/tock
    // it won't line up with the random offsets, but maybe we'll just say that adds to the wrongness of the clock
    std::chrono::milliseconds sleep_delay{std::chrono::milliseconds{400}};

    std::atomic<bool> errored {false};
    std::list<std::thread> thread_pool;

    void cleanup()
    {
        while(thread_pool.front().joinable())
        {
            thread_pool.front().join();
            thread_pool.erase(std::begin(thread_pool));
        }
    }

};
#elif defined(PULSEAUDIO_FOUND)
class Audio
{
public:
    Audio(): pa {pa_simple_new(
                    nullptr,            // Use the default server.
                    "Vetinari clock",   // Our application's name.
                    PA_STREAM_PLAYBACK,
                    nullptr,            // Use the default device.
                    "tick-tock",        // Description of our stream.
                    &spec,              // Our sample format.
                    nullptr,            // Use default channel map
                    nullptr,            // Use default buffering attributes.
                    nullptr             // Ignore error code.
                    )}
    {}

    ~Audio()
    {
        if(pa)
        {
            pa_simple_free(pa);
        }
    }

    void tick() { play(tick_pcm, tick_pcm_len); }
    void tock() { play(tock_pcm, tock_pcm_len); }

private:
    pa_sample_spec spec
    {
        .format = PA_SAMPLE_S16LE,
        .rate = 8000,
        .channels = 1
    };
    pa_simple * pa {nullptr};

    void play(const unsigned char * pcm, unsigned int pcm_len)
    {
        if(pa)
        {
            pa_simple_write(pa, pcm, pcm_len, nullptr);
            pa_simple_drain(pa, nullptr);
        }
    }
};
#else
class Audio
{
public:
    void tick() {}
    void tock() {}
};
#endif

void resize(int)
{
    winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);
    term_info.rows = ws.ws_row;
    term_info.cols = ws.ws_col;
    term_info.resized = true;
    draw(); // TODO: not safe
}

void set_terminal()
{
    std::cout<<ALT_BUFF ENABLED CLS CURSOR DISABLED<<std::flush;
    tcgetattr(STDIN_FILENO, &term_info.t); // save old term attrs
    auto newt = term_info.t;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    resize(0);
}
void cleanup()
{
    std::cout<<ALT_BUFF ENABLED<<std::flush;
    std::cout<<CLS ALT_BUFF DISABLED CURSOR ENABLED<<std::flush;
    tcsetattr(STDIN_FILENO, TCSANOW, &term_info.t);
}

void cleanup_exit(int)
{
    cleanup(); // TODO: not safe
    std::exit(1);
}

void suspend(int)
{
    cleanup(); // TODO: not safe

    // set default SIGTSTP handlerer
    if(signal(SIGTSTP, SIG_DFL) == SIG_ERR)
        std::exit(1);

    // unblock SIGTSTP so we can raise it again with the default handler
    raise(SIGTSTP);
    sigset_t save_mask, mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    if(sigprocmask(SIG_UNBLOCK, &mask, &save_mask) == -1)
        std::exit(1);

    // suspend, will continue here on resume

    if(sigprocmask(SIG_SETMASK, &save_mask, nullptr) == -1)
        std::exit(1);

    struct sigaction action{};
    sigemptyset(&action.sa_mask);
    action.sa_flags |= SA_RESTART;
    action.sa_handler = suspend;

    if(sigaction(SIGTSTP, &action, nullptr) == -1)
        std::exit(1);

    set_terminal(); // TODO: not safe
}

void set_signal(int sig, void(*handler)(int))
{
    std::string sigstr;
    #define CASESTR(x) case x: sigstr = #x; break;
    switch(sig)
    {
        CASESTR(SIGINT)
        CASESTR(SIGTERM)
        CASESTR(SIGWINCH)
        CASESTR(SIGTSTP)
        default: sigstr = std::to_string(sig); break;
    }
    #undef CASESTR
    struct sigaction action{};
    if(sigaction(sig, nullptr, &action) == -1)
    {
        std::cerr<<"Warning - could not get signal "<< sigstr <<": "<<std::strerror(errno)<<'\n';
        return;
    }
    if(!(action.sa_flags & SA_SIGINFO) && action.sa_handler == SIG_IGN)
    {
        std::cerr<<"Warning - signal "<< sigstr <<" is ignored\n";
        return;
    }
    sigemptyset(&action.sa_mask);
    action.sa_flags &= ~SA_SIGINFO;
    action.sa_flags |= SA_RESTART;
    action.sa_handler = handler;
    if(sigaction(sig, &action, nullptr) == -1)
    {
        std::cerr<<"Warning - could not set signal "<< sigstr <<": "<<std::strerror(errno)<<'\n';
        return;
    }
}

int main(int argc, char * argv[])
{
    std::atexit(cleanup);
    set_signal(SIGINT, cleanup_exit);
    set_signal(SIGTERM, cleanup_exit);
    set_signal(SIGWINCH, resize);
    set_signal(SIGTSTP, suspend);

    set_terminal();

    std::default_random_engine rnd_eng{std::random_device{}()};
    std::bernoulli_distribution wrong_tick {0.1f}; // probability of wrong_tick
    std::bernoulli_distribution skip_tock {0.1f}; // probability of tick-tick or tock-tock vs tick-tock
    std::normal_distribution tick_mod {1.0f, 0.75f}; // random tick length

    Audio audio;
    bool ticktock = true;

    while(true)
    {
        draw();

        if(ticktock)
            audio.tick();
        else
            audio.tock();
        if(!skip_tock(rnd_eng))
            ticktock = !ticktock;

        auto tick_length = std::chrono::duration<float>{
            wrong_tick(rnd_eng) ? std::max(0.0f, tick_mod(rnd_eng)) : 1.0f};

        std::this_thread::sleep_for(tick_length);
    }

    return 0;
}
