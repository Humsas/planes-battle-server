#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include<iostream>


template <class T>
class MyLinkedList
{
private: 
	struct Node
	{
		T		*t;
		Node	*next;
		Node	*prev;
		bool	deleteOnDestruction;

		Node()
		{
			t		= NULL;
			next	= NULL;
			prev	= NULL;
			deleteOnDestruction = false;
		}

		~Node()
		{
			if(t != NULL && deleteOnDestruction)
			{
				delete t;
			}
		}
	};


	Node *LIST;	
	int counter;

	Node *it; // iterator


	Node *renderPointer;
	Node *updatePointer;
	Node *networkReadPointer;
	Node *readAllListsPointer;


	Node *getElementPosition(T *t)
	{
		Node *ll = NULL;

		for(Node *l = LIST; l != NULL; l = l->next)
		{

			if(l->t == t)
			{
				ll = l;
			}
		}
		return ll;
	}

	T *getNextOf(Node **l)
	{
		T *r = NULL;

		if(*l != NULL)
		{
			r = (*l)->t;
			*l = (*l)->next;
		}
		else
		{
			r = NULL;
		}

		return r;
	}


public:
	MyLinkedList()
	{ 
		LIST	= NULL;
		it		= NULL;
		updatePointer = NULL;
		renderPointer = NULL;
		networkReadPointer = NULL;
		readAllListsPointer = NULL;
		counter = 0;
	}

	~MyLinkedList()
	{ 
		Node *ll;
		for(Node *l = LIST; l != NULL; )
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
		Node *L = new Node;
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
			for(Node *l = LIST; l != NULL; l = l->next)
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
		for(Node *l = LIST; l != NULL; l = l->next)
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
		for(Node *l = LIST; l != NULL; l = l->next)
		{
			std::cout << l << " " << l->prev << " " << l->next << endl;
			
		}
	}


	bool remove(T *t)
	{
		bool r = false;
		Node *l = getElementPosition(t);
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

	void renderIteratorReset()
	{
		renderPointer = LIST;
	}

	void updateIteratorReset()
	{
		updatePointer = LIST;
	}

	void networkReadIteratorReset()
	{
		networkReadPointer = LIST;
	}
	void readAllListsIteratorReset()
	{
		readAllListsPointer = LIST;
	}

	T *getNextUpdate()
	{
		return getNextOf(&updatePointer);
	}

	T *getNextRender()
	{
		return getNextOf(&renderPointer);
	}

	T *getNextNetwork()
	{
		return getNextOf(&networkReadPointer);
	}

	T *getNextReadAllLists()
	{
		return getNextOf(&readAllListsPointer);
	}

	T *getNext()
	{
		return getNextOf(&it);
	}

	Node *cut(T *t)
	{
		Node *r = NULL;

		if(t != NULL)
		{
			Node *l = getElementPosition(t);
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

	void paste(Node *listElement)
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
				for(Node *l = LIST; l != NULL; l = l->next)
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
};

#endif