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

int KH = 0;

int MAX_DEPTH;

int DEPTH_TO_START = 1;

int END_COST = 1000000;

int TIME_FOR_TURN = 145;

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

    int numberMoves = 0;

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

    void reset()
    {
        enemySpace = 0xAA55AA55AA55AA55;
        mySpace =    0x55AA55AA55AA55AA;
    }

    ull Shift(ull toShift, int dir)
    {
        return ((toShift >> shiftValR[dir]) << shiftValL[dir]) & masks[dir];
    }

    // bool goodMove(pair<int,int> move)
    // {
    //     if(Get(move.first, mySpace) && Get(move.second,enemySpace)) return true;
    //     return false;
    // }

    int genPossibleMoves(int depth)
    {
        ull freeSpace = ~(enemySpace | mySpace);

        ull possibleSpace = 0;

        possibleSpace |= Shift(mySpace,0) & enemySpace;
        possibleSpace |= Shift(mySpace,2) & enemySpace;
        possibleSpace |= Shift(mySpace,4) & enemySpace;
        possibleSpace |= Shift(mySpace,6) & enemySpace;


        int id = 0;

        // if(KH)
        // {

        //     possibleMoves[depth][id++] = killers[0];
        //     possibleMoves[depth][id++] = killers[1];
        
        // }

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
        ull myMobile = 0;
        ull enemyMobile = 0;

        myMobile |= Shift(enemySpace,0) & mySpace;
        myMobile |= Shift(enemySpace,2) & mySpace;
        myMobile |= Shift(enemySpace,4) & mySpace;
        myMobile |= Shift(enemySpace,6) & mySpace;



        enemyMobile |= Shift(mySpace,0) & enemySpace;
        enemyMobile |= Shift(mySpace,2) & enemySpace;
        enemyMobile |= Shift(mySpace,4) & enemySpace;
        enemyMobile |= Shift(mySpace,6) & enemySpace;


        // cerr << myMobile <<  " " << enemyMobile << "\n" << mySpace << " " << enemySpace << "\n";

        int ans = __builtin_popcountll(myMobile) - __builtin_popcountll(enemyMobile);
        if (player_color != color) ans *= -1;
        return ans;

    }   
    gameState doMove(pair<int,int> move)
    {

        int prev = move.first;
        int next = move.second;


        gameState newState(*this);

        newState.color = 1-color;

        if (prev != -1)
        {
            newState.mySpace &= ~(1ull << (prev));
            newState.Set(next, &(newState.mySpace));
            newState.enemySpace &= ~(1ull << (next));    
        } else 
        {
            newState.gameEnded = 1;
        }

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
        
        if (player_color != color) return 1;
        return -1;

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

    if(n == 0) return state.calculateResult() * END_COST;
    
    int evalForThis = (depth%2 == 0) ? INT_MIN : INT_MAX;


    for(int i = 0; i < n; i++)
    {
    
        gameState newState = state.doMove(possibleMoves[depth][i]);

        int eval = alfa_beta(newState, depth+1, alfa, beta);

        if (eval == INT_MIN) return INT_MIN;

        if(depth % 2 == 0)
        {
            evalForThis = max(evalForThis, eval);
            if(evalForThis >= beta)
            {
                return evalForThis;
            }
            alfa = max(alfa,evalForThis);
        } else 
        {
            evalForThis = min(evalForThis, eval);
            if(evalForThis <= alfa)
            {
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
        cerr << "wow: " << n << "\n";
        int maks = INT_MIN;
        pair<int,int> bestMove = {-1,-1};
        for(int i = 0; i < n; i++)
        {
            gameState newState = toEval.doMove(possibleMoves[0][i]);

            pair<int,int> move = possibleMoves[0][i];

            int minmaxEval = alfa_beta(newState, 1, INT_MIN, INT_MAX);

            cerr << "Move: " << i << " " << possibleMoves[0][i].first << " " << possibleMoves[0][i].second << " " << minmaxEval << "\n";

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



    gameState game; 
    game.reset();
    game.color = 0;

    // game loop
    while (1) {
        
        // ignore
        for (int i = 7; i >= 0; i--) {
            string line; // horizontal row      
            cin >> line; cin.ignore(); 
        }

        string last_action; // last action made by the opponent ("null" if it's the first turn)
        cin >> last_action; cin.ignore();

        if (last_action != "null")
        {
            int prev = (last_action[0] - 'a') + (last_action[1] - '0' - 1) * 8;
            int next = (last_action[2] - 'a') + (last_action[3] - '0' - 1) * 8;

            cerr << "Enemy: " << prev << " " <<  next << "\n";
            game = game.doMove({prev, next});

        }

        game.print();

        int actions_count; // number of legal actions
        cin >> actions_count; cin.ignore();

        int n = game.genPossibleMoves(0);


        cerr << n << " " << actions_count << "\n";
        if (n != actions_count)
        {
            cerr <<"DEBIL 2";
            return 0;
        }

        
        for (int i = 0 ; i < n; i++)
        {
            int prev = possibleMoves[0][i].first;
            int next = possibleMoves[0][i].second;
            string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
            cerr << i << " " << move << "\n";

        }

        start_time = chrono::high_resolution_clock::now();

        pair<int,int> decision = minmaxDecision(game);

        if(decision == make_pair(-1,-1))
        {
            int n = game.genPossibleMoves(0);

            int moveID = rand() % n;

            int prev = possibleMoves[0][moveID].first;
            int next = possibleMoves[0][moveID].second;

            string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);

            cerr << moveID << " " << move << " " << possibleMoves[0][moveID].first << "\n";
            cout << move << "\n";

            game = game.doMove({prev,next});

            // cout << "random" << endl;
            cerr << "Debil\n";
            return 0;
            continue;
        }

        int prev = decision.first;
        int next = decision.second;
        string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
        
        cerr << prev << " " << next << "\n";
        game = game.doMove({prev,next});

        game.print();

        cout << move << " MSG ";
        cout << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() << " " << MAX_DEPTH-2 << " " << bestWAGA << endl;


        TIME_FOR_TURN = 145;
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // cout << "random" << endl; // e.g. e2e3 (move piece at e2 to e3)
    }
}