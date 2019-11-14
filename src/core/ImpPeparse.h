#pragma once
#include <vector>
#include <string>
class ImpPeparse {
public:
	explicit ImpPeparse(const char* filePath);
	~ImpPeparse();

private:
	std::string get_decoding_cpp_name(const std::string& funName);

public:
	bool m_IsHasExports;

	std::vector<size_t> m_vecImpFunAddr;
	std::vector<size_t> m_vecExpFunAddr;
	std::vector<std::string> m_vecImpString;
	std::vector<std::string> m_vecDecodingImpString;
	std::vector<std::string> m_vecExpString;
	std::vector<std::string> m_vecDecodingExpString;

};

