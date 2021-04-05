#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <conio.h>

#define FIELD_WIDTH		(10 + 2)	// �t�B�[���h�̕�
#define FIELD_HEIGHT	(20 + 1)	// �t�B�[���h�̍���
#define BLOCK_WIDTH_MAX		(4)	// �u���b�N�̍ő啝
#define BLOCK_HEIGHT_MAX	(4)	// �u���b�N�̍ő卂��
#define FPS	(2)	// 1�b������̕`��p�x
#define INTERVAL	(1000 / FPS)	// �`��Ԋu�i�~���b�j

// �����u���b�N�̎�ނ�錾
enum 
{
	BLOCK_SHAPE_I, // I�^
	BLOCK_SHAPE_O, // O�^
	BLOCK_SHAPE_S, // S�^
	BLOCK_SHAPE_Z, // Z�^
	BLOCK_SHAPE_J, // J�^
	BLOCK_SHAPE_L, // L�^
	BLOCK_SHAPE_MAX, // �����u���b�N�̎�ނ̐�
};

// �����u���b�N�̌`��̍\���̂�錾
struct BLOCKSHAPE
{
	int width, height;		// ���ƍ���
	std::vector<std::vector<int>> pattern{ {BLOCK_HEIGHT_MAX}, {BLOCK_WIDTH_MAX} };	// �`��
};

// �����u���b�N�̍\���̂�錾
struct BLOCK
{
	int x, y;			// ���W
	BLOCKSHAPE shape;	// �`��
};

// �����u���b�N�̌`��ϐ���錾
const std::vector<BLOCKSHAPE> blockShapes
{
	// BLOCK_SHAPE_I
	{
		4, 4,
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

// �����o�b�t�@�ϐ���錾
std::vector<std::vector<int>> combineBuffer(FIELD_HEIGHT, std::vector<int>(FIELD_WIDTH));

// �t�B�[���h�ϐ���錾
std::vector<std::vector<int>> field (FIELD_HEIGHT, std::vector<int>(FIELD_WIDTH));

// �u���b�N�ϐ���錾
BLOCK block;


// �t�B�[���h��`�悷��֐�
void DrawField()
{
	// �t�B�[���h�������o�b�t�@�ɃR�s�[
	// combineBuffer = field�ł���������
	std::copy(field.begin(), field.end(), combineBuffer.begin());

	// �����u���b�N�������o�b�t�@�ɏ�������
	for (int y = 0; y < block.shape.height; y++)
	{
		for (int x = 0; x < block.shape.width; x++)
		{
			if(block.shape.pattern.at(y).at(x) == 1)
			combineBuffer.at(block.y + y).at(block.x + x) |= block.shape.pattern.at(y).at(x);
		}
	}

	// �S�Ẵ}�X��\��
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			if (combineBuffer.at(y).at(x) == 1)
			{
				std::cout << "��";
			}
			else
			{
				std::cout << "�@";
			}
		}
		std::cout << std::endl; // ���s
	}
}

// �����u���b�N�𐶐�����
void SpawnBlock()
{
	// �����u���b�N�̌`��������_���Ȍ`��ŏ���������
	block.shape = blockShapes.at(rand() % BLOCK_SHAPE_MAX);
	// �^�ォ�琶��
	block.y = 0;
	// �����u���b�N�𒆉��Ɋ񂹂�
	block.x = (FIELD_WIDTH - BLOCK_WIDTH_MAX) / 2;
}

// �t�B�[���h�ɂ���u���b�N��S�č폜����
void ResetBlock()
{
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			field.at(y).at(x) = 0;
		}
	}
}

// �Q�[�������Z�b�g����֐�
void Reset()
{
	// �t�B�[���h���N���A
	ResetBlock();

	// �t�B�[���h�̍��E�ɕǂ����
	for (int y = 0; y < FIELD_HEIGHT; y++)
	{
		field.at(y).at(0) = 1;
		field.at(y).at(FIELD_WIDTH - 1) = 1;
		// �t�B�[���h�̉��ɕǂ����
		for (int x = 0; x < FIELD_WIDTH; x++)
		{
			field.at(FIELD_HEIGHT-1).at(x) = 1;
		}
	}

	// �u���b�N�𐶐�
	SpawnBlock();
}

/*
**	�u���b�N�ƃt�B�[���h�̓����蔻�������֐�
**
**	return true		��������
**	return false	�������ĂȂ�
*/
bool BlockIntersectField()
{
	// �u���b�N�̑S�Ẵ}�X���m�F
	for (int y = 0; y < block.shape.height; y++)
	{
		for (int x = 0; x < block.shape.width; x++)
		{
			// �u���b�N�ƃt�B�[���h�̓����蔻������
			if (block.shape.pattern.at(y).at(x) == 1 &&
				field.at(block.y + y).at(block.x + x) == 1)
			{
				return true;
			}
		}
	}
	return false;
}

