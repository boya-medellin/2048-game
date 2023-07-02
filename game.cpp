#include "game.h"

int main(){
	char input;

	init_board();
	next_turn();
	print_board();
	std::cin >> input;
	move(input);

	while (1){
		if ( !is_same() ){
			next_turn();
			print_board();
		}
		prev_board = board;
		std::cin >> input;
		move(input);
		if (game_over()) break;
	}

	std::cout << "Game Over\nTotal Score: " << score << '\n';

	return 1;
}
