#pragma once

#include <iostream>
#include "TestFunc.hpp"
#include <stack>
#include <ListNode.h>

namespace test3{

	using std::cout;
	using std::endl;

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

	ListNode* swapPairs(ListNode* head) {
		if (!head) return nullptr;
		ListNode* res = nullptr;
		if (head->next)
			res = head->next;
		else
			return head;
		ListNode* l = nullptr;
		ListNode* n = nullptr;
		while (head)
		{
			if (l)
			{
				n = head->next;
				head->next = l;
				if (n == nullptr)
				{
					l->next = nullptr;
					return res;
				}
				if (n->next == nullptr)
				{
					l->next = n;
					return res;
				}
				l->next = n->next;
				l = nullptr;
				head = n;
			}
			else {
				l = head;
				head = head->next;
			}
		}
		return res;
	}

	void test_swapPairs()
	{
		ListNode *in = ListNode::from_num(4321);
		ListNode *res = swapPairs(in);
		dbg(res);
		delete res;
	}

	ListNode* reverseKGroup_reverse(ListNode* head,ListNode* foot)
    {
		if (!head || !foot)
			return nullptr;
		if (head == foot)
			return head;
		ListNode* t = nullptr;
		while (head)
		{
			if (t)
			{
				auto temp = head->next;
				head->next = t;
				if (head == foot)
					break;
				t = head;
				head = temp;
			}
			else {
				t = head;
				head = head->next;
			}
		}
        return head;
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
		if (k == 0 || k == 1)
			return head;
		if (k == 2)
			return swapPairs(head);
		ListNode* res = nullptr;
		ListNode* h = head;
		ListNode* l = nullptr;
		ListNode* f = nullptr;
		while (h)
		{
			f = h;
			for (uint32_t i = 0; i < k - 1; ++i)
			{
				if (f)
					f = f->next;
				else
				{
					if (!res) { return head; }
					else {
						l->next = h;
						return res; 
					}
				}
			}
			if (!f)
			{
				if (!res) { return head; }
				else {
					l->next = h;
					return res;
				}
			}
			auto n = f->next;
			auto l_h = h;
			auto temp = reverseKGroup_reverse(h, f);
			h = n;
			if (!res) res = temp;
			if (l)
			{
				l->next = temp;
				l = l_h;
			}
			else {
				l = l_h;
			}
		}
		if(l)
			l->next = nullptr;
        return res;
    }

    void test_reverseKGroup()
    {
	    auto in = ListNode::from_num(987654321);
		ListNode* res = reverseKGroup(in,3);
	    dbg(res);
	    delete res;
    }

	auto init()
	{
		return  wws::CreateTFArray(
			CREATE_TEST_FUNC(test_removeNthFromEnd),
			CREATE_TEST_FUNC(isValid),
			CREATE_TEST_FUNC(test_mergeTwoLists),
			CREATE_TEST_FUNC(generateParenthesis),
			CREATE_TEST_FUNC(test_mergeKLists),
			CREATE_TEST_FUNC(test_swapPairs),
			CREATE_TEST_FUNC(test_reverseKGroup)
		);
	}
}// test3