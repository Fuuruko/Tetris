#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<random>
#include<string>
//#include<Windows.h>

using namespace sf;
using namespace std;

class Tetris
{
private:
	static const int n = 20, m = 10;
	int field[n][m] = {0};
	int square_in_line[20] = { 0 };
	int figures[7][4]{
		{4,5,6,7}, //square
		{1,3,5,7}, //line
		{2,4,5,6}, //triangle
		{2,4,5,7}, //zigzag1
		{3,4,5,6}, //zigzag2
		{2,4,6,7}, //chesshorse1
		{3,5,6,7} //chesshorse2
	};
	struct shape{ 
		int x, y;
	} figure[4],next_figure[4];
	int score;
	Texture rs,bs,borders; //red square and blue square
	Sprite RS, BS,BorderS;
	Text Score;
	Font font;
protected:
public:
	bool GameOver = 0;
	Tetris() {
		create_figure();
		create_figure();
		score = 0;
		rs.loadFromFile("red_square.png");
		bs.loadFromFile("blue_square.png");
		borders.loadFromFile("border.png");
		RS.setTexture(rs);
		BS.setTexture(bs);
		font.loadFromFile("arial.ttf");
		Score.setFont(font);
		Score.setString("Score : "+to_string(score));
		Score.setCharacterSize(24);
		Score.setFillColor(Color(255,0,0));
		Score.setPosition(440,220);
		BorderS.setTexture(borders);
	}
	void create_figure() {
		int x = rand() % 7;
		for (int i = 0; i < 4; ++i) {
			if (field[next_figure[i].y][next_figure[i].x] == 0) {
				figure[i].x = next_figure[i].x;
				figure[i].y = next_figure[i].y;
			}
			else {
				GameOver = true;
				for (int i = 0; i < 4; ++i) {
					figure[i].x = -1;
					figure[i].y = -1;
				}
			}

		}
		for (int i = 0; i < 4; ++i) {
			next_figure[i].x = 5 + figures[x][i] % 2;
			next_figure[i].y = figures[x][i] / 2;
		}
	}

	void move_right() {
		for (int i = 0; i < 4; ++i) {
			if (figure[i].x == 9 or field[figure[i].y][figure[i].x + 1] == 1)
				return;
		}
		for (int i = 0; i < 4; ++i)
			++figure[i].x;
	};
	void move_left() {
		for (int i = 0; i < 4; ++i) {
			if (figure[i].x == 0 or field[figure[i].y][figure[i].x - 1] == 1)
				return;
		}
		for (int i = 0; i < 4; ++i)
			--figure[i].x;

	};
	float move_down(float delay) {
		for (int i = 0; i < 4; ++i)
			++figure[i].y;
		for (int i = 0; i < 4; ++i)
			if (figure[i].y == 20 or field[figure[i].y][figure[i].x] == 1) {
				for (int j = 0; j < 4; ++j)
					--figure[j].y;
				static_figure();
				return 0.3;
			}
		return delay;
	};

	void right_turn() {
		for (int i = 0; i < 4; ++i) {
			int x = figure[1].x - (figure[i].y - figure[1].y);
			int y = figure[1].y + (figure[i].x - figure[1].x);
			figure[i].x = x;
			figure[i].y = y;
		}
		for (int i = 0; i < 4; ++i) {
			if (10 <= figure[i].x or figure[i].x < 0 or 
				20 <= figure[i].y or figure[i].y < 0 or
				field[figure[i].y][figure[i].x] == 1) {
				left_turn();
				break;
			}
		}
	};
	void left_turn() {
		for (int i = 0; i < 4; ++i) {
			int x = figure[1].x + (figure[i].y - figure[1].y);
			int y = figure[1].y - (figure[i].x - figure[1].x);
			figure[i].x = x;
			figure[i].y = y;
		}
		for (int i = 0; i < 4; ++i) {
			if (10 <= figure[i].x or figure[i].x < 0 or 
				20 <= figure[i].y or figure[i].y < 0 or
				field[figure[i].y][figure[i].x] == 1) {
				right_turn();
				break;
			}
		}
	};

	void static_figure() {
		for (int i = 0; i < 4; ++i) {
			field[figure[i].y][figure[i].x] = 1;
			++square_in_line[figure[i].y];
		}
		del_lines();
		create_figure();
	};
	void del_lines() {
		int full_line[4] = { -1,-1,-1,-1 };
		
		for (int i = 0; i < 4; ++i) {
			if (square_in_line[figure[i].y] == 10) {
				full_line[i] = figure[i].y;
				for (int j = 0; j < i; ++j) {
					if (full_line[i] == full_line[j])
						full_line[i] = -1;
				}
			}
		}
		//bubble sort 
		for (int i = 1; i < 4; ++i)
			for (int j = 0; j < i; ++j)
				if (full_line[i] < full_line[j])
					swap(full_line[i], full_line[j]);

		int del_lines = 0;
		for (int i = 0; i < 4; ++i) {
			if (full_line[i] != -1) {
				++del_lines;
				for (int k = full_line[i]; k > 0; --k) {
					for (int j = 0; j < 10; ++j)
						field[k][j] = field[k - 1][j];
					square_in_line[k] = square_in_line[k - 1];
				}
			}
		}

		for (int i = 0; i < del_lines; ++i) {
			score += (i + 1) * 100;
			for (int j = 0; j < 10; ++j)
				field[i][j] = 0;
			square_in_line[i] = 0;
		}
	};

	void show_figure(RenderWindow* window) {
		for (int i = 0; i < 4; ++i) {
			RS.setPosition((figure[i].x+1) * 36, (figure[i].y+1) * 36);
			// Отрисовка спрайта
			window->draw(RS, RenderStates());
		}
	}
	void fill_field(RenderWindow* window) {
		for (int i = 0; i < 20; ++i)
			for (int j = 0; j < 10; ++j)
				if (field[i][j] == 1) {
					BS.setPosition((j+1) * 36, (i+1) * 36);
					// Отрисовка спрайта
					window->draw(BS, RenderStates());
				}
	}
	void fill_border(RenderWindow* window) {
		for (int i = 0; i <= 21; ++i) {
			BorderS.setPosition(0, i * 36);
			window->draw(BorderS, RenderStates());
			BorderS.setPosition(396, i * 36);
			window->draw(BorderS, RenderStates());
		}
		for (int i = 1; i <= 10; ++i) {
			BorderS.setPosition(i * 36, 0);
			// Отрисовка спрайта
			window->draw(BorderS, RenderStates());
			BorderS.setPosition(i * 36, 756);
			// Отрисовка спрайта
			window->draw(BorderS, RenderStates());
		}
	}
	void next_figures(RenderWindow* window) {
		// y-acis
		for (int i = 0; i < 6; ++i) {
			BorderS.setPosition(576, i * 36);
			window->draw(BorderS, RenderStates());
		}
		// x-acis
		for (int i = 12; i < 16; ++i) {
			BorderS.setPosition(i * 36, 0);
			// Отрисовка спрайта
			window->draw(BorderS, RenderStates());
			BorderS.setPosition(i * 36, 180);
			// Отрисовка спрайта
			window->draw(BorderS, RenderStates());
		}
		for (int i = 0; i < 4; ++i) {
			BS.setPosition((next_figure[i].x + 8) * 36, (next_figure[i].y + 1) * 36);
			// Отрисовка спрайта
			window->draw(BS, RenderStates());
		}
	}
	void show_score(RenderWindow* window) {
		Score.setString("Score : " + to_string(score));
		window->draw(Score, RenderStates());
	}
};