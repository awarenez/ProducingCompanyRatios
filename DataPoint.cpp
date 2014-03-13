#include "DataPoint.h"

// Setters
DataPoint::DataPoint(int year, int month, int day, float fRevenue, float fEPS, float fLastPx, float fBVPS) :
                     m_year(year), m_month(month), m_day(day), m_fRevenue(fRevenue), m_fEPS(fEPS), m_fLastPx(fLastPx), m_fBVPS(fBVPS)
{
}

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

// Getters

