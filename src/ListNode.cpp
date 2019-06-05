#include <ListNode.h>


namespace test3 {
	using  std::cout;
	using  std::endl;

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
			}
			else {
				out << n->val;
				break;
			}
		}
		return out;
	}
}

test3::ListNode::ListNode(int x) : val(x), next(nullptr) {}

test3::ListNode::~ListNode() noexcept(false)
{
	if (next)
	{
		delete next;
	}
}

test3::ListNode* test3::ListNode::from_num(int a)
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
