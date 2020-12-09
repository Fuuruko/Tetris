#include<iostream>
#include<SFML/Graphics.hpp>
#include"Tetris.h"
#include<ctime>
#include<conio.h> //используем _getch(),чтобы не использовать enter для ввода
#include<Windows.h>

using namespace sf;
using namespace std;

int main() {
	setlocale(LC_ALL, "Ru");
	RenderWindow window(VideoMode(612, 792), "Tetris");
	Tetris T;
	float timer = 0, delay = 0.35;
	Clock clock;
	while (window.isOpen())
	{
		if (!T.GameOver) {
			// Получаем время, прошедшее с начала отсчета, и конвертируем его в секунды
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;
			// Обрабатываем очередь событий в цикле
			Event event;
			while (window.pollEvent(event))
			{
				// закрыть окно
				if (event.type == Event::Closed)
					window.close();

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::Left or event.key.code == Keyboard::A)
						T.move_left();
					else if (event.key.code == Keyboard::Right or event.key.code == Keyboard::D)
						T.move_right();
					else if (event.key.code == Keyboard::Down or event.key.code == Keyboard::S)
						delay = 0.05;
					else if (event.key.code == Keyboard::Up or event.key.code == Keyboard::E)
						T.right_turn();
					else if (event.key.code == Keyboard::Q)
						T.left_turn();
				}
			}
			if (timer > delay)
			{
				delay = T.move_down(delay);
				timer = 0;

			}
			//цвет фона rgb
			window.clear(Color(255, 255, 255));
		}
		else {
			Event event;
			while (window.pollEvent(event))
				// закрыть окно
				if (event.type == Event::Closed or event.key.code == Keyboard::Escape)
					window.close();
			window.clear(Color(255, 255, 255));
			Font font;
			Text Score;
			font.loadFromFile("arial.ttf");
			Score.setFont(font);
			Score.setString("GameOver");
			Score.setCharacterSize(24);
			Score.setFillColor(Color(255, 0, 0));
			Score.setPosition(440, 240);
			window.draw(Score, RenderStates());
		}
		T.show_figure(&window);
		T.fill_field(&window);
		T.fill_border(&window);
		T.next_figures(&window);
		T.show_score(&window);
		// Отрисовка окна	
		window.display();
	}
}
