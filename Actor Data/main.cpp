
#include "ActorData.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "MovieDemographic.h"

using namespace std;

bool Contains_No_Lowercase(string word);
void add_actor_to_movie(ActorData actor, MovieDemographic& movie);
bool is_all_commas(string s);
string directory_selector();

int main() {
    
    
    string output_directory = directory_selector();
    //read from the main results file
    ifstream read_results;
    //read from the ActorData.csv file and get information on actors we have
    ifstream actor_sheet_read;
    //used to update the ActorData.csv File
    ofstream actor_sheet_write;
    
    //how many actors/actresses to do in one run. This is limited because if an error occurs, its best not to lose too much progress
    int how_many_to_do = 0;
    
    cout << "How many actors would you like to add information on? (< 10): ";
    cin >> how_many_to_do;
    
    while (how_many_to_do < 0 || how_many_to_do > 10) {
        cout << "How many actors would you like to add information on? (< 10): ";
        cin >> how_many_to_do;
    }
    
    //compared to the how_many_to_do variable
    int global_counter = 0;
    
    string line = "";
    bool start_reading_actors = false;
    
    //a list of ALL the actor names found in the results file
    vector <string> all_actors;
    //the names of every actor we have information on
    vector <string> actors_added;
    //the statistics of every actor we have done
    unordered_map <string, ActorData> all_actor_stats;
    
    //replace all NULLs with this if you want to get the non-number data from stoi and stof
    //size_t sz;
    
    actor_sheet_read.open(output_directory + "/Actor_Data.csv");
    
    while (actor_sheet_read.good())
    {
        
        //Windows people, change the \r to a \n (Or for Brett, we could try doing an #ifdef statement, if y'all want)
        getline(actor_sheet_read, line,'\r');
        
        //double check to see if we are at the end (if so, break)
        if (line == "") {
            break;
        }
        
        if (line[0] == '\n')
        {
            line.erase(0, 1);
        }
        
        if (line.back() == '\r')
        {
            line.pop_back();
        }
        
        //skip the first line of the csv file
        if (line == "Name,Gender,Total Lines,Total Words,Num. Movies,Words per Line,Age,Height,Weight,BMI,Net Worth,Percent lines,Percent words,Race,")
        {
            continue;
        }
        
        //these buffers are filled with all the information we can gather from the ActorData.csv file (actors we already have information on)
        string name_buffer = "";
        char gender_buffer = 'n';
        string lines_buffer = "";
        string words_buffer = "";
        //words per line
        string wpl_buffer = "";
        string age_buffer = "";
        string height_buffer = "";
        string weight_buffer = "";
        string BMI_buffer = "";
        string net_worth_buffer = "";
        vector <string> races;
        string race_buffer = "";
        //percent of lines in the film
        string pl_buffer = "";
        //percent of words in the film
        string pw_buffer = "";
        
        //we define i here so that we can continue to iterate over the same spot on the line after we gather the information from the particular section
        unsigned int i = 0;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            name_buffer += line[i];
        }
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            gender_buffer = line[i];
        }
        i++;
        
        //currently these are being used to march past the next comma (lines and words are gathered below)
        for (i; line[i] != ',' && i < line.size(); i++) {
        }
        
        int lb = 0;
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
        }
        
        int wb = 0;
        i++;

        for (i; line[i]!=',' && i < line.size(); i++){
        }
        
        i++;
        int num_movies = 0;
        
        //gathering words per line
        for (i; line[i] != ',' && i < line.size(); i++) {
            wpl_buffer += line[i];
        }
        
        float wpl = stof(wpl_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            age_buffer += line[i];
        }
        
        int ab = stoi(age_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            height_buffer += line[i];
        }
        
        int hb = stoi(height_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            weight_buffer += line[i];
        }
        
        //weight buffer as an int
        int kgsb = stoi(weight_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            BMI_buffer += line[i];
        }
        
        float bmib = stof(BMI_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            net_worth_buffer += line[i];
        }
        
        float nwb = stof(net_worth_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            pl_buffer += line[i];
        }
        
        float plb = stof(pl_buffer, NULL);
        i++;
        
        for (i; line[i] != ',' && i < line.size(); i++) {
            pw_buffer += line[i];
        }
        
        float pwb = stof(pw_buffer, NULL);
        i++;
        
        for (i; i < line.size(); i++) {
            if (line[i] != ',') {
                race_buffer += line[i];
            }
            else {
                races.push_back(race_buffer);
                race_buffer.clear();
            }
        }
        races.push_back(race_buffer);
        race_buffer.clear();
        
        //add the name to the name vector for iteration speed
        actors_added.push_back(name_buffer);
        //name, gender, lines, words,num movies, words per line, age, height, weight, bmi, percent lines, percent words, net worth, races
        ActorData this_actor(name_buffer, gender_buffer, lb, wb, num_movies, wpl, ab, hb, kgsb, bmib, nwb, plb, pwb, races);
        all_actor_stats[name_buffer] = this_actor;
        
    }
    
    actor_sheet_read.close();
    
    read_results.open(output_directory + "/Results.csv");
    
    string this_movie_title = "";
    int this_movie_year = 0;
    MovieDemographic this_movie(this_movie_title, this_movie_year);
    
    while (read_results.good()) {
        getline(read_results, line);
        
        //if we have reached the final line of the file, this double checks to exit
        if (line == "") {
            break;
        }
        
        if (is_all_commas(line))
        {
            start_reading_actors = false;
        }
        
        if (Contains_No_Lowercase(line) && !is_all_commas(line) && line[1] != ',' && line[2] != ',' &&
            global_counter < how_many_to_do)
        {
            if (this_movie.get_num_male() > 0 || this_movie.get_num_female() > 0)
            {
                this_movie.print(output_directory);
            }
            this_movie.clear();
            this_movie.set_title(line.substr(0, line.find(',')));
        }
        
        if (line == "Year,,,,,,,,")
        {
            getline(read_results, line);
            this_movie_year = stoi(line);
            this_movie.set_year(this_movie_year);
        }
        
        if (start_reading_actors == true)
        {
            
            string name_buffer = "";
            
            for (unsigned int i = 0; line[i] != ','; i++) {
                name_buffer += line[i];
            }
            
            //the character abbreviation of the actor's gender and importance in the film
            char actor_gender;
            char actor_importance = 'S';
            
            //the actors number of lines and words in the film, and the proportion of each the actor has in the film
            int actor_lines;
            int actor_words;
            float actor_pl;
            float actor_pw;
            
            //counter keeps track of the number of commas we hav eiterated over
            int counter = 0;
            
            //buffers to hold the information from the csv document
            string temp_lines = "";
            string temp_words = "";
            string temp_pl = "";
            string temp_pw = "";
            
            //this adds their gender, lines as the role, and words as the role
            for (unsigned int i = 0; i < line.size(); i++) {
                
                if (line[i] == ',') {
                    counter++;
                }
                
                if (counter == 3) {
                    i++;
                    actor_gender = line[i];
                }
                
                else if (counter == 4) {
                    i++;
                    actor_importance = line[i];
                    for (i; line[i] != ','; i++) {
                        //move forward past the first character
                    }
                    counter++;
                }
                
                else if (counter == 5) {
                    if (line[i] != ',') {
                        temp_lines += line[i];
                    }
                }
                
                else if (counter == 6) {
                    if (line[i] != ',') {
                        temp_words += line[i];
                    }
                }
                
                else if (counter == 7) {
                    if (line[i] != ',') {
                        temp_pl += line[i];
                    }
                }
                
                else if (counter == 8) {
                    if (line[i] != ',') {
                        temp_pw += line[i];
                    }
                }
                
            }
            //Brett moved this down here
            actor_lines = stoi(temp_lines, NULL);
            actor_words = stoi(temp_words, NULL);
            actor_pl = stof(temp_pl, NULL);
            actor_pw = stof(temp_pw, NULL);
            //////
            
            //add name to an unordered map, if already there then add to results
            //if not there, add to map and vector
            
            if (all_actor_stats[name_buffer].get_name() == "" && global_counter < how_many_to_do)
            {
                //if we hash on the actor's name and their name isn't there, we add it to the actors_list map
                
                //if ever you wanted to add supporting characters, just add actor_importance == 'S'
                if (actor_importance == 'm' || actor_importance == 'M') {
                    ActorData new_actor(name_buffer, actor_gender);
                    new_actor.add_lines(actor_lines);
                    new_actor.add_words(actor_words);
                    new_actor.add_pl(actor_pl);
                    new_actor.add_pw(actor_pw);
                    
                    all_actor_stats[name_buffer] = new_actor;
                    actors_added.push_back(name_buffer);
                    
                    add_actor_to_movie(all_actor_stats[name_buffer], this_movie);
                    
                    global_counter++;
                }
                
                if (global_counter >= how_many_to_do) {
                    cout << "Thank you, you are done entering information on actors until the next run." << endl;
                    //break;
                }
            }
            else if(all_actor_stats[name_buffer].get_name() != "")
            {
                if (actor_importance == 'm' || actor_importance == 'M')
                {
                    all_actor_stats[name_buffer].add_lines(actor_lines);
                    all_actor_stats[name_buffer].add_words(actor_words);
                    all_actor_stats[name_buffer].add_pl(actor_pl);
                    all_actor_stats[name_buffer].add_pw(actor_pw);
                    add_actor_to_movie(all_actor_stats[name_buffer], this_movie);

                }
            }
            
            //system("pause");
        }
        
        if (line == "Actor/Actress,IMDB Name,Script Name,Gender,Importance,Lines,Words,Percent Lines,Percent Words") {
            start_reading_actors = true;
        }
        
    }
    
    read_results.close();
    
    actor_sheet_write.open(output_directory + "/Actor_Data.csv");
    
    actor_sheet_write << "Name,Gender,Total Lines,Total Words,Num. Movies,Words per Line,Age,Height,Weight,BMI,Net Worth,Percent lines,Percent words,Race\n";
    
    for (int i = 0; i < actors_added.size(); i++) {
        actor_sheet_write << all_actor_stats[actors_added[i]].get_name() <<
        ',' << all_actor_stats[actors_added[i]].get_gender() << ',' <<
        all_actor_stats[actors_added[i]].get_lines() << ',' <<
        all_actor_stats[actors_added[i]].get_words() << ',' <<
        all_actor_stats[actors_added[i]].get_num_movies() << ',' <<
        all_actor_stats[actors_added[i]].get_words_per_line() << ',' <<
        all_actor_stats[actors_added[i]].get_age() << ',' <<
        all_actor_stats[actors_added[i]].get_height() << ',' <<
        all_actor_stats[actors_added[i]].get_weight() << ',' <<
        all_actor_stats[actors_added[i]].get_bmi() << ',' <<
        all_actor_stats[actors_added[i]].get_nw() << ',' <<
        all_actor_stats[actors_added[i]].get_percent_lines() << ',' <<
        all_actor_stats[actors_added[i]].get_percent_words() << ',';
        
        vector <string> temp = all_actor_stats[actors_added[i]].get_races();
        
        for (int j = 0; j < temp.size(); j++)
        {
            actor_sheet_write << temp[j] << ',';
        }
        
        actor_sheet_write << '\n';
    }
    
    actor_sheet_write.close();
    
    
    return 0;
}


