#include "character.h"
#include <vector>
#include <string>

using namespace std;


Character::Character()
{
	Num_Words = 0;
	Num_Lines = 0;
	Importance = 'b';
}
Character::Character(string name)
{
    Num_Words = 0;
    Num_Lines = 0;
    Name = name;
    Importance = 'b';
}
string Character::Get_Name()
{
	return Name;
}
vector <string> Character::Get_Lines()
{
	return Lines;
}

int Character::Get_Num_Lines()
{
	return Num_Lines;
}

int Character::Get_Num_Words()
{
    return Num_Words;
}

//changed according to character.h
void Character::Set_Name(string& This_Word)
{
	Name = This_Word;
}

//This function determines how many "lines" of the script belong
//to a piece of dialogue for a particular character.  It then returns the position
//in the script where we should be after attributing those lines.
int Character::Set_Next_Line(vector <string>& Script, int i)
{
	int j = i + 1;
	string This_Line = "";
	//We wish to look at the subsequent lines in the script.
	//The first test is so we don't step off the end of the script if we are at the end.
	//The second is testing to see if there is a blank line in the script
	//which signifies the end of the line as we see it.
	
	//THIS IS WHY IT WASN'T WORKING FOR 500 DAYS OF SUMMER
	//Instead of "empty" lines, the writer of the script put 10 spaces instead
	//Perhaps a better solution would be to write an is_blank test too determine
	//if the line contains no characters
	while (j < Script.size() && !is_blank(Script[j]))
	{
		This_Line = This_Line + Script[j];
		j++;
	}

	if (This_Line != "")
	{
		Lines.push_back(This_Line);
		
		// This was the only time I called Increment_Lines, and I think that the function
		// could be replaced with the one line of code it contains, to help condense even further.
		Num_Lines++;
	}
	//Because we increment before testing, we should decrement when we fail the test.
	j--;
	return j;
}

// While this does work, I only needed to call it once in th character class,
// and since it is only one line of code, I don't think we actually need a function for this,
// especially when the goal right now is to condense and simplify. I will leave it in for
// now, but I think we can probably just remove the function all together.
void Character::Increment_Lines()
{
	Num_Lines++;
}

void Character::Set_Num_Words()
{
	for (unsigned int i = 0; i < Lines.size(); i++)
    {
        for (unsigned int j = 0; j < Lines[i].size() - 1; j++)
        {
            if (Lines[i][j] == ' ' && Lines[i][j+1] != ' ')
            {
                Num_Words++;
            }
        }
        // moved this last increment here, so it would do it for every line the character spoke, not just the last one.
    	Num_Words++;
    }
    // ^^ was originally here.
}

//Sara says:  Let's consider testing a string for spaces, tabs, and newline characters.
bool Character::is_blank(string line)
{
	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line == "")
		{
			return false;
		}
		while (line.front() == ' ' || line.front() == '\t')
		{
			line.erase(0, 1);
			if (line == "")
			{
				return true;
			}
		}
		if (line != "")
		{
			return false;
		}
	}
	return true;
}
void Character::Add_Lines(vector<string> new_lines)
{
	for(int i = 0; i < new_lines.size(); i++)
	{
		Lines.push_back(new_lines[i]);
		Increment_Lines();
	}
}

void Character::Set_Importance(char c){
	Importance = c;
}

char Character::Get_Importance(){
	return Importance;
}




