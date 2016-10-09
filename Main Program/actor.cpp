
#include "actor.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

//real name, gender, age, height, net worth, race, eccentricity
Actor::Actor(string n, char g){
    
    name = n;
    gender = g;
}

Actor::Actor(){
    name = "";
    gender = 'N';
}

//sets the name of the real life actor (i.e. Tom Hanks)
void Actor::set_name(string n){
    name = n;
}
string Actor::get_name(){
    return name;
}

//sets the name of the character according to the IMDB Cast list (i.e. Forrest Gump)
void Actor::set_IMDB_name(string n)
{
    IMDB_name = n;
}
string Actor::get_IMDB_name()
{
    return IMDB_name;
}

void Actor::set_gender(vector<string> Male_Names, vector<string> Female_Names, string Male_Name_Database, string Female_Name_Database)
{
    string first_name;
    unsigned int n = 0;
    
    //Adds characters until we get to a space
    while (n < name.size() && name[n] != ' ')
    {
        first_name += name[n];
        n++;
    }
    
    first_name = decapitalize(first_name);
    
    for (unsigned int j = 0; j < Male_Names.size(); j++)
    {
        //Goes through every common male name and looks for a match
        if (first_name  == Male_Names[j] || get_name() == "Unidentified Male")
        {
            gender = 'm';
            return;
        }
    }
    
    if (get_gender() == 'N' || get_gender() == 'n')
    {
        for (unsigned int j = 0; j < Female_Names.size(); j++)
        {
            if (first_name == Female_Names[j] || get_name() == "Unidentified Female")
            {
                gender = 'f';
                return;
            }
        }
    }
    
    //at this point, if we have assigned a gender we can move on
    if (gender != 'N' && gender != 'n')
    {
        return;
    }
    
    //Now let's do the same thing but with the character name
    string Character_First_Name;
    unsigned int m = 0;
    
    while (m < role.Get_Name().size() && role.Get_Name()[m]!=' ')
    {
        Character_First_Name += role.Get_Name()[m];
        m++;
    }
    
    Character_First_Name = decapitalize(Character_First_Name);
    
    for (unsigned int j = 0; j < Male_Names.size(); j++)
    {
        if (Character_First_Name == Male_Names[j])
        {
            gender = 'm';
            return;
        }
    }
    
    for (unsigned int j = 0; j < Female_Names.size(); j++)
    {
        if (Character_First_Name == Female_Names[j])
        {
            gender = 'f';
            return;
        }
    }
    
    m++;
    
    //We can also look at the "second name" (to look for words like "boy")
    string Character_Second_Name;
    
    while (m < get_role().Get_Name().size() && get_role().Get_Name()[m]!=' ')
    {
        Character_Second_Name += get_role().Get_Name()[m];
        m++;
    }
    
    Character_Second_Name = decapitalize(Character_Second_Name);
    
    if (Character_Second_Name.size()!=0)
    {
        for (unsigned int j = 0; j < Male_Names.size(); j++)
        {
            if (Character_Second_Name == Male_Names[j])
            {
                gender = 'm';
                return;
            }
        }
        
        for (unsigned int j = 0; j < Female_Names.size(); j++)
        {
            if (Character_Second_Name == Female_Names[j])
            {
                gender = 'f';
                return;
            }
        }
    }
    
    
    //After all that and we STILL don't have a gender, let's assign it manually
    if (get_gender() == 'N' || get_gender() == 'n')
    {
        if(name == "Unidentified Neutral")
        {
            return;
        }
        cout << "We were unable to assign a gender for the following actor: " << name << endl;
        cout << "Select one of the following options: " << endl;
        cout << "1. Assign as Male" << endl;
        cout << "2. Assign as Female" << endl;
        cout << "3. View Character Name" << endl;
        
        int choice;
        cin >> choice;
        
        //Data validation!!
        while (choice < 0 || choice > 4)
        {
            cout << "Sorry, that is not a valid response. Please choose again." << endl;
            cin >> choice;
        }
        
        if (choice == 1)
        {
            gender = 'm';
            cout << "Would you like to add the name " << first_name << " to the Male gender database?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            int choice1;
            cin >> choice1;
            
            while (choice1 < 0 || choice1 > 2)
            {
                cout << "Sorry, that is not a valid response. Please choose again." << endl;
                cin >> choice1;
            }
            
            if (choice1 == 1)
            {
                ofstream fout;
                fout.open(Male_Name_Database, fstream::out | fstream::app);
               
                #ifdef __APPLE__
                    fout << '\r';
                #endif
                
                fout << first_name << endl;
            }
            else
                return;
        }
        if (choice == 2)
        {
            gender = 'f';
            cout << "Would you like to add the name " << first_name << " to the Female gender database?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            int choice1;
            cin >> choice1;
            
            while (choice1 < 0 || choice1 > 2)
            {
                cout << "Sorry, that is not a valid response. Please choose again." << endl;
                cin >> choice1;
            }
            
            if (choice1 == 1)
            {
                ofstream fout;
                fout.open(Female_Name_Database, fstream::out | fstream::app);
                
                 #ifdef __APPLE__
                    fout << '\r';
                #endif
                
                fout << first_name << endl;
            }
            else
                return;
        }
        if (choice == 3)
        {
            cout << "We were unable to assign a gender for the following character: " << Character_First_Name << endl;
            cout << "Select one of the following options: " << endl;
            cout << "1. Assign as Male" << endl;
            cout << "2. Assign as Female" << endl;
            cout << "3. View Second Character Name (if possible)" << endl;
            
            int choice1;
            cin >> choice1;
            
            while (choice1 < 0 || choice1 > 3)
            {
                cout << "Sorry, that is not a valid response. Please choose again." << endl;
                cin >> choice1;
            }
            
            if (choice1 == 1)
            {
                gender = 'm';
                cout << "Would you like to add the name " << Character_First_Name << " to the Male gender database?" << endl;
                cout << "1. Yes" << endl;
                cout << "2. No" << endl;
                int choice1;
                cin >> choice1;
                
                while (choice1 < 0 || choice1 > 2)
                {
                    cout << "Sorry, that is not a valid response. Please choose again." << endl;
                    cin >> choice1;
                }
                
                if (choice1 == 1)
                {
                    ofstream fout;
                    fout.open(Male_Name_Database, fstream::out | fstream::app);
                    fout << first_name << endl;
                }
                else
                    return;
            }
            if (choice1 == 2)
            {
                gender = 'f';
                cout << "Would you like to add the name " << Character_First_Name << " to the Female gender database?" << endl;
                cout << "1. Yes" << endl;
                cout << "2. No" << endl;
                int choice1;
                cin >> choice1;
                
                while (choice1 < 0 || choice1 > 2)
                {
                    cout << "Sorry, that is not a valid response. Please choose again." << endl;
                    cin >> choice1;
                }
                
                if (choice1 == 1)
                {
                    ofstream fout;
                    fout.open(Female_Name_Database, fstream::out | fstream::app);
                    fout << first_name << endl;
                }
                else {
                    return;
                }    
            }
            
            if (choice1 == 3)
            {
                cout << "We were unable to assign a gender for the following character: " << Character_Second_Name << endl;
                cout << "Select one of the following options: " << endl;
                cout << "1. Assign as Male" << endl;
                cout << "2. Assign as Female" << endl;
                cout << "3. Ignore - Assign as Neutral" << endl;
                
                int choice2;
                cin >> choice2;
                
                while (choice2 < 0 || choice2 > 3)
                {
                    cout << "Sorry, that is not a valid response. Please choose again." << endl;
                    cin >> choice2;
                }
                
                if (choice2 == 1)
                {
                    gender = 'm';
                    cout << "Would you like to add the name " << Character_Second_Name << " to the Male gender database?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;
                    int choice3;
                    cin >> choice3;
                    
                    while (choice2 < 0 || choice2 > 2)
                    {
                        cout << "Sorry, that is not a valid response. Please choose again." << endl;
                        cin >> choice3;
                    }
                    
                    if (choice3 == 1)
                    {
                        ofstream fout;
                        fout.open(Male_Name_Database, fstream::out | fstream::app);
                        fout << first_name << endl;
                    }
                    else
                        return;
                }
                if (choice2 == 2)
                {
                    gender = 'f';
                    cout << "Would you like to add the name " << Character_Second_Name << " to the Female gender database?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;
                    int choice3;
                    cin >> choice3;
                    
                    while (choice3 < 0 || choice3 > 2)
                    {
                        cout << "Sorry, that is not a valid response. Please choose again." << endl;
                        cin >> choice3;
                    }
                    
                    if (choice3 == 1)
                    {
                        ofstream fout;
                        fout.open(Female_Name_Database, fstream::out | fstream::app);
                        fout << first_name << endl;
                    }
                    else
                        return;
                }
            }
        }
    }
}

char Actor::get_gender(){
    return gender;
}

//sets the name of the character according to the character name we found in the script (i.e. FORREST)
void Actor::set_role(Character& c)
{
    role = c;
}
Character& Actor::get_role()
{
	return role;
}

void Actor::set_index(int i)
{
    index = i;
}
int Actor::get_index()
{
    return index;
}

string Actor::decapitalize(string word)
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
	    

