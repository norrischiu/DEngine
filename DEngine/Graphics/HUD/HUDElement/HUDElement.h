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

	enum FontSize {
		PT12 = 12, PT24 = 24, PT36 = 36, PT48 = 48, PT60 = 60, PT72 = 72, PT84 = 84, PT96 = 96, PT108 = 108, PT120 = 120, PT132 = 132, PT144 = 144, PT156 = 156, PT168 = 168, PT180 = 180, PT192 = 192, PT204 = 204
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

	HUDElement(char* id, const HUDElement::Position pos, const float duration, const bool visibility = true);
	~HUDElement();

	virtual int getTypeID() = 0;
	bool hasUpdate();
	void setHasUpdate(const bool hasUpdate);
	HUDElement::Position getPosition();
	void setPosition(const HUDElement::Position pos);
	float getDuration();
	void setDuration(const float duration);
	bool isVisible();
	void setVisibility(const bool visibility);
	char* getID();

private:
	char* m_id;
	float m_duration;
	HUDElement::Position m_pos;
	bool m_hasUpdate;
	bool m_visibility;
};

