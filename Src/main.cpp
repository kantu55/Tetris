#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>	// Vector型のコピーに使う
#include <time.h>		// 

#define FIELD_WIDTH		(10 + 2)	// フィールドの幅
#define FIELD_HEIGHT	(20 + 1)	// フィールドの高さ
#define BLOCK_WIDTH_MAX		(4)	// ブロックの最大幅
#define BLOCK_HEIGHT_MAX	(4)	// ブロックの最大高さ
#define FPS	(1)	// 1秒あたりの描画頻度
#define INTERVAL	(1000 / FPS)	// 描画間隔（ミリ秒）

// 落下ブロックの種類を宣言
enum 
{
	BLOCK_SHAPE_I, // I型
	BLOCK_SHAPE_O, // O型
	BLOCK_SHAPE_S, // S型
	BLOCK_SHAPE_Z, // Z型
	BLOCK_SHAPE_J, // J型
	BLOCK_SHAPE_L, // L型
	BLOCK_SHAPE_MAX, // 落下ブロックの種類の数
};

// 落下ブロックの形状の構造体を宣言
struct BLOCKSHAPE
{
	int width, height;		// 幅と高さ
	std::vector<std::vector<int>> pattern{ {BLOCK_HEIGHT_MAX}, {BLOCK_WIDTH_MAX} };	// 形状
};

// 落下ブロックの構造体を宣言
struct BLOCK
{
	int x, y;			// 座標
	BLOCKSHAPE shape;	// 形状
};

// 落下ブロックの形状変数を宣言
const std::vector<BLOCKSHAPE> blockShapes
{
	// BLOCK_SHAPE_I
	{
		// int width, height;	幅と高さ
		4, 4,
		// const std::vector<std::vector<int>> pattern{ {BLOCK_HEIGHT_MAX}, {BLOCK_WIDTH_MAX} }; 形状
		{
			{ 0, 0, 0, 0},
			{ 1, 1, 1, 1},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},
	
	// BLOCK_SHAPE_O
	{
		2, 2,
		{
			{ 1, 1, 0, 0},
			{ 1, 1, 0, 0},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},

	// BLOCK_SHAPE_S
	{
		3, 3,
		{
			{ 0, 1, 1, 0},
			{ 1, 1, 0, 0},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},

	// BLOCK_SHAPE_Z
	{
		3, 3,
		{
			{ 1, 1, 0, 0},
			{ 0, 1, 1, 0},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},

	// BLOCK_SHAPE_J
	{
		3, 3,
		{
			{ 1, 0, 0, 0},
			{ 1, 1, 1, 0},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},

	// BLOCK_SHAPE_L
	{
		3, 3,
		{
			{ 0, 0, 1, 0},
			{ 1, 1, 1, 0},
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0},
		}
	},
};

// 合成バッファ変数を宣言
std::vector<std::vector<int>> combineBuffer(FIELD_HEIGHT, std::vector<int>(FIELD_WIDTH));

// フィールド変数を宣言
std::vector<std::vector<int>> field (FIELD_HEIGHT, std::vector<int>(FIELD_WIDTH));

// ブロック変数を宣言
BLOCK block;


// フィールドを描画する関数
void DrawField()
{
	// フィールドを合成バッファにコピー
	std::copy(field.begin(), field.end(), combineBuffer.begin());

	// 落下ブロックを合成バッファに書き込む
	for (int y = 0; y < block.shape.height; y++)
	{
		for (int x = 0; x < block.shape.width; x++)
		{
			if(block.shape.pattern.at(y).at(x) == 1)
			combineBuffer.at(block.y + y).at(block.x + x) |= block.shape.pattern.at(y).at(x);
		}
	}

	// 全てのマスを表示
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			if (combineBuffer.at(y).at(x) == 1)
			{
				std::cout << "■";
			}
			else
			{
				std::cout << "□";
			}
		}
		std::cout << std::endl; // 改行
	}
}

// 落下ブロックを生成する
void SpawnBlock()
{
	// 落下ブロックの形状をランダムな形状で初期化する
	block.shape = blockShapes.at(rand() % BLOCK_SHAPE_MAX);
	// 真上から生成
	block.y = 0;
	// 落下ブロックを中央に寄せる
	block.x = (FIELD_WIDTH - BLOCK_WIDTH_MAX) / 2;
}

// ゲームをリセットする関数
void Reset()
{
	// フィールドの左右に壁を作る
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		field.at(y).at(0) = 1;
		field.at(y).at(FIELD_WIDTH - 1) = 1;
		// フィールドの下に壁を作る
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			field.at(FIELD_HEIGHT-1).at(x) = 1;
		}
	}

	// ブロックを生成
	SpawnBlock();
}

/*
**	ブロックとフィールドの当たり判定をする関数
**
**	return true		当たった
**	return false	当たってない
*/
bool BlockIntersectField()
{
	// ブロックの全てのマスを確認
	for (int y = 0; y < block.shape.height; y++)
	{
		for (int x = 0; x < block.shape.width; x++)
		{
			// ブロックとフィールドの当たり判定を取る
			if (block.shape.pattern.at(y).at(x) == 1 &&
				field.at(block.y + y).at(block.x + x) == 1)
			{
				return true;
			}
		}
	}
	return false;
}

// プログラム開始
int main()
{
	// 乱数
	srand((unsigned int)time(NULL));

	// ゲームをリセット
	Reset();

	// フィールドの描画
	DrawField();

	// 前回の経過時間を宣言
	clock_t lastClock = clock();

	// メインループ
	while (1)
	{
		// 現在の時刻を宣言
		clock_t newClock = clock();

		if (newClock >= (lastClock + INTERVAL))
		{
			// ブロックの移動前のバックアップを宣言
			BLOCK lastBlock = block;

			block.y++;	// ブロックを落下させる

			// ブロックとフィールドが重なったかどうか判定する
			if (BlockIntersectField())
			{
				block = lastBlock; // ブロックを移動前の状態に戻す

				for (int y = 0; y < block.shape.height; y++)
				{
					for (int x = 0; x < block.shape.width; x++)
					{
						// ブロックをフィールドに書き込む
						if (block.shape.pattern.at(y).at(x) == 1)
						field.at(block.y + y).at(block.x + x) |= block.shape.pattern.at(y).at(x);
					}
				}
				// ブロックを生成する
				SpawnBlock();
			}

			// コンソール画面をクリアにする
			system("cls");

			// フィールドの描画
			DrawField();

			lastClock = newClock; // 前回の時間を現在の時間に更新する
		}
	}
	return 0;
}