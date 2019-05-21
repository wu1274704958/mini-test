#pragma once

#include <iostream>
#include "TestFunc.hpp"
#include <stack>

namespace test3{
	using  std::cout;
	using  std::endl;
	struct ListNode {
		int val;
		ListNode *next;
		ListNode(int x) : val(x), next(nullptr) {}
		~ListNode() noexcept(false)
		{
			if (next)
			{
				delete next;
			}
		}
		static ListNode* from_num(int);

		friend std::ostream& operator<<(std::ostream&, ListNode*);
	};

	std::ostream& operator<<(std::ostream& out, ListNode* n)
	{
		if (!n)
		{
			out << "nullptr";
			return out;
		}
		while (n)
		{
			if (n->next) {
				out << n->val << "->";
				n = n->next;
			}else {
				out << n->val;
				break;
			}
		}
		return out;
	}

	ListNode* ListNode::from_num(int a)
	{
		if (a <= 0)
			return nullptr;
		ListNode *p = new ListNode(0);
		ListNode *res = p;
		while (a > 0)
		{
			p->val = a % 10;
			a /= 10;
			if (a > 0) {
				p->next = new ListNode(0);
				p = p->next;
			}
		}
		return res;
	}
	
	ListNode* removeNthFromEnd(ListNode* head, int n) {
		int len = 0;
		ListNode *p = head;
		while (p)
		{
			p = p->next;
			len++;
		}

		if (n == len)
		{
			if (head->next == nullptr)
			{
				delete head;
				return nullptr;
			}
			else {
				auto t = head->next;
				head->next = nullptr;
				delete head;
				return t;
			}
		}
		p = head;
		n = len - n;
		int i = 0;
		while (p)
		{
			if (i == n - 1)
			{
				if (p->next == nullptr)
				{
					return head;
				}
				else if(p->next->next == nullptr){
					delete p->next;
					p->next = nullptr;
					return head;
				}
				else {
					auto t = p->next->next;
					p->next->next = nullptr;
					delete p->next;
					p->next = t;
					return head;
				}
				break;
			}
			p = p->next;
			i++;
		}
		return head;
	}

	void test_removeNthFromEnd() {
		ListNode * res = removeNthFromEnd(ListNode::from_num(54321),2);
		cout << "res = " << res << endl;
		delete res;
		res = removeNthFromEnd(ListNode::from_num(21), 1);
		cout << "res = " << res << endl;
		delete res;
	}

	bool isValid(std::string s) {
		std::stack<char> sta;
		for (char c : s)
		{
			if (c == '(' || c == '[' || c == '{')
			{
				sta.push(c);
			}
			else {
				if (sta.empty()) return false;
				char ls = sta.top();
				sta.pop();
				switch (c)
				{
				case ')':
					if (ls != '(')
						return false;
					break;
				case ']':
					if (ls != '[')
						return false;
					break;
				case '}':
					if (ls != '{')
						return false;
					break;
				default:
					return false;
				}
			}
		}
		if (sta.empty())
			return true;
		else
			return false;
	}


	auto init()
	{
		return  wws::CreateTFArray(
			CREATE_TEST_FUNC(test_removeNthFromEnd),
			CREATE_TEST_FUNC(isValid)
		);
	}
}// test3