#pragma once 

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <version>

#ifdef __cpp_lib_filesystem
#include <filesystem>
#endif 




namespace token {

	struct Token {
		char per;
		char back;
		std::string body;
		constexpr static char None = '\0';
		Token()
		{
			none_per();
			none_back();
		}
		Token(std::string body_) : body(std::move(body_))
		{
			none_per();
			none_back();
		}
		Token(std::string body_,char per_,char back_) 
		:	body(std::move(body_)) ,
			per(per_),
			back(back_)
		{}

		Token(const Token&) = delete;
		Token(Token&& t)
		{
			per = t.per;
			back = t.back;
			body = std::move(t.body);
		}

		Token& operator=(const Token&) = delete;
		Token& operator=(Token &&t)
		{
			per = t.per;
			back = t.back;
			body = std::move(t.body);
			return *this;
		}
		void none_per()
		{
			per = None;
		}
		void none_back()
		{
			back = None;
		}
		bool per_is_none()
		{
			return per == None;
		}
		bool back_is_none()
		{
			return back == None;
		}
		friend std::ostream& operator<<(std::ostream& out, Token &t);
	};

	inline std::ostream& operator<<(std::ostream& out, Token &t)
	{
		if (!t.per_is_none())
		{
			if(t.per == '\n')
				out << '\'' << "\\n" << '\'';
			else
				out << '\'' << t.per << '\'';
		}
		out << t.body;
		if (!t.back_is_none())
		{
			if (t.back == '\n')
				out << '\'' << "\\n" << '\'';
			else
				out << '\'' << t.back << '\'';
		}
		return out;
	}

	struct NoNextErr {

	};

	template<typename T>
	struct CharIter;

	template<typename T>
	struct TokenStream {
		std::vector<Token> tokens;
		CharIter<T> iter;
		char c;
		bool not_take = false;
		bool not_del_space = false;
		TokenStream(T t) : iter(std::move(t)) {}
		void analyse(bool not_del_space = false)
		{
			this->not_del_space = not_del_space;
			while(true)
			{
				try {
					tokens.push_back(std::move(get_next_token()));
				}
				catch (NoNextErr e)
				{
					break;
				}
			}
		}
		bool is_token(char c)
		{
			return
				c == '\'' ||
				c == '"' ||
				c == ' ' ||
				c == '.' ||
				c == ',' ||
				c == '\t'||
				c == '=' ||
				c == '+' ||
				c == '-' ||
				c == '*' ||
				c == '/' ||
				c == '!' ||
				c == ':' ||
				c == '|' ||
				c == '&' ||

				c == '(' ||
				c == '{' ||
				c == '<' ||
				c == '[' ||
				c == ')' ||
				c == '}' ||
				c == '>' ||
				c == ']' ||
				c == ';' ||
				c == '\n';
		}
		Token get_next_token() noexcept(false)
		{
			try {
				Token t;
				int stage = 0;
				while (not_take || iter.has_next())
				{
					if (!not_take)
						c = iter.next();
					else
						not_take = false;
					if (c == '\r' ||
						c == -1) continue;
					if (c == '"' ||
						c == '\'')
					{
						char temp = 0;
						switch (stage)
						{
						case 0:
							t.per = c;
							while (iter.has_next() && (temp = iter.next()) != c) {
								t.body += temp;
							}
							if (is_token(temp))
							{
								t.back = temp;
								stage = 2;
							}
							else {
								not_take = true;
								stage = 1;
							}
							break;
						case 1:
							t.none_back();
							++stage;
							not_take = true;
							break;
						}
					}else
					if (c == ' ' ||
						c == '.' ||
						c == ',' || 
						c == '\t'||
						c == '=' ||
						c == '+' ||
						c == '-' ||
						c == '*' ||
						c == '/' ||
						c == '!' ||
						c == ':' ||
						c == '|' ||
						c == '&'  )
					{
						switch (stage)
						{
						case 0:
							t.per = c;
							if (c == ' ')
							{
								not_take = skip(c);
							}
							++stage;
							break;
						case 1:
							t.back = c;
							if (c == ' ')
							{
								not_take = skip(c);
							}
							++stage;
							break;
						}
					}else
					if (c == '(' ||
						c == '{' ||
						c == '<' ||
						c == '[')
					{
						switch (stage)
						{
						case 0:
							t.per = c;
							not_take = skip(c);
							++stage;
							break;
						case 1:
							t.none_back();
							++stage;
							not_take = true;
							break;
						}
					}
					else
					if (c == ')' ||
						c == '}' ||
						c == '>' ||
						c == ']' ||
						c == ';' ||
						c == '\n'
					 )
					{
						if (stage == 0)
							t.none_per();
						t.back = c;
						stage = 2;
					}else
					{
						if (stage == 0)
						{
							t.none_per();
							++stage;
							not_take = true;
						}else
						if (stage == 1)
						{
							t.body += c;
						}
					}
					if (stage == 2)
						break;
				}
				if (stage == 2)
					return t;
				else if(stage == 0)
					throw NoNextErr();
				else
				{
					t.none_back();
					return t;
				}
			}
			catch (NoNextErr e)
			{
				throw e;
			}
		}
	private:
		
