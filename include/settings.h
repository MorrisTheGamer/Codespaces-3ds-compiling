#ifndef SETTINGS_H
#define SETTINGS_H
#include <3ds.h>
#include <sys/stat.h>
#include <fstream>
#include <string>

class Settings {
public:
    static void initFileSystem() {
        mkdir("sdmc:/3ds", 0777);
        mkdir("sdmc:/3ds/MiiCord", 0777);
    }

    static std::string getUsername() {
        std::ifstream file("sdmc:/3ds/MiiCord/user.cfg");
        std::string name;
        if (file >> name) return name;
        return "Guest";
    }

    static void saveUsername(std::string name) {
        std::ofstream file("sdmc:/3ds/MiiCord/user.cfg");
        file << name;
    }
};
#endif