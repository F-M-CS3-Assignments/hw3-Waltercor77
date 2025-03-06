#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
#include <iomanip> // for setw, setfill
#include <chrono> // for sleep
#include <thread> // for sleep
#include "TimeCode.h" // for timecode's (duh)

using namespace std;

struct DryingSnapShot {
    // This is a struct, it's like an object
    // that doesn't have any methods.
    // You can read more about them in the ZyBook
    // just search for "struct"
    string name;
    time_t startTime;
    TimeCode *timeToDry;
};

long long int get_time_remaining(DryingSnapShot dss) {
    time_t currentTime = time(0);
    long long int totalDryTime = dss.startTime + dss.timeToDry->GetTimeCodeAsSeconds();
    return totalDryTime - currentTime;
}

string format_time_code(TimeCode* tc) {
    stringstream ss;
    ss << setfill('0') << setw(2) << tc->GetHours() << ":"
       << setfill('0') << setw(2) << tc->GetMinutes() << ":"
       << setfill('0') << setw(2) << tc->GetSeconds();
    return ss.str();
}

string drying_snap_shot_to_string(DryingSnapShot dss) {
    long long int timeRemaining = get_time_remaining(dss);
    
    if (timeRemaining <= 0) {
        return dss.name + " is dry!";
    }
    
    TimeCode remainingTC(
        timeRemaining / 3600,
        (timeRemaining % 3600) / 60,
        timeRemaining % 60
    );
    
    return dss.name + " will be dry in " + format_time_code(&remainingTC);
}

double get_sphere_sa(double rad) {
    return 4.0 * M_PI * rad * rad;
}

TimeCode* compute_time_code(double surfaceArea) {
    long long int totalSeconds = static_cast<long long int>(surfaceArea);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    return new TimeCode(hours, minutes, seconds);
}

void tests() {
    // get_time_remaining
    DryingSnapShot dss;
    dss.startTime = time(0);
    TimeCode tc = TimeCode(0, 0, 7);
    dss.timeToDry = &tc;
    long long int ans = get_time_remaining(dss);
    assert(ans > 6 && ans < 8);
    
    // get_sphere_sa
    double sa = get_sphere_sa(2.0);
    assert(50.2654 < sa && sa < 50.2655);
    
    // compute_time_code
    TimeCode* tc2 = compute_time_code(1.0);
    assert(tc2->GetTimeCodeAsSeconds() == 1);
    delete tc2;
    
    // est drying_snap_shot_to_string
    dss.name = "Test Batch";
    tc = TimeCode(0, 1, 0);  // 1 
    dss.timeToDry = &tc;
    dss.startTime = time(0);
    string status = drying_snap_shot_to_string(dss);
    assert(status.find("Test Batch will be dry in") != string::npos);
    
    cout << "ALL TESTS PASSED!" << endl;
}

void add_batch(vector<DryingSnapShot>& batches) {
    string batchName = "Batch-" + to_string(rand()); // Generate a random batch name
    double radius;
    
    cout << "Enter the radius of the items (in cm): ";
    cin >> radius;
    
    double surfaceArea = get_sphere_sa(radius);
    
    // Create a TimeCode object for drying time
    TimeCode* dryingTime = compute_time_code(surfaceArea);
    
    DryingSnapShot batch;
    batch.name = batchName;
    batch.startTime = time(0);
    batch.timeToDry = dryingTime;
    
    batches.push_back(batch);
    cout << batchName << " (takes " << format_time_code(dryingTime) << " to dry) added." << endl;
}

void view_batches(const vector<DryingSnapShot>& batches) {
    if (batches.empty()) {
        cout << "No batches to display." << endl;
        return;
    }
    
    for (const auto& batch : batches) {
        cout << drying_snap_shot_to_string(batch) << endl;
    }
    cout << batches.size() << " batches being tracked." << endl;
}
 // Clean up memory
void cleanup(vector<DryingSnapShot>& batches) {

	for (auto& batch : batches) {
		delete batch.timeToDry;
	}
}
int main() {
    vector<DryingSnapShot> batches;
    char option;

    tests(); // Run the tests before continuing with the main menu
    
    while (true) {
        cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
        cin >> option;
        
        if (option == 'A' || option == 'a') {
            add_batch(batches);
        } else if (option == 'V' || option == 'v') {
            view_batches(batches);
        } else if (option == 'Q' || option == 'q') {
            cout << "Exiting program..." << endl;
            break;
        }  
		else {
            cout << "Invalid option, please try again." << endl;
        }
        
        // Track drying progress and display it
        for (auto& batch : batches) {
            long long int timeRemaining = get_time_remaining(batch);
            if (timeRemaining > 0) {
                cout << drying_snap_shot_to_string(batch) << endl;
            } else {
                cout << batch.name << " is dry!" << endl;
            }
        }
        
        // Sleep for 1 second before the next iteration
        this_thread::sleep_for(chrono::seconds(1));
    }

	cleanup(batches);

    return 0;
}