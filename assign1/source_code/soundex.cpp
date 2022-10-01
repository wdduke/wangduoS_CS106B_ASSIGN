/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h" // in order to use vector class
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    //string result = charToString(s[0]);
    string result ;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

// 1. 将所有字母大写
string capitalLetter(string s){
    string result;
    for (int i = 0; i < s.length(); i++) {
        char cl = toUpperCase(s[i]);
        result += cl;
        }
    return result;
}


// 2. 字母编码化
string letterCode(string s){
    string result;

    string digital_0 = "AEIOUHWY";
    string digital_1 = "BFPV";
    string digital_2 = "CGJKQSXZ";
    string digital_3 = "DT";
    string digital_4 = "L";
    string digital_5 = "MN";
    string digital_6 = "R";

    string CL = capitalLetter(s);// 大写字母转换
    for (int i = 0; i < CL.length(); i++) {
        for (int j0 = 0; j0 < digital_0.length(); j0++){
            if (CL[i] == digital_0[j0]){
                result += "0";
                }
            }
        for (int j1 = 0; j1 < digital_1.length(); j1++){
            if (CL[i] == digital_1[j1]){
                result += "1";
                }
            }
        for (int j2 = 0; j2 < digital_2.length(); j2++){
            if (CL[i] == digital_2[j2]){
                result += "2";
                }
            }
        for (int j3 = 0; j3 < digital_3.length(); j3++){
            if (CL[i] == digital_3[j3]){
                result += "3";
                }
            }
        for (int j4 = 0; j4 < digital_4.length(); j4++){
            if (CL[i] == digital_4[j4]){
                result += "4";
                }
            }
        for (int j5 = 0; j5 < digital_5.length(); j5++){
            if (CL[i] == digital_5[j5]){
                result += "5";
                }
            }
        for (int j6 = 0; j6 < digital_6.length(); j6++){
            if (CL[i] == digital_6[j6]){
                result += "6";
                }
            }
    }
    return result;
}

// 3. 合并相邻重复数字
string adjacentNumber(string s){
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (result.back() == s[i])// result 长度未知
            continue;
        result += s[i];
    }
    return result;
}

// 4. 大写字母替换第一位
string replacefirstLetter(string s, string name){
    string CL = capitalLetter(name);// 大写
    //string code1 = letterCode(s);// 转为数字
    //string code2 = adjacentNumber(code1);// 合并相邻重复数字
        s[0] = CL[0];
    return s;
}

// 5. 删除所有的零
string removeZero(string s){
    //string code3 = replacefirstLetter(s);// 怎么用去重的的编码就跑不通？
    string result;
    for (int i = 0; i < s.length(); i++){
        if (s[i] != '0'){
            result += s[i];
        }
    }
    return result;
}

// 6. 控制编码位数为四位
string fourCode(string s){
    //string code4 = removeZero(s);// 怎么用去零的的编码就跑不通？
    string result;
    if (s.length() >= 4){
        for (int i = 0; i <= 3; i++){
            result += s[i];
        }
    }
    if (s.length() < 4){
         for (int i = 4 - s.size(); i > 0; i--) {
            s += "0";
        }
        result =s;
    }
    return result;
}

string soundex(string s) {
    string result;
    result = removeNonLetters(s);               // 0. 去除非字母符号
    result = capitalLetter(result);             // 1. 将所有字母大写
    result = letterCode(result);                // 2. 字母编码化
    result = adjacentNumber(result);            // 3. 合并相邻重复数字
    result = replacefirstLetter(result, s);     // 4. 大写字母替换第一位
    result = removeZero(result);                // 5. 删除所有的零
    result = fourCode(result);                  // 6. 控制编码位数为四位
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    // 1. 读取数据库
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;


    while (true) {
    // 2. 输入姓氏
    string inputname = getLine("Enter a surname (RETURN to quit): ");
        if (inputname != ""){       // 输入不为空时，执行程序

    // 3. 计算soundex编码
    string inputcode = soundex(inputname);
    cout << "Soundex code is " << inputcode << endl;
    // 4. 如果编码与用户输入的姓氏编码一致，则保存
    Vector<string> samename;
    for (const string& name : databaseNames){ // 这里不会，copy的
        if (inputcode == soundex(name)){
            samename.add(name);
        }
    }
    // 5. 按顺序输出这些匹配项
        samename.sort();
    cout << "The same name is ： " << samename << endl;
    }
    if (inputname == ""){       // 输入为空时，输出all done！
        cout << "All done! " << endl;
    }
    // The names in the database are now stored in the provided
    // vector named databaseNames
}
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";// 符号
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9"; // 数字
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";// 空格
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

// 测试删除非字母符号
STUDENT_TEST("Test removing minus symbol") {
    string s = "as-sa";// 横线
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "assa");
}
STUDENT_TEST("Test removing @ symbol"){
    string s = "@assa";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "assa");
}
STUDENT_TEST("Test removing void"){
    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

// 测试大写字母
STUDENT_TEST("captialLetter"){
    string s = "asASD";
    string result = capitalLetter(s);
    EXPECT_EQUAL(result, "ASASD");
}


// 测试编码
STUDENT_TEST("encode"){
    string s = "AEIOUPP";
    string result = letterCode(s);
    EXPECT_EQUAL(result, "0000011");
    s = "Planet9"; // 数字
    result = letterCode(s);
    EXPECT_EQUAL(result, "140503");
}

// 测试合并重复数字
STUDENT_TEST("adjacentNumber"){
    string s = "223344";
    string result = adjacentNumber(s);
    EXPECT_EQUAL(result, "234");
    s = "122345"; // 数字
    result = adjacentNumber(s);
    EXPECT_EQUAL(result, "12345");
}

// 测试首字替换
STUDENT_TEST("replacefirstLetter"){
    string s = "planet9";
    string s1 = letterCode(s);
    string result = replacefirstLetter(s1,s);
    EXPECT_EQUAL(result, "P40503");
    s = "asasa";
    s1 = letterCode(s);
    result = replacefirstLetter(s1,s);
    EXPECT_EQUAL(result, "A2020");
}

// 测试删除零
STUDENT_TEST("removeZero"){
    string s = "A2020";
    string result = removeZero(s);
    EXPECT_EQUAL(result, "A22");
}

//测试位数控制
STUDENT_TEST("fourCode"){
    string s = "A2020";
    string result = fourCode(s);
    EXPECT_EQUAL(result, "A202");
    s = "wangduo";
    result = fourCode(s);
    EXPECT_EQUAL(result, "wang");
}

// 测试顶层函数soundex
STUDENT_TEST("soundex") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
    EXPECT_EQUAL(soundex(""), "0000");
    EXPECT_EQUAL(soundex(" ——"), "0000");
    EXPECT_EQUAL(soundex("流浪剑客"), "0000");
}
