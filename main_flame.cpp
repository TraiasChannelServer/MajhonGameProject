// DxLib_template
//

#include "framework.h"
#include "main_flame.h"
#include "DxLib.h"

// 雨安実験コメントすぐ消します

const int dxLibError = -1;
inline bool DXLibError(int result) {
	return result == dxLibError ? true : false;
}

struct Size {
	int width, height;
};

class Window {
    HWND windowHandle = NULL;
    MSG message = {};
public:

    // ウィンドウプロシージャ
    static LRESULT CALLBACK Procedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            // 一応終了処理だけ
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(handle, message, wParam, lParam);
    }

    // ウィンドウハンドルを取得する
    HWND GetHandle() {
        return windowHandle;
    }

    // ウィンドウを生成する
    void Create(std::string title, std::string className) {
        // クラス情報を登録する
        WNDCLASS windowClass = {};
        windowClass.lpfnWndProc = Procedure;
        windowClass.hInstance = GetModuleHandle(NULL);
        windowClass.lpszClassName = className.c_str();
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClass(&windowClass);
        // ウィンドウを生成する
        windowHandle = CreateWindowEx(0, className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, 50, 50, 1280, 720, NULL, NULL, GetModuleHandle(NULL), NULL);

    }

    // ウィンドウのサイズを取得する
    Size GetSize() {
        Size result;
        GetWindowSize(&result.width, &result.height);
        return result;
    }

    // メッセージ処理
    bool Loop() {
        while (true) {
            if (message.message == WM_QUIT) return false;
            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else return true;
        }
    }

    // ウィンドウを表示する
    void Show() {
        ShowWindow(windowHandle, SW_SHOW);
    }
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    Window MYwindow;
    MYwindow.Create("MahjongGameProject_v999.0.1", "DXlib");
    SetUserWindow(MYwindow.GetHandle());
    SetUserWindowMessageProcessDXLibFlag(false);
    SetGraphMode(1280, 720, 32);
    if (DXLibError(DxLib_Init())) return dxLibError;
    MYwindow.Show();

    // file loading

    DxLib::SetMouseDispFlag(TRUE);
    DxLib::SetBackgroundColor(0, 0, 0);


    while (MYwindow.Loop()) {
        
        unsigned int color = GetColor(255, 255, 255);
        DrawFormatString(0, 0, color, "%s", "MJP_test");

        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

    }

	
	

	//WaitKey();					// キーの入力待ち((7-3)『WaitKey』を使用)

    DxLib::DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}