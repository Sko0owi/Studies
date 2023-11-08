// Assignment [4] [Dawid] [Skowronek] [330235] [Sko0owi]

#pragma GCC optimize("O3","unroll-loops","inline") //Optimization flags
#pragma GCC target("arch=haswell,avx")  //Enable AVX

#include<bits/stdc++.h>
#include <sys/time.h>

#define ull unsigned long long

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

constexpr int KH = 0;
constexpr int MAX = 1;
constexpr int MIN = -1;

int max_depth;
int best_eval;

int nodes_visited = 0;

int depth_to_start = 3;

int end_cost = 1000000;

int time_for_turn = 980;

int player_color;

int turn_index; 




timeval start_time, end_time;
int possibleMoves[500][10];
int killers[2][10];


ull masks[8] = {
    0x7fbfdfeff7fbfdfeULL, // left (<< 1)
    0xbfdfeff7fbfdfeffULL, // up-right (<< 8)
    0xfffffffffffffe00ULL, // up (<< 9)
    0x7fbfdfeff7fbfe00ULL, // up-left (<< 10)

    0xbfdfeff7fbfdfeffULL, // right (>> 1)
    0x7fbfdfeff7fbfdfeULL, // down-left (>> 8)
    0xffffffffffffffffULL, // down (>> 9)
    0xbfdfeff7fbfdfeffULL, // down-right (>> 10)

};

static short shiftValR[8] = {
    0,0,0,0,1,8,9,10
};
static short shiftValL[8] = {
    1,8,9,10,0,0,0
};

ull Shift(ull toShift, int dir)
{
    return ((toShift >> shiftValR[dir]) << shiftValL[dir]) & masks[dir];
}

inline void Set(int pos, ull *toSet)
{
    *toSet |= ((ull)1 << pos); 
}
inline int Get(int pos, ull toGet)
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
                    cerr << ".";
                else if (Get(ind, mySpace))
                    cerr << '1';
                else if (Get(ind, enemySpace))
                    cerr << '0';
            } 
            cerr << "\n";
        }
        cerr << "\n";
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

    int evalState()
    {
        ull freeSpace = ~(mySpace | enemySpace);

        int ans = 0;

        int my_threes = 0, enemy_threes = 0;
        for(int dir = 0; dir < 8; dir++)
        {
            ull x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) enemy_threes += __builtin_popcount(x);

            x = mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) my_threes += __builtin_popcount(x);
        }
        ans += (my_threes - enemy_threes) * 10;

        int my_duos = 0, enemy_duos = 0;
        for(int dir = 0; dir < 8; dir++)
        {
            ull x = enemySpace;
            x = Shift(x, dir) & enemySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) enemy_duos += __builtin_popcount(x);

            x = mySpace;
            x = Shift(x, dir) & mySpace;
            x = Shift(x, dir) & freeSpace;
            x = Shift(x, dir) & freeSpace;
            if (x > 0) my_duos += __builtin_popcount(x);
        }

        ans += my_duos - enemy_duos;
        
        if(color != player_color) ans *= -1;

        return ans;
    }   
    // gameState doMove(int move) TODO testing
    // {
    //     gameState newState(*this);

    //     newState.color = 1-color;

    //     if(move == -2) return newState; // STEAAAAL


    //     ull mask_col = 0x8040201008040201;
    //     mask_col >>= (move+1);

        
    //     ull fullSpace = (mySpace | enemySpace);
    //     ull freeSpace = ~fullSpace;

    //     ull mySpaceNew = mySpace | ((Shift(fullSpace, 2) & mask_col) & freeSpace) | (1 << (8 - move)) & freeSpace;
    //     newState.mySpace = mySpaceNew;

    //     swap(newState.mySpace, newState.enemySpace);
    //     return newState;
    // }

    gameState doMove(int move)
    {
        gameState newState(*this);

        newState.color = 1-color;

        if(move == -2) return newState; // STEAAAAL

        move = 8 - move; // magia bitowa time
        ull freeSpace = ~(enemySpace | mySpace);

        for(int i = 6; i >= 0; i--)
        {
            int ind = (i-1) * 9 + move;
            if(!Get(ind, freeSpace) || i == 0)
            {
                Set(i * 9 + move, &(newState.mySpace));
                break;
            }
        }

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

        if (color == player_color) prev_won *= -1;
        return prev_won;
    }
};


