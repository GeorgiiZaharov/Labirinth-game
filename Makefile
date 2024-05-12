all:
	g++ -Wall -Wextra -Werror *.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# 	g++ *.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

test:
	echo sometext
