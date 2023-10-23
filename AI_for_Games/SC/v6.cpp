#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX

#include<bits/stdc++.h>

#define ull unsigned long long
#define pb push_back
#define u64 uint64_t
#define INLINE inline

using namespace std;

const int MAX_N = 200;

const int MAX_EDGE = 100000;

int maxPathNodes = 7;

auto start_time = chrono::high_resolution_clock::now();

int timeForTurn = 800;


enum resource : int8_t {
    kEmpty = 0,
    kCrystal = 1,
    kEggs = 2
};


string translate(resource resource_)
{
    switch(resource_)
    {
        case kEmpty :
            return "Empty";
        case kCrystal :
            return "Crystal";
        case kEggs :
            return "Eggs";
    };
    return "NONE";
}

double rand_DoubleRange(double a, double b)
{
    return ((b-a) * ((double)rand() / RAND_MAX)) + a;
}


template <typename T, u64 MaxSize>
class Buffer {
 public:
  Buffer() : size_(0) {}

  INLINE void Reset() { size_ = 0; }

  INLINE void Add(const T& val) { buffer_[size_++] = val; }

  INLINE const T& operator[](u64 idx) const { return buffer_[idx]; }

  INLINE void PopBack() { --size_; }

  INLINE T& operator[](u64 idx) { return buffer_[idx]; }

  INLINE u64 Size() const { return size_; }

  INLINE void SetSize(u64 size) { size_ = size; }

  auto begin() const { return buffer_; }

  auto end() const { return buffer_ + size_; }

  auto begin() { return buffer_; }

  auto end() { return buffer_ + size_; }

 private:
  T buffer_[MaxSize];
  u64 size_;
};

struct beaconCell   // all info about beacon in cell
{
    void placeBeacon(int beaconVal_)
    {
        beacon = beaconVal_;
        wiggleRoom = 0;
    }

    int beacon = 0;
    int wiggleRoom = 0;

};

struct antCell  // all info about ants and chains
{   
    int enemyAnts = 0;
    int myAnts = 0;
    resource resourceType = kEmpty;
    int resourceCount = 0;
    int chainEnemy = 0;
    int chainMe = 0;

    void Start(resource resourceType_, int resourceCount_)
    {
        resourceType = resourceType_;
        resourceCount = resourceCount_;
    }

    void Update(int newResourceCount_, int myAnts_, int enemyAnts_)
    {
        resourceCount = newResourceCount_;
        enemyAnts = enemyAnts_;
        myAnts = myAnts_;
        chainEnemy = 0;
        chainMe = 0;
    }

    void resetChains()
    {
        chainEnemy = 0;
        chainMe = 0;
    }
};

Buffer<int,6> neighbours[MAX_N]; // neigbours of cell 'i'


Buffer<beaconCell,MAX_N> beaconCells;
Buffer<antCell,MAX_N> antCells;
Buffer<antCell,MAX_N> antCellsCopy;

Buffer<beaconCell,MAX_N> beaconCellsForAllocations;
Buffer<antCell,MAX_N> antCellsForAllocations;


Buffer<int,2> myBase, enemyBase;

Buffer<double,MAX_EDGE> edgeWeights;



Buffer<int, MAX_EDGE> beaconCellsIDX;
Buffer<int, MAX_EDGE> antCellsIDX;

Buffer<pair<int,pair<int,int>>, MAX_EDGE> allPairs;

Buffer<pair<int,int>, MAX_EDGE> sortedPairs[MAX_N];

Buffer<pair<int,pair<int,int>>, MAX_EDGE> allocations;


Buffer<pair<int,pair<int,int>>, MAX_EDGE> sortNeigbours;

Buffer<pair<int,pair<int,int>>, MAX_EDGE> moves;


Buffer<pair<int,int>,MAX_EDGE> currentlyChoosedEdges;
Buffer<pair<int,int>,MAX_EDGE> edgesToPick; 


Buffer<pair<int,int>,MAX_EDGE> bestEdges; 


Buffer<int,MAX_N> queueForDial[2000];


