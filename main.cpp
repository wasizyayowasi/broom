// �u���[�����ʊ�{
#include "DxLib.h"
#include <algorithm>

//�J�������
struct Camera {
	VECTOR pos;					//�J�����̏����|�W�V����
	int horizontalRotAngle;		//�J�����̐����p�x
	int verticalRotAngle;		//�J�����̐����p�x
	int twistRotAngle;			//�J�����̔P��p�x
};

int ColorScreen;			// ���ʂ̕`�挋�ʂ��������ރX�N���[��
int HighBrightScreen;		// ���ʂ̕`�挋�ʂ��獂�P�x�����𔲂��o�������ʂ��������ރX�N���[��
int DownScaleScreen;		// ���P�x�������k���������ʂ��������ރX�N���[��
int GaussScreen;			// �k���摜���K�E�X�t�B���^�łڂ��������ʂ��������ރX�N���[��

// WinMain�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Camera camera;
	int StageModel;				// �X�e�[�W���f��
	int GaussRatio;				// �K�E�X�t�B���^�̂ڂ����x����
	int screenWidth;			//��ʂ̉���
	int screenHeight;			//��ʂ̏c��
	int downScale;				//�k������T�C�Y
	int downScaleScreenWidth;	//�K�E�X�t�B���^���|����摜�̉���
	int downScaleScreenHeight;	//�K�E�X�t�B���^���|����摜�̏c��
	

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

	//�ϐ��̏�����
	//��ʂ̉����A�c��
	downScale = 8;
	screenWidth = 640;
	screenHeight = 480;
	downScaleScreenWidth = screenWidth / downScale;
	downScaleScreenHeight = screenHeight / downScale;

	//�K�E�X�t�B���^�̂ڂ����x������������
	//���̖c��݁A�ڂ����𒲐�����K�E�X�𒲐�����ϐ�
	GaussRatio = 900;

	//�J�����̐�����]�p�x�A������]�p�x�A�P���]�p�x
	camera.horizontalRotAngle = 0;
	camera.verticalRotAngle = 0;
	camera.twistRotAngle = 0;

	//�J�����̏����|�W�V����
	camera.pos = { 0.0f, 120.0f, 0.0f };

	// ��ʂ̉𑜓x�ƐF�r�b�g�[�x��ݒ�
	SetGraphMode(screenWidth, screenHeight, 32);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0)
		return -1;

	// �X�e�[�W���f���̓ǂݍ���
	StageModel = MV1LoadModel("BloomTestStage.mqo");

	// �ʏ�̕`�挋�ʂ��������ރX�N���[���ƁA�t�B���^�[�̏������ʂ��������ރX�N���[���̍쐬
	ColorScreen = MakeScreen(screenWidth, screenHeight, FALSE);

	//���P�x�A���̋P���������Ƃ����`�悷�邽�߂̃X�N���[��
	HighBrightScreen = MakeScreen(screenWidth, screenHeight, FALSE);

	//HighBrightScreen���k���`�悷�邽�߂̃X�N���[��
	DownScaleScreen = MakeScreen(downScaleScreenWidth, downScaleScreenHeight, FALSE);

	//���ɑ΂��ăK�E�X�ڂ������������X�N���[�����������ނ��߂̃X�N���[��
	GaussScreen = MakeScreen(downScaleScreenWidth, downScaleScreenHeight, FALSE);

	// ClearDrawScreen ���ɉ�ʂɓh��Ԃ��F���Z�b�g( �^���� )
	SetBackgroundColor(255, 255, 255);

	

	// ���C�����[�v(�����L�[�������ꂽ�烋�[�v�𔲂���)
	while (ProcessMessage() == 0)
	{
		
		//�����͂���Ȃ��@�����p
		{
			// ���E�L�[�������ꂽ��J��������]����
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				camera.horizontalRotAngle -= 3;
				if (camera.horizontalRotAngle < 0)
				{
					camera.horizontalRotAngle += 360;
				}
			}
			if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				camera.horizontalRotAngle += 3;
				if (camera.horizontalRotAngle > 360)
				{
					camera.horizontalRotAngle -= 360;
				}
			}

			// �㉺�L�[�������ꂽ��K�E�X�t�B���^�̂ڂ����x������ύX����
			if (CheckHitKey(KEY_INPUT_UP))
			{
				GaussRatio++;
			}
			if (CheckHitKey(KEY_INPUT_DOWN))
			{
				if (GaussRatio > 0)
					GaussRatio--;
			}
		}

		// �ʏ�̕`�挋�ʂ��������ރX�N���[����`��Ώۂɂ���
		SetDrawScreen(ColorScreen);

		// ��ʂ��N���A
		ClearDrawScreen();


		// �J�����̈ʒu�ƌ�����ݒ�
		SetCameraPositionAndAngle(camera.pos, camera.verticalRotAngle, camera.horizontalRotAngle / 180.0f * 3.14159f, camera.twistRotAngle);

		// �X�e�[�W���f���̕`��
		MV1DrawModel(StageModel);


		// �`�挋�ʂ��獂�P�x�����݂̂𔲂��o�����摜�𓾂�
		//��P�����Ƀt�B���^�[�������{���O���t�B�b�N�n���h��
		//��Q�����Ƀt�B���^�[���{�������ʂ��o�͂���O���t�B�b�N�n���h��
		//��R�����Ɏ{���t�B���^�[�̃^�C�v�@(����͖��邳�N���b�v�t�B���^)

		//�t�B���^�[�^�C�v�ȍ~�̈����̓t�B���^�[�^�C�v�ɒǐ�������̂��ƍl������
		GraphFilterBlt(ColorScreen, HighBrightScreen, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 230, TRUE, GetColor(0, 0, 0), 255);


		// ���P�x�������W���̂P�ɏk�������摜�𓾂�
		//��P�����Ƀt�B���^�[�������{���O���t�B�b�N�n���h��
		//��Q�����Ƀt�B���^�[���{�������ʂ��o�͂���O���t�B�b�N�n���h��
		//��R�����Ɏ{���t�B���^�[�̃^�C�v�@(����͏k���t�B���^)
		GraphFilterBlt(HighBrightScreen, DownScaleScreen, DX_GRAPH_FILTER_DOWN_SCALE, downScale);

		// �W���̂P�ɏk�������摜���K�E�X�t�B���^�łڂ���
		GraphFilterBlt(DownScaleScreen, GaussScreen, DX_GRAPH_FILTER_GAUSS, 16, GaussRatio);


		// �`��Ώۂ𗠉�ʂɂ���
		SetDrawScreen(DX_SCREEN_BACK);


		// �ʏ�̕`�挋�ʂ�`�悷��
		DrawGraph(0, 0, ColorScreen, false);


		// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɂ���( �g�債���Ƃ��Ƀh�b�g���ڂ₯��悤�ɂ��� )
		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// �`��u�����h���[�h�����Z�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

		// ���P�x�������k�����Ăڂ������摜����ʂ����ς��ɂQ��`�悷��( �Q��`�悷��̂͂�薾�邭�݂���悤�ɂ��邽�� )
		DrawExtendGraph(0, 0, screenWidth, screenHeight, GaussScreen, false);
		DrawExtendGraph(0, 0, screenWidth, screenHeight, GaussScreen, false);

		// �`��u�����h���[�h���u�����h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// �`�惂�[�h���A���X�g�ɖ߂�
		SetDrawMode(DX_DRAWMODE_NEAREST);


		//�����͎��ۂɎg���Ƃ��͂���Ȃ�
		{
			// �t�B���^�[�����̓r���o�߂�������悤�ɉ�ʉ����ɏk�����ĕ`�悷��
			DrawExtendGraph(180 * 0 + 24, 320, 180 * 0 + 24 + 160, 120 + 320, HighBrightScreen, false);
			DrawExtendGraph(180 * 1 + 24, 320, 180 * 1 + 24 + 160, 120 + 320, DownScaleScreen, false);
			DrawExtendGraph(180 * 2 + 24, 320, 180 * 2 + 24 + 160, 120 + 320, GaussScreen, false);

			// ���݂̃K�E�X�t�B���^�̂ڂ����x������`�悷��
			DrawFormatString(0, 0, GetColor(255, 255, 255), "Gauss:%d", GaussRatio);
			DrawFormatString(0, 16, 0xffffff, "%d", camera.horizontalRotAngle);
		}

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}
