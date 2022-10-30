/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
#include "gtypes.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * 只绘制并填充黑色的三角形
 * 输入：窗口、三个点、阶数
 * 输出：黑色三角形总个数
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0){
        // base case
        fillBlackTriangle(window, one, two, three);
        return 1;// 0阶1个三角形
    }else{
        GPoint p12 = {(one.x + two.x)/2, (one.y + two.y)/2};// 12的中点
        GPoint p13 = {(one.x + three.x)/2, (one.y + three.y)/2};// 13的中点
        GPoint p23 = {(three.x + two.x)/2, (three.y + two.y)/2};// 23的中点
        // recursive step
        int n_u = drawSierpinskiTriangle(window, one, p12, p13, order - 1);// 上三角
        int n_r = drawSierpinskiTriangle(window, p12, two, p23, order - 1);// 右三角
        int n_l = drawSierpinskiTriangle(window, p13, p23, three,order- 1);// 左三角
        return n_u + n_l + n_r;
    }
}

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

