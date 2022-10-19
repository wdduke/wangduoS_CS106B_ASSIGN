// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "gridlocation.h"
using namespace std;


/* 寻找当前位置的所有相邻位置
 * 产生可移动方向
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation north;     // 北坐标
    GridLocation sourth;    // 南坐标
    GridLocation east;      // 东坐标
    GridLocation west;      // 西坐标
    north  = GridLocation(cur.row-1,cur.col);
    sourth = GridLocation(cur.row+1,cur.col);
    east = GridLocation(cur.row,cur.col+1);
    west = GridLocation(cur.row,cur.col-1);
    // **可行邻点计算**
    if (maze.inBounds(north) && maze[north]){
        neighbors.add(north);
    }
    if (maze.inBounds(sourth) && maze[sourth]){
        neighbors.add(sourth);
    }
    if (maze.inBounds(east) && maze[east]){
        neighbors.add(east);
    }
    if (maze.inBounds(west) && maze[west]){
        neighbors.add(west);
    }
    return neighbors;
}
// 角落测试corner_test
STUDENT_TEST("generateValidMoves corner_test{0,0}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {{0, 1}, {1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves corner_test{2,2}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {2, 2};
    Set<GridLocation> expected = {{2, 1}, {1, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves corner_test{2,0}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {2, 0};
    Set<GridLocation> expected = {{2, 1}, {1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves corner_test{0,2}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {0, 2};
    Set<GridLocation> expected = {{0, 1}, {1, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
// 角落墙壁测试corner_wall_test
STUDENT_TEST("generateValidMoves corner_wall_test{0,0}") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}
STUDENT_TEST("generateValidMoves corner_wall_test{0,1}") {
    Grid<bool> maze = {{false,true },
                       {true, true}};
    GridLocation corner = {0, 1};
    Set<GridLocation> expected = {{1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}
STUDENT_TEST("generateValidMoves corner_wall_test{1,0}") {
    Grid<bool> maze = {{true, true},
                       {true, false}};
    GridLocation corner = {1, 0};
    Set<GridLocation> expected = {{0, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}
STUDENT_TEST("generateValidMoves corner_wall_test{1,1}") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {1, 1};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}
// 边缘测试edge_test
STUDENT_TEST("generateValidMoves edge_test{0,1}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {1, 1}, {0, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves edge_test{2,1}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {2, 1};
    Set<GridLocation> expected = {{2, 0}, {1, 1}, {2, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves edge_test{1,0}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 0};
    Set<GridLocation> expected = {{0, 0}, {1, 1}, {2, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
STUDENT_TEST("generateValidMoves edge_test{1,2}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 2};
    Set<GridLocation> expected = {{2, 2}, {1, 1}, {0, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
/* 验证路径
 *
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};// 出口坐标
    // 如果栈顶不是出口
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    GridLocation mazeStart = {0, 0};// 入口坐标
    GridLocation cur = path.pop();// 路线的当前位置(初始栈顶)出口
    GridLocation next_location;// 下一个位置
    // 记录走过的路
    Set<GridLocation> road;
    // 记录相邻有效位置;
    Set<GridLocation> neighbors;
    while (path.isEmpty()==false){
        // 移除栈顶之后第一个元素
        next_location = path.pop();
        // 计算相邻有效位置
        neighbors = generateValidMoves(maze, cur);
        // 下一个位置是不是有效邻点
        if (neighbors.contains(next_location)==false){
            if (neighbors.isEmpty()){
                continue;
            }
            else{
                error("It's locked down!");
            }
        }
        // 记录走过的路，不回头
        if (road.contains(cur)==false){
            road.add(cur);
        }
        else{
            error("Don’t go backwards");
        }
        cur = next_location;
    }
    // 如果栈底不是入口
    if (cur != mazeStart) {
        if (neighbors.isEmpty()){
            //continue;
        }
        else{
            error("Path does not start at maze entrance");
        }
    }
}
// 路线验证
STUDENT_TEST("validatePath {0,0}") {
    Grid<bool> maze = {{true}};
    Stack<GridLocation> revisit = {{0, 0}};
    EXPECT_NO_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("validatePath 走回头路") {
    Grid<bool> maze = {{true,false},{true,true}};;
    Stack<GridLocation> revisit = {{0, 0}, {1, 0},{0, 0},{1, 0}, {1, 1}};
    EXPECT_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("validatePath 撞墙回头") {
    Grid<bool> maze = {{true,false},{true,true}};;
    Stack<GridLocation> revisit = {{0, 0}, {0, 1},{0, 0},{1, 0}, {1, 1}};
    EXPECT_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("validatePath 原地踏步路径") {
    Grid<bool> maze = {{true, false}, {true, true}};
    Stack<GridLocation> revisit = {{0, 0}, {0, 0}, {1, 0}, {1, 1}};
    Stack<GridLocation> revisit2 = {{0, 0}, {1, 0}, {1, 0}, {1, 1}};

    EXPECT_ERROR(validatePath(maze, revisit));
    EXPECT_ERROR(validatePath(maze, revisit2));
}

STUDENT_TEST("generateValidMoves 绕圈") {
    Grid<bool> maze = {{true, true, true}, {true, true, true}, {true, true, true}};
    Stack<GridLocation> revisit
        = {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {0, 2}, {0, 1}, {1, 1}, {2, 1}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, revisit));
}
/* 获取Grid的行列信息，并打包成结构体
 *
 */

// 保存走过的路，防止回头
Set<GridLocation> collectPath(Stack<GridLocation> path) {
    Set<GridLocation> visited;
    while (!path.isEmpty()) {
        visited.add(path.pop());
    }
    return visited;
}
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);// 画图
    // 入口
    GridLocation mazeStart={0, 0};
    // 出口
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    // 有效位置
    Set<GridLocation> neighbors;
    // 从入口出发
    Stack<GridLocation> path = {mazeStart};
    // 可能的路径
    Queue<Stack<GridLocation>> may_path={path};
    // 当前路径
    Stack<GridLocation> cur_path;

    // 若仍存在多余路径
    while(may_path.isEmpty()==false){
         // 取出当前路径（队列首）
         cur_path = may_path.dequeue();// remove&return
         MazeGraphics::highlightPath(cur_path, "GREEN", 1);
         // 找出可以前进的位置
         neighbors = generateValidMoves(maze, cur_path.peek());
         // 如果当前路径已到达出口
        if (cur_path.peek() == mazeExit){// return& without removing
            // 当前路径即为最短,退出查询
            break;
        }
        // 否则，重新规划路径
        for (const auto &location : neighbors) {// 只读取neighbors中元素location,不会进行拷贝，也不会修改
            if (collectPath(cur_path).contains(location)==false) { // 避免回头
                auto copyPath = cur_path;
                copyPath.push(location);// 增加一个位置给路径副本
                may_path.enqueue(copyPath);// 增加一条路径给可能的队列
            }
        }
    }
    return cur_path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
