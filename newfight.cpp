#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Monster
{
public:
    string name;
    int health;
    int attackPower;

    Monster(string nm, int hp, int ap) : name(move(nm)), health(hp), attackPower(ap) {}
};

class Game
{
private:
    string username;
    int coins = 0;
    int weaponLevel = 0;
    int baseAttackPower = 10;
    double healthPoints = 20;
    bool isNewPlayer = true;
    bool signedToday = false;
    vector<int> dailyRewards = {5, 10, 15, 20, 25, 30, 40};
    int consecutiveDaysSignedIn = 0;

public:
    Game(const string &name) : username(name) {}

    void signIn()
    {
        if (!signedToday)
        {
            if (consecutiveDaysSignedIn < dailyRewards.size())
            {
                coins += dailyRewards[consecutiveDaysSignedIn];
                cout << "签到成功！获得 " << dailyRewards[consecutiveDaysSignedIn] << " 金币。" << endl;
            }
            else
            {
                coins += dailyRewards.back(); // 循环奖励
                cout << "签到成功！获得 " << dailyRewards.back() << " 金币。" << endl;
            }
            signedToday = true;
            consecutiveDaysSignedIn++;
            checkSignInAchievement();
        }
        else
        {
            cout << "今天你已经签到了！" << endl;
        }
    }

    void checkSignInAchievement()
    {
        if (consecutiveDaysSignedIn == 7 || consecutiveDaysSignedIn == 30 || consecutiveDaysSignedIn == 100)
        {
            cout << "恭喜你达成签到成就！" << endl;
            coins += 5;
            if (consecutiveDaysSignedIn == 7)
            {
                coins += 5;
                cout << "获得10金币！" << endl;
                this_thread::sleep_for(chrono::milliseconds(800));
            }
            else if (consecutiveDaysSignedIn == 30)
            {
                weaponLevel = 3;
                cout << "获得了5金币和传说级武器：大天恒健刃！" << endl;
                updatePlayerStats();
                this_thread::sleep_for(chrono::milliseconds(800));
            }
            else if (consecutiveDaysSignedIn == 100)
            {
                coins += 5;
                weaponLevel = 4;
                cout << "获得了15金币";
                updatePlayerStats();
                this_thread::sleep_for(chrono::milliseconds(800));
            }
        }
    }

    void resetSignIn()
    {
        signedToday = false;
        consecutiveDaysSignedIn = 0;
    }

    void updatePlayerStats()
    {
        baseAttackPower = weaponLevel * 5 + 10;
        healthPoints = weaponLevel * 1.5 + 20;
    }

    void fight(Monster &monster)
    {
        bool isPlayerAlive = true;
        while (isPlayerAlive && monster.health > 0)
        {
            cout << "你面对的是：" << monster.name << endl;
            cout << "怪物的生命值：" << monster.health << endl;
            cout << "你的攻击力：" << baseAttackPower << endl;
            cout << "你的生命值：" << healthPoints << endl;
            cout << "你的攻击！（输入攻击值，最大不超过你的攻击力）：";
            int playerAttack;
            cin >> playerAttack;
            if (playerAttack > baseAttackPower)
            {
                cout << "你的攻击值超过了你的攻击力！" << endl;
                continue;
            }
            monster.health -= playerAttack;
            if (monster.health <= 0)
            {
                cout << "你击败了怪物！" << endl;
                coins += 10; // 击败怪物获得金币
                isPlayerAlive = false;
            }
            else
            {
                cout << "怪物攻击你！（怪物攻击力：" << monster.attackPower << "）" << endl;
                healthPoints -= monster.attackPower;
                if (healthPoints <= 0)
                {
                    cout << "你被怪物击败了！" << endl;
                    isPlayerAlive = false;
                }
                else
                {
                    cout << "你的生命值：" << healthPoints << endl;
                }
            }
        }
        if (!isPlayerAlive)
        {
            cout << "战斗结束。" << endl;
            this_thread::sleep_for(chrono::milliseconds(1200));
            showMenu();
        }
        else
        {
            cout << "战斗结束，你安全返回。" << endl;
            this_thread::sleep_for(chrono::milliseconds(1200));
            showMenu();
        }
    }

    void purchaseWeapon(int weaponCost)
    {
        if (coins >= weaponCost)
        {
            coins -= weaponCost;
            weaponLevel++;
            cout << "武器购买成功！" << endl;
            updatePlayerStats();
        }
        else
        {
            cout << "金币不足，购买失败。" << endl;
        }
    }

    void showMenu()
    {
        cout << "菜单：" << endl;
        cout << "[1] 我的" << endl;
        cout << "[2] 签到" << endl;
        cout << "[3] 打怪" << endl;
        cout << "[4] 退出" << endl;
        cout << "[5] 商店" << endl;
        cout << "请输入选项：";
    }

    void run()
    {
        loadGame();
        int choice;
        while (true)
        {
            showMenu();
            cin >> choice;
            switch (choice)
            {
            case 1:
                showProfile();
                break;
            case 2:
                signIn();
                break;
            case 3:
                fight();
                break;
            case 4:
                cout << "感谢游戏，再见！" << endl;
                saveGame();
                return;
            case 5:
                shop();
                break;
            default:
                cout << "输入有误！请重新输入。" << endl;
                this_thread::sleep_for(chrono::milliseconds(1200));
                break;
            }
        }
    }

