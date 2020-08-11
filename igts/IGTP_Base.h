#ifndef _IGTP_BASE_H
#define _IGTP_BASE_H

#include <map>
#include <string>

using std::string;
using std::map;

class IGTP_Base
{
public:
	enum class IGTP_Version
	{
		UNKNOWN,
		v01
	};

	enum class IGTP_Method
	{
		UNKNOWN,
		Hello,
		Val
	};

	enum class IGTP_Status_Code
	{
		UNKNOWN = 0,
		s100 = 100
	};


	static map<IGTP_Method, string> method_to_str_dict;
	static map<string, IGTP_Method> str_to_method_dict;

	static map<IGTP_Version, string> version_to_str_dict;
	static map<string, IGTP_Version> str_to_version_dict;

	static map<IGTP_Status_Code, string> statuscode_to_str_dict;

};
#endif