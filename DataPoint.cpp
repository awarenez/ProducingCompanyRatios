#include "DataPoint.h"
#include <string>

using namespace std;

enum FIELDS { DATE = 1, REVENUE, EPS, LASTPX, BVPS };
enum DATE { MONTH = 1, DAY, YEAR };

DataPoint::DataPoint(int year, int month, int day, float fRevenue, float fEPS, float fLastPx, float fBVPS) :
                     m_year(year), m_month(month), m_day(day), m_fRevenue(fRevenue), m_fEPS(fEPS), m_fLastPx(fLastPx), m_fBVPS(fBVPS)
{
}

// Setters
void DataPoint::setDay(int day) {
    m_day = day;
}

void DataPoint::setMonth(int month) {
    m_month = month;
}

void DataPoint::setYear(int year) {
    m_year = year;
}

void DataPoint::setRevenue(float revenue) {
    m_fRevenue = revenue;
}

void DataPoint::setEPS(float fEPS) {
    m_fEPS = fEPS;
}

void DataPoint::setLastPx(float fLastPx) {
    m_fLastPx = fLastPx;
}

void DataPoint::setBVPS(float fBVPS) {
    m_fBVPS = fBVPS;
}

void DataPoint::SetDate(const string &s, int index) {
    switch (index) {
    case DAY:
        setDay(atoi(s.c_str()));
        break;
    case MONTH:
        setMonth(atoi(s.c_str()));
        break;
    case YEAR:
        setYear(atoi(s.c_str()));
        break;
    default:
        break;
    }
}

void DataPoint::SetField(const string &field, int index) {
    const char * cfield = field.c_str();
    int start = 0, end = 0, count = 1;
    
    switch (index) {
    case DATE:
        // Get month, day, year
        while ((end = static_cast<int>(field.find("/", start))) != std::string::npos) {
            SetDate(field.substr(start, end - start), count);
            start = end + 1;
            ++count;
        }
        SetDate(field.substr(start), count);
        break;
    case REVENUE:
        setRevenue(static_cast<float>(atof(cfield)));
        break;
    case EPS:
        setEPS(static_cast<float>(atof(cfield)));
        break;
    case LASTPX:
        setLastPx(static_cast<float>(atof(cfield)));
        break;
    case BVPS:
        setBVPS(static_cast<float>(atof(cfield)));
        break;
    default:
        break;
    }
}

// Getters
int DataPoint::getDay() {
    return m_day;
}

int DataPoint::getMonth() {
    return m_month;
}

int DataPoint::getYear() {
    return m_year;
}

float DataPoint::getRevenue() {
    return m_fRevenue;
}

float DataPoint::getEPS() {
    return m_fEPS;
}

float DataPoint::getLastPx() {
    return m_fLastPx;
}

float DataPoint::getBVPS() {
    return m_fBVPS;
}