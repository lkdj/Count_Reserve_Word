#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <chrono>
#include <stack>
#include <omp.h>
//#include <mpi.h>
using namespace std;
class SwitchPart{
	private:
		int switch_num;    // optimize argument
		int case_num;
		vector<int> case_array;
		bool switch_num_change;
	public:
		SwitchPart();
		int Count(string);
		int GetSwitchNum();
		int GetCaseNum();
		vector<int> GetCaseArray();
}; 
SwitchPart::SwitchPart()
{
	switch_num_change = 0;
	switch_num = 0;    
	case_num = 0;
	vector<int> case_array = {0};
}
int SwitchPart::GetCaseNum()
{
	return case_num;
}
int SwitchPart::GetSwitchNum()
{
	return switch_num;
}
vector<int> SwitchPart::GetCaseArray()
{
	return case_array;
}
int SwitchPart::Count(string temp)
{
	if(temp == "case")
	{	
		if(switch_num_change == 0)
		{
			int back = case_array.back();
			case_array.pop_back();
			case_array.push_back(++back);
		}
		else
		{
			case_array.push_back(1);
			switch_num_change = 0;
		}
		case_num++;
	}
	else if(temp == "switch")  // a new switch appear
	{
		switch_num_change = 1;
		switch_num++;
	}
	else
		return 0;
	return 1;
}
class BranchPart{
	private:
		int if_num;
		int else_num;
		stack<string> if_else_stack;
		int if_else_num;
		int if_elseif_else_num;
	public:
		BranchPart();
		int Count(string, stringstream&);
		int GetIfNum();
		int GetElseNum();
		int GetIfElseNum();
		int GetIfElseifElseNum();
}; 
BranchPart::BranchPart()
{
	if_num = 0;
	else_num = 0;
	if_else_num = 0;
	if_elseif_else_num = 0;
}
int BranchPart::GetIfNum(){
	return if_num;
}
int BranchPart::GetElseNum(){
	return else_num;
}
int BranchPart::GetIfElseNum(){
	return if_else_num;
}
int BranchPart::GetIfElseifElseNum(){
	return if_elseif_else_num;
}
int BranchPart::Count(string temp,stringstream& ss)
{
	if(temp == "if" || temp == "{")
	{
		if(temp == "if")
			if_num++;
		if_else_stack.push(temp);
	}			
	else if(temp == "else")  //else or elseif appear
	{	
		else_num++;
		ss >> temp;
		if(temp == "if")  //elseif appear
		{
			if_num++;
			if_else_stack.push("elseif");
		}	
		else  //else appear
		{	
			int flag = 0;
			while(if_else_stack.top() != "if")
			{
				if(if_else_stack.top() == "elseif")
					flag = 1;  
				if_else_stack.pop();
			}
			if_else_stack.pop();
			if(flag == 1)  //elseif has appeared
				if_elseif_else_num++;
			else
				if_else_num++;	
			if(temp == "{")
				if_else_stack.push("{");
			else 
				return 0;
		}		
	}
	else if(temp == "}")
	{
		while(if_else_stack.top() != "{")
			if_else_stack.pop();
		if_else_stack.pop();	
	}
	else
		return 0;
	return 1;		
}
typedef chrono::high_resolution_clock Clock;
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
void PrintResult(
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
 			cout << case_num[i] << ' ' ;
 		if(switch_num ==0 )
 			cout << '0';
	}
 	if(grade[0] > '2')
		cout << endl << "if-else num: " << if_else_num;
	if(grade[0] > '3')
		cout << endl << "if-elseif-else num: " << if_elseif_else_num << endl;
}
int main(int argc, char* argv[])
{
	auto start_time = Clock::now();
	string file_to_string = HandleSigns(TransformFileToString(argv[1]));
	stringstream ss(file_to_string);
	map<string, int> reserved_word_maps;
  	reserved_word_maps = {
    	{"auto",0},	{"break",0},	{"case",0},	{"char",0},	{"const",0},
	{"continue",0},	{"default",0},	{"do",0},	{"double",0},	{"else",0},
	{"enum",0},	{"extern",0},	{"float",0},	{"for",0},	{"goto",0},
	{"if",0},	{"int",0},	{"long",0},	{"register",0},	{"return",0},
	{"short",0},	{"signed",0},	{"sizeof",0},	{"static",0},	{"struct",0},
	{"switch",0},	{"typedef",0},	{"union",0},	{"unsigned",0},	{"void",0},				 
    	{"volatile",0},	{"while",0}
    };
	string temp; 
	int total_num = 0;
	SwitchPart count_switch_part;
	BranchPart count_branch_part;
	while (ss >> temp)
	{	
		if(count_branch_part.Count(temp, ss))
			continue;
		else if(count_switch_part.Count(temp))
			continue;	
		else
		{
			for (map<string, int>::iterator it = reserved_word_maps.begin(); it != reserved_word_maps.end(); it++)  //计算过程优化，本身不用遍历if,case,else,switch等但是为了表的完整性 
			{	
				if(it->first == temp)
					total_num++;
			}
		}			
	}
	total_num += (count_branch_part.GetIfNum() + count_branch_part.GetElseNum() + count_switch_part.GetSwitchNum() + count_switch_part.GetCaseNum());
 	PrintResult(total_num, count_switch_part.GetSwitchNum(), count_switch_part.GetCaseArray(), count_branch_part.GetIfElseNum(), count_branch_part.GetIfElseifElseNum(), argv[2]);
 	auto end_time = Clock::now();
 	auto computing_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
	cout << "Computing time = " << computing_time.count() << " μs" << endl;
	return 0;
}
