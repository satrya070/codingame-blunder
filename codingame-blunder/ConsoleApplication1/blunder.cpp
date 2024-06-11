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
    const tuple<int, int> CurrentPos, optional<bool> reversed = false
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

    if (reversed == true)
    {
        cout << "false reversed next positions..." << '\n';
        next_positions = { west, north, east, south };
    }

    return next_positions;
}



int main()
{
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



    // init vars
    vector<tuple<int, int>> next_positions;
    tuple<int, int> next_pos;
    next_pos = start_pos;

    // loop start
    for (int i = 0; i < 4; i++)
    {
        bool processed_next_direction = false;

        // get options
        next_positions = get_next_positions(next_pos);
        vector<int> default_order = vector<int>{ 0, 1, 2, 3 }; // S, E, N, W

        //clog << "start position: ";
        //printTuple(start_pos);
        // log positions
        cout << "current position: ";
        printTuple(next_pos);
        cout << " - next_position: ";
        printTuple(next_positions[0]);

        // check next position situation
        int index_next_position = 0;
        next_pos = next_positions[index_next_position];
        string next_pos_value = wmap[
            get<0>(next_pos)][get<1>(next_pos)
            ];

            std::cout << "\nnext direction: " << next_pos_value << "\n";

            while (processed_next_direction == false)
                if (next_pos_value == EMPTY)
                {
                    std::cout << "SOUTH" << endl;
                    processed_next_direction = true;
                }
                else if (next_pos_value == string(1, OBSTACLE))
                {
                    index_next_position += 1;
                    next_pos = next_positions[index_next_position];
                    next_pos_value = wmap[get<0>(next_pos)][get<1>(next_pos)];
                    continue;
                }
                else if (next_pos_value == string(1, BOOTH))
                {
                    cout << "Done it, live!!!";
                    break;
                }
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    std::cout << "answer" << endl;

    return 0;
}