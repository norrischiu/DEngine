#pragma once
class HUDElement
{

public:
	enum TypeID {
		TEXTBOX,
		PROGRESSBAR
	};

	enum Color {
		RED, BLUE, GREEN
	};

	struct Position {
		float x;
		float y;

		Position() {}

		Position(const float x, const float y) {
			this->x = x;
			this->y = y;
		}
	};

	struct Size {
		int width;
		int height;

		Size() {}

		Size(const int width, const int height) {
			this->width = width;
			this->height = height;
		}
	};

	HUDElement(char* id, const HUDElement::Position pos);
	~HUDElement();

	virtual int getTypeID() = 0;
	bool hasUpdate();
	void setHasUpdate(const bool hasUpdate);
	HUDElement::Position getPosition();
	void setPosition(const HUDElement::Position pos);
	char* getID();

private:
	char* m_id;
	bool m_hasUpdate;
	HUDElement::Position m_pos;
};

