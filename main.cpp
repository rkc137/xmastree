#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <unistd.h>
#include <termcolor/termcolor.hpp>
#include <SFML/Audio.hpp>

namespace fs = std::filesystem;

void snow(int s)
{
    for(int x = 0; x < s; x++)
    {
        if(rand() % 30 == 1)
            std::cout << '.';
        else
            std::cout << ' ';
    }
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
    if(argc > 2)
    {
        std::cout << "usage: xmstree [tall]\n";
        return 0;
    }
    int tall = (argc == 1) ? 10 : atoi(argv[1]);

    std::vector<std::string> music_path;
    for(auto & p: fs::recursive_directory_iterator("music"))
    {
        if(fs::is_regular_file(p))
        {
            std::string ext = p.path().extension().string();
            if(ext == ".ogg" || ext == ".wav")
                music_path.push_back(p.path().string());
        }
    }
    std::sort(music_path.begin(), music_path.end());
      
    sf::Music music;
    std::string music_name;
    int music_iterator = 0;
    for(int i = 0; i < music_path.size(); i++)
        std::cout << i + 1 << '\t' << music_path[i] << std::endl;
    
    std::cout << "\nctrl + c to exit\n";
    sleep(4);
    system("clear");
    for(int frame = 0; true; frame++)
    {
        if(music.getStatus() != sf::Music::Status::Playing)
        {
            if(music_iterator < music_path.size())
            {
                music.openFromFile(music_path[music_iterator]);
                music.play();
                music_name = "";
                if(!music_path[music_iterator].empty())
                    for(int i = 6; i < music_path[music_iterator].size() - 4; i++)
                        music_name += music_path[music_iterator][i];
                music_iterator++;
            }
            else
                music_iterator = 0;
        }

        int w = WEXITSTATUS(std::system("exit `tput cols`"));
        int h = WEXITSTATUS(std::system("exit `tput lines`"));
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

        std::cout << std::endl << termcolor::reset << music_name << std::endl;

        sleep(2);
        system("clear");
    }

    return 0;
}