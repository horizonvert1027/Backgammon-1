#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utils.h"
#include "Game.h"

#pragma region TestsHelpers

int _failedAsserts = 0;

void Assert(int result, char* msg) {
	if (result == 0)
	{
		printf("\n");
		PrintRed(msg);
		printf("\n");
		_failedAsserts++;
	}
}

void AssertNot(int result, char* msg) {
	if (result != 0)
	{
		printf("\n");
		PrintRed(msg);
		printf("\n");
		_failedAsserts++;
	}
}

void AssertAreEqual(char* s1, char* s2, char* msg) {
	if (strcmp(s1, s2))
	{
		PrintRed(msg);
		printf("\n");
		printf("Expected: %s\n", s1);
		printf("Actual:   %s\n", s2);
		_failedAsserts++;
	}
}

void AssertAreEqualInts(int expected, int actual, char* msg) {
	if (expected != actual)
	{
		printf("\n");
		PrintRed(msg);
		printf("\n");
		char str[24];
		snprintf(str, 24, "Expected %d", expected);
		PrintRed(str);
		printf("\n");
		snprintf(str, 24, "Actual   %d", actual);
		PrintRed(str);
		_failedAsserts++;
	}
}

void AssertAreEqualLongs(U64 expected, U64 actual, char* msg) {
	if (expected != actual)
	{
		printf("\n");
		PrintRed(msg);
		printf("\n");
		char str[24];
		snprintf(str, 24, "Expected %llu", expected);
		PrintRed(str);
		printf("\n");
		snprintf(str, 24, "Actual   %llu", actual);
		PrintRed(str);
		_failedAsserts++;
	}
}

#pragma endregion

void TestStartPos() {
	StartPosition();
	Assert(G.Dice[0] == 0, "Dice 0 is not reset");
	Assert(G.Dice[1] == 0, "Dice 1 is not reset");
}

void TestRollDice() {
	for (size_t i = 0; i < 10; i++)
	{
		RollDice();
		Assert(G.Dice[0] >= 1 && G.Dice[0] <= 6, "Invalid Dice value");
		Assert(G.Dice[1] >= 1 && G.Dice[1] <= 6, "Invalid Dice value");
	}
	printf("\n");

}

void TestWriteGameString() {
	StartPosition();
	char s[100];
	WriteGameString(s);
	
	char* expected = "0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0";
	AssertAreEqual(expected, s, "Unexpected GameString");
}

void TestReadGameString() {
	
	char* gameString = "0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0";
	ReadGameString(gameString);
	AssertAreEqualInts(G.Position[0], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[1], 2 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[2], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[3], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[4], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[5], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[6], 5 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[7], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[8], 3 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[9], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[10], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[11], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[12], 5 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[13], 5 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[14], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[15], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[16], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[17], 3 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[18], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[19], 5 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[20], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[21], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[22], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[23], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[24], 2 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[25], 0, "Invalid checker count");
	AssertAreEqualInts(G.BlackHome, 0, "Invalid black home count");
	AssertAreEqualInts(G.WhiteHome, 0, "Invalid white home count");
}

void TestGameStringRountTrip() {

	char* gameString = "b2 b1 w3 0 0 0 w2 w1 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 w2 3 4";
	ReadGameString(gameString);
	AssertAreEqualInts(G.Position[0], 2 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[1], 1 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[2], 3 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[3], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[4], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[5], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[6], 2 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[7], 1 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[8], 3 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[9], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[10], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[11], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[12], 5 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[13], 5 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[14], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[15], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[16], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[17], 3 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[18], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[19], 5 | Black, "Invalid checker count");
	AssertAreEqualInts(G.Position[20], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[21], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[22], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[23], 0, "Invalid checker count");
	AssertAreEqualInts(G.Position[24], 2 | White, "Invalid checker count");
	AssertAreEqualInts(G.Position[25], 2 | White, "Invalid checker count");
	AssertAreEqualInts(G.WhiteHome, 3, "Invalid black home count");
	AssertAreEqualInts(G.BlackHome, 4, "Invalid white home count");

	char written[100];
	WriteGameString(written);
	AssertAreEqual(gameString, written, "Read and written string not same");
}

void TestOtherColor() {
	AssertAreEqualInts(Black, OtherColor(White), "Other color should be Black");
	AssertAreEqualInts(White, OtherColor(Black), "Other color should be White");
}


