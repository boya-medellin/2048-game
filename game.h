/* TODO:
 *  - seg fault sometimes when moving right in late game 			X
 *  - randomize input (2 or 4) 										X
 *  - logging
 *  - game over 											X
 *  - continuous directional input (arrow keys)
 *  - ncurses graphics
 */

#include <cstddef>
#include <curses.h>
#include <iostream>
#include <iterator>
#include <array>
#include <vector>
#include <random>
#include <ncurses.h>

#define SIZE 4

std::array<std::array<long int, SIZE>, SIZE> board;
std::array<std::array<long int, SIZE>, SIZE> prev_board;
int score = 0;

/* debug */
std::vector<int> numbers_generated;
/* debug */


void init_board(){
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			board[i][j] = 0;
		}
	}
}


void print_board(){
	for (auto i = board.begin(); i != board.end(); ++i){
		for (auto j = i->begin(); j != i->end(); ++j){
			if ( *j == 0){
				std::cout << "  *  ";
			} else {
				std::cout << "  " << *j << "  ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "Score: " << score << '\n';
	std::cout << "\n";
}


int board_full(){
	int count = 0;
	for (auto i = board.begin(); i != board.end(); ++i){
		for (auto j = i->begin(); j != i->end(); ++j){
			if (i != 0)
				++count;
		}
	}

	if (count == 16){
		return 1;
	} else {
		return 0;
	}
}


std::vector<long int *> empty_cells(){
	std::vector<long int *> empty;
	for (auto i = board.begin(); i != board.end(); ++i){
		for (auto j = i->begin(); j != i->end(); ++j){
			if ( *j == 0)
				empty.push_back(j);
		}
	}
	return empty;
}


void next_turn(){
	int size, random_cell, random_input;
	std::array<int, 10> numbers{2, 2, 2, 2, 2, 2, 2, 2, 4, 4};

	//find all the empty space on the board 
	std::vector<long int*> empty = empty_cells();
	size = empty.size() - 1;

	// gen random cell
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr_cell(0, size);
	random_cell = distr_cell(gen);

	// gen random input
	std::uniform_int_distribution<> distr_input(0, 9);
	random_input = numbers[distr_input(gen)];

	// place random cell
	*empty[random_cell] = random_input;
	numbers_generated.push_back(random_input);
}


void move_elements(std::array<long int, SIZE> *ar){
	std::array<long int, SIZE>::iterator it;

	// move down empty cells
	for (int i = 0; i < SIZE; ++i){
		for (it = ar->begin(); it != ar->end(); ++it){
			while ( (*(it+1) == 0) && (it != ar->end()) ){ 
				*(it+1) = *it;
				*it = 0;
				++it;
			}
		}
	}
	// merge
	for (it = ar->end(); it != ar->begin() - 1; --it){
		if  ((*it == *(it-1)) && (*it != 0)) {
			*(it-1) += *it;
			*it = 0;
			score += *(it-1);
			--it;
		}
	}
	// move down empty cells
	for (int i = 0; i < SIZE; ++i){
		for (it = ar->begin(); it != ar->end(); ++it){
			while ( (*(it+1) == 0) && (it != ar->end()) ){ 
				*(it+1) = *it;
				*it = 0;
				++it;
			}
		}
	}
}


void move(char input){
	// get directional input

	std::array<long int, SIZE> ar;

	switch (input) {
		case 'h':
			// move left
			for (int i = 0; i < SIZE; ++i){
				for (int j = SIZE-1; j >= 0; --j){
					ar[SIZE -1 -j] = board[i][j];
				}
				move_elements(&ar);

				for (int j = SIZE-1; j >= 0; --j){
					board[i][j] = ar[SIZE -1 -j];
				}
			}
			break;

		case 'k':
			// move up
			for (int j = 0; j < SIZE; ++j){
				for (int i = SIZE-1; i >= 0; --i){
					ar[SIZE -1 - i] = board[i][j];
				}
				move_elements(&ar);

				for (int i = SIZE-1; i >=0; --i){
					board[i][j] = ar[SIZE -1 -i];
				}
			}
			break;

		case 'j':
			// move down
			for (int j = 0; j < SIZE; ++j){
				for (int i = 0; i < SIZE; ++i){
					ar[i] = board[i][j];
				}
				move_elements(&ar);

				for (int i = 0; i < SIZE; ++i){
					board[i][j] = ar[i];
				}
			}
			break;

		case 'l':
			// move right
			for (int i = 0; i < SIZE; ++i){
				for (int j = 0; j < SIZE; ++j){
					ar[j] = board[i][j];
				}
				move_elements(&ar);

				for (int j = 0; j < SIZE; ++j){
					board[i][j] = ar[j];
				}
			}
			break;
	}
}


int is_same(){
	for (int i = 0; i < SIZE; ++i){
		for (int j = 0; j < SIZE; ++j){
			if ( board[i][j] != prev_board[i][j] ){
				return 0;
			}
		}
	}
	return 1;
}

int game_over(){
	std::array<std::array<long int, SIZE>, SIZE> board_cp = board;
	std::array<std::array<long int, SIZE>, SIZE> prev_board_cp = prev_board;;

	if (board_full()){
		move('h');
		move('j');
		move('k');
		move('l');
		if ( is_same() ) return 1;
	}

	board = board_cp;
	prev_board = prev_board_cp;
	return 0;
}
