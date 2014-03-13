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

#include "DataPoint.h"

using namespace std;

enum FIELDS { DATE = 1, REVENUE, EPS, LASTPX, BVPS };
enum DATE { MONTH = 1, DAY, YEAR };

void SetDate(DataPoint* pd, const string &s, int index) {
    if (!pd) return;
    switch (index) {
    case DAY:
        pd->setDay(atoi(s.c_str()));
        break;
    case MONTH:
        pd->setMonth(atoi(s.c_str()));
        break;
    case YEAR:
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

// P/E = current stock price over trailing EPS
vector<float> getAnnualPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            int num_of_quarters = v.size();
            if (num_of_quarters != 4) continue; // make sure there are four quarters
            float fTrailingPE = 0.0F;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                float fQuarterlyPE = v[i]->getLastPx() / v[i]->getEPS(); // P/E = stock price over EPS
                fTrailingPE += fQuarterlyPE;
            }
            res.emplace_back(fTrailingPE);
        }
    }
    return res;
}

vector<float> getTrailingPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            int num_of_quaters = static_cast<int>(v.size()); 
            if (num_of_quaters != 4) continue;
            float fCurrentPrice = v[0]->getLastPx();
            float fTrailingEPS = 0.0F;
            for (int i = 0; i < num_of_quaters; i++) {
                fTrailingEPS += v[i]->getEPS(); 
            }
            float fAnnualPE = fCurrentPrice / fTrailingEPS;
            res.emplace_back(fAnnualPE);
        }
    }
    return res;
}

vector<float> getQuarterlyPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            float fTrailingPE = 0.0F;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                float fQuarterlyPE = v[i]->getLastPx() / v[i]->getEPS(); // P/E = stock price over EPS
                res.emplace_back(fQuarterlyPE);
            }
        }
    }
    return res;
}

vector<float> getYoyEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            int num_of_quarters = static_cast<int>(v.size());
            if (num_of_quarters != 4) continue; // make sure there are four quarters
            for (int i = 0; i < num_of_quarters; i++) {
                if (v[i]->getMonth() == 12) {
                    res.emplace_back(v[i]->getEPS());
                }
            }
        }
    }

    for (int i = 0; i < static_cast<int>(res.size() - 1); i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // in percentage
    }
    res.pop_back();

    return res;
}

vector<float> getYoyRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            int num_of_quarters = v.size();
            if (num_of_quarters != 4) continue; // make sure there are four quarters
            float yearlyRevenue = 0;
            for (int i = 0; i < num_of_quarters; i++) {
                yearlyRevenue += v[i]->getRevenue();
            }
            res.emplace_back(yearlyRevenue);
        }
    }

    for (int i = 0; i < static_cast<int>(res.size() - 1); i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}

vector<float> getQoqEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                res.emplace_back(v[i]->getEPS());
            }
        }
    }

    for (int i = 0; i < static_cast<int>(res.size() - 1); i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}

vector<float> getQoqRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) {
            vector<DataPoint*> v = mapit->second;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                res.emplace_back(v[i]->getRevenue());
            }
        }
    }

    for (int i = 0; i < static_cast<int>(res.size() - 1); i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}

int main(int argc, const char * argv[])
{
    // Read Data Points
    map<int, vector<DataPoint*>, greater<int>> datamap; // use year as key
    
    // FILE I/O
    string input_fname("D:/Projects/ProducingCompanyRatios/input[1].txt");
    //string input_fname(argv[1]);
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
            while ((end = static_cast<int>(line.find("\t",start))) != string::npos) {
                string field = line.substr(start, end - start);
                SetField(pd, field, nFieldIdx);
                start = end + 1;
                ++nFieldIdx;
            }
            SetField(pd, line.substr(start), nFieldIdx);
            datamap[pd->getYear()].emplace_back(pd);
        }
    }

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = datamap.begin(); mapit != datamap.end(); mapit++) {
        vector<DataPoint*> v = mapit->second;
        for (int i = 0; i < static_cast<int>(v.size()); i++) {
             cout << v[i]->getMonth() << "/" << v[i]->getDay() << "/" << v[i]->getYear() << " " <<
             v[i]->getRevenue() << " " << v[i]->getEPS() << " " << v[i]->getLastPx() << " " <<
             v[i]->getBVPS() << endl;
        }
    }
    cout << endl << endl;

    // TODO: Sort the data in each year

    
    // Get Annual P/E
    vector<float> annualPE = getAnnualPE(datamap);
    cout << "Annual P/E: ";
    for (int i = 0; i < static_cast<int>(annualPE.size()); i++) {
        cout << annualPE[i] << ", ";
    }
    cout << endl;
    
    // Get Tailing P/E
    vector<float> trailingPE = getTrailingPE(datamap);
    cout << "Trailing P/E: ";
    for (int i = 0; i < static_cast<int>(trailingPE.size()); i++) {
        cout << trailingPE[i] << ", ";
    }
    cout << endl;

    //Get Quarterly P/E
    vector<float> quarterlyPE = getQuarterlyPE(datamap);
    cout << "Quarterly P/E: ";
    for (int i = 0; i < static_cast<int>(quarterlyPE.size()); i++) {
        cout << quarterlyPE[i] << ", ";
    }
    cout << endl;


    // Get year over year EPS growth
    vector<float> yoyEPSGrowth = getYoyEPSGrowth(datamap);
    cout << "Yoy EPS Growth: ";
    for (int i = 0; i < static_cast<int>(yoyEPSGrowth.size()); i++) {
        cout << yoyEPSGrowth[i] << ", ";
    }
    cout << endl;

    // Get year over year Revenue growth
    vector<float> yoyRevenueGrowth = getYoyRevenueGrowth(datamap);
    cout << "Yoy Revenue Growth: ";
    for (int i = 0; i < static_cast<int>(yoyRevenueGrowth.size()); i++) {
        cout << yoyRevenueGrowth[i] << ", ";
    }
    cout << endl;

    // Get quarter over quarter EPS growth
    vector<float> qoqEPSGrowth = getQoqEPSGrowth(datamap);
    cout << "Qoq EPS Growth: ";
    for (int i = 0; i < static_cast<int>(qoqEPSGrowth.size()); i++) {
        cout << qoqEPSGrowth[i] << ", ";
    }
    cout << endl;

    // Get quarter over quater revenue growth
    vector<float> qoqRevenueGrowth = getQoqRevenueGrowth(datamap);
    cout << "Qoq Revenue Growth: ";
    for (int i = 0; i < static_cast<int>(qoqRevenueGrowth.size()); i++) {
        cout << qoqRevenueGrowth[i] << ", ";
    }
    cout << endl;
    
    // Delete Data Points in map
    for (mapit = datamap.begin(); mapit != datamap.end(); mapit++) {
        vector<DataPoint*> v = mapit->second;
        for (int i = 0; i < static_cast<int>(v.size()); i++) {
            delete v[i];
        }
    }

    return 0;
}

