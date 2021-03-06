#pragma once
#include <string>
#include <vector>

namespace rvn {
	namespace util {
		static std::vector<std::string> split(const std::string& str, const std::string& delim)
		{
			std::vector<std::string> out;
			size_t start;
			size_t end = 0;

			while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
			{
				end = str.find(delim, start);
				out.push_back(str.substr(start, end - start));
			}
			return out;
		}
	}
}