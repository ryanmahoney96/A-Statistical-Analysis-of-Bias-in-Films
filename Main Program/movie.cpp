
#include "movie.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "Bad_File_Exception.h"

using namespace std;

Movie::Movie()
{
	Title = "";
	Genres.push_back("Unknown");
	Year = 1890;
	Num_Noms = 0;
	MIN_LINES = 0;
}

Movie::Movie(string File_Name)
{
	Set_Script(File_Name);
	Set_Title();
	
	//Set_Genres();
	
	//For now, comment out set cast as it has changed from a vector
	//of characters to a vector of actors.
	//Set_Cast();
	
	//Set_Director();
	
	//Set to the year of the earliest film.
	Year = 1890;
	MIN_LINES = 0;
}

//Script_File and Cast_List_file are the whole path and the files, input is where we get the female/male names, MIN_LINES is a constant global int set and passed in main
Movie::Movie(string Script_File, string Cast_List_File, string input_directory, string output_directory, const int minLines)
{
	skip_film = false;
	MIN_LINES = minLines;
	Set_Script(Script_File);
	Set_Title();
	
	string possible_results_file = output_directory + '/' + Title + "_Results.csv";
				
	if(does_it_exist(possible_results_file)){
		char choice;
		cout << "Results have already been compiled on this film." << endl;
		cout << "Would you like to recompile the results? (y/n): ";
		cin >> choice;
		
		while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
			cout << "Would you like to recompile the results? (y/n): ";
			cin >> choice;
		}
		
		if (choice == 'N' || choice == 'n'){
			cout<< "Skipping " + Title << endl;
			skip_film = true;
		}
	}
	
	if (skip_film == false){
		
		Set_Genres();
		Set_Cast(Cast_List_File);
			
		vector <Character> Unlinked_Characters;
		
		//consider prompting for the script type here
		
		Actors_To_Characters(Unlinked_Characters);
		
		Set_Unlinked_Characters(Unlinked_Characters);
		Sort_Characters(Cast, 0, Cast.size() - 1);
		
		Set_Cast_Genders(input_directory);
		
		Set_Cast_Importance();
		
		Set_Directors();
		
		Year = 1890;
		Set_Year();
		Num_Noms = 0;
		Set_Num_Noms();
	}
}

void Movie::Set_Cast(string File_Name)
{
	ifstream fin;
	
	/////////////////////////////////////////////////////////////////
	//All this is just for brett and his silly Mac OS -> Ryan: I added this so you don't have to go in and uncomment a lot each time you run
	#ifdef __APPLE__
		fin.open(File_Name);
	
        
        if (fin.is_open())
        {
            string First_Line;
            getline(fin, First_Line);
            
            if (First_Line[First_Line.size() - 1] == '\r')
            {
                reset_files(File_Name);
            }
        }
        else
    	{
    		cout << "There was an error in opening the Cast List file." << endl;
    		Bad_File_Exception ex2;
    		throw ex2;
    	}
        
        fin.close();
    #endif
    //////////////////////////////////////////////////////////////////

	//if the script file has already been formatted for human readability, we continue. Else: format the file
	bool formatted = false;
    
    fin.open(File_Name);
    
	if (fin.is_open())
	{
		
		//takes in the first line of the script file
		string This_Line;
		getline(fin, This_Line);

		//if the first line of the cast list reads: "Formatted"
		if (This_Line == "Formatted") {
			//cout << "The cast list has been formatted"<<endl;
			//The cast list has already been made 'human readable'
			getline(fin, This_Line);
			formatted = true;

			do {
				Actor Current_Actor;
				Current_Actor.set_name(This_Line);

				//Now set the character name.
				string Character_Name;
				getline(fin, Character_Name);
				for (unsigned int i = 0; i < Character_Name.size() - 1; i++)
				{
					if (Character_Name[i] == '\"' || Character_Name[i] == ',' && Character_Name != "")
					{
						Character_Name.erase(Character_Name.begin() + i);
					}
				}
				Current_Actor.set_IMDB_name(Character_Name);
				Cast.push_back(Current_Actor);
				
				getline(fin, This_Line);
				getline(fin, This_Line);

			} while (fin.good());
		}
		else {
			cout << "Not Formatted"<<endl;
			
			//The code here is to ignore leading whitespace.
			while(This_Line == "")
			{
			    getline(fin, This_Line);
			}

			//if the cast list is not yet human readable, we take note and format it down the line
			//Though this step is not necessary, it better standardizes each cast list
			do {
				Actor Current_Actor;
				string actor_name = "";
				for (unsigned int i = 0; i < This_Line.size(); i++)
				{
					if (This_Line[i] != '\t')
					{
						//cout<<This_Line[i];
						actor_name += This_Line[i];
					}
					else
					{
						break;
					}
				}
				
				Current_Actor.set_name(actor_name);

				//Now set the character name.
				string character_name;
				getline(fin, character_name);
				Current_Actor.set_IMDB_name(character_name);
				Cast.push_back(Current_Actor);

				getline(fin, This_Line);

			} while (fin.good());
		}

	}
	else
	{
		cout << "There was an error opening the Cast List file." << endl;
		Bad_File_Exception ex2;
		throw ex2;
	}

	//change the name of this ofstream file name for the different movies
	//save over the old cast list, 
	//-- "abitrary/file/path" + "user-input-file-name.txt"
	if (formatted == false)
	{
		//output to the Cast we were using
		ofstream characters(File_Name);

		for (unsigned int i = 0; i < Cast.size(); i++)
		{
			if (i == 0) {
				characters << "Formatted" << endl;
			}

			characters << Cast[i].get_name() << endl;

			characters << Cast[i].get_IMDB_name() << endl;

			characters << "\n";
		}

	}

	fin.close();
}

