#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <filesystem>

#include <SFML/Audio.hpp>

#include "colorset.hpp"

namespace fs = std::filesystem;

std::string snow(int s)
{
    std::string line(s, ' ');
    for(char &c : line)
        if(rand() % 30 == 0)
            c = '.';
    return line;
}

std::string trees_toys(int x, int y, int frame)
{
    std::string toy;
    if(x % 4 == 0)
    {
        if(y % 2 == frame % 2)
            toy += clr::on_red;
        else
            toy += clr::on_cyan;
        toy += '&' + clr::white;
    }
    else
        toy += clr::green + '*' + clr::white;
    return toy;
}

int main(int argc, char *argv[])
{
    int tall = 10;
    bool is_ingore_mp3 = false;
    bool is_ingore_other_files = false;
    std::string music_folder_name = "music";
    for(int i = 1; i < argc; i++)
    {
        if(!is_ingore_mp3) is_ingore_mp3 = !strcmp(argv[i], "-ignmp3");
        if(!is_ingore_other_files) is_ingore_other_files = !strcmp(argv[i], "-ign");
        
        if(i + 1 != argc)
        {
            if(!strcmp(argv[i], "-f"))              
                music_folder_name = argv[i + 1];
            if(!strcmp(argv[i], "-t"))
                tall = atoi(argv[i + 1]);
            i++;
            continue;
        }
    }
    
    if(music_folder_name.back() == '/')
        music_folder_name.pop_back();
    
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
    std::cout << "\nwere found in " << music_folder_name << "\nctrl + c to exit\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));


    int w, h;
    int offtop_x, offtop_y;
    
    sf::Music music;
    auto music_iterator = music_names.end();
    std::string output;
    for(int frame = 0; true; frame++)
    {
        if(music.getStatus() != sf::Music::Status::Playing)
        {
            if(music_iterator != music_names.end())
                music_iterator++;
            else
                music_iterator = music_names.begin();

            music.openFromFile(music_folder_name + '/' + *music_iterator);
            music.play();
        }

        if(frame % 10 == 0)
            std::system("clear");
        
        w = WEXITSTATUS(std::system("exit `tput cols`"));
        h = WEXITSTATUS(std::system("exit `tput lines`")) + 7;
      
        offtop_x = w / 2 - tall;
        offtop_y = h * 3 / 4 - tall;

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
                        output += *clr::toys_clrs[rand() % clr::toys_clrs.size()] + '&' + clr::white;
                }
            }
            else if(frame % 20 > 10)
                for(int x = y * 2 + 1; x > 0; x--)
                    output += trees_toys(x, y, frame);
            else
                for(int x = 0; x < y * 2 + 1; x++)
                    output += trees_toys(x, y, frame);
            output += clr::white + '\\' + snow(tall - y + offtop_x - 3) + '\n';
        }

        output += clr::underline + snow(offtop_x);
        std::string under_tree(tall + 1, ' ');
        output += under_tree;
        output += clr::white + clr::underline + clr::brown + '#' + clr::white + clr::underline;
        output += under_tree + snow(offtop_x - 3);

        int file_name_start = music_iterator->find_last_of('/') + 1;
        output += '\n' + clr::white 
                + music_iterator->substr(file_name_start, music_iterator->find_last_of('.') - file_name_start)
                + '\n';
        std::cout << output;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}