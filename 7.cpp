#include <stdio.h>
#include "iostream"
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

class Parent
{  // динамический полиморфизм (даёт возможность вызова переопределенной функции для дочернего класса)
   virtual void PutChar(char ch)  { cout << Letter << endl; }  

 public:
   char Letter; 
   void PutLetter() { Letter = 'p'; }  
   void Print()     { PutChar(Letter); }  

   Parent () {Letter = 'P';}
};
class Child : public Parent
{  
    virtual void PutChar(char ch)   { cout << Letter << endl;} // динамический полиморфизм
  public:
    void PutLetter() { cin >> Letter ; } //не вызывается из указателя типа родительского класса

    Child () {Letter = 'C';}
};


 

int main(void)
{   
   // 1 задание ================================================================= 
   //Пример динамического связывания:
   int (*Function)(int, int) = add;  // Создаём указатель на функцию add
   
   int foo;
   cin >> foo;

   if (foo%2)   
      Function = add;
   else
      Function = subtract;  
   
   cout << Function(4, 5) << endl; // вызов ф-ции выбранной во время выполнения

   //Пример статического связывания:
   cout << add(4, 5) << endl; 
   cout << subtract(4, 5) << endl; 
   
   // 2ое задание ================================================================= 
   // Динамический полиморфизм достигнут посредствам использоания виртуальных функций
   // при каждом вызове виртуальной функции родительский класс будет искать реализацию
   // этой функции в дочерних классах для каждого конкретного типа.
   // А если реализации нет, то будет использована ф-ция по умолчанию.
   // Использование конкретных методов определяется во время выполнения программы.
   Parent* ptr[2];
   ptr[0] = new Parent;
   ptr[1] = new Child;

   ptr[0]->Print();     // родительский метод (Parent) - динам
   ptr[1]->Print();     // дочерний метод     (Child)  - динам
   ptr[0]->PutLetter(); // родительский метод (Parent) - стат
   ptr[1]->PutLetter(); // родительский метод (Child)  - стат
   ptr[0]->Print();     // родительский метод (Parent) - динам
   ptr[1]->Print();     // дочерний метод     (Child)  - динам
	
}
