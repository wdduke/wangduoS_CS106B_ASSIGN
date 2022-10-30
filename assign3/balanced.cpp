/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"
#include "strlib.h"

using namespace std;

/*
 *
 * 从输入中提取出括号操作符
 */
#define v_1
#ifdef v_2
string operatorsFrom(string str){
    // 不能把变量声明放在递归外面
    string first = str.substr(0,1);
    string remaind = str.substr(1);// 由第二个字符到末尾
    string aim = "(){}[]";
    if(str.empty()){
        return "";
    }else{
        // 只能放在这里递归才能正常
    // 处理第一个字符，确定是否可以放到最终结果中
        if (stringContains(aim, first)){
        // base case
        // 做递归调用处理剩下的字符串并保存结果
        // 整合第一个字符处理结果和递归调用处理结果，返回只包含括号操作符的最终形式
            return first + operatorsFrom(remaind);
        }else{
        // recursice step
            return operatorsFrom(remaind);
        }
    }
}
#endif
#ifdef v_1
string operatorsFrom(string str) {
    string aim = "(){}[]";
    if (str.empty()){
        return "";
    }else{
        // 处理第一个字符，确定是否可以放到最终结果中
        string first = str.substr(0,1);
        string remaind = str.substr(1);// 由第二个字符到末尾
        if (stringContains(aim, first)){
            // base case
            // 做递归调用处理剩下的字符串并保存结果
            // 整合第一个字符处理结果和递归调用处理结果，返回只包含括号操作符的最终形式
            return first + operatorsFrom(remaind);
        }else{
            // recursice step
            return operatorsFrom(remaind);
        }
    }
}
#endif

// test
STUDENT_TEST("operatorsFrom on simple example"){
    EXPECT_EQUAL(operatorsFrom("(a){b}[c]"), "(){}[]");
}
STUDENT_TEST("disorder"){
    EXPECT_EQUAL(operatorsFrom("(a}{b)}[c]{"), "(}{)}[]{");
}
STUDENT_TEST("empty"){
    EXPECT_EQUAL(operatorsFrom(""), "");
}
/*
 * 确定输入是否配对
 *
 */


bool operatorsAreMatched(string ops) {
    // 字符串为空
    if (ops.empty()){
        return true;
    }else{
            // base case
        string remaind;// 声明一个返回值
        if (stringContains(ops,"()" )){
           remaind = stringReplace(ops,"()","");
        }else if(stringContains(ops,"{}" )){
           remaind = stringReplace(ops,"{}","");
        }else if(stringContains(ops,"[]" )){
           remaind =stringReplace(ops,"[]","");
        }else {
            return false;// 不配对就返回false
        }
            // recursice step
            return operatorsAreMatched(remaind);
    }
}

#ifdef v_3
bool operatorsAreMatched(string ops) {
    // 字符串为空
    if (ops.empty()){
        return true;
    }else{
            // base case
        if (stringContains(ops,"()" )){
            stringReplaceInPlace(ops,"()","");
        }else if(stringContains(ops,"{}" )){
            stringReplaceInPlace(ops,"{}","");
        }else if(stringContains(ops,"[]" )){
            stringReplaceInPlace(ops,"[]","");
        }else {
            return false;// 不配对就返回false
        }
            // recursice step
            return operatorsAreMatched(ops);
    }
}
#endif
// test
STUDENT_TEST("()") {
    EXPECT(operatorsAreMatched("()"));
}
STUDENT_TEST("{([])}") {
    EXPECT(operatorsAreMatched("{([])}"));
}
STUDENT_TEST("{([}") {
    EXPECT_EQUAL(operatorsAreMatched("{([}"), false);
}
STUDENT_TEST("empty") {
    EXPECT_EQUAL(operatorsAreMatched(""), true);
}
STUDENT_TEST("{(asd[}") {
    EXPECT_EQUAL(operatorsAreMatched("{(asd[}"), false);
}
/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
