#include "GameEngine.h"

// ConfigManager 实现
bool ConfigManager::loadObjectConfigs(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开物体配置文件: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    ObjectType currentType = EMPTY;
    ObjectConfig currentConfig;
    bool readingObject = false;
    int lineCount = 0;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        if (line[0] == '[' && line.back() == ']') {
            // 保存前一个物体配置
            if (readingObject) {
                objectConfigs[currentType] = currentConfig;
            }
            
            // 开始新物体配置
            std::string typeName = line.substr(1, line.length() - 2);
            if (typeName == "WALL") currentType = WALL;
            else if (typeName == "BOX") currentType = BOX;
            else if (typeName == "TARGET") currentType = TARGET;
            else if (typeName == "BOX_ON_TARGET") currentType = BOX_ON_TARGET;
            else if (typeName == "PLAYER") currentType = PLAYER;
            else if (typeName == "PLAYER_ON_TARGET") currentType = PLAYER_ON_TARGET;
            else if (typeName == "EMPTY") currentType = EMPTY;
            
            currentConfig = ObjectConfig();
            readingObject = true;
            lineCount = 0;
        } else if (readingObject) {
            if (lineCount == 0) {
                // 读取配置行：宽度 高度 可碰撞 可推动
                std::istringstream iss(line);
                int collidable, pushable;
                iss >> currentConfig.width >> currentConfig.height >> collidable >> pushable;
                currentConfig.collidable = (collidable == 1);
                currentConfig.pushable = (pushable == 1);
            } else {
                // 读取显示字符
                currentConfig.display.push_back(line);
            }
            lineCount++;
        }
    }
    
    // 保存最后一个物体配置
    if (readingObject) {
        objectConfigs[currentType] = currentConfig;
    }
    
    file.close();
    return true;
}

bool ConfigManager::loadGameConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开游戏配置文件: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            gameSettings[key] = value;
        }
    }
    
    file.close();
    return true;
}

ObjectConfig ConfigManager::getObjectConfig(ObjectType type) const {
    auto it = objectConfigs.find(type);
    if (it != objectConfigs.end()) {
        return it->second;
    }
    return ObjectConfig(); // 返回默认配置
}

std::string ConfigManager::getGameSetting(const std::string& key) const {
    auto it = gameSettings.find(key);
    if (it != gameSettings.end()) {
        return it->second;
    }
    return "";
}

int ConfigManager::getObjectWidth() const {
    std::string width = getGameSetting("OBJECT_WIDTH");
    return width.empty() ? 3 : std::stoi(width);
}

int ConfigManager::getObjectHeight() const {
    std::string height = getGameSetting("OBJECT_HEIGHT");
    return height.empty() ? 3 : std::stoi(height);
}

// ScoreManager 实现
ScoreManager::ScoreManager(const std::string& file) : filename(file) {}

void ScoreManager::loadScores() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return; // 文件不存在，忽略
    }
    
    scores.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        int level, steps;
        std::string playerName;
        
        if (iss >> level >> steps >> playerName) {
            scores.push_back(ScoreRecord(level, steps, playerName));
        }
    }
    file.close();
}

void ScoreManager::saveScores() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法保存分数文件！" << std::endl;
        return;
    }
    
    file << "# 推箱子游戏分数记录 - 格式: 关卡 步数 玩家名" << std::endl;
    for (const auto& score : scores) {
        file << score.level << " " << score.steps << " " << score.playerName << std::endl;
    }
    file.close();
}

void ScoreManager::addScore(int level, int steps, const std::string& playerName) {
    scores.push_back(ScoreRecord(level, steps, playerName));
    saveScores();
}

int ScoreManager::getBestScore(int level) {
    int bestSteps = INT_MAX;
    for (const auto& score : scores) {
        if (score.level == level && score.steps < bestSteps) {
            bestSteps = score.steps;
        }
    }
    return (bestSteps == INT_MAX) ? 0 : bestSteps;
}

