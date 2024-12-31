#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
class shutdown{
public:
    static void rewriteInventory(const std::unordered_map<std::string, std::string>& inventory);

};




#endif