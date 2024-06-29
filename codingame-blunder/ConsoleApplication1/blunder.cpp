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
const char BREAKER = 'B';
const char TELEPORT = 'T';
const char BOOTH = '$';
const char INVERTER = 'I';

bool INVERTED = false;
char MODIFIER = '0';
char MODIFIER_CACHE = '0';
char DIRECTION = 'S';
bool BREAKER_MODE = false;

map<int, tuple<int, int>> TELEPORTERS;

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
			//MODIFIER = 'S';
			break;
		case 'E':
			is_modifier = true;
			//MODIFIER = 'E';
			break;
		case 'N':
			is_modifier = true;
			//MODIFIER = 'N';
			break;
		case 'W':
			is_modifier = true;
			//MODIFIER = 'W';
			break;
		default:
			break;
	}

	if (is_modifier == true)
	{
		//MODIFIER_CACHE = MODIFIER;
		MODIFIER = pos_value;
	}
	return is_modifier;
}

int findOtherTeleporterId(tuple<int, int> teleport_position)
{
	//find teleport id by position
	for (const auto& pair : TELEPORTERS)
	{
		if (pair.second != teleport_position)
		{
			return pair.first;
		}
	}

	return -1;
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
	//map<tuple<int, int>, int> teleporters;
	//int teleport_assign_id = 0;
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
	// // define teleporters (if there)
	//		if (row[j] == 'T')
	//		{
	//			teleporters[make_tuple(i, j)] = teleport_assign_id;
	//			teleport_assign_id += 1;
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

	int l = 10;
	int c = 10;
	tuple<int, int> start_pos;
	vector<tuple<int, int>> direction;
	//cin >> l >> c; cin.ignore();

	// construct the map in an array
	map<int, map<int, string>> wmap;
	int teleport_assign_id = 0;
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
			// define teleporters (if there)
			if (row[j] == 'T')
			{
				TELEPORTERS[teleport_assign_id] = make_tuple(i, j);
				teleport_assign_id += 1;
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
		int index_direction = 0;
		tuple<int, int> queued_pos;

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
				// process blockade up on next direction

				// break X if in breaker mode
				if ((BREAKER_MODE == true) && (next_pos_value == string(1, OBSTACLEX)) )
				{
					// destroy that wall
					wmap[get<0>(queued_pos)][get<1>(queued_pos)] = " ";
					break;
				}

				// TODO add reverse operation
				if (INVERTED == false)
				{
					// regular handling of obstacles
					for (int i = 0; i < 4; i++)
					{
						queued_pos = get_next_pos(current_pos, index_direction_map[index_direction]);
						next_pos_value = wmap[get<0>(queued_pos)][get<1>(queued_pos)];
						if ((next_pos_value != string(1, OBSTACLE)) && ((next_pos_value != string(1, OBSTACLEX))))
						{
							break;
						}
						index_direction += 1;
					}
				}
				else
				{
					index_direction = 3;
					// regular handling of obstacles
					for (int i = 0; i < 4; i++)
					{
						queued_pos = get_next_pos(current_pos, index_direction_map[index_direction - i]);
						next_pos_value = wmap[get<0>(queued_pos)][get<1>(queued_pos)];
						if ((next_pos_value != string(1, OBSTACLE)) && ((next_pos_value != string(1, OBSTACLEX))))
						{
							index_direction = index_direction - i;
							break;
						}
					}
				}

				DIRECTION = index_direction_map[index_direction];
			}
			else if (check_set_modifier(next_pos_value))
			{
				// check_modifier ^ updates the global MODIFIER if there is one.
				processed_next_direction = true;
				break;
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
			else if (next_pos_value == string(1, BREAKER))
			{
				if (BREAKER_MODE == false)
				{
					BREAKER_MODE = true;
				}
				else
				{
					BREAKER_MODE = false;
				}
				processed_next_direction = true;
				break;
			}
			else if (next_pos_value == string(1, TELEPORT))
			{
				int other_teleport_id = findOtherTeleporterId(make_tuple(get<0>(queued_pos), get<1>(queued_pos)));
				queued_pos = TELEPORTERS[other_teleport_id];
				processed_next_direction = true;
			}
			else if (next_pos_value == string(1, BOOTH))
			{
				endgame = true;
				break;
			}

		// set current pos after qeued pos is processed
		current_pos = queued_pos;

		// log positions
		//cout << "current position: ";
		//printTuple(current_pos);
		//cout << " - next_position: ";
		//printTuple(next_positions[0]);

		// output direction
		if (MODIFIER != '0')
		{
			// this means a modifier was already set
			if (MODIFIER_CACHE != '0')
			{
				cout << directionPrints[direction_index_map[MODIFIER_CACHE]] << '\n';
				MODIFIER_CACHE = MODIFIER;
			}
			// this means a modifier has just been set
			else
			{
				cout << directionPrints[direction_index_map[DIRECTION]] << '\n';
				MODIFIER_CACHE = MODIFIER;
			}
		}
		else
		{
			cout << directionPrints[direction_index_map[DIRECTION]] << '\n';
		}

		// increment loop
		loophole_index += 1;

		// stop this iteration after the last direction print
		if (endgame == true)
		{
			break;
		}
	}

	// Write an answer using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	return 0;
}