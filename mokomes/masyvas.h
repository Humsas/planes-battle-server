#include<string>
using namespace std;


enum GAME_CONSOLE_ERROR_NUM{
						GAME_CONSOLE_OK = 1,
						GAME_CONSOLE_ERROR = 2,
						GAME_CONSOLE_WARNING = 3
						} ;

struct console_content {
						string value;
						GAME_CONSOLE_ERROR_NUM num;
};

struct elementas {  //string index;
					console_content cont;
					elementas *next;
					elementas *prev;
};


class Masyvas
{
private:
	// kintamieji
	int skaitiklis;
	elementas *pointer;
	elementas *MASYVAS;
	elementas *END;

	// funkcijos
	//elementas *find(string index);

public:
	
	

	Masyvas() 
	{
		MASYVAS = NULL;
		END = NULL;
		pointer = NULL;
		skaitiklis = 0;
	}

	~Masyvas()
	{

		for(elementas *start = MASYVAS; start != NULL; start = start->next)
		{
			delete start;
		}
		//delete masyvas;
	}

	void add(string value);
	void add(string value, GAME_CONSOLE_ERROR_NUM num);
	void setPointer(int point);
	//bool remove(string index);
	//string get(string index);

	bool get(console_content &c_cont);
	
	int count();
	void var_dump();
	

};