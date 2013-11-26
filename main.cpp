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

void Combinate(std::vector<int>& code, int currentPos, int numReverseBits)
{
    if (numReverseBits > code.size() - currentPos)
        return;
    if (!numReverseBits) {
        std::cout << std::setw(2) << N++ << "   ";
        for (int i = 0; i < code.size(); ++i)
            std::cout << code[i] << " ";
        std::cout << std::endl;
        return;
    }
    for (int i = currentPos; i < code.size(); ++i) {
        code[i] = 1;
        Combinate(code, i + 1, numReverseBits - 1);
        code[i] = 0;
    }
}

int main(int argc, char** argv)
{
    std::vector<int> code;
    code.resize(10, 0);
    Combinate(code, 0, 3);
    return 0;
}

