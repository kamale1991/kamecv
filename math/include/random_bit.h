#include <iostream>

namespace math
{
	namespace random
	{
		class RandomBit
		{
		private:
			/* data */
			int IB1 = 1;
			int IB2 = 2;
			int IB5 = 16;
			int IB18 = 131072;
			int MASK = IB1 + IB2 + IB5;

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
			int irbit(uint32_t &iseed);

			// ハードウェアで実現するためのアルゴリズムの再現
			int irbit_register_representation(uint32_t &iseed);
		};

	} // namespace random

} // namespace math