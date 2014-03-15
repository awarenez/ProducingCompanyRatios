#ifndef CALCRATIOS_H
#define CALCRATIOS_H

#include "DataPoint.h"
#include <vector>
#include <map>

using namespace std;

class CalcRatios {
public:
    // Calculate Annual P/E
    vector<float> getAnnualPE(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Trailing P/E
    vector<float> getTrailingPE(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Quarterly P/E
    vector<float> getQuarterlyPE(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Year over year EPS growth
    vector<float> getYoyEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Year over year revenue growth
    vector<float> getYoyRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Quarter over Quarter EPS growth
    vector<float> getQoqEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap);

    // Calculate Quarter over Quarter revenue growth
    vector<float> getQoqRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap);
};

#endif