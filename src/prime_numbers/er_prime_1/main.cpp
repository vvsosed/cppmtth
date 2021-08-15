#include "config.h"

#include <iostream>
#include <optional>
#include <tuple>
#include <cstdlib>
#include <cstring>
#include <vector>

using NumType = unsigned int;

struct InputArgs {
    std::optional<NumType> m_begin, m_end;
    std::optional<std::string> m_outFile;

    void validate() const {
        bool isFailed{false};
        if (m_begin && m_end) {
            if ( *m_end < *m_begin){
                std::cout << "begin should be >= than end" << std::endl;
                isFailed = true;
            }
        }
        else {
            if(!m_begin) {
                std::cout << "Missing begin=<value>" << std::endl;
                isFailed = true;
            }

            if(!m_end) {
                std::cout << "Missing end=<value>" << std::endl;
                isFailed = true;
            }
        }

        if (m_begin <= 3) {
            std::cout << "begin should be > 3" << std::endl;
            isFailed = true;
        }

        if(m_outFile && m_outFile->empty()){
            std::cout << "Wrong out_file parameter" << std::endl;
            isFailed = true;
        }

        if(isFailed){
            std::exit(-1);
        }
    }
};

InputArgs parseCallArgs(const int argc, const char* argv[]) {
    InputArgs args;

    if(0 == argc){
        std::exit(-1);
    }

    for(int pos = 1; pos < argc; ++pos) {
        std::string argStr{argv[pos]};

        if ("--version" == argStr) {
            std::cout << PROJECT_NAME << ":er_prime_1 " << PROJECT_VER << std::endl;
            std::exit(0);
        }
        else if ("--help" == argStr) {
            std::cout << "#TODO print help there" << std::endl;
            std::exit(0);
        }

        auto dlPos = argStr.find('=');
        auto key = argStr.substr(0, dlPos);
        auto valStr = argStr.substr(dlPos + 1);
        if ("begin" == key) {
            args.m_begin = std::stoul(valStr);
        }
        else if ("end" == key) {
            args.m_end = std::stoul(valStr);
        }
        else if("out_file" == key){
            args.m_outFile = valStr;
        }
    }

    args.validate();

    return args;
}

bool checkPrime(const NumType _evenNum) {
    for(auto num = 3; num < _evenNum; num += 2){
        auto ost = _evenNum % num;
        if(0 == ost){
            return false;
        }
    }
    return true;
}

class PrimeVector {
public:
    PrimeVector( const NumType _begin, const NumType _end )
    : m_begin{_begin}
    , m_end{_end}
    , m_count{(_end - _begin) / 2}
    , m_IsPrime(m_count, true) {}

    PrimeVector(PrimeVector&&) = default;

    void setPrime( const std::size_t pos, const bool isPrime) {
        if (pos & 0x1) {
            const auto idx = getIndex(pos);
            m_IsPrime[idx] = isPrime;
        }
    }

    bool isPrime(const std::size_t pos) const {
        if (pos & 0x1) {
            const auto idx = getIndex(pos);
            return m_IsPrime[idx];
        }

        return false;
    }

    inline NumType begin() const {
        return m_begin;
    }

    inline NumType end() const {
        return m_end;
    }

private:
    inline std::size_t getIndex( const std::size_t pos ) const {
        return (pos - m_begin) / 2;
    }

private:
    const NumType m_begin, m_end, m_count;
    std::vector<bool> m_IsPrime;
};

PrimeVector primeSieve( const NumType begin, const NumType end) {
    PrimeVector primeVec{begin, end};
    auto numBeg = begin & 0x1 ? begin : begin + 1;
    for (NumType num = numBeg; num <= end; num += 2) {
        for (NumType i = 3; i <= end; i += 2) {
            if (i < begin && !checkPrime(i)) {
                std::cout << "skip i=" << i << std::endl;
                continue;
            }
            auto val = i + i;
            std::cout << "start sieve val=" << val << std::endl;
            while (val < begin) {
                std::cout << "skip val=" << val << std::endl;
                val += i;
            }
            while (val <= end) {
                std::cout << "Set not prime val=" << val << std::endl;
                primeVec.setPrime(val, false);
                val += i;
            }
        }
    } 
    std::cout << std::endl;

    return primeVec;
}

int main(const int argc, const char* argv[]) {
    auto inpArgs = parseCallArgs(argc, argv);
    
    auto res = primeSieve(*inpArgs.m_begin, *inpArgs.m_end);   
    for(auto v = res.begin(); v <= res.end(); ++v) {
        if(res.isPrime(v)){
            std::cout << v << ",";
        }
    }
    std::cout << std::endl;

    return 0;
}
