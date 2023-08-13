#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <experimental/filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

#include <termcolor/termcolor.hpp>
#include <SFML/Audio.hpp>

namespace fs = std::experimental::filesystem;

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
    int tall = (argc == 1) ? 10 : atoi(argv[1]);
    if(argc > 2)
    {
        std::cout << "usage: xmstree [tall]\n";
        return 0;
    }

    const std::set<std::string> available_exts = {".flac", ".wav", ".ogg"};
    std::string music_folder_name = "music";
    
    std::set<std::string> music_names;
    for(auto & p: fs::recursive_directory_iterator(music_folder_name))
    {
        //i think in one moment i will hate myself for that
        std::string ext(p.path().extension().string()),
               str_path(p.path().string());             //oop moment
        if(fs::is_regular_file(p) && (available_exts.find(ext) != available_exts.end()))
            music_names.insert(str_path.substr(music_folder_name.size() + 1, str_path.size()));
    }
    
    sf::Music music;
    for(auto &name : music_names)
        std::cout << name << std::endl;
    std::cout << "\nwere found in ./" << music_folder_name << "/\nctrl + c to exit\n";
    
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::set<std::string>::iterator music_iterator = music_names.begin();
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
            {
                music.openFromFile(music_folder_name + '/' + *music_iterator);
                music.play();
                music_iterator++;
            }
            else
                music_iterator = music_names.begin();
        }
        

        int w, h;
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
      
        int offtop_x = w / 2 - tall;
        int offtop_y = h * 3 / 4 - tall;

        for(int y = 0; y < offtop_y; y++)
        {
            snow(w);
            std::cout << std::endl;
        }
        snow(tall + offtop_x + 1);
        std::cout << termcolor::bright_yellow << '*' << termcolor::reset << std::endl;
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
                        std::cout << termcolor::on_yellow ;
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
            std::cout << std::endl;
        }

        std::cout << termcolor::underline;
        snow(offtop_x);
        for(int x = 0; x < tall + 1; x++)
            std::cout << ' ';
        std::cout << termcolor::reset << termcolor::color<101, 67, 33> << '#' << termcolor::reset << termcolor::underline;
        for(int x = 0; x < tall + 1; x++)
            std::cout << ' ';
        snow(offtop_x - 3);

        std::cout << std::endl << termcolor::reset << music_iterator->substr(0, music_iterator->find_last_of('.')) << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}