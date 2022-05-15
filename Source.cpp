#include "DxLib.h"
#define PI 3.14159265358979323

typedef struct body {
	int x;
	int y;
	double Angle;
}food_d,*pfood_d;

void normal_mode(int * , int);
void easy_mode(int* , int);
void difficult_mode(int*, int);
int openning(int *);
void gameover(int,int);
void gameclear(int,int);


int pos_check(int x, int y, pfood_d p_data , int num);
int food_run(int *x, int* y, pfood_d p_data , int num, int faceX, int faceY);

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Log.txt�̕\��(TRUE), ��\��(FALSE)
	SetOutApplicationLogValidFlag(TRUE);

	//Window���[�h
	ChangeWindowMode(TRUE);

	// Window�^�C�g��
	SetMainWindowText("�X�l�[�N�Q�[��");

	// ��ʃ��[�h�̐ݒ�
	SetGraphMode(640, 560, 16);//�Q�[�����640�~480

	// �c�w���C�u��������������
	if (DxLib_Init() == -1) {
		// �G���[���N�����璼���ɏI��
		return -1;
	}
	while (1) {
		int mode = 0;
		openning(&mode);

		int buf[3];
		FILE* fp;
		fopen_s(&fp, "text.dat", "rb");
		//fread(buf, sizeof(int), 3, fp);
		if (fp == NULL) {
			for (int i = 0; i < 3; i++) {
				buf[i] = 0;
			}
		}
		else {
			fread(buf, sizeof(int), 3, fp);
			fclose(fp);
		}



		//��
		int gamescore = 0;
		int highscore = buf[mode];

		if (mode == 0) {
			easy_mode(&gamescore, highscore);
		}
		else if (mode == 1) {
			normal_mode(&gamescore,highscore);
		}
		else if (mode == 2) {
			difficult_mode(&gamescore, highscore);
		}
		else {
			break;
		}
		//normal_mode(&score);
		//return ���l����@switch�ŋ������Ƃł����������@�ӂ��Ŏ�����邩���ˁB
		//gameover��
		buf[mode] = gamescore;
		fopen_s(&fp, "text.dat", "wb");
		fwrite(buf, sizeof(int), 3, fp);
		fclose(fp);
	}
	


	// �L�[���͑҂�
	WaitKey();

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�E�F�A�̏I��
	return 0;
}

int openning(int *mode) {
	//op anykey
	SetDrawScreen(DX_SCREEN_BACK);
	LoadGraphScreen(0, 0, "img/title.png", FALSE);
	ScreenFlip();
	WaitKey();
	
	

	int back_Img = LoadGraph("img/p305.jpg");
	int easy_Img = LoadGraph("img/easy1.png");
	int normal_Img = LoadGraph("img/normal1.png");
	int hard_Img = LoadGraph("img/hard1.png");
	int exit_Img = LoadGraph("img/exit1.png");

	int set = 0;
	DrawRotaGraph3(0, 0, 0, 0, 1.0, 1.0, 0, back_Img, TRUE, FALSE);
	SetDrawScreen(DX_SCREEN_BACK);
	WaitTimer(200);
	while(1){
		//�����̐ݒ�
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
		//��Ȃ̂�y�̒l�����炷
			set++;
			if (set > 3) {
				set = 0;
			}
		}
		else if (CheckHitKey(KEY_INPUT_UP) == 1) {
		//���Ȃ̂�y�̒l�𑝂₷
			set--;
			if (set < 0) {
				set = 3;
			}
		}
		else if (CheckHitKey(KEY_INPUT_RETURN) == 1) {
			*mode = set;
			break;
		}

		if (set == 0) {
			DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);
			DrawRotaGraph3(0, 0, 0, 0, 1.0, 1.0, 0, easy_Img, TRUE, FALSE);	
		}
		if (set == 1) {
			DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);
			DrawRotaGraph3(0, 0, 0, 0, 1.0, 1.0, 0, normal_Img, TRUE, FALSE);
		}
		if (set == 2) {
			DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);
			DrawRotaGraph3(0, 0, 0, 0, 1.0, 1.0, 0, hard_Img, TRUE, FALSE);
		}
		if (set == 3) {
			DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);
			DrawRotaGraph3(0, 0, 0, 0, 1.0, 1.0, 0, exit_Img, TRUE, FALSE);
		}
		WaitTimer(100);
		ScreenFlip();
	}
	return 0;
	
		
}
void gameover(int highscore, int score) {
	SetDrawScreen(DX_SCREEN_BACK);

	//�v���� �t�H���g�ݒ�
	int score_cha = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);
	//int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

	//�摜
	int over_img = LoadGraph("img/over.png"); 

	//�^�����w�i
	DrawBox(0, 0, 641, 561, GetColor(254, 254, 254), TRUE);

	//�����̔z�u
	//if����ɂ��@�n�C�X�R�A���X�V���邩���߂�B
	//�o�͂̂��߁@�Q�[���{�̕��ɕ�������Ă���������
	DrawRotaGraph2(164, 60, 0, 0, 1.0, 0, over_img, TRUE, FALSE);

	DrawFormatStringToHandle(135, 273, GetColor(0, 0, 0), score_cha, "SCORE");
	DrawFormatStringToHandle(370, 273, GetColor(0, 0, 0), score_cha, "HIGH SCORE");

	DrawFormatStringToHandle(115, 330, GetColor(0, 0, 0), score_cha, "%08d",score);
	DrawFormatStringToHandle(380, 330, GetColor(0, 0, 0), score_cha, "%08d", highscore);

	DrawFormatStringToHandle(210, 470, GetColor(0, 0, 0), score_cha, "Press any key!");

	ScreenFlip();
	WaitTimer(500);
	WaitKey();
}

