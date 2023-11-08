// Assignment [4] [Dawid] [Skowronek] [330235] [Sko0owi]

#pragma GCC optimize("O3","unroll-loops","inline") //Optimization flags
#pragma GCC target("arch=haswell,avx")  //Enable AVX



#include <bits/stdc++.h>
#include <sys/time.h>

#define INLINE inline __attribute__((always_inline))
#define ull unsigned long long
#define ll  long long

using namespace std;


constexpr int KH = 1;
constexpr int MAX = 1;
constexpr int MIN = -1;
constexpr int DEBUG = 0;
constexpr int PDF = 1;

int max_depth;
int best_eval;

int depth_to_start = 1;

int end_cost = 1000000;

int time_for_turn = 980;

int player_color;

int turn_index; 





timeval start_time, end_time;
int nodes_visited[500];
int possibleMoves[500][10];
int killers[500][10];


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



INLINE ull splittable64(ull x)
{
    x ^= x >> 30;
    x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27;
    x *= UINT64_C(0x94d049bb133111eb);
    x ^= x >> 31;
    return x;
}

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
        if(move >= 0 && !Get(62 - move,freeSpace)) return true;
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

    ull hash()
    {
        
        uint64_t lower_hash = splittable64(mySpace);
        uint64_t upper_hash = splittable64(enemySpace);
        uint64_t rotated_upper = upper_hash << 31 | upper_hash >> 33;
        return lower_hash ^ rotated_upper;
    
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

    ll evalState(bool verbose = false)
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


        ll my_duos = 0, enemy_duos = 0;
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

        if (color == player_color) prev_won *= -1;
        return prev_won;
    }
};


enum Flag : int8_t
{
    kdefault = 0,
    klowerbound = 1,
    kupperbound = 2,
};

struct TTentry
{
    gameState stanGry;
    uint64_t hash;
    int32_t value;
    int16_t move;
    int16_t depth;
    Flag flag;

    TTentry()
    {
        hash = 0;
        value = 0;
        move = -1;
        depth = 0;
        flag = kdefault;
    }
};

const int TTsize = (1 << 22);
const int INDEX_MASK = TTsize - 1;

TTentry TTtable[TTsize];

int ttstats_collisions, ttstats_movecollisions, ttstats_eqstate, ttstats_ERROR1;

bool isValid(const TTentry &entry, gameState &state2,
const uint64_t hash2)
{
    if constexpr (PDF)
    {
        if (entry.hash == 0) return false ; // invalid entry

        if (entry.hash != hash2) { // type-2 error
            ttstats_collisions ++; return false ;
        }
        if (!state2.goodMove(entry.move)) { // hash move error
            ttstats_movecollisions ++; return false ;
        }
        if(entry.stanGry.mySpace == state2.mySpace && entry.stanGry.enemySpace == state2.enemySpace)
        {
            ttstats_eqstate++;
            return true;
        }
        ttstats_ERROR1 ++; // type-1 error
        return false ; // or exit with error for debug
    } else 
    {
        return entry.hash == hash2 && state2.goodMove(entry.move);
    }
}

