#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class MovieDemographic
{
public:
    
    MovieDemographic(string n, int y);
    void clear();
    
    void add_male();
    void add_female();
    
    void add_white();
    void add_nonwhite();
    void add_asian();
    void add_black();
    void add_hispanic();
    void add_pi_or_hi();
    void add_na_or_ak();
    
    void set_average_age_male();
    void set_average_age_female();
    
    int get_num_male();
    int get_num_female();
    int get_white();
    int get_nonwhite();
    int get_asian();
    int get_black();
    int get_hispanic();
    int get_pi_or_hi();
    int get_na_or_ak();
    
    float get_average_age_male();
    float get_average_age_female();
    
    void set_year(int y);
    int get_year();
    
    void set_title(string t);
    
    void add_age_male(int a);
    void add_age_female(int a);
    
    void add_bmi_male(float b);
    void add_bmi_female(float b);
    
    void set_average_bmi_male();
    void set_average_bmi_female();
    
    void print(string output_directory);
    
    bool has_compiled(string output_directory);
    
private:
    string name;
    int year;
    
    int num_male;
    int num_female;
    
    int white;
    int non_white;
    
    int asian;
    int black;
    int hispanic;
    //pacific islander or native hawaiian
    int pi_or_hawaiian;
    //native american or alaskan native
    int na_or_ak;
    
    float average_age_male;
    float average_age_female;
    
    float average_bmi_male;
    float average_bmi_female;
    
    vector <int> male_ages;
    vector <int> female_ages;
    
    vector<float> male_bmi;
    vector<float> female_bmi;
    
    bool add_line;
};