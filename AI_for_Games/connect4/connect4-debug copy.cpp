// Assignment [4] [Dawid] [Skowronek] [330235] [Sko0owi]

#pragma GCC optimize("O3","unroll-loops","inline") //Optimization flags
#pragma GCC target("arch=haswell,avx")  //Enable AVX



#include<bits/stdc++.h>
#include <sys/time.h>

#define INLINE inline __attribute__((always_inline))
#define ull unsigned long long
#define ll  long long

using namespace std;


constexpr int KH = 9;
constexpr int MAX = 1;
constexpr int MIN = -1;
constexpr int DEBUG = 2;

int max_depth;
int best_eval;

int nodes_visited = 0;

int depth_to_start = 3;

int end_cost = 1000000;

int time_for_turn = 980;

int player_color;

int turn_index; 




timeval start_time, end_time;
int possibleMoves[500][100];
int killers[500][100];


ull masks[8] = {
    0x7fbfdfeff7fbfdfeULL, // left (<< 1)
    0x3fdfeff7fbfdfe00ULL, // up-right (<< 8)
    0x7ffffffffffffe00ULL, // up (<< 9)
    0x7fbfdfeff7fbfd00ULL, // up-left (<< 10)

    0x3fdfeff7fbfdfeffULL, // right (>> 1)
    0x7fbfdfeff7fbfdfeULL, // down-left (>> 8)
    0x7fffffffffffffffULL, // down (>> 9)
    0x3fdfeff7fbfdfeffULL, // down-right (>> 10)

};

static short shiftValR[8] = {
    0,0,0,0,1,8,9,10
};
static short shiftValL[8] = {
    1,8,9,10,0,0,0
};

INLINE ull Shift(ull toShift, int dir)
{
    return ((toShift >> shiftValR[dir]) << shiftValL[dir]) & masks[dir];
}

INLINE void Set(int pos, ull *toSet)
{
    *toSet |= ((ull)1 << pos); 
}
INLINE int Get(int pos, ull toGet)
{
    return (toGet & ((ull)1 << pos)) >> pos;
}

struct gameState
{
    ull enemySpace = 0;
    ull mySpace = 0;

    bool color = 0; // 0 - red, 1 - blue

    gameState()
    {
        enemySpace = 0;
        mySpace = 0;
        color = 0;
    }

    void reset()
    {
        enemySpace = 0;
        mySpace =    0;
        color = 0;
    }

    bool goodMove(int move)
    {
        ull freeSpace = ~(enemySpace | mySpace);
        if(!Get(62 - move,freeSpace)) return true;
        return false; 
    }
        

    void print()
    {
        ull freeSpace = ~(enemySpace | mySpace);

        for(int i = 6; i >= 0; i--)
        {
            for(int j = 8; j >= 0; j--)
            {
                int ind = i * 9 + j;
                if (Get(ind, freeSpace))
                    cout << ".";
                else if (Get(ind, mySpace))
                    cout << '1';
                else if (Get(ind, enemySpace))
                    cout << '0';
            } 
            cout << "\n";
        }
        cout << "\n";
    }



    int genPossibleMoves(int depth)
    {
        ull freeSpace = ~(enemySpace | mySpace);
                
        int ind = 0;
        for(int j = 0 ; j < 9; j++)
        {
            if(Get(62 - j, freeSpace))
            {
                possibleMoves[depth][ind++] = j;
            }
        }

        return ind;
    }

