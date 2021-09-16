#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
int main()
{	
	ifstream in_file;
	in_file.open("test1.cpp");   
	if (!in_file.is_open())
	{
		cout << "open file error" << endl;
		return 0;
	}
	string file_to_string, string_in_file, temp;
	while (getline(in_file, temp))
	{	
		string_in_file += temp;
		string_in_file.append(" ");
	}
	in_file.close();
	for (int i = 0; i < string_in_file.length(); i++)  //将除花括号外的标点符号转换成空格 
	{
		if(string_in_file[i] == '{' || string_in_file[i] == '}')
		{
			file_to_string += ' ';
			file_to_string += string_in_file[i];
		}
		else if (ispunct(string_in_file[i]))  //不可全部换成空格 
			file_to_string += ' ';
		else
			file_to_string += string_in_file[i];			
	}
	stringstream ss(file_to_string);
	stack<string> s;
	int if_else_num = 0;
	int if_elseif_else_num = 0;
	while(ss>>temp)
	{
		if(temp == "if"||temp == "{")
			s.push(temp);		
		else if(temp == "else") 
		{	
			ss >> temp;
			if(temp == "if")
				s.push("elseif");
			else
			{	
				int flag = 0;
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
			while(s.top()!="{")
				s.pop();
			s.pop();	
		}
	}
	cout << "if-else num: " << if_else_num << endl;
	cout << "if-elseif-else num: " << if_elseif_else_num << endl;
	return 0;	
}
