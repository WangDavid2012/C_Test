#include "IGTP_Base.h"

using namespace std;

// IGTP version.
map<IGTP_Base::IGTP_Version, string> IGTP_Base::version_to_str_dict =
{
	{ IGTP_Base::IGTP_Version::UNKNOWN, "" },
	{ IGTP_Base::IGTP_Version::v01, "IGTP/0.1" },
};

map<string, IGTP_Base::IGTP_Version> IGTP_Base::str_to_version_dict =
{
	{ "", IGTP_Base::IGTP_Version::UNKNOWN },
	{ "IGTP/0.1",IGTP_Base::IGTP_Version::v01 }
};

// IGTP method.
map<IGTP_Base::IGTP_Method, string> IGTP_Base::method_to_str_dict =
{
	{ IGTP_Base::IGTP_Method::UNKNOWN, "" },
	{ IGTP_Base::IGTP_Method::Hello, "Hello" },
	{ IGTP_Base::IGTP_Method::Val, "Val" },
};

map<string, IGTP_Base::IGTP_Method> IGTP_Base::str_to_method_dict =
{
	{ "",IGTP_Base::IGTP_Method::UNKNOWN },
	{ "Hello",IGTP_Base::IGTP_Method::Hello},
	{  "Val", IGTP_Base::IGTP_Method::Val }
};

map<IGTP_Base::IGTP_Status_Code,string>IGTP_Base::statuscode_to_str_dict=
{
	{IGTP_Base::IGTP_Status_Code::UNKNOWN,""},
	{ IGTP_Base::IGTP_Status_Code::s100,"100 OK" }
};
