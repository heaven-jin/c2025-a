#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
struct Product {
    std::string model;
    int quantity;
    int id;
    double price;
    Product(const std::string& m = "", int q = 0, double p = 0.0) 
        : model(m), quantity(q), price(p) {}
};


class WarehouseManager {
private:
    std::vector<Product> inventory;
    const std::string filename = "inventory.txt";
    
public:
    // 构造函数，启动时加载数据
    WarehouseManager() {
        loadInventory();
    }
    
    // 析构函数，退出时保存数据
    ~WarehouseManager() {
        saveInventory();
    }
    

    void loadInventory() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "库存文件不存在，将创建新的库存文件。" << std::endl;
            return;
        }
        
        inventory.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            // 解析格式: 型号 数量 单价
            std::istringstream iss(line);
            std::string model;
            int quantity;
            double price;
            
            if (iss >> model >> quantity >> price) {
                inventory.push_back(Product(model, quantity, price));
            }
        }
        file.close();
        std::cout << "成功加载 " << inventory.size() << " 种商品信息。" << std::endl;
    }
    

    void saveInventory() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法保存库存文件！" << std::endl;
            return;
        }
        
        file << "# 库存数据文件 - 格式: 型号 数量 单价" << std::endl;
        for (const auto& product : inventory) {
            file << product.model << " " << product.quantity << " " 
                 << std::fixed << std::setprecision(2) << product.price << std::endl;
        }
        file.close();
        std::cout << "库存数据已保存。" << std::endl;
    }
    

    void displayInventory() {
        std::cout << "\n========== 当前库存列表 ==========" << std::endl;
        if (inventory.empty()) {
            std::cout << "库存为空！" << std::endl;
            return;
        }
        
        std::cout << std::left << std::setw(15) << "型号" 
                  << std::setw(10) << "数量" 
                  << std::setw(10) << "单价" 
                  << std::setw(15) << "总价值" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        double totalValue = 0;
        for (const auto& product : inventory) {
            double value = product.quantity * product.price;
            totalValue += value;
            
            std::cout << std::left << std::setw(15) << product.model
                      << std::setw(10) << product.quantity
                      << std::setw(10) << std::fixed << std::setprecision(2) << product.price
                      << std::setw(15) << std::fixed << std::setprecision(2) << value << std::endl;
        }
        
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "库存总价值: " << std::fixed << std::setprecision(2) << totalValue << std::endl;
    }
    
    // 查找商品索引
    int findProduct(const std::string& model) {
        for (size_t i = 0; i < inventory.size(); i++) {
            if (inventory[i].model == model) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    

    void stockIn() {
        std::cout << "\n========== 商品入库 ==========" << std::endl;
        std::string model;
        int quantity;
        double price;
        
        std::cout << "请输入商品型号: ";
        std::cin >> model;
        
        std::cout << "请输入入库数量: ";
        std::cin >> quantity;
        if (quantity <= 0) {
            std::cout << "入库数量必须大于0！" << std::endl;
            return;
        }
        
        std::cout << "请输入商品单价: ";
        std::cin >> price;
        if (price < 0) {
            std::cout << "商品单价不能为负数！" << std::endl;
            return;
        }
        
        int index = findProduct(model);
        if (index != -1) {
            // 商品已存在，更新数量和价格
            inventory[index].quantity += quantity;
            inventory[index].price = price;  // 更新为最新价格
            std::cout << "商品 " << model << " 入库成功！当前库存: " 
                      << inventory[index].quantity << std::endl;
        } else {
            // 新商品
            inventory.push_back(Product(model, quantity, price));
            std::cout << "新商品 " << model << " 入库成功！库存: " << quantity << std::endl;
        }
    }
    

    void stockOut() {
        std::cout << "\n========== 商品出库 ==========" << std::endl;
        if (inventory.empty()) {
            std::cout << "库存为空，无法出库！" << std::endl;
            return;
        }
        
        std::string model;
        int quantity;
        
        std::cout << "请输入商品型号: ";
        std::cin >> model;
        
        int index = findProduct(model);
        if (index == -1) {
            std::cout << "商品 " << model << " 不存在！" << std::endl;
            return;
        }
        
        std::cout << "当前库存: " << inventory[index].quantity << std::endl;
        std::cout << "请输入出库数量: ";
        std::cin >> quantity;
        
        if (quantity <= 0) {
            std::cout << "出库数量必须大于0！" << std::endl;
            return;
        }
        
        if (quantity > inventory[index].quantity) {
            std::cout << "库存不足！当前库存: " << inventory[index].quantity << std::endl;
            return;
        }
        
        inventory[index].quantity -= quantity;
        std::cout << "商品 " << model << " 出库成功！剩余库存: " 
                  << inventory[index].quantity << std::endl;
        
        // 如果库存为0，询问是否删除该商品
        if (inventory[index].quantity == 0) {
            char choice;
            std::cout << "该商品库存为0，是否从库存列表中删除？(y/n): ";
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                inventory.erase(inventory.begin() + index);
                std::cout << "商品 " << model << " 已从库存列表中删除。" << std::endl;
            }
        }
    }
    

    void showMenu() {
        std::cout << "\n========== 进销存管理系统 ==========" << std::endl;
        std::cout << "1. 显示存货列表" << std::endl;
        std::cout << "2. 入库" << std::endl;
        std::cout << "3. 出库" << std::endl;
        std::cout << "4. 退出程序" << std::endl;
        std::cout << "请选择操作 (1-4): ";
    }
    

    void run() {
        int choice;
        
        std::cout << "欢迎使用进销存管理系统！" << std::endl;
        
        while (true) {
            showMenu();
            std::cin >> choice;
            
            // 清除输入缓冲区
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            
            switch (choice) {
                case 1:
                    displayInventory();
                    break;
                case 2:
                    stockIn();
                    break;
                case 3:
                    stockOut();
                    break;
                case 4:
                    std::cout << "感谢使用进销存管理系统！" << std::endl;
                    return;
                default:
                    std::cout << "无效选择，请输入1-4之间的数字！" << std::endl;
                    break;
            }
            
            // 暂停，等待用户按键
            std::cout << "\n按回车键继续...";
            std::cin.get();
        }
    }
};

int main() {

    
    WarehouseManager manager;
    manager.run();
    return 0;
}