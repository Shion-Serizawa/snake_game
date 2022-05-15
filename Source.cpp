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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Log.txtの表示(TRUE), 非表示(FALSE)
	SetOutApplicationLogValidFlag(TRUE);

	//Windowモード
	ChangeWindowMode(TRUE);

	// Windowタイトル
	SetMainWindowText("スネークゲーム");

	// 画面モードの設定
	SetGraphMode(640, 560, 16);//ゲーム画面640×480

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) {
		// エラーが起きたら直ちに終了
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



		//仮
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
		//return を考える　switchで挙動ごとでもいいかも　ふたつで事足りるかもね。
		//gameoverも
		buf[mode] = gamescore;
		fopen_s(&fp, "text.dat", "wb");
		fwrite(buf, sizeof(int), 3, fp);
		fclose(fp);
	}
	


	// キー入力待ち
	WaitKey();

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトウェアの終了
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
		//方向の設定
		if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
		//上なのでyの値を減らす
			set++;
			if (set > 3) {
				set = 0;
			}
		}
		else if (CheckHitKey(KEY_INPUT_UP) == 1) {
		//下なのでyの値を増やす
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

	//要調整 フォント設定
	int score_cha = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);
	//int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);

	//画像
	int over_img = LoadGraph("img/over.png"); 

	//真っ白背景
	DrawBox(0, 0, 641, 561, GetColor(254, 254, 254), TRUE);

	//文字の配置
	//if分岐により　ハイスコアを更新するか決める。
	//出力のため　ゲーム本体部に分岐を入れてもいいかも
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

	//要調整 フォント設定
	//int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 30, -1, DX_FONTTYPE_ANTIALIASING);

	//真っ白背景
	DrawBox(0, 0, 641, 561, GetColor(0, 0, 0), TRUE);

	//画像
	int score_img = LoadGraph("img/score.png");
	DrawRotaGraph2(0, 0, 0, 0, 1.0, 0, score_img, TRUE, FALSE);

	//文字の配置
	//if分岐により　ハイスコアを更新するか決める。
	//出力のため　ゲーム本体部に分岐を入れてもいいかも

	DrawFormatStringToHandle(76, 330, GetColor(0, 0, 0), score_num, "%08d", score);
	DrawFormatStringToHandle(115, 447, GetColor(0, 0, 0), score_num, "%08d", highscore);


	ScreenFlip();
	WaitTimer(500);
	WaitKey();
}



int pos_check(int x, int y, pfood_d p_data , int num) {
	int yn = 0; //問題なしで0　自分にぶつかったときorかぶりで1
	
	for (int i = 0; i < num; i++) {
		if ((p_data + i)->x == x && (p_data + i)->y == y) {//アンドにしないと一列ぶんチェックしちゃう
			yn = 1;
			return yn;
		}
		
	}
	return yn;
}

int food_run(int* x, int* y, pfood_d p_data , int num, int faceX, int faceY) {
	int new_x, new_y;

	while (1) {

		//範囲内で乱数を起こす
		new_x = GetRand(19) * 32 + 16;
		new_y = GetRand(14) * 32 + 16;

		if ((!(pos_check(new_x, new_y, p_data, num))) || !(new_x == faceX && new_y == faceY)) {
			break;//顔と体とかぶらなかったら1
		}
		
	}

	//新しいfoodの座標を出力
	*x = new_x;
	*y = new_y;

	return 0;//お忘れなく！
}

