/* 
 * File:   main.cpp
 * Author: iananass
 *
 * Created on November 26, 2013, 9:41 PM
 */

#include <cstdlib>
#include <vector>
#include <iomanip>
#include <iostream>
#include <assert.h>
#include <thread>
#include <pthread.h>

int N = 0;

class RepeatCode
{
    int m_r; // repeat Number
    int m_k; // code len
    std::vector<int> m_code;
public:

    RepeatCode(int K, int R)
    : m_r(R)
    , m_k(K)
    {
        m_code.resize(m_k, 0);
    }

    RepeatCode(const RepeatCode& c)
    : m_r(c.m_r)
    , m_k(c.m_k)
    {
        m_code = c.m_code;
    }

    int Len() const
    {
        return m_r * m_k;
    }

    int SetBit(int offset)
    {
        ++m_code[offset % m_k];
        if (m_code[offset % m_k] > m_r / 2)
            return 1;
        return 0;
    }

    int UnsetBit(int offset)
    {
        --m_code[offset % m_k];
    }
};

void Combinate(RepeatCode& code, int currentPos, int numReverseBits, u_int32_t* res)
{
    if (numReverseBits > code.Len() - currentPos) {
        assert(0);
        return;
    }
    if (!numReverseBits) {
        *res += 1;
        return;
    }
    for (int i = currentPos; i < code.Len() - numReverseBits + 1; ++i) {
        if (code.SetBit(i) == 0)
            Combinate(code, i + 1, numReverseBits - 1, res);
        code.UnsetBit(i);
    }
    return;
}

u_int32_t mtCombinate(RepeatCode& code, int numReverseBits)
{
    std::vector<std::thread> threads;
    std::vector<RepeatCode> codes;
    std::vector<u_int32_t> results;
    results.resize(code.Len() - numReverseBits + 1, 0);

    for (int i = 0; i < code.Len() - numReverseBits + 1; ++i) {
        codes.push_back(code);
        codes[i].SetBit(i);
        threads.push_back(std::thread(Combinate, codes[i], i + 1, numReverseBits - 1, &results[i]));
    }


    u_int32_t res = 0;
    for (int i = 0; i < code.Len() - numReverseBits + 1; ++i) {
        threads[i].join();
        res += results[i];
    }
    return res;

}

u_int64_t binomial(int n, int k)
{
    u_int64_t dividend = 1; // Делимое = k * (k-1) * ... * (k - (n-1))
    for (int i = 0; i < n; ++i)
        dividend *= k - i;

    u_int64_t divisor = 1; // Делитель =  n!
    for (int i = 1; i <= n; ++i)
        divisor *= i;
    return dividend / divisor;
}

int main(int argc, char** argv)
{
    int k = 0, r = 0;
    if (argc == 3) {
        k = atoi(argv[1]);
        r = atoi(argv[2]);
    }
    else {
        while (!(k > 0)) {
            std::cout << "Input k (greater than 0): ";
            std::cin >> k;
        }

        while (!(r > 1 && r & 1)) {
            std::cout << "Input r (odd, greater than 1): ";
            std::cin >> r;
        }
    }

    RepeatCode code(k, r);

    std::cout << "k = " << k << "   r = " << r << std::endl << std::endl;
    std::cout << std::setw(7) << "M" << std::setw(15) << "correct" << std::setw(15) << "incorrect" << std::endl;
    std::cout << std::setw(7) << r / 2 << std::setw(15) << "all" << std::setw(15) << "0" << std::endl;
    for (int i = r / 2 + 1; i < k * (r / 2) + 1; ++i) {
        RepeatCode code(k, r);
        u_int32_t total = binomial(i, r * k);
        u_int32_t correct = 0;
        //Combinate(code, 0, i, &correct);
        correct = mtCombinate(code, i);
        std::cout << std::setw(7) << i << std::setw(15) << (correct) << std::setw(15) << (total - correct) << std::setw(15) << total << std::endl;
    }
    std::cout << std::setw(7) << k * (r / 2) + 1 << std::setw(15) << "0" << std::setw(15) << "all" << std::endl;
    return 0;
}