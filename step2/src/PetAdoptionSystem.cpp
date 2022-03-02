#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< general understanding of C/C++ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

class Animal {
protected:
    std::string name;
    int age;
    std::string breed;
    double height;
public:
    Animal() {}
    virtual ~Animal() {}

    void setName(std::string name) { this->name = name; }
    void setAge(int age) { this->age = age; }
    void setBreed(std::string breed) { this->breed = breed; }
    void setHeight(float height) { this->height = height; }

    std::string getName() { return this->name; }
    int getAge() { return this->age; }
    std::string getBreed() { return this->breed; }
    float getHeight() { return this->height; }

    virtual void makeSoud() = 0;
    virtual void draw() = 0;
};

class Dog : public Animal
{
public:
    Dog() {}
    void makeSoud() override {
        std::cout << "\n                   " << name << " says woof woof\n" << std::endl;
    };

    void draw() override {
        std::cout << "                                    __" << std::endl;
        std::cout << "                                   /\/'-," << std::endl;
        std::cout << "                           ,--'''''   /""" << std::endl;
        std::cout << "                     ____,'.  )       \___" << std::endl;
        std::cout << "                    ''''''------'''''-----'" << std::endl;

    }

    ~Dog() {

    }
    friend ofstream& operator<<(ofstream& ofs, Dog* d);
    friend ifstream& operator>>(ifstream& ifs, Dog* d);

};
// overloading ifstream and ofstream to accept class Dog
ifstream& operator >>(ifstream& ifs, Dog* d) {
    std::string name;
    int age;
    std::string breed;
    double height;

    ifs >> name >> age >> breed >> height;
    d->setName(name);
    d->setAge(age);
    d->setBreed(breed);
    d->setHeight(height);

    return ifs;
}   
ofstream& operator<<(ofstream& ofs, Dog* d) {
    ofs << d->getName() << std::endl;
    ofs << d->getAge() << std::endl;
    ofs << d->getBreed() << std::endl;
    ofs << d->getHeight() << std::endl;
    return ofs;
}

class Cat : public Animal
{
public:
    Cat() {}
    void makeSoud() override {
        std::cout << "\n                   " << name << " says meow meow\n" << std::endl;
    };
    void draw() override {
        std::cout << "                                 /*_/*   /*               " << std::endl;
        std::cout << "                                * o o * * *              " << std::endl;
        std::cout << "                               *   Y   */ /              " << std::endl;
        std::cout << "                              *         */               " << std::endl;
        std::cout << "                              * | | | | /                " << std::endl;
        std::cout << "                               `|_|-|_|'                 " << std::endl;
    }
    ~Cat() {

    }
    friend ofstream& operator<<(ofstream& ofs, Cat* c);
    friend ifstream& operator>>(ifstream& ifs, Cat* c);
};
// overloading ifstream and ofstream to accept class Cat
ifstream& operator>>(ifstream& ifs, Cat* c) {
    std::string name;
    int age;
    std::string breed;
    double height;

    ifs >> name >> age >> breed >> height;
    c->setName(name);
    c->setAge(age);
    c->setBreed(breed);
    c->setHeight(height);

    return ifs;
}
ofstream& operator<<(ofstream& ofs, Cat* c) {
    ofs << c->getName() << std::endl;
    ofs << c->getAge() << std::endl;
    ofs << c->getBreed() << std::endl;
    ofs << c->getHeight() << std::endl;
    return ofs;
}

//vectors that stores cats and dogs in the adoption center
std::vector <Cat*> CatsAv;
std::vector <Dog*> DogsAv;

class Costumer
{
private:
    std::string name;
    std::string lastName;
    long id;
    bool canAdopt;
public:

    void setName(std::string name) { this->name = name; }
    void setLastName(std::string lastName) { this->lastName = lastName; }
    void setId(long id) { this->id = id; }
    void setCanAdopt(bool canAdopt) { this->canAdopt = canAdopt; }

    std::string getName() { return this->name; }
    std::string getLastName() { return this->lastName; }
    long getId() { return this->id; }
    bool getCanAdopt() { return this->canAdopt; }
};


// functions called as final message when the animals are succesfully put in adoption
void dogAdded(Dog* dog) {
    cout << "***************************************************************************************************" << std::endl;
    cout << "Thank you for putting " << dog->getName()<< " in adoption, we'll let you know if someone adopts it." << std::endl;
    cout << "***************************************************************************************************" << std::endl;

    cin.get();
}

void catAdded(Cat* cat){
    cout << "***************************************************************************************************" << std::endl;
    cout << "Thank you for putting " << cat->getName() << " in adoption, we'll let you know if someone adopts it." << std::endl;
    cout << "***************************************************************************************************" << std::endl;
    cin.get();
}