    void showProfile()
    {
        cout << "\n你的用户名：" << username << endl;
        cout << "你的金币数量为：" << coins << endl;
        cout << "你的武器为：";
        if (weaponLevel == 0)
        {
            cout << "无" << endl;
        }
        else if (weaponLevel == 1)
        {
            cout << "飞来龙枪" << endl;
        }
        else if (weaponLevel == 2)
        {
            cout << "控告之剑" << endl;
        }
        cout << "\nWNT游戏" << endl;
        copyrightNotice();
        this_thread::sleep_for(chrono::milliseconds(1200));
    }

    void fight()
    {
        int choice;
        cout << "\n\n战斗：";
        cout << "\n[1]主线任务(难度：#~###) [2]副本任务(难度：#~####) [3]挑战魔王(难度：#####) [4]退出\n请输入：";
        cin >> choice;
        updatePlayerStats();
        switch (choice)
        {
        case 1:
        {
            // 主线任务怪物
            Monster mainQuestMonster{"小怪", 30, 5};
            fightMonster(mainQuestMonster);
            break;
        }
        case 2:
        {
            // 副本任务怪物
            Monster dungeonMonster{"中怪", 50, 8};
            fightMonster(dungeonMonster);
            break;
        }
        case 3:
        {
            // 魔王
            Monster bossMonster{"大魔王", 100, 12};
            fightMonster(bossMonster);
            break;
        }
        case 4:
            showMenu();
            break;
        default:
            cout << "\n输入错误！";
            this_thread::sleep_for(chrono::milliseconds(800));
            showMenu();
            return;
        }
    }

    void shop()
    {
        cout << "商店" << endl;
        cout << "[1]飞来龙枪(10金币) [2]控告之剑(30金币) [3]退出\n请选择：";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            if (coins >= 10)
            {
                coins -= 10;
                cout << "购买成功！";
                weaponLevel = 1;
                return;
            }
            else
            {
                cout << "购买失败，钱不够！";
                return;
            }
        }
        else if (choice == 2)
        {
            if (coins >= 30)
            {
                coins -= 30;
                cout << "购买成功！";
                weaponLevel = 2;
                return;
            }
            else
            {
                cout << "购买失败，钱不够！";
                return;
            }
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            cout << "输入错误！";
            this_thread::sleep_for(chrono::milliseconds(800));
            return;
        }
    }

    void saveGame()
    {
        ofstream file((username + "_savegame.json").c_str());
        if (file.is_open())
        {
            json j;
            j["Username"] = username;
            j["Coins"] = coins;
            j["WeaponLevel"] = weaponLevel;
            file << j.dump(4); // 以美化格式保存 JSON
            file.close();
            cout << "游戏保存成功！" << endl;
        }
        else
        {
            cout << "无法保存游戏！" << endl;
        }
    }

    void loadGame()
    {
        ifstream file((username + "_savegame.json").c_str());
        if (file.is_open())
        {
            json j;
            file >> j;
            username = j["Username"];
            coins = j["Coins"];
            weaponLevel = j["WeaponLevel"];
            file.close();
            updatePlayerStats(); // 更新玩家状态
            cout << "游戏加载成功！" << endl;
        }
        else
        {
            cout << "未找到保存的游戏，开始新游戏。" << endl;
        }
    }

    void earnCoinsWhileAFK()
    {
        cout << "挂机赚金币中..." << endl;
        this_thread::sleep_for(chrono::seconds(5)); // 假设挂机5秒可以赚取1金币
        coins += 1;
        cout << "你通过挂机赚取了1金币。" << endl;
        saveGame();
    }

    void copyrightNotice()
    {
        cout << "\n\n\n\n----------------\n版权所有(c) Piaoztsdy2022-2024.\n";
    }

    void fightMonster(Monster &monster)
    {
        bool isPlayerAlive = true;
        while (isPlayerAlive && monster.health > 0)
        {
            cout << "你面对的是：" << monster.name << endl;
            cout << "怪物的生命值：" << monster.health << endl;
            cout << "你的攻击力：" << baseAttackPower << endl;
            cout << "你的生命值：" << healthPoints << endl;
            cout << "你的攻击！（输入攻击值，最大不超过你的攻击力）：";
            int playerAttack;
            cin >> playerAttack;
            if (playerAttack > baseAttackPower)
            {
                cout << "你的攻击值超过了你的攻击力！" << endl;
                continue;
            }
            monster.health -= playerAttack;
            if (monster.health <= 0)
            {
                cout << "你击败了怪物！" << endl;
                coins += 10; // 击败怪物获得金币
                isPlayerAlive = false;
            }
            else
            {
                cout << "怪物攻击你！（怪物攻击力：" << monster.attackPower << "）" << endl;
                healthPoints -= monster.attackPower;
                if (healthPoints <= 0)
                {
                    cout << "你被怪物击败了！" << endl;
                    isPlayerAlive = false;
                }
                else
                {
                    cout << "你的生命值：" << healthPoints << endl;
                }
            }
        }
        if (!isPlayerAlive)
        {
            cout << "战斗结束。" << endl;
            this_thread::sleep_for(chrono::milliseconds(1200));
            showMenu();
        }
        else
        {
            cout << "战斗结束，你安全返回。" << endl;
            this_thread::sleep_for(chrono::milliseconds(1200));
            showMenu();
        }
    }
};

int main()
{
    cout << "请输入您的用户名：";
    string username;
    cin >> username;
    Game game(username);
    game.run();
    return 0;
}