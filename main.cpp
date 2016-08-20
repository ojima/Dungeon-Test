#include "include.hpp"

#define WIDTH 320
#define HEIGHT 240
#define SCALE 2
#define TICKS 60

const double TimePerTick = 1.0 / TICKS;
Game* game;
bool running;
SDL_Window* window;
SDL_Surface *surface, *buffer;
Display* display;

void Tick() {
	static const unsigned char* keys = SDL_GetKeyboardState(nullptr);
	static bool screenshot = false;
	
	if (keys[SDL_SCANCODE_F10]) {
		static int fileid = 0;
		while (!screenshot) {
			std::string filename = "./screenshots/screenshot" + std::to_string(fileid) + ".png";
			
			if (!FileExists(filename)) {
				if (IMG_SavePNG(surface, filename.c_str()) == -1)
					std::cout << "Failed to save screenshot: " << SDL_GetError() << std::endl;
				else
					std::cout << "Saved a screenshot to " << filename << std::endl;
				break;
			}
			
			fileid++;
		}
		
		screenshot = true;
	} else {
		screenshot = false;
	}
	
	game->Tick(keys);
}

void Render() {
	display->Render(game);
	
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			unsigned int a = (unsigned int) 0xff << 24;
			unsigned int col = display->pixels[i + j * WIDTH];
			
			if (game->retromode) col = ColorTo8Bit(col);
			
			((unsigned int*) buffer->pixels)[i + j * WIDTH] = a | col;
		}
	}
	
	SDL_BlitScaled(buffer, nullptr, surface, nullptr);
	SDL_UpdateWindowSurface(window);
}

int main(int argc, char* argv[]) {
	// Initial initializations
	std::srand(std::time(nullptr));
	
	// SDL initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	// Display initialization
	window = SDL_CreateWindow("The Legends of the Arcahill",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH * SCALE, HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	
	surface = SDL_GetWindowSurface(window);
	buffer = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	display = new Display(WIDTH, HEIGHT);
	
	LoadTextures();
	
	// Other initializations
	game = new Game();
	game->menu = new TitleMenu();
	
	int ticks = 0;
	int frames = 0;
	int lastTime = 0;
	double silentTime = 0.0;
	running = true;
	
	// Primary game loop heartbeat
	while (running) {
		int nowTime = SDL_GetTicks();
		int deltaTime = nowTime - lastTime;
		lastTime = nowTime;
		
		if (deltaTime < 0) deltaTime = 0;
		if (deltaTime > 10000) deltaTime = 10000;
		
		silentTime += deltaTime / 1000.0;
		
		bool ticked = false;
		while (silentTime > TimePerTick) {
			silentTime -= TimePerTick;
			// Game ticks here
			Tick();
			ticks++;
			ticked = true;
			
			if (ticks % TICKS == 0) {
				std::cout << frames << " fps" << std::endl;
				frames = 0;
			}
		}
		
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		
		if (ticked) {
			// Game renders if ticked during one heartbeat
			Render();
			frames++;
		} else {
			SDL_Delay(1);
		}
	}
	
	// SDL quit
	SDL_DestroyWindow(window);
	SDL_Quit();
}
