#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point {
    int x, y;
};

int player;
const int SIZE = 8;
array<array<int, SIZE>, SIZE> board;
vector<Point> next_valid_points;

void read_board(ifstream& fin) {
    fin >> player;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) 
            fin >> board[i][j];
    }
}

void read_valid_points(ifstream& fin) {
    int n_valid_points;
    int x, y;
    fin >> n_valid_points;
    for(int i = 0; i < n_valid_points; i++) {
        fin >> x >> y;
        next_valid_points.push_back({x, y});
    }
}

void write_valid_point(ofstream& fout) {
    int n_valid_points = next_valid_points.size();
    int idx;
    Point p = next_valid_points[idx];
    fout << p.x << " " << p.y << endl;
    fout.flush();
}
