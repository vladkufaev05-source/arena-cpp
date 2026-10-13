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

    void Heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
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
    int medkits;

public:
    Game()
        : player("hero", 30, 8),
        enemies{ Fighter("enemy 1", 12, 3), Fighter("enemy 2", 18, 4), Fighter("enemy 3", 24, 5) },
        currentTarget(nullptr),
        medkits(1) {

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
            std::cout << "\n Бой" << std::endl;
            std::cout << "Игрок: "; PrintFighter(player);
            std::cout << "Аптечки: " << medkits << std::endl;
            for (int i = 0; i < 3; ++i) {
                std::cout << (i + 1) << ". ";
                PrintFighter(enemies[i]);
                std::cout << "\n";
            }
            std::cout << "____________" << std::endl;
            std::cout << "Меню: 1-выбрать цель, 2-атаковать, 3-аптечка, 0-выйти\n выбор: ";

            int command;
            if (!(std::cin >> command)) {
                std::cout << "Ошибка ввода\n";
                break;
            }

            if (command == 0) {
                std::cout << "вне зоны\n";
                break;
            }
            else if (command == 1) {
                std::cout << "Номер противника (1-3): ";
                int num;
                if (!(std::cin >> num)) {
                    std::cout << "Ошибка ввода\n";
                    break;
                }
                if (TrySelectTarget(num, currentTarget)) {
                    std::cout << "Цель: "; PrintFighter(*currentTarget);
                    std::cout << std::endl;
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
            else if (command == 3) {
                if (currentTarget == nullptr) {
                    std::cout << "Сначала выберите цель\n";
                }
                else if (medkits <= 0) {
                    std::cout << "У вас нет аптечек\n";
                }
                else {
                    std::cout << "Вы используете аптечку\n";
                    player.Heal(8);
                    medkits--;
                    std::cout << "Игрок: "; PrintFighter(player);

                    currentTarget->Attack(player);

                    if (!player.IsAlive()) {
                        std::cout << "Поражение!\n";
                        break;
                    }
                }
            }
            else {
                std::cout << "Неизвестная команда\n";
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