vector <Actor> Movie::Get_Cast()
{
	return Cast;
}

string Movie::Get_Title()
{
	return Title;
}
void Movie::Set_Title()
{
	int position = 0;
	if (Script.size() > 0)
	{
		//In case there is any leading blank lines in the script
		while(Script[position] == "")
		{
			position++;
		}
		Title = Script[position];
		
		//eliminate leading whitespace on the title
		while (isspace(Title[0]))
		{
			Title.erase(Title.begin());
		}
		//To eliminate quotation marks in titles.
		if(Title[0] == '\"')
		{
			Title.erase(Title.begin());
		}
		if(Title[Title.size() - 1] == '\"')
		{
			Title.pop_back();
		}
	}
	else
	{
		Title = "";
	}
}

void Movie::Set_Title(string T)
{
	Title = T;
}

vector <string> Movie::Get_Script()
{
	return Script;
}
void Movie::Set_Script(string File_Name)
{
	ifstream fin;
    /////////////////////////////////////////////////////////////////
    //All this is for Brett and his silly Mac OS -> Fixed to prevent crashes on windows - Ryan
    #ifdef __APPLE__
        fin.open(File_Name);

	    if (fin.is_open())
	    {
	        string First_Line;
	        getline(fin, First_Line);
	        
	        if (First_Line[First_Line.size() - 1] == '\r')
	        {
	            reset_files(File_Name);
	        }
	    }
	    //If the file did not open, alert the user.
		else
		{
			Bad_File_Exception No_File;
			throw No_File;
		}
	    /////////////////////////////////////////////////////////////////
	    fin.close();
    #endif
    
    fin.open(File_Name);
	//Check to see that the file did in fact open
	if (fin.is_open())
	{
		while (fin.good())
		{
			//takes in each LINE of the script
			string Current_Word;
			getline(fin, Current_Word);
			Script.push_back(Current_Word);
		}

		fin.close();
	}
	//If the file did not open, alert the user.
	else
	{
		Bad_File_Exception No_File;
		throw No_File;
	}
}

vector <string> Movie::Get_Genres()
{
	return Genres;
}
void Movie::Set_Genres()
{
	cout << "Please select a genre for " << Title << " from the following list." << endl;
	cout << "You WILL have an opportunity to enter more than 1 genre." << endl;
	
	vector <string> types = Movie_Types();
	int Response = -1;
	while (Response != 0 && types.size() > 0)
	{
		if (Response != -1)
		{
			cout << " 0) Finished entering genres" << endl;
		}
		for (unsigned int i = 0; i < types.size(); i++)
		{
			cout << setw(2) << i+1 << ") " << types[i] << endl;
		}
		cin >> Response;
		while (Response > types.size() && Response < 0)
		{
			cout << "Invalid response.  Please select a number less than " << types.size() << ":"; 
			cin >> Response;
		}

		//Don't try to erase something if they want to quit.
		if (Response > 0)
		{
			Genres.push_back(types[Response - 1]);
			types.erase(types.begin() + (Response-1));
		}
	}
}


void Movie::Sort_Characters(vector <Actor>& List, int left, int right) {

	int min = (left + right) / 2;

	int i = left;
	int j = right;
	int pivot = List[min].get_role().Get_Num_Lines();

	while (left < j || i < right) {
		while (List[i].get_role().Get_Num_Lines() > pivot){
			i++;
		}
		
		while (List[j].get_role().Get_Num_Lines() < pivot){
			j--;
		}

		if (i <= j) {
			Actor temp = List[j];
			List[j] = List[i];
			List[i] = temp;
			i++;
			j--;
		}
		else {
			if (left < j){
				Sort_Characters(List, left, j);
			}
			
			if (i < right){
				Sort_Characters(List, i, right);
			}
			
			return;
		}
	}
}

