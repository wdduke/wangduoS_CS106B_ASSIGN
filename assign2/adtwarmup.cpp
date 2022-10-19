#include "testing/SimpleTest.h" // 测试框架
#include "set.h"                // 集
#include "queue.h"              // 队列
#include "stack.h"              // 栈
using namespace std;            // 明确命名空间

/* This function correctly reverses the elements of a queue.
 */
void reverse(Queue<int>& q ) {
    Stack<int> s;// 局部变量没有初始值
    int val;

    while (!q.isEmpty()) {
        val = q.dequeue();      // 出队
        s.push(val);            // 入栈
    }
    while (!s.isEmpty()) {
        val = s.pop();          // 出栈
        q.enqueue(val);         // 入队
    }
}

// minimum TEST :reverse queue
STUDENT_TEST("reverse queue") {
    Queue<int> q = {1};
    Queue<int> expected = {1};

    reverse(q);
    EXPECT_EQUAL(q, expected);
}

/* This function is intended to modify a queue of characters to duplicate
 * any negative numbers.
 *
 * WARNING: the given code is buggy. See exercise writeup for more
 * information on how to test and diagnose.
 */
void duplicateNegatives(Queue<int>& q) {
    // 计算初始队列中的负数个数
    int num = 0;
    int length = 0;
    for (int j = 0; j < q.size(); j++){
        int samp = q.dequeue();
        q.enqueue(samp);
        if (samp < 0){
            num += 1;
        }
    }
    // 计算重复负数后队列长度
     length = q.size() + num;
     // 无负数队列
     if (num == 0){
         for (int i = 0; i <= length - 1; i++) {
             int val = q.dequeue();
             q.enqueue(val);
         }
     }
     // 有负数队列
        // 有1个负数
     if (num == 1){
        for (int i = 0; i <= length - 2; i++) {
            int val = q.dequeue();
            q.enqueue(val);
            if (val < 0) {
                q.enqueue(val);   // double up on negative numbers
            }
        }
     }
        // 有n个负数
    if (num != 1 && num !=0){
        for (int i = num; i < length; i++) {
            int val = q.dequeue();
            q.enqueue(val);
            if (val < 0) {
                q.enqueue(val);   // double up on negative numbers
            }
        }
    }
}


// minimum TEST:input has  ONLY ONE negatives
STUDENT_TEST("input has ONLY ONE negatives") {
    Queue<int> q = {-1};
    Queue<int> expected = {-1, -1};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

// ACCURACY TEST:WHICH negatives MAKE THE BUG?
STUDENT_TEST("START NEGATIVE") {
    // START NEGATIVE
    Queue<int> q = {-1, 2, 3};
    Queue<int> expected = {-1, -1, 2, 3};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}
STUDENT_TEST("MIDDLE NEGATIVE") {
    // MIDDLE NEGATIVE
    Queue<int> q = {1, -2, 3};
    Queue<int> expected = {1, -2, -2, 3};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}
STUDENT_TEST("END NEGATIVE") {
    // END NEGATIVE
    Queue<int> q = {1, 2, -3};
    Queue<int> expected = {1, 2, -3, -3};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

// This function is intended to return the sum of all values in
// the stack
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
int sumStack(Stack<int> s) {
    if (s.isEmpty()) {// 不改变原始状态，不操作返回0
      return 0;
    }
    //s.push(0);// 加一个0给栈，避免为空栈，且不影响全体求和
    int total = s.pop();
    while (!s.isEmpty()) {
        total += s.pop();
    }
    return total;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> q = {1, 2, 3, 4, 5};
    Queue<int> expected = {5, 4, 3, 2, 1};

    reverse(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> q = {2, 10};
    Queue<int> expected = q;

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has one negative, one positive") {
    Queue<int> q = {-7, 9};
    Queue<int> expected = {-7, -7, 9};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("duplicateNegatives, input has negatives interspersed with positives") {
    Queue<int> q = {-3, 4, -5, 10};
    Queue<int> expected = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(q);
    EXPECT_EQUAL(q, expected);
}

PROVIDED_TEST("sumStack, 3 elements") {
    Stack<int> s = {1, 8, -5};

    EXPECT_EQUAL(sumStack(s), 4);
}

PROVIDED_TEST("sumStack, empty stack") {
    Stack<int> empty;

    EXPECT_EQUAL(sumStack(empty), 0);
}
