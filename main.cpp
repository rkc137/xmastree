#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

#include <termcolor/termcolor.hpp>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;

void snow(int s)
{
    std::string line(s, ' ');
    for(char &c : line)
        if(rand() % 30 == 0)
            c = '.';
    std::cout << line;
}

void trees_toys(int x, int y, int frame)
{
    if(x % 4 == 0)
    {
        if(y % 2 == frame % 2)
            std::cout << termcolor::on_red;
        else
            std::cout << termcolor::on_cyan;
        std::cout << '&' << termcolor::reset;
    }
    else
        std::cout << termcolor::green << '*' << termcolor::reset;
}

int main(int argc, char *argv[])
{
    int tall = 10;
    bool is_ingore_mp3 = false;
    bool is_ingore_other_files = false;
    std::string music_folder_name = "music";
    for(int i = 1; i < argc; i++)
    {
        if(i + 1 != argc)
        {
            if(!strcmp(argv[i], "-f"))
            {                 
                music_folder_name = argv[i + 1];
                i++;
                continue;
            }
            if(!strcmp(argv[i], "-t"))
            {                 
                tall = atoi(argv[i + 1]);
                i++;
                continue;
            }
        }
        if(!is_ingore_mp3) is_ingore_mp3 = !strcmp(argv[i], "-ignmp3");
        if(!is_ingore_other_files) is_ingore_other_files = !strcmp(argv[i], "-ign");
    }
    
    const std::set<std::string> available_exts = {".flac", ".wav", ".ogg"};
    std::set<std::string> music_names;
    for(auto &p: fs::recursive_directory_iterator(music_folder_name))
    {
        std::string ext(p.path().extension().string()),
               str_path(p.path().string()),
              file_name(str_path.substr(music_folder_name.size() + 1, str_path.size()));
        
        if(fs::is_directory(p))
            continue;
        if(ext == ".mp3")
        {
            if(is_ingore_mp3 || is_ingore_other_files)
                continue;
            std::cout << "mp3 is not working, so you need to use -ign to ignore all not available\n"
                      << "files or -ignmp3 to ignore mp3 files in folder\n" 
                      << "and ofcse you can just format " 
                      << file_name << " to some of there:\n";
            for(auto ext : available_exts)
                std::cout << ext << '\n';
            return 1;
        }
        if(available_exts.find(ext) == available_exts.end())
        {
            if(is_ingore_other_files)
                continue;
            std::cout << file_name << " is wrong format\nyou can use -ign to ignore all not available files\n";
            return 1;
        }
        if(fs::is_regular_file(p))
            music_names.insert(file_name);
    }

    for(auto &name : music_names)
        std::cout << name << '\n';
    std::cout << "\nwere found in " << music_folder_name << "/\nctrl + c to exit\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));


    int w, h;
    int offtop_x, offtop_y;
    
    sf::Music music;
    auto music_iterator = music_names.end();
    for(int frame = 0; true; frame++)
    {
        #ifdef __linux__ 
            std::system("clear");
        #elif _WIN32
            std::system("cls");
        #else
            std::cout << ":)))\n\n\n\n\n\n\n\n\n";
        #endif

        if(music.getStatus() != sf::Music::Status::Playing)
        {
            if(music_iterator != music_names.end())
                music_iterator++;
            else
                music_iterator = music_names.begin();

            music.openFromFile(music_folder_name + '/' + *music_iterator);
            music.play();
        }
        
        #ifdef __linux__ 
            w = WEXITSTATUS(std::system("exit `tput cols`"));
            h = WEXITSTATUS(std::system("exit `tput lines`"));
        #elif _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;        
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        #else
            w = 40;
            h = 30;
        #endif
      
        offtop_x = w / 2 - tall;
        offtop_y = h * 3 / 4 - tall;

        for(int y = 0; y < offtop_y; y++)
        {
            snow(w);
            std::cout << '\n';
        }
        snow(tall + offtop_x + 1);
        std::cout << termcolor::bright_yellow << '*' << termcolor::reset << '\n';
        for(int y = 0; y < tall; y++)
        {
            snow(tall - y + offtop_x);
            std::cout << '/';
            if(frame % 40 > 20)
            {
                for(int x = 0; x < y * 2 + 1; x++)
                {
                    switch(rand() % (30 + tall))
                    {
                    case 0:
                        std::cout << termcolor::on_blue;
                        break;
                    case 1:
                        std::cout << termcolor::on_yellow;
                        break;
                    case 2:
                        std::cout << termcolor::on_red;
                        break;
                    case 3:
                        std::cout << termcolor::on_magenta;
                        break;
                    case 4:
                        std::cout << termcolor::on_cyan;
                        break;
                    default:
                        std::cout << termcolor::green << '*' << termcolor::reset;
                        continue;
                    }
                    std::cout << '&' << termcolor::reset;
                }
            }
            else if(frame % 20 > 10)
                for(int x = y * 2 + 1; x > 0; x--)
                    trees_toys(x, y, frame);
            else
                for(int x = 0; x < y * 2 + 1; x++)
                    trees_toys(x, y, frame);
            std::cout << termcolor::reset << '\\';
            snow(tall - y + offtop_x - 3);
            std::cout << '\n';
        }

        std::cout << termcolor::underline;
        snow(offtop_x);
        std::string under_tree(tall + 1, ' ');
        std::cout << under_tree;
        std::cout << termcolor::reset << termcolor::color<101, 67, 33> << '#' << termcolor::reset << termcolor::underline;
        std::cout << under_tree;
        snow(offtop_x - 3);

        int file_name_start = music_iterator->find_last_of('/') + 1;
        std::cout << '\n' << termcolor::reset 
                  << music_iterator->substr(file_name_start, music_iterator->find_last_of('.') - file_name_start)
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}