// called when the user wants to put a dog for adoption
void dogForm(int& option) {
    option = 0;
    Dog* dog = new Dog();

    std::string name;
    int age;
    std::string breed;
    double height;

    system("CLS");
    std::cout << "\n                         <<<<<<<<<<<< PLEASE GIVE US INFORMATION ABOUT THE ANIMAL >>>>>>>>>>\n" << std::endl;
    std::cout << "Insert the dog's name: ";
    std::cin >> name;
    dog->setName(name);

    std::cout << "Insert the dog's age: ";
    std::cin >> age;
    dog->setAge(age);

    std::cout << "Insert the dog's breed: ";
    std::cin >> breed;
    dog->setBreed(breed);

    std::cout << "Insert the dog's height(cm): ";
    std::cin >> height;
    dog->setHeight(height);

    DogsAv.push_back(dog);

    std::ofstream  dogofs("DogData.txt", ios::trunc);
    for (int i = 0; i < DogsAv.size(); i++) {
        dogofs << DogsAv[i];
    }
    dogofs.close();

    dogAdded(dog);

    std::cout << "\n|||||||||||||||||||||||||||||||| PRESS ENTER TO EXIT ||||||||||||||||||||||||||||||||\n" << std::endl;
}

// called when the user wants to put a cat for adoption
void catForm(int& option) {
    option = 0;
    Cat* cat = new Cat();

    std::string name;
    int age;
    std::string breed;
    double height;

    system("CLS");
    std::cout << "\n                         <<<<<<<<<<<< PLEASE GIVE US INFORMATION ABOUT THE ANIMAL >>>>>>>>>>\n" << std::endl;
    std::cout << "Insert the cat's name: ";
    std::cin >> name;
    cat->setName(name);

    std::cout << "Insert the cat's age: ";
    std::cin >> age;
    cat->setAge(age);

    std::cout << "Insert the cat's breed: ";
    std::cin >> breed;
    cat->setBreed(breed);

    std::cout << "Insert the cat's height(cm): ";
    std::cin >> height;
    cat->setHeight(height);

    CatsAv.push_back(cat);

    std::ofstream  catfs("CatData.txt", ios::trunc);
    for (int i = 0; i < CatsAv.size(); i++) {
        catfs << CatsAv[i];
    }
    catfs.close();

    catAdded(cat);
}

//called when the users wants to put an animal for adoption
void putUpOnAdoption(int& option) {
    system("CLS");
    option = 0;
    do {
        std::cout << "\n                         <<<<<<<<<<<< PLEASE SELECT WHAT ANIMAL YOU WANT TO ADOPT >>>>>>>>>>\n" << std::endl;
        std::cout << "1. a DOG" << std::endl;
        std::cout << "2. a CAT" << std::endl;
        std::cout << "3. << BACK <<" << std::endl;

        std::cin >> option;
        switch (option)
        {
        case 1:
            dogForm(option);
            break;
        case 2:
            catForm(option);
            break;       
        case 3:
            option = 5;
            return;           
        }
    } while (option > 2);
}

// called when the user is in conditions to adopt a dog
void chooseDog() {
    system("CLS");
    int option = 0;
    std::cout << "\n                         <<<<<<<<<<<< PLEASE CHOOSE A DOG >>>>>>>>>>\n" << std::endl;
    for (int i = 0; i < DogsAv.size(); i++) {
        std::cout << i << ". " << DogsAv[i]->getName() << " is " << DogsAv[i]->getAge() << " years old, it's a " << DogsAv[i]->getBreed() << " and it's " << DogsAv[i]->getHeight() << "cm height" << std::endl;
    }

    std::cout << "Select the dog: ";
    std::cin >> option;


    system("CLS");
    std::cout << "\n                            <<<<<<<<<<<< CONGRATULATIONS >>>>>>>>>>\n" << std::endl;
    std::cout << "\n                  <<<<<<<<<<<< You Adopted " << DogsAv[option]->getName() << "  >>>>>>>>>>\n" << std::endl;
    DogsAv[option]->draw();
    DogsAv[option]->makeSoud();
    DogsAv.erase(DogsAv.begin() + option);

    std::ofstream  dogofs("DogData.txt", ios::trunc);
    for (int i = 0; i < DogsAv.size(); i++) {
        dogofs << DogsAv[i];
    }
    dogofs.close();

    std::cout << "\n|||||||||||||||||||||||||||||||| PRESS ENTER TO EXIT ||||||||||||||||||||||||||||||||\n" << std::endl;



}

