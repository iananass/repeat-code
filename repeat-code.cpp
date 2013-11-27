#include <cstdlib>
#include <vector>
#include <iomanip>
#include <assert.h>
#include <iostream>
#include <inttypes.h>

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

    // В конструкторе передаем R, k

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
    RepeatCode code(5, 3);
    std::cout << code.CalcFixedErrors(5) << std::endl;
    std::cout << binomial(5,15);
    
    return 0;
}