int shortestDist[MAX_N][MAX_N]; // From A to B

bool alreadyInPath[MAX_N];

int numberOfCells; 

int allMyAnts = 0;
int allCrystal = 0;

bool earlyGame = 0;

int simulations = 0;


queue<int> kol;

void generateShortestPaths()
{
    for (int cell = 0; cell < numberOfCells; cell++)
    {
        kol.push(cell);
        shortestDist[cell][cell] = 1;

        while(!kol.empty())
        {
            auto top = kol.front(); kol.pop();
            
            for (auto neigbour: neighbours[top])
            {
                if(shortestDist[cell][neigbour] == 0)
                {
                    shortestDist[cell][neigbour] = shortestDist[cell][top] + 1;
                    kol.push(neigbour);
                }
            }
        }
    }
}

double generateWeight(int from, int to)
{
    assert(shortestDist[from][to] != 0);
    return (double)(0.5 / (double) (shortestDist[from][to]));
}

pair<int,int> getScore()
{
    pair<int,int> capturedResources; // first Crystal, second Eggs

    for(int i = 0; i < numberOfCells; i++)
        antCells[i].resetChains();

    
    //priority_queue<pair<int,int>> kol; // TODO DIAL ALGORITHM

    
    int idx = 0;

    for(auto base : enemyBase)
    {
        //cerr << antCells[base].enemyAnts << "\n";
        queueForDial[antCells[base].enemyAnts].Add(base);
        //kol.push({antCells[base].enemyAnts,base});
        antCells[base].chainEnemy = antCells[base].enemyAnts;
        idx = max(idx,antCells[base].enemyAnts);
    }

    for(int i = 0; i < idx; i++)
        queueForDial[i].Reset();

    assert(idx < 2000);


    while(idx != 0)
    {
        //cerr << idx << " " << queueForDial[idx].Size() << "\n";
        while(queueForDial[idx].Size() == 0 && idx != 0) idx --;

        if(idx == 0 || queueForDial[idx].Size() == 0) break;

        auto top = queueForDial[idx][queueForDial[idx].Size() - 1]; queueForDial[idx].PopBack();

        for (auto neigbour: neighbours[top])
        {
            int newValue = min(antCells[top].chainEnemy, antCells[neigbour].enemyAnts);
            
            if(newValue != 0 && antCells[neigbour].chainEnemy < newValue)
            {
                antCells[neigbour].chainEnemy = newValue;
                queueForDial[antCells[neigbour].chainEnemy].Add(neigbour);
            }
        }
    }

    // for(int i = 0; i < 2000; i++)
    //     queueForDial[i].Reset();

    for(auto base : myBase)
    {
        //cerr << antCells[base].myAnts << "\n";
        queueForDial[antCells[base].myAnts].Add(base);
        // kol.push({antCells[base].myAnts,base});
        antCells[base].chainMe = antCells[base].myAnts;
        idx = max(idx,antCells[base].myAnts);
    }

    for(int i = 0; i < idx; i++)
        queueForDial[i].Reset();

    assert(idx < 2000);

    while(idx != 0)
    {
        while(queueForDial[idx].Size() == 0 && idx != 0) idx --;

        if(idx == 0 || queueForDial[idx].Size() == 0) break;

        auto top = queueForDial[idx][queueForDial[idx].Size() - 1]; queueForDial[idx].PopBack();

        if(antCells[top].resourceType != kEmpty)
        {
            if(antCells[top].resourceType == kCrystal)
            {
                double weightCrytal = 1;
                if(shortestDist[0][top] == 1) weightCrytal = 4;
                if(shortestDist[0][top] == 2) weightCrytal = 3;
                if(shortestDist[0][top] == 3) weightCrytal = 2;
                //if(shortestDist[0][top.second] == 4) weightCrytal = 1.5;

                int extracted = min(antCells[top].chainMe, antCells[top].resourceCount) * weightCrytal;

                capturedResources.first += extracted;
                antCells[top].resourceCount -= min(antCells[top].chainMe, antCells[top].resourceCount);
            } else
            {
                capturedResources.second += min(antCells[top].chainMe, antCells[top].resourceCount);
                antCells[top].resourceCount -= min(antCells[top].chainMe, antCells[top].resourceCount);
            }
        }

        for (auto neigbour: neighbours[top])
        {

            int newValue = min(antCells[top].chainMe, antCells[neigbour].myAnts);
            
            if(newValue != 0 && antCells[neigbour].chainMe < newValue && newValue >= antCells[neigbour].chainEnemy)
            {
                antCells[neigbour].chainMe = newValue;
                queueForDial[antCells[neigbour].chainMe].Add(neigbour);
                //kol.push({antCells[neigbour].chainMe,neigbour});
            }
        }
    }
    return capturedResources;
}

