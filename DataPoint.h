#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>

using namespace std;

class DataPoint {
    // data fields
    int m_year;
    int m_month;
    int m_day;
    float m_fRevenue;
    float m_fEPS;
    float m_fLastPx;
    float m_fBVPS;
public:
    DataPoint(int year = -1, int month = -1, int day = -1, float fRevenue = -1, float fEPS = -1.0F , float fLastPx = -1.0F, float m_fBVPS = -1.0F);

    // Setters
    void setDay(int day);
    void setMonth(int month);
    void setYear(int year);
    void setRevenue(float revenue);
    void setEPS(float fEPS);
    void setLastPx(float fLastPx);
    void setBVPS(float fBVPS);
    void SetDate(const string &s, int index);
    void SetField(const string &field, int index);

    // Getters
    int getDay();
    int getMonth();
    int getYear();
    float getRevenue();
    float getEPS();
    float getLastPx();
    float getBVPS();
};

#endif