void ScoreManager::displayScores(int level) {
    std::cout << "\n========== 分数排行榜 ==========" << std::endl;
    if (scores.empty()) {
        std::cout << "暂无分数记录！" << std::endl;
        return;
    }
    
    std::vector<ScoreRecord> sortedScores = scores;
    std::sort(sortedScores.begin(), sortedScores.end(), 
              [](const ScoreRecord& a, const ScoreRecord& b) {
                  if (a.level != b.level) return a.level < b.level;
                  return a.steps < b.steps;
              });
    
    std::cout << std::left << std::setw(8) << "关卡" 
              << std::setw(8) << "步数" 
              << std::setw(15) << "玩家" << std::endl;
    std::cout << std::string(31, '-') << std::endl;
    
    for (const auto& score : sortedScores) {
        if (level == -1 || score.level == level) {
            std::cout << std::left << std::setw(8) << score.level
                      << std::setw(8) << score.steps
                      << std::setw(15) << score.playerName << std::endl;
        }
    }
}

// Renderer 实现
Renderer::Renderer(ConfigManager* config) : configManager(config), bufferWidth(0), bufferHeight(0) {}

void Renderer::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Renderer::hideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Renderer::clearScreen() {
    system("cls");
}

void Renderer::initBuffer(int width, int height) {
    bufferWidth = width;
    bufferHeight = height;
    screenBuffer.resize(bufferHeight);
    for (int y = 0; y < bufferHeight; y++) {
        screenBuffer[y].resize(bufferWidth, " ");
    }
}

void Renderer::clearBuffer() {
    for (int y = 0; y < bufferHeight; y++) {
        for (int x = 0; x < bufferWidth; x++) {
            screenBuffer[y][x] = " ";
        }
    }
}

void Renderer::renderToBuffer(const GameObject& obj) {
    ObjectConfig config = configManager->getObjectConfig(obj.type);
    
    // 计算在缓冲区中的起始位置
    int startX = obj.position.x * configManager->getObjectWidth();
    int startY = obj.position.y * configManager->getObjectHeight();
    
    // 渲染物体到缓冲区
    for (int y = 0; y < config.height && y < (int)config.display.size(); y++) {
        int bufferY = startY + y;
        if (bufferY >= 0 && bufferY < bufferHeight) {
            std::string line = (y < (int)config.display.size()) ? config.display[y] : "";
            
            // 确保行长度正确
            while ((int)line.length() < config.width) {
                line += ' ';
            }
            if ((int)line.length() > config.width) {
                line = line.substr(0, config.width);
            }
            
            // 将字符逐个写入缓冲区
            for (int x = 0; x < config.width && x < (int)line.length(); x++) {
                int bufferX = startX + x;
                if (bufferX >= 0 && bufferX < bufferWidth) {
                    screenBuffer[bufferY][bufferX] = line.substr(x, 1);
                }
            }
        }
    }
}

void Renderer::flushBuffer() {
    setCursorPosition(0, 0);
    
    for (int y = 0; y < bufferHeight; y++) {
        for (int x = 0; x < bufferWidth; x++) {
            std::cout << screenBuffer[y][x];
        }
        if (y < bufferHeight - 1) {
            std::cout << std::endl;
        }
    }
}

void Renderer::renderObject(const GameObject& obj) {
    ObjectConfig config = configManager->getObjectConfig(obj.type);
    
    for (int y = 0; y < config.height && y < (int)config.display.size(); y++) {
        setCursorPosition(obj.position.x * configManager->getObjectWidth(), 
                         obj.position.y * configManager->getObjectHeight() + y);
        
        std::string line = (y < (int)config.display.size()) ? config.display[y] : "";
        // 确保行长度正确
        while ((int)line.length() < config.width) {
            line += ' ';
        }
        if ((int)line.length() > config.width) {
            line = line.substr(0, config.width);
        }
        
        std::cout << line;
    }
}

