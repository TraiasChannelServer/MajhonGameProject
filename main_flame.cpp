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

// 全モニターのRECTを取得するためのコールバック関数
static BOOL monitorenumproc(HMONITOR hMon, HDC hMonDC, LPRECT lpMonRect, LPARAM monRects) {
    ((std::vector<RECT>*)monRects)->push_back(*lpMonRect);
    return TRUE;
}

// DXライブラリのウィンドウメッセージ処理をフックする関数
// スナップ処理を自作するために作成
// スナップとは？(https://win10-navi.com/snap/)
static LRESULT CALLBACK WndProcHook(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    // 全モニターのRECTを格納する
    static std::vector<RECT> s_monRects;

    // スナップするか否か、するなら四隅のどこか
    static enum class DoSnap {
        NO, TL, TR, BL, BR
    } s_doSnap = DoSnap::NO;

    // スナップすることが決まったときのモニターのインデックス
    static int s_monIndex = 0;

    // ウィンドウサイズとクライアントサイズの差
    static RECT s_wndDiffRect = {};

    switch (message) {
    case WM_CREATE:
    {
        // 全モニターのRECTを取得する
        EnumDisplayMonitors(NULL, NULL, monitorenumproc, (LPARAM)(&s_monRects));

        // ウィンドウサイズとクライアントサイズの差を計算
        DWORD style = (DWORD)GetWindowLong(handle, GWL_STYLE);
        s_wndDiffRect = { 0 , 0, INIT_WINDOW_CLIENT_X_SIZE, INIT_WINDOW_CLIENT_Y_SIZE };
        AdjustWindowRect(&s_wndDiffRect, style, FALSE);
        s_wndDiffRect.right -= INIT_WINDOW_CLIENT_X_SIZE;
        s_wndDiffRect.bottom -= INIT_WINDOW_CLIENT_Y_SIZE;
    }
    break;
    case WM_MOVING:
    {
        s_doSnap = DoSnap::NO;

        // 各モニターのRECTでループ
        for (int i = 0; i < s_monRects.size(); ++i)
        {
            RECT& r = s_monRects[i];

            POINT p = {};
            GetCursorPos(&p);

            // カーソルが存在するモニターでしか処理しない
            if (p.x < r.left || r.right < p.x || p.y < r.top || r.bottom < p.y)
                continue;

            // スナップ処理が発動するマウス座標の画面端判定範囲に余裕を持たせる
            static constexpr int padding = 50;

            // 左上
            if (p.x <= r.left + padding && p.y <= r.top + padding)
            {
                s_doSnap = DoSnap::TL;
                s_monIndex = i;
            }
            // 右上
            if (p.x >= r.right - padding && p.y <= r.top + padding)
            {
                s_doSnap = DoSnap::TR;
                s_monIndex = i;
            }
            // 左下
            if (p.x <= r.left + padding && p.y >= r.bottom - padding)
            {
                s_doSnap = DoSnap::BL;
                s_monIndex = i;
            }
            // 右下
            if (p.x >= r.right - padding && p.y >= r.bottom - padding)
            {
                s_doSnap = DoSnap::BR;
                s_monIndex = i;
            }
        }
    }
    break;
    case WM_EXITSIZEMOVE:
    {
        if (s_doSnap == DoSnap::NO)
            break;

        // モニターのRECT
        RECT& monRect = s_monRects[s_monIndex];

        // ウィンドウのRECT(見た目より当たり判定?が広い)
        RECT wndRect = {};
        GetWindowRect(handle, &wndRect);

        // ウィンドウの見たままの大きさのRECT
        RECT looksRect = {};
        DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

        // A.横幅基準でウィンドウサイズを計算
        RECT aRect = {};
        {
            aRect.right = (monRect.right - monRect.left) / 2;                                   // 見た目の幅が画面の 1/2
            aRect.right += (wndRect.right - wndRect.left) - (looksRect.right - looksRect.left); // 見た目の幅をウィンドウの幅に変換
            aRect.right -= (s_wndDiffRect.right - s_wndDiffRect.left);                          // ウィンドウの幅をクライアントの幅に変換
            aRect.bottom = aRect.right * INIT_WINDOW_CLIENT_Y_SIZE / INIT_WINDOW_CLIENT_X_SIZE; // クライアントのアスペクト比を保つように高さを計算
        }

        // B.高さ基準でウィンドウサイズを計算
        //RECT bRect = {};
        //{
        //    bRect.bottom = (monRect.bottom - monRect.top) / 2;                                   // 見た目の高さが画面の 1/2
        //    bRect.bottom += (wndRect.bottom - wndRect.top) - (looksRect.bottom - looksRect.top); // 見た目の高さをウィンドウの高さに変換
        //    bRect.bottom -= (s_wndDiffRect.bottom - s_wndDiffRect.top);                          // ウィンドウの高さをクライアントの高さに変換
        //    bRect.right = bRect.bottom * INIT_WINDOW_CLIENT_X_SIZE / INIT_WINDOW_CLIENT_Y_SIZE;  // クライアントのアスペクト比を保つように幅を計算
        //}

        //RECT dstRect = {};

        // AとBの内、小さい方を選択
        //if (aRect.right <= bRect.right)
        //    dstRect = aRect;
        //else
        //    dstRect = bRect;

        // ウィンドウを縦に並べる使い方はしないので
        // Bはコメントアウトし、Aを選択することにした
        RECT dstRect = aRect;

        // クライアントサイズをこの関数に設定する
        DxLib::SetWindowSize(dstRect.right, dstRect.bottom);

        // ウィンドウサイズを変更したのでサイズを取得し直す
        GetWindowRect(handle, &wndRect);
        DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &looksRect, sizeof(RECT));

        switch (s_doSnap)
        {
        case DoSnap::TL:
            // 左上
            SetWindowPosition(monRect.left + (wndRect.left - looksRect.left), monRect.top + (wndRect.top - looksRect.top));
            break;
        case DoSnap::TR:
            // 右上
            SetWindowPosition(monRect.right - (looksRect.right - looksRect.left) + (wndRect.left - looksRect.left), monRect.top + (wndRect.top - looksRect.top));
            break;
        case DoSnap::BL:
            // 左下
            SetWindowPosition(monRect.left + (wndRect.left - looksRect.left), monRect.bottom - (looksRect.bottom - looksRect.top) + (wndRect.top - looksRect.top));
            break;
        case DoSnap::BR:
            // 右下
            SetWindowPosition(monRect.right - (looksRect.right - looksRect.left) + (wndRect.left - looksRect.left), monRect.bottom - (looksRect.bottom - looksRect.top) + (wndRect.top - looksRect.top));
            break;
        }

        s_doSnap = DoSnap::NO;
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