#include <iostream>
#include<string>
#include<vector>
#include<cmath>
#include <cctype>
#include "Activity.h"
using namespace std;

string upper(string word)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (islower(word[i]))
        {
            word[i] = toupper(word[i]);
        }
    }
    return word;
}

Activity ReadActivity()
{
    string pre;
    int duration;
    
    char Name;
    cout << "Name: ";
    cin >> Name;
    Name = toupper(Name);
    cout << "predecessors(Enter them Concatonated || if there is none Enter 0):";
    cin >> pre;
    pre = upper(pre);
    cout << "duration: ";
    cin >> duration;


    Activity Act = Activity(Name, duration,pre);

    return Act;

}

vector <Activity> ReadActivties(int No_of_Activitys)
{
    vector <Activity> vActivites;
    for (int i = 1; i <= No_of_Activitys; i++)
    {
        cout << "--------------\n";
        vActivites.push_back(ReadActivity());
    }

    return vActivites;
}


void ForwardPass(vector<Activity>& Activitys) {
    int maxpreef;

    for (int i = 0; i < Activitys.size(); i++) {
        if (Activitys[i].pre == "0") {
            Activitys[i].early_start = 0;
        }
        else {
            maxpreef = -1;


            for (char preChar : Activitys[i].pre) {

                for (int j = 0; j < i; j++) {
                    if (Activitys[j].name == preChar) {
                        maxpreef = max(maxpreef, Activitys[j].early_Finish);
                    }
                }
            }

            Activitys[i].early_start = maxpreef;
        }

        Activitys[i].early_Finish = Activitys[i].early_start + Activitys[i].Duration;
    }
}

void BackwardPath(vector<Activity>& activities) {
    int n = activities.size();
    activities[n - 1].late_finish = activities[n - 1].early_Finish;
    activities[n - 1].late_start = activities[n - 1].late_finish - activities[n - 1].Duration;

    /*
        start from the activity that previous the last one and find the min late_start of its successors and assign it to its late_finish
    */
    for (int i = n - 2; i >= 0; --i) {
        int min_ls = INT_MAX;
        for (int j = i + 1; j < n; j++) {
            for (char prechar : activities[j].pre) {
                if (activities[i].name == prechar) {
                    min_ls = min(min_ls, activities[j].late_start);
                }
            }
        }
        // if min_ls == INT_MAX => this activity has no successors
        activities[i].late_finish = (min_ls == INT_MAX) ? activities[i].early_Finish : min_ls;
        activities[i].late_start = activities[i].late_finish - activities[i].Duration;
        activities[i].tf = abs(activities[i].late_finish - activities[i].early_Finish);
    }

}

vector<Activity> make_tf_vector(vector<Activity> activity)
{
    vector <Activity> vActivities_tf;
    for (int i = 0; i < activity.size(); i++)
    {
        if (activity[i].tf == 0) {
            vActivities_tf.push_back(activity[i]);
        }
    }
    return vActivities_tf;
}


vector<Activity> find_critical_path(vector<Activity> vActibitys_tf, vector<Activity> map_tf_vector)
{
    vector<Activity> result;
    int counter = 0;
    for (int i = 0; i < vActibitys_tf.size(); i++)
    {
        if (result.size() == 0)// search on start activity
        {
            for (int y = 0; y < map_tf_vector.size(); y++) //search on Activity in map
            {
                if (vActibitys_tf[i].name == map_tf_vector[y].name)
                {
                    if (map_tf_vector[y].pre == "0") //search on the value of activity if it is start
                    {
                        result.push_back(vActibitys_tf[i]);
                        vActibitys_tf[i].map_member = false; // remove from vector
                        map_tf_vector[y].map_member = false;// remove from map
                        i = 0;
                        counter++;
                    }
                    else
                    {
                        break;
                    }

                    if (result.size() > 0)//to remove any activity had same value
                    {
                        for (int j = 0; j < map_tf_vector.size(); j++)
                        {
                            if (map_tf_vector[j].map_member == true && map_tf_vector[j].pre == "0")
                            {
                                map_tf_vector[j].map_member == false;
                                counter++;
                            }
                        }
                        break;
                    }
                }

            }
        }

        else if (vActibitys_tf[i].map_member == true) {

            for (int j = 0; j < map_tf_vector.size(); j++)
            {
                if (map_tf_vector[j].name == vActibitys_tf[i].name && map_tf_vector[j].map_member == true)
                {
                    int old_lenght_result = result.size();
                    for (int y = 0; y < vActibitys_tf[i].pre.size(); y++)
                    {
                        if (vActibitys_tf[i].pre[y] == result[result.size() - 1].name)
                        {

                            result.push_back(vActibitys_tf[i]);
                            vActibitys_tf[i].map_member = false;
                            map_tf_vector[j].map_member = false;
                            counter++;
                            i = 0;
                            break;
                        }

                    }
                    if (result.size() > old_lenght_result)
                    {
                        for (int y = 0; y < map_tf_vector.size(); y++)
                        {
                            if (map_tf_vector[y].map_member == true)
                            {
                                for (char c : map_tf_vector[y].pre)
                                {
                                    if (c == result[result.size() - 2].name)
                                    {
                                        map_tf_vector[y].map_member = false;
                                        counter++;
                                    }
                                }
                            }
                        }

                    }
                    break;
                }
                else if (map_tf_vector[j].name == vActibitys_tf[i].name && map_tf_vector[j].map_member == false)
                {
                    vActibitys_tf[i].map_member = false;
                    break;
                }
            }
        }

        if (counter == map_tf_vector.size())
        {
            return result;
        }

    }
    return result;
}



int main()
{

    int No_of_Activitys;
    int total_dur = 0;

    cout << "Enter Number Of Activities : ";
    cin >> No_of_Activitys;

    vector<Activity> vActivitys = ReadActivties(No_of_Activitys);

    system("cls");

    ForwardPass(vActivitys);
    BackwardPath(vActivitys);
    vector<Activity> vActibitys_tf = make_tf_vector(vActivitys);
    vector<Activity> vMap_vectors = vActibitys_tf;

    vector<Activity> result = find_critical_path(vActibitys_tf, vMap_vectors);

    for (int i = 0; i < vActivitys.size(); i++)
    {
        cout <<"Activity " << vActivitys[i].name << "\n";
        cout << "------------------------------------------\n";
        cout << "Early Start For Activity " << vActivitys[i].name << " is :" << vActivitys[i].early_start << "\n";
        cout << "Early Finish For Activity " << vActivitys[i].name << " is :" << vActivitys[i].early_Finish << "\n";
        cout << "Late Start For Activity " << vActivitys[i].name << " is :" << vActivitys[i].late_start << "\n";
        cout << "Late Finish For Activity " << vActivitys[i].name << " is :" << vActivitys[i].late_finish << "\n";
        cout << "Total Float For Activity " << vActivitys[i].name << " is :" << vActivitys[i].tf << "\n";
        cout << "============================================================\n";
    }
    cout << "the critical path is: ";
    for (size_t i = 0; i < result.size(); i++)
    {
        cout << result[i].name;
        total_dur += result[i].Duration;
    }

    cout << "\nthe total duration = " << total_dur << "\n";
}