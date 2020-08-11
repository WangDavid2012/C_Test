#ifndef _HELPER_H
#define _HELPER_H

#include <string>

using std::string;

/*return uri and save query_str in query*/
string parse_uri(string &path, string &query);

string get_now_time();

#endif