void placeBeacons()
{
    for(int i = 0 ; i < numberOfCells; i++)
        beaconCells[i].placeBeacon(0);
    
    for(auto edge : currentlyChoosedEdges)
    {  
        int startPoint = edge.first, endPoint = edge.second;

        if(startPoint != endPoint) beaconCells[startPoint].placeBeacon(1);
        while(endPoint != startPoint)
        {
            beaconCells[endPoint].placeBeacon(1);
            for(auto neigbour : neighbours[endPoint])
            {
                if(shortestDist[startPoint][endPoint] == shortestDist[startPoint][neigbour] + 1)
                {
                    endPoint = neigbour;
                    break;
                }
            }
        }
    }
}


int evalBoard(int turns)
{   
    pair<int,int> collectedResources;


    for(int i = 0; i < numberOfCells; i++)
    {
        antCellsCopy[i] = antCells[i];
    }



    placeBeacons();
    

    for(int turn = 0 ; turn < turns; turn++)
    {
        antCellsIDX.Reset();
        beaconCellsIDX.Reset();

        antCellsForAllocations.Reset();
        beaconCellsForAllocations.Reset();


        int antSum = 0;
        int beaconSum = 0;

        for(int i = 0; i < numberOfCells; i++)
        {
            antCellsForAllocations.Add(antCells[i]);
            beaconCellsForAllocations.Add(beaconCells[i]);

            if(beaconCells[i].beacon > 0)
            {
                beaconCellsIDX.Add(i);
                beaconSum += beaconCells[i].beacon;
            } 

            if(antCells[i].myAnts > 0)
            {
                antCellsIDX.Add(i);
                antSum += antCells[i].myAnts;
            } 
        }

        double scalingFactor = (double) antSum / beaconSum;

        for(auto beaconCell : beaconCellsIDX)
        {
            int highBeaconValue = (int) ceil(beaconCellsForAllocations[beaconCell].beacon * scalingFactor);
            int lowBeaconValue = (int) (beaconCellsForAllocations[beaconCell].beacon * scalingFactor);
            beaconCellsForAllocations[beaconCell].beacon = max(1,lowBeaconValue);
            beaconCellsForAllocations[beaconCell].wiggleRoom = highBeaconValue - lowBeaconValue;
        }


        allPairs.Reset();
        for(auto antCell : antCellsIDX)
        {
            for(auto beaconCell : beaconCellsIDX)
            {
                allPairs.Add({shortestDist[antCell][beaconCell],{antCell, beaconCell}});
            }
        }

        for(int i = 0 ; i < numberOfCells; i++)
            sortedPairs[i].Reset();
        
        for(auto pair : allPairs)
            sortedPairs[pair.first].Add({pair.second.first, pair.second.second});
        
        allPairs.Reset();
        for(int i = 0 ; i < numberOfCells; i++)
            for(auto pair : sortedPairs[i])
                allPairs.Add({i,pair});
            


        allocations.Reset();
        
        bool stragglers = 0;
        while(allPairs.Size() != 0)
        {
            for(auto pair : allPairs)
            {
                int antCell = pair.second.first;
                int beaconCell = pair.second.second;

                int antCount = antCellsForAllocations[antCell].myAnts;

                int beaconCount = beaconCellsForAllocations[beaconCell].beacon;
                int wiggleRoom = beaconCellsForAllocations[beaconCell].wiggleRoom;

                int maxAlloc = (int) (stragglers ? min(antCount, beaconCount + wiggleRoom) : min(antCount, beaconCount));
                if(maxAlloc > 0)
                {
                    allocations.Add({maxAlloc, {antCell, beaconCell}});
                    antCellsForAllocations[antCell].myAnts -= maxAlloc;

                    if(!stragglers)
                    {
                        beaconCellsForAllocations[beaconCell].beacon -= maxAlloc;
                    } else 
                    {
                        beaconCellsForAllocations[beaconCell].beacon -= (maxAlloc - wiggleRoom);
                        beaconCellsForAllocations[beaconCell].wiggleRoom = 0;
                    }
                }
            }
            int n = allPairs.Size();

            for(int i = n-1; i >= 0; i--)
            {
                if(allPairs.Size() == 0) break;
                if(antCellsForAllocations[allPairs[i].second.first].myAnts <= 0)
                {
                    swap(allPairs[i], allPairs[allPairs.Size()-1]);
                    allPairs.PopBack();
                }
            }
            stragglers = true;
        }

        moves.Reset();
        for(auto alloc : allocations)
        {   
            
            int from = alloc.second.first, to = alloc.second.second, amount = alloc.first;

            if(from == to) continue;

            
            sortNeigbours.Reset();


            for(auto neigbour: neighbours[from])
            {
                if(shortestDist[to][from] == shortestDist[to][neigbour] + 1)
                {
                    sortNeigbours.Add({-antCells[neigbour].myAnts,{-beaconCells[neigbour].beacon, neigbour}});
                }
            }
            sort(sortNeigbours.begin(), sortNeigbours.end());

            int bestNeigbour = sortNeigbours[0].second.second;

            moves.Add({amount,{from, bestNeigbour}});
        }

        for(auto move : moves)
        {
            int from = move.second.first;
            int to = move.second.second;
            int amount = move.first;

            antCells[from].myAnts -= amount;
            antCells[to].myAnts += amount;
        }

        pair<int,int> afterTurnScore = getScore();
        collectedResources.first += afterTurnScore.first;
        collectedResources.second += afterTurnScore.second;
    }

    for(int i = 0; i < numberOfCells; i++)
    {
        antCells[i] = antCellsCopy[i];
    }

    int weight = 2;
    if(earlyGame) weight = 5;

    int collectedEval = collectedResources.first + collectedResources.second * weight;
    return collectedEval;
}

