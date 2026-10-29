#pragma once

#include <fstream>
#include "FilePAL.h"

namespace PlayFab
{

class FileStreamContainer : public FileContainer
{
public:
    std::fstream file;
    // The failure that put the stream into the bad state. A bad stream does no further I/O, so the
    // platform error is gone by the next write; this keeps reporting the real cause.
    HRESULT lastFailure{ S_OK };
};

};
