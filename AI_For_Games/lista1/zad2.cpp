// Assignment [2] [Dawid] [Skowronek] [330235] [Sko0owi]

#pragma GCC optimize("O3","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX

#include<bits/stdc++.h>

#define ull unsigned long long

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

pair<int,int> possibleMoves[500][500];

int CG = 1;

int KH = 1;

int MAX_DEPTH;

int DEPTH_TO_START = 3;

int END_COST = 1000000;

int TIME_FOR_TURN = 95;

int player_color;

pair<int,int> killers[2];

auto start_time = chrono::high_resolution_clock::now();

ull masks[8] = {
    0x7F7F7F7F7F7F7F7FULL, // right (>> 1)
    0x007F7F7F7F7F7F7FULL, // down-right (>> 9)
    0xFFFFFFFFFFFFFFFFULL, // down (>> 8)
    0x00FEFEFEFEFEFEFEULL, // down-left (>> 7)
    0xFEFEFEFEFEFEFEFEULL, // left (<< 1)
    0xFEFEFEFEFEFEFE00ULL, // up-left (<< 9)
    0xFFFFFFFFFFFFFFFFULL, // up (<< 8)
    0x7F7F7F7F7F7F7F00ULL, // up-right (<< 7)
};
static short shiftValR[8] = {
    1,9,8,7,0,0,0,0
};
static short shiftValL[8] = {
    0,0,0,0,1,9,8,7
};



struct gameState
{
    ull enemySpace = 0;
    ull mySpace = 0;

    bool color = 0; // 0 - white, 1 - black

    bool gameEnded = 0;

    gameState()
    {
        enemySpace = 0;
        mySpace = 0;
    }


    inline void Set(int pos, ull *toSet)
    {
        *toSet |= ((ull)1 << pos); 
    }
    inline int Get(int pos, ull toGet)
    {
        return (toGet & ((ull)1 << pos)) >> pos;
    }
        

    void print()
    {
        ull free_space = ~(enemySpace | mySpace);

        for (int i = 7; i>= 0; i--)
        {
            for(int j = 0; j < 8; j++)
            {
                int ind = i * 8 + j;
                if (Get(ind, free_space))
                    cerr << ".";
                if (Get(ind, enemySpace))
                    cerr << '0';
                if (Get(ind, mySpace))
                    cerr << '1';
            } 
            cerr << "\n";
        }
        cerr << "\n";
    }

    ull Shift(ull toShift, int dir)
    {
        return ((toShift >> shiftValR[dir]) << shiftValL[dir]) & masks[dir];
    }

    bool goodMove(pair<int,int> move)
    {
        if(Get(move.first, mySpace) && Get(move.second,enemySpace)) return true;
        return false;
    }

    int genPossibleMoves(int depth)
    {
        ull freeSpace = ~(enemySpace | mySpace);

        ull possibleSpace = 0;

        possibleSpace |= Shift(mySpace,0) & enemySpace;
        possibleSpace |= Shift(mySpace,2) & enemySpace;
        possibleSpace |= Shift(mySpace,4) & enemySpace;
        possibleSpace |= Shift(mySpace,6) & enemySpace;


        int id = 9;

        if(KH)
        {

            possibleMoves[depth][id++] = killers[0];
            possibleMoves[depth][id++] = killers[1];
        
        }

        for(int i = 0; i < 64; i++)
        {
            if(Get(i,possibleSpace))
            {
                
                if(Shift((1ull<<i),0) & mySpace)
                    possibleMoves[depth][id++] = {i-1,i};
                if(Shift((1ull<<i),2) & mySpace)
                    possibleMoves[depth][id++] = {i-8,i};
                if(Shift((1ull<<i),4) & mySpace)
                    possibleMoves[depth][id++] = {i+1,i};
                if(Shift((1ull<<i),6) & mySpace)
                    possibleMoves[depth][id++] = {i+8,i};
            }
        }

        return id;
    }

    int evalState()
    {
        int n = genPossibleMoves(100);
        return n;
        
    }   
    gameState doMove(pair<int,int> move)
    {

        int prev = move.first;
        int next = move.second;

        gameState newState(*this);

        newState.color = 1-color;

        newState.mySpace &= ~(1ull << (prev));
        newState.Set(next, &(newState.mySpace));
        newState.enemySpace &= ~(1ull << (next));    
        

        swap(newState.mySpace, newState.enemySpace);
        return newState;
    }
    pair<int,int> randomMove()
    {
        int n = genPossibleMoves(0);
        return possibleMoves[0][rand() % n + 2];
    }
    int calculateResult()
    {
        int n = genPossibleMoves(100);
        int ans = (n > 0) ? 1 : -1;
        if (player_color != color) ans*=-1;
        return ans;

    }
};



