#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;
typedef chrono::high_resolution_clock Clock;
int main()
{
	auto start_time = Clock::now();
	ifstream in_file;
	map<string, int> reserved_word_maps;
  	reserved_word_maps = {
    {"auto",0},{"break",0},{"case",0},{"char",0},{"const",0},
	{"continue",0},{"default",0},{"do",0},{"double",0},{"else",0},
	{"enum",0},{"extern",0},{"float",0},{"for",0},{"goto",0},
	{"if",0},{"int",0},{"long",0},{"register",0},{"return",0},
	{"short",0},{"signed",0},{"sizeof",0},{"static",0},{"struct",0},
	{"switch",0},{"typedef",0},{"union",0},{"unsigned",0},{"void",0},				 
    {"volatile",0},{"while",0}
    };
	in_file.open("test2.cpp");   
	if (!in_file.is_open())
	{
		cout << "open file error" << endl;
		return 0;
	}
	string file_to_string, string_in_file, temp;
	while (getline(in_file, temp))
	{	
		int i = 0;
		while(temp[i] == ' ')
			++i;
		if(temp[i] == '/' && temp[i+1] == '/')
			continue;
		string_in_file += temp;
		string_in_file.append(" ");
	}
	in_file.close();
	for (int i = 0; i < string_in_file.length(); ++i)	//handle signs
	{
		if(string_in_file[i] == '\"' ){
			++i;
			while(string_in_file[i++] != '\"'){}		
		}	
		else if(string_in_file[i] == '/' && string_in_file[i+1] == '*' )
			while(string_in_file[i] != '*'||string_in_file[i+1] != '/')
				++i;
		else if(string_in_file[i] == '{' || string_in_file[i] == '}')
		{
			file_to_string += ' ';
			file_to_string += string_in_file[i];
			file_to_string += ' ';
		}
		else if (ispunct(string_in_file[i]))  
			file_to_string += ' ';
		else
			file_to_string += string_in_file[i];
	}
	stringstream ss(file_to_string);
	while (ss >> temp)
	{
		for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
		{	
			if(it->first == temp)
				reserved_word_maps[temp]++;
		}
	}
 	int total_num = 0;
	for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
		total_num += it->second;	
 	cout << "total num = " << total_num<<endl;
 	auto end_time = Clock::now();
 	auto computing_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
	cout << "Computing time = " << computing_time.count() << " ns" << endl;
	return 0;
}