void Renderer::renderGame(const GameState& game) {
    // 计算缓冲区大小
    int totalWidth = game.width * configManager->getObjectWidth();
    int totalHeight = game.height * configManager->getObjectHeight();
    
    // 初始化缓冲区
    initBuffer(totalWidth, totalHeight);
    clearBuffer();
    
    // 渲染所有物体到缓冲区
    for (int y = 0; y < game.height; y++) {
        for (int x = 0; x < game.width; x++) {
            GameObject obj(game.map[y][x], Vector2D(x, y));
            renderToBuffer(obj);
        }
    }
    
    // 清屏并输出缓冲区
    clearScreen();
    flushBuffer();
}

void Renderer::renderUI(const GameState& game) {
    int uiY = game.height * configManager->getObjectHeight() + 1;
    
    setCursorPosition(0, uiY);
    std::cout << "推箱子游戏 - 移动: WASD, 退出: Q, 重置: R";
    
    setCursorPosition(0, uiY + 1);
    std::cout << "关卡: " << game.currentLevel 
              << " | 进度: " << game.completedBoxes << "/" << game.boxCount 
              << " | 步数: " << game.steps;
    
    if (game.completedBoxes == game.boxCount && game.boxCount > 0 && !game.levelCompleted) {
        setCursorPosition(0, uiY + 2);
        std::cout << "恭喜过关！用了 " << game.steps << " 步完成关卡 " << game.currentLevel << "！";
        setCursorPosition(0, uiY + 3);
        std::cout << "按 N 进入下一关，按 Q 退出游戏";
    }
}

// LevelManager 实现
LevelManager::LevelManager(ConfigManager* config) : configManager(config) {}

bool LevelManager::loadLevel(const std::string& filename, GameState& game) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开关卡文件: " << filename << std::endl;
        return false;
    }
    
    // 读取地图尺寸
    std::string sizeLine;
    std::getline(file, sizeLine);
    if (sizeLine.size() < 5 || sizeLine[0] != '#' || sizeLine[3] != '#') {
        std::cerr << "关卡文件格式错误" << std::endl;
        return false;
    }
    
    try {
        game.width = std::stoi(sizeLine.substr(1, 2));
        game.height = std::stoi(sizeLine.substr(4, 2));
    } catch (...) {
        std::cerr << "关卡尺寸解析错误" << std::endl;
        return false;
    }
    
    // 初始化游戏状态
    game.map.resize(game.height, std::vector<ObjectType>(game.width, EMPTY));
    game.objects.clear();
    game.boxCount = 0;
    game.completedBoxes = 0;
    game.steps = 0;
    game.levelCompleted = false;
    
    // 读取地图内容
    std::string line;
    for (int y = 0; y < game.height && std::getline(file, line); y++) {
        for (int x = 0; x < game.width && x < line.size(); x++) {
            char c = line[x];
            ObjectType type = EMPTY;
            
            switch (c) {
                case '#': type = WALL; break;
                case 'T': type = TARGET; break;
                case 'B': type = BOX; game.boxCount++; break;
                case 'X': type = BOX_ON_TARGET; game.boxCount++; game.completedBoxes++; break;
                case 'P': type = PLAYER; game.playerPos = Vector2D(x, y); break;
                case 'S': type = PLAYER_ON_TARGET; game.playerPos = Vector2D(x, y); break;
                default: type = EMPTY; break;
            }
            
            game.map[y][x] = type;
            if (type != EMPTY) {
                game.objects.push_back(GameObject(type, Vector2D(x, y)));
            }
        }
    }
    
    file.close();
    return true;
}

bool LevelManager::loadLevelByNumber(int levelNumber, GameState& game) {
    std::string filename = getLevelFilename(levelNumber);
    if (loadLevel(filename, game)) {
        game.currentLevel = levelNumber;
        return true;
    }
    return false;
}

std::string LevelManager::getLevelFilename(int levelNumber) {
    return "levels/level" + std::to_string(levelNumber) + ".txt";
}

// GameEngine 实现
GameEngine::GameEngine() : renderer(&configManager), levelManager(&configManager) {}

