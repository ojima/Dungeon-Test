#ifndef MENU_HPP
#define MENU_HPP

class Game;
class Bitmap;

class Menu {
	public:
		Menu();
		virtual ~Menu();
		
		virtual void Render(Game* game, Bitmap* target);
		virtual void Tick(Game* game, bool up, bool down, bool left, bool right, bool enter);
};

class TitleMenu : public Menu {
	public:
		TitleMenu();
		~TitleMenu();
		
		void Render(Game* game, Bitmap* target);
		void Tick(Game* game, bool up, bool down, bool left, bool right, bool enter);
		
	private:
		unsigned int selected, submenu;
};

#endif // MENU_HPP
