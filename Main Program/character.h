#pragma once

#include <vector>
#include <string>

using namespace std;

class Character
{
public:
	//Default Constructor.
	Character();
	//We may possibly overload this constructor.
	Character(string name);
	//Getters
	int Get_Num_Words();
	int Get_Num_Lines();
	string Get_Name();
	vector <string> Get_Lines();
	char Get_Importance();

	//Setters
	void Set_Name(string& This_Word);
	int Set_Next_Line(vector <string>& Script, int i);

	//A function to add one to the line count.
	void Increment_Lines();
	//This function determines the number of words once all lines have been set
	//for a character.
	void Set_Num_Words();

	//By default 'b' for Bit character; 's' for supporting character; 'm' for main character
	void Set_Importance(char c);
	
	//Helper functions - these may change as we move things to/from this class.
	bool is_blank(string line);


	void Add_Lines(vector<string> new_lines);

private:
	// Name changed from word to string
	string Name;
	vector <string> Lines;
	//gender moved to actor
	int Num_Words;
	//Added Num_Lines
	int Num_Lines;
	char Importance;
};

