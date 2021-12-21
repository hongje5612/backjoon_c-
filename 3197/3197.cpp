#include <iostream>
#include <queue>
#include <memory>
#include <cstring>
#include <string>
#include <utility>
using namespace std;

struct location {
    location(int, int);

    int row, col;
};

location::location(int row, int col) : row(row), col(col) {}

bool operator==(const location& lhs, const location& rhs) {
    return lhs.row == rhs.row && lhs.col == rhs.col;
}

class lake {
public :
    static constexpr char WATER = '.';
    static constexpr char SWAN = 'L';
    static constexpr char ICE = 'X';

    static constexpr size_t NUMBER_OF_SWANS = 2;

    static constexpr int D_ROW[] = { -1, 1, 0, 0 }; // 상하좌우
    static constexpr int D_COL[] = { 0, 0, -1, 1 };

    static constexpr char VISITED = 'V';

    lake(int, int);
    lake(const lake&) = delete;
    lake(lake&&) = delete;

    ~lake();

    lake& operator=(const lake&) = delete;
    lake& operator=(lake&&) = delete;

    void a_day_passes();
    
    bool possible_to_meet();

    void show() const;

private :
    bool bfs();
    bool in_contack_with_water(int, int) const;

private :
    char** _map;
    int _row_size, _col_size;
    location* _loc_of_swans;
};

int main()
{
    int row_size, col_size;
    cin >> row_size >> col_size;

    lake l(row_size, col_size);
    int elaped_day = 0;

    while (!l.possible_to_meet()) {
        l.a_day_passes();
        elaped_day++;
    }

    cout << elaped_day << endl;

    return 0;
}

lake::lake(int row_size, int col_size)
{
    allocator<char> char_alloc;
    allocator<char*> ptr_char_alloc;
    allocator<location> location_alloc;

    _loc_of_swans = location_alloc.allocate(NUMBER_OF_SWANS);
    _row_size = row_size;
    _col_size = col_size;

    _map = ptr_char_alloc.allocate(row_size);
    
    size_t index = 0;
    for (int i = 0; i < row_size; i++) {
        _map[i] = char_alloc.allocate(col_size);

        string s;
        cin >> s;

        int j = 0;
        for (char c : s) {
            if (c == SWAN) {
                _loc_of_swans[index++] = { i, j };
                _map[i][j] = WATER;
            }
            else {
                _map[i][j] = c;
            }
        }
    }
}

lake::~lake()
{
    allocator<char> char_alloc;
    allocator<char*> ptr_char_alloc;
    allocator<location> location_alloc;

    for (int i = 0; i < _row_size; i++) {
        char_alloc.deallocate(_map[i], _col_size);
    }
    ptr_char_alloc.deallocate(_map, _row_size);

    location_alloc.deallocate(_loc_of_swans, NUMBER_OF_SWANS);
}

void lake::a_day_passes()
{
    allocator<char> char_alloc;
    allocator<char*> ptr_char_alloc;

    char** map = ptr_char_alloc.allocate(_row_size);

    for (int i = 0; i < _row_size; i++) {
        map[i] = char_alloc.allocate(_col_size);
    }

    for (int r = 0; r < _row_size; r++) {
        for (int c = 0; c < _col_size; c++) {
            if (_map[r][c] == ICE && in_contack_with_water(r, c)) map[r][c] = WATER;
            else map[r][c] = _map[r][c];
        }
    }

    swap(map, _map);

    for (int i = 0; i < _row_size; i++) {
        char_alloc.deallocate(map[i], _col_size);
    }
    ptr_char_alloc.deallocate(map, _row_size);
}

bool lake::possible_to_meet()
{
    return bfs();
}

void lake::show() const
{
    for (int i = 0; i < _row_size; i++) {
        for (int j = 0; j < _col_size; j++) {
            cout << _map[i][j];
        }
        cout << endl;
    }
}

bool lake::bfs()
{
    allocator<bool> bool_alloc;
    allocator<bool*> ptr_bool_alloc;

    bool** in_queue = ptr_bool_alloc.allocate(_row_size);
    for (int i = 0; i < _row_size; i++) {
        in_queue[i] = bool_alloc.allocate(_col_size);
        for (int j = 0; j < _col_size; j++) {
            in_queue[i][j] = false;
        }
    }

    queue<location> q;
    location l = _loc_of_swans[0];
    q.push(l);
    in_queue[l.row][l.col] = true;
    bool meet = false;

    while (!q.empty()) {
        l = q.front();
        if (l == _loc_of_swans[1]) {
            meet = true;
            break;
        }
        q.pop();
        in_queue[l.row][l.col] = false;
        _map[l.row][l.col] = VISITED;

        for (int i = 0; i < sizeof(D_ROW) / sizeof(int); i++) {
            int r = l.row + D_ROW[i];
            int c = l.col + D_COL[i];

            if (r >= 0 && r < _row_size && c >= 0 && c < _col_size && !in_queue[r][c] && _map[r][c] == WATER) {
                q.push({ r, c });
                in_queue[r][c] = true;
            }
        }
    }

    for (int i = 0; i < _row_size; i++) {
        bool_alloc.deallocate(in_queue[i], _col_size);
    }
    ptr_bool_alloc.deallocate(in_queue, _row_size);

    return meet;
}

bool lake::in_contack_with_water(int row, int col) const
{
    for (int i = 0; i < sizeof(D_ROW) / sizeof(int); i++) {
        int r = row + D_ROW[i];
        int c = col + D_COL[i];

        if (r >= 0 && r < _row_size && c >= 0 && c < _col_size) {
            if (_map[r][c] == WATER) return true;
        }
    }
    return false;
}
