//This is my 5th c++ program
//Programmer = Walter Cordero

#include <iostream>
#include <assert.h>
using namespace std;

#include "TimeCode.h"

//Constructor 
//t, total time in secs, takes all parameters and calculates them to secs
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec){
    t= (hr*60*60)+(min*60)+(sec);
}

//constructor 2.0
TimeCode::TimeCode(const TimeCode& tc){
    t=tc.t;
}

//converts the hours to secs, doesn't change anything else
void TimeCode::SetHours(unsigned int hours){
    t = (hours * 60 * 60) + (t % 3600);
}
//does the same as above but for mins instead
void TimeCode::SetMinutes(unsigned int minutes){
    t = (t / 3600) * 3600 + (minutes * 60) + (t % 60);
}

//once again, same type of conversion but for secs
void TimeCode::SetSeconds(unsigned int seconds){
    t = (t / 60) * 60 + seconds;
}

//time resets to being 0
void TimeCode::reset(){
    t = 0;
}


//gets the new hours in secs, to convert them to hours 
unsigned int TimeCode::GetHours() const{
    return t / 3600;
}

//does the same however not for mins, changing them back to such
unsigned int TimeCode::GetMinutes() const{
    return (t % 3600) / 60;
}

//keeps the number of seconds
unsigned int TimeCode::GetSeconds() const{
    return t % 60;
}

//gets all values for hr, min, sec
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const{
    hr = GetHours();
    min = GetMinutes();
    sec = GetSeconds();
}

//converts all parameters to seconds 
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec){
    return (hr * 3600) + (min * 60) + sec;
}

//converts all the secs into readable numbers split into the parameters
string TimeCode::ToString() const{
    return to_string(GetHours()) + ":" + to_string(GetMinutes()) + ":" + to_string(GetSeconds());
}

//adds two objects together
TimeCode TimeCode::operator+(const TimeCode& other) const{
    return TimeCode(0,0, t+other.t);
}

//subtracts objects from eachother disallowing -values
TimeCode TimeCode::operator-(const TimeCode& other) const{
    return TimeCode(0, 0, t-other.t);
}

//multiplies time 
TimeCode TimeCode::operator*(double a) const{
    return TimeCode(0, 0, static_cast<long long unsigned int>(t * a));

}

//divides time 
TimeCode TimeCode::operator/(double a) const{
    return TimeCode(0, 0, static_cast<long long unsigned int>(t / a));

}

//checks if the objects are the same 
bool TimeCode::operator == (const TimeCode& other) const{
    return t == other.t;
}

//checks if such objects are different
bool TimeCode::operator != (const TimeCode& other) const{
    return t != other.t;
}

//checks if one of them is less than the other 
bool TimeCode::operator < (const TimeCode& other) const{
    return t < other.t;
}

//checks if one of them is less than or equal to the other 
bool TimeCode::operator <= (const TimeCode& other) const{
    return t <= other.t;
}

//checks if one of them is more than the other 
bool TimeCode::operator > (const TimeCode& other) const{
    return t > other.t;
}

//checks if one of them is more than or equal to the other 
bool TimeCode::operator >= (const TimeCode& other) const{
    return t >= other.t;
}