		bool skip(char &curr,char tar = ' ')
		{
			if (not_del_space)
				return false;
			bool has_one_not_tar = false;
			while (iter.has_next()) 
			{
				if ((curr = iter.next()) != tar)
				{
					has_one_not_tar = true;
					break;
				}
			}
			return has_one_not_tar;
		}
	public:
		/*
		* o is 如果文件已存在是否重写
		**/
		bool save(std::string& path,bool o = false)
		{
#ifdef  __cpp_lib_filesystem

			namespace fs = std::filesystem;
			fs::path p(path);
			if (fs::exists(p))
			{
				if (o )
				{
					//fs::remove(p);
					std::ofstream os(p.generic_string(), std::ios::binary);
					return save_(os);
				}
				else
					return false;
			}
			else {
				std::ofstream os(p.generic_string(), std::ios::binary);
				return save_(os);
			}
#else
			std::cout << "Not support filesystem!" << std::endl;
#endif //  __cpp_lib_filesystem
		}

	private:
		bool save_(std::ofstream& os)
		{
#ifdef WIN32
			const char *enter = "\r\n";
			const long long enter_len = 2;
#else
			const char *enter = "\n";
			const long long enter_len = 1;
#endif // WIN32
			for (auto& t : tokens)
			{
				if (!t.per_is_none())
				{
					if (t.per == '\n')
					{
						os.write(enter, enter_len);
					}
					else {
						os.write(&t.per, 1);
					}
				}
				if (t.body.size() > 0)
				{
					os.write(t.body.data(), t.body.size());
				}
				if (!t.back_is_none())
				{
					if (t.back == '\n')
					{
						os.write(enter, enter_len);
					}
					else {
						os.write(&t.back, 1);
					}
				}
			}
			os.flush();
			return true;
		}
	};

	template<>
	struct CharIter<std::string>
	{
		std::string str;
		uint32_t i;
		CharIter(std::string s) : str(std::move(s)) { i = 0; }

		char next() noexcept(false)
		{
			if (!has_next())
			{
				throw NoNextErr();
			}
			else {
				return str[i++];
			}
		}

		bool has_next()
		{
			return i < str.size();
		}
	};

	template<>
	struct CharIter<std::ifstream>
	{
		std::ifstream str;
		CharIter(std::ifstream s) : str(std::move(s)) { }

		char next() noexcept(false)
		{
			if (!has_next())
			{
				throw NoNextErr();
			}
			else {
				return str.get();
			}
		}

		bool has_next()
		{
			return !str.eof();
		}
	};

}