// Assignment [1] [Dawid] [Skowronek] [330235] [Sko0owi]


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
int CG = 1;

pair<int,int> possibleMoves[500][200];

int MAX_DEPTH;

int DEPTH_TO_START = 3;

int END_COST = 1000000;

int TIME_FOR_TURN = 995;

int PLAYER;

auto start_time = chrono::high_resolution_clock::now();



// ull masks[8] = {
//     0xFEFEFEFEFEFEFEFEULL, // right (>> 1)
//     0x7F7F7F7F7F7F7F00ULL, // down-right (>> 9)
//     0xFFFFFFFFFFFFFFFFULL, // down (>> 8)
//     0xFEFEFEFEFEFEFE00ULL, // down-left (>> 7)
//     0xFEFEFEFEFEFEFEFEULL, // left (<< 1)
//     0x00FEFEFEFEFEFEFEULL, // up-left (<< 9)
//     0xFFFFFFFFFFFFFFFFULL, // up (<< 8)
//     0x007F7F7F7F7F7F7FULL, // up-right (<< 7)
// };

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

    bool currPlayer = 0;
    bool whiteTurn = 0;

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
        

    void reset(int PLAYER)
    {
        whiteTurn = 1;

        mySpace = 0;
        enemySpace = 0;


        for (int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if (PLAYER == 0)
                {
                    Set(8*i + j, &mySpace);
                } else 
                {
                    Set(8*i + j, &enemySpace);
                }
            }
        }

        for (int i = 6; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if (PLAYER == 0)
                    Set(8*i + j, &enemySpace);
                else 
                    Set(8*i + j, &mySpace);

            }
        }
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

    int genPossibleMoves(int depth)
    {
        ull freeSpace = ~(enemySpace | mySpace);

        ull possibleSpace = 0;

        if(currPlayer == 0)
        {
            if(whiteTurn)
            {
                possibleSpace |= Shift(enemySpace, 6) & freeSpace; // UP check

                possibleSpace |= Shift(enemySpace, 5) & (freeSpace | mySpace); // UP left
                possibleSpace |= Shift(enemySpace, 7) & (freeSpace | mySpace); // UP right
            } else 
            {
                possibleSpace |= Shift(enemySpace, 2) & freeSpace; // DOWN

                possibleSpace |= Shift(enemySpace, 1) & (freeSpace | mySpace); // DOWN RIGHT
                possibleSpace |= Shift(enemySpace, 3) & (freeSpace | mySpace); // DOWN LEFT
            
            }


            int id = 0;

            for(int i = 0; i < 64; i++)
            {
                if(Get(i,possibleSpace))
                {
                    if(whiteTurn)
                    {
                        if(!Get(i,mySpace) &&  Shift((1ull << i),2) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i - 8, i};
                        } 
                        if(Shift((1ull << i),1) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i - 9, i};
                        } 
                        if(Shift((1ull << i),3) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i - 7, i};
                        } 
                    } else 
                    {

                        string move_debug = (char)((i%8)+'a') + to_string(i/8 + 1);
    
                        if(!Get(i,mySpace) && Shift((1ull << i),6) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i + 8, i};
                        } 
                        if(Shift((1ull << i),7) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i + 7, i};
                        } 
                        if(Shift((1ull << i),5) & enemySpace)
                        {
                            possibleMoves[depth][id++] = {i + 9, i};
                        } 
                    }
                }
            }
            return id;
        } else 
        {
            if(whiteTurn)
            {
                possibleSpace |= Shift(mySpace, 6) & freeSpace; // UP check

                possibleSpace |= Shift(mySpace, 5) & (freeSpace | enemySpace); // UP left
                possibleSpace |= Shift(mySpace, 7) & (freeSpace | enemySpace); // UP right
            } else 
            {
                possibleSpace |= Shift(mySpace, 2) & freeSpace; // DOWN

                possibleSpace |= Shift(mySpace, 1) & (freeSpace | enemySpace); // DOWN RIGHT
                possibleSpace |= Shift(mySpace, 3) & (freeSpace | enemySpace); // DOWN LEFT
            
            }


            int id = 0;

            for(int i = 0; i < 64; i++)
            {
                if(Get(i,possibleSpace))
                {
                    if(whiteTurn)
                    {
                        if(!Get(i,enemySpace) &&  Shift((1ull << i),2) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i - 8, i};
                        } 
                        if(Shift((1ull << i),1) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i - 9, i};
                        } 
                        if(Shift((1ull << i),3) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i - 7, i};
                        } 
                    } else 
                    {

                        string move_debug = (char)((i%8)+'a') + to_string(i/8 + 1);
    
                        if(!Get(i,enemySpace) && Shift((1ull << i),6) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i + 8, i};
                        } 
                        if(Shift((1ull << i),7) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i + 7, i};
                        } 
                        if(Shift((1ull << i),5) & mySpace)
                        {
                            possibleMoves[depth][id++] = {i + 9, i};
                        } 
                    }
                }
            }
            return id;
        }

        
    }

    int evalState()
    {
        int enemyPieces = __builtin_popcountll(enemySpace);
        int myPieces = __builtin_popcountll(mySpace);
        if(currPlayer == 0)
        {
            return myPieces - enemyPieces;
        } else 
        {
            return enemyPieces - myPieces;
        }
    }   
    gameState doMove(pair<int,int> move)
    {
        int prev = move.first;
        int next = move.second;
        gameState newState(*this);

        if(newState.currPlayer == 0)
        {
            newState.enemySpace &= ~(1ull << (prev));
            newState.Set(next, &(newState.enemySpace));
            newState.mySpace &= ~(1ull << (next));

        } else 
        {
            newState.mySpace &= ~(1ull << (prev));
            newState.Set(next, &(newState.mySpace));
            newState.enemySpace &= ~(1ull << (next));

        }

        newState.currPlayer = 1-currPlayer;
        newState.whiteTurn = 1-whiteTurn;

        return newState;
    }
    pair<int,int> randomMove()
    {
        int n = genPossibleMoves(0);
        return possibleMoves[0][rand() % n];
    }
    int calculateResult()
    {
        gameEnded = 1;
        if (currPlayer == 0)
        {
            bool lose = 0;
            bool win = 0;
            if(whiteTurn)
            {
                for(int i = 0 ; i < 8 ; i++)
                {
                    if(Get(8*7+i, enemySpace)) win = 1;
                    if(Get(i, mySpace)) lose = 1;
                }
            } else 
            {
                for(int i = 0 ; i < 8 ; i++)
                {
                    if(Get(8*7+i, mySpace)) lose = 1;
                    if(Get(i, enemySpace)) win = 1;
                }
            }
            if (win) return 1;
            return -1;
        } else 
        {
            bool lose = 0;
            bool win = 0;
            if(whiteTurn)
            {
                for(int i = 0 ; i < 8 ; i++)
                {
                    if(Get(8*7+i, mySpace)) win = 1;
                    if(Get(i, enemySpace)) lose = 1;
                }
            } else 
            {
                for(int i = 0 ; i < 8 ; i++)
                {
                    if(Get(8*7+i, enemySpace)) lose = 1;
                    if(Get(i, mySpace)) win = 1;
                }
            }
            if (win) return 1;
            return -1;
        }
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

pair<int,int> minmaxDecision(gameState toEval)
{
    pair<int,int> bestMoveANS;

    MAX_DEPTH = DEPTH_TO_START;
    bool stillTime = 1;

    while(stillTime && MAX_DEPTH <= 64)
    {
        int n = toEval.genPossibleMoves(0);
        int maks = INT_MIN;
        pair<int,int> bestMove;
        for(int i = 0; i < n; i++)
        {
            gameState newState = toEval.doMove(possibleMoves[0][i]);

            pair<int,int> move = possibleMoves[0][i];
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
    int PLAYER = -1;
    gameState game;
    
    // game loop
    while (1) {
        string opponent_move;
        getline(cin, opponent_move); // last move played or "None"

        int legal_moves; // number of legal moves
        cin >> legal_moves; cin.ignore();

        vector<string> moves;
        for (int i = 0; i < legal_moves; i++) {
            string move_string; // a legal move
            cin >> move_string; cin.ignore();
            moves.push_back((move_string));
        }

        if(PLAYER == -1)
        {
            PLAYER = opponent_move == "None" ? 0 : 1;
            cerr << "PLAYER: " << PLAYER << "\n";
            game.reset(PLAYER);
            if (PLAYER == 0) game.currPlayer = 1;
            else game.currPlayer = 0;

        }

        game.print();

        


        if(opponent_move != "None")
        {
            int prev = (opponent_move[0]-'a') + (opponent_move[1]-'0'-1)*8;
            int next = (opponent_move[2]-'a') + (opponent_move[3]-'0'-1)*8;

            game = game.doMove({prev,next});

            cerr << prev << " " << next << "\n";
        }
        

        game.print();

        string move = moves[rand() % moves.size()];
        
        int prev = (move[0]-'a') + (move[1]-'0'-1)*8;
        int next = (move[2]-'a') + (move[3]-'0'-1)*8;

        int ruchy = game.genPossibleMoves(0);


        vector<string> pog;
        for(int i = 0 ; i < ruchy; i++)
        {
            int prev = possibleMoves[0][i].first;
            int next = possibleMoves[0][i].second;
            string move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
            pog.push_back(move);
        }
        sort(pog.begin(), pog.end());
        sort(moves.begin(), moves.end());



        bool GG = 0;
        if(moves.size() != pog.size())
        {
            GG = 1;
        }

        for(int i = 0 ; i < pog.size(); i++)
        {
            cerr << pog[i] << " " << moves[i] << "\n";
            if(pog[i] != moves[i])
            {
                GG = 1;
            }
        }

        if (GG) return 1;



        start_time = chrono::high_resolution_clock::now();

        pair<int,int> decision = minmaxDecision(game);

        prev = decision.first;
        next = decision.second;
        move = (char)(prev%8 + 'a') + to_string((prev/8) +1) + (char)(next%8 + 'a') + to_string((next/8) +1);
            
        cout << move << " MSG ";

        game = game.doMove(decision);

        cout << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start_time).count() << " " << MAX_DEPTH-2 << " " << bestWAGA << endl;


        TIME_FOR_TURN = 95;

    }
}