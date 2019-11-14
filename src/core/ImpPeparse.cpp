#include "ImpPeparse.h"
#include <iostream>
#include <LIEF/PE.hpp>
#include <LIEF/logging.hpp>

using namespace LIEF::PE;


/*
由于与windows.h等头文件冲突 所以放在此处
*/




extern "C" _CRTIMP char * __cdecl __unDNameEx(
	char * outputString ,
	char const * name ,
	int maxStringLength ,
	void * (__cdecl * pAlloc)(size_t) = malloc ,
	void(__cdecl * pFree)(void *) = free ,
	char * (__cdecl * pGetParameter)(long) = 0 ,
	unsigned long disableFlags = 0
);





ImpPeparse::ImpPeparse(const char* filePath) {
	std::unique_ptr<const Binary> binary{ Parser::parse(filePath) };

	// 导入表
	if (binary->imports().size() > 0) {
		for (const Import& Imports : binary->imports()) {
			for (const auto& Ent : Imports.entries()) {
				m_vecImpFunAddr.push_back((size_t)Ent.iat_address());
				auto funName = Ent.name();
				if (funName.c_str()[0]) {
					m_vecImpString.push_back(std::string(Imports.name() + " => " + funName));
					m_vecDecodingImpString.push_back(std::string(Imports.name() + " => " + get_decoding_cpp_name(funName)));
				} else {
					m_vecImpString.push_back(std::string(Imports.name() + " => (Ordinal)" + std::to_string(Ent.ordinal())));
					m_vecDecodingImpString.push_back("N/A");
				}
			}
		}
	}

	// 导出表
	if (binary->has_exports()) {
		m_IsHasExports = true;
		const auto& Exports = binary->get_export();
		for (const auto& Ent : Exports.entries()) {
			m_vecExpFunAddr.push_back(Ent.address());
			char tempBuf[0x500] = { 0 };
			auto funName = Ent.name();
			sprintf_s(tempBuf , sizeof(tempBuf) , "(Ordinal)%02X  => %s" , Ent.ordinal() , funName.c_str());
			m_vecExpString.push_back(std::string(tempBuf));
			memset(tempBuf , 0 , sizeof(tempBuf));
			sprintf_s(tempBuf , sizeof(tempBuf) , "(Ordinal)%02X  => %s" , Ent.ordinal() , get_decoding_cpp_name(funName).c_str());
			m_vecDecodingExpString.push_back(std::string(tempBuf));
		}
	}

	// 签名

}

ImpPeparse::~ImpPeparse() {

}

std::string ImpPeparse::get_decoding_cpp_name(const std::string & funName) {
	const char* decodingName = nullptr;
	if (funName.find("?") != std::string::npos) {
		decodingName = __unDNameEx(0 , funName.c_str() , 0);
	} else {
		decodingName = "N/A";
	}
	return decodingName;
}

