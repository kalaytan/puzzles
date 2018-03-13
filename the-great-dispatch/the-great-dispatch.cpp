#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
    struct parcel
    {
        size_t index;
        size_t truck;
        double weight;
        double volume;
    };

    struct truck
    {
        size_t index;
        vector<size_t> boxes;
    };

    vector <truck> trucks;
    //
    // Read the file into parcels vector
    //
    std::ifstream infile("C:/projects/puzzle/the-great-dispatch/CG-Dispatch-Tests/test.01");
    if (!infile.good())
    {
        cout << "Failed to read sample file." << endl;
        return 1;
    }
    string length;
    getline(infile, length);

    vector <parcel> parcels(stoi(length));

    string line;
    double weight, volume;
    for (size_t n = 0; n < stoi(length) && getline(infile, line); n++)
    {
        istringstream iss(line);
        if (!(iss >> weight >> volume))
        {
            cout << "FILE PARSING ERROR;";
            throw std::runtime_error("Not enough lines in sample file.");
        }
        parcels[n] = { n, numeric_limits<size_t>::max(), weight, volume };
    }




    return 0;
}
