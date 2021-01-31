#include "core/file/FilePath.hpp"

std::string joinPath(const std::initializer_list<StringSpan>& parts)
{
	std::string s;
	bool first = true;
	for(auto&& part: parts)
	{
		if(!first) {
			s += PATH_SEP_STR;
		}
		else
		{
			first = false;
		}

		s.insert(s.end(), part.begin(), part.end());
	}

	return s;
}

std::vector<std::string> splitPath(StringSpan stringSpan)
{
	auto prev = stringSpan.begin();
	auto end = stringSpan.end();
	auto search = prev;

	std::vector<std::string> parts;

	while(search != end) {
		if((*search) == PATH_SEP_CHAR) {
			parts.push_back(StringSpan(prev,search).to_string());
			search++;
			prev = search;
			continue;
		}

		search++;

	}

	if(prev != end) {
		parts.push_back(StringSpan(prev,end).to_string());
	}

	return parts;
}

std::string getPathEntryName(StringSpan stringSpan) {
	auto parts = splitPath(stringSpan);
	if(parts.size()==0) {
		return std::string();
	}

	return parts[parts.size()-1];
}