bool GameEngine::initialize() {
    // 加载配置文件
    if (!configManager.loadObjectConfigs("objects/objects.txt")) {
        std::cerr << "加载物体配置失败！" << std::endl;
        return false;
    }
    
    if (!configManager.loadGameConfig("objects/game_config.txt")) {
        std::cerr << "加载游戏配置失败！" << std::endl;
        return false;
    }
    
    // 加载分数记录
    scoreManager.loadScores();
    
    // 设置控制台
    renderer.hideCursor();
    
    // 获取玩家姓名
    std::cout << "欢迎来到推箱子游戏！" << std::endl;
    std::cout << "请输入您的姓名: ";
    std::getline(std::cin, playerName);
    if (playerName.empty()) {
        playerName = "Player";
    }
    
    return true;
}

void GameEngine::run() {
    if (!initialize()) {
        std::cerr << "游戏初始化失败！" << std::endl;
        return;
    }
    
    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                startGame(1);
                break;
            case 2: {
                std::cout << "请输入关卡号: ";
                int level;
                std::cin >> level;
                std::cin.ignore();
                startGame(level);
                break;
            }
            case 3:
                scoreManager.displayScores();
                std::cout << "\n按任意键返回菜单...";
                _getch();
                break;
            case 4:
                std::cout << "感谢游戏！" << std::endl;
                return;
            default:
                std::cout << "无效选择！" << std::endl;
                Sleep(1000);
                break;
        }
    }
}

void GameEngine::startGame(int startLevel) {
    if (!levelManager.loadLevelByNumber(startLevel, game)) {
        std::cerr << "无法加载关卡 " << startLevel << "，按任意键返回菜单..." << std::endl;
        _getch();
        return;
    }
    
    originalGame = game;
    
    // 显示关卡最佳分数
    int bestScore = scoreManager.getBestScore(game.currentLevel);
    if (bestScore > 0) {
        std::cout << "关卡 " << game.currentLevel << " 最佳分数: " << bestScore << " 步" << std::endl;
        std::cout << "按任意键开始游戏...";
        _getch();
    }
    
    gameLoop();
}

void GameEngine::gameLoop() {
    while (true) {
        // 渲染游戏
        renderer.renderGame(game);
        renderer.renderUI(game);
        
        // 处理输入
        InputResult input = handleInput();
        
        if (input.quit) {
            std::cout << "\n感谢游戏！" << std::endl;
            break;
        }
        
        if (input.reset) {
            game = originalGame;
            continue;
        }
        
        if (input.nextLevel && game.levelCompleted) {
            // 记录分数
            scoreManager.addScore(game.currentLevel, game.steps, playerName);
            
            // 尝试加载下一关
            if (levelManager.loadLevelByNumber(game.currentLevel + 1, game)) {
                originalGame = game;
                
                // 显示下一关最佳分数
                int bestScore = scoreManager.getBestScore(game.currentLevel);
                if (bestScore > 0) {
                    int uiY = game.height * configManager.getObjectHeight() + 4;
                    renderer.setCursorPosition(0, uiY);
                    std::cout << "关卡 " << game.currentLevel << " 最佳分数: " << bestScore << " 步";
                }
            } else {
                int uiY = game.height * configManager.getObjectHeight() + 4;
                renderer.setCursorPosition(0, uiY);
                std::cout << "恭喜！您已完成所有关卡！";
                renderer.setCursorPosition(0, uiY + 1);
                std::cout << "按 Q 退出游戏";
            }
            continue;
        }
        
        // 移动玩家
        if (!game.levelCompleted && (input.direction.x != 0 || input.direction.y != 0)) {
            movePlayer(input.direction);
            
            // 检查是否完成关卡
            if (game.completedBoxes == game.boxCount && game.boxCount > 0) {
                game.levelCompleted = true;
            }
        }
        
        // 控制游戏速度
        Sleep(50);
    }
}

