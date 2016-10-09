
#pragma once

#include <string>
#include "character.h"

class Actor {
    
    public:
        //real name, gender, age, height, weigh net worth, race, eccentricity
        Actor(string n, char g);
        Actor();
        
        //sets the name of the real life actor (i.e. Tom Hanks)
        void set_name(string n);
        string get_name();
        
        //sets the name of the character according to the IMDB Cast list (i.e. Forrest Gump)
        void set_IMDB_name(string n);
        string get_IMDB_name();
        
        //sets the name of the character according to the character name we found in the script (i.e. FORREST)
        void set_role(Character& c);
        Character& get_role();
        
        void set_gender(vector<string> Male_Names, vector<string> Female_Names, string Male_Name_Database, string Female_Name_Database);
        char get_gender();
        
        void set_index(int i);
        int get_index();
        
	    string decapitalize(string word);
	    
	private:
	    //name of the actor
        string name;
        
        // Name of the character according to IMDB.
        string IMDB_name;
        // The character as they appear in the script
        Character role;
        
         //helper member
        int index;
        
        //Current key - can be changed.
        //Gender can be m - male, f - female, u - unidentified, n - not a thing having a gender
        char gender;
};



