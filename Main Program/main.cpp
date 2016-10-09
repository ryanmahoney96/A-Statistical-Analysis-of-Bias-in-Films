#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <ctime>
#include <time.h>
#include "Bad_File_Exception.h"
#include "Mismatched_File_Exception.h"
#include "character.h"
#include "movie.h"
#include "actor.h"

/*

#include <iostream>
#include <time.h>

using namespace std;

int main() {

	srand(time(0));

	int genres [] = { 118, 1837, 358, 52, 664 };

	//animation 118
	//drama 1837
	//mystery 358
	//western 52
	//crime 664

	for (int i = 1; i < 26; i++) {
		cout << (rand() % genres[(i - 1) / 5]) + 2 << endl;

		if (i % 5 == 0) {
			cout << "--------------------" << endl;
		}
	}


	return 0;
}
*/

using namespace std;

//a function used to find files within a directory
#ifdef _WIN32
	#include <io.h>
	#include <stdio.h>
	void find_files(string parent_directory, vector <string>& file_names);
#endif

#ifdef __APPLE__
	#include <dirent.h>
	void reset_files(string& path_to_file);
	void find_files2(string parent_directory, vector <string>& file_names);
#endif

vector <string> directory_selector();

//06-16-16 Setting some tasks in main to functions
//vector <Actor>& Get_Cast_List(ifstream& fin, string File_Name);

void Compute_Gender_Stats(string File_Name, vector <Actor>& Actor_Roles);
void Unfound_Gender_Prompt(vector <Character>& Characters_In_Film);

void out_results(string Results_File, Movie& This_Movie, string movie_name, bool& Is_First, int& choice);

void individual_out(string Results_File, Movie& This_Movie);

void Sort_By_Words(vector <float>& List, int left, int right);

void display_options(int& choice);

// Integer which sets the cut off point for minimum number of lines spoken in the film
//we may be able to make this a less arbitrary number -> we could cut it off at 10% of the lines, for example
const int MIN_LINES = 2;


