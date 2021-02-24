#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDl_ttf.h>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

const int SCREEN_WIDTH = 1280;							//window size
const int SCREEN_HEIGHT = 720;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Mix_Music* Music = nullptr;

vector<int> positionsX;									//vectords for the positions body snake
vector<int> positionsY;

int score = 0;
int new_score;
string record_score = "";

SDL_Texture* RenderText(const std::string& message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer)	//for render text
{
	SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		TTF_CloseFont(font);
		SDL_FreeSurface(surf);
	}

	SDL_FreeSurface(surf);
	return texture;
}   

int main(int argc, char* args[])					
{
	srand(time(NULL));			//random


	if (SDL_Init(SDL_INIT_VIDEO) < 0)				 //create window with title
	{
		return 1;
	}	  

	window = SDL_CreateWindow(("Snake 1.0.1  by MArchenko Yur4ik                    Score: " + to_string(score)).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); //title


	if (window == nullptr)				//create renderer
	{
		return 2;
	}				
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	if (renderer == nullptr)
	{
		return 3;
	}


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)		//mixer for music
	{
		return 4;
	}  


	if (TTF_Init() != 0)							//font
	{
		return 5;
	}

	SDL_Texture* image1 = nullptr;									//sprites for snalke and fruit	
	image1 = IMG_LoadTexture(renderer, "head_snake.png");

	SDL_Texture* image2 = nullptr;									
	image2 = IMG_LoadTexture(renderer, "body_snake.png");

	SDL_Texture* image3 = nullptr;										
	image3 = IMG_LoadTexture(renderer, "fruit.png");

	SDL_Texture* text = nullptr;								//texture for text
	SDL_Texture* text1 = nullptr;

	TTF_Font* font = TTF_OpenFont("calibri.ttf", 100);					//font
	if (font == nullptr)
	{
		return 4;
	}

	SDL_Rect position;											//position text

	position.x = SCREEN_WIDTH / 4 + 60;
	position.y = SCREEN_HEIGHT / 4;

	SDL_Rect position_record;											//position text

	position_record.x = SCREEN_WIDTH / 4 + 20;
	position_record.y = SCREEN_HEIGHT / 4 + 90;

	SDL_Rect snake;											//position of the snake's head
	snake.x = 0;
	snake.y = 0;
	snake.w = 30;
	snake.h = 30;

	vector<SDL_Rect> body;									//position of the snake's body			
	
	SDL_Rect fruit;											//position fruit
	fruit.x = rand() % 1250;
	fruit.y = rand() % 690;
	fruit.w = 30;
	fruit.h = 30;

	SDL_Event e;

	SDL_Keycode last = SDL_KeyCode::SDLK_RIGHT;				//the initial movement of the snake

	bool game_over = false;	
	bool move = false;

	string g_over = "GAME OVER";
	string new_record_game = "New record: ";

	Music = Mix_LoadMUS("eat.mp3");							//the sound of eating

	while (!game_over)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			break;
		}

		SDL_RenderClear(renderer);
							
		if (!move)
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)											//continuous sprite motion 
				{
				case SDL_KeyCode::SDLK_UP:
					if (last != SDL_KeyCode::SDLK_DOWN)
					{
						last = SDLK_UP;
					}
					break;
				case SDL_KeyCode::SDLK_DOWN:
					if (last != SDL_KeyCode::SDLK_UP)
					{
						last = SDLK_DOWN;
					}
					break;
				case SDL_KeyCode::SDLK_RIGHT:
					if (last != SDL_KeyCode::SDLK_LEFT)
					{
						last = SDLK_RIGHT;
					}
					break;
				case SDL_KeyCode::SDLK_LEFT:
					if (last != SDL_KeyCode::SDLK_RIGHT)
					{
						last = SDLK_LEFT;
					}
				}
			}

			for (int i = body.size() - 1; i > 0; i--)				//memorizing previous positions
			{
				body[i].y = body[i - 1].y;
				body[i].x = body[i - 1].x;
			}
			if (!body.empty())
			{
				body[0].y = snake.y;
				body[0].x = snake.x;
			}

			switch (last)											//change the direction of sprite motion
			{
			case SDL_KeyCode::SDLK_UP:
				snake.y -= 30;
				break;
			case SDL_KeyCode::SDLK_DOWN:
				snake.y += 30;
				break;
			case SDL_KeyCode::SDLK_LEFT:
				snake.x -= 30;
				break;
			case SDL_KeyCode::SDLK_RIGHT:
				snake.x += 30;
				break;
			case SDL_KeyCode::SDLK_SPACE:
				game_over = false;
			}
		}
		
		if (snake.x > SCREEN_WIDTH - snake.w || snake.x < 0 || snake.y > SCREEN_HEIGHT - snake.h || snake.y < 0)
		{
			move = true;

			ifstream record;							//read file with record
			record.open("score.txt");	
			if (record.is_open())									
			{
				while (getline(record, record_score))
				{
				
					istringstream iss(record_score,istringstream::in);

					iss >> new_score;
				}
			}
			record.close();

			if (new_score < score)								//memorize record
			{
				ofstream new_record;
				new_record.open("score.txt");

				if (new_record.is_open())
				{
					new_record << score;
					new_record_game = new_record_game + to_string(score);
				}
				record.close();		
				SDL_DestroyTexture(text1);
				text1 = RenderText(new_record_game, font, { 255,255,255 }, renderer);
				
			}
						
		}
		if (move)	
		{	
			SDL_DestroyTexture(text);

			text = RenderText(g_over, font, { 255,255,255 }, renderer);		
			SDL_QueryTexture(text1, NULL, NULL, &position_record.w, &position_record.h);
			SDL_QueryTexture(text, NULL, NULL, &position.w, &position.h);		
			SDL_RenderCopy(renderer, text, NULL, &position);
			SDL_RenderCopy(renderer, text1, NULL, &position_record);
			
		}

		if (snake.x > fruit.x - snake.w && snake.y > fruit.y - snake.h && snake.x < fruit.x + fruit.w && snake.y < fruit.y + fruit.h)
		{
			fruit.x = rand() % 1250;
			fruit.y = rand() % 690;
			body.push_back(SDL_Rect());

			body.back().x = -100;
			body.back().y = -100;
			body.back().w = 30;
			body.back().h = 30;

			score += 50;
		
			SDL_SetWindowTitle(window, ("Snake 1.0.1  by MArchenko Yur4ik                    Score : " + to_string(score)).c_str());
			Mix_PlayMusic(Music, 1);
		}	
		
		for (SDL_Rect i : body)															//if the game is over 
		{
			if (i.x == snake.x && i.y == snake.y)
			{
				game_over = true;

				SDL_DestroyTexture(text);
				SDL_DestroyTexture(text1);

				text = RenderText(g_over, font, { 255,255,255 }, renderer);
				text1 = RenderText(g_over, font, { 255,255,255 }, renderer);
				SDL_QueryTexture(text, NULL, NULL, &position.w, &position.h);
				SDL_QueryTexture(text1, NULL, NULL, &position_record.w, &position_record.h);
				SDL_RenderCopy(renderer, text, NULL, &position);
				SDL_RenderCopy(renderer, text1, NULL, &position_record);
			}
		}

		if (!game_over)
		{
			for (int i = 0; i < body.size(); i++)								//motion rendering 
			{
				SDL_RenderCopy(renderer, image2, NULL, &body[i]);
			}

			SDL_RenderCopy(renderer, image1, NULL, &snake);
			SDL_RenderCopy(renderer, image3, NULL, &fruit);
			SDL_RenderPresent(renderer);
			SDL_Delay(50);
		}
		
	}
	SDL_DestroyTexture(image1);			
	SDL_DestroyTexture(image2);	
	SDL_DestroyTexture(image3);
	SDL_DestroyTexture(text);
	SDL_DestroyTexture(text1);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Mix_Quit();
}