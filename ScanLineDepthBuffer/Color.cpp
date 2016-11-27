#include <random>
#include "Color.h"


const Color Color::RED{255, 0, 0};
const Color Color::GREEN{0, 255, 0};
const Color Color::BLUE{0, 0, 255};
const Color Color::BLACK{0, 0, 0};
const Color Color::WHITE{255, 255, 255};

Color Color::RandomColor()
{
    std::mt19937 eng;
    eng.seed(std::random_device()());
    std::uniform_int_distribution<unsigned int> u(0, 255);
    return Color{ static_cast<UINT8>(u(eng)),
                  static_cast<UINT8>(u(eng)),
                  static_cast<UINT8>(u(eng)) };
}
