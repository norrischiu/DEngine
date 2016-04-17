// GameLoop.h: the class of main game loop
#ifndef GAMELOOP_H
#define GAMELOOP_H

class GameLoop
{
public: 
	GameLoop();
	~GameLoop();

	void Construct();

	static GameLoop* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new GameLoop();
		return m_pInstance;
	}

	// Update the game with elipsed time
	void Update(float deltaTime);

private:

	// Empty constructor
	
	// Singleton instance
	static GameLoop* m_pInstance;
};

#endif GAMELOOP_H