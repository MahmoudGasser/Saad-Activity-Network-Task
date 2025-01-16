#include "Activity.h"
using namespace std;

Activity::Activity(char Name, int Duration, string pre)
{
    this->pre = pre;
    this->name = Name;
    this->Duration = Duration;
}

