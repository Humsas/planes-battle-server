#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>
#include <omp.h>

#include "masyvas.h"

using namespace std;




class Console
{
private:
	Masyvas *contents;

public:
	Console() { contents = new Masyvas(); }
	//~Console() { if(contents != NULL) { contents->~Masyvas(); } }

	void add(string cont);
	void add(string cont, GAME_CONSOLE_ERROR_NUM num);
	void render(LPDIRECT3DDEVICE9 &d3);

};