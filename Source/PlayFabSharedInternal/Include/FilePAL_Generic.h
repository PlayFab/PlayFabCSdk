#pragma once

#include <fstream>
#include "FilePAL.h"

namespace PlayFab
{

class FileStreamContainer : public FileContainer
{
public:
    std::fstream file;
};

};
