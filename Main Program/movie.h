#pragma once

#include<string>
#include<vector>
#include"character.h"
#include "actor.h"

class Movie
{
public:

	Movie();
	//Note: Change this constructor once set cast has been edited.
	Movie(string File_Name);

	Movie(string Script_File, string Cast_List_File, string input_directory, string output_directory, const int minLines);

	string Get_Title();
	void Set_Title();
	void Set_Title(string T);

	vector <string> Get_Script();
	void Set_Script(string File_Name);
	
	void Set_Cast_Genders(string input_directory);

	vector <string> Get_Genres();
	void Set_Genres();

	vector <string> Get_Directors();
	void Set_Directors();

	int Get_Year();
	void Set_Year();

	//This function prints the title, genres, year, and director of a film. 
	//It does not print the cast as that is a long process.
	void Print_Movie_Stats();
	
	//A function for Brett to help his Mac
	void reset_files(string& path_to_file);

	//I made this function a static function in case we ever want to call it outside
	//of a particular movie.  For example, if we want to use it to only look at data
	//gathered about a particular type of movie.
	static vector <string> Movie_Types();
	
	//This reads in the Cast_List file for  a movie and sets the IMDB info.
	//That is, it creates the vector of actors for the film,
	//setting the actor's name and IMDB name.
	void Set_Cast(string File_Name);
	vector <Actor> Get_Cast();
	
	//A helper function for The_Characters 	
	static bool Contains_No_Lowercase(string word);

	//A helper function.  If a character has already been identified
	//and is speaking again, determined the index of the character in the cast list.
	int Index_Of(vector <Character>& Cast_Of_Characters, string This_Member);

	//A function that gets the characters and attributes their lines to them
	//all at once.
	vector <Character> The_Characters();

	//This sorts the actors according to the number of lines of their characters. 
	//I would like to change this to Sort_Cast after receiving permission.
	void Sort_Characters(vector <Actor>& List, int left, int right);
	
	//I would like to add the functions that link the IMDB files with the script
	//to the movie class.  
	void Actors_To_Characters(vector <Character>& Unlinked_Characters);
	
	//A function that determines the total number of lines in the script.  
	int Total_Lines();
	
	//A function that determines the total number of words in the script. 
	int Total_Words();
	
	string decapitalize(string word);
	
	void Set_Unlinked_Characters(vector <Character>& Unlinked_Characters);
	
	vector <int> Potential_Matches(string Character_Name);
	
	void Add_Lines_To_Character(Character& c);
	
	vector<string> Get_Male_Database(string Directory);
    vector<string> Get_Female_Database(string Directory);
	
	void compile_minor_characters(vector<Character> all_characters, string Directory);
    
    void Set_Cast_Importance();
    
    bool should_we_skip();
    
    int Get_Num_Noms();
    void Set_Num_Noms();
    
    bool potentially_swap_characters(Character c);
    
private:

	string Title;
	vector <string> Script;
	vector<string> Genres;
	//This has been changed from a vector of Characters
	vector <Actor> Cast;
	vector <string> Directors;
	int Year;
	inline bool does_it_exist(string filepath);
	bool skip_film;
	int Num_Noms;
	int MIN_LINES;
};


