#include <iostream>
#include "board.h"
#include "pointers.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <climits>

Board::Board()
{
    // Set everything to 0; needed to reserve the memory locations since it's a vector
    for (int i = 0; i < 8; i++)
    {
        board.push_back(vector<int>()); // create and insert the row
        vector<int>& row = board[i]; // take a reference to the row that was just added
        for (int j = 0; j < 8; j++)
        {
            row.push_back(0);
        }
    }
    createMyGame();
    gameOver = false;
    // std::cout << "Just created a new board" << std::endl;
}

void Board::printRow(int i)
{
    const vector<int>& row = board[i]; // const ref: since I'm looking at it, not modifying it (for printing)
    cout << i << "        |";

    for (int j = 0; j < 8; j++)
    {
        int piece = row[j]; // same as board[i][j]
        if (piece == 0) {
            cout << "     |";
        }
        else if (piece == 1) {
            cout << "  b  |";
        }
        else if (piece == 2) {
            cout << "  r  |";
        }
        else if (piece == 3) {
            cout << "  B  |";
        }
        else if (piece == 4) {
            cout << "  R  |";
        }
    }
    cout << endl;
}

void Board::printBoard()
{
    cout << "           0      1     2     3     4     5      6     7" << endl;
    cout << "        -------------------------------------------------" << endl;
    for (int i = 0; i < 8; i++)
    {
        printRow(i);
        cout << "        -------------------------------------------------" << endl;
    }
}

Board::GameBoard Board::getNewBoard()
{
    GameBoard newboard =
            {
                    {0, 1, 0, 1, 0, 1, 0, 1},
                    {1, 0, 1, 0, 1, 0, 1, 0},
                    {0, 1, 0, 1, 0, 1, 0, 1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {2, 0, 2, 0, 2, 0, 2, 0},
                    {0, 2, 0, 2, 0, 2, 0, 2},
                    {2, 0, 2, 0, 2, 0, 2, 0},
            };
    return newboard;
}

void Board::createMyGame()
{
    board = Board::getNewBoard();
}

bool isValidJump(const Turn& t)
{
    return false;
}

vector<Turn> Board::getLegalMoves()
{
    vector<Turn> allturns;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            // Check if there's piece at board[i][j]
            int piece = board[i][j];
            if (piece == 0) {
                // cout << "Emtpy spot at (" << i << "," << j << ")" << endl;
                continue;
            }
            // The piece must belong to the current player.
            if (currentPlayer == 0 and (piece == 1 or piece == 3)) {
                const auto turns = getLegalMoves(i, j);
                allturns.insert( allturns.end(), turns.begin(), turns.end() );
            }
            if (currentPlayer == 1 and (piece == 2 or piece == 4)) {
                const auto turns = getLegalMoves(i, j);
                allturns.insert( allturns.end(), turns.begin(), turns.end() );
            }
        }
    }

    // If there are distance=2 moves, discard distance=1 moves.
    bool distance2 = false;
    vector<Turn> bigTurns;
    for (const auto& t : allturns)
    {
        if (t.distance == 2) {
            distance2 = true;
            bigTurns.push_back(t);
        }
    }

    cout << "The possible moves are: " << endl;
    const vector<Turn>& turnsToUse = distance2 ? bigTurns : allturns;

    for (int i = 0; i < turnsToUse.size(); i++)
    {
        cout << i << ". " << turnsToUse[i].print() << endl;
    }
    return turnsToUse;
}

vector<Turn> Board::getLegalMoves(int i, int j)
{
    //cout << "getLegalMoves called with (" << i << "," << j << ")" << endl;
    vector<Turn> turns;
    // ... code

    Turn t = {};
    t.distance = 1;
    t.x0 = i;
    t.y0 = j;
    // First set of cases: distance = 1
    // Case: up-left
    t.x1 = i - 1;
    t.y1 = j - 1;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: up-right
    t.x1 = i - 1;
    t.y1 = j + 1;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: down-left
    t.x1 = i + 1;
    t.y1 = j - 1;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: down-right
    t.x1 = i + 1;
    t.y1 = j + 1;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Second set of cases: distance=2
    t.distance = 2;
    // Case: top-left
    t.x1 = i - 1;
    t.y1 = j - 1;
    t.x2 = i - 2;
    t.y2 = j - 2;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: top-right
    t.x1 = i - 1;
    t.y1 = j + 1;
    t.x2 = i - 2;
    t.y2 = j + 2;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: bottom-left
    t.x1 = i + 1;
    t.y1 = j - 1;
    t.x2 = i + 2;
    t.y2 = j - 2;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    // Case: bottom-right
    t.x1 = i + 1;
    t.y1 = j + 1;
    t.x2 = i + 2;
    t.y2 = j + 2;
    if (isValidTurn(t)) {
        turns.push_back(t);
    }

    return turns;
}