// called when the user is in conditions to adopt a dog
void chooseCat() {
    system("CLS");
    int option = 0;
    std::cout << "\n                         <<<<<<<<<<<< PLEASE CHOOSE A CAT >>>>>>>>>>\n" << std::endl;
    for (int i = 0; i < CatsAv.size(); i++) {
        std::cout << i << ". " << CatsAv[i]->getName() << " is " << CatsAv[i]->getAge() << " years old, it's a " << CatsAv[i]->getBreed() << " and it's " << CatsAv[i]->getHeight() << "cm height" << std::endl;
    }

    std::cout << "Select the cat: ";
    std::cin >> option;


    system("CLS");
    std::cout << "\n                            <<<<<<<<<<<< CONGRATULATIONS >>>>>>>>>>\n" << std::endl;
    std::cout << "\n                  <<<<<<<<<<<< You Adopted " << CatsAv[option]->getName() << "  >>>>>>>>>>\n" << std::endl;
    CatsAv[option]->draw();
    CatsAv[option]->makeSoud();
    CatsAv.erase(CatsAv.begin() + option);

    std::ofstream  catofs("CatData.txt", ios::trunc);
    for (int i = 0; i < CatsAv.size(); i++) {
        catofs << CatsAv[i];
    }
    catofs.close();

    std::cout << "\n|||||||||||||||||||||||||||||||| PRESS ENTER TO EXIT ||||||||||||||||||||||||||||||||\n" << std::endl;



}

//called when the users wants to adopt a cat
void adoptCat(int& option) {
    option = 0;

    Costumer* costumer = new Costumer();

    std::string name = "";
    std::string lastName = "";
    long id = 0;
    char canAdoptchar;


    system("CLS");
    std::cout << "\n                         <<<<<<<<<<<< ADOPT A CAT >>>>>>>>>>\n" << std::endl;
    std::cout << "\n                    <<<<<<<<<<<< Please fill the form >>>>>>>>>>\n" << std::endl;

    std::cout << "Your name: ";
    std::cin >> name;
    costumer->setName(name);

    std::cout << "Your last name: ";
    std::cin >> lastName;
    costumer->setLastName(lastName);

    std::cout << "Insert your ID number: ";
    std::cin >> id;
    costumer->setId(id);

    std::cout << "Are you in conditions to adopt a cat? y/n ";
    std::cin >> canAdoptchar;
    bool canAdopt = (canAdoptchar == 'y') ? true : false;
    costumer->setCanAdopt(canAdopt);

    do {
        std::cout << "          1. << Back <<                                        2. >> Next >>" << std::endl;
        std::cout << "Select an option: ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            option = 4;
            delete costumer;
            return;
        case 2:
            if (costumer->getCanAdopt())
                chooseCat();
            else
            {
                std::cout << "Sorry, you are not in conditions to have a cat" << std::endl;
                option = 3;
            }
            break;
        }
    } while (option > 2);
}

//called when the users wants to adopt a dog
void adoptDog(int& option) {
    option = 0;

    Costumer* costumer = new Costumer();

    std::string name = "";
    std::string lastName = "";
    long id = 0;
    char canAdoptchar;


    system("CLS");
    std::cout << "\n                         <<<<<<<<<<<< ADOPT A DOG >>>>>>>>>>\n" << std::endl;
    std::cout << "\n                    <<<<<<<<<<<< Please fill the form >>>>>>>>>>\n" << std::endl;

    std::cout << "Your name: ";
    std::cin >> name;
    costumer->setName(name);

    std::cout << "Your last name: ";
    std::cin >> lastName;
    costumer->setLastName(lastName);

    std::cout << "Insert your ID number: ";
    std::cin >> id;
    costumer->setId(id);

    std::cout << "Are you in conditions to adopt a dog? y/n ";
    std::cin >> canAdoptchar;
    bool canAdopt = (canAdoptchar == 'y') ? true : false;
    costumer->setCanAdopt(canAdopt);

    do {
        std::cout << "          1. << Back <<                                        2. >> Next >>" << std::endl;
        std::cout << "Select an option: ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            option = 4;
            delete costumer;
            return;
        case 2:
            if (costumer->getCanAdopt())
                chooseDog();
            else
            {
                std::cout << "Sorry, you are not in conditions to have a dog" << std::endl;
                option = 3;
            }
            break;
        }
    } while (option > 2);
}

// called when the user wants to adopt an animal
void adoptAnimalSelction(int& option) {
    option = 0;
    do {
        system("CLS");

        std::cout << "\n                         <<<<<<<<<<<< ADOPT AN ANIMAL >>>>>>>>>>\n" << std::endl;
        std::cout << "1. Adopt a dog" << std::endl;
        std::cout << "2. Adopt a cat" << std::endl;
        std::cout << "3. << Back <<" << std::endl;
        std::cin >> option;
        switch (option)
        {
        case 1:
            adoptDog(option);
            break;
        case 2:
            adoptCat(option);
            break;
        case 3:
            option = 4;
            return;
        }
    } while (option > 3);
}



