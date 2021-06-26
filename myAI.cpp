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
    int edge_heuristic;
    int stable_heuristic;
public:
    State(): coin_heuristic{0}, mobil_heuristic{0}, corner_heuristic{0}, edge_heuristic{0},
             stable_heuristic{0} {}
    State(const State& S): coin_heuristic{S.coin_heuristic}, mobil_heuristic{S.mobil_heuristic}, 
                            corner_heuristic{S.corner_heuristic}, edge_heuristic{S.edge_heuristic},
                            stable_heuristic{S.stable_heuristic}
                            {
                                Board = S.Board;
                            }
    State(const array<array<int, SIZE>, SIZE>& board) :coin_heuristic{0}, mobil_heuristic{0}, corner_heuristic{0} {
        Board = board;
    }
    int cal_ratio(const int& my, const int& oppo) {
        if((my + oppo) != 0)
            return (my - oppo) / (my + oppo);
        else 
            return 0;
    }
    bool is_point_inside(Point p) const {
        return (0 <= p.x && p.x < SIZE && 0 <=p.y && p.y < SIZE);
    }
    bool is_point_valid(Point center, int who) const {
        if(Board[center.x][center.y] != 0)
            return false;
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] != 3 - who)
                continue;
            p = p + d;
            while(is_point_inside(p) && Board[p.x][p.y] != 0) {
                if(Board[p.x][p.y] == who) 
                    return true;
                p = p + d;
            }
        }
        return false;
    }
    vector<Point> get_valid_points(int who) const {
        vector<Point> valid_points;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if(Board[i][j] != 0)
                    continue;
                if(is_point_valid(p, who))
                    valid_points.push_back(p);
            }
        }
        return valid_points;
    }
    bool is_point_stable(Point center, int who) {
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] != who)
                continue;
            while(Board[p.x][p.y] == who && is_point_inside(p)) {
                for(Point c : corner) {
                    if(p == c)
                        return true;
                } 
                p = p + d;
            }
        }
        return false;
    }
    int Cal_heuristic() {
        int my_coins = 0, oppo_coins = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(Board[i][j] == player) 
                    my_coins++;
                else if(Board[i][j] == 3 - player)
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
        mobil_heuristic = 1000 * cal_ratio(my_mobil, oppo_mobil);
        int my_corner = 0, oppo_corner = 0;
        for(Point i : corner) {
            if(Board[i.x][i.y] == player)
                my_corner++;
            if(Board[i.x][i.y] == 3 - player)
                oppo_corner++;
        }
        corner_heuristic = 1000 * cal_ratio(my_corner, oppo_corner);
        int my_edge = 0, oppo_edge = 0;
        for(int i : {0, 7}) {
            for(int j = 1; j < SIZE - 1; j++) {
                if(Board[i][j] == player)
                    my_edge++;
                if(Board[i][j] == 3 - player)
                    oppo_edge++;
            }
        }
        for(int i : {0, 7}) {
            for(int j = 1; j < 7; j++) {
                if(Board[j][i] == player)
                    my_edge++;
                if(Board[j][i] == 3 - player)
                    oppo_edge++;
            }
        }
        edge_heuristic = 1000 * cal_ratio(my_edge, oppo_edge);
        int my_stabil = 0, oppo_stabil = 0;
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(Board[i][j] != 0) {
                    Point p(i, j);
                    if(is_point_stable(p, player)) 
                    my_stabil++;
                    if(is_point_stable(p, 3 - player))
                        oppo_stabil++;
                }
            }
        }
        stable_heuristic = 1000 * cal_ratio(my_stabil, oppo_stabil);
        return (coin_heuristic + 5 * mobil_heuristic +  10 * corner_heuristic +   edge_heuristic +  stable_heuristic);
    }
    void flip_coins(Point center, int who) {
        for(Point d : directions) {
            Point p = center + d;
            if(!is_point_inside(p) || Board[p.x][p.y] != 3 - who)
                continue;
            vector<Point> coins({p});
            p = p + d;
            while(is_point_inside(p) && Board[p.x][p.y] != 0) {
                if(Board[p.x][p.y] == who) {
                    for(Point e : coins) {
                        Board[e.x][e.y] = who;
                    } 
                    Board[center.x][center.y] = who;
                    break;
                }   
                coins.push_back(p);
                p = p + d;
            }
        }
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
int alpha_beta_prune(State node, int depth, int alpha, int beta, int Player) {
    if(depth == 0) 
        return node.Cal_heuristic();
    if(Player == player) {
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            alpha = max(alpha, alpha_beta_prune(next, depth - 1, alpha, beta, 3 - Player));
            if(beta <= alpha)
                break;
        }
        return alpha;
    }
    else {
        vector<Point> valid_points = node.get_valid_points(Player);
        int points_number = valid_points.size();
        for(int i = 0; i < points_number; i++) {
            State next = node;
            Point p = valid_points[i];
            next.flip_coins(p, Player);
            beta = min(beta, alpha_beta_prune(next, depth - 1, alpha, beta, 3 - Player));
            if(beta <= alpha)
                break;
        }
        return beta;
    }
}
void write_valid_point(ofstream& fout) {
    State cur(board);
    int n_valid_points = next_valid_points.size();
    int best_heuristic = -10000000;
    for(int i = 0; i < n_valid_points; i++) {
        State next = cur;
        Point p = next_valid_points[i];
        next.flip_coins(p, player);
        int h = alpha_beta_prune(next, 6, -1000000, 1000000,  3 - player);
        if(h > best_heuristic) {
            best_heuristic = h;
            fout << p.x << " " << p.y << endl;
            fout.flush();
        }
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