void TestDoUndo() {
	StartPosition();
	Move move;
	move.from = 1;
	move.to = 2;
	move.color = Black;
	bool hit = DoMove(&move);
	AssertAreEqualInts(1 | Black, G.Position[1], "Should be one black checker on 1");
	AssertAreEqualInts(1 | Black, G.Position[2], "Should be one black checker on 2");
	UndoMove(&move, hit);
	AssertAreEqualInts(2 | Black, G.Position[1], "Should be 2 black checkers on 1");
	AssertAreEqualInts(0, G.Position[2], "Should be no checker on 2");
}

void TestDoUndoHomeBlack() {
	ReadGameString("0 0 0 0 0 0 w2 w1 w3 0 0 0 0 w5 0 0 0 0 0 b5 0 0 0 0 w2 w2 0 0");
	
	AssertAreEqualInts(5 | Black, G.Position[19], "Should be 5 black checkers on 19");
	AssertAreEqualInts(0, G.BlackHome, "Should be no black checker on Black Home");

	Move move;
	move.from = 19;
	move.to = 25;
	move.color = Black;
	bool hit = DoMove(&move);

	AssertAreEqualInts(4 | Black, G.Position[19], "Should be 4 black checkers on 19");
	AssertAreEqualInts(1 , G.BlackHome, "Should be one black checker on Black Home");
	UndoMove(&move, hit);
	AssertAreEqualInts(5 | Black, G.Position[19], "Should be 5 black checkers on 19");
	AssertAreEqualInts(0, G.BlackHome, "Should be no black checker on Black Home");
}

void TestDoUndoHomeWhite() {
	ReadGameString("0 w1 0 0 w2 0 0 0 0 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0");

	AssertAreEqualInts(2 | White, G.Position[4], "Should be 2 white checkers on 4");
	AssertAreEqualInts(0, G.WhiteHome, "Should be no white checker on White Home");

	Move move;
	move.from = 4;
	move.to = 0;
	move.color = White;
	bool hit = DoMove(&move);

	AssertAreEqualInts(1 | White, G.Position[4], "Should be 1 white checkers on 4");
	AssertAreEqualInts(1, G.WhiteHome, "Should be one white checker on White Home");
	UndoMove(&move, hit);
	AssertAreEqualInts(2 | White, G.Position[4], "Should be 2 white checkers on 4");
	AssertAreEqualInts(0, G.WhiteHome, "Should be no white checker on White Home");
}

void TestIsBlocked() {
	StartPosition();
	Assert(IsBlockedFor(1, White), "Pos 1 should be blocked for white");
	AssertNot(IsBlockedFor(1, Black), "Pos 1 should not be blocked for black");

	AssertNot(IsBlockedFor(2, White), "Pos 1 should not be blocked for white");
	AssertNot(IsBlockedFor(2, Black), "Pos 1 should not be blocked for black");
}

void PrintMoves() {
	for (size_t i = 0; i < G.MoveSetsCount; i++)
	{
		for (size_t j = 0; j < G.SetLengths[i]; j++)
			printf("%d-%d, ", G.PossibleMoveSets[i][j].from, G.PossibleMoveSets[i][j].to);
		printf("(%d)\n", G.SetLengths[i]);
	}
}

void TestSimpleBlack() {
	char* gameString = "0 b2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 1;
	G.Dice[1] = 2;
	G.CurrentPlayer = Black;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(2, G.MoveSetsCount, "There should be 2 sets of moves.");
}

void TestCreateMovesBlackStart() {
	StartPosition();
	G.CurrentPlayer = Black;
	G.Dice[0] = 3;
	G.Dice[1] = 4;
	CreateMoves();
	char gs[100];
	WriteGameString(gs);
	AssertAreEqual("0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0", gs, "Game string should be start string.");
	AssertAreEqualInts(17, G.MoveSetsCount, "There should be 20 sets of moves.");
	//PrintMoves();

	// TODO: Assert moves
}

void TestCreateMovesWhiteStart() {
	StartPosition();
	G.CurrentPlayer = White;
	G.Dice[0] = 3;
	G.Dice[1] = 4;
	CreateMoves();
	char gs[100];
	WriteGameString(gs);
	AssertAreEqual("0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0", gs, "Game string should be start string.");
	AssertAreEqualInts(17, G.MoveSetsCount, "There should be 20 sets of moves.");
}