int alfa_beta(gameState state, int depth, int alfa, int beta)
{
    if(CG) if(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() >= TIME_FOR_TURN) return INT_MIN;

    if(state.gameEnded)
    {
        //cerr << state.calculateResult() * END_COST << "\n";
        return state.calculateResult() * END_COST;
    } 
    if(depth >= MAX_DEPTH) return state.evalState();

    
    int n = state.genPossibleMoves(depth);

    int evalForThis = (depth%2 == 0) ? INT_MIN : INT_MAX;


    for(int i = 0; i < n; i++)
    {
        if(state.goodMove(possibleMoves[depth][i]) == 0) continue;
    
        gameState newState = state.doMove(possibleMoves[depth][i]);

        int eval = alfa_beta(newState, depth+1, alfa, beta);

        if (eval == INT_MIN) return INT_MIN;

        if(depth % 2 == 0)
        {
            evalForThis = max(evalForThis, eval);
            if(evalForThis >= beta)
            {
                if(KH)
                {
                    if (i == 1)
                    {
                        swap(killers[0], killers[1]);
                    }

                    if (i >= 2)
                    {
                        killers[1] = possibleMoves[depth][i];
                    } 
                }
                return evalForThis;
            }
            alfa = max(alfa,evalForThis);
        } else 
        {
            evalForThis = min(evalForThis, eval);
            if(evalForThis <= alfa)
            {
                if(KH)
                {
                    if (i == 1)
                    {
                        swap(killers[0], killers[1]);
                    }

                    if (i >= 2)
                    {
                        killers[1] = possibleMoves[depth][i];
                    } 
                }
                return evalForThis;
            }
            beta = min(beta,evalForThis);
        }
    }

    return evalForThis;
}

int bestWAGA;

pair<int,int> minmaxDecision(gameState toEval)
{
    pair<int,int> bestMoveANS = {-1,-1};

    MAX_DEPTH = DEPTH_TO_START;
    bool stillTime = 1;

    while(stillTime && MAX_DEPTH <= 64)
    {
        int n = toEval.genPossibleMoves(0);
        int maks = INT_MIN;
        pair<int,int> bestMove = {-1,-1};
        for(int i = 0; i < n; i++)
        {
            if(toEval.goodMove(possibleMoves[0][i]) == 0) continue;
            gameState newState = toEval.doMove(possibleMoves[0][i]);

            pair<int,int> move = possibleMoves[0][i];

            int minmaxEval = alfa_beta(newState, 1, INT_MIN, INT_MAX);

            if(minmaxEval == INT_MIN) return bestMoveANS;


            if(minmaxEval >= maks)
            {
                maks = minmaxEval;
                bestWAGA = minmaxEval;
                bestMove = possibleMoves[0][i];
            }
        }
        bestMoveANS = bestMove;
        MAX_DEPTH += 2;

        if(CG == 0) stillTime = 0;
    }
    return bestMoveANS;
}



int main()
{
    int board_size; // height and width of the board
    cin >> board_size; cin.ignore();
    string color; // current color of your pieces ("w" or "b")
    cin >> color; cin.ignore();

    if (color == "w") player_color = 0;
    else player_color = 1;






    // game loop
    while (1) {

        gameState game;
        for (int i = 7; i >= 0; i--) {
            string line; // horizontal row
            cin >> line; cin.ignore();

            for(int j = 0; j < 8; j++)
            {
                if(line[j] == 'w')
                    game.Set(i*8 + j, &game.mySpace);
                else if(line[j] == 'b')
                    game.Set(i*8 + j, &game.enemySpace);
            }
        }

        game.color = player_color;

        if(player_color == 1)
            swap(game.mySpace, game.enemySpace);



        string last_action; // last action made by the opponent ("null" if it's the first turn)
        cin >> last_action; cin.ignore();
        int actions_count; // number of legal actions
        cin >> actions_count; cin.ignore();

        int n = game.genPossibleMoves(0);

        
        for (int i = 0 ; i < n; i++)
        {
            int prev = possibleMoves[0][i].first;
            int next = possibleMoves[0][i].second;
            string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
            cerr << move << "\n";

        }

        start_time = chrono::high_resolution_clock::now();

        pair<int,int> decision = minmaxDecision(game);

        if(decision == make_pair(-1,-1))
        {
            cout << "random" << endl;
            continue;
        }

        int prev = decision.first;
        int next = decision.second;
        string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
        
        cout << move << " MSG ";
        cout << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() << " " << MAX_DEPTH-2 << " " << bestWAGA << endl;


        TIME_FOR_TURN = 95;
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // cout << "random" << endl; // e.g. e2e3 (move piece at e2 to e3)
    }
}