    int evalState(bool verbose = false)
    {
        ull freeSpace = ~(mySpace | enemySpace);

        ll ans = 0;

        ll my_threesProb = 0, enemy_threesProb = 0;
        ll my_threesDef = 0, enemy_threesDef = 0;



        if constexpr (DEBUG == 2)
        {
            if(verbose)
            {
                ull x = enemySpace;
                x = Shift(x, 2) & enemySpace;
                x = Shift(x, 2) & enemySpace;
                x = Shift(x, 2) & freeSpace;
                cout << "POMOCY: " << x << "\n";
                ull xDef = (Shift(x,6) & (enemySpace | mySpace)) | (x & 0x1ff);
                cout << "POMOCYDEF: " << xDef << "\n";
            }
            
        }
        for(int dir = 0; dir < 8; dir++)
        {
            ull x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & freeSpace;

            ull xDef = (Shift(x,6) & (enemySpace | mySpace)) | (x & 0x1ff);

            
            enemy_threesDef += __builtin_popcountll(xDef);
            enemy_threesProb += __builtin_popcountll(x & (~xDef));
            

            x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & enemySpace;

            x = Shift(x, (dir + 4) % 8) & freeSpace;
            xDef = (Shift(x,6) & (enemySpace | mySpace)) | (x & 0x1ff);

            enemy_threesDef += __builtin_popcountll(xDef);
            enemy_threesProb += __builtin_popcountll(x & (~xDef));

            x = mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & freeSpace;

            xDef = (Shift(x,6) & (enemySpace | mySpace)) | (x & 0x1ff);
            my_threesDef += __builtin_popcountll(xDef);
            my_threesProb += __builtin_popcountll(x & (~xDef));

            x = mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & mySpace;

            x = Shift(x, (dir + 4) % 8) & freeSpace;
            xDef = (Shift(x,6) & (enemySpace | mySpace)) | (x & 0x1ff);
            my_threesDef += __builtin_popcountll(xDef);
            my_threesProb += __builtin_popcountll(x & (~xDef));
        }
        if(verbose)
        {
            cout << my_threesProb << " " << enemy_threesProb << " " << my_threesDef << " " << enemy_threesDef << "\n";
        }

        ans += (my_threesProb - enemy_threesProb) * 10;


        int my_duos = 0, enemy_duos = 0;
        for(int dir = 0; dir < 8; dir++)
        {
            ull x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) enemy_duos += __builtin_popcountll(x);

            x = mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) my_duos += __builtin_popcountll(x);
        }
        if(verbose)
        {
            cout << my_duos << " " << enemy_duos << "\n";
        }

        ans += my_duos - enemy_duos;

        if(verbose)
        {
            cout << "Kolory: " << color << " " << player_color << "\n";
        }

        if(color != player_color)
        {
            if(my_threesDef > 0) return -1000;
        } else 
        {
            if(enemy_threesDef > 0) return -1000;
        }

        
        if(color != player_color) ans *= -1;

        return ans;
    }   
    gameState doMove(int move)
    {
        gameState newState(*this);

        newState.color = 1-color;

        if(move == -2) return newState; // STEAAAAL


        ull mask_col = 0x8040201008040201;
        mask_col >>= (move+1);

        
        ull fullSpace = (mySpace | enemySpace);
        ull freeSpace = ~fullSpace;

        ull mySpaceNew = mySpace | ((Shift(fullSpace, 2) & mask_col) & freeSpace) | (1 << (8 - move)) & freeSpace;
        newState.mySpace = mySpaceNew;

        swap(newState.mySpace, newState.enemySpace);
        return newState;
    }

    int checkWin()
    {
        int prev_won = 0;

        for(int dir = 0; dir < 8; dir++)
        {
            ull x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & enemySpace;
            if (x > 0) prev_won = 1;
        }

        if(prev_won)
        {
            // cout << "EZ ";
            // if(color == player_color) cout << "ENEMY\n";
            // else cout << "JA\n";depth
            // print();
            // cout << mySpace << " " << enemySpace << "\n";
        }

        if (color == player_color) prev_won *= -1;
        return prev_won;
    }
};

int move_chosen = 0;

template <int player> 
int alfa_beta(gameState state, int depth, int alfa, int beta)
{
    if constexpr (DEBUG) nodes_visited ++;
    int checkForWin = state.checkWin();
    if (checkForWin != 0) return checkForWin * end_cost;
    

    gettimeofday(&end_time,NULL);
    ull diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
    if( diff >= time_for_turn) return INT_MIN;

    
    if(depth >= max_depth) return state.evalState();
    
    int n = state.genPossibleMoves(depth);
    
    if(n == 0)
    {
        return 0;
    }

    if constexpr (KH)
    {
        for(int i = 0 ; i < 2 ; i++)
        {
            if(!state.goodMove(killers[depth][i]))
            {
                continue;
            }

            if constexpr (player == MAX)
            {
                gameState newState = state.doMove(killers[depth][i]);
                int eval = alfa_beta<MIN>(newState, depth+1, alfa, beta);

                if (eval == INT_MIN) return INT_MIN;

                if(eval >= beta)
                {
                    if(i == 1)
                    {
                        swap(killers[depth][0], killers[depth][1]);
                    }
                    return beta;
                }
                alfa = max(alfa,eval);
            } else // player MIN
            {
                
                gameState newState = state.doMove(killers[depth][i]);
                int eval = alfa_beta<MAX>(newState, depth+1, alfa, beta);

                if (eval == INT_MIN) return INT_MIN;

                if(eval <= alfa)
                {
                    if(i == 1)
                    {
                        swap(killers[depth][0], killers[depth][1]);
                    }
                    return alfa;
                }
                beta = min(beta,eval);
                
            }
        }
    } 
    

    if constexpr (player == MAX)
    {
        vector<pair<int,gameState>> debug;
        int id = 0;
        bool debugBeta = 0;
        for(int i = 0; i < n; i++)
        {
            gameState newState = state.doMove(possibleMoves[depth][i]);
            int eval = alfa_beta<MIN>(newState, depth+1, alfa, beta);

            // if (eval == end_cost || eval == -end_cost) return eval;
            debug.push_back({eval, newState});

            // cout << "Move " << move_chosen << " Max_depth " << max_depth << " Depth: " << depth << " MOVE: " << possibleMoves[depth][i] << " " << eval << "\n";
            // newState.print();

            if (eval == INT_MIN) return INT_MIN;

            if(eval >= beta)
            {
                if constexpr (KH)
                {
                    killers[depth][1] = killers[depth][0];
                    killers[depth][0] = possibleMoves[depth][i];
                }
                debugBeta = 1;
                // return beta;
            }
            alfa = max(alfa,eval);
            
        }

        for(auto it : debug)
        {
            int eval = it.first;
            gameState newState = it.second;
            cout << "Move " << move_chosen << " Max_depth " << max_depth << " Depth: " << depth << " MOVE: " << possibleMoves[depth][id++] << " " << eval << "\n";
            cout << newState.mySpace << " " << newState.enemySpace << "\n";
            newState.print();
        }

        if(debugBeta) return beta;
        return alfa;

        return beta;
    } else // player MIN
    {
        vector<pair<int,gameState>> debug;
        int id = 0;
        bool debugAlfa = 0;
        for(int i = 0; i < n; i++)
        {
            gameState newState = state.doMove(possibleMoves[depth][i]);
            int eval = alfa_beta<MAX>(newState, depth+1, alfa, beta);

            debug.push_back({eval, newState});

            // if (eval == end_cost || eval == -end_cost) return eval;

            if (eval == INT_MIN) return INT_MIN;

            if(eval <= alfa)
            {
                if constexpr (KH)
                {
                    killers[depth][1] = killers[depth][0];
                    killers[depth][0] = possibleMoves[depth][i];
                }
                debugAlfa = 1;
                // return alfa;
            }
            beta = min(beta,eval);
        }

        for(auto it : debug)
        {
            int eval = it.first;
            gameState newState = it.second;
            cout << "Move " << move_chosen << " Max_depth " << max_depth << " Depth: " << depth << " MOVE: " << possibleMoves[depth][id++] << " " << eval << "\n";
            cout << newState.mySpace << " " << newState.enemySpace << "\n";
            newState.print();
        }

        if(debugAlfa) return alfa;
        
        return beta;

    }
        
}

