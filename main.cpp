//
//  main.cpp
//  ProducingCompanyRatios
//
//  Created by Chih-Yang Chang on 2/28/14.
//  Copyright (c) 2014 Chih-Yang Chang. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>

#include <vector>
#include <map>
#include <algorithm>

#include "DataPoint.h"
#include "CalcRatios.h"

using namespace std;

int comparator(DataPoint* A, DataPoint* B) {
    if (A->getYear() == B->getYear()) {
        if (A->getMonth() == B->getMonth()) {
            return A->getDay() > B->getDay();
        } else {
            return A->getMonth() > B->getMonth();
        }
    } else {
        return A->getYear() > B->getYear();
    }
}

int main(int argc, const char * argv[])
{
    // Data structure
    map<int, vector<DataPoint*>, greater<int>> datamap; // use year as key
    
    // FILE I/O
    string input_fname(argv[1]);

    ifstream ifs;
    ifs.open(input_fname.c_str());
    string line;
    if (ifs.is_open()) {
        // Read header
        getline(ifs, line);

        // Read data from each line
        while (ifs.good()) {
            getline(ifs, line);
            int start = 0, end = 0, nFieldIdx = 1;
            DataPoint* pd = new DataPoint;

            // Set fields for each data
            while ((end = static_cast<int>(line.find("\t",start))) != string::npos) {
                string field = line.substr(start, end - start);
                pd->SetField(field, nFieldIdx);
                start = end + 1;
                ++nFieldIdx;
            }
            pd->SetField(line.substr(start), nFieldIdx);
            datamap[pd->getYear()].emplace_back(pd);
        }
    }

    // sort the data in each year in case the input data is not sorted
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = datamap.begin(); mapit != datamap.end(); mapit++) {
        // sort data in each year
        vector<DataPoint*> *v = &mapit->second;
        sort(v->begin(), v->end(), comparator);
    }

    CalcRatios calculator;

    // Get Annual P/E
    vector<float> annualPE = calculator.getAnnualPE(datamap);
    cout << "Annual P/E: " << endl;
    for (int i = 0; i < static_cast<int>(annualPE.size()); i++) {
        cout << annualPE[i] << ", ";
    }
    cout << endl << endl;
    
    // Get Tailing P/E
    vector<float> trailingPE = calculator.getTrailingPE(datamap);
    cout << "Trailing P/E: " << endl;
    for (int i = 0; i < static_cast<int>(trailingPE.size()); i++) {
        cout << trailingPE[i] << ", ";
    }
    cout << endl << endl;

    //Get Quarterly P/E
    vector<float> quarterlyPE = calculator.getQuarterlyPE(datamap);
    cout << "Quarterly P/E: " << endl;
    for (int i = 0; i < static_cast<int>(quarterlyPE.size()); i++) {
        cout << quarterlyPE[i] << ", ";
    }
    cout << endl << endl;

    // Get year over year EPS growth
    vector<float> yoyEPSGrowth = calculator.getYoyEPSGrowth(datamap);
    cout << "Yoy EPS Growth: " << endl;
    for (int i = 0; i < static_cast<int>(yoyEPSGrowth.size()); i++) {
        cout << yoyEPSGrowth[i] << ", ";
    }
    cout << endl << endl;

    // Get year over year Revenue growth
    vector<float> yoyRevenueGrowth = calculator.getYoyRevenueGrowth(datamap);
    cout << "Yoy Revenue Growth: " << endl;
    for (int i = 0; i < static_cast<int>(yoyRevenueGrowth.size()); i++) {
        cout << yoyRevenueGrowth[i] << ", ";
    }
    cout << endl << endl;

    // Get quarter over quarter EPS growth
    vector<float> qoqEPSGrowth = calculator.getQoqEPSGrowth(datamap);
    cout << "Qoq EPS Growth: " << endl;
    for (int i = 0; i < static_cast<int>(qoqEPSGrowth.size()); i++) {
        cout << qoqEPSGrowth[i] << ", ";
    }
    cout << endl << endl;

    // Get quarter over quater revenue growth
    vector<float> qoqRevenueGrowth = calculator.getQoqRevenueGrowth(datamap);
    cout << "Qoq Revenue Growth: " << endl;
    for (int i = 0; i < static_cast<int>(qoqRevenueGrowth.size()); i++) {
        cout << qoqRevenueGrowth[i] << ", ";
    }
    cout << endl << endl;
    
    // Verify data points
    //for (mapit = datamap.begin(); mapit != datamap.end(); mapit++) {
    //    vector<DataPoint*> v = mapit->second;
    //    for (int i = 0; i < static_cast<int>(v.size()); i++) {
    //         cout << v[i]->getMonth() << "/" << v[i]->getDay() << "/" << v[i]->getYear() << " " <<
    //         v[i]->getRevenue() << " " << v[i]->getEPS() << " " << v[i]->getLastPx() << " " <<
    //         v[i]->getBVPS() << endl;
    //    }
    //}
    //cout << endl << endl;

    // Delete Data Points in map
    for (mapit = datamap.begin(); mapit != datamap.end(); mapit++) {
        vector<DataPoint*> v = mapit->second;
        for (int i = 0; i < static_cast<int>(v.size()); i++) {
            delete v[i];
        }
    }

    return 0;
}

