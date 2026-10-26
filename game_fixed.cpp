#include <iostream>
#include <string>

const int MAX_MONSTERS = 5;

class Monster {
public:
    int id;
    std::string name;
    int health;
    int attack;

    Monster(int i, std::string n, int h, int a) {
        id = i;
        name = n;
        health = h;
        attack = a;
    }
};

class Game {
private:
    Monster* monsters[MAX_MONSTERS];
    int monsterCount;

public:
    Game() {
        monsterCount = 0;
        for (int i = 0; i < MAX_MONSTERS; i++)
            monsters[i] = nullptr;
    }

    ~Game() {
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i] != nullptr)
                delete monsters[i];
        }
    }

    // Disable copy to prevent double-free crash
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void spawnMonster(int id, std::string name, int health, int attack) {
        // Bounds check
        if (monsterCount >= MAX_MONSTERS) {
            std::cout << "Cannot spawn " << name << ", list is full.\n";
            return;
        }
        monsters[monsterCount] = new Monster(id, name, health, attack);
        monsterCount++;
    }

    Monster* findMonster(int id) {
        for (int i = 0; i < monsterCount; i++) {
            // Compare using ==
            if (monsters[i] != nullptr && monsters[i]->id == id)
                return monsters[i];
        }
        return nullptr;
    }

    int attackMonster(int id, int damage) {
        Monster* m = findMonster(id);
        // Null check
        if (m == nullptr)
            return -1;

        if (m->health <= damage) {
            m->health = 0;
            return 0;
        }
        m->health -= damage;
        return m->health;
    }

    int totalMonsterHealth() {
        int total = 0;
        // Strict < loop condition
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i] != nullptr)
                total += monsters[i]->health;
        }
        return total;
    }

    // Return by value, not reference
    std::string getMonsterLabel(int id) {
        Monster* m = findMonster(id);
        if (m == nullptr)
            return "Monster-Unknown";
        return "Monster-" + std::to_string(m->id) + ":" + m->name;
    }

    int averageAttack() {
        if (monsterCount == 0) return 0;
        double sum = 0.0; // Initialized to 0
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i] != nullptr)
                sum += monsters[i]->attack;
        }
        return sum / monsterCount;
    }

    void defeatMonster(int id) {
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i] != nullptr && monsters[i]->id == id) {
                // Print before delete to avoid use-after-free
                std::cout << "Defeated " << monsters[i]->name << "\n";
                delete monsters[i];

                for (int j = i; j < monsterCount - 1; j++)
                    monsters[j] = monsters[j + 1];

                monsters[monsterCount - 1] = nullptr;
                monsterCount--;
                return;
            }
        }
    }
};

double calculateDamage(int attack, int armorPercent) {
    return (attack * armorPercent) / 100.0;
}

int playerRank(int score) {
    if (score > 1000)
        return 1;
    else if (score > 100)
        return 2;
    else
        return 3; // Default return
}

void readBoss(Game& game) {
    std::string name;
    int health = 0;
    std::cout << "Enter boss name: ";
    std::cin >> name;
    std::cout << "Enter boss health: ";
    std::cin >> health;
    game.spawnMonster(100, name, health, 50);
}

int main() {
    Game game;
    game.spawnMonster(1, "Goblin", 30, 5);
    game.spawnMonster(2, "Orc", 60, 12);
    game.spawnMonster(3, "Skeleton", 40, 8);
    game.spawnMonster(4, "Troll", 90, 20);
    game.spawnMonster(5, "Dragon", 200, 45);
    game.spawnMonster(6, "Bat", 10, 3);

    Monster* bonus = new Monster(0, "Slime", 1, 0);

    std::cout << "Attack: " << game.attackMonster(99, 20) << "\n";
    std::cout << "Label: " << game.getMonsterLabel(2) << "\n";
    std::cout << "Total health: " << game.totalMonsterHealth() << "\n";
    std::cout << "Average attack: " << game.averageAttack() << "\n";
    std::cout << "Damage: " << calculateDamage(80, 25) << "\n";
    std::cout << "Rank: " << playerRank(50) << "\n";

    game.defeatMonster(3);
    readBoss(game);

    delete bonus; // Clean up bonus memory
    return 0;
}
