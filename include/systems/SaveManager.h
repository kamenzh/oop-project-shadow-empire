#ifndef SHADOW_EMPIRE_SAVE_MANAGER_H
#define SHADOW_EMPIRE_SAVE_MANAGER_H

#include <string>

class Organization;

class SaveManager {
private:
    std::string filePath;

public:
    SaveManager();
    explicit SaveManager(const std::string& filePath);

    bool save(const Organization& org) const;
    bool load(Organization& org) const;
};

#endif
