#include "CalcRatios.h"
#include "DataPoint.h"
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// P/E = current stock price over trailing EPS
vector<float> CalcRatios::getAnnualPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;

            // make sure there are four quarters
            int num_of_quarters = v.size();
            if (num_of_quarters != 4) continue;

            // Accumulate P/E in each quarter in an year
            float fAnnualPE = 0.0F; 
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                float fQuarterlyPE = v[i]->getLastPx() / v[i]->getEPS(); // P/E = stock price over EPS
                fAnnualPE += fQuarterlyPE;
            }

            res.emplace_back(fAnnualPE);
        }
    }

    return res;
}

vector<float> CalcRatios::getTrailingPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;

            // make sure there are four quarters
            int num_of_quaters = static_cast<int>(v.size()); 
            if (num_of_quaters != 4) continue;

            // Trailing P/E = current P/E divided by Trailing EPS
            float fCurrentPrice = v[0]->getLastPx();
            float fTrailingEPS = 0.0F;
            for (int i = 0; i < num_of_quaters; i++) {
                fTrailingEPS += v[i]->getEPS(); 
            }
            float fTrailingPE = fCurrentPrice / fTrailingEPS;
            res.emplace_back(fTrailingPE);
        }
    }

    return res;
}

vector<float> CalcRatios::getQuarterlyPE(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                float fQuarterlyPE = v[i]->getLastPx() / v[i]->getEPS(); // P/E = stock price over EPS
                res.emplace_back(fQuarterlyPE);
            }
        }
    }

    return res;
}

vector<float> CalcRatios::getYoyEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;

            // make sure there are four quarters
            int num_of_quarters = static_cast<int>(v.size());
            if (num_of_quarters != 4) continue;

            // Use the EPS on December to calculate yoy EPS growth(?)
            float fEPSSum = 0.0F;
            for (int i = 0; i < num_of_quarters; i++) {
                fEPSSum += v[i]->getEPS();
            }
            res.emplace_back(fEPSSum);
        }
    }

    // Calculate the growth needs at least two data
    int n = static_cast<int>(res.size());
    if (n < 2) return vector<float>();

    for (int i = 0; i < n - 1; i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // in percentage
    }
    res.pop_back();

    return res;
}

vector<float> CalcRatios::getYoyRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;
    
    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;

            // make sure there are four quarters
            int num_of_quarters = v.size();
            if (num_of_quarters != 4) continue;

            float yearlyRevenue = 0;
            for (int i = 0; i < num_of_quarters; i++) {
                yearlyRevenue += v[i]->getRevenue();
            }
            res.emplace_back(yearlyRevenue);
        }
    }

    // Calculate the growth needs at least two data
    int n = static_cast<int>(res.size());
    if (n < 2) return vector<float>();

    for (int i = 0; i < n - 1; i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}

vector<float> CalcRatios::getQoqEPSGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                res.emplace_back(v[i]->getEPS());
            }
        }
    }

    // Calculate the growth needs at least two data
    int n = static_cast<int>(res.size());
    if (n < 2) return vector<float>();

    for (int i = 0; i < n - 1; i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}

vector<float> CalcRatios::getQoqRevenueGrowth(map<int, vector<DataPoint*>, greater<int>> mymap) {
    vector<float> res;

    map<int, vector<DataPoint*>, greater<int>>::iterator mapit;
    for (mapit = mymap.begin(); mapit != mymap.end(); mapit++) {
        // get vector according to year
        if (mapit->first != -1) { // -1 means invalid year
            vector<DataPoint*> v = mapit->second;
            for (int i = 0; i < static_cast<int>(v.size()); i++) {
                if (v[i]->getMonth() % 3 != 0 || v[i]->getMonth() < 1 || v[i]->getMonth() > 12) continue;
                res.emplace_back(v[i]->getRevenue());
            }
        }
    }

    // Calculate the growth needs at least two data
    int n = static_cast<int>(res.size());
    if (n < 2) return vector<float>();

    for (int i = 0; i < n - 1; i++) {
        res[i] = (res[i] - res[i + 1]) / res[i + 1] * 100.0F; // growth in percentage
    }
    res.pop_back();

    return res;
}