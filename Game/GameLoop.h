// GameLoop.h: the class of main game loop

#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <windows.h>

class GameLoop
{
public: 

	// Default constructor
	GameLoop();

	static GameLoop* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new GameLoop();
		return m_pInstance;
	}

	// Update the game with elipsed time
	void Update(float deltaTime);

private:

	// Singleton instance
	static GameLoop* m_pInstance;
};

#endif GAMELOOP_H