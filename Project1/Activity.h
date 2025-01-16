#pragma once
#include <string>
using namespace std;
class Activity {

public:
Activity(char Name, int Duration, string pre);

char name;
int Duration;
int tf;
int early_start;
int early_Finish;
int late_start;
int late_finish;
string pre;
bool map_member = true;

};