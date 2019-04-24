#ifndef __WWS_TEST2_H__
#define __WWS_TEST2_H__

#include <iostream>
#include "TestFunc.hpp"
#include <functional>
#include <any>
#include <vector>
#include <set>

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#include <Commctrl.h>

#undef max
#undef min

#endif // _MSC_VER

namespace test2{
using std::cout;
using std::endl;

template<int N, int... Indexes>
struct IndexTuple {
    using type = typename IndexTuple<N - 1,N - 1,Indexes...>::type;
};// 3 -> 2 2 -> 1 1 2 -> 0 0 1 2 -> 0 1 2       

template<int... indexes>
struct IndexTuple<0, indexes...>
{
    typedef IndexTuple<indexes...> type;
};
template<int I>
struct make_index{
    using type = typename IndexTuple<I>::type;
};

template<int ...Argc>
void print_index_tuple(IndexTuple<Argc ...> it)
{
    ((std::cout << Argc << " "),...); 
}

void test2_1()
{
    using type = make_index<7>::type;
    print_index_tuple(type());
}

int add(int a,int b)
{
    std::cout << a + b <<std::endl;
    return a + b;
}

void test2_2()
{
    int a = 9;
    auto r = std::reference_wrapper<int>(a);
    int& d = r;
    int&& e = std::forward<int>(r);

    d = 98;
    e = 90;

    std::cout << a << std::endl;

    auto f = std::bind(add,a,a);
    f();
}

class Test{
public:
	int *p;
    Test(){
		p = new int(9);
        printf("Test()\n");
    }
	Test(Test&& t) {
		this->p = t.p;
		t.p = nullptr;
		printf("Test(Test&& )\n");
	}
	Test(const Test& t) {
		this->p = t.p;
		printf("Test(Test& )\n");
	}
    ~Test(){
		if (p)
			delete p;
        printf("~Test()\n");
    }
	int operator=(int a)
	{
		*p = a;
		return a;
	}
	 operator int()
	{
		return *p;
	}

};

void f(Test t){
    
}

void test2_3()
{
    try{
		//int *p = reinterpret_cast<int*>(1);
		//*p = 9;
    }catch(...)
    {
        printf("exception!\n");
    }
    f(Test());
}
using std::any;
using std::any_cast;
void test_any()
{
    any a = 1;
    if(a.has_value())
    {
        printf("%s  %d\n",a.type().name(),any_cast<int>(a));
    }

    int *p = any_cast<int>(&a);
    cout << *p << endl;
}


Test&& retRr(Test &t)
{
	return std::move(t);
}

void test_rr()
{
	Test t;
	t = 90;
	Test t2 = retRr(t);
	cout << "t2 = "<< t2 << endl;
}

double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
{
	int max_len = nums1.size() + nums2.size();
	int len = max_len / 2 + 1;
	std::vector<int> arr3;
	arr3.reserve(len);
	int x = 0, y = 0;
	for (int i = 0; i < len; ++i)
	{
		if (x >= nums1.size()) {
			arr3.push_back(nums2[y++]); continue;
		}
		else if (y >= nums2.size()) {
			arr3.push_back(nums1[x++]); continue;
		}
		if (nums1[x] <= nums2[y])
		{
			arr3.push_back(nums1[x++]);
		}
		else {
			arr3.push_back(nums2[y++]);
		}
	}
	if (max_len % 2 != 0)
		return arr3[len - 1];
	else
		return (((double)arr3[len - 1]) + ((double)arr3[len - 2])) / 2.0;
}
double findMedianSortedArrays2(std::vector<int>& nums1, std::vector<int>& nums2)
{
	int m = nums1.size();
	int n = nums2.size();
	if (m > n) { // to ensure m<=n
		std::vector<int> temp = std::move(nums1); nums1 = std::move(nums2); nums2 = std::move(temp);
		int tmp = m; m = n; n = tmp;
	}
	int iMin = 0, iMax = m, halfLen = (m + n + 1) / 2;
	while (iMin <= iMax) {
		int i = (iMin + iMax) / 2;
		int j = halfLen - i;
		if (i < iMax && nums2[j - 1] > nums1[i]) {
			iMin = i + 1; // i is too small
		}
		else if (i > iMin && nums1[i - 1] > nums2[j]) {
			iMax = i - 1; // i is too big
		}
		else { // i is perfect
			int maxLeft = 0;
			if (i == 0) { maxLeft = nums2[j - 1]; }
			else if (j == 0) { maxLeft = nums1[i - 1]; }
			else { maxLeft = std::max(nums1[i - 1], nums2[j - 1]); }
			if ((m + n) % 2 == 1) { return maxLeft; }

			int minRight = 0;
			if (i == m) { minRight = nums2[j]; }
			else if (j == n) { minRight = nums1[i]; }
			else { minRight = std::min(nums2[j], nums1[i]); }

			return (maxLeft + minRight) / 2.0;
		}
	}
	return 0.0;
}

std::tuple<int,int> get_plalindrome_len(std::string& str,int i, int j)
{
	int len = str.size();
	while (i >= 0 && j < len && str[i] == str[j]) {
		--i; ++j;
	}
	++i; --j;
	return std::make_tuple(i ,j - i + 1);
}

std::string longestPalindrome(std::string& s) {
	if (s.size() == 0 || (s.size() == 2 && s[0] == s[1])) {
		return s;
	}
	int b = 0, max_len = 1;
	int len = s.size() - 1;
	for (int i = 0; i < len; ++i)
	{
		if (s[i] == s[i + 1])
		{
			std::tuple<int, int> res = get_plalindrome_len(s, i - 1, i + 2);
			if (std::get<1>(res) > max_len) {
				b = std::get<0>(res);
				max_len = std::get<1>(res);
			}
		}
		
		if (i > 0 && s[i - 1] == s[i + 1])
		{
			std::tuple<int, int> res = get_plalindrome_len(s, i - 1, i + 1);
			if (std::get<1>(res) > max_len) {
				b = std::get<0>(res);
				max_len = std::get<1>(res);
			}
		}
	}
	return s.substr(b, max_len);
}

std::string convert(std::string &s, int numRows) {
	if (numRows == 1) return s;

	std::string ret;
	int n = s.size();
	int cycleLen = 2 * numRows - 2;

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j + i < n; j += cycleLen) {
			ret += s[j + i];
			if (i != 0 && i != numRows - 1 && j + cycleLen - i < n)
				ret += s[j + cycleLen - i];
		}
	}
	return ret;
}