//Finds the characters according to the film and assigns their lines
vector <Character> Movie::The_Characters()
{
	vector <Character> Found_Characters;

	for (unsigned int i = 0; i < Script.size(); i++)
	{
		//This Means we have something
		//with all caps, spaces, symbols, etc.
		if (Contains_No_Lowercase(Script[i]) == true)
		{
			string New_Word;
			string This_Word = Script[i];
			
			for (unsigned int j = 0; j < This_Word.length(); j++)
			{
				//If we find a character name with parantheses in it, e.g. FORREST (V.O.),
				//we do not add it to the name.  This way, FORREST and FORREST (V.O.)
				//become the same character.
				if (This_Word[j] != '(')
				{
					New_Word += This_Word[j];
				}
				else
				{
					break;
				}
			}
			
			//Before using the back function, we want to make sure our New_Word
			//is not the empty string.
			if (New_Word == "")
			{
				continue;
			}
			//We also wish to ignore writing directions which are commonly misattributed
			//as characters, such as CONTINUE, FADE IN, etc.
			if (New_Word.find("CONTINUE") != string::npos)
			{
				continue;
			}

			// This loop removes the possibility that 'Anna', 'Anna  ', 'Anna:', 'Anna.', 'Anna: ' and so on are seen as separate characters.
			while (New_Word.size() > 0 && (New_Word.back() == ' '  || New_Word.back() == '\t' || New_Word.back() == '.' || New_Word.back() == ':'))
			{
				New_Word.pop_back();
			}
			
			while (New_Word.size() > 0 && ((New_Word.front() == ' ') || (New_Word.front() == '\t')))
			{
				New_Word.erase(0, 1);
			}
			
			if (New_Word.find("CUT TO") != string::npos || New_Word.find("FADE IN") != string::npos || New_Word.find("FADE OUT") != string::npos)
			{
				continue;
			}
			
			int is_vo = New_Word.find("\'S VOICE");
			if ( is_vo != string::npos)
			{
				New_Word.erase( is_vo, New_Word.back());
			}
			
			//I'm adding one more check for the empty word, just to be ultra safe.
			if(New_Word == "")
			{
				continue;
			}
		
			//If the character has been seen before, get its index in the cast vector.
			//If the character is new, this value will be -1.
		
			int Position = Index_Of(Found_Characters, New_Word);

			//Search for the index of words that have yet to be seen.
			//Number >= 0 implies it has already been seen.
			if (Position >= 0)
			{
				//We have now made it the job of Set_Next_Line to increment.
				//If this doesn't work, return to this code.
				
				//Found_Characters[Position].Increment_Lines();
				i = Found_Characters[Position].Set_Next_Line(Script, i);
			}
			else
			{
				Character Current_Character;
				Current_Character.Set_Name(New_Word);
				int Old_i = i;
				i = Current_Character.Set_Next_Line(Script, i);
				//Sneaky test to see if we've actually added something or not
				if (Old_i != i)
				{
					Found_Characters.push_back(Current_Character);
				}
			}
		}
	}
	
	for (int i = 0; i < Found_Characters.size(); i++)
    {
        Found_Characters[i].Set_Num_Words();
    }
    
    //This has to be changed
    //string input = "/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Input_Files";
    //compile_minor_characters(Found_Characters, input);

	return Found_Characters;
}

int Movie::Index_Of(vector <Character>& Cast_Of_Characters, string This_Member)
{
	//Default a negative number.  This is a flag to indicate we did not find the word This_Member
	int Ret_Val = -1;

	bool Been_Seen = false;
	for (unsigned int j = 0; j < Cast_Of_Characters.size(); j++)
	{
		if (This_Member == Cast_Of_Characters[j].Get_Name())
		{
			Ret_Val = j;
			break;
		}
	}

	return Ret_Val;
}



vector <string> Movie::Get_Directors()
{
	return Directors;
}

void Movie::Set_Directors()
{
	//cin.ignore(256, '\n');
	int Continue = 1;
	do
	{
		
	cin.ignore(256, '\n');

	string Director;
	cout << "Please enter the director's name: ";
	getline(cin, Director);
	Directors.push_back(Director);
	//cin.ignore(256, '\n');
	
	cout << "Would you like to add another director?" << endl;
	cout << "1) Yes"<< endl;
	cout << "2) No" << endl;
	cin >> Continue;
	
	while(Continue != 1 && Continue != 2)
	{
		cout << "Invalid Response. Please enter 1 for Yes and 2 for No:";
		cin >> Continue;
	}

	}while(Continue == 1);
}

int Movie::Get_Year()
{
	return Year;
}

void Movie::Set_Year()
{
	cout << "Please enter the year in which this movie was made: ";
	cin >> Year;
	while(Year < 1890)
	{
		cout << "That is not a valid year.  Please enter the year in which this movie was made (after 1889):";
		cin >> Year;
	}
}

void Movie::Print_Movie_Stats()
{
	cout << "Title: " << Title << endl;
	cout << "Genres:" << endl;
	for (unsigned int i = 0; i < Genres.size(); i++)
	{
		cout << i+1 << ") " << Genres[i] << endl;
	}
	cout << "Directed by: " << endl;
	for(unsigned int i =0; i < Directors.size();i++)
	{
		cout << Directors[i] << endl;
	}
	cout << "Filmed in " << Year << endl;
}

vector <string> Movie::Movie_Types()
{
	vector <string> types;

	//Types of movies according to IMSDB
	types.push_back("Action");
	types.push_back("Adventure");
	types.push_back("Animated");
	types.push_back("Comedy");
	types.push_back("Crime");
	types.push_back("Drama");
	types.push_back("Family");
	types.push_back("Fantasy");
	types.push_back("Horror");
	types.push_back("Musical");
	types.push_back("Mystery");
	types.push_back("Romance");
	types.push_back("Sci-Fi");
	types.push_back("Thriller");
	types.push_back("War");
	types.push_back("Western");

	return types;
}

void Movie::reset_files(string& path_to_file)
{
	//This is all for Brett
    ifstream fin;
    fin.open(path_to_file);
    
    vector<string> v;
    
    string This_Line;
    getline(fin, This_Line);
    
    do
    {
        if (This_Line.back() == '\r')
        {
            This_Line.pop_back();
        }
        v.push_back(This_Line);
        getline(fin, This_Line);
        
    }while (fin.good());
    
    string new_path = "/Users/Bgrimes/Desktop/Temp_File.txt";
    ofstream fout(new_path);
    
    for (int i = 0; i < v.size(); i++)
    {
        fout << v[i] << endl;
    }
    
    path_to_file = new_path;
}

