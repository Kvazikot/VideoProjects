#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <regex>
#include <map>

#define _STR(text) " "
#define _MSTR(text) ()
#define STR_(text) decode(text)

std::map<char, char> cod_table;
std::map<char, char> decod_table;


void make_coding_table(int seed)
{
	std::regex word_regex("([ \\w]+)");
	std::string s = " ";

	srand(seed);
	cod_table.clear();
	decod_table.clear();
	printf("coding table\n");
	for (char c = -127; c != 127; c++)
	{
		s[0] = c;
		std::smatch match;
		if (!std::regex_search(s, match, word_regex))
		{
			cod_table[c] = '@';
			decod_table['@'] = '@';
			continue;
		}
		char c2=' ';
		int max_iters = 100000;
		while (max_iters > 0)
		{
			c2 = (char)rand() & 0xFF;			
			s[0] = c2;
			std::smatch match2;
			if (std::regex_search(s, match2, word_regex))
			{
				if( decod_table.find(c2) == decod_table.end())
				  break;
			}
			max_iters--;
		}
		cod_table[c] = c2;
		decod_table[c2] = c;
		//printf("%c - %c\n", c, c2);
	}
	std::regex punct_regex("([~!@#$%^&*()/]+)");
	for (char c = -127; c != 127; c++)
	{
		s[0] = c;
		if (c == '\\')
			printf("slash %c", c);
		std::smatch match;
		if (std::regex_search(s, match, punct_regex))
		{
			cod_table[c] = c;
			decod_table[c] = c;
		}
	}
	cod_table['\\'] = '\\';
	decod_table['\\'] = '\\';
	cod_table['\n'] = '\n';
	decod_table['\n'] = '\n';

	for(auto it = decod_table.begin(); it!= decod_table.end(); it++)
		printf("%c - %c\n", it->first, it->second);
	printf("-------------------------------\n");
}


std::string obfuscate(std::string in_str, bool bDecode=false)
{
	std::string::iterator is = in_str.begin();
	std::string::iterator ie = in_str.end()-1;
	std::string::iterator im = is;

	for (int i = 0; i < in_str.size() / 2; i++) im++;
	while (is != im)
	{
		std::swap(*is, *ie);
		if (bDecode)
		{
			*is = decod_table[*is];
			*ie = decod_table[*ie];
		}
		else
		{
			*is = cod_table[*is];
			*ie = cod_table[*ie];
		}
		is++; ie--;
	}


	//multiline fix
	is = in_str.begin() + 2;
	while (is != in_str.end())
	{
		auto prev = is; (prev--)--;
		if (*is == '\\' && *prev == '\n') 
			std::swap(*is, *prev);
		is++; 
	}


	return in_str;
}

std::string decode(std::string in_str)
{
	return obfuscate(in_str, true);
}

void test_code_with_STR_macros()
{
	make_coding_table(64278);
	std::string string1 = _STR("This is a test string to obfuscate");
	std::cout << string1 << "\n";
	std::string string5 = _STR("1234\
5678\
abcd");
	std::cout << string5 << "\n";
	std::cout << string1 << "\n";
	std::string string12 = _STR("1\
5678\
abcd");

    std::string string3 = "Pukin is a looser who want to kills everybody in a zombie apocalypse!";
	std::string string2 = obfuscate(string5, false);
	std::string string22 = obfuscate(string2, true);
	std::cout <<  "1. " << string5 << " == " << string2 << " \n";
	std::cout << "2. " << string2 << "==" << string22 << " \n";

	std::string string4 = _STR("But we do not allow him to do it!");
	std::cout << string4;
	std::cout << string12;
}

void obfuscate_cpp_file(std::string src_filename, std::string& src)
{
	std::ifstream f(src_filename);
	std::stringstream s;
	make_coding_table(64278);
	//return;
	if (f) {
		s << f.rdbuf();
		f.close();
		src = s.str();
		std::regex word_regex("(#include [\"<]+[\\S]+[\">]+)");
		auto words_begin =
			std::sregex_iterator(src.begin(), src.end(), word_regex);
		auto words_end = std::sregex_iterator();
		
		std::cout << "Found "
			<< std::distance(words_begin, words_end)
			<< " includes\n";
		
		std::cout << "includes: \n";
		std::smatch match;
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			match = *i;			
			std::string match_str = match.str();
			std::cout << "  " << match_str << '\n';
		}

		//insert namespace after last include
		std::regex filter_name_regex("\\w+");
		std::smatch match2;
		std::regex_search(src_filename, match2, filter_name_regex);
		src.insert(match.position() + match.length(), "\n\nnamespace " + match2.str() + "_ns{");
		src += "\n}";

		//obfuscate strings
			//[a - zA - Z_0 - 9() \"]+
		std::regex str_regex("(_STR)[(]+[\"]+([^\"]+)[\"]+[)]+");
		words_begin =
			std::sregex_iterator(src.begin(), src.end(), str_regex);
		words_end = std::sregex_iterator();

		std::cout << "Found "
			<< std::distance(words_begin, words_end)
			<< " STR macroses\n";

		const int N = 6;
		std::cout << "MATCHES: \n";
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str(2);
			std::string obfuscated_str = obfuscate(match_str);
			src.replace(match.position(1), match.length(1), "STR_");
			src.replace(match.position(2), match.length(2), obfuscated_str);
			std::cout << "  " << match_str << "< - >" << obfuscated_str << '\n';
			
		}
		std::cout << "\n------------------- source code -------------------";
		std::cout << src;
		std::cout << "\n------------------- end of source code -------------------\n";
		//src = "";
		
	}
}

void save_cpp_source(std::string filename, std::string src)
{
	std::ofstream f(filename);
	if (f) {
		f << src;
		f.close();
	}
	else
		std::cout << "Cannot open " << filename;
}