void normal_mode(int *gamescore, int highscore) {
	//ゲームの速度
	int delay = 140;

	// 画面に背景を表示
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//スネークの変数を設定する
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	  //4方向を保存
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//スネークのbodyのデータ用配列を作成
	food_d data[320];
	pfood_d p_data = data;
	  //配列の個数
	int body_num = 0;

	//餌の座標の変数
	int foodX;
	int foodY;

	//初期の餌　配置。
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//フォントの設定。
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 1;

	//ここで
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "方向キーをおしてね", GetColor(255, 255, 255), score_cha);
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
		// 背景を表示
		DrawGraph(0, 0, bgImg, FALSE);

		//体の座標移動
		for (int i = body_num - 1; i > 0; i--) {//-1はbody_numと配列で一つのズレが有るため
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//directionより4方向指定
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

		//ぶつかったら一周(微妙な値なのは画像の中心に座標点があるため)
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


		//餌を表示　(たべたときは次の設置場所へ)
		//餌の座標に行ったときに後ろに伸ばす
		if (snakeX == foodX && snakeY == foodY) {
			
			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //一個目は顔のデータが必要
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //ぶつかったので後ろに伸ばす。

			//new体の場所に出現しないように情報追加後にfoodを設置
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//餌の描写
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//体に当たる！
		//現在は止まる仕様   /////////////////////////////////////////////////////////
		//return かな？
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


		// 画面にヘビの頭を表示
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//蛇の体の表示	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//returnからop?

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 500/1000秒待つ
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//方向の設定
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//左なのでxの値を減らす
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//右なのでxの値を増やす
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//上なのでyの値を減らす
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//下なのでyの値を増やす
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE画面のすべて
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//理論値的には1秒に100増えている。
		score++;

	}
}

void easy_mode(int* gamescore , int highscore){
	//ゲームの速度
	int delay = 300;

	// 画面に背景を表示
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//スネークの変数を設定する
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	//4方向を保存
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//スネークのbodyのデータ用配列を作成
	food_d data[320];
	pfood_d p_data = data;
	//配列の個数
	int body_num = 0;

	//餌の座標の変数
	int foodX;
	int foodY;

	//初期の餌　配置。
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//フォントの設定。
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 0;

	//ここで
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "方向キーをおしてね", GetColor(255, 255, 255), score_cha);
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
		// 背景を表示
		DrawGraph(0, 0, bgImg, FALSE);

		//体の座標移動
		for (int i = body_num - 1; i > 0; i--) {//-1はbody_numと配列で一つのズレが有るため
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//directionより4方向指定
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

		//ぶつかったら一周(微妙な値なのは画像の中心に座標点があるため)
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


		//餌を表示　(たべたときは次の設置場所へ)
		//餌の座標に行ったときに後ろに伸ばす
		if (snakeX == foodX && snakeY == foodY) {
			
			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //一個目は顔のデータが必要
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //ぶつかったので後ろに伸ばす。

			//新しい体追加後にfoodを...
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//餌の描写
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//体に当たる！
		//現在は止まる仕様   /////////////////////////////////////////////////////////
		//return かな？
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




		// 画面にヘビの頭を表示
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//蛇の体の表示	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//returnからop?

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 500/1000秒待つ
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//方向の設定
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//左なのでxの値を減らす
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//右なのでxの値を増やす
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//上なのでyの値を減らす
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//下なのでyの値を増やす
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE画面のすべて
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//理論値的には1秒に100増えている。
		score++;

	}
}

