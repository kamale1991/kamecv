#include <iostream>

namespace math
{
	namespace random
	{
		// Numerical Recipe in Cの実装
		class RandomBit
		{
		private:
			/* data */
			uint32_t IB1 = 1;
			uint32_t IB2 = 2;
			uint32_t IB5 = 16;
			uint32_t IB18 = 131072;
			uint32_t MASK = IB1 + IB2 + IB5;

		public:
			// 必要なインターフェースは？
			// 簡単のためにタップ列は固定にする
			// どの数値型にキャストするか選べるようにする？
			// シード値は32ビット固定
			// 出力はintにする
			// 将来、周期を変えたくなるかもしれないのでそのときはタップ列を選択できるようにする
			RandomBit(/* args */);
			~RandomBit();
			// 1ワードの処理で次の乱数を生成する
			uint32_t irbit(uint32_t &iseed);

			// ハードウェアで実現するためのアルゴリズムの再現
			uint32_t irbit_register_representation(uint32_t &iseed);
		};

		class RnadomBit32
		{
		private:
			uint32_t IB1 = 1;
			uint32_t IB2 = 0x00000001;	// 2^1
			uint32_t IB3 = 0x00000004;	// 2^2
			uint32_t IB5 = 0x00000010;	// 2^4
			uint32_t IB7 = 0x00000040;	// 2^6
			uint32_t IB32 = 0x80000000; // 2^31

		public:
			uint32_t irbit(uint32_t &iseed);
		};

		// Wikipediaの実装
		class RandomBit16
		{
		private:
			uint16_t IB1 = 1;
			uint16_t IB2 = 0x0001;
			uint16_t IB3 = 0x0004;
			uint16_t IB5 = 0x0010;
			uint16_t IB16 = 0x8000;
			uint16_t MASK = IB2 + IB3 + IB5;

		public:
			uint16_t irbit(uint16_t &iseed);
			uint16_t irbit2(uint16_t &iseed);
		};

	} // namespace random

} // namespace math