void test_win_desktop_item()
{
#ifdef _MSC_VER

	int index = 8;

	HWND progman = FindWindowA("Progman", "Program Manager");
	HWND def_view = FindWindowExA(progman, NULL, "SHELLDLL_DefView", NULL);
	HWND hwnd = FindWindowExA(def_view, NULL, "SysListView32", "FolderView");

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);

	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE , FALSE, dwProcessId);
	PVOID pv = VirtualAllocEx(hProcess, NULL, sizeof(POINT), MEM_COMMIT, PAGE_READWRITE);

	ListView_GetItemPosition(hwnd, index, pv);

	POINT  pt;

	size_t rw_size = 0;

	ReadProcessMemory(hProcess, pv, &pt, sizeof(POINT), &rw_size);
	printf("rw_size = %d\n", rw_size);
	printf("%d %d\n", pt.x, pt.y);
	
	RECT rect = { 0 };
	rect.left = 0;

	rw_size = 0;
	BOOL ret = WriteProcessMemory(hProcess, pv, &rect, sizeof(RECT), &rw_size);
	printf("write res = %d, rw_size = %d\n", ret, rw_size);
	SendMessageA(hwnd, LVM_GETITEMRECT, index, (WPARAM)pv);
	rw_size = 0;
	ReadProcessMemory(hProcess, pv, &rect, sizeof(RECT), &rw_size);
	//printf("rw_size = %d\n", rw_size);
	printf("%d ,%d, %d, %d\n", rect.left, rect.top, rect.right, rect.bottom);
	

	int half_w = (rect.right - rect.left) / 2;
	int half_h = (rect.bottom - rect.top) / 2;
	int offsetx = pt.x - rect.left;
	int offsety = pt.y - rect.top;

	printf("offsetx = %d offsety = %d", offsetx, offsety);

	printf("half_w = %d half_h = %d", half_w, half_h);


	VirtualFreeEx(hProcess, pv, sizeof(RECT), MEM_RELEASE);
	CloseHandle(hProcess);

	ListView_SetItemPosition(hwnd, index, -half_w + offsetx, -half_h + offsety);

#endif //_MSC_VER
}

class A {
public:
	virtual void f(int a) {
		std::cout << "f(" << a << ")" << std::endl;
	}
};

class B : public A{
public:
	virtual void f(int a) override {
		std::cout << "son f(" << a << ")" << std::endl;
	}
	void f(int a,int b) {
		std::cout << "son f(" << a << "," << b << ")" << std::endl;
	}
};

void test_override_overwrite()
{
	B b;
	A& a = b;
	a.f(8);
	b.f(7, 8);
	A a2;
	a2.f(10);
}
template<typename T> std::vector<T> template_var;
void test_template_var()
{
	void * p1 = &template_var<int>;
	void * p2 = &template_var<float>;

	printf("%p %p\n", p1, p2);
}