void generatePath()
{
    start_time = chrono::high_resolution_clock::now();
    int bestEval = -1;
    simulations = 0;
    allMyAnts = 0;

    for(int i = 0 ; i < numberOfCells; i++)     // counting all my Ants
        allMyAnts += antCells[i].myAnts;


    while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() <= timeForTurn)
    {

        simulations ++;
        currentlyChoosedEdges.Reset(); // reseting to state with no nodes
        

        for(int cell = 0; cell < numberOfCells; cell++) // reset of bool table
            alreadyInPath[cell] = 0;

        for (auto base : myBase) 
        {
            currentlyChoosedEdges.Add({base,base});   // adding bases
            alreadyInPath[base] = 1;
        }

        edgesToPick.Reset();
        for(auto node : currentlyChoosedEdges)
        {
            for(int cell = 0 ; cell < numberOfCells; cell++)
            {
                if(antCells[cell].resourceType != kEmpty && antCells[cell].resourceCount != 0 && alreadyInPath[cell] == 0) edgesToPick.Add({node.first, cell});
            }
        }
        
        int pathCost = 0; // current path cost 
        while(currentlyChoosedEdges.Size() < maxPathNodes)
        {

            if(edgesToPick.Size() == 0) break; // no edges left

            edgeWeights.Reset();
            for(auto candidate : edgesToPick)
            {
                edgeWeights.Add(generateWeight(candidate.first,candidate.second));  // calculating ppb of all edge
            }

            double sum = 0;
            for(auto weight : edgeWeights) // sum of ppb
                sum += weight;  

            double rand = rand_DoubleRange(0.0,sum);

            int nodeAdded = -1;
            for(int i = 0; i < edgeWeights.Size(); i++)
            {
                rand -= edgeWeights[i];
                if(rand <= 0) 
                {
                    pathCost += shortestDist[edgesToPick[i].first][edgesToPick[i].second];
                    if(pathCost <= allMyAnts)   // if I can afford adding new edge, add it!
                    {
                        nodeAdded = edgesToPick[i].second;
                        alreadyInPath[nodeAdded] = 1;
                        currentlyChoosedEdges.Add({edgesToPick[i].first,edgesToPick[i].second});
                    }
                        
                    break;
                }
            }

            if(pathCost > allMyAnts) break; // not enough ants to cover path

            for(int cell = 0 ; cell < numberOfCells; cell++)
            {
                if(antCells[cell].resourceType != kEmpty && antCells[cell].resourceCount != 0 && alreadyInPath[cell] == 0) edgesToPick.Add({nodeAdded, cell}); // adding new edges
            }

            int n = edgesToPick.Size();
            for(int i = n - 1; i >= 0; i--)
            {   
                if(edgesToPick.Size() == 0) break; // if no edges leave
                if(edgesToPick[i].second == nodeAdded) // found edge to new created node
                {
                    swap(edgesToPick[i], edgesToPick[edgesToPick.Size() - 1]);
                    edgesToPick.PopBack();
                }
            }


            int currEval = evalBoard(4); // evaluating
            if(bestEval < currEval)
            {
                bestEval = currEval;
                bestEdges.Reset();
                for(auto edge : currentlyChoosedEdges)
                    bestEdges.Add(edge);
            }    
        }
    }

    currentlyChoosedEdges.Reset();
    for(auto edge : bestEdges)
    {
        currentlyChoosedEdges.Add(edge);
    }
    
}

