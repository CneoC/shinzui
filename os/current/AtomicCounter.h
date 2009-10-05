#ifdef _WIN32 // TODO: Check for compiler as well
#include "os/win/AtomicCounter.h"
#else
#pragma error "Only windows time supported right now."
#endif