void TestBlackCheckerOnBar() {
	char* gameString = "b1 b1 0 0 0 0 w5 0 w1 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 2;
	G.Dice[1] = 6;
	G.CurrentPlayer = Black;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(4, G.MoveSetsCount, "There should be 4 sets of moves.");
}

void TestWhiteCheckerOnBar() {
	char* gameString = "b1 b1 0 0 0 0 w5 0 w1 0 0 0 b5 w5 0 0 0 b1 0 b5 0 0 0 0 w1 w1 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 2;
	G.Dice[1] = 6;
	G.CurrentPlayer = White;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(4, G.MoveSetsCount, "There should be 4 sets of moves.");
}

void TestBearingOffBlack() {
	char* gameString = "0 0 0 0 0 0 w5 0 w1 0 0 0 0 w5 0 0 0 0 0 b5 b2 b2 0 b2 b2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 2;
	G.Dice[1] = 4;
	G.CurrentPlayer = Black;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(10, G.MoveSetsCount, "There should be 10 sets of moves.");
}

void TestBearingOffWhite() {
	char* gameString = "0 w2 w2 0 w2 w2 w5 0 0 0 0 0 0 0 0 0 0 0 0 b5 b2 b2 0 b2 b2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 2;
	G.Dice[1] = 4;
	G.CurrentPlayer = White;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(10, G.MoveSetsCount, "There should be 10 sets of moves.");
}

void TestDoubleDiceBlack() {
	char* gameString = "0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 4;
	G.Dice[1] = 4;
	G.CurrentPlayer = Black;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(284, G.MoveSetsCount, "There should be 248 sets of moves.");
}

void TestDoubleDiceWhite() {
	char* gameString = "0 b2 0 0 0 0 w5 0 w3 0 0 0 b5 w5 0 0 0 b3 0 b5 0 0 0 0 w2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 4;
	G.Dice[1] = 4;
	G.CurrentPlayer = White;
	CreateMoves();
	//PrintMoves();
	AssertAreEqualInts(284, G.MoveSetsCount, "There should be 248 sets of moves.");
}

void PlayBothDiceIfPossible() {
	char* gameString = "0 0 b2 b2 0 0 w5 w3 0 b2 0 0 0 w5 0 0 0 0 0 b3 b2 0 b2 b2 w2 0 0 0";
	ReadGameString(gameString);
	G.CurrentPlayer = White;
	G.Dice[0] = 4;
	G.Dice[1] = 6;
	CreateMoves();
	AssertAreEqualInts(1, G.MoveSetsCount, "There should be 1 set of moves.");
	//PrintMoves();
}

void TestRemoveShorterSets()
{
	char* gameString = "0 0 0 0 0 0 w5 0 w1 0 0 0 0 w5 0 0 0 0 0 b5 b2 b2 0 b2 b2 0 0 0";
	ReadGameString(gameString);
	G.Dice[0] = 2;
	G.Dice[1] = 4;
	G.CurrentPlayer = Black;
	CreateMoves();
	AssertAreEqualInts(10, G.MoveSetsCount, "There should be 10 moves");

	//PrintMoves();
	G.SetLengths[2] = 1;
	G.SetLengths[5] = 1;
	RemoveShorterSets(2);
	AssertAreEqualInts(8, G.MoveSetsCount, "There should be 8 moves left");
	/*ConsoleWriteLine("==================");
	PrintMoves();*/
}

void RunAll() {
	TestStartPos();
	TestRollDice();
	TestWriteGameString();
	TestReadGameString();
	TestGameStringRountTrip();
	TestDoUndo();
	TestIsBlocked();
	TestSimpleBlack();
	TestDoUndoHomeBlack();
	TestDoUndoHomeWhite();
	TestCreateMovesBlackStart();
	TestCreateMovesWhiteStart();
	TestOtherColor();
	TestBlackCheckerOnBar();
	TestWhiteCheckerOnBar();
	TestBearingOffBlack();
	TestBearingOffWhite();
	TestDoubleDiceBlack();
	TestDoubleDiceWhite();
	PlayBothDiceIfPossible();
	TestRemoveShorterSets();

	if (_failedAsserts == 0)
		PrintGreen("Success! Tests are good!\n");
	else
		PrintRed("There are failed tests.\n");
}