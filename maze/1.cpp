#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <utility>
using namespace std;

class Map
{
public:
    void take_raw_data();
    void seperate_the_region();
    void dfs(int, int);

    void find_escape_path();
    bool can_go(int, int, int, int);

    void faraway_fire();
    bool fire_can_go(int, int, int);

    void output();

private:
    int row, column;
    pair<int, int> fire;
    pair<int, int> MAYDAY;

    int **clear_map;
    int **region_map;
    int **escape_map;
    int **fire_map;

    int region;
    int escape;
    queue<pair<pair<int, int>, int>> escape_chance;
    int when_fire;
    int survive;
};

int main()
{
    Map map;

    map.take_raw_data();
    map.seperate_the_region();
    map.find_escape_path();
    map.faraway_fire();
    map.output();

    return 0;
}

void Map::take_raw_data()
{
    cin >> row >> column >> fire.first >> fire.second;

    char tmp;

    clear_map = new int *[row];
    for (int i = 0; i < row; i++)
        clear_map[i] = new int[column];

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            cin >> tmp;
            switch (tmp)
            {
            case '$':
                clear_map[i][j] = 0;
                break;
            case '+':
                clear_map[i][j] = 1;
                break;
            case '%':
                clear_map[i][j] = -1;
                MAYDAY = make_pair(i, j);
                break;
            default:
                break;
            }
        }
    }
}

void Map::seperate_the_region()
{
    region_map = new int *[row];
    for (int i = 0; i < row; i++)
        region_map[i] = new int[column];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            region_map[i][j] = 0;

    region = 0;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (clear_map[i][j] == 1)
                ; //region_map[i][j] = 0;
            else if (region_map[i][j] == 0)
            {
                region++;
                dfs(i, j);
            }
        }
    }
}

void Map ::dfs(int x, int y)
{
    if (x >= row || x < 0 || y >= column || y < 0) //check boundary
        return;
    if (clear_map[x][y] == 1 || region_map[x][y] != 0) //,then ocupied
        return;

    region_map[x][y] = region; //means visited

    dfs(x - 1, y);
    dfs(x + 1, y);
    dfs(x, y + 1);
    dfs(x, y - 1);
}

void Map::find_escape_path()
{
    escape_map = new int *[row];
    for (int i = 0; i < row; i++)
        escape_map[i] = new int[column];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            escape_map[i][j] = 0;

    queue<pair<pair<int, int>, int>> record;
    record.push(make_pair(MAYDAY, 0));
    escape_map[MAYDAY.first][MAYDAY.second] = -1;
    escape = -1;

    int group = region_map[MAYDAY.first][MAYDAY.second];

    while (!record.empty())
    {
        pair<pair<int, int>, int> tmp = record.front(); //pair<pair<x, y>, distance>
        record.pop();

        if (can_go(tmp.first.first - 1, tmp.first.second, group, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first - 1, tmp.first.second), tmp.second + 1));
            escape_map[tmp.first.first - 1][tmp.first.second] = tmp.second + 1; //step
        }
        if (can_go(tmp.first.first + 1, tmp.first.second, group, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first + 1, tmp.first.second), tmp.second + 1));
            escape_map[tmp.first.first + 1][tmp.first.second] = tmp.second + 1;
        }
        if (can_go(tmp.first.first, tmp.first.second + 1, group, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first, tmp.first.second + 1), tmp.second + 1));
            escape_map[tmp.first.first][tmp.first.second + 1] = tmp.second + 1;
        }
        if (can_go(tmp.first.first, tmp.first.second - 1, group, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first, tmp.first.second - 1), tmp.second + 1));
            escape_map[tmp.first.first][tmp.first.second - 1] = tmp.second + 1;
        }
    }
}

