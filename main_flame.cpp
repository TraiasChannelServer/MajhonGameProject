// DxLib_template
//

#include "framework.h"
#include "main_flame.h"
#include "DxLib.h"


// ウィンドウ出現時の初期サイズ
static constexpr int INIT_WINDOW_CLIENT_X_SIZE = 1280;
static constexpr int INIT_WINDOW_CLIENT_Y_SIZE = 720;

const int dxLibError = -1;
inline bool DXLibError(int result) {
    return result == dxLibError ? true : false;
}

// 全モニターのRECTを格納する
static std::vector<RECT> g_monRects;

// 全モニターのRECTを取得するためのコールバック関数
BOOL monitorenumproc(HMONITOR hMon, HDC hMonDC, LPRECT lpMonRect, LPARAM dwData) {
    g_monRects.push_back(*lpMonRect);
    return TRUE;
}

// DXライブラリのウィンドウメッセージ処理をフックする関数
static LRESULT CALLBACK WndProcHook(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_MOVE:
    {
        // 以下自作スナップ処理の記述
        // スナップとは？(https://win10-navi.com/snap/)

        // 各モニターのRECTでループ
        for (auto& r : g_monRects)
        {
            POINT p = {};
            GetCursorPos(&p);

            // カーソルが存在するモニターでしか処理しない
            if (p.x < r.left || r.right < p.x || p.y < r.top || r.bottom < p.y)
                continue;

            // ウィンドウの比率を維持したまま
            // 画面の1/4の大きさになるように調整
            int w = r.right - r.left;
            int h = r.bottom - r.top;
            if (INIT_WINDOW_CLIENT_X_SIZE > INIT_WINDOW_CLIENT_Y_SIZE) {
                w /= 2;
                h = w * INIT_WINDOW_CLIENT_Y_SIZE / INIT_WINDOW_CLIENT_X_SIZE;
            }
            else {
                h /= 2;
                w = h * INIT_WINDOW_CLIENT_X_SIZE / INIT_WINDOW_CLIENT_Y_SIZE;
            }

            // スナップ処理が発動するマウス座標の画面端判定範囲に余裕を持たせる
            static constexpr int padding = 50;

            // 左上
            if (p.x <= r.left + padding && p.y <= r.top + padding)
            {
                SetWindowPosition(r.left, r.top);
                SetWindowSize(w, h);
            }
            // 左下
            if (p.x <= r.left + padding && p.y >= r.bottom - padding)
            {
                SetWindowPosition(r.left, r.bottom - h);
                SetWindowSize(w, h);
            }
            // 右上
            if (p.x >= r.right - padding && p.y <= r.top + padding)
            {
                SetWindowPosition(r.right - w, r.top);
                SetWindowSize(w, h);
            }
            // 右下
            if (p.x >= r.right - padding && p.y >= r.bottom - padding)
            {
                SetWindowPosition(r.right - w, r.bottom - h);
                SetWindowSize(w, h);
            }
        }
    }
    break;
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(INIT_WINDOW_CLIENT_X_SIZE, INIT_WINDOW_CLIENT_Y_SIZE, 32);
    ChangeWindowMode(TRUE);
    SetWindowSizeChangeEnableFlag(TRUE);

    // 全モニターのRECTを取得する
    EnumDisplayMonitors(NULL, NULL, monitorenumproc, 0);

    // ウィンドウメッセージのフック
    SetHookWinProc(WndProcHook);

    if (DXLibError(DxLib_Init())) return dxLibError;

    // file loading

    SetDrawScreen(DX_SCREEN_BACK);
    DxLib::SetMouseDispFlag(TRUE);
    DxLib::SetBackgroundColor(0, 0, 0);


    while (ProcessMessage() == 0) {

        ClearDrawScreen();

        unsigned int color = GetColor(255, 255, 255);
        DrawFormatString(0, 0, color, "%s", "MJP_test");

        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        ScreenFlip();
    }




    //WaitKey();					// キーの入力待ち((7-3)『WaitKey』を使用)

    DxLib::DxLib_End();			// ＤＸライブラリ使用の終了処理

    return 0;					// ソフトの終了
}