#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;
typedef unsigned long long int ulli_t;
struct parcel
{
    size_t index;
    unsigned int weight;
    unsigned int volume;
};
typedef shared_ptr<parcel> parcel_ptr;

class helping_vars
{
private:
    unsigned int double_multiplier_ = 1000000;
    size_t counter_ = 0;
    unsigned int volume_limit_;
    unsigned int weight_limit_ = 0;

public:
    helping_vars()
    {
        volume_limit_ = 100 * double_multiplier_;
    }
    void set_weight_limit(int wl)
    {
        weight_limit_ = wl;
    }
    // getters
    size_t get_counter() const { return counter_; }
    unsigned int get_double_multiplier() const { return double_multiplier_; }
    unsigned int get_volume_limit() const { return volume_limit_; }
    unsigned int get_weight_limit() const { return weight_limit_; }
};

class truck
{
public:
    //truck(size_t i, parcel_ptr p)
    //{
    //    //parcels_.reserve(10);
    //}

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
        if (volume_ > help_vars_.get_volume_limit())
        {
            return false;
        }
        // TODO implement weight limit
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
    unsigned int max_volume_;
    // todo implement max_weight_ func.
    unsigned int max_weight_;
    helping_vars help_vars_;
};

class file_read
{
public:
    file_read(vector<parcel>& parcels)
    {
        raad_parcels_vector(parcels);
    }
    //~file_read() {};
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

        helping_vars help_vars;
        unsigned int multiplier = help_vars.get_double_multiplier();

        for (size_t i = 0; i < length && getline(infile, line); i++)
        {
            istringstream iss(line);
            if (!(iss >> weight >> volume))
            {
                cout << "FILE PARSING ERROR;";
                throw std::runtime_error("Not enough lines in sample file.");
            }
            unsigned int w = (unsigned int)round(weight*multiplier);
            if (w <= 0)
            {
                throw std::runtime_error("weight is below 0 in file");
            }
            unsigned int v = (unsigned int)round(volume*multiplier);
            if (v <= 0)
            {
                throw std::runtime_error("volume is below 0 in file");
            }
            parcels.push_back({ i, w, v });
        }
    }
};

void load_parcel(vector<truck>& trucks, vector<parcel>& parcels, size_t parcels_loaded = 0)
{
    //++counter;

    // increment parcels_loaded for current try.
    ++parcels_loaded;

    if (parcels.size() == parcels_loaded)
    {
        // TODO add success code here
        throw::runtime_error("SUCCESS");
        return;
    }

    parcel box = parcels.at(parcels_loaded);

    // try creating truck for the first time
    if (trucks.size() < 100)
    {
        truck tr;
        tr.add_parcel(make_shared<parcel>(box));
        trucks.push_back(tr);
        load_parcel(trucks, parcels, parcels_loaded);
        --parcels_loaded;
        trucks.pop_back();
    }

    // add the parcel to each track
    for (truck tr : trucks)
    {
        tr.add_parcel(make_shared<parcel>(box));

        if (tr.is_allowed())
        {
            load_parcel(trucks, parcels, parcels_loaded);
            --parcels_loaded;
        }
        tr.remove_last_parcel();
    }

}

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

    load_parcel(trucks, parcels);

    cout << "weight" << endl << "total: " << parcels_total_weight << endl;
    cout << "average: " << parcels_average_weight << endl;
    cout << "parcels: " << parcels.size() << endl;

    cout << numeric_limits<ulli_t>::max();
    //answer: map<id parcel, id truck>
    return 0;
}