bool Board::isValidTurn(const Turn& t)
{
    // Corner case: move out of board bounds
    if (t.x1 < 0 || t.x1 > 7) {
        return false;
    }

    if (t.y1 < 0 || t.y1 > 7) {
        return false;
    }

    if (t.x2 < 0 || t.x2 > 7) {
        return false;
    }

    if (t.y2 < 0 or t.y2 > 7) {
        return false;
    }

    if (t.x3 < 0 or t.x3 > 7) {
        return false;
    }

    if (t.y3 < 0 or t.y3 > 7) {
        return false;
    }

    // Check for validity based on distance
    if (t.distance == 1) {
        // Destination must be empty spot
        if (board[t.x1][t.y1] != 0) {
            return false;
        }

        // Human non-king pieces can only move DOWN;
        // AI non-king pieces can only move UP.
        if (isValidDirectionMove(t)) {
            return true;
        }
    }

    if (t.distance == 2) {
        // Destination must be empty spot.
        if (board[t.x2][t.y2] != 0) {
            return false;
        }

        // Spot being jumped over must have opponent's piece
        int jumpedOver = board[t.x1][t.y1];
        int originPiece = board[t.x0][t.y0];

        if (currentPlayer == 0) {
            // Human player must jump over AI piece
            if (originPiece == 1 or originPiece == 3) {
                if (jumpedOver == 2 or jumpedOver == 4) {
                    if (isValidDirectionMove(t)) {
                        return true;
                    }
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else {
            // AI player must jump over human piece
            if (originPiece == 2 or originPiece == 4) {
                if (jumpedOver == 1 or jumpedOver == 3) {
                    if (isValidDirectionMove(t)) {
                        return true;
                    }
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
    }

    return false;
}

bool Board::isValidDirectionMove(const Turn& t)
{
    int piece = board[t.x0][t.y0];

    if (currentPlayer == 0) {
        // Non-kings can only move down

        if (piece == 3) { // king
            return true;
        }
        if (t.distance == 1) {
            if (t.x1 <= t.x0) {
                return false;
            }
            else {
                return true;
            }
        }
        else if (t.distance == 2) {
            if (t.x2 <= t.x0) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            return false; // longer distances not supported
        }
    }
    else {
        if (piece == 4) {
            return true;
        }
        if (t.distance == 1) {
            // Non-king can only move up
            if (t.x1 >= t.x0) {
                return false;
            }
            else {
                return true;
            }
        }
        else if (t.distance == 2) {
            if (t.x2 >= t.x0) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            return false; // longer distances, more jumps not yet implemented
        }
    }
    return false; // Any other unsupported cases
}

string Turn::print() const
{
    stringstream s;
    s << "Move of distance " << distance
        << " from (" << x0 << "," << y0
        << ") to (" << x1 << "," << y1 << ")";

    if (distance > 1) {
        s << " to (" << x2 << "," << y2 << ")";
    }
    if (distance > 2) {
        s << " to (" << x3 << "," << y3 << ")";
    }

    return s.str();
}

void Board::play()
{
    player1pieces = 12;
    player2pieces = 12;
    currentPlayer = 0;

    while (!gameOver) {

        printBoard();

        if (currentPlayer == 0) {
            // If human's turn, list the moves
            auto turns = getLegalMoves();
            if (turns.empty()) {
                cout << "No more possible moves. AI wins!" << endl;
                gameOver = true;
                break;
            }

            // Make the human pick
            cout << "HUMAN: Pick your next move" << endl;
            int humanMove; // humanMove will be parameter for whatever function "applies the move"
            cin >> humanMove;

            makeMove(turns, humanMove);
        }
        else {
            auto turns = getLegalMoves();
            if (turns.empty()) {
                cout << "No more possible moves. Human wins!" << endl;
                gameOver = true;
                break;
            }

            cout << "AI: Making the next move" << endl;
            makeAgentMove(turns);
       }
    }
}

void Board::makeAgentMove(const vector<Turn>& turns)
{
    pointers<Board> sboard(new Board*);
    miniMaxAlpha(sboard, 15, numeric_limits<int>::min(), numeric_limits<int>::max());
}

int Board::miniMaxAlpha(pointers<Board>& sboard, int depth, int alpha, int beta){
    if (depth != maxD && sboard->terminal()){
        curDepth = maxD;
        gameOver = true;
        if (sboard->getLegalMoves().empty()){
            return numeric_limits<int>::max();
        }
        else {
            return numeric_limits<int>::min();
        }
    }
    gameOver = false;

    if (depth == 0){
        return sboard -> heuristic();
    }

    int compareAlpha = numeric_limits<int>::min();
    int compareBeta = numeric_limits<int>::max();
    auto turns = sboard->getLegalMoves();

    using IT = vector<Turn>::iterator;
    if (sboard->currentPlayer == 0){
        for (IT it=turns.begin(); it != turns.end(); ++it){
            time(&endTime);
            if (difftime(endTime, startTime) >= (Board::timeLimit)){
                timeUp = true;
                break;
            }
            sboard->makeMove(*it);
            pointers<Board> freshBoard(new Board());
            int v = miniMaxAlpha(freshBoard, depth -1, alpha, min(compareBeta, beta));
            sboard -> undoMove(*it);

            //sboard -> nextTurn();
            if (v > alpha){
                alpha = v;
                if (depth == maxD){
                    tempBestMove = (*it);
                }
            }
            if (alpha >= beta && depth < maxD){
                return alpha;
            }
        }
        if (!timeUp && depth == maxD){
            curDepth = depth;
        }

        return alpha;
    }
    else {
        for (IT it = sboard->getLegalMoves(); it != turns.end(); ++it){
            time (&endTime);
            if (difftime(endTime, startTime) >= board::timeLimit -1){
                timeUp = true;
                break;
            }
            sboard ->makeMove(*it);
            pointers<Board> freshBoard(new Board);
            int v = miniMaxAlpha(freshBoard, depth -1, max(compareAlpha, alpha), beta);
            sboard ->undoMove(*it);
            sboard ->nextTurn();
            if (v < beta){
                beta = v;
                if (depth == maxD){
                    tempBestMove = (*it);
                }
            }
            if (alpha >= beta){
                return beta;
            }
        }
        if (!timeUp && depth == maxD){
            curDepth = depth;
        }
        return beta;
    }
}

void Board::makeMove(const vector<Turn>& turns, const int index)
{
    if (index < 0 or index >= turns.size()) {
        cout << "You picked an invalid move. Try again" << endl;
        return;
    }

    const Turn& t = turns[index];
    makeMove(t);
}

void Board::makeMove(const Turn& t)
{
    int piece = board[t.x0][t.y0];
    if (t.distance == 1) {

        // Check if non-king piece should become king
        if (piece == 1) {
            if (t.x1 == 7) { // reached the bottom row
                piece = 3; // turn into human king piece
            }
        }
        else if (piece == 2) {
            if (t.x1 == 0) { // reached the top row
                piece = 4; // turn into AI king piece
            }
        }

        board[t.x0][t.y0] = 0;
        board[t.x1][t.y1] = piece;

    }
    else if (t.distance == 2) {

        // Check if non-king piece should become king
        if (piece == 1) {
            if (t.x2 == 7) {
                piece = 3;
            }
        }
        else if (piece == 2) {
            if (t.x2 == 0) {
                piece = 4;
            }
        }

        // There will be a captured piece
        int captured = board[t.x1][t.y1];
        board[t.x1][t.y1] = 0; // captured piece
        board[t.x2][t.y2] = piece; // final destination
        board[t.x0][t.y0] = 0; // origin

        // Update the number of pieces
        if (captured == 1 or captured == 3) {
            player1pieces--;
        }
        else if (captured == 2 or captured == 4) {
            player2pieces++;
        }

    }

    // Conditions for if game has ended - INCOMPLETE
    int totalPieces = player1pieces + player2pieces;
    if (totalPieces == 1) {
        gameOver = true;
    }
    if (player1pieces == 0) {
        gameOver = true;
    }
    if (player2pieces == 0) {
        gameOver = true;
    }

    // Switch the players
    currentPlayer++;
    currentPlayer = currentPlayer % 2; // always 0 or 1
}

int Board::heuristic()
{
    int x, y, b, c, d;
    x = y = b = c = d = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j <4; ++j)
        {
            if (pieces[i][j] == AiRegularPiece)
            {
                x = x + 2;
                if (i == 0)
                    b += 9;
                else
                {
                    b += i;

                }
                c += 1;
            }
            else if (pieces[i][j] == HumanRegularPiece)
            {
                y -=2;
                if (i == 7)
                    b -= 9;
                else b -= (7 - i);
                c -= 1;
            }
            else if (pieces[i][j] == AiKingPiece)
            {
                x = x+3;
                c += 1;
            }
            else if (pieces[i][j] == HumanKingPiece)
            {
                y -= 3;
                c -= 1;
            }
        }

    x = x * 500000;
    y *= 500000;
    b *= 100000;
    c *= 1000;
    d *= 100;
    int e = rand() % 100;
    if (color == HumanRegularPiece)
        e = -e;
    return x + y + b + c + d + e;
}

void Board::undoMove(const Turn& t)
{
    currentTurns.erase(currentTurns.size() - 1);
}

Turn& Board::nextTurn()
{
    if (tempBestMoveIndex > currentTurns.size()) {

        Turn t;
        return t;
    }

    return currentTurns[tempBestMoveIndex];
}