#include <iostream>

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;

public:
    Fighter(const char* name, int health, int damage) : name(name), health(health), maxHealth(health), damage(damage) {
    }

    const char* GetName() const {
        return name;
    }

    void TakeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0;
        }
    }

    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }

    bool IsAlive() const {
        return health > 0;
    }

    void PrintStatus() const {
        std::cout << name << "HP: " << health << "/" << maxHealth << "\n";
    }
};

void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget;

public:
    Game()
        : player("hero", 30, 8),
        enemies{ Fighter("enemy 1", 12, 3), Fighter("enemy 2", 18, 4), Fighter("enemy 3", 24, 5) },
        currentTarget(nullptr) {

    }

    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;
        if (number < 1 || number > 3) return false;
        if (!enemies[number - 1].IsAlive()) return false;
        outTarget = &enemies[number - 1];
        return true;
    }

    bool AllEnemiesDefeated() const {
        for (int i = 0; i < 3; ++i) {
            if (enemies[i].IsAlive()) return false;
        }
        return true;
    }

    void Run() {
        std::cout << "Добро пожаловать в битву\n";

        while (true) {
            std::cout << "Игрок: "; PrintFighter(player);
            for (int i = 0; i < 3; ++i) {
                std::cout << (i + 1) << ". ";
                PrintFighter(enemies[i]);
                std::cout << "\n";
            }
            std::cout << "Меню: 1-выбрать цель, 2-атаковать, 0-выйти\n выбор: ";

            int command;
            std::cin >> command;

            if (command == 0) {
                std::cout << "вне зоны\n";
                break;
            }
            else if (command == 1) {
                std::cout << "Номер противника (1-3): ";
                int num;
                std::cin >> num;
                if (TrySelectTarget(num, currentTarget)) {
                    std::cout << "Цель: "; PrintFighter(*currentTarget);
                }
                else {
                    currentTarget = nullptr;
                    std::cout << "Неверный выбор или противник умер\n";
                }
            }
            else if (command == 2) {
                if (currentTarget == nullptr) {
                    std::cout << "Сначала выберите противника\n";
                }
                else {
                    player.Attack(*currentTarget);

                    if (!currentTarget->IsAlive()) {
                        std::cout << currentTarget->GetName() << " повержены!\n";
                        currentTarget = nullptr;

                        if (AllEnemiesDefeated()) {
                            std::cout << "Победа!\n";
                            break;
                        }
                    }
                    else {
                        currentTarget->Attack(player);

                        if (!player.IsAlive()) {
                            std::cout << "Поражение!\n";
                            break;
                        }
                    }
                }
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Game game;
    game.Run();
    return 0;
}