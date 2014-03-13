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
#include <vector>
#include <string>
#include <cstdio>
#include <memory>

#include "DataPoint.h"

using namespace std;

enum FIELDS { DATE = 1, REVENUE, EPS, LASTPX, BVPS };
enum DATE { MONTH = 1, DAY, YEAR };

void SetDate(DataPoint* pd, const string &s, int index) {
    if (!pd) return;
    switch (index) {
    case DAY:
        //cout << "Day: " << atoi(s.c_str()) << endl;
        pd->setDay(atoi(s.c_str()));
        break;
    case MONTH:
        //cout << "Month: " << atoi(s.c_str()) << endl;
        pd->setMonth(atoi(s.c_str()));
        break;
    case YEAR:
        //cout << "Year: " << atoi(s.c_str()) << endl;
        pd->setYear(atoi(s.c_str()));
        break;
    default:
        break;
    }
}

void SetField(DataPoint* pd, const string &field, int index) {
    if (!pd) {
        cout << "pd is null" << endl;
        return;
    }
    
    const char * cfield = field.c_str();
    int start = 0, end = 0, count = 1;
    
    switch (index) {
    case DATE:
        // Get month, day, year
        while ((end = static_cast<int>(field.find("/", start))) != std::string::npos) {
            SetDate(pd, field.substr(start, end - start), count);
            start = end + 1;
            ++count;
        }
        SetDate(pd, field.substr(start), count);
        //cout << field.substr(start) << endl;;
        break;
    case REVENUE:
        pd->setRevenue(static_cast<float>(atof(cfield)));
        break;
    case EPS:
        pd->setEPS(static_cast<float>(atof(cfield)));
        break;
    case LASTPX:
        pd->setLastPx(static_cast<float>(atof(cfield)));
        break;
    case BVPS:
        pd->setBVPS(static_cast<float>(atof(cfield)));
        break;
    default:
        break;
    }
}

vector<float> getAnnualPE(const vector<DataPoint*> &v) {
    vector<float> res;
    return res;
}

vector<float> getTrailingPE(const vector<DataPoint*> &v) {
    vector<float> res;
    return res;
}

int main(int argc, const char * argv[])
{
    // Read Data Points
    vector<DataPoint*> v;
    
    // FILE I/O
    //string input_fname("D:/Projects/ProducingCompanyRatios/input[1].txt");
    string input_fname(argv[1]);
    ifstream ifs;
    ifs.open(input_fname.c_str());
    string line;
    if (ifs.is_open()) { // Read header
        getline(ifs, line);
        //cout << line << endl;

        // Read data from each line
        while (ifs.good()) {
            getline(ifs, line);
            int start = 0, end = 0, nFieldIdx = 1;
            DataPoint* pd = new DataPoint;
            while ((end = static_cast<int>(line.find("\t",start))) != string::npos) {
                string field = line.substr(start, end - start);
                //cout << field << endl;
                SetField(pd, field, nFieldIdx);
                
                start = end + 1;
                ++nFieldIdx;
            }
            SetField(pd, line.substr(start), nFieldIdx);
            v.push_back(pd);
            //cout << line.substr(start) << endl; // BVPS
            //cout << line.c_str() << endl;
        }
    }

    int nDataPoints = static_cast<int>(v.size());
    cout << "Data Points: " << nDataPoints << endl;
    // Calculations
    for (int i = 0; i < nDataPoints; i++) {
        cout << v[i]->getMonth() << "/" << v[i]->getDay() << "/" << v[i]->getYear() << " " <<
             v[i]->getRevenue() << " " << v[i]->getEPS() << " " << v[i]->getLastPx() << " " <<
             v[i]->getBVPS() << endl;
    }
    
    // Get Annual P/E
    vector<float> AnnualPE = getAnnualPE(v);
    
    // Get Tailing P/E
    vector<float> TrailingPE = getTrailingPE(v);

    // Delete Data Points
    for (int i = 0; i < nDataPoints; i++) {
        delete v[i];
    }
    
    return 0;
}

