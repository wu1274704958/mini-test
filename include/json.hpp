#pragma once 
#include "serialization.hpp"
#include "token_stream.hpp"
#include <unordered_map>
#include <exception>

namespace wws {
	struct BadJsonErr : public std::exception
	{
		BadJsonErr() : exception("Bad Json string!!!") {}
	};

	class Json {
		std::unordered_map<std::string,std::string> data;
		std::unordered_map<std::string,Json> chs;
		void parser(std::string& str)
		{
			token::TokenStream<std::string> st(str);
			st.analyse();
			auto& ts = st.tokens;
			int deep = 0;
			int stage = 0;
			std::string key;
			for (int i = 0; i < ts.size(); ++i)
			{
				auto& curr = ts[i];
				if (curr.per == '{')
				{
					if (deep == 1)
					{
						if (stage != 1 || key.empty())
							throw BadJsonErr();
						insert_ch(ts, key, i);
						++i;
						stage = 0;
					}else
						++deep;
				}

				if (stage == 1 && !curr.body.empty())
				{
					if (curr.back == '.')
					{
						std::string temp;
						temp += curr.body;
						temp += curr.back;
						temp += ts[i + 1].body;
						data.insert(std::pair(std::move(key), std::move(temp)));
						++i;
					}
					else {
						data.insert(std::pair(std::move(key), std::move(curr.body)));
					}
					stage = 0;
					continue;
				}

				if (curr.back == ':')
				{
					int b = i;
					while (ts[b].body.empty()) { --b; }
					key = ts[b].body;
					stage = 1;
				}
				else if (curr.per == ':')
				{
					int b = i - 1;
					while (ts[b].body.empty()) { --b; }
					key = ts[b].body;
					stage = 1;
				}
			}
		}

		void insert_ch(std::vector<token::Token>& ts,std::string& key,int& i)
		{
			std::string temp;
			int deep = 0;
			while (true) {
				if (ts[i].per != token::Token::None)
					temp += ts[i].per;
				if (!ts[i].body.empty())
					temp += ts[i].body;
				if (ts[i].back != token::Token::None)
					temp += ts[i].back;
				if (ts[i].per == '{')
					++deep;
				if (ts[i].back == '}')
				{
					if (deep == 1) break;
					else
						--deep;
				}
				++i;
			}
			chs.insert(std::pair(std::move(key), Json(temp)));
		}
	public:
		Json() {}
		Json(std::string& str) {
			parser(str);
		}
		Json(const Json&) = delete;
		Json(Json&&) = default;
		Json& operator=(const Json&) = delete;
		Json& operator=(Json&&) = default;
	};
}