// �u���b�N���폜����֐�
void EraseBlocks()
{
	for (int y = 0; y < FIELD_HEIGHT - 1; y++)
	{
		bool aligned = true;	// �u���b�N�����ɑ��������t���O
		for (int x = 1; x < FIELD_WIDTH - 1; x++)
		{
			// �Ώۂ̃}�X�Ƀu���b�N�����邩����
			if (field.at(y).at(x) == 0)
			{
				// �u���b�N���Ȃ���Α����ĂȂ��̂ŏI��
				aligned = false;
				break;
			}
		}
		// ���������`�F�b�N
		if (aligned)
		{
			for (int x = 1; x < FIELD_WIDTH - 1; x++)
			{
				// �����Ă��u���b�N���폜
				field.at(y).at(x) = 0;
			}

			// �������Ƃ��납�����`�F�b�N
			for (int y2 = y; y2 > 0; y2--)
			{
				for (int x = 1; x < FIELD_WIDTH - 1; x++)
				{
					// ��̃}�X�����̃}�X�ɑ��
					field.at(y2).at(x) = field.at(y2 - 1).at(x);
				}
			}

			for (int x = 1; x < FIELD_WIDTH - 1; x++)
			{
				// ��ԏ�̃u���b�N���폜
				field.at(0).at(x) = 0;
			}
		}
	}
}

// �v���O�����J�n
int main()
{
	// ����
	srand((unsigned int)time(NULL));

	// �Q�[�������Z�b�g
	Reset();

	// �t�B�[���h�̕`��
	DrawField();

	// �O��̌o�ߎ��Ԃ�錾
	clock_t lastClock = clock();

	// ���C�����[�v
	while (1)
	{
		// ���݂̎�����錾
		clock_t newClock = clock();

		if (newClock >= (lastClock + INTERVAL))
		{
			// �u���b�N�̈ړ��O�̃o�b�N�A�b�v��錾
			BLOCK lastBlock = block;

			block.y++;	// �u���b�N�𗎉�������

			// �u���b�N�ƃt�B�[���h���d�Ȃ������ǂ������肷��
			if (BlockIntersectField())
			{
				block = lastBlock; // �u���b�N���ړ��O�̏�Ԃɖ߂�

				for (int y = 0; y < block.shape.height; y++)
				{
					for (int x = 0; x < block.shape.width; x++)
					{
						// �u���b�N���t�B�[���h�ɏ�������
						if (block.shape.pattern.at(y).at(x) == 1)
						{
							field.at(block.y + y).at(block.x + x) |= block.shape.pattern.at(y).at(x);
						}
						EraseBlocks();
					}
				}
				// �u���b�N�𐶐�����
				SpawnBlock();

				// �����������_�Ńu���b�N����������Q�[���I��
				if (BlockIntersectField())
				{
					Reset();
				}
			}

			// �R���\�[����ʂ��N���A�ɂ���
			system("cls");

			// �t�B�[���h�̕`��
			DrawField();

			lastClock = newClock; // �O��̎��Ԃ����݂̎��ԂɍX�V����
		}

		// �L�[�{�[�h���͂�������������
		if (_kbhit())
		{
			BLOCK lastBlock = block;

			// ���͂��ꂽ�L�[�ɂ���ĕ���
			switch (_getch())
			{
				case 'w':
					break;
				// �E�ړ�
				case 's':
					block.y++;
					break;
				// ���ړ�
				case 'a':
					block.x--;
					break;
				// ���ړ�
				case 'd':
					block.x++;
					break;
				// w,a,s,d�ȊO�̃L�[�͉�]�i�����v���j
				default:
					{
						// ��]��̃u���b�N��錾
						BLOCK rotatedBlock = block;

						for (int y = 0; y < block.shape.height; y++)
						{
							for (int x = 0; x < block.shape.width; x++)
							{
								// ��]��̃u���b�N�`����쐬
								rotatedBlock.shape.pattern.at(block.shape.width - 1 - x).at(y) =
									block.shape.pattern.at(y).at(x);
							}
							
						}
						// ��]��̃u���b�N��K�p
						block = rotatedBlock;
					}
					break;
			}

			if (BlockIntersectField())
			{
				block = lastBlock;
			}
			else
			{
				// �R���\�[����ʂ��N���A�ɂ���
				system("cls");

				// �t�B�[���h�̕`��
				DrawField();
			}
		}
	}
	return 0;
}