int alfa_beta(gameState state, int depth, int alfa, int beta)
{
    nodes_visited ++;
    int checkForWin = state.checkWin();
    if (checkForWin != 0)
    {
        return checkForWin * end_cost;
    }

    gettimeofday(&end_time,NULL);
    ull diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
    if( diff >= time_for_turn) return INT_MIN;

    
    if(depth >= max_depth) return state.evalState();
    

    
    int n = state.genPossibleMoves(depth);

    int evalForThis = (depth%2 == 0) ? INT_MIN : INT_MAX;

    if constexpr (KH)
    {
        for(int i = 0 ; i < 2 ; i++)
        {
            if(!state.goodMove(killers[i][depth]))
            {
                continue;
            }

            gameState newState = state.doMove(killers[i][depth]);

            int eval = alfa_beta(newState, depth+1, alfa, beta);

            if (eval == INT_MIN) return INT_MIN;

            if (depth % 2 == 0)
            {
                evalForThis = max(evalForThis, eval);
                if(evalForThis >= beta)
                {
                    if (i == 1)
                    {
                        swap(killers[0][depth], killers[1][depth]);
                    }
                    return evalForThis;
                }
                alfa = max(alfa,evalForThis);
            } else 
            {
                evalForThis = min(evalForThis, eval);
                if(evalForThis <= alfa)
                {
                    if (i == 1)
                    {
                        swap(killers[0][depth], killers[1][depth]);
                    }
                    return evalForThis;
                }
                beta = min(beta,evalForThis);
            }
        } 
    }


    for(int i = 0; i < n; i++)
    {
        gameState newState = state.doMove(possibleMoves[depth][i]);
        
        int eval = alfa_beta(newState, depth+1, alfa, beta);

        if (eval == INT_MIN) return INT_MIN;

        if (depth % 2 == 0)
        {
            evalForThis = max(evalForThis, eval);
            if(evalForThis >= beta)
            {
                if constexpr (KH)
                {
                    killers[1][depth] = possibleMoves[depth][i];
                }
                return evalForThis;
            }
            alfa = max(alfa,evalForThis);
        } else 
        {
            evalForThis = min(evalForThis, eval);
            if(evalForThis <= alfa)
            {
                if constexpr (KH)
                {
                    killers[1][depth] = possibleMoves[depth][i];
                }
                return evalForThis;
            }
            beta = min(beta,evalForThis);
        }
    }

    return evalForThis;
}


int minmaxDecision(gameState toEval)
{
    int best_move = -1;

    max_depth = depth_to_start;
    bool stillTime = 1;

    while(stillTime && max_depth <= 64)
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

            int minmaxEval = alfa_beta(newState, 1, INT_MIN, INT_MAX);

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
    int my_id, opp_id;
    cin >> my_id >> opp_id; cin.ignore();

    if (my_id == 0) player_color = 0;
    else player_color = 1;

    gameState game; 
    game.reset();

    // game loop
    while (1) {
        cin >> turn_index; cin.ignore(); // ignore
        cerr << "CURRENT_TURN :" << turn_index << "\n";

        gettimeofday(&start_time,NULL);
        nodes_visited = 0;

        for (int i = 0; i < 7; i++) { // ignore
            string board_row; 
            cin >> board_row; cin.ignore();
        } 


        int num_valid_actions;
        cin >> num_valid_actions; cin.ignore();


        for (int i = 0; i < num_valid_actions; i++) { // ignore
            int action;
            cin >> action; cin.ignore();
        }

        int opp_previous_action;
        cin >> opp_previous_action; cin.ignore();

        if(opp_previous_action != -1)
        {
            game = game.doMove(opp_previous_action);
            cerr << "After Enemy\n";
            game.print();
        } 


        if (turn_index == 1) num_valid_actions --;

        
        int my_valid_actions = game.genPossibleMoves(0);


        assert(my_valid_actions == num_valid_actions);

        int my_move = -1;
        
        my_move = minmaxDecision(game);
        game = game.doMove(my_move);

        cerr << "After Me\n";
        game.print();

        cout << my_move << " ";

        gettimeofday(&end_time,NULL);
        ull diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;

        cout << diff << " " << max_depth-2 << " " << best_eval << " " << nodes_visited << endl;

        time_for_turn = 95;
    }
}