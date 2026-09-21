#pragma once
#include <stdint.h>

// 2相エンコーダを A/B 両エッジの割り込みで 4 逓倍カウントする
namespace encoder {
void    begin(uint8_t pinA, uint8_t pinB);
int32_t count();
}
