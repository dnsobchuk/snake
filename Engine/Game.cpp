/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng( std::random_device()() ),
	snek( {2,2} ),
	brd(gfx)
{
	for (int i = 0; i < nGoal; i++) 
	{
		brd.SpawnContent(rng, snek, Board::CellContents::Goal);
	}
	for (int i = 0; i < nPoison; i++)
	{
		brd.SpawnContent(rng, snek, Board::CellContents::Poison);
	}
	sndTitle.Play( 1.0f,1.0f );
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();

	if( gameIsStarted )
	{
		if( !gameIsOver )
		{
			if( wnd.kbd.KeyIsPressed( VK_UP ) )
			{
				delta_loc = { 0,-1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
			{
				delta_loc = { 0,1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
			{
				delta_loc = { -1,0 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
			{
				delta_loc = { 1,0 };
			}
			float snekModifiedMovePeriod = snekMovePeriod;
			if (wnd.kbd.KeyIsPressed(VK_CONTROL)) {
				snekModifiedMovePeriod = std::min(snekMovePeriod, snekMovePeriodSpeedup);
			}

			snekMoveCounter += dt;
			if( snekMoveCounter >= snekModifiedMovePeriod)
			{
				snekMoveCounter -= snekModifiedMovePeriod;
				const Location next = snek.GetNextHeadLocation( delta_loc );
				
				const Board::CellContents content = brd.IsInsideBoard(next) ? brd.GetContent(next)
					: Board::CellContents::Empty;

				if( !brd.IsInsideBoard( next ) ||
					snek.IsInTileExceptEnd( next ) ||
					content == Board::CellContents::Obstacle)
				{
					gameIsOver = true;
					sndFart.Play();
					sndMusic.StopAll();
				}
				else
				{
					if( content == Board::CellContents::Goal )
					{
						snek.GrowAndMoveBy( delta_loc );
						brd.ConsumeContent(next);
						brd.SpawnContent(rng,snek,Board::CellContents::Goal);
						brd.SpawnContent(rng, snek, Board::CellContents::Obstacle);
						sfxEat.Play( rng,0.8f );
					}
					else
					{
						if ( content == Board::CellContents::Poison)
						{
							sfxEat.Play(rng, 0.8f);
							brd.ConsumeContent(next);
							snekMovePeriod = std::max(snekMovePeriod * snekSpeedupFactor, snekMovePeriodMin);
						}
						snek.MoveBy( delta_loc );
					}
					sfxSlither.Play( rng,0.08f );
				}
			}
		}
	}
	else
	{
		if( wnd.kbd.KeyIsPressed( VK_RETURN ) )
		{
			sndMusic.Play( 1.0f,0.6f );
			gameIsStarted = true;
		}
	}
}

void Game::ComposeFrame()
{
	if( gameIsStarted )
	{
		snek.Draw( brd );
		brd.DrawBorder();
		brd.DrawCells();
		if (gameIsOver)
		{
			SpriteCodex::DrawGameOver(350, 265, gfx);
		}
	}
	else
	{
		SpriteCodex::DrawTitle( 290,225,gfx );
	}
}
