// Programmer: Walter Cordero
// This is my 6th c++ program

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "TimeCode.h"  

using namespace std;

// Splitting strings to substrings
vector<string> split(const string& str, char delim) {
    vector<string> result;
    stringstream ss(str);
    string item;
    while (getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

// function to parse a line from the CSV and return the TimeCode 
TimeCode parse_line(const string& line) {
    vector<string> columns = split(line, ',');
    
    // Look for a column that contains the time in UTC format
    for (const string& col : columns) {
        if (col.find("UTC") != string::npos) {
            string datetime_str = col;
            
            // find all spaces in the string
            vector<size_t> spaces;
            size_t pos = 0;
            while ((pos = datetime_str.find(" ", pos)) != string::npos) {
                spaces.push_back(pos);
                pos++;
            }
            
            //time is usualy right before utc, which should be after the last space
            if (spaces.size() >= 2) {
                // Get the text between the second-to-last space and the last space
                size_t start = spaces[spaces.size() - 2] + 1;
                size_t length = spaces[spaces.size() - 1] - start;
                string time_str = datetime_str.substr(start, length);
                
                // checks to see time bc it contains, :
                if (time_str.find(':') != string::npos) {
                    vector<string> time_parts = split(time_str, ':');
                    if (time_parts.size() >= 2) {
                        try {
                            int hours = stoi(time_parts[0]);
                            int minutes = stoi(time_parts[1]);
                            return TimeCode(hours, minutes, 0);
                        } catch (...) {
                            
                        }
                    }
                }
            }
        }
    }
    
    return TimeCode(0, 0, 0);
}

int main() {
    ifstream inputFile("Space_Corrected.csv"); //checks correct file 
    if (!inputFile) {
        cerr << "Error: Unable to open the file." << endl;
        return 1;
    }

    string line;
    vector<TimeCode> launchTimes;
    long long totalSeconds = 0;
    int dataPointCount = 0;

    while (getline(inputFile, line)) {
        TimeCode parsedTime = parse_line(line);
        
        if (parsedTime.GetHours() == 0 && parsedTime.GetMinutes() == 0 && parsedTime.GetSeconds() == 0) {
            continue; // Skip lines without valid time data
        }

        // total time in seconds
        totalSeconds += parsedTime.GetHours() * 3600 + parsedTime.GetMinutes() * 60 + parsedTime.GetSeconds();
        launchTimes.push_back(parsedTime);
        dataPointCount++;
    }

    cout << dataPointCount << " data points." << endl;
    if (dataPointCount == 0) {
        cout << "Error: No valid data points found." << endl;
        return 1; // exit if no data
    }

    //  the average time in seconds
    long long averageSeconds = totalSeconds / dataPointCount;
    int averageHours = averageSeconds / 3600;
    int averageMinutes = (averageSeconds % 3600) / 60;
    int averageSecondsRemaining = averageSeconds % 60;

    // average time
    cout << "AVERAGE: " << averageHours << ":" << averageMinutes << ":" << averageSecondsRemaining << endl;

    return 0;
}