//check to see if the word contains lowercase characters
bool Movie::Contains_No_Lowercase(string word)
{
	bool Ret_Val = true;

	//For now, I am stating that the empty word is NOT a no lowercase word.
	if (word == "")
	{
		Ret_Val = false;
	}
	else
	{
		for (unsigned int i = 0; i < word.size(); i++)
		{
			if ('a' <= word[i] && word[i] <= 'z')
			{
				Ret_Val = false;
				break;
			}
		}
	}

	return Ret_Val;
}

void Movie::Actors_To_Characters(vector <Character>& Unlinked_Characters)
{
	vector <Character> Cast_Of_Characters = The_Characters();
	
	//Loop through the characters looking for the associated actor.
	for(unsigned int k = 0; k < Cast_Of_Characters.size(); k++)
	{
	    //this is the size of the scripted name
	    int size = int(Cast_Of_Characters[k].Get_Name().size());
	    string script_name = decapitalize(Cast_Of_Characters[k].Get_Name());
	  
		//If the character speaks very little, we will not bother attempting to find an actor.
	    if (Cast_Of_Characters[k].Get_Num_Lines() < MIN_LINES)
	    {
	        continue;
	    }
	    
	    //This loop goes through the actors searching for an actor that fits the character.
	    for (unsigned int n = 0; n < Cast.size(); n++)
	    {
	    	if (Cast[n].get_role().Get_Num_Lines() > 0)
            {
                continue;
            }
	        // full name is the full character name (generally the script shortens the name)
	        string full_name = decapitalize(Cast[n].get_IMDB_name());
	        string test_name = "";
	        
	        //Search through the full name for a substring that matches the cast name. 
            //it looks like this loop looks at substrings of full_name of length size
            //starting from 0 to size up until the last possible substring of length size
	        for (int j = 0; j < int(full_name.size()) - size + 1; j++)
	        {
	        	
	        	//This creates the substring of full_name to test for equality with the script name
               for (unsigned int i = j; i < j + size; i++)
	            {
	                //creates the test name to be a substring of the full name of length "size"
	                test_name.push_back(full_name[i]);
	            }
	            
	            //Sara says: what about adjectives like teenaged or baby or old?  Is that a concern?
	            //this is the first five letters of the full name. If it says young, we don't want to attribute the "normal aged" actor to it
	            string first_five = full_name.substr(0, 5);
	           
	            // if test_name matches script_name but it isn't something like "Forrest's mom" or "young Forrest"
	            if (test_name == script_name && full_name[j+size] != '\'' && first_five != "young" )
	            {
	            	//Improve/generalize this test
	            	// maybe check for substring woman, and if true, then break?
	                if (test_name == "man" && full_name.size() != 3)
	                {
	                    //test to make sure it doesn't attribute "man" as "woman"
	                    break;
	                }
	                //cout << Actor_List[n].Get_Name() << endl;
	                Cast[n].set_role(Cast_Of_Characters[k]);
	                //return;
	                
	                //Sara is scared.  The point of this is to break out of two loops.
	                n = Cast.size();
	                j = full_name.size() - size + 1;
	            }
	            
	            //reset the test name
	            test_name = "";
	        }
	        
	        //This means we have been through the whole actor list and not found an actor
			if(n + 1 == Cast.size())
			{
	            //We will take care of these characters later
	            Unlinked_Characters.push_back(Cast_Of_Characters[k]);
	        }
	    }
	}
}

int Movie::Total_Lines()
{
	int Ret_Val = 0;
	for(unsigned int i = 0; i < Cast.size();i++)
	{
		//I'm hoping this if statement will be superfluous with our new formatting.
		//That is, if a member of the script isn't a character, we will not add
		//that character to the cast!  Yay improvement!
		if(Cast[i].get_gender() != 'n')
		{
			Ret_Val += Cast[i].get_role().Get_Num_Lines();
		}
	}
	return Ret_Val;
}

int Movie::Total_Words()
{
	int Ret_Val = 0;
	
	for (unsigned int i = 0; i < Cast.size(); i++)
	{
		if(Cast[i].get_gender() != 'n')
		{
			Ret_Val += Cast[i].get_role().Get_Num_Words();
		}
	}
	
	return Ret_Val;	
}

//if the word is in all caps, like most character names, we convert it to lower case using the ASCII values
string Movie::decapitalize(string word)
{
    for (unsigned int i = 0; i < word.size(); i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
        	//Decaptilizes
            word[i] = word[i] + 32;        
        }
    }
    return word;
}