int main()
{
	cout << "A Statistical Analysis of Bias in Film" << endl;

	//used to open our cast list and script files
	ifstream fin;

	//holds every line of the film's script as an entry
	vector <string> Script;
	//holds every character in a particular film
	vector <Actor> Cast_List;

	//this will be filled with every file name within a directory after the find_files funciton is called
	vector <string> All_Cast_Lists;
	vector <string> All_Scripts;

	//This is helpful for outputting the results
	bool Is_First = true;
	//bool Is_Debugging = false;
	int choice = 0;
	//right now, if one of the files fails to open we throw an exception and exit
	try {

		//fill these in with the dropbox file paths prior to a run on your machine
		string cast_list_directory = "";
		string script_directory = "";
		string input_directory = "";
		string output_directory = "";

		//this allows us to select our file directories quickly and easily
		vector <string> directories = directory_selector();
		cast_list_directory = directories[0];
		script_directory = directories[1];
		input_directory = directories[2];
		output_directory = directories[3];
		string total_results = output_directory + "/Results.csv";


		//finds the files in the given directories
#ifdef _WIN32
		find_files(cast_list_directory, All_Cast_Lists);
		find_files(script_directory, All_Scripts);
#endif

#ifdef __APPLE__
		find_files2(cast_list_directory, All_Cast_Lists);
		find_files2(script_directory, All_Scripts);
#endif

		char list_titles = 0;
		cout << "Would you like to display the movie names with their indices? (y/n): ";
		cin >> list_titles;

		while (list_titles != 'y' && list_titles != 'Y' && list_titles != 'n' && list_titles != 'N'){
			cout << "Invalid response, enter (y) or (n):";
			cin >> list_titles;
		}

		if (list_titles == 'y' || list_titles == 'Y'){
			for (int it = 0; it < All_Scripts.size(); it++){
				cout << it << ". " << All_Scripts[it] << endl;
			}
		}

		//This bit of code allows us to jump to a position/file in the array of scripts.
		int My_Start = 0;
		int My_End = All_Scripts.size() - 1;

		cout << "Please enter the position in the array where you wish to begin: ";
		cin >> My_Start;

		while (My_Start < 0 || My_Start > All_Scripts.size())
		{
			cout << "Invalid entry.  Please enter a value greater than 0 and less than "
				<< All_Scripts.size() << ": ";
			cin >> My_Start;
		}

		cout << "Please enter the position in the array where you wish to end searching: ";
		cin >> My_End;

		while (My_End < My_Start || My_End > All_Scripts.size())
		{
			if (My_End > All_Scripts.size() - 1){
				cout << "Defaulting end of search to " << All_Scripts.size() << endl;
				My_End = All_Scripts.size();
			}
			else {
				cout << "Invalid entry.  Please enter a value greater than " << My_Start << " and less than " << All_Scripts.size() << ": ";
				cin >> My_End;
			}
		}

		if (All_Cast_Lists.size() != All_Scripts.size()){
			cout << "There are not the same number of Cast Lists in " << cast_list_directory << " as there are Scripts in " << script_directory << endl;
			Mismatched_File_Exception mfe1;
			throw mfe1;
		}


		else {
			//for (unsigned int file_it = 0; file_it < All_Cast_Lists.size() && file_it < All_Scripts.size(); file_it++)
			for (unsigned int file_it = My_Start; file_it < My_End; file_it++)
			{
				
				Script.clear();
				Cast_List.clear();

				//Tests to see if the files are named the same thing with the exception of 
				//"Cast" at the end of the cast list file.
				string cast_test = All_Cast_Lists[file_it].substr(0, All_Cast_Lists[file_it].size() - 8);
				string script_test = All_Scripts[file_it].substr(0, All_Scripts[file_it].size() - 4);

				//testing the file names against these abbrev.
				//cout << cast_test << " - " << All_Cast_Lists[file_it] << endl;
				//cout<<script_test << " - " << All_Scripts[file_it] <<endl;

				if (cast_test != script_test){
					cout << "The Cast Lists and Scripts are not aligned properly in their folders." << endl;
					Mismatched_File_Exception mfe2;
					throw mfe2;
				}
				else {
					string Cast_List_File = cast_list_directory + "/" + All_Cast_Lists[file_it];

					//This sets the actor to character association from the IMDB file
					//06-22-16:This function is now a part of the movie class.
					//Set_Cast_List(fin, Cast_List_File, Cast_List);
					//Cast_List = Get_Cast_List(fin,Cast_List_File);


					//The section below will allow the user to type in a script name within a preset folder
					/*string parent_directory = "arbitrary/file/path/"
					string Script_File;
					cout << "What is the name of the Script?"<<endl;
					cin >> Script_File;
					*/

					string Script_File = script_directory + "/" + All_Scripts[file_it];

					Movie This_Movie(Script_File, Cast_List_File, input_directory, output_directory, MIN_LINES);
					
					if(!This_Movie.should_we_skip()){
						vector <Character> Unfound_Actor;

						vector <Actor> Characters_According_To_Film = This_Movie.Get_Cast();
	
						//Set the actors
						/*for (unsigned int i = 0; i < Characters_According_To_Film.size(); i++)
						{
						// Characters_According_To_Film[i].Get_Name().Print_Word();
						Set_Actor(Characters_According_To_Film[i], Cast_List, Unfound_Actor);
						}*/
	
						//A little printing mechanism to update the console.  Not important.
						cout << This_Movie.Get_Title() << " contains " << Characters_According_To_Film.size() << " characters" << endl << endl;
	
						//Brett says: we shouldn't need this Unfound_Gender_Prompt function, I believe the Set_Actor_Gender function should take care of all gender assignments
						//Unfound_Gender_Prompt(Characters_According_To_Film);
	
						string results_file = output_directory + "/";
						results_file += All_Scripts[file_it].substr(0, All_Scripts[file_it].size() - 4);
						results_file += "_Results.txt";
	
						Compute_Gender_Stats(results_file, Characters_According_To_Film);
	
						//cout<<All_Scripts[file_it];
	
						out_results(total_results, This_Movie, All_Scripts[file_it], Is_First, choice);
					}

				}
			}
		}
	}

	catch (Bad_File_Exception bfe){
		cout << "Please ensure that the file exists and that its name in the program matches the one in memory." << endl;
	}
	catch (Mismatched_File_Exception mfe){
		cout << "Please find the missing/misaligned file(s) and try again." << endl;
	}
	/*
	//this is a general catch block, which we should avoid putting in until final product
	catch {
	cout<<"An Unknown Error Occurred."
	}
	*/

	system("pause");
	return 0;
}


