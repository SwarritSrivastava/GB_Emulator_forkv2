#pragma once

#include "common.hpp"

class ProcessingUnit;
class MMU;

using OpcodeFunc = int (*)(ProcessingUnit&, MMU&);

extern OpcodeFunc instructionTable[256];