#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>

using namespace std;
typedef unsigned long long int ulli_t;
const int TRUCKS = 100;
const int DOUBLE_MULTIPLIER = 1000000;

struct parcel
{
    size_t index;
    unsigned int weight;
    unsigned int volume;
}; // parcel
typedef shared_ptr<parcel> parcel_ptr;

class settings // singleton
{
public:
    inline static settings& instance()
    {
        static settings settings_instance;
        return settings_instance;
    }

    // delete copy and move constructors and assign operators
    settings(settings const&) = delete;
    settings(settings&&) = delete;
    settings& operator=(settings const&) = delete;
    settings& operator=(settings &&) = delete;

private:
    settings()
    {
        volume_limit_ = 100 * DOUBLE_MULTIPLIER;
    }

public:
    void set_load_limit(unsigned int wl)
    {
        load_limit_ = wl;
    }
    // getters
    size_t get_counter() const { return counter_; }
    unsigned int get_volume_limit() const { assert(volume_limit_ > 0); return volume_limit_; }
    unsigned int get_weight_limit() const { assert(load_limit_ > 0);  return load_limit_; }
private:
    size_t counter_ = 0;
    unsigned int volume_limit_;
    unsigned int load_limit_ = 0;
}; // settings

class truck
{
public:
    truck() {};

    void add_parcel(parcel_ptr p)
    {
        weight_ += p->weight;
        volume_ += p->volume;
        parcels_.push_back(p);
    }

    void remove_last_parcel()
    {
        parcel_ptr p = parcels_.back();
        weight_ -= p->weight;
        volume_ -= p->volume;
        parcels_.pop_back();
    }
    bool is_allowed()
    {
        if (volume_ > settings::instance().get_volume_limit())
        {
            return false;
        }
        if (weight_ > settings::instance().get_weight_limit())
        {
            return false;
        }

        return true;
    }
    // getters
    int get_index() const { return index_; }
    unsigned int get_weight() const { return weight_; }
    unsigned int get_volume() const { return volume_; }

private:
    int index_;
    vector<parcel_ptr> parcels_;
    unsigned int weight_;
    unsigned int volume_;
}; // truck

class file_read
{
public:
    file_read(vector<parcel>& parcels)
    {
        raad_parcels_vector(parcels);
    }
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
        size_t length = stoul(length_string);

        parcels.reserve(length);

        string line;
        double weight, volume;

        for (size_t i = 0; i < length && getline(infile, line); i++)
        {
            istringstream iss(line);
            if (!(iss >> weight >> volume))
            {
                cout << "FILE PARSING ERROR;";
                throw std::runtime_error("Not enough lines in sample file.");
            }
            unsigned int w = (unsigned int)round(weight*DOUBLE_MULTIPLIER);
            if (w <= 0)
            {
                throw std::runtime_error("weight is below 0 in file");
            }
            unsigned int v = (unsigned int)round(volume*DOUBLE_MULTIPLIER);
            if (v <= 0)
            {
                throw std::runtime_error("volume is below 0 in file");
            }
            parcels.push_back({ i, w, v });
        }
    }
}; // file_read

void load_parcel(vector<truck>& trucks, vector<parcel>& parcels, size_t parcels_loaded = 0)
{
    ++parcels_loaded;

    if (parcels.size() == parcels_loaded)
    {
        // TODO add success code here
        cout << "success!!!!";
        throw::runtime_error("SUCCESS");
        return;
    }

    parcel box = parcels.at(parcels_loaded);
    // try creating truck for the first time
    if (trucks.size() < TRUCKS)
    {
        truck tr;
        tr.add_parcel(make_shared<parcel>(box));
        trucks.push_back(tr);
        load_parcel(trucks, parcels, parcels_loaded);
        --parcels_loaded;
        trucks.pop_back();
    }

    // add the parcel to each track
    for (truck& tr : trucks)
    {
        tr.add_parcel(make_shared<parcel>(box));

        if (tr.is_allowed())
        {
            load_parcel(trucks, parcels, parcels_loaded);
            --parcels_loaded;
        }
        tr.remove_last_parcel();
    }
}; // load_parcel

int main()
{
    vector<truck> trucks;
    vector<parcel> parcels;

    file_read read(parcels);

    ulli_t parcels_total_weight = 0;
    for (auto& p : parcels)
    {
        parcels_total_weight += p.weight;
    }
    double average = static_cast<double>(parcels_total_weight) / parcels.size();
    unsigned int parcels_average_weight = static_cast<unsigned int>(round(average));
    unsigned int max_truck_load = parcels_average_weight + static_cast<int>(0.5 * DOUBLE_MULTIPLIER);
    settings::instance().set_load_limit(max_truck_load);

    cout << "weight" << endl << "total: " << parcels_total_weight << endl;
    cout << "average: " << parcels_average_weight << endl;
    cout << "max allowed load: " << max_truck_load << endl;
    cout << "parcels: " << parcels.size() << endl;
    try
    {
        load_parcel(trucks, parcels);
    }
    catch (const std::exception&)
    {
        cout << "cought exception" << endl;
    }


    cout << endl << numeric_limits<int>::max() << endl;
    cout << numeric_limits<unsigned int>::max() << endl;
    cout << numeric_limits<ulli_t>::max() << endl;
    //answer: map<id parcel, id truck>
    return 0;
} // main