#ifdef _WIN32
//this function can be used to look through directories. 
//It currently finds all files in the directory that match the .txt extension and stores their names
void find_files(string parent_directory, vector <string>& file_names) {

	//creates a finddata object, which holds the information we need about the file
	struct _finddata_t txt_file;
	//a search handle, which is filled with the files that match our search in _findfirst and _findnext
	long hFile;

	string s_file_path = parent_directory + "/" + "*.txt";
	const char * c_file_path = s_file_path.c_str();

	//here we will store all of the file names that match our search. If we need more information about the files later down the line
	//we change string -> _finddata_t
	//vector <string> file_names;

	//we can define our parent directory in _findfirst (i.e.  _findfirst("C:/Users/Ryangigs/Documents/*.txt", &txt_file) )
	//otherwise it defaults to the current working directory
	//C:/Users/Ryangigs/Documents/

	//finds the first instance of a file that matches our parameters and stores it in txt_file
	hFile = _findfirst(c_file_path, &txt_file);
	//cout<<txt_file.name<<endl;

	//if hFile returns a -1, that means an error occurred. We assume this means there were no files in the directory that mathed the search
	// more information can be gathered from ->  https://msdn.microsoft.com/en-us/library/aa246869(v=vs.60).aspx
	if (hFile == -1L) {
		cout << "No *.txt files in directory: " << parent_directory << endl;
	}
	else {
		//while we can still find files that match our search parameters, add their names to the vector
		do {
			file_names.push_back(txt_file.name);
		} while (_findnext(hFile, &txt_file) == 0);

		//when no more files match the search, _findnext returns -1. 
		//Other return types can be found at https://msdn.microsoft.com/en-us/library/6tkkkc1y.aspx

		//Closes the specified search handle and releases associated resources.
		_findclose(hFile);
	}

	//print the names of the files, or use them to open the script/cast list files in an automated function
	/*for (unsigned int i = 0; i < file_names.size(); i++) {
	file_names[i].erase(file_names[i].end() - 4, file_names[i].end());
	cout << file_names[i] << endl;
	}*/

}
#endif

#ifdef __APPLE__

void reset_files(string& path_to_file)
{
	ifstream fin;
	fin.open(path_to_file);

	vector<string> v;

	string This_Line;
	getline(fin, This_Line);

	do
	{

		This_Line.pop_back();
		v.push_back(This_Line);
		getline(fin, This_Line);

	} while (fin.good());

	string new_path = "/Users/Bgrimes/Desktop/Temp_File.txt";
	ofstream fout(new_path);

	for (int i = 0; i < v.size(); i++)
	{
		fout << v[i] << endl;
	}

	path_to_file = new_path;
}

void find_files2(string parent_directory, vector <string>& file_names) {

	dirent *pDirent;
	DIR *pDir;

	pDir = opendir(parent_directory.c_str());

	if (pDir == NULL) {
		cout << "Cannot open directory '%s'\n" << endl;
	}

	while ((pDirent = readdir(pDir)) != NULL)
	{
		string test(pDirent->d_name);
		if (test != "." && test != ".." && test != ".DS_Store")
		{
			file_names.push_back(test);
		}
	}

	for (unsigned int i = 0; i < file_names.size(); i++) {
		cout << file_names[i] << endl;
	}

	closedir(pDir);
}
#endif

