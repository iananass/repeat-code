/* 
 * File:   main.cpp
 * Author: iananass
 *
 * Created on November 26, 2013, 9:41 PM
 */

#include <assert.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <thread>
#include <vector>

class RepeatCode {
    int m_r; // repeat Number
    int m_k; // code len
    std::vector<int> m_code;
public:

    RepeatCode(int K, int R)
    : m_r(R)
    , m_k(K) {
        m_code.resize(m_k, 0);
    }

    RepeatCode(const RepeatCode& c)
    : m_r(c.m_r)
    , m_k(c.m_k) {
        m_code = c.m_code;
    }

    int Len() const {
        return m_r * m_k;
    }

    int SetBit(int offset) {
        ++m_code[offset % m_k];
        if (m_code[offset % m_k] > m_r / 2)
            return 1;
        return 0;
    }

    int UnsetBit(int offset) {
        --m_code[offset % m_k];
    }
};

void Combinate(RepeatCode& code, int currentPos, int numReverseBits, u_int64_t* res) {
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

u_int64_t mtCombinate(RepeatCode& code, int numReverseBits) {
    std::vector<std::thread> threads;
    std::vector<RepeatCode> codes;
    std::vector<u_int64_t> results;
    results.resize(code.Len() - numReverseBits + 1, 0);

    for (int i = 0; i < code.Len() - numReverseBits + 1; ++i) {
        codes.push_back(code);
        codes[i].SetBit(i);
        threads.push_back(std::thread(Combinate, codes[i], i + 1, numReverseBits - 1, &results[i]));
    }


    u_int64_t res = 0;
    for (int i = 0; i < code.Len() - numReverseBits + 1; ++i) {
        threads[i].join();
        res += results[i];
    }
    return res;

}

u_int64_t binomial(int n, int k) {
    double dividend = 1; // Делимое = k * (k-1) * ... * (k - (n-1))
    for (int i = 0; i < n; ++i)
        dividend *= k - i;

    double divisor = 1; // Делитель =  n!
    for (int i = 1; i <= n; ++i)
        divisor *= i;
    return dividend / divisor;
}

struct entry {
    int m;
    u_int64_t correct;
    u_int64_t total;
};

void PrintDestribution(const std::vector<entry>& results, int k, int r) {
    std::cout << "k = " << k << "   r = " << r << std::endl << std::endl;
    std::cout << std::setw(7) << "M" << std::setw(15) << "correct, %" << std::setw(15) << "incorrect, %" << std::endl;
    std::cout << std::setw(7) << r / 2 << std::setw(15) << "all" << std::setw(15) << "0" << std::endl;
    std::cout.precision(6);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    for (std::vector<entry>::const_iterator it = results.begin(), ie = results.end(); it != ie; ++it) {
        std::cout << std::setw(7) << it->m << std::setw(15) << double(it->correct) / it->total << std::setw(15) << double(it->total - it->correct) / it->total << std::endl;
    }
    std::cout << std::setw(7) << k * (r / 2) + 1 << std::setw(15) << "0" << std::setw(15) << "all" << std::endl;
}

void CalcDestribution(std::vector<entry>& results, int k, int r) {
    std::cout << "k = " << k << "   r = " << r << std::endl << std::endl;
    std::cout << std::setw(7) << "M" << std::setw(15) << "correct, %" << std::setw(15) << "incorrect, %" << std::endl;
    std::cout << std::setw(7) << r / 2 << std::setw(15) << "100" << std::setw(15) << "0" << std::endl;
    std::cout.precision(6);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    for (int i = r / 2 + 1; i < k * (r / 2) + 1; ++i) {
        RepeatCode code(k, r);
        u_int64_t total = binomial(i, r * k);
        u_int64_t correct = 0;
        correct = mtCombinate(code, i);
        std::cout << std::setw(7) << i << std::setw(15) << double(correct) / total << std::setw(15) << double(total - correct) / total << std::endl;
        results.push_back({i, correct, total});
    }
    std::cout << std::setw(7) << k * (r / 2) + 1 << std::setw(15) << "0" << std::setw(15) << "100" << std::endl;
}

void load(std::ifstream& is, std::vector<entry>& vc) {
    vc.clear();
    while (!is.eof()) {
        int m;
        u_int64_t correct;
        u_int64_t total;
        is >> m >> correct >> total;
        vc.push_back({m, correct, total});
    }
}

void save(std::ofstream& is, const std::vector<entry>& vc) {
    if (vc.size() == 0)
        return;
    is << std::setw(7) << vc[0].m << std::setw(15) << vc[0].correct << std::setw(15) << vc[0].total;

    for (std::vector<entry>::const_iterator it = vc.begin() + 1, ie = vc.end(); it != ie; ++it) {
        is << std::endl << std::setw(7) << it->m << std::setw(15) << it->correct << std::setw(15) << it->total;
    }
}

int main(int argc, char** argv) {
    int k = 0, r = 0;
    if (argc == 3) {
        k = atoi(argv[1]);
        r = atoi(argv[2]);
    } else {
        while (!(k > 0)) {
            std::cout << "Input k (greater than 0): ";
            std::cin >> k;
        }

        while (!(r > 1 && r & 1)) {
            std::cout << "Input r (odd, greater than 1): ";
            std::cin >> r;
        }
    }

    std::vector<entry> results;
    std::stringstream strm;
    strm << "cache/" << k << '-' << r << ".cache";
    std::ifstream file(strm.str());
    if (file.is_open()) {
        load(file, results);
        PrintDestribution(results, k, r);
        return 0;
    }
    std::ofstream out(strm.str());
    CalcDestribution(results, k, r);
    save(out, results);



    return 0;
}