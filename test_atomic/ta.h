#pragma once

#include <atomic>

class Ta
{
public:
	Ta() : av(0) {}
	Ta(int n) : av(n) {}
	~Ta() = default;
	Ta(const Ta& oth) = default;
	Ta(Ta &&oth) = default;
	Ta& operator=(Ta &&oth) = default;
	Ta& operator=(const Ta &oth) = default;
	
	int& operator*();

	Ta& operator+(Ta& ta);

	friend std::ostream& operator<<(std::ostream&, Ta& t);
	int av = 0;
};
