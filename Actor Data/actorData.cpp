
#include "ActorData.h"
#include <string>

ActorData::ActorData() {
    
    name = "";
    gender = '0';
    num_lines = 0;
    num_words = 0;
    words_per_line = 0;
    age = 0;
    height = 0;
    weight = 0;
    bmi = 0;
    net_worth = 0;
    num_movies = 0;
}

ActorData::ActorData(string n, char g) {
    
    race_list.push_back("Invalid");
    race_list.push_back("American Indian or Alaska Native");
    race_list.push_back("Asian");
    race_list.push_back("Black or African American");
    race_list.push_back("Native Hawaiian or Other Pacific Islander");
    race_list.push_back("White");
    race_list.push_back("Hispanic or Latin(x)");
    race_list.push_back("Unknown");
    
    
    num_lines = 0;
    num_words = 0;
    num_movies = 0;
    
    cout << "We have identified ";
    
    set_name(n);
    set_gender(g);
    
    if (gender == 'm') {
        cout << "an actor";
    }
    
    else if (gender == 'f') {
        cout << "an actress";
    }
    
    else {
        cout << "a person";
    }
    
    cout << " named " << name << endl;
    
    set_age();
    set_height();
    set_weight();
    set_bmi();
    set_races();
    set_nw();
}

ActorData::ActorData(string n, char g, int l, int nm, int w, float wpl, int a, int h, float kgs, float bmi_to_set, float nw, float plb, float pwb, vector <string> races_to_add) {
    
    race_list.push_back("Invalid");
    race_list.push_back("American Indian or Alaska Native");
    race_list.push_back("Asian");
    race_list.push_back("Black or African American");
    race_list.push_back("Native Hawaiian or Other Pacific Islander");
    race_list.push_back("White");
    race_list.push_back("Hispanic or Latin(x)");
    race_list.push_back("Unknown");
    
    name = n;
    gender = g;
    num_lines = l;
    num_words = w;
    num_movies = nm;
    words_per_line = wpl;
    age = a;
    height = h;
    weight = kgs;
    bmi = bmi_to_set;
    net_worth = nw;
    percent_lines = plb;
    percent_words = pwb;
    races = races_to_add;
}


void ActorData::set_name(string n) {
    name = n;
}

string ActorData::get_name() {
    return name;
}

void ActorData::set_gender(char g) {
    if (g == 'm' || g == 'f' || g == 'n' || g == 'u') {
        gender = g;
    }
    else {
        cout << name << "'s gender data is invalid. Please enter an 'm' for male, 'f' for female, 'n' for neutral, or 'u' for unidentified: ";
        cin >> gender;
        
        while (gender != 'm' && gender != 'f' && gender != 'n' && gender != 'u') {
            cout << "That is an invalid entry. Please enter an 'm' for male, 'f' for female, 'n' for neutral, or 'u' for unidentified: ";
            cin >> gender;
        }
    }
}

char ActorData::get_gender() {
    return gender;
}

void ActorData::set_age() {
    cout << "How old is " << name << " now? If they have passed, approximate their age as it would be today: ";
    cin >> age;
    
    while (age < 0) {
        cout << "Invalid age, please enter " << name << "'s age as of today: ";
        cin >> age;
    }
}

int ActorData::get_age() {
    return age;
}

void ActorData::set_height() {
    cout << "How tall is " << name << "? (in rounded centimeters): ";
    cin >> height;
    
    while (height < 0) {
        cout << "Invalid height, please enter " << name << "'s height: ";
        cin >> height;
    }
}

int ActorData::get_height() {
    return height;
}

void ActorData::set_weight() {
    cout << "How much does " << name << " weigh? (in kgs) (enter 0 if you do not know): ";
    cin >> weight;
    
    while (weight < 0) {
        cout << "Invalid weight, please enter " << name << "'s weight: ";
        cin >> weight;
    }
}

float ActorData::get_weight() {
    return weight;
}

