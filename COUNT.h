#ifndef COUNT_H
#define COUNT_H
#include <string>
#include <sstream>
#include <vector>
#include <stack>
using namespace std;
class SwitchPart{
	private:
		int switch_num_;    // optimize argument
		int case_num_;
		vector<int> case_array_;
		bool switch_num_change_;
	public:
		SwitchPart();
		int Count(string);
		int GetSwitch();
		int GetCase();
		vector<int> GetArray();
};
class IfPart{
	private:
		int if_num_;
		int else_num_;
		stack<string> if_else_stack_;
		int if_else_num_;
		int if_elseif_else_num_;
	public:
		IfPart();
		int Count(string, stringstream&);
		int GetIf();
		int GetElse();
		int GetIfElse();
		int GetElseif();
}; 
string TransformFileToString(string file_position);
string HandleSigns(string string_in_file);
void Print(
	int total_num,
	int switch_num, 
	vector<int> case_num,
	int if_else_num,
	int if_elseif_else_num,
	string grade);
	

#endif
