#pragma once
#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <vector>
#include <string>

#include "../common/Config.h"

class InputParser
{
public:
    static Config parseFile(const std::string& fileName);

private:
    static void exitWithError(const std::string& line);
    static bool isLineEmpty(std::stringstream& ss);
};

#endif