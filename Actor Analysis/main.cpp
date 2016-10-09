#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

string directory_selector();
vector<string> filter_actors(vector<string> all_actors, string& destination);
vector<string> filter_movies(vector<string> all_movies, string& destination);
vector<string> get_data(vector<string> actors, int num_commas, string test);

vector<string> get_movies_by_type(vector<string> all_movies, string type);

vector<string> read_file(string path);

char get_new_line(string path);

void print(string path, vector<string> data);

void frequency_distribution(int start, int end, int interval, vector<string>& data, int num_commas);

int num_whatever(vector<string> data, int num_commas);

int main()
{
    string output_directory = directory_selector();
    
    while (true)
    {
        cout << "Would you like to analyze the Movie file or the Actor file?" << endl;
        cout << "1. Movie" << endl;
        cout << "2. Actor" << endl;
        
        int choice;
        cin >> choice;
        
        
        while(choice < 1 || choice > 2)
        {
            cout << "Would you like to analyze the Movie file or the Actor file?" << endl;
            cin >> choice;
        }
        
        if (choice == 1)
        {
            vector<string> All_Movies = read_file(output_directory+"/MovieDemographics.csv");
            print(output_directory+"/All_Movie_Demographics.csv", All_Movies);
            string destination = output_directory + "/TypeDemographics/";
            vector<string> filtered_movies = filter_movies(All_Movies, destination);
            print(destination,filtered_movies);
        }
        else if (choice == 2)
        {
            vector<string> All_Actors = read_file(output_directory+"/Actor_Data.csv");
            string destination = output_directory + "/ActorData/";
            vector<string> filtered_actors = filter_actors(All_Actors, destination);
            destination += ".csv";
            print(destination,filtered_actors);
        }
    }
    
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
vector<string> filter_movies(vector<string> all_movies, string& destination)
{
    vector<string> retval;
    
    cout << "How would you like to filter the movies?" << endl;
    cout << "1. Oscars" << endl;
    cout << "2. Disney" << endl;
    cout << "3. IMDB Top 250" << endl;
    cout << "4. Female-Lead Movies" << endl;
    cout << "5. Kids Movies" << endl;
    cout << "6. Movies w/ Minority Lead" << endl;
    cout << "7. Movies w/ Male and Female Leads" << endl;
    cout << "8. Male Leads" << endl;
    
    int choice;
    cin >> choice;
    
    while(choice < 1 || choice > 8)
    {
        cout << "How would you like to filter the movies?" << endl;
        cin >> choice;
    }
    
    if (choice == 1)
    {
        destination += "Oscars";
        retval = get_movies_by_type(all_movies, "Oscars");
    }
    else if(choice == 2)
    {
        destination += "Disney";
        retval = get_movies_by_type(all_movies, "Disney");
    }
    else if(choice == 3)
    {
        destination += "IMDB";
        retval = get_movies_by_type(all_movies, "IMDB");
    }
    else if(choice == 4)
    {
        destination += "FemaleLeads";
        retval = get_movies_by_type(all_movies, "FemaleLeads");
    }
    else if(choice == 5)
    {
        destination += "Kids";
        retval = get_movies_by_type(all_movies, "Kids");
    }
    else if(choice == 6)
    {
        destination += "MinorityLead";
        retval = get_data(all_movies, 5, "0");
    }
    else if(choice == 7)
    {
        destination += "MaleAndFemale";
        retval = get_data(all_movies, 2, "0");
        retval = get_data(retval, 3, "0");
    }
    else if(choice == 8)
    {
        destination += "MaleLeads";
        retval = get_data(all_movies, 2, "0");
    }
    
    destination += ".csv";
    
    return retval;
}
vector<string> filter_actors(vector<string> all_actors, string& destination)
{
    vector<string> retval;
    
    cout << "How would you like to filter the actors? (For 2-6 only actors who we were able to find such data will be added)" << endl;
    cout << "1. By Gender - Male" << endl;
    cout << "2. By Gender - Female" << endl;
    cout << "3. By Height" << endl;
    cout << "4. By Weight" << endl;
    cout << "5. By BMI" << endl;
    cout << "6. By Net Worth" << endl;
    
    int choice;
    
    cin >> choice;
    
    while (choice < 1 || choice > 6)
    {
        cout << "How would you like to filter the actors?" << endl;
        cin >> choice;
    }
    
    if (choice == 1)
    {
        destination += "Male";
        retval = get_data(all_actors, 1, "f");
    }
    else if(choice == 2)
    {
        destination += "Female";
        retval = get_data(all_actors, 1, "m");
    }
    else if(choice == 3)
    {
        destination += "Height";
        retval = get_data(all_actors, 7, "0");
        frequency_distribution(150, 200, 5, retval, 7);
    }
    else if(choice == 4)
    {
        destination += "Weight";
        retval = get_data(all_actors, 8, "0");
        frequency_distribution(40, 140, 10, retval, 8);
    }
    else if(choice == 5)
    {
        destination += "BMI";
        retval = get_data(all_actors, 9, "0");
        frequency_distribution(14, 28, 2, retval, 9);
    }
    else if(choice == 6)
    {
        destination += "NetWorth";
        retval = get_data(all_actors, 10, "0");
        frequency_distribution(10000000, 100000000, 10000000, retval, 10);
    }
    
    cout << "Would you like to run another filter on this new set of actors?" << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    
    cin >> choice;
    
    while (choice < 1 || choice > 2)
    {
        cout << "Would you like to run another filter on this new set of actors?" << endl;
        cin >> choice;
    }
    
    if (choice == 1)
    {
        retval = filter_actors(retval, destination);
    }
    
    return retval;
}
vector<string> get_data(vector <string> actors, int num_commas, string test)
{
    vector<string> retval;
    string buffer = "";
    int counter = 0;
    int n = 0;
    for (int i = 0; i < actors.size(); i++)
    {
        if(actors[i] == "")
        {
            continue;
        }
        while (counter < num_commas)
        {
            if (actors[i][n] == ',')
            {
                counter++;
            }
            n++;
        }
        counter = 0;
        while (actors[i][n]!=',')
        {
            buffer += actors[i][n];
            n++;
        }
        if (buffer != test && buffer != "-1")
        {
            retval.push_back(actors[i]);
        }
        n = 0;
        buffer = "";
    }
    
    return retval;
}

vector<string> get_movies_by_type(vector<string> all_movies, string type)
{
    vector<string> retval;
    retval.push_back(all_movies[0]);
//    cout << "We must ask again: ";
//    string new_directory = directory_selector();
    string new_directory = "/Users/Bgrimes/Dropbox/SURE 2016 Team Folder/Output_Files";
    new_directory += "/MovieTypes/" + type + ".txt";
    vector<string> selected_movies = read_file(new_directory);
    
    string name = "";
    int n = 0;
    
    for (int i = 0; i < all_movies.size(); i++)
    {
        if(all_movies[i] == "")
        {
            continue;
        }
        while (all_movies[i][n] != ',')
        {
            name += all_movies[i][n];
            n++;
        }
        for (int j = 0; j < selected_movies.size(); j++)
        {
            if (name == selected_movies[j])
            {
                retval.push_back(all_movies[i]);
            }
        }
        name = "";
        n = 0;
    }

    return retval;
}

vector<string> read_file(string path)
{
    vector<string> retval;
    
    ifstream fin;
    fin.open(path);
    
    string This_Line;
    
    char new_line = get_new_line(path);
    
    while (fin.good())
    {
        getline(fin,This_Line,new_line);
        retval.push_back(This_Line);
    }
    
    return retval;
}
char get_new_line(string path)
{
    ifstream fin;
    fin.open(path);
    string This_Line;
    
    if (fin.good())
    {
        getline(fin,This_Line);
    }
    
    if (This_Line.find('\r') != string::npos)
    {
        return '\r';
    }
    else
        return '\n';
}
void print(string path,vector<string> data)
{
    ofstream fout;
    fout.open(path);
    
    for (int i = 0; i < data.size(); i++)
    {
        fout << data[i] << '\n';
    }
    
    if (data[0][0] == 'M')
    {
        int size = data.size();
        fout << endl << endl;
        fout << data[0] << endl;
        fout << "Totals:,,";
        
        string c[14] = {"B","C","D","E","F","G","H","I","J","K","L","M","N","O"};
        string output = "";
        
        for (int i = 1; i < 10; i++)
        {
            output += "=SUM(" + c[i] + "1:" + c[i] + to_string(size) + "),";
        }
        
        fout << output;
        output = "";
        fout << endl << "Averages,";
        
        for (int i = 0; i < 10; i++)
        {
            output += "=SUM(" + c[i] + "1:" + c[i] + to_string(size+1) + ")/" + to_string(size-1) + ",";
        }
        
        int num_male = num_whatever(data, 2);
        int num_female = num_whatever(data, 3);
        
        output += "=SUM(" + c[10] + "1:" + c[10] + to_string(size+1) + ")/" + to_string(num_male) + ",";
        output += "=SUM(" + c[11] + "1:" + c[11] + to_string(size+1) + ")/" + to_string(num_female) + ",";
        
        int num_male_BMI = num_whatever(data,13);
        int num_female_BMI = num_whatever(data,14);
        
        output += "=SUM(" + c[12] + "1:" + c[12] + to_string(size+1) + ")/" + to_string(num_male_BMI) + ",";
        output += "=SUM(" + c[13] + "1:" + c[13] + to_string(size+1) + ")/" + to_string(num_female_BMI) + ",";
        
        fout << output << endl;
    }
    fout.close();
}

void frequency_distribution(int start, int end, int interval, vector<string>& data, int num_commas)
{
    int n = (end - start)/interval;
    
    vector<int> distribution;
    vector<int> a;
    
    a.push_back(start);
    
    distribution.push_back(0);
    for (int i = 1; i < n+1; i++)
    {
        distribution.push_back(0);
        a.push_back((float)start+(float)i*(float)interval);
    }
    
    distribution.push_back(0);
    
    int counter = 0;
    string buffer;
    int m = 0;
    int value = 0;
    for (int i = 0; i < data.size(); i++)
    {
        buffer = "";
        counter = 0;
        m = 0;
        while (counter < num_commas)
        {
            if (data[i][m] == ',')
            {
                counter++;
            }
            m++;
        }
        while (data[i][m] != ',')
        {
            buffer += data[i][m];
            m++;
        }
        
        if (buffer == "BMI" || buffer == "Net Worth" || buffer == "Height" || buffer == "Weight")
        {
            continue;
        }
        
        value = stof(buffer);
        for (int j = 0; j < n+1; j++)
        {
            if (value < (float)a[j])
            {
                distribution[j]++;
                break;
            }
            else if (j == n)
            {
                distribution[n+1]++;
            }
        }
        
    }
    
    string to_add = "";
    
    to_add = "0 - " + to_string(a[0]) + "," + to_string(distribution[0]);
    data.push_back(to_add);
    for (int i = 0; i < n; i++)
    {
        to_add = to_string(a[i]) + " - " + to_string(a[i+1]) + " , " + to_string(distribution[i+1]);
        data.push_back(to_add);
    }
    to_add = to_string(a[n]) + "+ , " + to_string(distribution[n+1]);
    data.push_back(to_add);
}
int num_whatever(vector<string> data, int num_commas)
{
    int retval = data.size() - 1;
    int counter = 0;
    int n = 0;
    string buffer;
    for (int i = 0; i < data.size(); i++)
    {
        buffer = "";
        if(data[i] == "")
        {
            continue;
        }
        while (counter < num_commas)
        {
            if (data[i][n] == ',')
            {
                counter++;
            }
            n++;
        }
        counter = 0;
        while (data[i][n]!=',' && n < data[i].size())
        {
            buffer += data[i][n];
            n++;
        }
        if (buffer.find("BMI") != string::npos || buffer.find("Males") != string::npos || buffer.find("Females") != string::npos)
        {
            n = 0;
            continue;
        }
        float number = stof(buffer);
        if (number <= 0)
        {
            retval--;
        }
        n = 0;
    }
    
    return retval;
}