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
    {"if",0},		{"return",0},	{"void",0},		{"int",0},		{"const",0}, 
    {"else",0},		{"for",0},		{"case",0},		{"break",0},	{"sizeof",0}, 
    {"static",0},	{"while",0},	{"long",0},		{"default",0},	{"typedef",0},
    {"switch",0},	{"short",0},	{"char",0},		{"struct",0},	{"double",0}, 
    {"continue",0},	{"do",0},		{"unsigned",0},	{"extern",0},	{"float",0}, 
    {"enum",0},		{"goto",0},		{"register",0},	{"auto",0},		{"signed",0},
    {"union",0},	{"volatile",0}
    };
	in_file.open("test1.cpp");   
	if (!in_file.is_open())
	{
		cout << "open file error" << endl;
		return 0;
	}
	string file_to_string, temp;
	while (getline(in_file, temp))
	{	
		file_to_string += temp;
		file_to_string.append(" ");
	}
	in_file.close();
	//将标点符号转换成空格 
	for (int i = 0; i < file_to_string.length(); i++)
	{
		if (ispunct(file_to_string[i]))  
			file_to_string[i] = ' ';
	}
	stringstream ss(file_to_string);
	while (ss >> temp)
	{
		for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
		{	if(it->first == temp)
				reserved_word_maps[temp]++;
		}
	}
 	int total_num = 0;
	for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
		total_num += it->second;
 	cout << "total num = " << total_num<<endl;
 	auto end_time = Clock::now();
 	auto computing_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
	cout << "Computing time = " << computing_time.count() / 1000 << " ms" << endl;
	
	return 0;
}
