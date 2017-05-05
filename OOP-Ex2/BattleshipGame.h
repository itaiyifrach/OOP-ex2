#pragma once

#include "BattleshipPlayerFromFile.h"
#define ROWS 10
#define COLS 10
#define NUM_OF_SHIPS 5
#define PLAYER_A_WON_STR "Player A won"
#define PLAYER_B_WON_STR "Player B won"
#define POINTS_STR "Points:"
#define PLAYER_A_POINTS_STR "Player A: "
#define PLAYER_B_POINTS_STR "Player B: "

class BattleshipGame
{
	static const int cols = COLS;
	static const int rows = ROWS;
	IBattleshipGameAlgo* PlayerA;
	IBattleshipGameAlgo* PlayerB;
	int numOfShipsA = NUM_OF_SHIPS;
	int numOfShipsB = NUM_OF_SHIPS;
	int scoreA = 0;
	int scoreB = 0;
	char** mainBoard;
	bool corrupted;

public:
	BattleshipGame(IBattleshipGameAlgo* playerA, IBattleshipGameAlgo* playerB, char** mainBoard, const std::string& path)
		: PlayerA(playerA), PlayerB(playerB), corrupted(false) {

		//copy mainBoard
		this->mainBoard = new char*[rows];
		for (int i = 0; i < rows; ++i)
		{
			this->mainBoard[i] = new char[cols];
			for (int j = 0; j < cols; ++j)
				this->mainBoard[i][j] = mainBoard[i][j];
		}
		initPlayerBoard(0);
		initPlayerBoard(1);
		
		//check if player init was successfull (without throwing exceptions from constructor)
		if (PlayerA->init(path) && PlayerB->init(path))
		{
			corrupted = true;
			delete[](mainBoard);
		}
	}

	BattleshipGame(const BattleshipGame&) = delete;

	BattleshipGame& operator=(const BattleshipGame&) = delete;

	~BattleshipGame() {
		delete[](mainBoard);
	}

	bool isCorrupted() const
	{
		return corrupted;
	}

	//Main loop of the gameplay
	void playGame();

private:
	// returns attack result of the attack in coor(i,j) and updates mainBoard using "updateBaordAndCheckSink"
	std::pair<AttackResult, bool> getAttackResult(int i, int j) const;

	//init player Board from the main board (erase other's player ships) and set player's board
	void initPlayerBoard(int playerNum) const;
	
	//called by getAttackResult. Checks if the attack resulted by a sink and updates the mainBoard according to attack result:
	//if it's a sink, erases the ship and returns true
	//if it isn't a sink, updates the (i,j)th coordinate as 'X' (for hit) and returns false
	bool updateBaordAndCheckSink(int i, int j) const;

	void BattleshipGame::getNextAttack(int& turnOf, bool& endGame, pair<int, int>& currAttack) const;
};