string directory_selector() {
    
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
    
    while (individual < 1 || individual > 4) {
        cout << "Who are you? (1 - 4): ";
        cin >> individual;
        if (individual == 5) {
            cout << "Exiting" << endl;
            exit(0);
        }
    }
    
    if (individual == 1) {
        return ("C:/Users/sjensen/Dropbox/SURE 2016 Team Folder/Output_Files");
    }
    else if (individual == 2) {
        return ("C:/Users/zach erickson/Dropbox/SURE 2016 Team Folder/Output_Files");
    }
    else if (individual == 3) {
        return ("/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Output_Files");
    }
    else {
        return ("C:/Users/Ryangigs/Dropbox/SURE_2016_Team_Folder/Output_Files");
    }
}

bool Contains_No_Lowercase(string word)
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

void add_actor_to_movie(ActorData actor, MovieDemographic& movie)
{
    //gender
    actor.set_bmi();
    
    if (actor.get_gender() == 'm')
    {
        movie.add_male();
        movie.add_age_male(actor.get_age());
        movie.add_bmi_male(actor.get_bmi());
    }
    else if (actor.get_gender() == 'f')
    {
        movie.add_female();
        movie.add_age_female(actor.get_age());
        movie.add_bmi_female(actor.get_bmi());
    }
    //race
    
    vector<string> races = actor.get_races();
    
    bool is_added = false;
    
    for (int i = 0; i < races.size(); i++)
    {
        if (races[i] == "")
        {
            continue;
        }
        if (races[i] != "White" && !is_added)
        {
            movie.add_nonwhite();
        }
        if (races[i] == "White")
        {
            movie.add_white();
        }
        else if (races[i] == "Asian")
        {
            movie.add_asian();
        }
        else if (races[i] == "Black or African American")
        {
            movie.add_black();
        }
        else if (races[i] == "Hispanic or Latin(x)")
        {
            movie.add_hispanic();
        }
    }
    
}

bool is_all_commas(string s)
{
    for (int i = 0; i < s.size() - 1; i++)
    {
        if (s[i] != ',')
        {
            return false;
        }
    }
    return true;
}