vector <string> directory_selector() {

	int individual;
	vector <string> file_paths;

	cout << "1) Sara" << endl;
	cout << "2) Zach" << endl;
	cout << "3) Brett" << endl;
	cout << "4) Ryan" << endl;
	cout << "5) Exit" << endl;

	cout << "Who are you? (1 - 4): ";
	cin >> individual;

	if (individual == 5) {
		cout << "Exiting" << endl;
		exit(0);
	}

	while (individual < 1 && individual > 4) {
		cout << "Who are you? (1 - 4): ";
		cin >> individual;
	}

	if (individual == 1) {
		file_paths.push_back("C:/Users/sjensen/Dropbox/SURE 2016 Team Folder/Cast_Lists");
		file_paths.push_back("C:/Users/sjensen/Dropbox/SURE 2016 Team Folder/Scripts");
		file_paths.push_back("C:/Users/sjensen/Dropbox/SURE 2016 Team Folder/Input_Files");
		file_paths.push_back("C:/Users/sjensen/Dropbox/SURE 2016 Team Folder/Output_Files");
	}
	else if (individual == 2) {
		file_paths.push_back("C:/Users/zach erickson/Dropbox/SURE 2016 Team Folder/Cast_Lists/");
		file_paths.push_back("C:/Users/zach erickson/Dropbox/SURE 2016 Team Folder/Scripts/");
		file_paths.push_back("C:/Users/zach erickson/Dropbox/SURE 2016 Team Folder/Input_Files");
		file_paths.push_back("C:/Users/zach erickson/Dropbox/SURE 2016 Team Folder/Output_Files");
	}
	else if (individual == 3) {
		file_paths.push_back("/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Cast_Lists");
		file_paths.push_back("/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Scripts");
		file_paths.push_back("/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Input_Files");
		file_paths.push_back("/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Output_Files");
	}
	else {
		file_paths.push_back("C:/Users/Ryangigs/Dropbox/SURE_2016_Team_Folder/Cast_Lists");
		file_paths.push_back("C:/Users/Ryangigs/Dropbox/SURE_2016_Team_Folder/Scripts");
		file_paths.push_back("C:/Users/Ryangigs/Dropbox/SURE_2016_Team_Folder/Input_Files");
		file_paths.push_back("C:/Users/Ryangigs/Dropbox/SURE_2016_Team_Folder/Output_Files");
	}

	return file_paths;
}


void Compute_Gender_Stats(string File_Name, vector <Actor>& Actor_Roles)
{
	//These variables count the number of lines spoken by each type of character//
	int Female_Count_Freq = 0;
	int Male_Count_Freq = 0;
	int Undetermined_Freq = 0;
	int NA_Freq = 0;

	//export the results to a .txt file
	ofstream fout(File_Name);

	for (unsigned int i = 0; i < Actor_Roles.size(); i++)
	{
		if (Actor_Roles[i].get_gender() == 'F' || Actor_Roles[i].get_gender() == 'f')
		{
			Female_Count_Freq += Actor_Roles[i].get_role().Get_Num_Lines();
		}

		else if (Actor_Roles[i].get_gender() == 'M' || Actor_Roles[i].get_gender() == 'm')
		{
			Male_Count_Freq += Actor_Roles[i].get_role().Get_Num_Lines();
		}

		else if (Actor_Roles[i].get_gender() == 'U' || Actor_Roles[i].get_gender() == 'u')
		{
			Undetermined_Freq += Actor_Roles[i].get_role().Get_Num_Lines();
		}

		else
		{
			NA_Freq += Actor_Roles[i].get_role().Get_Num_Lines();
		}
	}

	int	total_lines = Female_Count_Freq + Male_Count_Freq + Undetermined_Freq + NA_Freq;

	fout << "Female Count Frequency is " << Female_Count_Freq << endl;
	fout << "Male Count Frequency is " << Male_Count_Freq << endl;
	fout << "Undetermined Count Frequency is " << Undetermined_Freq << endl;
	fout << "N/A Count Frequency is " << NA_Freq << endl;
	fout << "Total lines spoken by characters is " << total_lines << endl;
	fout << "Total lines including N/A is " << total_lines + NA_Freq << endl;

	//print out the percentage of lines spoken by each character
	/*for (unsigned int i = 0; i < Characters_According_To_Film.size(); i++)
	{
	Word temp = Characters_According_To_Film[i].Get_Name();
	//check += float(temp.Get_Frequency()) / float(total_lines);
	cout << temp.Get_Word() << " has " << (float(temp.Get_Frequency()) / total_lines) * 100 << "% of the lines in the film. " << endl;
	}*/
}

