#ifndef BLINK_DEF
#define BLINK_DEF

#include "../standard.h"
#include "../Entity.h"
#include "Animation.h"

class Blink : public Animation {
public:
    Blink();
private:
    float interval;
}

#endif