void Movie::Set_Unlinked_Characters(vector <Character>& Unlinked_Characters)
{
    if (Unlinked_Characters.size() == 0)
    {
        cout << "Nice! We were able to assign an actor for every scripted character!" << endl;
    }
    else
    {
        cout << "We were unable to assign an actor for the following scripted characters: " << endl;
        for (unsigned int i = 0; i < Unlinked_Characters.size(); i++)
        {
            cout << Unlinked_Characters[i].Get_Name() << " (" << Unlinked_Characters[i].Get_Num_Lines() << " lines)" << endl;
        }
    }
    
    //If we have failed to find a character in the script for one of the top ten
    //actors according to IMDB, tell us about it.
    
	bool Someone_Missing = false;
	vector<int> top_indexes;

	//Make sure there are at least 10 characters before searching through 10 of them
	if (Cast.size() > 9)
	{
		for (unsigned int i = 0; i < 10; i++)
		{
			if (Cast[i].get_role().Get_Name() == "")
			{
				Someone_Missing = true;
			}
		}
		if (Someone_Missing == true)
		{
			cout << "Of the top ten billed actors and actresses, we were unable to find a script name for: " << endl;
			for (int i = 0; i < 10; i++)
			{
				if (Cast[i].get_role().Get_Name() == "")
				{
					cout << i + 1 << ": " << Cast[i].get_name() << " as " << Cast[i].get_IMDB_name() << endl;
					top_indexes.push_back(i);
				}
			}
		}
	}
	cout << endl;

	//A vector to keep track of the indices of the possible actors that could match the character.
 
    // We have to manually assign names for certain characters, but we can help the user
    while (Unlinked_Characters.size() > 0)
    {
	   vector <int> potential_matches = Potential_Matches(Unlinked_Characters[0].Get_Name());
	   
        cout << "Select the number that corresponds to the following Scripted Character's name on IMDB: "
        << Unlinked_Characters[0].Get_Name() << " : "<< Unlinked_Characters[0].Get_Num_Lines() << endl;
        
        for (unsigned int i = 0; i < potential_matches.size(); i++)
        {
        	//Do we want a plus 1?
            cout << potential_matches[i] + 1 << ": " << Cast[potential_matches[i]].get_IMDB_name()
            << " - " << Cast[potential_matches[i]].get_role().Get_Num_Lines() << " Lines " << endl;
        }
        
        if (potential_matches.size() == 0 || Unlinked_Characters[0].Get_Num_Lines() > 10)
        {
            for (int i = 0; i < top_indexes.size(); i++)
            {
                cout << top_indexes[i] + 1 << ": " << Cast[top_indexes[i]].get_IMDB_name() << endl;
            }
        }
        
		cout << "Please enter the character number or press:" << endl;
		cout << " 0) to assign a name" << endl;
		cout << "-1) to assign as NOT a character" << endl;
		cout << "-2) to swap characters" << endl;

		int choice = 0;
		cin >> choice;
		while (choice < -2)
		{
			cout << "Invalid entry.  Please enter the number, 0 to assign a name, -1 for not a character, or -2 to swap characters." << endl;
			cin >> choice;
		}
        /*cout << "Enter the character number, input 0 to assign a name, enter -1 to assign as 'Not a Character', or enter -2 to swap characters" << endl;
        
        cout << endl;*/
        
        if (Unlinked_Characters[0].Get_Num_Lines() > 25)
        {
            cout << "NOTE: This character has many lines, it may need to be swapped!" << endl;
        }
        
		for(int i = 0; i < top_indexes.size(); i++)
    	{
			potential_matches.push_back(top_indexes[i]);
        }
    	  
    	if (choice == -1){
    		Unlinked_Characters.erase(Unlinked_Characters.begin());
    		continue;
    	}
    	if ( choice == -2)
    	{
    		if(potentially_swap_characters(Unlinked_Characters[0]))
            {
                Unlinked_Characters.erase(Unlinked_Characters.begin());
                continue;
            }
            
            cout << "Select the number that corresponds to the following Scripted Character's name on IMDB: "
            << Unlinked_Characters[0].Get_Name() << " : "<< Unlinked_Characters[0].Get_Num_Lines() << endl;
            
            for (unsigned int i = 0; i < potential_matches.size(); i++)
            {
                //Do we want a plus 1?
                cout << potential_matches[i] + 1 << ": " << Cast[potential_matches[i]].get_IMDB_name() << " - "
                << Cast[potential_matches[i]].get_role().Get_Num_Lines() << endl;
            }
            
            cout << "Enter the character number, input 0 to assign a name" << endl;
            
            cin >> choice;
            
            while (choice < -1 )
            {
                cout << "That is not a valid input. Please enter again:" << endl;
                cin >> choice;
            }

    	}
       
    	//If they make a choice other than 0, validate that this is a possible choice
    	//Also validate that they CAN make a choice other than zero.
        else if (choice != 0 && potential_matches.size() > 0)
        {
        	//Let's make sure they entered a proper value.
    		bool Been_Found = false;
        	do{
	        	choice--;
	        	for(unsigned int k = 0; k < potential_matches.size();k++)
	        	{
	        		if(potential_matches[k] == choice)
	        		{
	        			Been_Found = true;
	        			if (Cast[choice].get_role().Get_Num_Lines() > 0)
                        {
                            Cast[choice].get_role().Add_Lines(Unlinked_Characters[0].Get_Lines());
                            break;
                        }
	        			Cast[choice].set_role(Unlinked_Characters[0]);
	        			break;
	        		}
	        	}
	        	if(Been_Found == false)
	        	{
	        		cout << "Sorry, that value is not an option.  Please enter a different choice:";
	        		cin >> choice;
	        	}
        	}while(Been_Found == false);
        	
            
        
        }
        else
        {
            cout << "Choose from the following:" << endl;
            cout << "1. Unidentified Male" << endl;
            cout << "2. Unidentified Female" << endl;
            cout << "3. Unidentified Neutral/Unknown" << endl;
            cout << "4. Not a Character" << endl;
            cout << "5. Add lines to a previously found character/actor" << endl;
            
            cin >> choice;
            while(choice > 5 || choice < 1)
            {
                cout << "Please enter a value 1-5:";
                cin >> choice;
            }
            
            //We need a new actor which we will assign a name and everything else
            Actor new_actor;
            
            if (choice == 1)
            {
                new_actor.set_name("Unidentified Male");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(Unlinked_Characters[0]);
            }
            if (choice == 2)
            {
                new_actor.set_name("Unidentified Female");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(Unlinked_Characters[0]);
            }
            if (choice == 3)
            {
                new_actor.set_name("Unidentified Neutral");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(Unlinked_Characters[0]);
            }
            if (choice == 4)
            {
                Unlinked_Characters.erase(Unlinked_Characters.begin());
                continue;
            }
            if (choice == 5)
            {
                Add_Lines_To_Character(Unlinked_Characters[0]);
            }
            if(choice !=5)
            {
            	Cast.push_back(new_actor);
            }
            
        }
        
        //potential_matches.erase(potential_matches.begin(), potential_matches.end());
        potential_matches.clear();
        
        //Get rid of the unfound actor, either we found it or we created a new one
        Unlinked_Characters.erase(Unlinked_Characters.begin());
    }
    
}

	
vector <int> Movie::Potential_Matches(string Character_Name)
{
	char Initial_Letter = Character_Name[0];
	Character_Name = decapitalize(Character_Name);
	vector <int> Ret_Val;
		
		//If we find a space in Character name there are two words in the name
		if(Character_Name.find(" ") != string::npos)
		{
			string first_name;
            string second_name;
            
            //The next bit of code breaks up the name into a first and second name.
            unsigned int n = 0;
            //add chars to the first_name until you get to the first space 
            while (Character_Name[n] != ' ')
            {
                first_name += Character_Name[n];
                n++;
            }
            
            //skip over the space
            n++;
            
            //add chars to second_name until the end of the name
            while (n < Character_Name.size())
            {
                second_name += Character_Name[n];
                n++;
            }
            
            //If we find part of the name in the Cast list, this is a 'potential' match.
            for(int i = 0; i < Cast.size(); i++)
            {
            	if(decapitalize(Cast[i].get_IMDB_name()).find(first_name) != string::npos || 
            	decapitalize(Cast[i].get_IMDB_name()).find(second_name) != string::npos)
            	{
            		Ret_Val.push_back(i);
            	}
            }
		}
        
        for(int i = 0; i < Cast.size(); i++)
    	{
    		if(decapitalize(Cast[i].get_IMDB_name()).find(Character_Name) != string::npos)
        	{
        	    Ret_Val.push_back(i);
        	}
		 }
        
        if (Ret_Val.size() == 0)
        {
            for (unsigned int i = 0; i < Cast.size(); i++)
            {
                //Tests to see if the first letters match
                if (Cast[i].get_role().Get_Num_Lines() != 0 && Cast[i].get_role().Get_Name() == "" && Cast[i].get_IMDB_name()[0] == Initial_Letter)
				{
					Ret_Val.push_back(i);
				}
			}
			
			for (unsigned int i = 0; i < Cast.size(); i++)
			{
				// This looks at the last letter
				if (Cast[i].get_role().Get_Num_Lines() != 0 && Cast[i].get_role().Get_Name() == "" && Cast[i].get_IMDB_name().back() == Character_Name.back())
				{
                    Ret_Val.push_back(i);
                }
            }
        }
		
	return Ret_Val;
}

