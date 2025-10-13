#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <climits>

// 二维向量结构
struct Vector2D {
    int x, y;
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }
};

// 物体类型枚举
enum ObjectType {
    EMPTY = 0,
    WALL = 1,
    TARGET = 2,
    BOX = 3,
    BOX_ON_TARGET = 4,
    PLAYER = 5,
    PLAYER_ON_TARGET = 6
};

// 物体配置结构
struct ObjectConfig {
    int width, height;
    bool collidable;
    bool pushable;
    std::vector<std::string> display;
    
    ObjectConfig() : width(3), height(3), collidable(false), pushable(false) {}
};

// 游戏物体结构
struct GameObject {
    ObjectType type;
    Vector2D position;
    ObjectConfig config;
    
    GameObject(ObjectType t = EMPTY, Vector2D pos = Vector2D(0, 0)) 
        : type(t), position(pos) {}
};

// 游戏状态结构
struct GameState {
    std::vector<std::vector<ObjectType>> map;
    std::vector<GameObject> objects;
    Vector2D playerPos;
    int width, height;
    int boxCount;
    int completedBoxes;
    int steps;
    int currentLevel;
    bool levelCompleted;
    
    GameState() : width(0), height(0), boxCount(0), completedBoxes(0), 
                  steps(0), currentLevel(1), levelCompleted(false) {}
};

// 分数记录结构
struct ScoreRecord {
    int level;
    int steps;
    std::string playerName;
    
    ScoreRecord(int l = 0, int s = 0, const std::string& name = "Player") 
        : level(l), steps(s), playerName(name) {}
};

// 输入结果结构
struct InputResult {
    Vector2D direction;
    bool reset;
    bool nextLevel;
    bool quit;
    
    InputResult() : direction(0, 0), reset(false), nextLevel(false), quit(false) {}
};

// 配置管理器类
class ConfigManager {
private:
    std::map<ObjectType, ObjectConfig> objectConfigs;
    std::map<std::string, std::string> gameSettings;
    
public:
    bool loadObjectConfigs(const std::string& filename);
    bool loadGameConfig(const std::string& filename);
    ObjectConfig getObjectConfig(ObjectType type) const;
    std::string getGameSetting(const std::string& key) const;
    int getObjectWidth() const;
    int getObjectHeight() const;
};

// 分数管理器类
class ScoreManager {
private:
    std::vector<ScoreRecord> scores;
    std::string filename;
    
public:
    ScoreManager(const std::string& file = "data/scores.txt");
    void loadScores();
    void saveScores();
    void addScore(int level, int steps, const std::string& playerName = "Player");
    int getBestScore(int level);
    void displayScores(int level = -1);
};

// 渲染器类
class Renderer {
private:
    ConfigManager* configManager;
    std::vector<std::vector<std::string>> screenBuffer; // 屏幕缓冲区
    int bufferWidth;  // 缓冲区宽度
    int bufferHeight; // 缓冲区高度
    
    // 初始化缓冲区
    void initBuffer(int width, int height);
    
public:
    Renderer(ConfigManager* config);
    void setCursorPosition(int x, int y);
    void hideCursor();
    void clearScreen();
    
    // 缓冲区相关方法
    void clearBuffer();
    void renderToBuffer(const GameObject& obj);
    void flushBuffer();
    
    // 渲染方法
    void renderObject(const GameObject& obj);
    void renderGame(const GameState& game);
    void renderUI(const GameState& game);
};

// 关卡管理器类
class LevelManager {
private:
    ConfigManager* configManager;
    
public:
    LevelManager(ConfigManager* config);
    bool loadLevel(const std::string& filename, GameState& game);
    bool loadLevelByNumber(int levelNumber, GameState& game);
    std::string getLevelFilename(int levelNumber);
};

// 游戏引擎主类
class GameEngine {
private:
    ConfigManager configManager;
    ScoreManager scoreManager;
    Renderer renderer;
    LevelManager levelManager;
    GameState game;
    GameState originalGame;
    std::string playerName;
    
public:
    GameEngine();
    bool initialize();
    void run();
    void startGame(int startLevel = 1);
    void gameLoop();
    void showMenu();
    
    // 游戏逻辑方法
    bool checkCollision(const Vector2D& position, ObjectType objectType = EMPTY);
    bool isBox(const Vector2D& position);
    bool movePlayer(const Vector2D& direction);
    InputResult handleInput();
    
    // 工具方法
    Vector2D getGridPosition(const Vector2D& pixelPos);
    Vector2D getPixelPosition(const Vector2D& gridPos);
    bool isValidPosition(const Vector2D& position);
};

#endif // GAMEENGINE_H