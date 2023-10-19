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

int possibleMoves[100][64];

int CG = 1;

int MAX_DEPTH;

int DEPTH_TO_START = 3;

int END_COST = 1000000;

int TIME_FOR_TURN = 1995;

int PLAYER;

auto start_time = chrono::high_resolution_clock::now();

int V[8][8] = {
            {20, -3, 11, 8, 8, 11, -3, 20},
            {-3, -7, -4, -3, -3, -4, -7, -3},
            {11, -4, 2, 2, 2, 2, -4, 11},
            {8, -3, 2, -3, -3, 2, -3, 8},
            {8, -3, 2, -3, -3, 2, -3, 8},
            {11, -4, 2, 2, 2, 2, -4, 11},
            {-3, -7, -4, -3, -3, -4, -7, -3},
            {20, -3, 11, 8, 8, 11, -3, 20}};

ull masks[8] = {
    0xFEFEFEFEFEFEFEFEULL, // right (>> 1)
    0x7F7F7F7F7F7F7F00ULL, // down-right (>> 9)
    0xFFFFFFFFFFFFFFFFULL, // down (>> 8)
    0xFEFEFEFEFEFEFE00ULL, // down-left (>> 7)
    0xFEFEFEFEFEFEFEFEULL, // left (<< 1)
    0x00FEFEFEFEFEFEFEULL, // up-left (<< 9)
    0xFFFFFFFFFFFFFFFFULL, // up (<< 8)
    0x007F7F7F7F7F7F7FULL, // up-right (<< 7)
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

    bool currPlayer = 0;

    bool prevPass = 0;
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
        

    void reset()
    {
        Set(28,&mySpace);  // czarne zaczynaja
        Set(35,&mySpace); 
        Set(27,&enemySpace); 
        Set(36,&enemySpace);
    }

    void print()
    {
        ull free_space = ~(enemySpace | mySpace);


        char enemy = (PLAYER == currPlayer) ? 'o' : 'x';
        char player = (PLAYER == currPlayer) ? 'x' : 'o';
        for (int i = 0; i < 64; i++)
        {
            if (i % 8 == 0)
                cerr << "\n";
            if (Get(i, free_space))
                cerr << ".";
            if (Get(i, enemySpace))
                cerr << 'o';
            if (Get(i, mySpace))
                cerr << 'x';
        }
        cerr << "\n";
    }

    ull Shift(ull toShift, int dir)
    {
        return ((toShift >> shiftValR[dir]) << shiftValL[dir]) & masks[dir];
    }

    int genPossibleMoves(int depth)
    {
        ull freeSpace = ~(enemySpace | mySpace);

        ull possibleSpace = 0;
        for(int dir = 0 ; dir < 8; dir++)
        {
            ull x = Shift(mySpace,dir) & enemySpace;

            x |= Shift(x,dir) & enemySpace;
            x |= Shift(x,dir) & enemySpace;
            x |= Shift(x,dir) & enemySpace;
            x |= Shift(x,dir) & enemySpace;
            x |= Shift(x,dir) & enemySpace;

            possibleSpace |= (Shift(x,dir) & freeSpace);
        }

        int id = 0;
        possibleMoves[depth][0] = -1;

        for(int i = 0; i < 64; i++)
        {
            if(Get(i,possibleSpace))
            {
                possibleMoves[depth][id++] = i;
            }
        }
        
        if(id == 0) id = 1;

        return id;
    }

    int evalState()
    {

        double cornersMe = 0;
        double cornersEnemy = 0;
        
        if(Get(0,mySpace)) cornersMe ++;
        if(Get(0,enemySpace)) cornersEnemy ++;

        if(Get(56,mySpace)) cornersMe ++;
        if(Get(56,enemySpace)) cornersEnemy ++;
        
        if(Get(7,mySpace)) cornersMe ++;
        if(Get(7,enemySpace)) cornersEnemy ++;

        if(Get(63,mySpace)) cornersMe ++;
        if(Get(63,enemySpace)) cornersEnemy ++;


        


        int coinMe = 0;
        int coinEnemy = 0;
        int enemyWeight = 0;
        int myWeight = 0;

        for(int i = 0; i < 63; i++)
        {
            int x = i/8, y = i%8;
            if(Get(i,mySpace))
            {
                coinMe ++; 
                myWeight += V[x][y];
            } 
            if(Get(i,enemySpace))
            {
                coinEnemy ++; 
                enemyWeight += V[x][y];
            } 
        }
                
        int possibleMovesMeSize = genPossibleMoves(98);

        gameState newState = doMove(-1);
        int possibleMovesEnemySize = newState.genPossibleMoves(98);

        int mobility = 0;
        if(possibleMovesMeSize > possibleMovesEnemySize)
            mobility = (100.0*possibleMovesMeSize) / (possibleMovesEnemySize + possibleMovesMeSize);
        else if(possibleMovesMeSize < possibleMovesEnemySize)
            mobility = -(100.0*possibleMovesEnemySize) / (possibleMovesEnemySize + possibleMovesMeSize);

        int weight = 0;
        if(myWeight > enemyWeight)
        {
            weight = (100.0*myWeight) / (myWeight + enemyWeight);
        } else if(myWeight < enemyWeight)
        {
            weight = -(100.0*enemyWeight) / (myWeight + enemyWeight);
        }

        int coins = 0;
        if(coinMe > coinEnemy)
        {
            coins = (100.0*coinMe) / (coinMe + coinEnemy);
        } else if (coinMe < coinEnemy)
        {
            coins = -(100.0*coinEnemy) / (coinMe + coinEnemy);
        }

        int corners = 0;
        if(cornersMe > cornersEnemy)
        {
            corners = (100.0*cornersMe) / (cornersMe + cornersEnemy);
        } else if(cornersMe < cornersEnemy)
        {
            corners = -(100.0*cornersEnemy) / (cornersMe + cornersEnemy);
        }

        int ans = 0;
        ans += 200 * weight;
        ans += 50 * coins;
        ans += 250 * mobility;
        ans += 400 * corners;

        if (PLAYER != currPlayer) ans *= -1;

        return (int)ans;
    }   
    gameState doMove(int move)
    {
        gameState newState(*this);

        newState.currPlayer = 1-currPlayer;

        if(move == -1)
        {
            if(prevPass) newState.gameEnded = true;
            else newState.gameEnded = false;
            newState.prevPass = true;

            swap(newState.mySpace, newState.enemySpace);

            return newState;
        } 
        newState.prevPass = false;
        newState.gameEnded = false;
        


        ull newMove = 0;

        Set(move,&newMove);

        newState.Set(move,&(newState.mySpace));

        ull toFlip = 0;
        for(int dir = 0 ; dir < 8; dir++)
        {
            ull x = Shift(newMove,dir) & newState.enemySpace;

            x |= Shift(x,dir) & newState.enemySpace;
            x |= Shift(x,dir) & newState.enemySpace;
            x |= Shift(x,dir) & newState.enemySpace;
            x |= Shift(x,dir) & newState.enemySpace;
            x |= Shift(x,dir) & newState.enemySpace;

            toFlip |= ((Shift(x,dir) & newState.mySpace) ? x : 0ULL);
        }

        newState.mySpace ^= toFlip;
        newState.enemySpace ^= toFlip;

        swap(newState.mySpace, newState.enemySpace);
        return newState;
    }
    int randomMove()
    {
        int n = genPossibleMoves(0);
        return possibleMoves[0][rand() % n];
    }
    int calculateResult()
    {
        int ans = 0;

        for(int i = 0; i < 64; i++)
        {
            if(Get(i,mySpace)) ans++;
            if(Get(i,enemySpace)) ans--;
        }
        if (PLAYER != currPlayer) ans*=-1;
        return ans > 0 ? 1 : -1;

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
        gameState newState = state.doMove(possibleMoves[depth][i]);

        int eval = alfa_beta(newState, depth+1, alfa, beta);

        if (eval == INT_MIN) return INT_MIN;

        if(depth % 2 == 0)
        {
            evalForThis = max(evalForThis, eval);
            if(evalForThis >= beta) return evalForThis;
            alfa = max(alfa,evalForThis);
        } else 
        {
            evalForThis = min(evalForThis, eval);
            if(evalForThis <= alfa) return evalForThis;
            beta = min(beta,evalForThis);
        }
    }

    return evalForThis;
}