void AdoptionSystemMain() {
    int option = 0;
    do {
        system("CLS");
        std::cout << "                        THIS PROGRAM CAN SHOWS SOME OF MY C++ KNOWLEDGE" << std::endl;
        std::cout << "This is an animal adoption system, that writes the data in .txt files allowing that data to persist in the system" << std::endl;
        std::cout << "     So whenever the execution is stopped, the changes will be saved for the next time the program executes" << std::endl;
        std::cout << "\n                 <<<<<<<<<<<< WELCOME TO THE ANIMAL ADOPTION SYSTEM >>>>>>>>>>>>\n" << std::endl;
        std::cout << "\n                       <<<<<<<<<<<<< Please select an option >>>>>>>>>>>>\n\n\n" << std::endl;
        std::cout << "1. I want to adopt an animal." << std::endl;
        std::cout << "2. I want to put up for adoption." << std::endl;
        std::cout << "3. Exit" << std::endl;

        std::cout << "Option: ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            adoptAnimalSelction(option);
            break;
        case 2:
            putUpOnAdoption(option);
            break;
        }
    } while (option > 3);
}



void CreateInitialDogData() {

    // INIT DOG DATA //

    Dog* dog1 = new Dog();
    dog1->setName("Rex");
    dog1->setAge(10);
    dog1->setBreed("Husky");
    dog1->setHeight(90.3);

    Dog* dog2 = new Dog();
    dog2->setName("Leena");
    dog2->setAge(4);
    dog2->setBreed("Mixed");
    dog2->setHeight(80.4);

    Dog* dog3 = new Dog();
    dog3->setName("Firulais");
    dog3->setAge(8);
    dog3->setBreed("Doberman");
    dog3->setHeight(100.01);

    Dog* dog4 = new Dog();
    dog4->setName("Luna");
    dog4->setAge(8);
    dog4->setBreed("Dalmata");
    dog4->setHeight(88.5);

    std::ofstream  dogofs("DogData.txt", ios::app);
    dogofs << dog1;
    dogofs << dog2;
    dogofs << dog3;
    dogofs << dog4;
    dogofs.close();
}
void CreateInitialCatData() {

    // INIT CAT DATA // 

    Cat* cat1 = new Cat();
    cat1->setName("Mona");
    cat1->setAge(4);
    cat1->setBreed("Romanian");
    cat1->setHeight(40.3);

    Cat* cat2 = new Cat();
    cat2->setName("Wendell");
    cat2->setAge(5);
    cat2->setBreed("Romanian");
    cat2->setHeight(40.1);

    Cat* cat3 = new Cat();
    cat3->setName("Michi");
    cat3->setAge(2);
    cat3->setBreed("Siames");
    cat3->setHeight(29.01);

    Cat* cat4 = new Cat();
    cat4->setName("Garfield");
    cat4->setAge(9);
    cat4->setBreed("Exotic");
    cat4->setHeight(48.5);

    std::ofstream  catofs("CatData.txt", ios::app);
    catofs << cat1;
    catofs << cat2;
    catofs << cat3;
    catofs << cat4;
    catofs.close();
}

//reads all cats from a created .txt file and asigns its values to vector of Cats CatsAV

void ReadCatsInVector() {
    ifstream(catifs);
    catifs.open("CatData.txt");
    if (!catifs) {
        CreateInitialCatData();
        catifs.open("CatData.txt");
    }
    std::string line = "";
    int cont = 0;
    for (cont = 0; std::getline(catifs, line); ++cont);
    cout << "Lines in file: " << cont;
    int objects = cont / 4;

    catifs.close();
    catifs.open("CatData.txt");

    for (int i = 0; i < objects; i++) {
        CatsAv.push_back(new Cat());
        catifs >> CatsAv[i];
    }
}

//reads all dogs from a created .txt file and asigns its values to vector of Dogs DogsAv

void ReadDogsInVector() {
    ifstream(dogifs);
    dogifs.open("DogData.txt");
    if (!dogifs) {
        CreateInitialDogData();
        dogifs.open("DogData.txt");
    }

    std::string line = "";
    int cont = 0;
    for (cont = 0; std::getline(dogifs, line); ++cont);
    cout << "Lines in file: " << cont;

    int objects = cont / 4;

    dogifs.close();
    dogifs.open("DogData.txt");


    for (int i = 0; i < objects; i++) {
        DogsAv.push_back(new Dog());
        dogifs >> DogsAv[i];
    }

}


int main()
{   
    ReadCatsInVector();
    ReadDogsInVector();   
    system("CLS");
    AdoptionSystemMain(); // calls to the adoption system main menu
}


