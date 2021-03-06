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
int TEST_COUNTER = 0;

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
    truck(size_t index) : index_(index) {};

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
    bool is_allowed(truck& t)
    {
        if (volume_ + t.get_volume() > settings::instance().get_volume_limit())
        {
            return false;
        }

        //if (weight_ + t.get_weight() > settings::instance().get_weight_limit())
        //{
        //    return false;
        //}
        return true;
    }
    bool is_allowed()
    {
        if (volume_ > settings::instance().get_volume_limit())
        {
            return false;
        }
        //if (weight_ > settings::instance().get_weight_limit())
        //{
        //    return false;
        //}

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

bool sort_parcels_by_weight(parcel l, parcel r)
{
    return l.weight > r.weight;
}

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
    unsigned int max_truck_load = round(parcels_average_weight * 0.2);
    settings::instance().set_load_limit(max_truck_load);

    cout << "weight" << endl << "total: " << parcels_total_weight << endl;
    cout << "average: " << parcels_average_weight << endl;
    cout << "max allowed load: " << max_truck_load << endl;
    cout << "parcels: " << parcels.size() << endl;

    sort(parcels.begin(), parcels.end(), sort_parcels_by_weight);
    try
    {
        size_t cnt = 1;
        for (auto box : parcels)
        {
            cout << cnt++ << ". " << box.index << ". " << static_cast<double>(box.weight) / DOUBLE_MULTIPLIER << ". w:" << box.weight << ". volume: " << box.volume << endl;

        }
    }
    catch (const std::exception& e)
    {
        cout << "cought exception" << e.what() << endl;
    }


    cout << endl << numeric_limits<int>::max() << endl;
    cout << numeric_limits<unsigned int>::max() << endl;
    cout << numeric_limits<ulli_t>::max() << endl;
    //answer: map<id parcel, id truck>
    return 0;
} // main

