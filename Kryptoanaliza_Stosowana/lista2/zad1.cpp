#include<bits/stdc++.h>

using namespace std;

const int STR_LEN = 6;

set<string> name_map;




void gen_names(int pos, string &curr_name)
{   
    if (name_map.find(curr_name) == name_map.end())
    {
        name_map.insert(curr_name);
    }

    if (pos == STR_LEN)
        return;

    for(int i = 'a'; i <= 'z'; i++)
    {
       curr_name.push_back((char)i);
       gen_names(pos+1, curr_name);
       curr_name.pop_back(); 
    }
    return;
}
int main()
{
    string foo = "";
    gen_names(0, foo);

    cout << name_map.size() << "\n";
    for(auto it : name_map)
    {
        // cout << it.first << " " << it.second << "\n";
    }

}