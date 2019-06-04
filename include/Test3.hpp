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

	ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
		if(!l1 && !l2) return nullptr;
		ListNode* res = new ListNode(0);
		ListNode* curr = res;
		for (;;)
		{
			if (!l1 && !l2) break;
			int v = 0;
			if (l1 && !l2)
			{
				v = l1->val;
				l1 = l1->next;
				goto Go;
			}
			if(l2 && !l1)
			{
				v = l2->val;	
				l2 = l2->next;
				goto Go;
			}
			if (l1->val <= l2->val)
			{
				v = l1->val; l1 = l1->next;
			}
			else {
				v = l2->val; l2 = l2->next;
			}
			Go:
			curr->val = v;
			if (l1 || l2)
			{
				curr->next = new ListNode(0);
			}
			curr = curr->next;
		}
		return res;
	}

	void test_mergeTwoLists()
	{
		ListNode* in1 = ListNode::from_num(421);
		ListNode* in2 = ListNode::from_num(431);
		dbg(in1);
		dbg(in2);
		auto res = mergeTwoLists(in1, in2);
		dbg(res);
		delete in1;
		delete in2;
		delete res;
	}


    std::vector<std::string> generateParenthesis(int n) {
        if(n == 0)
            return {""};
        std::vector<std::string> res;
        std::stack<std::tuple<std::string,int,int>> st;
        st.push(std::make_tuple("",0,0));
        for(;;)
        {
            if(st.empty()) break;
            auto&& curr = std::move(st.top());
            std::string str = std::move(std::get<0>(curr));
            if(str.size() == n * 2)
            {
                res.push_back(std::move(str));
                continue;
            }
            int b = std::get<1>(curr);
            int e = std::get<2>(curr);
            st.pop();
            if(b < n)
                st.push(std::make_tuple(str + '(',b + 1,e));
            if(e < b)
                st.push(std::make_tuple(str + ')',b,e + 1 ));
        }
        return res;
    }

	ListNode* mergeKLists(std::vector<ListNode*>& lists) {
		for (auto ln : lists)
		{
			if (ln)
				goto BEG;
		}
		return nullptr;
	BEG:
		ListNode* res = new ListNode(0);
		ListNode* curr = res;
		for (;;)
		{
			int v = std::numeric_limits<int>::max();
			ListNode **choose = nullptr;
			for (auto& ln : lists)
			{
				if (ln)
				{
					if (ln->val < v)
					{
						v = ln->val;
						choose = &ln;
					}
				}
			}
			if (!choose)
				break;
			(*choose) = (*choose)->next;
			curr->val = v;
			bool is_find = false;
			for (auto ln : lists)
			{
				if (ln) { is_find = true; break; }
			}
			if (is_find)
				curr->next = new ListNode(0);
			curr = curr->next;
		}
		return res;
	}

    ListNode* mergeKLists2_unit(std::vector<ListNode*>& lists,int b,int e) {
        if(b == e)
            return lists[b];
        if(e - b == 1)
            return mergeTwoLists(lists[b],lists[e]);
        int mid = (b + e) / 2;
        ListNode* l1 = mergeKLists2_unit(lists,b,mid);
        ListNode* l2 = mergeKLists2_unit(lists,mid+1,e);

        ListNode* res = mergeTwoLists(l1,l2);
		if(l1 != lists[b] && l1 != lists[mid])
			delete l1;
		if(l2 != lists[mid + 1] && l2 != lists[e])
			delete l2;
        return res;
	}

    ListNode* mergeKLists2(std::vector<ListNode*>& lists) {
        if(lists.empty())
            return nullptr;
	    if(lists.size() == 2)
        {
            return mergeTwoLists(lists[0],lists[1]);
        }
	    return mergeKLists2_unit(lists,0,lists.size() - 1);
    }

	void test_mergeKLists()
	{
		std::vector<ListNode*> list;
		list.push_back(ListNode::from_num(541));
		list.push_back(ListNode::from_num(431));
		list.push_back(ListNode::from_num(62));
		auto res = mergeKLists2(list);
		dbg(res);
		delete res;
		for (auto ln : list)
		{
			delete ln;
		}
	}

	auto init()
	{
		return  wws::CreateTFArray(
			CREATE_TEST_FUNC(test_removeNthFromEnd),
			CREATE_TEST_FUNC(isValid),
			CREATE_TEST_FUNC(test_mergeTwoLists),
			CREATE_TEST_FUNC(generateParenthesis),
			CREATE_TEST_FUNC(test_mergeKLists)
		);
	}
}// test3