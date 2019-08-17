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

	struct BadKeyErr : public std::exception
	{
		BadKeyErr() : exception("Bad key !!!") {}
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
						stage = 0;
						continue;
					}else
						++deep;
				}

				if (stage == 1 && curr.per == '[')
				{
					std::string temp;
					while (true)
					{
						if (i >= ts.size())
							throw BadJsonErr();
						if (ts[i].per == '[' || ts[i].per == ',' || ts[i].per == '"' || ts[i].per == '\'')
							temp += ts[i].per;
						if (!ts[i].body.empty())
							temp += ts[i].body;
						if (ts[i].back == ']' || ts[i].back == ',' || ts[i].per == '"' || ts[i].per == '\'')
							temp += ts[i].back;
						if (ts[i].back == ']')
							break;
						++i;
					}
					data.insert(std::pair(std::move(key), std::move(temp)));
					stage = 0;
					continue;
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
						std::string temp;
						if ((curr.per == '"' && curr.back == '"') || (curr.per == '\'' && curr.back == '\''))
						{
							temp += curr.per;
							temp += curr.body;
							temp += curr.back;
						}
						else
						{
							temp += curr.body;
						}
						data.insert(std::pair(std::move(key), std::move(temp)));
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
					if (!ts[i].body.empty()) --i;
				}
			}
		}

		void insert_ch(std::vector<token::Token>& ts,std::string& key,int& i) noexcept(false)
		{
			std::string temp;
			int deep = 0;
			while (true) {
				if (i >= ts.size())
					throw BadJsonErr();
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
		template<typename T>
		T get(std::string& key)
		{
			if (data.find(key) == data.end())
			{
				throw BadKeyErr();
			}
			if constexpr (std::is_same_v<T, std::string>)
			{
				std::string res = data[key];
				remove_qm(res);
				return res;
			}
			else {
				return wws::parser<T>(data[key]);
			}
		}

		void remove_qm(std::string& str)
		{
			str.erase(0, 1);
			str.pop_back();
		}

		void insert_qm(std::string& str)
		{
			str.insert(0, 1, '"');
			str += '"';
		}

		template<typename T>
		T get(std::string&& key)
		{
			return get<T>(key);
		}

		std::string& get_str(std::string& key) 
		{
			if (data.find(key) == data.end())
			{
				throw BadKeyErr();
			}
			return data[key];
		}

		std::string& get_str(std::string&& key)
		{
			return get_str(key);
		}

		bool has_key(std::string& key)
		{
			if (chs.find(key) != chs.end())
			{
				return true;
			}else if(data.find(key) != data.end()){
				return true;
			}
			return false;
		}

		bool has_key(std::string&& key)
		{
			return has_key(key);
		}

		Json& get_obj(std::string& key)
		{
			if (chs.find(key) == chs.end())
			{
				throw BadKeyErr();
			}
			return chs[key];
		}

		Json& get_obj(std::string&& key)
		{
			return get_obj(key);
		}

		template<typename T>
		std::vector<T> get_arr(std::string& key)
		{
			if (data.find(key) == data.end())
			{
				throw BadKeyErr();
			}
			std::vector<T> res;
			wws::parser_stl(data[key], res);
			return res;
		}

		template<typename T>
		std::vector<T> get_arr(std::string&& key)
		{
			return get_arr<T>(key);
		}

		template<typename T>
		Json& put(std::string&& key,T&& t)
		{
			if constexpr ( std::is_same_v<std::string, std::remove_reference_t<T>> )
			{
				insert_qm(t);
				data[key] = t;
			}
			else {
				data[key] = wws::to_string(std::forward<T>(t));
			}
			return *this;
		}

		Json& put(std::string&& key, const char* t)
		{
			std::string res;
			res += '"';
			res += t;
			res += '"';
			data[key] = res;
			return *this;
		}

		template<typename T>
		Json& put(std::string&& key, std::vector<T>& v)
		{
			if constexpr (std::is_same_v<T, std::string>)
			{
				data[key] = wws::to_string(v, true);
			}
			else {
				data[key] = wws::to_string(v);
			}
			return *this;
		}

		Json& put(std::string&& key, Json&& j)
		{
			chs[key] = std::move(j);
			return *this;
		}

		std::string to_string()
		{
			std::string res;
			res += '{';
			for (auto it = data.begin(); it != data.end(); ++it)
			{
				res += '"';
				res += it->first;
				res += '"';
				res += ':';
				res += it->second;
				res += ',';
			}
		
			for (auto it = chs.begin(); it != chs.end(); ++it)
			{
				res += '"';
				res += it->first;
				res += '"';
				res += ':';
				res += it->second.to_string();
				res += ',';
			}

			res.pop_back();
			res += '}';
			return res;
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