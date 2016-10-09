#include "MovieDemographic.h"

MovieDemographic::MovieDemographic(string n, int y)
{
    name = n;
    year = y;
    num_male = 0;
    num_female = 0;
    
    white = 0;
    non_white = 0;
    
    asian = 0;
    black = 0;
    hispanic = 0;
    //pacific islander or native hawaiian
    pi_or_hawaiian = 0;
    //native american or alaskan native
    na_or_ak = 0;
    
    add_line = false;
    
    average_age_male = 0;
    average_age_female = 0;
}

void MovieDemographic::clear()
{
    name = "";
    year = 0;
    num_male = 0;
    num_female = 0;
    white = 0;
    non_white = 0;
    asian = 0;
    black = 0;
    hispanic = 0;
    pi_or_hawaiian = 0;
    na_or_ak = 0;
    average_age_male = 0;
    average_age_female = 0;
    average_bmi_male = 0;
    average_bmi_female = 0;
    
    add_line = false;
    
    male_ages.clear();
    female_ages.clear();
    male_bmi.clear();
    female_bmi.clear();
}

void MovieDemographic::add_male()
{
    num_male++;
}

void MovieDemographic::add_female()
{
    num_female++;
}

void MovieDemographic::add_white()
{
    white++;
}

void MovieDemographic::add_nonwhite()
{
    non_white++;
}

void MovieDemographic::add_asian()
{
    asian++;
}

void MovieDemographic::add_black()
{
    black++;
}

void MovieDemographic::add_hispanic()
{
    hispanic++;
}

void MovieDemographic::add_pi_or_hi(){
    pi_or_hawaiian++;
}

void MovieDemographic::add_na_or_ak(){
    na_or_ak++;
}

void MovieDemographic::set_average_age_male()
{
    if (male_ages.size() == 0)
    {
        average_age_male = 0;
        return;
    }
    float sum = 0;
    for (int i = 0; i < male_ages.size(); i++)
    {
        sum += male_ages[i];
    }
    average_age_male = sum / (float)male_ages.size();
}

void MovieDemographic::set_average_age_female()
{
    if (female_ages.size() == 0)
    {
        average_age_female = 0;
        return;
    }
    float sum = 0;
    for (int i = 0; i < female_ages.size(); i++)
    {
        sum += female_ages[i];
    }
    average_age_female = sum / (float)female_ages.size();
}

int MovieDemographic::get_num_male()
{
    return num_male;
}
int MovieDemographic::get_num_female()
{
    return num_female;
}
int MovieDemographic::get_white()
{
    return white;
}
int MovieDemographic::get_nonwhite()
{
    return non_white;
}
int MovieDemographic::get_asian()
{
    return asian;
}
int MovieDemographic::get_black()
{
    return black;
}
int MovieDemographic::get_hispanic()
{
    return hispanic;
}

int MovieDemographic::get_pi_or_hi(){
    return pi_or_hawaiian;
}

int MovieDemographic::get_na_or_ak(){
    return na_or_ak;
}

float MovieDemographic::get_average_age_male()
{
    return average_age_male;
}
float MovieDemographic::get_average_age_female()
{
    return average_age_male;
}

int MovieDemographic::get_year()
{
    return year;
}
void MovieDemographic::add_age_male(int a)
{
    int difference = 2016 - year;
    a = a - difference;
    male_ages.push_back(a);
}
void MovieDemographic::add_age_female(int a)
{
    int difference = 2016 - year;
    a = a - difference;
    female_ages.push_back(a);
}
void MovieDemographic::print(string output_directory)
{
    if (has_compiled(output_directory))
    {
        return;
    }
    
    set_average_age_female();
    set_average_age_male();
    set_average_bmi_male();
    set_average_bmi_female();
    
    string Results_File = output_directory + "/MovieDemographics.csv";
    
    ofstream fout;
    
    fout.open(Results_File, fstream::out | fstream::app);
    
    if (add_line)
    {
        fout << '\n';
    }
    
    fout << name << ',' << year << ',' << num_male << ',' << num_female << ',' << white << ',' << non_white << ','
    << black << ',' << asian << ',' << hispanic << ',' << pi_or_hawaiian << ',' << na_or_ak << ','
    << average_age_male << ',' << average_age_female
    << ',' << average_bmi_male << ',' << average_bmi_female << "\n";
    
    ofstream fout2;
    string Compiled_File = output_directory + "/CompiledList.txt";
    fout2.open(Compiled_File, fstream::out | fstream::app);
    
    fout2 << name << "\n";
}

void MovieDemographic::set_title(string t)
{
    name = t;
}

void MovieDemographic::set_year(int y)
{
    year = y;
}

bool MovieDemographic::has_compiled(string output_directory)
{
    ifstream fin;
    string Compiled_File = output_directory + "/CompiledList.txt";
    fin.open(Compiled_File);
    
    string this_name;
    vector<string> all_names;
    
    while (fin.good())
    {
        getline(fin, this_name);
        all_names.push_back(this_name);
    }
    
    for (int i = 0; i < all_names.size(); i++)
    {
        if (name == all_names[i])
        {
            if (i == all_names.size()-1)
            {
                add_line = true;
            }
            return true;
        }
    }
    
    return false;
}
void MovieDemographic::add_bmi_male(float b)
{
    male_bmi.push_back(b);
}
void MovieDemographic::add_bmi_female(float b)
{
    female_bmi.push_back(b);
}
void MovieDemographic::set_average_bmi_male()
{
    if (male_bmi.size() == 0)
    {
        average_bmi_male = 0;
        return;
    }
    float sum = 0;
    for (int i = 0; i < male_bmi.size(); i++)
    {
        sum += male_bmi[i];
    }
    average_bmi_male = sum / (float)male_bmi.size();
}
void MovieDemographic::set_average_bmi_female()
{
    if (female_bmi.size() == 0)
    {
        average_bmi_female = 0;
        return;
    }
    float sum = 0;
    for (int i = 0; i < female_bmi.size(); i++)
    {
        sum += female_bmi[i];
    }
    average_bmi_female = sum / (float)female_bmi.size();
}