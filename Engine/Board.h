#pragma once

#include "Graphics.h"
#include "Location.h"
#include <random>

class Board
{
public:
	enum class CellContents
	{
		Empty,
		Obstacle,
		Poison,
		Goal
	};
public:
	Board(std::mt19937 rng, const class Snake& snake, Graphics& gfx);
	void DrawCell( const Location& loc,Color c );
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard( const Location& loc ) const;
	bool CheckForObstacle(const Location& loc) const;
	bool CheckForPoison(const Location& loc) const;
	bool CheckForGoal(const Location& loc) const;
	void SpawnObstacle(std::mt19937 rng, const class Snake& snake);
	void InitGoal(std::mt19937 rng, const class Snake& snake);
	void SpawnGoal(std::mt19937 rng, const class Snake& snake);
	void SpawnPoison(std::mt19937 rng, const class Snake& snake);
	void Poisoned(const Location& loc);
	void GoalCollide(const Location& loc);
	void DrawBorder();
	void DrawObstacle();
	void DrawPoison();
	void DrawGoal();
private:
	static constexpr Color borderColor = Colors::Blue;
	static constexpr Color obstacleColor = Colors::Gray;
	static constexpr Color poisonColor = Colors::Magenta;
	static constexpr Color goalColor = Colors::Red;
	static constexpr int dimension = 20;
	static constexpr int cellPadding = 1;
	static constexpr int width = 32;
	static constexpr int height = 24;
	static constexpr int borderWidth = 4;
	static constexpr int borderPadding = 2;
	static constexpr int x = 70;
	static constexpr int y = 50;
	static constexpr int nGoal = 10;
	CellContents hasContent[width  * height] = { CellContents::Empty };
	Graphics& gfx;
};