void Movie::Add_Lines_To_Character(Character& c)
{
	
	char choice;
	cout << "Would you like to display ALL of the characters' IMDB names? (y/n): ";
	cin >> choice;

	while (choice != 'y' && choice != 'n') {
		cout << "Would you like to display ALL of the characters' IMDB names? (y/n): ";
		cin >> choice;
	}

	if (choice == 'y') {
		for (int i = 0; i < Cast.size(); i++) {
			cout << Cast[i].get_IMDB_name() << endl;
		}

	}
	
	cin.ignore(256,'\n');
	cout << "Which character would you like to add " << c.Get_Name() << "'s lines to? Please enter the IMDB name:" << endl;
	string user_name;
	getline(cin, user_name);
	
	bool Been_Found = false;
	
	do
	{
		for(int i = 0; i < Cast.size(); i++)
		{
			if(Cast[i].get_IMDB_name().find(user_name) != string::npos)
			{
				Been_Found = true;
				Cast[i].get_role().Add_Lines(c.Get_Lines());
				Cast[i].get_role().Set_Num_Words();
				return;
			}
		}
		cout << "Sorry, we couldn't find that character. Please enter again or type \"exit\":" << endl;
		cin.ignore(256,'\n');
		getline(cin, user_name);
		
	} while(Been_Found == false && user_name != "exit");	
}

void Movie::Set_Cast_Genders(string Directory)
{
	string Male_Name_Database = Directory + "/Modified_Male_Names.txt";
    string Female_Name_Database = Directory + "/Modified_Female_Names.txt";
    
    for (unsigned int i = 0; i < Cast.size(); i++)
    {
        //We don't care about people who don't speak
        if (Cast[i].get_role().Get_Num_Lines() > MIN_LINES)
        {
            Cast[i].set_gender(Get_Male_Database(Directory), Get_Female_Database(Directory), Male_Name_Database, Female_Name_Database);
        }
    }
}

