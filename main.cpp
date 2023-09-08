#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

SDL_Window *win = SDL_CreateWindow("PONG | Abthahi & Programming", 0, 0, 1024, 648, 0);
SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

class Particle{
	
public:
	int x, y;
	int a, s;
	Particle(int x, int y){
		
		this->x = x;
		this->y = y;
		
		this->a = 255;
		this->s = 20;
	}
	
	void update(){
		
		a-=10;
		
	}
	
	bool isGone(){
		return a < 0;
	}
	
	void draw(){
		SDL_Rect rect = {x, y, s, s};
		SDL_SetRenderDrawColor(ren, 255, 134, 0, a);
		SDL_RenderFillRect(ren, &rect);
	}
	
};


class Ball{
public:
	int x, y, w, h;
	int vx, vy, t;
	vector<Particle> fire;
	Ball(int x, int y){
		this->x = x;
		this->y = y;
		
		w = 20;
		h = 20;
		
		vx = -3;
		vy = 3;
		
		t = 0;
	}
	void reset(){
		x = 512;
		y = 300;
	}
	
	void update(){
		
		x += vx;
		y += vy;
		
//		if (x < 0 || x + 20 > 1024) vx *= -1;
		if (y < 0 || y + h > 648) vy *= -1;
		
		if (t > 2){
			fire.push_back(Particle(x, y));
			t = 0;
		}
		
		t++;
		for (unsigned i = 0; i < fire.size(); i++){
			fire[i].update();
			if (fire[i].isGone()){
				fire.erase(fire.begin() + i);
			}
		}
		
	}
	
	
	void draw(){
		for (unsigned i = 0; i < fire.size(); i++){
			fire[i].draw();
		}
		SDL_Rect rect = {x, y, w, h};
		SDL_SetRenderDrawColor(ren, 174, 184, 254, 255);
		SDL_RenderFillRect(ren, &rect);
	}
	
};


class Player{
	
public:
	int x, y, w, h;
	int score;
	
	Player(int x, int y){
		
		this->x = x;
		this->y = y;
		this->score = 0;
		
		
		w = 10;
		h = 100;
	}
	

	
	void update(){
		if (y < 0) y = 0;
		if (y + h > 648) y = 548;
	}
	
	void up(){
		y -= 50;
	}
	
	void down(){
		y += 50;
	}
	
	void collide(Ball& ball, int side){ // side = 1 for right and side = 0 for left		
		if (ball.y >= y && ball.y <= y + h && ball.y + ball.w >= y && ball.y + ball.w <= y + h){
			if (side == 0){
				if (ball.x <= x + w){
					ball.x = x + w;
					ball.vx *= -1;
				}
			}else{
				if (ball.x + ball.w > x){
					ball.x = x - ball.w;
					ball.vx *= -1;
				}					
			}
		}
	}
	
	
	void draw(){
		
		SDL_Rect rect = {x, y, w, h};
		SDL_SetRenderDrawColor(ren, 255, 195, 0, 255);
		SDL_RenderFillRect(ren, &rect);
	}
};



void drawText(string text, int x, int y, TTF_Font *font){
	
	SDL_Color color = {255, 255, 255};
	
	SDL_Surface *tmp = TTF_RenderText_Blended(font, text.c_str(), color);
	
	SDL_Rect geo = {x, y, tmp->w, tmp->h};
	
	SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, tmp);
	
	SDL_FreeSurface(tmp);
	
	SDL_RenderCopy(ren, texture, NULL, &geo);
	
}


int main(int argc, char* argv[]){
	
	
	// Enable Alpha
	
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	
	TTF_Init();
	
	TTF_Font *poppins = TTF_OpenFont("fonts/poppins.ttf", 64);
	
	if (poppins){
		cout<<"Font Loaded Successfully"<<endl;
	}else{
		cout<<"Error on loading Fonts. Error : "<<TTF_GetError()<<endl;
	}
	
	
	bool isRunning = true;
	

	Player p1 = Player(0, 200);
	Player p2 = Player(1014, 200);
	
	
	Ball ball = Ball(512, 300);
	

	while(isRunning){
		
		SDL_SetRenderDrawColor(ren, 0, 29, 61, 255);
		SDL_RenderClear(ren);
		
		
		
		
		// Game Logic
		if (ball.x > 1024){
			p1.score++;
			ball.reset();
		}
		if (ball.x + 10 < 0){
			p2.score++;
			ball.reset();
		}
		
		p1.collide(ball, 0);
		p2.collide(ball, 1);
		
		if(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				isRunning = false;
			}
			
			if (event.type == SDL_KEYDOWN){
				
				if (event.key.keysym.sym == SDLK_UP){
					p2.up();
				}
				else if (event.key.keysym.sym == SDLK_DOWN){
					p2.down();
				}
				
				if (event.key.keysym.sym == SDLK_w){
					p1.up();
				}
				else if (event.key.keysym.sym == SDLK_s){
					p1.down();
				}
			}
		}

		
		p1.update();
		p2.update();
		ball.update();
		
		SDL_Rect divide = {510, 0, 5, 648};
		SDL_SetRenderDrawColor(ren, 241, 242, 246, 255);
		SDL_RenderFillRect(ren, &divide);
		
		drawText(to_string(p1.score), 250, 270, poppins);
		drawText(to_string(p2.score), 740, 270, poppins);
		
		p1.draw();
		p2.draw();
		ball.draw();
		
		SDL_RenderPresent(ren);
		
		SDL_Delay(1000 / 70);
	}

	return 0;
}