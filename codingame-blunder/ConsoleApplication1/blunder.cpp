#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <fstream>
#include <optional>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

const string EMPTY = " ";
const char OBSTACLEX = 'X';
const char OBSTACLE = '#';
const char SOUTH = 'S';
const char EAST = 'E';
const char NORTH = 'N';
const char WEST = 'W';
const char BEER = 'B';
const char TELEPORT = 'T';
const char BOOTH = '$';
const char INVERTER = 'I';

bool INVERTED = false;
char MODIFIER = '0';
char DIRECTION = 'S';
int INDEX_DIRECTION = 0;


map<char, int> direction_index_map = {
	{'S', 0},
	{'E', 1},
	{'N', 2},
	{'W', 3}
};

map<char, int> index_direction_map = {
	{0, 'S'},
	{1, 'E'},
	{2, 'N'},
	{3, 'W'}
};

template<size_t Index = 0, typename... Types>
void printTuple(const tuple<Types...>& myTuple)
{
	if constexpr (Index < sizeof...(Types))
	{
		cout << get<Index>(myTuple) << " ";
		printTuple<Index + 1>(myTuple);
	}
}


tuple<int, int> get_next_pos(tuple<int, int> current_pos, char direction_identifier)
{
	int new_pos_r = get<0>(current_pos);
	int new_pos_c = get<1>(current_pos);

	if (direction_identifier == 'N')
	{
		new_pos_r -= 1;
	}
	else if (direction_identifier == 'E')
	{
		new_pos_c += 1;
	}
	else if (direction_identifier == 'S')
	{
		new_pos_r += 1;
	}
	else  // W
	{
		new_pos_c -= 1;
	}

	return make_tuple(new_pos_r, new_pos_c);
}


bool check_set_modifier(string new_pos_value)
{	
	char pos_value = new_pos_value.at(0);
	bool is_modifier = false;
	switch(pos_value)
	{
		case 'S':
			is_modifier = true;
			MODIFIER = 'S';
			break;
		case 'E':
			is_modifier = true;
			MODIFIER = 'E';
			break;
		case 'N':
			is_modifier = true;
			MODIFIER = 'N';
			break;
		case 'W':
			is_modifier = true;
			MODIFIER = 'W';
			break;
		default:
			break;
	}
	return is_modifier;
}



int main()
{
	// CODINGAME INPUT -----------------------------------------------------
	//int l; int c;
	//tuple<int, int> start_pos;
	//vector<tuple<int, int>> direction;
	//cin >> l >> c; cin.ignore();

	//// construct the map in an array
	//map<int, map<int, string>> wmap;
	//for (int i = 0; i < l; i++) {
	//	string row;
	//	getline(cin, row);

	//	for (int j = 0; j < l; j++)
	//	{
	//		// define start position
	//		if (row[j] == '@')
	//		{
	//			start_pos = tuple(i, j);
	//		}
	//		wmap[i][j] = row[j];
	//	}
	//}

	// ------------------ LOCAL INOUT ----------------------------------------------------
	ifstream inputFile("./input.txt");

	if (!inputFile.is_open()) {
		std::cerr << "Error opening the file." << std::endl;
		return 1; // Return error code
	}

	int l = 5;
	int c = 5;
	tuple<int, int> start_pos;
	vector<tuple<int, int>> direction;
	//cin >> l >> c; cin.ignore();

	// construct the map in an array
	map<int, map<int, string>> wmap;
	for (int i = 0; i < l; i++) {
		string row;
		getline(inputFile, row);

		for (int j = 0; j < l; j++)
		{
			// define start position
			if (row[j] == '@')
			{
				start_pos = tuple(i, j);
			}
			wmap[i][j] = row[j];
		}
		clog << row << endl;

		// by default
	}

	inputFile.close();

	// ------------------------------- PROCES PATH ---------------------------------------------------------

	// init vars
	vector<tuple<int, int>> next_positions;
	tuple<int, int> current_pos;
	current_pos = start_pos;
	map<int, string> directionPrints = {
		{0, "SOUTH"},
		{1, "EAST"},
		{2, "NORTH"},
		{3, "WEST"}
	};
	bool endgame = false;

	// loop start
	int loophole_index = 0;
	while(endgame != true)
	{
		bool processed_next_direction = false;
		//int index_direction = 0;
		tuple<int, int> queued_pos;

		if (INVERTED == true) { INDEX_DIRECTION = 3; }

		// for very first iteration no need to fetch direction
		if (MODIFIER == '0')
		{
			queued_pos = get_next_pos(current_pos, DIRECTION);
		}
		else
		{
			queued_pos = get_next_pos(current_pos, MODIFIER);
		}

		// proces next position conditions (if any)
		string next_pos_value = wmap[
			get<0>(queued_pos)][get<1>(queued_pos)
		];

		// process next to be pos (non-modifier)
		while (processed_next_direction == false)
			if (next_pos_value == EMPTY)
			{
				processed_next_direction = true;
			}
			else if ((next_pos_value == string(1, OBSTACLE)) || (next_pos_value == string(1, OBSTACLEX)) )
			{
				// blockade up on next direction

				// TODO break X if in breaker mode

				// and if inverted go previous
				if (INVERTED == false)
				{
					INDEX_DIRECTION = (INDEX_DIRECTION + 1) % 4;
				}
				else
				{
					// TODO fix previous
					INDEX_DIRECTION = (INDEX_DIRECTION - 1) % 4;
				}
				// gets the next priority pos from the ORIGINAL position!(hence queued pos)
				queued_pos = get_next_pos(current_pos, index_direction_map[INDEX_DIRECTION]);
				next_pos_value = wmap[get<0>(queued_pos)][get<1>(queued_pos)];
				continue;
			}
			else if (check_set_modifier(next_pos_value))
			{
				// check_modifier ^ updates the global MODIFIER if there is one.
				INDEX_DIRECTION = direction_index_map[MODIFIER];
				processed_next_direction = true;
			}
			else if (next_pos_value == string(1, INVERTER))
			{
				if (INVERTED == false)
				{
					INVERTED = true;
				}
				else
				{
					INVERTED = false;
				}
				processed_next_direction = true;
			}
			else if (next_pos_value == string(1, BOOTH))
			{
				endgame = true;
				break;
			}

		// set current pos after qeued pos is processed
		current_pos = queued_pos;
		DIRECTION = index_direction_map[INDEX_DIRECTION];

		// log positions
		//cout << "current position: ";
		printTuple(current_pos);
		//cout << " - next_position: ";
		//printTuple(next_positions[0]);

		// output direction
		cout << directionPrints[INDEX_DIRECTION] << '\n';

		// increment loop
		loophole_index += 1;
	}

	// Write an answer using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	return 0;
}