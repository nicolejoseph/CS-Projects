
#include <vector>

using namespace std;
struct Turn;

class Board {
public:
    Board(); // default constructor
    void createMyGame(); // create a specific game board
    void printRow(int i);
    void printBoard();

    void play();

    bool isValidTurn(const Turn& t);

    bool isValidDirectionMove(const Turn& t);

    void makeMove(const vector<Turn>& turns, const int index);
    void makeMove(const Turn& t);
    int heuristic();

    // DATA

    static const int HumanRegularPiece = 1;
    static const int HumanKingPiece = 3;
    static const int AiRegularPiece = 2;
    static const int AiKingPiece = 4;

    using GameBoard = vector< vector<int> >;
    GameBoard board; // board[i][j] refers to that position on the board
        // 0 for empty space;
        // 1 for player 0 (human);
        // 2 for player 1 (AI);
        // 3 for player 0 king (human);
        // 4 for player 1 king (AI).

    int currentPlayer; // 0 or 1: whose turn is it

    int player1pieces;
    int player2pieces;
    bool gameOver;

    char color;
    char pieces[8][4];

    // Related to the time limit
    bool timeUp;
    time_t startTime;
    time_t endTime;
    static int timeLimit = 5; // Time limit in seconds

    // OTHER METHODS
    vector<Turn> getLegalMoves();

    vector<Turn> getLegalMoves(int i, int j);
    // Returns a list of legal moves from the current position (i, j).
    // The vector could be empty or have multiple turns.
    // whatever calls it can disregard non-capture moves

    static GameBoard getNewBoard();
    // Create and return a board in the beginning state.

    // Related to backtracking/searching through game tree
    vector<Turn> currentTurns;
    Turn tempBestMove;
    int tempBestMoveIndex;
    Turn& nextTurn();
    void undoMove(const Turn& t);
    int curDepth = -1;
    int maxD = 15;
};

class Piece {
public:

};

struct Turn {
    // Represents a move (either move or jump) from one spot to another.
    int distance; // how many spots you're moving over.

    int x0;
    int y0;

    int x1;
    int y1;

    int x2;
    int y2;

    int x3;
    int y3;

    string print() const;

    bool operator<(const Turn& t) const
    {
        return this->distance < t.distance;
    }
};
