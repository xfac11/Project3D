#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H
class KeyboardEvent
{
public:
	enum EventType
	{
		Press,
		Relsease,
		Invalid
	};
private:
	EventType type;
	unsigned char key;
public:

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	~KeyboardEvent();
	bool IsPress()const;
	bool IsRelease() const;
	bool isValid() const;
	unsigned char GetKeyCode()const;
};
#endif