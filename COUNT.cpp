#include <iostream>
#include <fstream>
#include <map>
#include <omp.h>
#include "COUNT.h"
SwitchPart::SwitchPart()
{
	switch_num_change_ = 0;
	switch_num_ = 0;    
	case_num_ = 0;
	vector<int> case_array_ = {0};
}
int SwitchPart::GetCase()
{
	return case_num_;
}
int SwitchPart::GetSwitch()
{
	return switch_num_;
}
vector<int> SwitchPart::GetArray()
{
	return case_array_;
}
int SwitchPart::Count(string temp)
{
	if(temp == "case")
	{	
		if(switch_num_change_ == 0)
		{
			int back = case_array_.back();
			case_array_.pop_back();
			case_array_.push_back(++back);
		}
		else
		{
			case_array_.push_back(1);
			switch_num_change_ = 0;
		}
		case_num_++;
	}
	else if(temp == "switch")  // a new switch appear
	{
		switch_num_change_ = 1;
		switch_num_++;
	}
	else
		return 0;
	return 1;
}
IfPart::IfPart()
{
	if_num_ = 0;
	else_num_ = 0;
	if_else_num_ = 0;
	if_elseif_else_num_ = 0;
}
int IfPart::GetIf(){
	return if_num_;
}
int IfPart::GetElse(){
	return else_num_;
}
int IfPart::GetIfElse(){
	return if_else_num_;
}
int IfPart::GetElseif(){
	return if_elseif_else_num_;
}
int IfPart::Count(string& temp,stringstream& ss)
{
	if(temp == "if" || temp == "{")
	{
		if(temp == "if")
			if_num_++;
		if_else_stack_.push(temp);
	}			
	else if(temp == "else")  //else or elseif appear
	{	
		else_num_++;
		ss >> temp;
		if(temp == "if")  //elseif appear
		{
			if_num_++;
			if_else_stack_.push("elseif");
		}	
		else  //else appear
		{	
			int flag = 0;
			while(if_else_stack_.top() != "if")
			{
				if(if_else_stack_.top() == "elseif")
					flag = 1;  
				if_else_stack_.pop();
			}
			if_else_stack_.pop();
			if(flag == 1)  //elseif has appeared
				if_elseif_else_num_++;
			else
				if_else_num_++;	
			if(temp == "{")
				if_else_stack_.push("{");
			else 
				return 0;
		}		
	}
	else if(temp == "}")
	{
		while(if_else_stack_.top() != "{")
			if_else_stack_.pop();
		if_else_stack_.pop();	
	}
	else
		return 0;
	return 1;		
}
string TransformFileToString(string file_position)
{
	ifstream in_file;
	in_file.open(file_position);   
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
		if(temp[i] == '/' && temp[i+1] == '/')  //no need to transform "//" kind annotation
			continue;
		string_in_file += temp;
		string_in_file.append(" ");  //add space between adjacent lines
	}
	in_file.close();
	return string_in_file;
}
string HandleSigns(string string_in_file)
{	
	string file_to_string;
	for (int i = 0; i < string_in_file.length(); ++i)	
	{
		if(string_in_file[i] == '\"' )  //no need to transform words in double quotation marks
		{
			++i;
			while(string_in_file[i++] != '\"');		
		}	
		else if(string_in_file[i] == '/' && string_in_file[i+1] == '*' )  //don't transform "/**/" kind annotation
			while(string_in_file[i] != '*' || string_in_file[i+1] != '/')
				++i;
		else if(string_in_file[i] == '{' || string_in_file[i] == '}')  //retain braces
		{
			file_to_string += ' ';
			file_to_string += string_in_file[i];
			file_to_string += ' ';
		}
		else if (ispunct(string_in_file[i]))  //transform punctuation into space
			file_to_string += ' ';
		else
			file_to_string += string_in_file[i];
	}
	return file_to_string;
}
void Print(
	int total_num,
	int switch_num, 
	vector<int> case_num,
	int if_else_num,
	int if_elseif_else_num,
	string grade)
{
	cout << "total num: " << total_num << endl;
	if(grade[0] > '1')
	{
		cout << "switch num: " << switch_num << endl << "case num: "; 
 		for(int i = 0; i < switch_num; ++i)
 			cout << case_num[i] << ' ';
 		if(switch_num ==0 )
 			cout << '0';
	}
 	if(grade[0] > '2')
		cout << endl << "if-else num: " << if_else_num;
	if(grade[0] > '3')
		cout << endl << "if-elseif-else num: " << if_elseif_else_num << endl;
}
void FunctionControl(
	string grade, 
	stringstream& string_temp, 
	map<string, int>& word_maps, 
	int& total_num, 
	SwitchPart& switch_part, 
	IfPart& if_part)
{	
	string temp;
	if(grade[0] == '1')
	{
		while (string_temp >> temp)
			for (map<string, int>::iterator it = word_maps.begin(); it != word_maps.end(); it++)  //still traverse "if" part to remain map's completeness 
			{	
				if(it -> first == temp)
					total_num++;
			}			
	}
	else if(grade[0] == '2')
	{
		while (string_temp >> temp)
		{	
			if(switch_part.Count(temp))
				continue;	
			else	
				for (map<string, int>::iterator it = word_maps.begin(); it != word_maps.end(); it++)  //still traverse "if" part to remain map's completeness 
				{	
					if(it -> first == temp)
						total_num++;
				}
		}			
	}
	else
	{
		while (string_temp >> temp)
		{	
			if(if_part.Count(temp, string_temp))
				continue;
			else if(switch_part.Count(temp))
				continue;	
			else
			{		
				for (map<string, int>::iterator it = word_maps.begin(); it != word_maps.end(); it++)  //still traverse "if" part to remain map's completeness 
				{	
					if(it->first == temp)
						total_num++;
				}
			}			
		}			
	}
}
int main(int argc, char* argv[])
{
	string file_to_string = HandleSigns(TransformFileToString(argv[1]));
	stringstream string_temp(file_to_string);
	map<string, int> word_maps = {
    	{"auto",0},	{"break",0},	{"case",0},	{"char",0},	{"const",0},
	{"continue",0},	{"default",0},	{"do",0},	{"double",0},	{"else",0},
	{"enum",0},	{"extern",0},	{"float",0},	{"for",0},	{"goto",0},
	{"if",0},	{"int",0},	{"long",0},	{"register",0},	{"return",0},
	{"short",0},	{"signed",0},	{"sizeof",0},	{"static",0},	{"struct",0},
	{"switch",0},	{"typedef",0},	{"union",0},	{"unsigned",0},	{"void",0},				 
    	{"volatile",0},	{"while",0}
    };
	int total_num = 0;
	SwitchPart switch_part;
	IfPart if_part;
	FunctionControl(argv[2], string_temp, word_maps, total_num, switch_part, if_part);
	total_num += (if_part.GetIf() + if_part.GetElse() + switch_part.GetSwitch() + switch_part.GetCase());
 	Print(total_num, switch_part.GetSwitch(), switch_part.GetArray(), 
	 	if_part.GetIfElse(), if_part.GetElseif(), argv[2]);
	return 0;
}
