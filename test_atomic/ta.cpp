#include "ta.h"
#include <iostream>

//Ta::Ta()
//{
//	av = new int(0);
//}
//
//Ta::~Ta()
//{
//	if (av)
//		delete av;
//}
//Ta::Ta(const Ta& oth)
//{
//	if (oth.av)
//	{
//		av = new int(*(oth.av));
//	}
//}
//Ta::Ta(Ta && oth)
//{
//	if (oth.av)
//	{
//		av = oth.av;
//		oth.av = nullptr;
//	}
//}

//Ta& Ta::operator=(const Ta &oth)
//{
//	if (oth.av)
//	{
//		if (av)
//			delete av;
//		av = new int(*(oth.av));
//	}
//	else {
//		if (av)
//		{
//			delete av;
//			av = nullptr;
//		}
//	}
//	return *this;
//}

//Ta& Ta::operator=(Ta &&oth)
//{
//	if (oth.av)
//	{
//		if (av)
//			delete av;
//		av = oth.av;
//		oth.av = nullptr;
//	}
//	else {
//		if (av)
//		{
//			delete av;
//			av = nullptr;
//		}
//	}
//	return *this;
//}

std::ostream& operator<<(std::ostream& out, Ta& t)
{
	out << "av = " << t.av;
	return out;
}

int& Ta::operator*()
{
	return this->av;
}

Ta& Ta::operator+(Ta & ta)
{
	av += ta.av;
	return *this;
}