void Movie::compile_minor_characters(vector<Character> all_characters, string Directory)
{
    Actor minor_males("Minor Males", 'm');
    Actor minor_females("Minor Females", 'f');
    
    Character all_males("Minor Males");
    
    Character all_females("Minor Females");
    
    minor_males.set_role(all_males);
    minor_females.set_role(all_females);
    
    vector<string> Male_Names = Get_Male_Database(Directory);
    vector<string> Female_Names = Get_Female_Database(Directory);
    
    
    
    for (int i = 0; i < all_characters.size(); i++)
    {
        bool found = false;
        
        if (all_characters[i].Get_Num_Lines() < 3 && all_characters[i].Get_Num_Lines() > 0)
        {
            string test_name = decapitalize(all_characters[i].Get_Name());
            
            for (int j = 0; j < Female_Names.size(); j++)
            {
                if (test_name == Female_Names[j] || test_name.find("woman") != string::npos || test_name.find("mrs.") != string::npos || test_name.find("miss") != string::npos)
                {
                    all_females.Add_Lines(all_characters[i].Get_Lines());
                    found = true;
                    break;
                }
            }
            if (found)
            {
                continue;
            }
            for (int j = 0; j < Male_Names.size(); j++)
            {
                if (test_name == Male_Names[j] || test_name.find("man") != string::npos)
                {
                    all_males.Add_Lines(all_characters[i].Get_Lines());
                    break;
                }
            }
        }
    }
    
    all_males.Set_Num_Words();
    all_females.Set_Num_Words();
    
    minor_males.set_role(all_males);
    minor_females.set_role(all_females);
    
    Cast.push_back(minor_males);
    Cast.push_back(minor_females);
}
vector<string> Movie::Get_Male_Database(string Directory)
{
    vector <string> Male_Names;
    
    string next_name;
    
    //Read in a list of common male names
    string Male_Name_Database = Directory + "/Modified_Male_Names.txt";
    
    
    ifstream fin;
    /////////////////////////////////////////////////////////////////
    //All this is just for brett and his silly Mac OS -> Ryan: I added this so you don't have to go in and uncomment a lot each time you run
#ifdef __APPLE__
    fin.open(Male_Name_Database);
    
    
    if (fin.is_open())
    {
        string First_Line;
        getline(fin, First_Line);
        
        if (First_Line[First_Line.size() - 1] == '\r')
        {
            reset_files(Male_Name_Database);
        }
    }
    else
    {
        cout << "There was an error in opening the Cast List file." << endl;
        Bad_File_Exception ex2;
        throw ex2;
    }
    
    fin.close();
#endif
    //////////////////////////////////////////////////////////////////
    
    fin.open(Male_Name_Database);
    
    getline(fin, next_name);
    Male_Names.push_back(decapitalize(next_name));
    
    while (fin.good())
    {
        getline(fin, next_name);
        Male_Names.push_back(decapitalize(next_name));
    }
    
    fin.close();
    
    return Male_Names;
}
vector<string> Movie::Get_Female_Database(string Directory)
{
    vector <string> Female_Names;
    string Female_Name_Database = Directory + "/Modified_Female_Names.txt";
    string next_name;
    //Now we do the same thing for the female names
    ifstream fin1;
    
    
#ifdef __APPLE__
    fin1.open(Female_Name_Database);
    
    if (fin1.is_open())
    {
        string First_Line;
        getline(fin1, First_Line);
        
        if (First_Line[First_Line.size() - 1] == '\r')
        {
            reset_files(Female_Name_Database);
        }
    }
    else
    {
        cout << "There was an error in opening the Cast List file." << endl;
        Bad_File_Exception ex2;
        throw ex2;
    }
    
    fin1.close();
#endif
    //////////////////////////////////////////////////////////////////
    
    fin1.open(Female_Name_Database);
    
    getline(fin1, next_name);
    Female_Names.push_back(decapitalize(next_name));
    
    while (fin1.good())
    {
        getline(fin1, next_name);
        Female_Names.push_back(decapitalize(next_name));
    }
    
    return Female_Names;
}

void Movie::Set_Cast_Importance()
{
	int sum_lines = 0;
	int sum_words = 0;
	
	for(int i = 0; i < 5; i++)
	{
		sum_lines += Cast[i].get_role().Get_Num_Lines();
	}
	
	float avg_lines = float(sum_lines) / 5;

	vector <int> Word_Counts;
	for(unsigned int i = 0; i < Cast.size(); i++)
	{
		Word_Counts.push_back(Cast[i].get_role().Get_Num_Words());
	}
	
	//sort the word counts
	for(unsigned int i = 0; i < Word_Counts.size() - 1; i++)
	{
		for(unsigned int j = i+1; j < Word_Counts.size(); j++)
		{
			if(Word_Counts[i] < Word_Counts[j])
			{
				swap(Word_Counts[i],Word_Counts[j]);
			}
		}
	}
	
	//Add the top five in terms of words
	for(int i = 0; i < 5; i++)
	{
		sum_words += Word_Counts[i];
	}
	
	float avg_words = float(sum_words)/5;
	
	for(int i = 0; i < Cast.size(); i++)
	{
		if(Cast[i].get_role().Get_Num_Lines() > avg_lines || Cast[i].get_role().Get_Num_Words() > avg_words)
		{
			Cast[i].get_role().Set_Importance('m');
		}
		else if(Cast[i].get_role().Get_Num_Lines() > 6)
		{
			Cast[i].get_role().Set_Importance('s');
		}
	}
}

inline bool Movie::does_it_exist(string filepath) {
	ifstream infile(filepath);
	return infile.good();
}

