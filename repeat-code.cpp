#include <assert.h>
#include <cstdlib>
#include <inttypes.h>
#include <iomanip>
#include <iostream>
#include <vector>

class RepeatCode
{
    u_int64_t m_r; // Количество повторений
    u_int64_t m_k; // Длина кода
    std::vector<u_int64_t> m_code; // Вектор длины m_k для хранения бит ошибок

    // Расставляет numReverseBits бит ошибок в промежутке от currentPos до конца
    // Записывает в numSuccess кол-во исправленных ВЕРНО ошибок

    void Combinate(u_int64_t currentPos, u_int64_t numReverseBits, u_int64_t& numSuccess)
    {
        if (numReverseBits > Len() - currentPos)
            return;
        if (!numReverseBits) {
            ++numSuccess;
            return;
        }
        for (u_int64_t i = currentPos; i < Len(); ++i) {
            if (SetBit(i) == 0)
                Combinate(i + 1, numReverseBits - 1, numSuccess);
            UnsetBit(i);
        }
    }

    // Длина всей последовательности

    u_int64_t Len() const
    {
        return m_r * m_k;
    }

    // Записывает ошибку в указанную позицию
    // Возвращает 1, если ошибка будет исправлена НЕверно, иначе - 0

    u_int64_t SetBit(u_int64_t offset)
    {
        ++m_code[offset % m_k];
        if (m_code[offset % m_k] > m_r / 2)
            return 1;
        return 0;
    }

    // Стирает ошибку из указанной позиции

    void UnsetBit(u_int64_t offset)
    {
        assert(m_code[offset % m_k]);
        --m_code[offset % m_k];
    }


public:

    // При создании передаем R, k

    RepeatCode(u_int64_t K, u_int64_t R)
    : m_r(R)
    , m_k(K)
    {
        m_code.resize(m_k, 0);
    }

    // Расставляет M бит по всей комбинации
    // Возвращает общее количество ВЕРНО исправленных ошибок

    u_int64_t CalcFixedErrors(u_int64_t M)
    {
        u_int64_t successes = 0;
        Combinate(0, M, successes);
        return successes;
    }
};

// Рассчет количества возможных комбинаций (!) по сокращенной формуле (!) 
// Иначе не влезем в размерность Unsigned long long

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
    while (!(k > 0)) {
        std::cout << "Input k (greater than 0): ";
        std::cin >> k;
    }

    while (!(r > 1 && r & 1)) {
        std::cout << "Input r (odd, greater than 1): ";
        std::cin >> r;
    }

    std::cout << "k = " << k << "   r = " << r << std::endl << std::endl << std::endl;
    std::cout << std::setw(7) << "M" << std::setw(15) << "correct, %" << std::setw(15) << "incorrect, %" << std::endl;
    std::cout << std::setw(7) << r / 2 << std::setw(15) << "100" << std::setw(15) << "0" << std::endl;
    for (int i = r / 2 + 1; i < k * (r / 2) + 1; ++i) {
        RepeatCode code(k, r);
        u_int32_t total = binomial(i, r * k);
        u_int32_t correct = code.CalcFixedErrors(i);
        std::cout << std::setw(7) << i << std::setw(15) << double(correct) / total * 100 << std::setw(15) << double(total - correct) / total * 100 << std::endl;
    }
    std::cout << std::setw(7) << k * (r / 2) + 1 << std::setw(15) << "0" << std::setw(15) << "100" << std::endl;
    return 0;
}

