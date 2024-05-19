#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <vector>
#include <map>
#include <tuple>
#include <fstream>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

template<size_t Index = 0, typename... Types>
void printTuple(const tuple<Types...> &myTuple)
{
    if constexpr(Index < sizeof...(Types))
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
    next_positions = {south, east, north, west};

    if(reversed)
    {
        next_positions = {west, north, east, south};
    }

    return ;
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

    ifstream inputFile("./input.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1; // Return error code
    }

    // input from codeingame
    // string input_line;
    // while(getline(inputFile, input_line))
    // {
    //     cout << input_line << endl;
    // };
    // cout << "lalalaal"  << endl;
    // cout << 'a';

    int l = 5;
    int c = 5;
    tuple<int, int> start_pos;
    vector<tuple<int, int>> direction;
    //cin >> l >> c; cin.ignore();

    map<int, map<int, string>> wmap;
    
    for (int i = 0; i < l; i++) {
        string row;
        getline(inputFile, row);

        for(int j = 0;j < l; j++)
        {
            // define start position
            if(row[j] == '@')
            {
                start_pos = tuple(i, j);
            }
            wmap[i][j] = row[j];
        }
        clog << row << endl;

        // by default
    }

    // loop start

    // get options
    vector<tuple<int, int>> next_positions = get_next_positions(start_pos);
    vector<int> default_order = vector<int>{0, 1, 2, 3}; // S, E, N, W

    clog << "start position: ";
    printTuple(start_pos);
    //clog << "\n";

    // check next position situation
    tuple<int, int> next_pos = next_positions[0];
    string next_pos_value = wmap[
        get<0>(next_pos)][get<1>(next_pos)
    ];
    
    cout << "\ntop check: "<< next_pos_value << "\n";

    if(next_pos_value == EMPTY)
    {
        cout << "SOUTH" << endl;
    }

    // get the next position
    next_positions = get_next_positions(next_pos);
    printTuple(next_positions[0]);

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "answer" << endl;

    return 0;
}