bool Movie::should_we_skip(){
	return skip_film;
}

    
int Movie::Get_Num_Noms()
{
	return Num_Noms;
}
void Movie::Set_Num_Noms()
{
	cout << "Please enter the number of Academy Award Nominations this movie received: ";
	cin >> Num_Noms;
	while(Num_Noms < 0)
	{
		cout << "Please enter a non-negative number of Academy Award Nominations: ";
		cin >> Num_Noms;
	}
}

bool Movie::potentially_swap_characters(Character c)
{
	vector <Actor*> potential_swaps;
    for(int i = 0; i < Cast.size(); i++)
    {
        if(decapitalize(Cast[i].get_IMDB_name()).find(decapitalize(c.Get_Name())) != string::npos)
        {
            potential_swaps.push_back(&Cast[i]);
        }
    }
    
    cout << "Which of the following is the correct actor/actress - character:" << endl;
    
    for(int j = 0; j < potential_swaps.size(); j++)
    {
        cout << j << " : " << potential_swaps[j]->get_name() << " - " << potential_swaps[j]->get_IMDB_name() << endl;
    }
    
    cout << "Enter the corresponding number to swap or enter -1 to exit the swap_character option: " << endl;
    
    int choice;
    cin >> choice;
    
    while (choice < -1 || choice > potential_swaps.size() - 1)
    {
        cout << "That is not valid. Please enter again:" << endl;
    }
    
    if (choice == -1)
    {
        return false;
    }
    
    cout << "You have selected " << potential_swaps[choice]->get_name() << " - " << potential_swaps[choice]->get_IMDB_name() << endl;
    
    cout << "The scripted name for " <<  potential_swaps[choice]->get_IMDB_name() << " is " << potential_swaps[choice]->get_role().Get_Name() << endl;
    
    cout << "Would you like to swap this character with " << c.Get_Name() << "? (y/n)";
    
    string choice2;
    cin >> choice2;
    
    while (choice2 != "y" && choice2 != "n")
    {
        cout << "That is not a valid option. Please enter again:" << endl;
        cin >> choice2;
    }
    
    if (choice2 == "y")
    {
        Character temp_character = potential_swaps[choice]->get_role();
        potential_swaps[choice]->set_role(c);
        
        cout << "Now we need to assign an actor to " << temp_character.Get_Name() << endl;
        
        vector <int> potential_matches = Potential_Matches(temp_character.Get_Name());
        
        cout << "Select the number that corresponds to the following Scripted Character's name on IMDB: "
        << temp_character.Get_Name() << " : "<< temp_character.Get_Num_Lines() << endl;
        
        for (unsigned int i = 0; i < potential_matches.size(); i++)
        {
            //Do we want a plus 1?
            cout << potential_matches[i] + 1 << ": " << Cast[potential_matches[i]].get_IMDB_name()
            << " - " << Cast[potential_matches[i]].get_role().Get_Num_Lines() << " Lines " << endl;
        }
        
        cout << "Enter the character number, input 0 to assign a name, or enter -1 to assign as 'Not a Character'" << endl;
        int choice3;
        cin >> choice3;
        
        if (choice3 == -1)
        {
            return true;
        }
        
        if ( choice3 == -2)
        {
            potentially_swap_characters(temp_character);
        }
        
        //If they make a choice other than 0, validate that this is a possible choice
        //Also validate that they CAN make a choice other than zero.
        else if (choice3 != 0 && potential_matches.size() > 0)
        {
            //Let's make sure they entered a proper value.
            bool Been_Found = false;
            do{
                choice3--;
                for(unsigned int k = 0; k < potential_matches.size();k++)
                {
                    if(potential_matches[k] == choice3)
                    {
                        Been_Found = true;
                        if (Cast[choice3].get_role().Get_Num_Lines() > 0)
                        {
                            Cast[choice3].get_role().Add_Lines(temp_character.Get_Lines());
                            return true;
                        }
                        Cast[choice3].set_role(temp_character);
                        return true;
                    }
                }
                if(Been_Found == false)
                {
                    cout << "Sorry, that value is not an option.  Please enter a different choice:";
                    cin >> choice;
                }
            }while(Been_Found == false);
        }
        else
        {
            cout << "Choose from the following:" << endl;
            cout << "1. Unidentified Male" << endl;
            cout << "2. Unidentified Female" << endl;
            cout << "3. Unidentified Neutral/Unknown" << endl;
            cout << "4. Not a Character" << endl;
            cout << "5. Add lines to a previously found character/actor" << endl;
            
            int choice4;
            cin >> choice4;
            while(choice4 > 5 || choice4 < 1)
            {
                cout << "Please enter a value 1-5:";
                cin >> choice;
            }
            
            //We need a new actor which we will assign a name and everything else
            Actor new_actor;
            
            if (choice4 == 1)
            {
                new_actor.set_name("Unidentified Male");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(temp_character);
                            }
            if (choice4 == 2)
            {
                new_actor.set_name("Unidentified Female");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(temp_character);
            }
            if (choice4 == 3)
            {
                new_actor.set_name("Unidentified Neutral");
                new_actor.set_IMDB_name("N/A");
                new_actor.set_role(temp_character);
            }
            if (choice4 == 4)
            {
                return true;
            }
            if (choice4 == 5)
            {
                Add_Lines_To_Character(temp_character);
                return true;
            }
            if(choice4 != 5)
            {
                Cast.push_back(new_actor);
                return true;
            }
        }
    }
    else
    {
        return false;
    }
    
    return true;
}



