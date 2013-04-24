#include "valdymas.h"


void Valdymas::mouseMove(long x, long y)
{
this-> mouseX = x;
this-> mouseY = y;
this-> m_update = true;
}

void Valdymas::mouseRatas(int r)
{
this-> ratukas += r;
if(this->ratukas < 1) ratukas = 1;
this-> m_update = true;
}