template <int player> 
int alfa_beta(gameState state, int depth, int alfa, int beta)
{

    if constexpr (PDF) nodes_visited[depth] ++;



    int initAlpha = alfa, initBeta = beta;
    uint64_t hash = state.hash();

    //state.print();
    //cout << "HASH " << hash << "\n";

    //cout << TTsize << "\n";
    //cout << INDEX_MASK << "\n";
    //cout << (hash & INDEX_MASK) << "\n";
    //cout << "HASH & INDEX " << (hash & INDEX_MASK) << "\n";

    TTentry &entry = TTtable[hash & INDEX_MASK]; // retrieve

    //cout << entry.hash << " " << entry.depth << " " << entry.value << " " << entry.move << "\n";
    if (isValid(entry, state, hash))
    { 
        if (entry.depth + depth >= max_depth) // and of enough quality
        { 
            switch (entry.flag) 
            {
                case Flag::kdefault : { return entry.value; }
                case Flag::klowerbound : {if (entry.value > alfa) alfa = entry.value;} break;
                case Flag::kupperbound : {if (entry.value < beta) beta = entry.value;} break;
            }
            if (alfa >= beta) return entry.value; // cutoff
        }
    }

    int checkForWin = state.checkWin();
    if (checkForWin != 0) return checkForWin * end_cost;

    if (depth >= max_depth) return state.evalState();
    
    

    gettimeofday(&end_time,NULL);
    ull diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;
    if( diff >= time_for_turn) return INT_MIN;
    
    int n = state.genPossibleMoves(depth);

    int score = INT_MIN;
    uint8_t move;

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
        score = INT_MIN;

        if (state.goodMove(entry.move))
        {
            gameState newState = state.doMove(entry.move);
            int eval = alfa_beta<MIN>(newState, depth+1, alfa, beta);
            if (eval>score)
            {
                score=eval; move=entry.move;
                if (eval > alfa) 
                {
                    alfa = eval;
                    if(eval >= beta)
                    {
                        if constexpr (KH)
                        {
                            killers[depth][1] = killers[depth][0];
                            killers[depth][0] = entry.move;
                        }
                        goto end;
                    } 
                }
            }
            
        }

        for(int i = 0; i < n; i++)
        {
            gameState newState = state.doMove(possibleMoves[depth][i]);
            int eval = alfa_beta<MIN>(newState, depth+1, alfa, beta);

            if (eval == INT_MIN) return INT_MIN;

            if (eval > score)
            {
                score=eval; move=possibleMoves[depth][i];
                if (eval > alfa) 
                {
                    alfa = eval;
                    if (eval >= beta)
                    {
                        if constexpr (KH)
                        {
                            killers[depth][1] = killers[depth][0];
                            killers[depth][0] = possibleMoves[depth][i];
                        }
                        goto end;
                    } 
                }
            }
        }

    } else // player MIN
    {
        score = INT_MAX;

        if (state.goodMove(entry.move))
        {
            gameState newState = state.doMove(entry.move);
            int eval = alfa_beta<MAX>(newState, depth+1, alfa, beta);
            if (eval < score)
            {
                score=eval; move=entry.move;
                if (eval < beta) 
                {
                    beta = eval;
                    if(eval <= alfa)
                    {
                        if constexpr (KH)
                        {
                            killers[depth][1] = killers[depth][0];
                            killers[depth][0] = entry.move;
                        }
                        goto end;
                    } 
                }
            }
        }
        for(int i = 0; i < n; i++)
        {
            gameState newState = state.doMove(possibleMoves[depth][i]);
            int eval = alfa_beta<MAX>(newState, depth+1, alfa, beta);

            if (eval == INT_MIN) return INT_MIN;

            if (eval < score)
            {
                score=eval; move=entry.move;
                if (eval < beta) 
                {
                    beta = eval;
                    if(eval <= alfa)
                    {
                        if constexpr (KH)
                        {
                            killers[depth][1] = killers[depth][0];
                            killers[depth][0] = possibleMoves[depth][i];
                        }
                        goto end;
                    } 
                }
            }
        }
    }

    end:
    // if (depth >= entry.depth) 
    // { // replace by depth strategy
        Flag flag;
        if (score <= initAlpha ) flag = Flag::kupperbound;
        else if (score >= initBeta ) flag = Flag::klowerbound ;
        else flag = Flag::kdefault ;
        //store(entry, hash, depth, value, move);
        entry.hash=hash; entry.depth=depth; entry.value=score; entry.move=move; entry.stanGry = state;
    // }
    return score;
        
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
            if constexpr (DEBUG == 2)
            {
                if(max_depth == 7 || max_depth == 3 || max_depth == 13)
                {
                    cout << "depth: " << max_depth << " ";
                    cout << "game After move " << possibleMoves[0][i] << "\n";
                    newState.print();
                    int val = newState.evalState(true);
                    cout << "eval planszy: " << val << "\n"; 
                }
            }
            
            int checkForWin = newState.checkWin();
            if (checkForWin != 0)
            {
                best_eval = 80085;
                return possibleMoves[0][i]; // wygrywający ruch
            }

            int move = possibleMoves[0][i];

            int minmaxEval = alfa_beta<MIN>(newState, 1, INT_MIN, INT_MAX);

            if constexpr (DEBUG == 2)
            {
                if(max_depth == 7 || max_depth == 13)
                {
                    cout << "minmax val: " << minmaxEval << "\n";
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

int enemyMoves[] = {1,  3,  1,  3,  4,  6,  4,  4,  6,  7,  3,  1,  2,  4,  6,  1,  7,  6,  8,  1,  0,  0,  0,  0,  8,  8,  5};

int main()
{
    int my_id, opp_id;
    // cin >> my_id >> opp_id; cin.ignore();

    player_color = 1;

    gameState game; 
    game.reset();

    int id = 0;

    // game loop
    while (1) {
        // cin >> turn_index; cin.ignore(); // ignore
        if constexpr (DEBUG) cout << "CURRENT_TURN :" << turn_index << "\n";

        gettimeofday(&start_time,NULL);
        for(int i = 0; i < 64; i++)
        {
            nodes_visited[i] = 0;
        }


        int opp_previous_action = enemyMoves[id++];
        if(id == 27) break;

        if(opp_previous_action != -1)
        {
            game = game.doMove(opp_previous_action);

            if constexpr (DEBUG)
            {
                cout << "After Enemy\n";
                game.print();
                int val = game.evalState(true);
                cout << "EVAL: " << val << "\n";
            }
            
        } 

        cout << game.mySpace << " " << game.enemySpace << "\n";


        int my_move = -1;
        
        my_move = minmaxDecision(game);
        game = game.doMove(my_move);

        if constexpr (DEBUG)
        {
            cout << "After Me\n";
            game.print();
            int val = game.evalState(true);
            cout << "EVAL: " << val << "\n";
        }
        

        cout << my_move << " ";

        gettimeofday(&end_time,NULL);
        ull diff = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_usec - start_time.tv_usec) / 1000;

        ull sum = 0;
        if constexpr (PDF)
        {
            for(int i = 0; i <= max_depth-2; i++)
            {   
                sum += nodes_visited[i];
                cout << "Depth Nodes Visited: " << i << " " << nodes_visited[i] << "\n";
            }
        }   
        cout << "Sum: " << sum << "\n";
        cout << "STATY: " << ttstats_collisions << " " <<  ttstats_movecollisions << " " << ttstats_eqstate << " " <<  ttstats_ERROR1 << "\n";
        ttstats_collisions = ttstats_movecollisions =ttstats_eqstate = ttstats_ERROR1 = 0;
        
        cout << diff << " " << max_depth-2 << " " << best_eval << endl;

        time_for_turn = 95;
    }
}