void gameclear(int highscore, int score) {
	SetDrawScreen(DX_SCREEN_BACK);

	//�v���� �t�H���g�ݒ�
	//int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

	//�^�����w�i
	DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);

	//�摜
	int score_img = LoadGraph("img/score.png");
	DrawRotaGraph2(0, 0, 0, 0, 1.0, 0, score_img, TRUE, FALSE);

	//�����̔z�u
	//if����ɂ��@�n�C�X�R�A���X�V���邩���߂�B
	//�o�͂̂��߁@�Q�[���{�̕��ɕ�������Ă���������

	DrawFormatStringToHandle(76, 330, GetColor(0, 0, 0), score_num, "%08d", score);
	DrawFormatStringToHandle(115, 447, GetColor(0, 0, 0), score_num, "%08d", highscore);


	ScreenFlip();
	WaitTimer(500);
	WaitKey();
}



int pos_check(int x, int y, pfood_d p_data , int num) {
	int yn = 0; //���Ȃ���0�@�����ɂԂ������Ƃ�or���Ԃ��1
	
	for (int i = 0; i < num; i++) {
		if ((p_data + i)->x == x && (p_data + i)->y == y) {//�A���h�ɂ��Ȃ��ƈ��Ԃ�`�F�b�N�����Ⴄ
			yn = 1;
			return yn;
		}
		
	}
	return yn;
}

int food_run(int* x, int* y, pfood_d p_data , int num, int faceX, int faceY) {
	int new_x, new_y;

	while (1) {

		//�͈͓��ŗ������N����
		new_x = GetRand(19) * 32 + 16;
		new_y = GetRand(14) * 32 + 16;

		if ((!(pos_check(new_x, new_y, p_data, num))) || !(new_x == faceX && new_y == faceY)) {
			break;//��Ƒ̂Ƃ��Ԃ�Ȃ�������1
		}
		
	}

	//�V����food�̍��W���o��
	*x = new_x;
	*y = new_y;

	return 0;//���Y��Ȃ��I
}

