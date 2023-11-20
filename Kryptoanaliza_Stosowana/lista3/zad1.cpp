#include<bits/stdc++.h>

using namespace std;

const int STR_LEN = 6;

map<string,bool> name_map;




void gen_names(int pos, string &curr_name)
{   


    if (pos == STR_LEN)
    {
        if (!name_map[curr_name])
        {
            cout << curr_name << "\n";
            name_map[curr_name] = true;
        }
        return;
    }
        
    

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



}