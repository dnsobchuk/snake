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
	brd(rng,snek,gfx)
{
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
				if( !brd.IsInsideBoard( next ) ||
					snek.IsInTileExceptEnd( next ) ||
					brd.CheckForObstacle(next))
				{
					gameIsOver = true;
					sndFart.Play();
					sndMusic.StopAll();
				}
				else
				{
					if( brd.CheckForGoal(next) )
					{
						snek.GrowAndMoveBy( delta_loc );
						brd.GoalCollide(next);
						brd.SpawnGoal(rng,snek);
						brd.SpawnObstacle(rng, snek);
						sfxEat.Play( rng,0.8f );
					}
					else
					{
						if (brd.CheckForPoison(next)) 
						{
							sfxEat.Play(rng, 0.8f);
							brd.Poisoned(next);
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
		brd.DrawGoal();
		brd.DrawPoison();
		brd.DrawBorder();
		brd.DrawObstacle();
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