void prepare()
{
    cin >> numberOfCells;

    for(int i = 0 ; i < numberOfCells; i++)
    {
        int type, resourceCount;  cin >> type >> resourceCount; 

        resource resourceType = type == 0 ? kEmpty : (type == 1 ? kEggs : kCrystal);
        antCells[i].Start(resourceType, resourceCount);

        for(int j = 0; j < 6; j++)
        {
            int neighbour; cin >> neighbour;
            if(neighbour != -1) neighbours[i].Add(neighbour);
        }

        if(antCells[i].resourceType == kCrystal) allCrystal += resourceCount;
    }

    int numberOfBases; cin >> numberOfBases;
    for(int i = 0 ; i < numberOfBases; i++)
    {
        int baseID; cin >> baseID;
        myBase.Add(baseID);
    }
    for(int i = 0 ; i < numberOfBases; i++)
    {
        int baseID; cin >> baseID;
        enemyBase.Add(baseID);
    }

    generateShortestPaths();

}

int main()
{
    srand(time(NULL));

    prepare();

    while(true)
    {

        int currentCrystalCount = 0;

        int myScore, enemyScore; cin >> myScore >> enemyScore;


        for(int i = 0; i < numberOfCells; i++)
        {
            int resourcesCount, myAnts, enemyAnts; cin >> resourcesCount >> myAnts >> enemyAnts;
            antCells[i].Update(resourcesCount, myAnts, enemyAnts);
            if (antCells[i].resourceType == kCrystal) currentCrystalCount += resourcesCount;
        }
        earlyGame = (100.0 * currentCrystalCount / allCrystal) > 85;

        generatePath();

        assert(currentlyChoosedEdges.Size() != 0);


        placeBeacons();

        string myMove = "";

        for(int i = 0 ; i < numberOfCells; i++)
        {
            if(beaconCells[i].beacon > 0)
            {
                myMove += "BEACON " + to_string(i) + " " + to_string(beaconCells[i].beacon) + ";";
            }
        }


        cout << myMove << "MESSAGE T: " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() << " S: " << simulations;
        cout << endl;

        timeForTurn = 97;
    }


}