void difficult_mode(int* gamescore, int highscore) {
	//ゲームの速度
	int delay = 100;

	// 画面に背景を表示
	LoadGraphScreen(0, 0, "img/BG.png", FALSE);

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int bgImg = LoadGraph("img/BG.png");
	int snakeImg = LoadGraph("img/head.png");
	int snakeb_img = LoadGraph("img/body.png");
	int food_img = LoadGraph("img/food.png");

	//スネークの変数を設定する
	int snakeX = 32 * 10 + 16;
	int snakeY = 32 * 7 + 16;
	int snakeAddX = 32;
	int snakeAddY = 32;
	double snakeAngle = 0.0;
	//4方向を保存
	int direction = 0;
	DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

	//スネークのbodyのデータ用配列を作成
	food_d data[320];
	pfood_d p_data = data;
	//配列の個数
	int body_num = 0;

	//餌の座標の変数
	int foodX;
	int foodY;

	//初期の餌　配置。
	food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
	DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

	//フォントの設定。
	int score_cha = CreateFontToHandle(NULL, 20, -1, DX_FONTTYPE_ANTIALIASING);
	int score_num = CreateFontToHandle(NULL, 25, -1, DX_FONTTYPE_ANTIALIASING);


	int score = 1;

	//ここで
	DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);
	DrawStringToHandle(40, 485, "方向キーをおしてね", GetColor(255, 255, 255), score_cha);
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
		// 背景を表示
		DrawGraph(0, 0, bgImg, FALSE);

		//体の座標移動
		for (int i = body_num - 1; i > 0; i--) {//-1はbody_numと配列で一つのズレが有るため
			(p_data + i)->x = (p_data + (i - 1))->x;
			(p_data + i)->y = (p_data + (i - 1))->y;
		}
		p_data->x = snakeX;
		p_data->y = snakeY;


		//directionより4方向指定
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




		//餌を表示　(たべたときは次の設置場所へ)
		//餌の座標に行ったときに後ろに伸ばす
		if (snakeX == foodX && snakeY == foodY) {

			if (body_num == 0) {
				(p_data + body_num)->x = snakeX - snakeAddX;
				(p_data + body_num)->y = snakeY - snakeAddY; //一個目は顔のデータが必要
			}
			else {
				(p_data + body_num)->x = (p_data + (body_num - 1))->x - snakeAddX;
				(p_data + body_num)->y = (p_data + (body_num - 1))->y - snakeAddY;
			}
			body_num++; //ぶつかったので後ろに伸ばす。

			//new体の場所に出現しないように情報追加後にfoodを設置
			food_run(&foodX, &foodY, p_data, body_num, snakeX, snakeY);
		}

		//餌の描写
		DrawRotaGraph2(foodX, foodY, 16, 16, 1.0, 0, food_img, TRUE, FALSE);

		//体に当たる！
		//現在は止まる仕様   /////////////////////////////////////////////////////////
		//return かな？
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


		// 画面にヘビの頭を表示
		DrawRotaGraph2(snakeX, snakeY, 16, 16, 1.0, snakeAngle, snakeImg, TRUE, FALSE);

		//蛇の体の表示	
		for (int i = 0; i < body_num; i++) {
			DrawRotaGraph2((p_data + i)->x, (p_data + i)->y, 16, 16, 1.0, 0, snakeb_img, TRUE, FALSE);
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
		//returnからop?

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		// 500/1000秒待つ
		//WaitTimer(500);
		for (int i = 0; i < delay / 10; i++) {
			WaitTimer(10);
			//方向の設定
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && direction != 2) {
				//左なのでxの値を減らす
				//snakeX -= snakeAddX;
				direction = 1;
				snakeAngle = 0.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && direction != 1) {
				//右なのでxの値を増やす
				//snakeX += snakeAddX;
				direction = 2;
				snakeAngle = 1.5 * PI;
			}
			else if (CheckHitKey(KEY_INPUT_UP) == 1 && direction != 4) {
				//上なのでyの値を減らす
				//snakeY -= snakeAddX;
				direction = 3;
				snakeAngle = PI;
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) == 1 && direction != 3) {
				//下なのでyの値を増やす
				//snakeY += snakeAddX;
				direction = 4;
				snakeAngle = 0;
			}
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
				break;

			//SCORE画面のすべて
			DrawBox(0, 480, 641, 561, GetColor(0, 0, 0), TRUE);

			//SetFontSize(20);
			DrawStringToHandle(40, 485, "SCORE", GetColor(255, 255, 255), score_cha);
			DrawStringToHandle(330, 485, "BODY LENGTH", GetColor(255, 255, 255), score_cha);
			DrawFormatStringToHandle(45, 525, GetColor(255, 255, 255), score_num, "%08d", (score)+body_num * 300);
			DrawFormatStringToHandle(335, 525, GetColor(255, 255, 255), score_num, "%03d", body_num + 1);
		}
		//理論値的には1秒に100増えている。
		score++;

	}
}