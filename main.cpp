// ブルーム効果基本
#include "DxLib.h"
#include <algorithm>

//カメラ情報
struct Camera {
	VECTOR pos;					//カメラの初期ポジション
	int horizontalRotAngle;		//カメラの水平角度
	int verticalRotAngle;		//カメラの垂直角度
	int twistRotAngle;			//カメラの捻り角度
};

int ColorScreen;			// 普通の描画結果を書き込むスクリーン
int HighBrightScreen;		// 普通の描画結果から高輝度部分を抜き出した結果を書き込むスクリーン
int DownScaleScreen;		// 高輝度部分を縮小した結果を書き込むスクリーン
int GaussScreen;			// 縮小画像をガウスフィルタでぼかした結果を書き込むスクリーン

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Camera camera;
	int StageModel;				// ステージモデル
	int GaussRatio;				// ガウスフィルタのぼかし度合い
	int screenWidth;			//画面の横幅
	int screenHeight;			//画面の縦幅
	int downScale;				//縮小するサイズ
	int downScaleScreenWidth;	//ガウスフィルタを掛ける画像の横幅
	int downScaleScreenHeight;	//ガウスフィルタを掛ける画像の縦幅
	

	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	//変数の初期化
	//画面の横幅、縦幅
	downScale = 8;
	screenWidth = 640;
	screenHeight = 480;
	downScaleScreenWidth = screenWidth / downScale;
	downScaleScreenHeight = screenHeight / downScale;

	//ガウスフィルタのぼかし度合いを初期化
	//光の膨らみ、ぼかしを調整するガウスを調整する変数
	GaussRatio = 900;

	//カメラの垂直回転角度、水平回転角度、捻り回転角度
	camera.horizontalRotAngle = 0;
	camera.verticalRotAngle = 0;
	camera.twistRotAngle = 0;

	//カメラの初期ポジション
	camera.pos = { 0.0f, 120.0f, 0.0f };

	// 画面の解像度と色ビット深度を設定
	SetGraphMode(screenWidth, screenHeight, 32);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
		return -1;

	// ステージモデルの読み込み
	StageModel = MV1LoadModel("BloomTestStage.mqo");

	// 通常の描画結果を書き込むスクリーンと、フィルターの処理結果を書き込むスクリーンの作成
	ColorScreen = MakeScreen(screenWidth, screenHeight, FALSE);

	//高輝度、光の輝きが強いところを描画するためのスクリーン
	HighBrightScreen = MakeScreen(screenWidth, screenHeight, FALSE);

	//HighBrightScreenを縮小描画するためのスクリーン
	DownScaleScreen = MakeScreen(downScaleScreenWidth, downScaleScreenHeight, FALSE);

	//光に対してガウスぼかしをかけたスクリーンを書き込むためのスクリーン
	GaussScreen = MakeScreen(downScaleScreenWidth, downScaleScreenHeight, FALSE);

	// ClearDrawScreen 時に画面に塗りつぶす色をセット( 真っ白 )
	SetBackgroundColor(255, 255, 255);

	

	// メインループ(何かキーが押されたらループを抜ける)
	while (ProcessMessage() == 0)
	{
		// 左右キーが押されたらカメラを回転する
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

		// 上下キーが押されたらガウスフィルタのぼかし度合いを変更する
		if (CheckHitKey(KEY_INPUT_UP))
		{
			GaussRatio++;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			if (GaussRatio > 0)
				GaussRatio--;
		}


		// 通常の描画結果を書き込むスクリーンを描画対象にする
		SetDrawScreen(ColorScreen);

		// 画面をクリア
		ClearDrawScreen();


		// カメラの位置と向きを設定
		SetCameraPositionAndAngle(camera.pos, camera.verticalRotAngle, camera.horizontalRotAngle / 180.0f * 3.14159f, camera.twistRotAngle);

		// ステージモデルの描画
		MV1DrawModel(StageModel);


		// 描画結果から高輝度部分のみを抜き出した画像を得る
		//第１引数にフィルター処理を施すグラフィックハンドル
		//第２引数にフィルターを施した結果を出力するグラフィックハンドル
		//第３引数に施すフィルターのタイプ　(今回は明るさクリップフィルタ)

		//フィルタータイプ以降の引数はフィルタータイプに追随するものだと考えられる
		GraphFilterBlt(ColorScreen, HighBrightScreen, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 230, TRUE, GetColor(0, 0, 0), 255);


		// 高輝度部分を８分の１に縮小した画像を得る
		//第１引数にフィルター処理を施すグラフィックハンドル
		//第２引数にフィルターを施した結果を出力するグラフィックハンドル
		//第３引数に施すフィルターのタイプ　(今回は縮小フィルタ)
		GraphFilterBlt(HighBrightScreen, DownScaleScreen, DX_GRAPH_FILTER_DOWN_SCALE, downScale);

		// ８分の１に縮小した画像をガウスフィルタでぼかす
		GraphFilterBlt(DownScaleScreen, GaussScreen, DX_GRAPH_FILTER_GAUSS, 16, GaussRatio);


		// 描画対象を裏画面にする
		SetDrawScreen(DX_SCREEN_BACK);


		// 通常の描画結果を描画する
		DrawGraph(0, 0, ColorScreen, FALSE);


		// 描画モードをバイリニアフィルタリングにする( 拡大したときにドットがぼやけるようにする )
		SetDrawMode(DX_DRAWMODE_BILINEAR);

		// 描画ブレンドモードを加算にする
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

		// 高輝度部分を縮小してぼかした画像を画面いっぱいに２回描画する( ２回描画するのはより明るくみえるようにするため )
		DrawExtendGraph(0, 0, screenWidth, screenHeight, GaussScreen, FALSE);
		DrawExtendGraph(0, 0, screenWidth, screenHeight, GaussScreen, FALSE);

		// 描画ブレンドモードをブレンド無しに戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// 描画モードを二アレストに戻す
		SetDrawMode(DX_DRAWMODE_NEAREST);


		// フィルター処理の途中経過が分かるように画面下部に縮小して描画する
		DrawExtendGraph(180 * 0 + 24, 320, 180 * 0 + 24 + 160, 120 + 320, HighBrightScreen, FALSE);
		DrawExtendGraph(180 * 1 + 24, 320, 180 * 1 + 24 + 160, 120 + 320, DownScaleScreen, FALSE);
		DrawExtendGraph(180 * 2 + 24, 320, 180 * 2 + 24 + 160, 120 + 320, GaussScreen, FALSE);


		// 現在のガウスフィルタのぼかし度合いを描画する
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Gauss:%d", GaussRatio);
		DrawFormatString(0, 16, 0xffffff, "%d", camera.horizontalRotAngle);


		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}