int bestWAGA;

int minmaxDecision(gameState toEval)
{
    int bestMoveANS = -1;

    MAX_DEPTH = DEPTH_TO_START;
    bool stillTime = 1;

    while(stillTime && MAX_DEPTH <= 64)
    {
        int n = toEval.genPossibleMoves(0);
        int maks = INT_MIN;
        int bestMove = -1;
        for(int i = 0; i < n; i++)
        {
            gameState newState = toEval.doMove(possibleMoves[0][i]);

            int move = possibleMoves[0][i];
            //cerr << (char)(move%8 + 'a') << (move/8) +1  << "\n";

            int minmaxEval = alfa_beta(newState, 1, INT_MIN, INT_MAX);

            if(minmaxEval == INT_MIN) return bestMoveANS;

            //cerr << "TESTY " << minmaxEval << " " << possibleMoves[0][i] << "\n";

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
    cin >> PLAYER; cin.ignore();
    cerr << "PLAYER POOG " << PLAYER << "\n";

    int board_size;
    cin >> board_size; cin.ignore();
    
    // game loop
    while (1) {

        gameState game;

        game.currPlayer = PLAYER;
        

        cerr << "NEW TURN\n";
        for (int i = 0; i < board_size; i++) {
            string line; // rows from top to bottom (viewer perspective).
            cin >> line; cin.ignore();
            //cerr << line << "\n";

            for(int j = 0; j < 8; j++)
            {
                int x = i*8 + j;
                if(line[j] == '1')
                    if(PLAYER == 1)
                    {
                        game.Set(x,&(game.mySpace));
                        //cerr << "testSeta1 " << game.Get(i,j,game.mySpace)<< "\n";
                    } else {
                        game.Set(x,&(game.enemySpace));
                        //cerr << "testSeta1 " << game.Get(i,j,game.enemySpace)<< "\n";
                    }
                        
                if(line[j] == '0')
                    if(PLAYER == 1)
                    {
                        game.Set(x,&(game.enemySpace));
                        //cerr << "testSeta2 " << game.Get(i,j,game.enemySpace) << "\n";
                    } else {
                        game.Set(x,&(game.mySpace));
                        //cerr << "testSeta2 " << game.Get(i,j,game.mySpace) << "\n";
                        
                    }
            }
        }   
        //cerr << "Moja plansza Before\n";
        //game.print();
        //cerr << "\n";

        int action_count; // number of legal actions for this turn.
        cin >> action_count; cin.ignore();
        cerr << "MOZLIWE RUCHY\n";
        for (int i = 0; i < action_count; i++) {
            string action; // the action
            cin >> action; cin.ignore();
            cerr << action << "\n";
        }

        start_time = chrono::high_resolution_clock::now();

        int move = minmaxDecision(game);
        
        cout << (char)(move%8 + 'a') << (move/8) +1 << " MSG ";

        cout << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() << " " << MAX_DEPTH-2 << " " << bestWAGA << endl;


        TIME_FOR_TURN = 147;
        //cerr << "Moja plansza After\n";
        //game.print();
        //cerr << "\n";
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // cout << "f4" << endl; // a-h1-8
    }
}