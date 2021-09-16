#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <stack>
using namespace std;
typedef chrono::high_resolution_clock Clock;
string TransformFileToString()
{
	ifstream in_file;
	in_file.open("test1.cpp");   
	if (!in_file.is_open())
	{
		cout << "open file error" << endl;
		return 0;
	}
	string string_in_file, temp;
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
	return string_in_file;
}
string HandleSigns(string string_in_file)
{	
	string file_to_string;
	for (int i = 0; i < string_in_file.length(); ++i)	
	{
		if(string_in_file[i] == '\"' )
		{
			++i;
			while(string_in_file[i++] != '\"'){}		
		}	
		else if(string_in_file[i] == '/' && string_in_file[i+1] == '*' )
			while(string_in_file[i] != '*' || string_in_file[i+1] != '/')
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
	return file_to_string;
}
int BranchPart(
	string temp,
	stringstream& ss,
	map<string, int> &reserved_word_maps,
	stack<string> &s,
	int& if_else_num,
	int& if_elseif_else_num)
{
	if(temp == "if" || temp == "{")
	{
		if(temp == "if")
			reserved_word_maps["if"]++;
		s.push(temp);
	}			
	else if(temp == "else") 
	{	
		reserved_word_maps["else"]++;
		ss >> temp;
		if(temp == "if")
		{
			reserved_word_maps["if"]++;
			s.push("elseif");
		}	
		else
		{	
			int flag = 0;
			for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
			{	
				if(it -> first == temp)
					reserved_word_maps[temp]++;
			}
			while(s.top() != "if")
			{
				if(s.top() == "elseif")
					flag = 1;
				s.pop();
			}
			s.pop();
			if(flag == 1)
				if_elseif_else_num++;
			else
				if_else_num++;
		}	
		if(temp == "{")
			s.push("{");	
	}
	else if(temp == "}")
	{
		while(s.top() != "{")
			s.pop();
		s.pop();	
	}
	else
		return 0;
	return 1;		
}
int SwitchPart(
	string temp,
	stringstream& ss,
	map<string, int> &reserved_word_maps,
	vector<int>& case_num,
	bool& switch_num_change )
{
	if(temp == "case")
	{	
		if(switch_num_change == 0)
		{
			int back = case_num.back();
			case_num.pop_back();
			case_num.push_back(++back);
		}
		else
		{
			case_num.push_back(1);
			switch_num_change = 0;
		}
		reserved_word_maps["case"]++;
	}
	else if(temp == "switch")
	{
		switch_num_change = 1;
		reserved_word_maps["switch"]++;
	}
	else
		return 0;
	return 1;	
}
void PrintResult(
	int total_num,
	int switch_num,
	vector<int> case_num,
	int if_else_num,
	int if_elseif_else_num,
	auto start_time)
{
	cout << "total num: " << total_num << endl;
 	cout << "switch num: " << switch_num << endl;
 	cout << "case num: "; 
 	for(int i = 0; i < switch_num; ++i)
 		cout << case_num[i] << ' ' ;
	cout << endl << "if-else num: " << if_else_num << endl;
	cout << "if-elseif-else num: " << if_elseif_else_num << endl;
 	auto end_time = Clock::now();
 	auto computing_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
	cout << "Computing time = " << computing_time.count() << " ns" << endl;
}
int main()
{
	auto start_time = Clock::now();
	string file_to_string = HandleSigns(TransformFileToString());
	stringstream ss(file_to_string);
	map<string, int> reserved_word_maps;
  	reserved_word_maps = {
    {"auto",0},		{"break",0},	{"case",0},		{"char",0},		{"const",0},
	{"continue",0},	{"default",0},	{"do",0},		{"double",0},	{"else",0},
	{"enum",0},		{"extern",0},	{"float",0},	{"for",0},		{"goto",0},
	{"if",0},		{"int",0},		{"long",0},		{"register",0},	{"return",0},
	{"short",0},	{"signed",0},	{"sizeof",0},	{"static",0},	{"struct",0},
	{"switch",0},	{"typedef",0},	{"union",0},	{"unsigned",0},	{"void",0},				 
    {"volatile",0},	{"while",0}
    };
    vector<int> case_num;
    bool switch_num_change = 1; 
	stack<string> s;
	int if_else_num = 0;
	int if_elseif_else_num = 0;
	string temp; 
	while (ss >> temp)
	{	
		if(BranchPart(temp, ss, reserved_word_maps, s, if_else_num, if_elseif_else_num))
			continue;
		else if(SwitchPart(temp, ss, reserved_word_maps, case_num, switch_num_change))
			continue;	
		else
		{
			for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
			{	
				if(it->first == temp)
					reserved_word_maps[temp]++;
			}
		}			
	}
 	int total_num = 0;
	for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)
		total_num += it->second;
	int switch_num = reserved_word_maps["switch"];
 	PrintResult(total_num, switch_num, case_num, if_else_num, if_elseif_else_num, start_time);
	return 0;
}