int romanToInt(std::string s) {

	std::tuple<const char *, int> as_arr[] = {		std::make_tuple("M" ,	1000),
													std::make_tuple("CM",	900),
													std::make_tuple("D",	500),
													std::make_tuple("CD",	400),
													std::make_tuple("C",	100),
													std::make_tuple("XC",	90),
													std::make_tuple("L",	50),
													std::make_tuple("XL",	40),
													std::make_tuple("X",	10),
													std::make_tuple("IX",	9),
													std::make_tuple("V",	5),
													std::make_tuple("IV",	4),
													std::make_tuple("I",	1) };

	/*for(auto a : as_arr) {
		cout << std::get<0>(a)  << "  " << std::get<0>(a).size() << "  " << std::get<1>(a) << "\n";
	}*/


	int j = 0;
	int res = 0;
	for (int i = 0; i < s.size();)
	{
		const char *ps = std::get<0>(as_arr[j]);
		int pn = std::get<1>(as_arr[j]);
		int ps_size = j % 2 == 0 ? 1 : 2;
		if (ps_size == 1) {
			if (ps[0] == s[i])
			{
				res += pn;
				i += 1;
			}
			else {
				j++;
			}
		}
		else {
			if(i + 1 >= s.size())
			{
				j++;
			}else if (ps[0] == s[i] && ps[1] == s[i + 1])
			{
				res += pn;
				i += 2;
			}
			else {
				j++;
			}
		}
	}
	return res;
}

std::string longestCommonPrefix(std::vector<std::string> &strs) {
	if (strs.size() == 1)
		return strs[0];
	std::string res;
	int min_len = 0x7fffffff;
	for (auto &s : strs)
	{
		if (s.size() < min_len)
		{
			min_len = s.size();
		}
	}
	if (min_len == 0x7fffffff)
		return "";
	int n = strs.size() - 1;
	for (int i = 0; i < min_len; ++i)
	{
		char last = strs[0][i];
		for (int j = 1;j < strs.size(); j++)
		{
			if (last != strs[j][i])
			{
				return res;
			}
			if (j == n)
			{
				res += strs[j][i];
			}
			last = strs[j][i];
		}
	}
	return res;
}

std::vector<std::vector<int>> threeSum(std::vector<int>& nums)
{
	std::vector<std::vector<int>> res;
	std::sort(nums.begin(), nums.end());
	int last = static_cast<int>(nums.size()) - 1;
	for (int i = 0; i < static_cast<int>(nums.size()); ++i)
	{
		if (nums[i] > 0)
			break;
		if (i > 0 && nums[i] == nums[i - 1])
			continue;
		int b = i + 1;
		int e = last;
		int t = 0 - nums[i];
		while (b < e)
		{
			if (nums[b] + nums[e] == t)
			{
				res.push_back(std::vector<int>({ nums[i],nums[b],nums[e] }));
				while (b < e && nums[b] == nums[b + 1])
					++b;
				while (b < e && nums[e] == nums[e - 1])
					--e;
				++b; --e;
			}
			else if (nums[b] + nums[e] > t)
			{
				--e;
			}
			else
			{
				++b;
			}
		}
	}
	return res;
}

std::vector<std::string> letterCombinations(std::string&& digits_) {
	std::string digits = std::move(digits_);

	std::vector<std::string> res;
	if (digits.empty())
	{
		return res;
	}
	int map_nums[] = { 3,3,3,3,3,4,3,4 };
	std::vector<std::vector<int>> map_v = { {0,1,2},{3,4,5},{6,7,8},{9,10,11},{12,13,14},{15,16,17,18},{19,20,21},{22,23,24,25} };
	int len = 8;
	int offset = -2;
	int offset_ = -48;
	char a = 'a';
	
	std::vector<int>indexs;
	for (int i = 0; i < digits.size(); ++i)
	{
		indexs.push_back(0);
	}
	std::vector<int> digits__;
	for (int i = 0; i < digits.size(); ++i)
	{
		digits__.push_back(static_cast<int>(digits[i]) +offset_ + offset);
	}

	for (;;)
	{
		std::string temp;
		for (int i = 0;i < indexs.size();++i)
		{	
			temp += (a + (char)map_v[ digits__[i] ][indexs[i]]);
		}
		res.push_back(temp);


		for (int i = indexs.size() - 1;i >= 0 ; --i)
		{
			if (indexs[i] < map_nums[digits__[i]] - 1)
			{
				indexs[i] += 1;
				for (int j = i + 1; j < indexs.size(); ++j)
				{
					indexs[j] = 0;
				}
				break;
			}
			if (i == 0)
				goto END;
		}
	}

END:
	return res;
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1),
                                    CREATE_TEST_FUNC(test2_2),
                                    CREATE_TEST_FUNC(test2_3),
                                    CREATE_TEST_FUNC(test_any),
									CREATE_TEST_FUNC(test_rr),
									CREATE_TEST_FUNC(findMedianSortedArrays),
									CREATE_TEST_FUNC(findMedianSortedArrays2),
									CREATE_TEST_FUNC(longestPalindrome),
									CREATE_TEST_FUNC(convert),
									CREATE_TEST_FUNC(test_win_desktop_item),
									CREATE_TEST_FUNC(test_override_overwrite),
									CREATE_TEST_FUNC(test_template_var),
									CREATE_TEST_FUNC(romanToInt),
									CREATE_TEST_FUNC(longestCommonPrefix),
									CREATE_TEST_FUNC(threeSum),
									CREATE_TEST_FUNC(letterCombinations)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__
