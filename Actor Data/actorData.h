
#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class ActorData {
    
public:
    
    //add how many movie they are in
    
    ActorData();
    ActorData(string n, char g);
    ActorData(string n, char g, int l, int w, int nm, float wpl, int a, int h, float kgs, float bmi_to_add, float nw, float pl, float pw, vector <string> races_to_add);
    
    //sets the name of the real life actor (i.e. Tom Hanks)
    void set_name(string n);
    string get_name();
    
    void set_gender(char g);
    char get_gender();
    
    void set_age();
    int get_age();
    
    void set_height();
    int get_height();
    
    void set_weight();
    float get_weight();
    
    void set_nw();
    float get_nw();
    
    void set_bmi();
    float get_bmi();
    
    void set_races();
    vector <string> get_races();
    
    void add_lines(int lines);
    int get_lines();
    
    void add_words(int words);
    int get_words();
    
    float get_words_per_line();
    
    void add_pl(float pl);
    float get_percent_lines();
    
    void add_pw(float pw);
    float get_percent_words();
    
    int get_num_movies();
    
private:
    //name of the actor
    string name;
    
    //PHYSICAL CHARACTERISTICS
    
    int age;
    
    char gender;
    
    //in centimeters
    int height;
    
    //in kilograms
    float weight;
    
    //in $
    float net_worth;
    
    vector <string> races;
    vector <string> race_list;
    
    //body mass index is weight (kg) / (height^2)(meters)
    float bmi;
    
    int num_lines;
    int num_words;
    
    float percent_lines;
    vector <float> pls;
    float percent_words;
    vector <float> pwds;
    
    float words_per_line;
    
    int num_movies;
};



