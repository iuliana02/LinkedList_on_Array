#include "SMMIterator.h"
#include "SortedMultiMap.h"


//Toate functiile iteratorului au complexitate constanta, adica best,worst,average: Theta(1)

//Constructor
//Initializez iteratorul cu head-ul listei
SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	//TODO - Implementation
	iterator = map.head;
}

void SMMIterator::first(){
	//TODO - Implementation
	iterator = map.head;
}

//Daca nu am ajuns la sfarsitul listei, merg cu iteratorul pe lista de next-uri (iteratorul primeste indicele urmatorului element)
void SMMIterator::next(){
	//TODO - Implementation
	if (iterator != -1)
		iterator = map.next[iterator];
	else
	{
		std::exception e;
		throw e;
	}
}

//Daca nu am ajuns la sfarsitul listei( deci sunt pe o pozitie!=-1), returnez true
bool SMMIterator::valid() const{
	//TODO - Implementation
	if (iterator != -1)
		return true;
	return false;
}

//Daca iteratorul e valid, returnez perechea de pe acea pozitie
TElem SMMIterator::getCurrent() const{
	//TODO - Implementation
	if (valid() == true)
		return map.elements[iterator];
	else
	{
		std::exception e;
		throw e;
	}
}