void ActorData::set_nw() {
    cout << "What is " << name << "'s net worth? (No commas, enter 0 if unknown): ";
    cin >> net_worth;
    
    while (net_worth < 0)
    {
        cout << name << "'s net worth should be greater than or equal to 0. Re-enter: ";
        cin >> net_worth;
    }
}

float ActorData::get_nw() {
    return net_worth;
}

void ActorData::set_races() {
    
    
    cout << "Please enter " << name << "'s race(s): " << endl;
    cout << "0) Descriptions on the races/ethnicities" << endl;
    cout << "1) American Indian or Alaska Native" << endl;
    cout << "2) Asian" << endl;
    cout << "3) Black or African American" << endl;
    cout << "4) Native Hawaiian or Other Pacific Islander" << endl;
    cout << "5) White" << endl;
    cout << "6) Hispanic or Latin(x)" << endl;
    cout << "7) Unknown" << endl;
    
    
    int choice;
    cin >> choice;
    
    while (choice < 0 || choice > 7) {
        cout << "Invalid choice, please try again: ";
        cin >> choice;
    }
    
    if (choice == 0) {
        cout << "Hispanic or Latin(x): A person of Cuban, Mexican, Puerto Rican, South or Central American, or other Spanish culture or origin, regardless of race. " << endl;
        cout << "American Indian or Alaska Native: A person having origins in any of the original peoples of North and South America (including Central America) who maintains cultural identification through tribal affiliation or community attachment. " << endl;
        cout << "Asian: A person having origins in any of the original peoples of the Far East, Southeast Asia, or the Indian Subcontinent, including, for example, Cambodia, China, India, Japan, Korea, Malaysia, Pakistan, the Philippine Islands, Thailand, and Vietnam." << endl;
        cout << "Black or African American: A person having origins in any of the black racial groups of Africa." << endl;
        cout << "Native Hawaiian or Other Pacific Islander: A person having origins in any of the original peoples of Hawaii, Guam, Samoa, or other Pacific Islands." << endl;
        cout << "White: A person having origins in any of the original peoples of Europe, the Middle East, or North Africa." << endl;
        set_races();
    }
    
    else {
        
        races.push_back(race_list[choice]);
        
        if (races.size() < 6) {
            cout << "Would you like to enter another race/ethnicity for " << name << "? \n(1) yes \n(2) no \n: ";
            cin >> choice;
            
            while (choice != 1 && choice != 2) {
                cout << "Invalid. Would you like to enter another race/ethnicity for " << name << "? \n(1) yes \n(2) no \n:";
                cin >> choice;
            }
            
            if (choice == 1) {
                set_races();
            }
        }
    }
    
}

vector <string> ActorData::get_races() {
    return races;
}

void ActorData::set_bmi() {
    
    if (weight >= 0 && height > 0) {
        bmi = float(weight) / ((float(height) / 100) * (float(height) / 100));
    }
    
    else {
        bmi = -1;
    }
}

float ActorData::get_bmi() {
    return bmi;
}

void ActorData::add_lines(int lines)
{
    num_movies++;
    num_lines += lines;
}

int ActorData::get_lines() {
    return num_lines;
}

void ActorData::add_words(int words) {
    num_words += words;
}

int ActorData::get_words() {
    return num_words;
}

float ActorData::get_words_per_line() {
    return float(float(num_words) / float(num_lines));
}

void ActorData::add_pl(float pl)
{
    pls.push_back(pl);
    
    percent_lines = 0;
    for(int i = 0; i < pls.size(); i++)
    {
        percent_lines += pls[i];
    }
    
    percent_lines = percent_lines/(float)pls.size();
}

float ActorData::get_percent_lines(){
    return percent_lines;
}

void ActorData::add_pw(float pw){
    pwds.push_back(pw);
    
    percent_words = 0;
    for(int i = 0; i < pwds.size(); i++)
    {
        percent_words += pwds[i];
    }
    
    percent_words = percent_words/(float)pwds.size();
}

float ActorData::get_percent_words()
{
    return percent_words;
}
int ActorData::get_num_movies()
{
    return num_movies;
}



