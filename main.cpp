#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <set>
#include <stdlib.h>
#include <filesystem>
#include <windows.h>

#include <SFML/Audio.hpp>

#include "colorset.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    int tall = 10;
    bool without_music = false;
    const std::set<std::string_view> available_exts = {".flac", ".wav", ".ogg", ".mp3"};
    fs::path music_folder = {"./music"};
    std::vector<std::string> music_names;


    for(int i = 1; i < argc; i++)
    {
        std::string_view arg = argv[i];
        bool uknown_flag = false;

        if(arg == "-nm" || arg == "-nomusic")
            without_music = true;
        else if(i + 1 != argc)
        {
            i++;
            std::string_view arg_value = argv[i];

            if(arg == "-f" || arg == "--folder")
            {
                music_folder = arg_value;
                if(!fs::exists(music_folder) || !fs::is_directory(music_folder))
                {
                    std::cerr << music_folder << " did not exist or not a folder\n";
                    return EXIT_FAILURE;
                }
            }
            else if(arg == "-t" || arg == "--tall")
            {
                if(auto result = std::from_chars(arg_value.data(), arg_value.data() + arg_value.size(), tall);
                    result.ec == std::errc::invalid_argument)
                {
                    std::cerr << "bad tall size\n";
                    return EXIT_FAILURE;
                }
            }
            else
                uknown_flag = true;
            i -= uknown_flag;
        }
        else
            uknown_flag = true;

        if(uknown_flag)
            std::cout << "unknown flag \"" << arg << "\", ignored\n";
    }

    if(!without_music)
    {
        if(!fs::exists(music_folder) )
        {
            std::cerr << music_folder << " folder did not exist";
            return EXIT_FAILURE;
        }
        else for(const auto &p : fs::recursive_directory_iterator(music_folder))
        {
            if(!fs::is_regular_file(p))
                continue;
            if(const auto &path = p.path(); available_exts.contains(path.extension().string()))
                music_names.push_back(path.filename().string());
        }
    }

    for(auto &name : music_names)
        std::cout << name << '\n';
    std::cout << "\nwere found in " << music_folder.filename() << "\nctrl + c to exit\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));


    int w, h;
    int offtop_x, offtop_y;

    sf::Music music;
    auto music_iterator = music_names.end();
    fs::path music_path;
    std::string output;
    for(int frame = 0; true; frame++)
    {
        if(!without_music && music.getStatus() != sf::Music::Status::Playing)
        {
            if(music_iterator != music_names.end())
                music_iterator++;
            else
                music_iterator = music_names.begin();

            music_path = music_folder / *music_iterator;
            if(music.openFromFile(music_path))
                music.play();
            else
                std::cerr << "unable to open: " << *music_iterator;
        }
        output.clear();

        {
            static auto console = std::invoke([&]{
                struct {
                    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
                    CONSOLE_SCREEN_BUFFER_INFO Info;
                } console;
                GetConsoleScreenBufferInfo(console.handle, &console.Info);
                return console;
            });
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(console.handle, &csbi);
            DWORD written;
            COORD topLeft = {0, 0};
            DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
            FillConsoleOutputCharacter(console.handle, ' ', size, topLeft, &written);
            FillConsoleOutputAttribute(console.handle, csbi.wAttributes, size, topLeft, &written);

            static auto setCursorPosition = [&](short x, short y){
                COORD coord = {x, y};
                SetConsoleCursorPosition(console.handle, coord);
            };
            setCursorPosition(0, 0);
            w = console.Info.srWindow.Right - console.Info.srWindow.Left + 1;
            h = console.Info.srWindow.Bottom - console.Info.srWindow.Top + 10;
        }


        offtop_x = w / 2 - tall;
        offtop_y = h * 3 / 4 - tall;

        static auto snow = [](int s){
            std::string line(s, ' ');
            for(char &c : line)
                if(rand() % 30 == 0)
                    c = '.';
            return line;
        };

        static auto toy = [&frame](int x, int y){
            std::string toy;
            if(x % 4 == 0)
            {
                if(y % 2 == frame % 2)
                    toy += clr::on_red;
                else
                    toy += clr::on_cyan;
                toy += '&';
            }
            else
                toy += clr::green + '*';
            return toy + clr::white;
        };

        for(int y = 0; y < offtop_y; y++)
            output += snow(w) + '\n';
        output += snow(tall + offtop_x + 1);
        output += clr::bright_yellow + '*' + clr::white + '\n';
        for(int y = 0; y < tall; y++)
        {
            output += snow(tall - y + offtop_x) + '/';
            if(frame % 40 > 20)
            {
                for(int x = 0; x < y * 2 + 1; x++)
                {
                    if(rand() % (30 + tall) > clr::toys_clrs.size())
                        output += clr::green + '*' + clr::white;
                    else
                    {
                        output += clr::toys_clrs[rand() % clr::toys_clrs.size()];
                        output += '&' + clr::white;
                    }
                }
            }
            else if(frame % 20 > 10)
                for(int x = y * 2 + 1; x > 0; x--)
                    output += toy(x, y);
            else
                for(int x = 0; x < y * 2 + 1; x++)
                    output += toy(x, y);
            output += clr::white + '\\' + snow(tall - y + offtop_x - 3) + '\n';
        }

        output += clr::underline + snow(offtop_x);
        std::string under_tree(tall + 1, ' ');
        output += under_tree;
        output += clr::white + clr::underline + clr::brown + '#' + clr::white + clr::underline;
        output += under_tree + snow(offtop_x - 3) + '\n';

        output += clr::white;
        if(!without_music)
            output += music_path.stem().string();

        std::cout << output << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return EXIT_SUCCESS;
}
