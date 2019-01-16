#include <iostream>
#include <cmath>
#include <random>

std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);

inline double rnd() {
    return dist(mt);
}

inline double f(double x) {
    return (x + 1) * std::cos(x);
}

// 確率密度関数
inline double p(double x) {
    return std::cos(x);
}

// 累積分布関数の逆関数
inline double Pinv(double x) {
    return std::asin(x);
}

int main() {
    const int N = 100000;

    double sum = 0;
    for(int i = 0; i < N; i++) {
        double u = rnd();
        double x = Pinv(u);
        sum += f(x) / p(x);
    }

    sum /= N;
    std::cout << sum << std::endl;
    
    return 0;
}