void normal_mode(int *gamescore, int highscore) {
	//�Q�[���̑��x
	int delay = 140;

	// ��ʂɔw�i��\��
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//�X�l�[�N�̕ϐ���ݒ肷��
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	  //4������ۑ�
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//�X�l�[�N��body�̃f�[�^�p�z����쐬
	food_d data[320];
	pfood_d p_data = data;
	  //�z��̌�
	int body_num = 0;

	//�a�̍��W�̕ϐ�
	int foodX;
	int foodY;

	//�����̉a�@�z�u�B
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//�t�H���g�̐ݒ�B
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 1;

	//������
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "�����L�[�������Ă�", GetColor(255, 255, 255), score_cha);
	ScreenFlip();
	while (1) {
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_UP) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			break;
		}
	}

	while (1) {
		// �w�i��\��
		DrawGraph(0, 0, bgImg, FALSE);

		//�̂̍��W�ړ�
		for (int i = body_num - 1; i > 0; i--) {//-1��body_num�Ɣz��ň�̃Y�����L�邽��
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//direction���4�����w��
		switch (direction)
		{
		case 0:
			break;
		case 1:
			snakeAddX = -32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 2:
			snakeAddX = 32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 3:
			snakeAddX = 0;
			snakeAddY = -32;
			snakeY += snakeAddY;
			break;
		case 4:
			snakeAddX = 0;
			snakeAddY = 32;
			snakeY += snakeAddY;
			break;
		default:
			break;
		}

		//�Ԃ���������(�����Ȓl�Ȃ͉̂摜�̒��S�ɍ��W�_�����邽��)
		if (snakeX >= 640) {
			snakeX = 16;
		}
		if (snakeX < 0) {
			snakeX = 608 + 16;//+16?
		}
		if (snakeY >= 480) {
			snakeY = 16;
		}
		if (snakeY < 0) {
			snakeY = 448 + 16;
		}


		//�a��\���@(���ׂ��Ƃ��͎��̐ݒu�ꏊ��)
		//�a�̍��W�ɍs�����Ƃ��Ɍ��ɐL�΂�
		if (snakeX == foodX && snakeY == foodY) {
			
			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //��ڂ͊�̃f�[�^���K�v
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //�Ԃ������̂Ō��ɐL�΂��B

			//new�̂̏ꏊ�ɏo�����Ȃ��悤�ɏ��ǉ����food��ݒu
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//�a�̕`��
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//�̂ɓ�����I
		//���݂͎~�܂�d�l   /////////////////////////////////////////////////////////
		//return ���ȁH
		if (pos_check(snakeX, snakeY, p_data, body_num)){
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameover(highscore,score);

			
			break;
		}

		if (body_num == 299) {
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameclear(highscore, score);

			break;
		}


		// ��ʂɃw�r�̓���\��
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//�ւ̑̂̕\��	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//return����op?

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// 500/1000�b�҂�
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//�����̐ݒ�
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//���Ȃ̂�x�̒l�����炷
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//�E�Ȃ̂�x�̒l�𑝂₷
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//��Ȃ̂�y�̒l�����炷
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//���Ȃ̂�y�̒l�𑝂₷
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE��ʂ̂��ׂ�
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//���_�l�I�ɂ�1�b��100�����Ă���B
		score++;

	}
}

void easy_mode(int* gamescore , int highscore){
	//�Q�[���̑��x
	int delay = 300;

	// ��ʂɔw�i��\��
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//�X�l�[�N�̕ϐ���ݒ肷��
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	//4������ۑ�
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//�X�l�[�N��body�̃f�[�^�p�z����쐬
	food_d data[320];
	pfood_d p_data = data;
	//�z��̌�
	int body_num = 0;

	//�a�̍��W�̕ϐ�
	int foodX;
	int foodY;

	//�����̉a�@�z�u�B
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//�t�H���g�̐ݒ�B
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 0;

	//������
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "�����L�[�������Ă�", GetColor(255, 255, 255), score_cha);
	ScreenFlip();
	while (1) {
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_UP) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			break;
		}
	}

	while (1) {
		// �w�i��\��
		DrawGraph(0, 0, bgImg, FALSE);

		//�̂̍��W�ړ�
		for (int i = body_num - 1; i > 0; i--) {//-1��body_num�Ɣz��ň�̃Y�����L�邽��
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//direction���4�����w��
		switch (direction)
		{
		case 0:
			break;
		case 1:
			snakeAddX = -32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 2:
			snakeAddX = 32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 3:
			snakeAddX = 0;
			snakeAddY = -32;
			snakeY += snakeAddY;
			break;
		case 4:
			snakeAddX = 0;
			snakeAddY = 32;
			snakeY += snakeAddY;
			break;
		default:
			break;
		}

		//�Ԃ���������(�����Ȓl�Ȃ͉̂摜�̒��S�ɍ��W�_�����邽��)
		if (snakeX >= 640) {
			snakeX = 16;
		}
		if (snakeX < 0) {
			snakeX = 608 + 16;//+16?
		}
		if (snakeY >= 480) {
			snakeY = 16;
		}
		if (snakeY < 0) {
			snakeY = 448 + 16;
		}


		//�a��\���@(���ׂ��Ƃ��͎��̐ݒu�ꏊ��)
		//�a�̍��W�ɍs�����Ƃ��Ɍ��ɐL�΂�
		if (snakeX == foodX && snakeY == foodY) {
			
			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //��ڂ͊�̃f�[�^���K�v
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //�Ԃ������̂Ō��ɐL�΂��B

			//�V�����̒ǉ����food��...
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//�a�̕`��
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//�̂ɓ�����I
		//���݂͎~�܂�d�l   /////////////////////////////////////////////////////////
		//return ���ȁH
		if (pos_check(snakeX, snakeY, p_data, body_num)) {
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameover(highscore, score);


			break;
		}

		if (body_num == 15) {
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameclear(highscore, score);

			break;
		}




		// ��ʂɃw�r�̓���\��
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//�ւ̑̂̕\��	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//return����op?

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// 500/1000�b�҂�
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//�����̐ݒ�
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//���Ȃ̂�x�̒l�����炷
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//�E�Ȃ̂�x�̒l�𑝂₷
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//��Ȃ̂�y�̒l�����炷
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//���Ȃ̂�y�̒l�𑝂₷
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE��ʂ̂��ׂ�
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//���_�l�I�ɂ�1�b��100�����Ă���B
		score++;

	}
}

void difficult_mode(int* gamescore, int highscore) {
	//�Q�[���̑��x
	int delay = 100;

	// ��ʂɔw�i��\��
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̃��[�h
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//�X�l�[�N�̕ϐ���ݒ肷��
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	//4������ۑ�
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//�X�l�[�N��body�̃f�[�^�p�z����쐬
	food_d data[320];
	pfood_d p_data = data;
	//�z��̌�
	int body_num = 0;

	//�a�̍��W�̕ϐ�
	int foodX;
	int foodY;

	//�����̉a�@�z�u�B
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//�t�H���g�̐ݒ�B
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 1;

	//������
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "�����L�[�������Ă�", GetColor(255, 255, 255), score_cha);
	ScreenFlip();
	while (1) {
		if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_UP) == 1) {
			break;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
			break;
		}
	}

	while (1) {
		// �w�i��\��
		DrawGraph(0, 0, bgImg, FALSE);

		//�̂̍��W�ړ�
		for (int i = body_num - 1; i > 0; i--) {//-1��body_num�Ɣz��ň�̃Y�����L�邽��
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//direction���4�����w��
		switch (direction)
		{
		case 0:
			break;
		case 1:
			snakeAddX = -32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 2:
			snakeAddX = 32;
			snakeAddY = 0;
			snakeX += snakeAddX;
			break;
		case 3:
			snakeAddX = 0;
			snakeAddY = -32;
			snakeY += snakeAddY;
			break;
		case 4:
			snakeAddX = 0;
			snakeAddY = 32;
			snakeY += snakeAddY;
			break;
		default:
			break;
		}




		//�a��\���@(���ׂ��Ƃ��͎��̐ݒu�ꏊ��)
		//�a�̍��W�ɍs�����Ƃ��Ɍ��ɐL�΂�
		if (snakeX == foodX && snakeY == foodY) {

			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //��ڂ͊�̃f�[�^���K�v
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //�Ԃ������̂Ō��ɐL�΂��B

			//new�̂̏ꏊ�ɏo�����Ȃ��悤�ɏ��ǉ����food��ݒu
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//�a�̕`��
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//�̂ɓ�����I
		//���݂͎~�܂�d�l   /////////////////////////////////////////////////////////
		//return ���ȁH
		if (pos_check(snakeX, snakeY, p_data, body_num)||(snakeX < 0 || snakeX > 640 || snakeY < 0 || snakeY > 480)) {
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameover(highscore, score);


			break;
		}

		if (body_num == 299) {
			score = (score)+body_num * 300;
			if (highscore < score) {
				*gamescore = score;
				highscore = score;
			}
			else {
				*gamescore = highscore;
			}
			gameclear(highscore, score);

			break;
		}


		// ��ʂɃw�r�̓���\��
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//�ւ̑̂̕\��	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//return����op?

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		// 500/1000�b�҂�
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//�����̐ݒ�
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//���Ȃ̂�x�̒l�����炷
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//�E�Ȃ̂�x�̒l�𑝂₷
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//��Ȃ̂�y�̒l�����炷
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//���Ȃ̂�y�̒l�𑝂₷
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE��ʂ̂��ׂ�
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//���_�l�I�ɂ�1�b��100�����Ă���B
		score++;

	}
}