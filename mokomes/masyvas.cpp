#include "masyvas.h"
#include <iostream>



// pildo grandine reiksmemis
void Masyvas::add(string value)
{
	// tikrinam ar masyvas is viso egzistuoja
	if(MASYVAS == NULL)
	{	// masyvas neegzistuoja
		// reiskia jis neturi celiu
		// jei neturi celiu reiskia kursim nauja cele ir i ja desim turini

		elementas *cell = new elementas;
		//cell->index = index;
		cell->cont.value = value;
		cell->cont.num	= GAME_CONSOLE_OK;
		cell->next  = NULL;
		cell->prev  = NULL;

		// prijungiam nauja cele prie grandineles galo
		MASYVAS = cell;
		END = MASYVAS;
		skaitiklis++;

	}
	else
	{
	

		elementas *cell = new elementas;
		
		cell->cont.value = value;
		cell->cont.num	= GAME_CONSOLE_OK;
		cell->next  = NULL;
		cell->prev  = END;


		
		// prijungiam nauja cele prie grandineles galo
		END->next = cell;
		END = cell;
		skaitiklis++;
	
		// GAME_CONSOLE_ERROR_NUM lolo;
		//lolo = GAME_CONSOLE_OK;
	}

}


// pildo grandine reiksmemis
void Masyvas::add(string value, GAME_CONSOLE_ERROR_NUM num)
{
	// tikrinam ar masyvas is viso egzistuoja
	if(MASYVAS == NULL)
	{	// masyvas neegzistuoja
		// reiskia jis neturi celiu
		// jei neturi celiu reiskia kursim nauja cele ir i ja desim turini

		elementas *cell = new elementas;
		//cell->index = index;
		cell->cont.value = value;
		cell->cont.num	= num;
		cell->next  = NULL;
		cell->prev  = NULL;

		// prijungiam nauja cele prie grandineles galo
		MASYVAS = cell;
		END = MASYVAS;
		skaitiklis++;

	}
	else
	{
	

		elementas *cell = new elementas;
		
		cell->cont.value = value;
		cell->cont.num	= num;
		cell->next  = NULL;
		cell->prev  = END;


		
		// prijungiam nauja cele prie grandineles galo
		END->next = cell;
		END = cell;
		skaitiklis++;
	
	}

}




// grazina grandines ilgi
int Masyvas::count()
{
	return skaitiklis;
}


// isveda grandines adresu ir turinio informacija
void Masyvas::var_dump()
{
	for(elementas *start = MASYVAS; start != NULL; start = start->next)
		{
			cout << start << "  prev(" << start->prev << ")  next(" << start->next << ")  | [" 
				 << "INDEXSAS" << "] = " << start->cont.value << endl;;		
		}
}



void Masyvas::setPointer(int point)
{
	if(point > skaitiklis) point = skaitiklis;
	if(abs(point) > skaitiklis) point = 0;

	int tmpPoint = 0;
	
	if(point >= 0)
		tmpPoint = point;
	else
		tmpPoint = skaitiklis + point;

	if(skaitiklis == 0)
	{
		pointer = MASYVAS;
	}
	else
	{
		if(skaitiklis - tmpPoint > tmpPoint)
		{
			// -->
			int tmp = 0;
			for(elementas *start = MASYVAS; start != NULL; start = start->next)
			{
				if(tmp == tmpPoint)
				{
					pointer = start;
					break;
				}
				tmp++;
			}


		}
		else
		{
			// <--
			int tmp = skaitiklis;
			for(elementas *start = END; start != NULL; start = start->prev)
			{
				if(tmp == tmpPoint)
				{
					pointer = start;
					break;
				}
				tmp--;
			}

		}



	}


	//cout << endl << pointer;

}


/*
// grazina celes reiksme
string Masyvas::get(string index)
{
	string ret_val = "";

	elementas *cele = find(index);
	if(cele->index == index)
		ret_val = cele->value;

	return ret_val;
}
*/



bool Masyvas::get(console_content &c_cont)
{
	if(pointer == NULL)
	{
		//c_cont = "";
		return false;
	}
	else
	{
		c_cont = pointer->cont;
		pointer = pointer->next;
		return true;
	}
	

}