void GameEngine::showMenu() {
    renderer.clearScreen();
    std::cout << "========== 推箱子游戏 ==========" << std::endl;
    std::cout << "玩家: " << playerName << std::endl;
    std::cout << "1. 开始游戏" << std::endl;
    std::cout << "2. 选择关卡" << std::endl;
    std::cout << "3. 查看分数排行榜" << std::endl;
    std::cout << "4. 退出游戏" << std::endl;
    std::cout << "请选择 (1-4): ";
}

bool GameEngine::checkCollision(const Vector2D& position, ObjectType objectType) {
    if (!isValidPosition(position)) {
        return true;
    }
    
    ObjectType type = game.map[position.y][position.x];
    ObjectConfig config = configManager.getObjectConfig(type);
    
    return config.collidable;
}

bool GameEngine::isBox(const Vector2D& position) {
    if (!isValidPosition(position)) {
        return false;
    }
    
    ObjectType type = game.map[position.y][position.x];
    return type == BOX || type == BOX_ON_TARGET;
}

bool GameEngine::movePlayer(const Vector2D& direction) {
    Vector2D newPos = game.playerPos + direction;
    
    // 检查是否碰撞墙壁
    if (checkCollision(newPos)) {
        return false;
    }
    
    // 检查是否有箱子
    if (isBox(newPos)) {
        Vector2D boxNewPos = newPos + direction;
        
        // 检查箱子是否可以移动
        if (checkCollision(boxNewPos) || isBox(boxNewPos)) {
            return false;
        }
        
        // 移动箱子
        ObjectType boxOldType = game.map[newPos.y][newPos.x];
        ObjectType boxNewType = game.map[boxNewPos.y][boxNewPos.x];
        
        // 更新原箱子位置
        game.map[newPos.y][newPos.x] = (boxOldType == BOX_ON_TARGET) ? TARGET : EMPTY;
        
        // 更新新箱子位置
        if (boxNewType == TARGET) {
            game.map[boxNewPos.y][boxNewPos.x] = BOX_ON_TARGET;
            game.completedBoxes++;
        } else {
            game.map[boxNewPos.y][boxNewPos.x] = BOX;
            // 如果箱子从目标点移开，减少完成计数
            if (boxOldType == BOX_ON_TARGET) {
                game.completedBoxes--;
            }
        }
    }
    
    // 移动玩家
    ObjectType oldType = game.map[game.playerPos.y][game.playerPos.x];
    ObjectType newType = game.map[newPos.y][newPos.x];
    
    // 更新原玩家位置
    game.map[game.playerPos.y][game.playerPos.x] = (oldType == PLAYER_ON_TARGET) ? TARGET : EMPTY;
    
    // 更新新玩家位置
    game.map[newPos.y][newPos.x] = (newType == TARGET || newType == BOX_ON_TARGET) ? PLAYER_ON_TARGET : PLAYER;
    
    // 更新玩家位置
    game.playerPos = newPos;
    game.steps++;
    return true;
}

InputResult GameEngine::handleInput() {
    InputResult result;
    if (_kbhit()) {
        char input = _getch();
        switch (input) {
            case 'w': case 'W': result.direction = Vector2D(0, -1); break;
            case 's': case 'S': result.direction = Vector2D(0, 1); break;
            case 'a': case 'A': result.direction = Vector2D(-1, 0); break;
            case 'd': case 'D': result.direction = Vector2D(1, 0); break;
            case 'r': case 'R': result.reset = true; break;
            case 'q': case 'Q': result.quit = true; break;
            case 'n': case 'N': result.nextLevel = true; break;
        }
    }
    return result;
}

Vector2D GameEngine::getGridPosition(const Vector2D& pixelPos) {
    return Vector2D(pixelPos.x / configManager.getObjectWidth(), 
                   pixelPos.y / configManager.getObjectHeight());
}

Vector2D GameEngine::getPixelPosition(const Vector2D& gridPos) {
    return Vector2D(gridPos.x * configManager.getObjectWidth(), 
                   gridPos.y * configManager.getObjectHeight());
}

bool GameEngine::isValidPosition(const Vector2D& position) {
    return position.x >= 0 && position.x < game.width && 
           position.y >= 0 && position.y < game.height;
}