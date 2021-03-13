#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

//Constructor
// best,worst,average=Theta(n), fiindca trebuie parcursa toata lista next pentru a o initializa
SortedMultiMap::SortedMultiMap(Relation r) {
	//TODO - Implementation
	capacity = 30;
	count = 0;
	elements = new TElem[capacity];
	next = new int[capacity];
	head = -1;
	for (int i = 0; i < capacity-1; i++)
		next[i] = i+1;
	next[capacity-1] = -1; //ultimul elem din lista
	firstEmpty = 0;
	relatie = r;
}

//Daca firstEmpty=-1, inseamna ca lista e plina si trebuie sa fac resize: fac 2 array-uri noi de capacitate dubla, iar in array-ul de pozitii copiez pana la jumatate 
//pozitiile din array-ul anterior si de la jumatate initializez pozitiile goale
//Altfel, setez cele necesare pentru adaugare si caut pozitia unde trebuie adaugat elementul, tinand cont de relatia data
//best: Theta(1)(cand lista e goala), worst:O(n), average: O(n)
void SortedMultiMap::add(TKey c, TValue v) {
	//TODO - Implementation
	if (firstEmpty == -1) //lista e plina, deci trebuie sa fac resize
	{
		TElem* newElements = new TElem[capacity * 2];
		int* newPositions = new int[capacity * 2];
		for (int i = 0; i < capacity; i++)
		{
			newElements[i] = elements[i];
			newPositions[i] = next[i];
		}
		for (int i = capacity ; i < (capacity * 2)-1; i++)
			newPositions[i] = i + 1;
		newPositions[capacity * 2-1] = -1;//ultimul element din sirul positions=-1

		delete[] elements;
		delete[] next;
		elements = newElements;
		next = newPositions;
		firstEmpty = capacity + 1;
		capacity *= 2;
	}
	else
	{			
		TElem elem = pair <TKey,TValue>(c,v); //perechea care trebuie inserata
		int pos=firstEmpty;
		firstEmpty = next[firstEmpty];
		elements[pos] = elem;
		next[pos] = -1;
		if (head == -1)//lista goala
		{
			head = pos;
			count++;
		}
		else
		{
			int current = head;
			int prev = 0;
			while (current!= -1 && (relatie(elements[current].first,c)))  {
					prev = current;
					current = next[current];
			}
			if (prev != -1)
			{
				next[pos] = next[prev];
				next[prev] =pos;
				count++;
			}
		}
	}
}

//Caut dupa o cheie si salvez toate valorile acesteia intr-un vector pe care il returnez
//best,worst,average: Theta(n), fiindca trebuie sa parcurg toata lista, nu ma opresc la prima pereche gasita
vector<TValue> SortedMultiMap::search(TKey c) const {
	//TODO - Implementation
	vector<TValue> values;
	int current = head;
	while (current != -1 ) {
		if (elements[current].first == c)
		{
			values.push_back(elements[current].second);
		}
		current = next[current];
	}
	return values;
}

//Elimin o pereche si returnez true, daca era in lista, altfel false
//best: Theta(1)- cand il gasesc pe prima pozitie, worst: O(n)- daca e pe ultima pozitie, average: O(n)
bool SortedMultiMap::remove(TKey c, TValue v) {
	//TODO - Implementation
	int nodC = head;
	int prev = 0;
	while (nodC != -1 && elements[nodC].first != c)
	{
		prev = nodC;
		nodC = next[nodC];
	}
	if (elements[nodC].first == c && elements[nodC].second != v)
	{
		prev = nodC;
		nodC = next[nodC];
	}
	if (nodC != -1 && elements[nodC].first == c && elements[nodC].second == v)
	{
		if (nodC == head) //elimin primul elem
			head = next[head];
		else
			next[prev] = next[nodC]; //sar peste un elem
		//adaug pozitia elementului sters la lista de pozitii goale
		next[nodC] = firstEmpty;
		firstEmpty = nodC;
		count--;
		return true;
	}
	return false;
}

//in clasa SortedMultiMap am declarat un count pe care il cresc cand adaug o pereche si il scad cand sterg una
//best,worst,average: Theta(1)
int SortedMultiMap::size() const {
	//TODO - Implementation
	return count;
}

//best,worst,average: Theta(1)
bool SortedMultiMap::isEmpty() const {
	//TODO - Implementation
	if (count == 0)
		return true;
	else
		return false;
}

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

//Destructor
//best,worst,average: Theta(1)
SortedMultiMap::~SortedMultiMap() {
	//TODO - Implementation
	delete[] elements;
	delete[] next;
}


//Functia suplimentara
//Ca si parametri voi da 2 obiecte de tipul SortedMultiMap, voi face intersectia celor 2 si voi returna un container nou
//Voi parcurge cu current1 si current2 cele 2 containere
//Complexitate: Theta (n^2) in toate cazurile, fiindca trebuie sa parcurg cele doua containere pana la final
SortedMultiMap SortedMultiMap::durchschnitt(SortedMultiMap C1,SortedMultiMap C2)
{
	SortedMultiMap CNou = SortedMultiMap(relatie);
	int current = CNou.head;
	int current1 = C1.head;
	int current2 = C2.head;

	while (current1 != -1)
	{
		//parcurg prima data primul slla, si compar fiecare element cu toate celelalte din al doilea slla
		TElem elem1 = pair <TKey, TValue>(C1.elements[current1].first, C1.elements[current1].second);
		TElem elem2 = pair <TKey, TValue>(C2.elements[current2].first, C2.elements[current2].second);

		int pos = CNou.firstEmpty;
		CNou.firstEmpty = next[CNou.firstEmpty];
		if (CNou.head == -1) {
			head = pos;
			CNou.count++;
		}

		while (current2 != -1 && elem1!=elem2)
		{
			if (elem1 == elem2 && relatie(CNou.elements[current].first, elements[current1].first)) //daca am gasit 2 perechi egale si cheile respecta relatia, adaug perechea in containerul nou
				CNou.add(elements[current1].first, elements[current1].second);
			current2 = next[current2];
		}
		current1 = next[current1]; //trec la urmatorul element din primul slla
	}

	return CNou;
}