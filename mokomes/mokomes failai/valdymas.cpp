#include "valdymas.h"


void Valdymas::mouseMove(long x, long y)
{
this-> mouseX = x;
this-> mouseY = y;
this-> m_update = true;
}

