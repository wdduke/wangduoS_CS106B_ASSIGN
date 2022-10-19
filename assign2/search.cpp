// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <ctype.h>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;


// 预处理文本函数
// 去除字符串首尾的标点，并将所有字母转换为小写。
// 检查字符串是否全为非字母字符，如果都是非字母，则丢弃该字符串并返回一个空字符串。
string cleanToken(string s){
    string token;
    string token1;
    string s1 = s;
// 检查字符串是否全为非字母字符，全是非字母返回空字符串
    int j = 0;// 非字母字符数计数
    for (int i = 0;i < s1.length();i++){
         if (isalpha(s1[i]) == 0){
            j = j + 1; // 若为非字母字符，计数+1
         }
    }
         if (j == s1.length()){// 若最后计数值等于字符长度，返回空字符串
            return token1 = "";
         }
         else{// 否则返回原字符串

         // 当字首有标点,去除首标点
            if (ispunct(s[0])!=0){
                while (ispunct(s[0]) != 0){
                        token = s.erase(0, 1);
                }
            }
         // 当字首无标点
            else{
                        token = s;
            }
         // 当字尾有标点，去除尾标点
            if (ispunct(s[token.size()-1])!=0){
                while (ispunct(token[token.size()-1]) != 0){
                        token1 = token.erase(token.size()-1, 1);
                }
            }
         // 当字尾无标点
            else{
                  token1 = token;
            }
         }
// 将字母转换为小写
    return toLowerCase(token1);
}
// 测试
STUDENT_TEST("cleanToken test") {
    EXPECT_EQUAL(cleanToken("w1"), "w1");
    EXPECT_EQUAL(cleanToken("w1!"), "w1");
    EXPECT_EQUAL(cleanToken("!w1"), "w1");
    EXPECT_EQUAL(cleanToken("!123!"), "");
    EXPECT_EQUAL(cleanToken("**"), "");
    EXPECT_EQUAL(cleanToken("<<blue>>"), "blue");
}

// 从正文文本中提取出所有不重复的字符串，并以集合的形式返回
//
Set<string> gatherTokens(string bodytext){
    Vector<string> sub_text;
    Set<string> tokens;
    string trans;
// 先以空格为分隔符，把正文文本拆分成一个个独立的子字符串
    sub_text = stringSplit(bodytext," ");
// 然后调用 cleanTokens 处理每个子字符串并将结果存放到集合中
    for(int i = 0;i < sub_text.size();i++){
        trans = sub_text.get(i);
        if (cleanToken(trans)!=""){// 如果清理后字符串不为空字符串，则计入集合
            tokens.add(cleanToken(trans));
        }
    }
    return tokens;
}


// 从数据库文件读取内容，并将其处理为反向索引的形式
//
int buildIndex(string dbfile, Map<string, Set<string>>& index){
    // 从文件中读取数据
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    // 数据存入向量lines
    Vector<string> lines;
    readEntireFile(in, lines);
    // 对于每一个网页，先调用 gatherTokens 提取出网页文本中独一无二的字符串集合
    int pages = 0;
        // 偶数行为网址，奇数行为网页数据
    for (int i  = 0; i < lines.size(); i+= 2) {
        // 取出网页数据content
        Set<string> content = gatherTokens(lines[i + 1]);
        // 根据每一个字符串，将该页面的 URL 添加到反向索引的条目中
        for (const string &website : content) { //只读取content中元素website,不会进行拷贝，也不会修改
            index[website].add(lines[i]);
        }
        pages++;
    }
    return pages;
}
// 测试
STUDENT_TEST("建立反向索引的时间") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    TIME_OPERATION(11,buildIndex("res/tiny.txt", index));
}

// 找到给定字符串对应的网页
//
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query){
    Vector<string> key_words;
    Set<string> result;
    // 拆分复合搜索关键词
    key_words = stringSplit(query," ");
    for (const string &word : key_words) { //只读取key_words中元素word,不会进行拷贝，也不会修改
        // 使用+连接，交集
        if (startsWith(word, "+") != 0) {// 如果关键词以+开头，返回true
            result.intersect(index[cleanToken(word)]);// 保留result与index交集的结果
        }
        // 使用-连接，搜索结果将会从前面关键词的匹配项中，去除 - 号后面关键词的匹配项
        else if (startsWith(word, "-") != 0) {// 如果关键词以-开头，返回true
            result.difference(index[cleanToken(word)]);// 保留result与index差异的结果
        }
        // 没有使用修饰符连接，并集
        else {
            result.unionWith(index[cleanToken(word)]);// 保留result与index合并的结果
        }
    }
    return result;
}
// 测试
STUDENT_TEST("查询不区分大小写") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "grEEn");
    EXPECT_EQUAL(matchesRedOrFish.size(), 1);
}
// 实现搜索引擎
//
void searchEngine(string dbfile){
    Map<string, Set<string>> index;
    // 首先根据数据库文件内容，构建一个反向索引
    int num_pages = buildIndex(dbfile, index);
    int num_words = index.size();
    // 打印出构建反向索引用到的网页数量以及所有页面中包含了多少独一无二的单词
    cout << "用到的网页数量： " << num_pages << endl;
    cout << "独一无二的单词数量： " << num_words << endl;

    // 进入一个循环，提示用户输入查询关键词
    while (true) {
        string terms = getLine("输入查询关键词(回车退出)：");
        // 当用户输入空字符串时，提示查询完成，程序结束。
        if (terms.empty()) {
            break;
        }
        // 对于每一个查询关键词，找出所有匹配页面并打印出 URLs
        Set<string> matches = findQueryMatches(index, terms);
        cout << " 找到匹配页面："<< matches.size() << "个"<< endl;
        cout << matches << endl;
        cout << endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