bool Map::can_go(int x, int y, int group, int safe)
{ //x, y are simpler and clear-looked than pair<int,int> coordinate
    if (x < 0 || x >= row || y < 0 || y >= column)
    {
        if (escape == -1) // minimum escape path
            escape = safe;
        //escape_chance in some concept is sorted
        if (x < 0)  //for question 4
            escape_chance.push(make_pair(make_pair(0, y), safe));
        else if (y < 0)
            escape_chance.push(make_pair(make_pair(x, 0), safe));
        else if (x >= row)
            escape_chance.push(make_pair(make_pair(row - 1, y), safe));
        else if (y >= column)
            escape_chance.push(make_pair(make_pair(x, column - 1), safe));
        return false;
    }
    if (escape_map[x][y] == 0 && region_map[x][y] == group)
        return true;
    return false;
}

void Map::faraway_fire()
{
    fire_map = new int *[row];
    for (int i = 0; i < row; i++)
        fire_map[i] = new int[column];

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            fire_map[i][j] = 0;

    when_fire = 0;
    queue<pair<pair<int, int>, int>> record;
    record.push(make_pair(fire, when_fire));
    fire_map[fire.first][fire.second] = -1;

    while (!record.empty())
    {
        pair<pair<int, int>, int> tmp = record.front();
        record.pop();

        tmp.second++;

        if (fire_can_go(tmp.first.first - 1, tmp.first.second - 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first - 1, tmp.first.second - 1), tmp.second));
            fire_map[tmp.first.first - 1][tmp.first.second - 1] = tmp.second;
        }
        if (fire_can_go(tmp.first.first - 1, tmp.first.second, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first - 1, tmp.first.second), tmp.second));
            fire_map[tmp.first.first - 1][tmp.first.second] = tmp.second;
        }
        if (fire_can_go(tmp.first.first - 1, tmp.first.second + 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first - 1, tmp.first.second + 1), tmp.second));
            fire_map[tmp.first.first - 1][tmp.first.second + 1] = tmp.second;
        }
        if (fire_can_go(tmp.first.first, tmp.first.second + 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first, tmp.first.second + 1), tmp.second));
            fire_map[tmp.first.first][tmp.first.second + 1] = tmp.second;
        }
        if (fire_can_go(tmp.first.first + 1, tmp.first.second + 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first + 1, tmp.first.second + 1), tmp.second));
            fire_map[tmp.first.first + 1][tmp.first.second + 1] = tmp.second;
        }
        if (fire_can_go(tmp.first.first + 1, tmp.first.second, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first + 1, tmp.first.second), tmp.second));
            fire_map[tmp.first.first + 1][tmp.first.second] = tmp.second;
        }
        if (fire_can_go(tmp.first.first + 1, tmp.first.second - 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first + 1, tmp.first.second - 1), tmp.second));
            fire_map[tmp.first.first + 1][tmp.first.second - 1] = tmp.second;
        }
        if (fire_can_go(tmp.first.first, tmp.first.second - 1, tmp.second))
        {
            record.push(make_pair(make_pair(tmp.first.first, tmp.first.second - 1), tmp.second));
            fire_map[tmp.first.first][tmp.first.second - 1] = tmp.second;
        }
    }

    survive = -1;
    while (!escape_chance.empty())
    {
        int x, y, time;
        x = escape_chance.front().first.first;
        y = escape_chance.front().first.second;
        time = escape_chance.front().second;

        escape_chance.pop();
        if (fire_map[x][y] > time)
        {
            survive = time;
            return;
        }
    }
}

bool Map::fire_can_go(int x, int y, int range)
{
    if (x < 0 || x >= row || y < 0 || y >= column)
        return false;
    if (fire_map[x][y] == 0 && clear_map[x][y] != 1)
        return true;
    return false;
}

void Map::output()
{
    cout << "[1]show the map:" << endl;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
            cout << setw(2) << clear_map[i][j];
        cout << endl;
    }
    cout << "========================================" << endl
         << "[2]show the map:" << endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
            cout << setw(3) << region_map[i][j];
        cout << endl;
    }
    cout << "The number of regions is " << region << endl
         << "========================================" << endl
         << "[3]show the map:" << endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
            cout << setw(3) << escape_map[i][j];
        cout << endl;
    }
    cout << "Is there a minumum path? " << escape << endl
         << "========================================" << endl
         << "[4]show the map:" << endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
            cout << setw(3) << fire_map[i][j];
        cout << endl;
    }
    cout << "Can they escape from the fire? " << survive << endl;
}