int minmaxDecision(gameState toEval)
{
    int best_move = -1;

    max_depth = depth_to_start;
    bool stillTime = 1;

    while(stillTime && max_depth <= 63)
    {
        int n = toEval.genPossibleMoves(0);
        if(turn_index == 1) possibleMoves[0][n++] = -2;

        int maks = INT_MIN;
        int cbest_move = -1;
        int cbest_eval = INT_MIN;
        for(int i = 0; i < n; i++)
        {

            gameState newState = toEval.doMove(possibleMoves[0][i]);
            
            int checkForWin = newState.checkWin();
            if (checkForWin != 0)
            {
                best_eval = 80085;
                return possibleMoves[0][i]; // wygrywający ruch
            }

            int move = possibleMoves[0][i];

            move_chosen = move;

            int minmaxEval = alfa_beta<MIN>(newState, 1, INT_MIN, INT_MAX);

            if constexpr (DEBUG == 2)
            {
                if(i != -1)
                {
                    cout << "depth: " << max_depth << " ";
                    cout << "game After move " << possibleMoves[0][i] << "\n";
                    cout << newState.mySpace << " " << newState.enemySpace << "\n";
                    newState.print();
                    int val = newState.evalState(true);
                    cout << "eval planszy: " << val << "\n"; 
                }
            }

            if constexpr (DEBUG == 2)
            {
                if(i != -1)
                {
                    cout << "minmax val: " << minmaxEval << " " << maks << "\n\n\n";
                }
            }

            if(minmaxEval == INT_MIN) return best_move;


            if(minmaxEval >= maks)
            {
                maks = minmaxEval;
                cbest_eval = minmaxEval;
                cbest_move = possibleMoves[0][i];
            }
        }
        best_eval = cbest_eval;
        best_move = cbest_move;
        max_depth += 2;
    }
    return best_move;
}


int main()
{

    // stdout = stderr;
    // int my_id, opp_id;
    // cin >> my_id; cin.ignore();


    gameState game; 
    game.reset();

    time_for_turn = 95;

    gettimeofday(&start_time,NULL);

    player_color = 0;
    game.mySpace = 2486866679208948599;
    game.enemySpace = 888418195334907016;




    // player_color = 0;
    // game.mySpace = 2486866679276057463;
    // game.enemySpace = 888418195334909064;

    // cout << "WTF " << game.checkWin();
    // return 0;

    if constexpr (DEBUG)
    {
        cout << "After Enemy\n";
        game.print();
        int val = game.evalState(true);
        cout << "EVAL: " << val << "\n";
    }
            


    int my_move = minmaxDecision(game);
    game = game.doMove(my_move);

    if constexpr (DEBUG)
    {
        cout << "After Me\n";
        game.print();
        int val = game.evalState(true);
        cout << "EVAL: " << val << "\n";
    }
    

    cout << my_move << " ";


    // game loop
    
}