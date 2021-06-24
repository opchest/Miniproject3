#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#define SIZE 8

using namespace std;

int player;
struct Point {
    int x, y;
    Point() : Point(0, 0){}
    Point(int x, int y) : x{x}, y{y} {}
    bool operator==(const Point& p) const {
		return (x == p.x && y == p.y);
	}
	bool operator!=(const Point& p) const {
		return !operator==(p);
	}
	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y);
	}
	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y);
	}
};
const array<Point, 8> directions {{
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};
const array<Point, 4> corner {{
    Point(0, 0), Point(0, 7),
    Point(7, 0), Point(7, 7)
}};
array<array<int, SIZE>, SIZE> board;
vector<Point> next_valid_points;

class State {
private:
    array<array<int, SIZE>, SIZE> Board;
    int coin_heuristic;
    int mobil_heuristic;
    int corner_heuristic;
public:
    State(): coin_heuristic{0}, mobil_heuristic{0}, corner_heuristic{0} {}
    State(const State& S): coin_heuristic{S.coin_heuristic}, mobil_heuristic{S.mobil_heuristic}, 
                            corner_heuristic{S.corner_heuristic} {}
    State(const array<array<int, SIZE>, SIZE>& board) {
        Board = board;
    }
    int cal_ratio(const int& my, const int& oppo) {
        return (my - oppo) / (my + oppo);
    }
    bool is_point_inside(Point p) const {
        return (0 <= p.x && p.x < SIZE && 0 <=p.y && p.y < SIZE);
    }
    bool is_point_valid(Point center, int who) const {
        if(board[center.x][center.y] != 0)
            return false;
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || board[p.x][p.y] == who)
                continue;
            p = p + d;
            while(is_point_inside(p) && board[p.x][p.y] != 0) {
                if(board[p.x][p.y] == who) 
                    return true;
                p = p + d;
            }
        }
        return false;
    }
    void Cal_heuristic() {
        int my_coins = 0, oppo_coins = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(board[i][j] == player) 
                    my_coins++;
                else if(board[i][j] == 3 - player)
                    oppo_coins++;
            }
        }
        coin_heuristic = 100 * cal_ratio(my_coins, oppo_coins);
        int my_mobil = 0, oppo_mobil = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                Point center(i, j);
                if(is_point_valid(center, player)) 
                    my_mobil++;
                if(is_point_valid(center, 3 - player))
                    oppo_mobil++;
            }
        }
        mobil_heuristic = 100 * cal_ratio(my_mobil, oppo_mobil);
        int my_corner = 0, oppo_corner = 0;
        for(Point i : corner) {
            if(board[i.x][i.y] == player)
                my_corner++;
            if(board[i.x][i.y] == 3 - player)
                oppo_corner++;
        }
        corner_heuristic = 100 * cal_ratio(my_corner, oppo_corner);
        // Stability.
    }
};
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
    int alpha = -10000000;
    for(int i = 0; i < n_valid_points; i++) {
        State next(board);
        
    }
}

int main(int, char **argv) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    read_board(fin);
    read_valid_points(fin);
    write_valid_point(fout);
    fin.close();
    fout.close();
    return 0;
}