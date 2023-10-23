#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX

#include<bits/stdc++.h>

#define ull unsigned long long

#define pb push_back

using namespace std;

const int MAX_N = 1e3;

vector<vector<int>> graf;

int cellType[MAX_N];

int crystalAmount[MAX_N];

int vis[MAX_N];

int odl[MAX_N][MAX_N];
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int myBase[10], enemyBase[10];


void bfsForShortestPaths(int startPos)
{
    queue<pair<int,int>> kol;
    kol.push({1,startPos});
    odl[startPos][startPos] = 1;
    while(!kol.empty())
    {
        auto top = kol.front();  kol.pop();
        
        for(auto it: graf[top.second])
        {
            if(odl[startPos][it] == 0)
            {
                odl[startPos][it] = top.first;
                kol.push({top.first + 1, it});
            }
        }
    }
}

vector<pair<int,int>> bfs(int startPos, bool Eggs, int turn)
{

    for(int i = 0 ; i < MAX_N; i++) vis[i] = 0;

    vector<pair<int,int>> toChooseEggs;
    vector<pair<int,int>> toChooseCrystal;
    queue<pair<int,int>> kol;
    kol.push({0,startPos});
    vis[startPos] = 1;

    while(!kol.empty())
    {
        auto top = kol.front(); 
        kol.pop();
        if(cellType[top.second] == 2 && crystalAmount[top.second] != 0){
            cerr << "mam to:" << top.second << "\n";
            toChooseCrystal.push_back({top.first,top.second});
        }
        if(cellType[top.second] == 1 && crystalAmount[top.second] != 0 && top.first < 3+turn){
            cerr << "mam to2:" << top.second << "\n";
            toChooseEggs.push_back({top.first,top.second});
        }
        for(auto it: graf[top.second])
        {
            if(vis[it] == 0)
            {
                vis[it] = 1;
                kol.push({top.first + 1, it});
            }
        }
    }

    sort(toChooseEggs.begin(), toChooseEggs.end());

    //while(toChooseEggs.size() > 3) toChooseEggs.pop_back();

    if(Eggs && toChooseEggs.size() != 0) return toChooseEggs;

    sort(toChooseCrystal.begin(), toChooseCrystal.end());

    return toChooseCrystal; 

}

int main()
{
    int crystalOverall = 0;
    int number_of_cells; // amount of hexagonal cells in this map
    cin >> number_of_cells; cin.ignore();
    graf.resize(number_of_cells);
    
    for (int i = 0; i < number_of_cells; i++) {
        int type; // 0 for empty, 1 for eggs, 2 for crystal
        int initial_resources; // the initial amount of eggs/crystals on this cell
        int neigh_0; // the index of the neighbouring cell for each direction
        int neigh_1;
        int neigh_2;
        int neigh_3;
        int neigh_4;
        int neigh_5;
        cin >> type >> initial_resources >> neigh_0 >> neigh_1 >> neigh_2 >> neigh_3 >> neigh_4 >> neigh_5; cin.ignore();

        

        cellType[i] = type;
        crystalAmount[i] = initial_resources;
        if(cellType[i] == 2)
            crystalOverall += initial_resources;


        if(neigh_0 != -1)
        {
            graf[i].pb(neigh_0);
            graf[neigh_0].pb(i);
        }

        if(neigh_1 != -1)
        {
            graf[i].pb(neigh_1);
            graf[neigh_1].pb(i);
        }
        if(neigh_2 != -1)
        {
            graf[i].pb(neigh_2);
            graf[neigh_2].pb(i);
        }

        if(neigh_3 != -1)
        {
            graf[i].pb(neigh_3);
            graf[neigh_3].pb(i);
        }
        if(neigh_4 != -1)
        {
            graf[i].pb(neigh_4);
            graf[neigh_4].pb(i);
        }

        if(neigh_5 != -1)
        {
            graf[i].pb(neigh_5);
            graf[neigh_5].pb(i);
        }

    }

    for(int i = 0; i < number_of_cells; i++)
    {
        bfsForShortestPaths(i);
    }


    int number_of_bases;
    cin >> number_of_bases; cin.ignore();
    for (int i = 0; i < number_of_bases; i++) {
        cin >> myBase[i]; cin.ignore();
    }
    for (int i = 0; i < number_of_bases; i++) {
        cin >> enemyBase[i]; cin.ignore();
    }

    int turn = 0;
    // game loop
    while (1) {

        int ants_cnt = 0;
        for (int i = 0; i < number_of_cells; i++) {
            int resources; // the current amount of eggs/crystals on this cell
            int my_ants; // the amount of your ants on this cell
            int opp_ants; // the amount of opponent ants on this cell
            cin >> resources >> my_ants >> opp_ants; cin.ignore();
            ants_cnt += my_ants;
            crystalAmount[i] = resources;
        }

        int resourcesCount = 0;
        for(int i = 0 ; i < number_of_cells; i++)
        {
            if(cellType[i] == 2)
                resourcesCount += crystalAmount[i];
        }   

        cerr << "Status Zdobycia: " << 100*resourcesCount/crystalOverall << "\n";

        for(int base = 0; base < number_of_bases; base++)
        {
            vector<pair<int,int>> goal =  bfs(myBase[base], (100*resourcesCount/crystalOverall > 80), turn);


            goal.insert(goal.begin(), {0,myBase[base]});

            int goalSize = goal.size();
            int maksWeight = 0;

            for(int i = 0 ; i < goal.size(); i++)
            {
                maksWeight = max(maksWeight, goal[i].first+1);
            }

            cerr << "ile sciezek + mrowki: " << goalSize << " " << ants_cnt << "\n";
            // int weightSum = 0;
            for(int i = 1 ; i < goalSize; i++)
            {
                cerr << "DEBUG: " << i << " " << goal[i].first << " " << goal[i].second << "\n";

                int minn = 1e9, minID = 0, weightFrom = 0;
                for(int pos = 0; pos < i; pos++)
                {
                    int from = goal[i].second;
                    int to = goal[pos].second;
                    if(from != to && odl[from][to] < minn)
                    {
                        minn = odl[from][to];
                        minID = to;
                    }
                }
                cout << "LINE " << goal[i].second << " " << minID << " " << 1 << ";";




        }

    

            //cout << "LINE " << goal[i].second << " " << myBase << " " << 100*goalSize/ants_cnt << ";";

            // if(i == 0)
            //     cout << "LINE " << myBase << " " << goal[i].second << " " << maksWeight - goal[i].first << ";";
            // else
            //     cout << "LINE " << myBase << " " << goal[i].second << " " << maksWeight - goal[i].first << ";";
        }
        cout << endl;
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // WAIT | LINE <sourceIdx> <targetIdx> <strength> | BEACON <cellIdx> <strength> | MESSAGE <text>
        //cout << "WAIT" << endl;
        turn ++;
    }
}