void Unfound_Gender_Prompt(vector <Character>& Characters_In_Film)
{
	float total_lines = 0;

	//Sara says: Should total lines only count a frequency if the gender is not N/A? 
	//then we wouldn't count "lines" attributed to things like CUT TO:
	for (unsigned int i = 0; i < Characters_In_Film.size(); i++)
	{
		total_lines += Characters_In_Film[i].Get_Num_Lines();
	}

	for (unsigned int i = 0; i < Characters_In_Film.size(); i++)
	{
		int Word_Count = 0;
		float Percent_Lines = (Characters_In_Film[i].Get_Num_Lines() / total_lines) * 100;
	}
}

void out_results(string Results_File, Movie& This_Movie, string movie_name, bool& Is_First, int& choice)
{
	ofstream fout;

	vector <Actor> Cast_List = This_Movie.Get_Cast();
	//cast as floats to avoid integer division.
	float Total_Movie_Lines = float(This_Movie.Total_Lines());
	float Total_Movie_Words = float(This_Movie.Total_Words());

	//for the other function creating a separate excel file, which makes a new excel sheet.
	string Separate_Results = Results_File;
	for (int i = 0; i < 11; i++)
	{
		Separate_Results.pop_back();
	}
	string Title = This_Movie.Get_Title();
	if (Title.size() > 0 && Title[0] == '\"')
	{
		//cout << "In here!" << endl;
		Title.pop_back();
		Title.erase(Title.begin());
	}

	Separate_Results += Title += "_Results.csv";

	const time_t today = time(0);

	char string_today[26];

	//reference:
	//errno_t ctime_s(char *buffer, rsize_t bufsz, const time_t *time);
	ctime_s(string_today, sizeof string_today, &today);
	
	//time_t now = time(0);
    //char* string_today = ctime(&now);

	if (Is_First)
	{
		Is_First = false;

		//Gives a value 1 - 9.  Data validation also given to this function.
		display_options(choice);

		//These choices overwite the results file. 
		if (choice == 1 || choice == 3 || choice == 5)
		{
			//This accomplishes two things
			//First, we wipe out all existing information in the results file
			//Second, we output to the Results file exactly what day and time we ran the program 
			//We might want to consider doing this each time a movie's results gets outputted 
			fout.open(Results_File);

			fout << "These are the results as of " << string_today << endl;

			fout.close();
		}

		//These options overwite the inidividual file
		if (choice == 1 || choice == 2 || choice == 4)
		{
			fout.open(Separate_Results);

			fout << "These are the results as of " << string_today << endl;

			fout.close();
		}
	}
	//9 means write nothing
	if (choice == 9)
	{
		return;
	}

	//At this point, we have exited the function if we are debugging everything.
	//We have also taken care of wiping the previous files if they wished to overwrite.
	//Thus everything can be treated as an append.

	//choices 5 and 7 do not write to the inidividual file.
	if (choice != 5 && choice != 7)
	{
		individual_out(Separate_Results, This_Movie);
	}

	//Choices 4 and 6 do not write to the results file.
	if (choice != 4 && choice != 6)
	{
		individual_out(Results_File, This_Movie);
	}
}


