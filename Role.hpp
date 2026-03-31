#include <string>
#include <iostream>
#ifndef ROLE_HPP
#define ROLE_HPP
namespace final{

    class Fighter;
    class Protector;
    class Caster;

    //uncompleted, you should add code into this class
    class Base{
        friend class Fighter;
        friend class Protector;
        friend class Caster;
    private:
        //WARNING: NO OTHER MEMBER VARIABLE IS ALLOWED TO BE ADDED HERE.
        std::string name;
        int health,attack;
        bool alive,defensive;
        //you only need to define aliveNumber here.
        static int aliveNumber;
    public:
        //WARNING: NO OTHER PUBLIC FUNCTION IS ALLOWED TO BE ADDED.
        //WARNING: THIS CLASS(Base) CAN NOT BE INSTANTIATED（实例化）.
        //WARNING: THE FUNCTION PROVIDED NEED NO MODIFICATION
        //the function you need to implement or define is as follows.
        //(1) getAliveNumber
        //(2) Constructor
        //(3) launch_attack
        //(4) defend
        static int getAliveNumber(){
            return aliveNumber;
        }

        Base (const std::string& name, const int health,const int attack)
            : name(name), health(health), attack(attack), alive(true), defensive(false)
        {
            ++aliveNumber;
        }

        virtual ~Base() = default;

        virtual void launch_attack(Base* target) = 0;
        virtual void defend(bool flag) = 0;

        bool isAlive(){
            return alive;
        }

        bool isDefensive(){
            return defensive;
        }
    };

    class Fighter:public Base{
    public:
        //WARNING: NO OTHER PUBLIC FUNCTION IS ALLOWED TO BE ADDED.
        //WARNING: THE FUNCTION PROVIDED NEED NO MODIFICATION
        Fighter (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 3;
                attack += 2;
            } else {
                defensive = 0;
            }
        }
    };

    class Protector:public Base{
    public:
        //WARNING: NO OTHER PUBLIC FUNCTION IS ALLOWED TO BE ADDED.
        //WARNING: THE FUNCTION PROVIDED NEED NO MODIFICATION
        Protector (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 7;
                attack += 1;
            } else {
                defensive = 0;
            }
        }
    };

    class Caster:public Base{
    public:
        //WARNING: NO OTHER PUBLIC FUNCTION IS ALLOWED TO BE ADDED.
        //WARNING: THE FUNCTION PROVIDED NEED NO MODIFICATION
        Caster (const std::string& name, const int health,const int attack):Base(name,health,attack){}

        void launch_attack(Base* target);

        void defend(bool flag){
            if (flag) {
                defensive = 1;
                health += 5;
            } else {
                defensive = 0;
            }
        }
    };

    static inline int calc_multiplier_for_fighter(Base* target){
        return (dynamic_cast<Caster*>(target) != nullptr) ? 2 : 1;
    }
    static inline int calc_multiplier_for_protector(Base* target){
        return (dynamic_cast<Fighter*>(target) != nullptr) ? 2 : 1;
    }
    static inline int calc_multiplier_for_caster(Base* target){
        return (dynamic_cast<Protector*>(target) != nullptr) ? 2 : 1;
    }

    static inline const char* role_name_of(Base* obj){
        if (dynamic_cast<Fighter*>(obj)) return "Fighter";
        if (dynamic_cast<Protector*>(obj)) return "Protector";
        return "Caster";
    }

    void Fighter::launch_attack(Base *target) {
        int mult = calc_multiplier_for_fighter(target);
        int damage_to_target;
        if (!target->defensive) {
            damage_to_target = attack * mult;
        } else {
            int raw = attack * mult - target->attack;
            damage_to_target = raw > 0 ? raw : 0;
        }

        target->health -= damage_to_target;

        int damage_to_self = 0;
        if (target->defensive) {
            damage_to_self = target->attack; // Fighter is not Caster, so takes counter if target defended
        }
        health -= damage_to_self;

        // handle deaths: print target first if both die
        if (target->alive && target->health <= 0) {
            target->alive = false;
            --aliveNumber;
            std::cout << role_name_of(target) << ' ' << target->name << " is killed\n";
        }
        if (alive && health <= 0) {
            alive = false;
            --aliveNumber;
            std::cout << "Fighter " << name << " is killed\n";
        }
    }

    void Protector::launch_attack(Base *target) {
        int mult = calc_multiplier_for_protector(target);
        int damage_to_target;
        if (!target->defensive) {
            damage_to_target = attack * mult;
        } else {
            int raw = attack * mult - target->attack;
            damage_to_target = raw > 0 ? raw : 0;
        }

        target->health -= damage_to_target;

        int damage_to_self = 0;
        if (target->defensive) {
            damage_to_self = target->attack; // Protector is not Caster
        }
        health -= damage_to_self;

        if (target->alive && target->health <= 0) {
            target->alive = false;
            --aliveNumber;
            std::cout << role_name_of(target) << ' ' << target->name << " is killed\n";
        }
        if (alive && health <= 0) {
            alive = false;
            --aliveNumber;
            std::cout << "Protector " << name << " is killed\n";
        }
    }

    void Caster::launch_attack(Base *target) {
        int mult = calc_multiplier_for_caster(target);
        int damage_to_target;
        if (!target->defensive) {
            damage_to_target = attack * mult;
        } else {
            int raw = attack * mult - target->attack;
            damage_to_target = raw > 0 ? raw : 0;
        }

        target->health -= damage_to_target;

        // Caster does not take counter-damage even if target defended
        if (target->alive && target->health <= 0) {
            target->alive = false;
            --aliveNumber;
            std::cout << role_name_of(target) << ' ' << target->name << " is killed\n";
        }
        if (alive && health <= 0) {
            alive = false;
            --aliveNumber;
            std::cout << "Caster " << name << " is killed\n";
        }
    }
    
    // define static member
    int Base::aliveNumber = 0;
}
#endif //ROLE_HPP
