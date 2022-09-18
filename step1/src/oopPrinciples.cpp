#include <cstdint>
#include <vector>
#include <random>
#include <memory>

#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define kFallThrough    [[fallthrough]]
#endif

//--------------------------------------
enum class EnemyState {
    Idle,
    Patrolling,
    Chasing,
    Attacking,
    Dying,
    Dead,
};

//--------------------------------------
class Enemy {
public:
    Enemy() = default;
    virtual ~Enemy() = default;

    // [Encapsulation] Indirect access to the state (we could do some modification to damage due to armor, dodge, ...)
    virtual void Hit(uint32_t damage) { life -= damage * (1.0f - armor); }   

    // [Encapsulation] Direct access to the state
    float   GetLife() const { return life;  }
    // [Encapsulation] Indirect access to the state. Bonus: We hide the alive condition that could be complex
    bool    IsAlive() const { return life > 0.0f; }

    // [Abstraction] Hide implementation details exposing a high-level interface.
    virtual void    DoWork() {
        switch (state) {
            case EnemyState::Idle:
                ManageIdle();
                break;
        
            case EnemyState::Patrolling:
                ManagePatrol();
                break;
        
            case EnemyState::Chasing:
                ManageChase();
                break;
        
            case EnemyState::Attacking:
                ManageAttack();
                break;
        
            case EnemyState::Dying:
                ManageDead();
                break;
        
            case EnemyState::Dead:
                kFallThrough;
            default:
                break;
        }
    }

protected:
    // [Polymorphism] All derived classes must implement these methods
    virtual void ManageIdle()   = 0;
    virtual void ManagePatrol() = 0;
    virtual void ManageChase()  = 0;
    virtual void ManageAttack() = 0;
    virtual void ManageDead()   = 0;

protected:
    // [Encapsulation] The state is private or protected 
    float       life  { 42 };
    float       armor { 0 };
    EnemyState  state { EnemyState::Idle };
};

// Inheritance Orc inherits from Enemy
//--------------------------------------
class Orc : public Enemy {
public:
        Orc() { armor = 0.1f;  }

        // [Polymorphism] Reimplement method defined in base class
        void DoWork() override {
            // Do some actions before work

            Enemy::DoWork();    // [Inheritance] Reuse the main logic
            
            // Do some actions after work
        }

protected:
    // [Polymorphism] Concrete logic for every shared state
    void ManageIdle() override {}
    void ManagePatrol() override {}
    void ManageChase() override {}
    void ManageAttack() override {}
    void ManageDead() override {}
};

// Inheritance Troll inherits from Enemy
//--------------------------------------
class Troll : public Enemy {
public:
    Troll() {
        maxLife = life; // [Inheritance] we can use 'life' variable from base class
    }

    // [Polymorphism] Reimplement method defined in base class
    void DoWork() override {
        if (life < maxLife)
            life += healingRate;

        Enemy::DoWork();    // [Inheritance] Reuse the main logic
    }

protected:
    // [Polymorphism] Concrete logic for every shared state
    void ManageIdle() override {}
    void ManagePatrol() override {}
    void ManageChase() override {}
    void ManageAttack() override {}
    void ManageDead() override {}

protected:
    float       maxLife;
    float       healingRate { 0.1f };
};

//--------------------------------------
int
main() {
    std::vector<std::shared_ptr<Enemy>> enemies;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> random(0, 100);

    // Create some enemies
    for (size_t i = 0; i < 10; ++i) {
        if (random(mt) > 5)
            enemies.emplace_back(std::make_shared<Orc>());
        else
            enemies.emplace_back(std::make_shared<Troll>());
    }

    // ...

    // [Polymorphism] Same interface for different objects
    for (auto &enemy : enemies) {
        if(enemy->IsAlive())
            enemy->DoWork();
    }

    return 0;
}