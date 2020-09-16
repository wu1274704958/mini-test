#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <stack>
#include <comm.hpp>

namespace wws{

template<size_t Buf_size,typename Path,typename ...Args>
std::optional<std::string> read_from_file(Path&& path,Args&& ...args)
{
    std::ifstream ifs;
    if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::string>)
    {
        ifs = std::ifstream(path.c_str(),std::forward<Args>(args)...);
    }else if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::filesystem::path>)
    {
        std::string temp =  path.generic_string();
        ifs = std::ifstream(temp.c_str(),std::forward<Args>(args)...);
    }
    if(ifs.good())
    {
        std::string res;
        res.reserve(Buf_size);
		while (!ifs.eof())
		{
			char buf[Buf_size] = { 0 };
			ifs.read(buf, Buf_size - 1);
			res.append(buf);
		}
		return std::optional<std::string>(res);
    }
    else
        return {};
}

template<typename Str,typename Path,typename ...Args>
bool write_to_file(Path&& path,Str&& str,Args&& ...args)
{
    static_assert( std::is_same_v<std::remove_cv_t<std::remove_reference_t<Str>>,std::string>,"this Str must be std::string!");
    std::ofstream ofs;
    if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::string>)
    {
        ofs = std::ofstream(path.c_str(),std::forward<Args>(args)...);
    }else if constexpr(std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>,std::filesystem::path>)
    {
        std::string temp =  path.generic_string();
        ofs = std::ofstream(temp.c_str(),std::forward<Args>(args)...);
    }
    if(ofs.good())
    {
        ofs << str;
		return true;
    }
    else
        return false;
}

template<size_t S,typename Path, typename ...Args>
bool write_to_file_bin(Path&& path,const unsigned char* data,uint64_t len, Args&& ...args)
{
    std::ofstream ofs;
    if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>, std::string>)
    {
        ofs = std::ofstream(path.c_str(), std::forward<Args>(args)...);
    }
    else if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>, std::filesystem::path>)
    {
        std::string temp = path.generic_string();
        ofs = std::ofstream(temp.c_str(), std::forward<Args>(args)...);
    }
    if (ofs.good())
    {
        ofs.write(reinterpret_cast<const char*>(data), len);
        return true;
    }
    else
        return false;
}

template<size_t MAX, typename Path, typename ...Args>
std::tuple<std::unique_ptr<char[]>, std::uint64_t> read_from_file_bin(Path&& path, Args&& ...args)
{
    std::ifstream ifs;
    if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>, std::string>)
    {
        ifs = std::ifstream(path.c_str(), std::forward<Args>(args)...);
    }
    else if constexpr (std::is_same_v<std::remove_reference_t<std::remove_cv_t<Path>>, std::filesystem::path>)
    {
        std::string temp = path.generic_string();
        ifs = std::ifstream(temp.c_str(), std::forward<Args>(args)...);
    }
    if (ifs.good())
    {
        size_t max_size = MAX;
        size_t size = 0;
        char* data = reinterpret_cast<char*>( std::malloc(MAX));
        if(!data)  throw std::runtime_error("alloc failed!");
        while (!ifs.eof())
        {
            char buf[MAX] = { 0 };
            ifs.read(buf, wws::arrLen(buf));
            size_t t_s = ifs.gcount();
            if (t_s <= 0)
                break;
            if (size + t_s > max_size)
            {
                auto n_data = reinterpret_cast<char*>(std::realloc(data, max_size + MAX));
                if (n_data)
                {
                    data = n_data;
                    max_size += MAX;
                }
                else {
                    free(data);
                    throw std::runtime_error("realloc failed!");
                }
            }
            std::memcpy(data + size, buf, t_s);
            size += t_s;
        }
        return std::make_tuple(std::unique_ptr<char[]>(data), size);
    }
    else
        return std::make_tuple(nullptr, 0);
}

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string get_parent(Path&& path)
{
    if(path.empty()) return path;
    for(size_t i = path.size() - 1;i >= 0; --i)
    {
        if(path[i] == '/')
        {
            return path.substr(0,i);
        }
    }
    return path;
}

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string to_absolute(Path&& path)
{
    if(path.empty()) return path;
    int b = 0;
    std::stack<std::string> sta;
    for(int i = 0;i < path.size();++i)
    {
        if(path[i] == '/' && b < i)
        {
            auto str = path.substr(b,i - b);
            if(str == ".")
            {}else 
            if(str == "..")
            {
                if(sta.empty())
                    throw std::runtime_error("Bad path for func to_absolute!!");
                sta.pop();
            }else{
                str += '/';
                sta.push(std::move(str));
            }
            b = i + 1;++i;
        }
    }
    if(b < path.size() - 1)
    {
        auto str = path.substr(b,path.size() - b);
        sta.push(std::move(str));
    }
    std::string res;
    while (!sta.empty())
    {
        res.insert(0,sta.top().c_str());
        sta.pop();
    }
    return res;
}

template<typename Path,typename = std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<Path>>,std::string>>>
inline std::string get_suffix(Path&& path)
{
    if(path.empty()) return path;
    for(int i = path.size() - 1;i >= 0;--i)
    {
        if(path[i] == '.')
        {
            return path.substr(i + 1,path.size() - i - 1);
        }
    }
    return "";
}

inline bool is_absolute(const std::string& path)
{
    if(path.empty()) return false;
    if(path[0] == '/') return true;
    if(path[1] == ':' && path[2] == '\\') return true;
    return false;
}

}