#include "..\Debug\Debug.h"
#include "..\Font\Font.h"
#include "..\Object\ObjectLoader.h"
#include "..\Game\String.h"

class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void CreateGameContent();
	void Update(const float elaspedTime);

private:
	Debug m_debug;
	Font m_font;
	ObjectLoader m_objectLoader;
};