void individual_out(string Results_File, Movie& This_Movie)
{
	//Print out all of the movie data we have so far.
	vector <Actor> Cast_List = This_Movie.Get_Cast();
	int counter = 0;
	while (Cast_List[Cast_List.size() - 1].get_role().Get_Num_Lines() == 0)
	{
		Cast_List.pop_back();
	}

	float Total_Movie_Lines = float(This_Movie.Total_Lines());
	float Total_Movie_Words = float(This_Movie.Total_Words());

	ofstream fout;
	fout.open(Results_File, fstream::out | fstream::app);

	vector <string> Genres = This_Movie.Get_Genres();
	fout << This_Movie.Get_Title() << endl;
	fout << "Genres:" << endl;
	for (unsigned int i = 0; i < Genres.size(); i++)
	{
		fout << Genres[i] << ',';
	}
	fout << endl;

	fout << "Director(s):" << endl;
	vector <string> Directors = This_Movie.Get_Directors();
	for (unsigned int i = 0; i < Directors.size(); i++)
	{
		fout << Directors[i] << ',';
	}

	fout << endl;
	fout << "Year" << endl;
	fout << This_Movie.Get_Year() << endl;
	
	fout << "Number of Nominations" << endl;
	fout << This_Movie.Get_Num_Noms() << endl << endl;
	

	//Now print all specific cast information.
	fout << "Actor/Actress" << ',' << "IMDB Name" << ',' << "Script Name" << ',' << "Gender" << ',' << "Importance" << ','
		<< "Lines" << ',' << "Words" << ',' << "Percent Lines" << ',' << "Percent Words" << endl;
	
	//For legibility
	float percent_lines = 0;
	float percent_words = 0;
	

	for (unsigned int i = 0; i < Cast_List.size(); i++)
	{
		//if (Cast_List[i].get_role().Get_Num_Lines() > 0)
		//{
		//I added these purely for legibility 
		percent_lines = (Cast_List[i].get_role().Get_Num_Lines() / Total_Movie_Lines) * 100;
		percent_words = (Cast_List[i].get_role().Get_Num_Words() / Total_Movie_Words) * 100;
		string Importance = "Bit Character";
		if (Cast_List[i].get_role().Get_Importance() == 'm')
		{
			Importance = "Main Character";
		}
		else if (Cast_List[i].get_role().Get_Importance() == 's')
		{
			Importance = "Supporting Character";
		}

		//if this line bothers you, go to ^View and select Wrap Lines
		fout << Cast_List[i].get_name() << ',' << Cast_List[i].get_IMDB_name() << ',' << Cast_List[i].get_role().Get_Name() << ',' << Cast_List[i].get_gender() << ',' 
			 << Importance << ',' << Cast_List[i].get_role().Get_Num_Lines() << ',' << Cast_List[i].get_role().Get_Num_Words() << ',' << percent_lines << ',' << percent_words << endl;
		//}
	}

	fout << endl;

	fout.close();
}

void display_options(int& choice)
{
	int Response = 0;
	while (choice < 6 && Response != 1)
	{
		cout << "Results have been compiled. Choose one of the following (You will only be asked this once)" << endl;
		cout << "1. Overwrite ALL previous results" << endl;
		cout << "2. Overwrite the individual file; append to the combined file" << endl;
		cout << "3. Overwrite the combined file; append to the individual file" << endl;
		cout << "4. Overwrite the individual file; write nothing to the combined file" << endl;
		cout << "5. Overwrite the combined file; write nothing to the individual file" << endl;
		cout << "6. Append to the individual file; write nothing to the combined file" << endl;
		cout << "7. Append to the combined file; write nothing to the individual file" << endl;
		cout << "8. Append to ALL existing results files" << endl;
		cout << "9. I am debugging. Ignore ALL results." << endl;

		cin >> choice;

		while (choice < 1 || choice > 9)
		{
			cout << "Sorry that is not valid. Please respond agian:" << endl;
			cin >> choice;
		}
		if (choice < 6)
		{
			cout << "The option you selected involves deleting data.  Are you SURE you wish to delete/overwrite files?" << endl;
			cout << "1. Yes, I am prepared to delete/overwite data." << endl;
			cout << "2. No, I would like to look at my options again." << endl;

			cin >> Response;
			while (Response != 1 && Response != 2)
			{
				cout << "That is not a valid response.  Please enter 1 for Yes and 2 for No:";
				cin >> Response;
			}
		}
	}
}
 
 
 