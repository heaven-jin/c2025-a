#include "GameEngine.h"

int main() {
    // 设置控制台编码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 创建游戏引擎实例
    GameEngine game;
    
    // 运行游戏
    game.run();
    
    return 0;
}