#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include<iostream>


template <class T>
class MyLinkedList
{
private: 
	struct List
	{
		T		*t;
		List	*next;
		List	*prev;

		bool	deleteOnDestruction;
		List()
		{
			t		= NULL;
			next	= NULL;
			prev	= NULL;
			deleteOnDestruction = false;
			//std::cout << "CREATEstruct\n"; 
		}

		~List()
		{
			if(t != NULL && deleteOnDestruction)
			{
				delete t;
				//std::cout << "DELETEonDestruction\n";
			}
		}
	};


	List *LIST;	
	int counter;

	List *it; // iterator

	//T *test;

	List *getElementPosition(T *t)
	{
		List *ll = NULL;

		for(List *l = LIST; l != NULL; l = l->next)
		{

			if(l->t == t)
			{
				ll = l;
			}
		}
		return ll;
	}


public:
	// Konstruktorius
	MyLinkedList()
	{ 
		//std::cout << "CREATE\n";  
		LIST	= NULL;
		it		= NULL;
		counter = 0;
	}

	// Destruktorius
	~MyLinkedList()
	{ 
		//std::cout << "DELETE\n"; 
		//delete LIST;
		List *ll;
		for(List *l = LIST; l != NULL; )
		{
			ll = l;
			l = l->next;
			delete ll;
		}
	}

	int count()
	{
		return counter;
	}

	void add(T *t, bool deleteOnDestruction = false)
	{
		List *L = new List;
		L->t	= t;
		L->next = NULL;
		L->deleteOnDestruction = deleteOnDestruction;

		if(LIST == NULL)
		{ // pirmas elementas
			L->prev = NULL;
			LIST = L;
			it	 = L;
			
		}
		else
		{ // jungiam prie pabaigos
			
			//ieskom galinio elemento
			for(List *l = LIST; l != NULL; l = l->next)
			{

				if(l->next == NULL)
				{
					L->prev = l;
					l->next = L;
					break;
				}
			}
		}
		counter++;
	}

	T *get(int index)
	{
		int i = 0;
		for(List *l = LIST; l != NULL; l = l->next)
		{
			if(i == index)
			{
				return l->t;
			}
			i++;
		}
		return NULL;
	}

	void printR()
	{
		//std::cout << LIST << endl;
		for(List *l = LIST; l != NULL; l = l->next)
		{
			std::cout << l << " " << l->prev << " " << l->next << endl;
			
		}
	}


	bool remove(T *t)
	{
		bool r = false;
		List *l = getElementPosition(t);
		if(l != NULL)
		{
			// vidurinis elementas
			if(l->prev != NULL && l->next != NULL)
			{
				l->prev->next = l->next;
				l->next->prev = l->prev;
				it = l->prev;
			}
			else
			{
				// pirmas arba galinis
				if(l->prev == NULL)
				{ // pirmas
					LIST = l->next;
					it = LIST;
					if(LIST != NULL)
					{
						LIST->prev = NULL;
					}
				}
				else
				{ // paskutinis
					l->prev->next = NULL;
					it = l->prev;
				}
			}

			delete l;
			counter--;
			r = true;
		}
		return r;
	}


	void iteratorReset()
	{
		it = LIST;
	}

	T *getNext()
	{
		T *r = NULL;
			
		if(it != NULL)
		{
			r = it->t;
			it = it->next;
		}
		else
		{
			r = NULL;
		}

		return r;
	}

	List *cut(T *t)
	{
		List *r = NULL;

		if(t != NULL)
		{
			List *l = getElementPosition(t);
			if(l != NULL)
			{
				// vidurinis elementas
				if(l->prev != NULL && l->next != NULL)
				{
					l->prev->next = l->next;
					l->next->prev = l->prev;
				}
				else
				{
					// pirmas arba galinis
					if(l->prev == NULL)
					{ // pirmas
						LIST = l->next;
						if(LIST != NULL)
						{
							LIST->prev = NULL;
						}
					}
					else
					{ // paskutinis
						l->prev->next = NULL;
					}
				}

				if(it == l) { it = NULL; }
				
				counter--;
				r = l;
			}


		}

		return r;
	}

	void paste(List *listElement)
	{
		if(listElement != NULL)
		{
			listElement->next = NULL;

			if(LIST == NULL)
			{ // pirmas elementas
				listElement->prev = NULL;
				LIST = listElement;
				it	 = listElement;
			
			}
			else
			{ // jungiam prie pabaigos
			
				//ieskom galinio elemento
				for(List *l = LIST; l != NULL; l = l->next)
				{

					if(l->next == NULL)
					{
						listElement->prev = l;
						l->next = listElement;
						break;
					}
				}
			}
			counter++;
		}
	}

	void iteratorNext()
	{
	}



};

//template <class T>
//OneWayLinkedList::OneWayLinkedList()
//{
//	std::cout << "CREATE\n";
//}
//
//template <class T>
//OneWayLinkedList::~OneWayLinkedList()
//{
//	std::cout << "DELETE\n";
//}
//#include "oneWayLinkedList.cpp"



#endif