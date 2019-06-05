#pragma once
#include <iostream>
namespace test3 {
	
	struct ListNode {
		int val;
		ListNode *next;
		ListNode(int x);
		~ListNode() noexcept(false);
		static ListNode* from_num(int);

		friend std::ostream& operator<<(std::ostream&, ListNode*);
	};
}