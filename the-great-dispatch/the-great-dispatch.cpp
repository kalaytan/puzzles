#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
struct parcel
{
    int index;
    unsigned int weight;
    unsigned int volume;
};

struct truck
{
    int index;
    vector<int> boxes;
    unsigned int weight;
    unsigned int volume;
};

int double_multiplyer = 1000000;

class file_read
{
public:
    file_read(vector<parcel>& parcels)
    {
        raad_parcels_vector(parcels);
    }
    ~file_read() {};
private:
    void raad_parcels_vector(vector<parcel>& parcels)
    {
        std::ifstream infile("C:/projects/puzzle/the-great-dispatch/CG-Dispatch-Tests/test.01");
        if (!infile.good())
        {
            throw std::runtime_error("Failed to read sample file.");
        }
        string length_string;
        getline(infile, length_string);
        int length = stoi(length_string);

        parcels.reserve(length);

        string line;
        double weight, volume;

        for (int i = 0; i < length && getline(infile, line); i++)
        {
            istringstream iss(line);
            if (!(iss >> weight >> volume))
            {
                cout << "FILE PARSING ERROR;";
                throw std::runtime_error("Not enough lines in sample file.");
            }
            unsigned int w = (unsigned int)round(weight*double_multiplyer);
            if (w <= 0)
            {
                throw std::runtime_error("weight is below 0 in file");
            }
            unsigned int v = (unsigned int)round(volume*double_multiplyer);
            if (v <= 0)
            {
                throw std::runtime_error("volume is below 0 in file");
            }
            parcels.push_back({ i, w, v });
        }
    }
};



int main()
{

    // <unsigned int>::max -> 4,294,967,295


    vector <truck> trucks;
    vector <parcel> parcels;

    file_read read(parcels);

    cout << numeric_limits<unsigned long int>::max();
    //answer: map<id parcel, id truck>
    return 0;
}

