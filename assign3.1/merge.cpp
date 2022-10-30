/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 *判断队列是否是按顺序的
 *
 */
bool order(Queue<int> a){
    int temp = a.dequeue();// 取出队首
    bool result;
    if (a.isEmpty()){error("queue is empty!");}// 空值报错
    while (a.size()>0){
        if(a.peek() > temp){// 若后一个元素大于前一个元素，返回真
            result = true;
            temp = a.dequeue();
        }else{// 若存在后一个元素小于前一个元素，返回假，退出循环，输出结果
            result = false;
            break;
        }
    }
    return result;
}
// test
STUDENT_TEST("order 正确顺序") {
    Queue<int> a = {1, 2, 3};
    EXPECT_EQUAL(order(a), true);
}
STUDENT_TEST("order 错误顺序1") {
    Queue<int> a = {3, 2, 1, 5, 4, 6};
    EXPECT_EQUAL(order(a), false);
}
STUDENT_TEST("order 错误顺序2") {
    Queue<int> a = {1, 2, 3, 5, 4, 6};
    EXPECT_EQUAL(order(a), false);
}
STUDENT_TEST("order 错误顺序3") {
    Queue<int> a = {1, 1, 1, 5, 4, 6};
    EXPECT_EQUAL(order(a), false);
}
STUDENT_TEST("order 空队列") {
    Queue<int> a = {};
    EXPECT_ERROR(order(a));
}
STUDENT_TEST("order 常数队列") {
    Queue<int> a = {1, 1, 1, 1};
    //EXPECT_ERROR(order(a));
    EXPECT_EQUAL(order(a), false);
}
/*
 *将两个独立的有序序列合并成一个更大的有序序列
 */
#define v_2
#ifdef v_2
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int temp;
    int temp_c = 0;
    while (a.size()>0 && b.size()>0){
        // 取出前两个序列比较、排序，选出较小的
        if (a.peek()  < b.peek()){
            temp = a.dequeue();
        }else{
            temp = b.dequeue();
        }
       if (!result.isEmpty() && temp_c > temp){
            error("order error");
           }
       result.enqueue(temp);
       temp_c = temp;// 输出的队尾
    }
    // 若a队列还有剩余
    while (a.size()>0){
       // 取出队首
       temp = a.dequeue();
       if (!result.isEmpty() && temp_c > temp) {
           error("order error");
       }
       result.enqueue(temp);
       temp_c = temp;

    }
    // 若b队列还有剩余
    while (b.size()>0){
       // 取出队首
       temp = b.dequeue();
       if (!result.isEmpty() && temp_c > temp) {
           error("order error");
       }
       result.enqueue(temp);
       temp_c = temp;
    }
    return result;
}
#endif

// test
STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5, 6, 7, 8};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5, 6, 7, 8};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, two short sequences disorder") {
    Queue<int> a = {2, 4, 5, 6, 8, 7, 9, 10};
    Queue<int> b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}
STUDENT_TEST("binaryMerge 两个全为空") {
    Queue<int> a = {};
    Queue<int> b = {};
    EXPECT_EQUAL(binaryMerge(a, b), {});
    EXPECT_EQUAL(binaryMerge(b, a), {});
}

STUDENT_TEST("binaryMerge 其中一个为空") {
    Queue<int> a = {};
    Queue<int> b = {1};
    EXPECT_EQUAL(binaryMerge(a, b), {1});
    EXPECT_EQUAL(binaryMerge(b, a), {1});
}

STUDENT_TEST("binaryMerge 输入没有排序") {
    Queue<int> a = {2, 1, 5}; // 等长
    Queue<int> b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    a = {2, 3, 5, 6, 2}; // 不等长
    b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("binaryMerge 一个为空且没有排序") {
    Queue<int> a = {};
    Queue<int> b = {1, 5, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * 应用递归分而治之的力量来实现一个更有效的多路合并的变体
 */
#define v2
#ifdef v1
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    // 将包含 k 个队列的向量分成两半。左半部分是向量中的前一个 k/2 序列，右半部分是剩余的序列。
    Vector<Queue<int>> left = all.subList(0, all.size()/2);
    Vector<Queue<int>> right = all.subList(all.size()/2) ;
    Queue<int> l_re;
    Queue<int> r_re;
    // 递归调用 recMultiMerge 处理左半部分，可以得到一个排好序的子序列。用同样的逻辑处理右半部分，又可以得到一个排好序的子序列
    // base case
    for (Queue<int>& q : left) {
        l_re = binaryMerge(q, l_re);
    }
    for (Queue<int>& q : right) {
        r_re = binaryMerge(q, r_re);
    }

    // 最后使用函数 binaryMerge 合并上述两个子序列，并将结果返回
    result = binaryMerge(l_re, r_re);

    return result;
}
#endif


#ifdef v2
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    if (all.isEmpty()){
        error("no queue");
    }else{
        // base case 若为单元素队列，直接返回，不必归并
        if (all.size() == 1 ){
            return all[0];
        // crusive 否则，一分为二，前后两个子数组，直至剩一个元素的队列，再用二路归并返回
        }else{
            int mid = all.size()/2;
            Vector<Queue<int>> left = all.subList(0, mid);
            Vector<Queue<int>> right = all.subList(mid) ;
            return binaryMerge(recMultiMerge(left),recMultiMerge(right));
        }
    }
}
#endif

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

// test
STUDENT_TEST(" NOUGHT recMultiMerge") {
    int n = 0;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_ERROR(recMultiMerge(all));
}
STUDENT_TEST(" ONLY ONE recMultiMerge, compare to naiveMultiMerge") {
    int n = 1;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}
STUDENT_TEST(" ODD recMultiMerge, compare to naiveMultiMerge") {
    int n = 23;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}
STUDENT_TEST(" EVEN recMultiMerge, compare to naiveMultiMerge") {
    int n = 10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}
// ********************************************* //
PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}
// 1e8 22.9s;9e8 204.1s
//STUDENT_TEST("Time binaryMerge operation") {
 //   int n = 900000000;
  //  Queue<int> a = createSequence(n);
  //  Queue<int> b = createSequence(n);
  //  TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
//}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
