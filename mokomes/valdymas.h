#include <string>
using namespace std;

struct SINPUTMAP
{
 bool shift;

 bool w;
 bool s;
 bool a;
 bool d;
};


typedef SINPUTMAP INPUTMAP;






class Valdymas
{
//private:
	
	
	

public:
	SINPUTMAP keymap;
	bool m_update;
	long mouseX, mouseY;
	int ratukas;
	void mouseMove(long x, long y);
	void mouseRatas(int r);

	Valdymas() { ratukas = 10; }

	//void setKey(char x, bool busena); 


};