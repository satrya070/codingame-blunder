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

const bool RESERVED = false;
char MODIFIER = '0';

map<char, int> modifier_directions = {
	{'S', 0},
	{'E', 1},
	{'N', 2},
	{'W', 3}
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

vector<tuple<int, int>> get_next_positions(
	const tuple<int, int> CurrentPos
)
{
	int CurrentRow = get<0>(CurrentPos);
	int CurrentCol = get<1>(CurrentPos);

	// TODO filter out out of bounds

	tuple<int, int> south = tuple<int, int>(CurrentRow + 1, CurrentCol);
	tuple<int, int> east = tuple<int, int>(CurrentRow, CurrentCol + 1);
	tuple<int, int> north = tuple<int, int>(CurrentRow - 1, CurrentCol);
	tuple<int, int> west = tuple<int, int>(CurrentRow, CurrentCol - 1);

	vector<tuple<int, int>> next_positions;
	next_positions = { south, east, north, west };

	if (RESERVED == true)
	{
		next_positions = { west, north, east, south };
	}

	return next_positions;
}


tuple<int, int> get_next_modifier_pos(tuple<int, int> current_pos)
{
	int new_pos_r = get<0>(current_pos);
	int new_pos_c = get<1>(current_pos);

	if (MODIFIER == 'N')
	{
		new_pos_r -= 1;
	}
	else if (MODIFIER == 'E')
	{
		new_pos_c += 1;
	}
	else if (MODIFIER == 'S')
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
		int index_direction = 0;

		// for very first iteration no need to fetch direction
		//if (loophole_index > 0)
		//{
			// fetch the next step
			if (MODIFIER == '0')
			{
				next_positions = get_next_positions(current_pos);
				current_pos = next_positions[index_direction];
			}
			else
			{
				// just get the next modifier position
				current_pos = get_next_modifier_pos(current_pos);
				index_direction = modifier_directions[MODIFIER];
			}
		//}

		// proces next position conditions (if any)
		string next_pos_value = wmap[
			get<0>(current_pos)][get<1>(current_pos)
		];

		// process next to be pos (non-modifier)
		while (processed_next_direction == false)
			if (next_pos_value == EMPTY)
			{
				processed_next_direction = true;
			}
			else if (next_pos_value == string(1, OBSTACLE))
			{
				// blockade so change direction
				index_direction += 1;
				current_pos = next_positions[index_direction];
				next_pos_value = wmap[get<0>(current_pos)][get<1>(current_pos)];
				continue;
			}
			else if (check_set_modifier(next_pos_value))
			{
				// check_modifier ^ updates the global MODIFIER if there is one.
				processed_next_direction = true;
			}
			else if (next_pos_value == string(1, BOOTH))
			{
				endgame = true;
				break;
			}

		// log positions
		//cout << "current position: ";
		printTuple(current_pos);
		//cout << " - next_position: ";
		//printTuple(next_positions[0]);

		// output direction
		cout << directionPrints[index_direction] << '\n';

		// increment loop
		loophole_index += 1;
	}

	// Write an answer using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	return 0;
}