/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    if (n >= 0) {
        return (n != 0) && (n == divisorSum(n));
    }
    else {
        return false;
    }
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long smarterSum(long n) {
    if (n == 1){    // 判断1
        return 0;
    }
    long total = 1;// 除去1之外的数
    for (long divisor = 2; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total = total  + divisor + n/divisor ;
        }
        if (divisor * divisor == n){
            total -= divisor;
        }
    }
    return total;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isPerfectSmarter(long n) {
    if (n >= 0) {
        return (n != 0) && (n == smarterSum(n));
    }
    else {
        return false;
    }
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {
    long m = 0;
    for (long k = 1; k < LONG_MAX; k++) {
         m = pow(2, k) - 1;
        if (smarterSum(m) == 1){    // 是素数
            if (--n == 0)           // ???
                return pow(2, k - 1) * m;
            }
       }
        return 0;
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here

// 添加 4 个TIME_OPERATION 测试案例。
// 要求是，每个测试案例的数据量翻倍，
// 并保证最后一个测试案例的执行时间在一分钟左右。
STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(20000, findPerfects(20000));
   TIME_OPERATION(80000, findPerfects(80000));
   TIME_OPERATION(240000, findPerfects(240000));
   TIME_OPERATION(320000, findPerfects(320000));
}


// 添加一个新的 STUDENT_TEST 测试案例，
// 用不同的负值调用 isPerfect 函数。
// 期望的 isPerfect 返回值应该为 false。
STUDENT_TEST("Test minus: return 'false' ") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-3));
}

// 测试smarterSum函数
STUDENT_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
    EXPECT_EQUAL(smarterSum(4), divisorSum(4));
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
    EXPECT_EQUAL(smarterSum(9), divisorSum(9));
    EXPECT_EQUAL(smarterSum(16), divisorSum(16));
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
}

// 测试findperfectsSmarterSum函数
STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(240000, findPerfectsSmarter(240000));
    TIME_OPERATION(320000, findPerfectsSmarter(320000));
    TIME_OPERATION(64000000, findPerfectsSmarter(6400000));
}

// 测试findNthPerfectEuclid函数
STUDENT_TEST("Confirm findNthPerfectEuclid of inputs") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), (6));
    EXPECT_EQUAL(findNthPerfectEuclid(2), (28));
    EXPECT_EQUAL(findNthPerfectEuclid(3), (496));
    EXPECT_EQUAL(findNthPerfectEuclid(4), (8128));
    EXPECT_EQUAL(findNthPerfectEuclid(5), (33550336));
}
