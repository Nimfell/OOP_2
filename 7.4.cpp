// Я что то увлеклась в реализации и сделала немного не то, но задание то уже нужно сдавать.
// В с++ - нет базовых типов, хотя может string подходит.
// Поэтому сделала иерархию GeneralShape -> AnyShape -> конкретные фигуры.
// В GeneralShape я обозначила обязательные виртуальные функции для фигур, 
// которые нужно определить для работы с ними и со стандартными методами GeneralShape.
// В AnyShape запретила переопределение некоторых функций в дочерних классах.
// Теперь их можно править в GeneralShape и в AnyShape. 
// Хотя GeneralShape - у нас по договорённости не изменяем
// (не знаю как его закрыть что бы наследование нормально работало).
// Из лишнего сделан дочерний класс сircle... И даже проверен переопределенный оператор сравнения.
// В коде не всё так гладно, остались пара вопросов. Но это уже по языку.
//--------------------------------------------------------------------------- 
#include "stdafx.h" 
#include <iostream>
#include <sstream> 
#include <list>
using namespace std;
 
class GeneralShape
{
protected:  
   double area;
   string info;

public:
   virtual bool operator==(const GeneralShape& obj) const = 0;
   virtual double        calculate_area()            =0; // расчет в зависимости от типа фигуры 
   virtual bool          copy_to (GeneralShape* obj) =0; // скопировать данные в другой объект
   virtual GeneralShape* get_clon()                  =0; // даёт ссылку на новую копию объекта
   virtual string*       shape_to_string()           =0; // данные объекта в строке
   virtual bool          string_to_shape(string)     =0; // копирование данных из строки в объект 
   
   double  get_area() {return (area = calculate_area()); } // обновить площадь и вернуть

   void print() { cout << shape_to_string() << endl; }     // вывод строкового обозначения

   bool equal_in_area(GeneralShape* obj_1, GeneralShape* obj_2) // сравнить любые фигуры по площади
   {  if(obj_1->get_area() == obj_2->get_area()) return true;
      else return false;
   }

   bool type_equality(GeneralShape* obj)  // типы фигур динаковые
   {  if (typeid(this) == typeid(*obj)) return true; // если тип данного объекта равен типу объекта по указателю
      else return false;
   }; 
};

class AnyShape: public GeneralShape
{ 
public:  
   virtual double get_area() sealed {return (area = calculate_area()); } // запрет на переопределение в дочерних классах
   virtual void   print() sealed  { cout << shape_to_string() << endl; }

   virtual bool equal_in_area (GeneralShape* obj_1, GeneralShape* obj_2) sealed
   {  if(obj_1->get_area() == obj_2->get_area()) return true;
      else return false;
   }

   virtual bool  type_equality (GeneralShape* obj) sealed 
   {  if (typeid(this) == typeid(*obj)) 
         return true; // если тип данного объекта равен типу объекта по указателю
      else 
         return false;
   };
};

class circle: public AnyShape
{  
private:   

public:   
   int radius;
    bool operator==(const GeneralShape& obj) const
    {   const circle *p = dynamic_cast<const circle *>(&obj);
        if (!p) return false;
        else    return radius == p->radius;
    }

   double calculate_area() // расчет в зависимости от типа фигуры 
   { return (area = 3.14 * radius * radius);
   }

   bool copy_to (GeneralShape* obj) // скопировать данные в другой объект того же типа
   { if ( type_equality(obj) ) 
      { //(*obj).radius = radius;
        //(*obj).calculate_area();
        return true; // удачно
       }
     else return false;
   }

   GeneralShape* get_clon()  // даёт ссылку на новую копию объекта
   { GeneralShape* new_circle = new circle;
     copy_to(new_circle);                    //всегда удачно
     GeneralShape* new_shape = new_circle;
     return new_shape;
   }

   string* shape_to_string() // данные объекта в строке
   { //info = "Circle with a radius of " + radius + ".";
     return &info;
   }

   // Некоторые неверные форматы смогут пролезть, 
   // но просто конвертация строк не является приорететной задачей в данном упражнении,
   // а мой навык владения string пока не так хорош.
   bool string_to_shape(string) // копирование данных из строки в объект 
   {  string chek_str = info.substr(1,24);
      if (chek_str == "Circle with a radius of ") // if (info.compare(1, 24, "Circle with a radius of ") const;)
      {  string str_radius[16];
         int j = 0;
         int strLength = info.length();
         for (int i = 25; i < strLength; i++)
         {  if (info[i] != '.')
               str_radius[j++] = info[i];
            else 
            {  str_radius[j] = info[i];
               break;
            }
         }
         if (str_radius[j] == ".")
         {  stringstream geek(radius);
            int x = 0; 
            geek >> x;  //запись в радиус символов до точки
            if (x != 0)
            {   radius = x;
                return true;
            }
         }         
      }
      return false;
   }

   circle()
   {  radius = 1;
   }

   circle(int r)
   {  radius = r;
   }

};

//class rect: public AnyShape
//{
//public:
//    int w, h;
// 
//    virtual bool operator==(const shape &obj) const
//    {
//        const rect *p = dynamic_cast<const rect *>(&obj);
//        if (!p) 
//           return false;
//        else    
//           return (w == p->w && h == p->h);
//    }
//};

int main(int argc, char* argv[])
{
    circle c;
    c.radius = 10;
 
    circle _c;
    _c.radius = 10;
 
    circle __c;
    __c.radius = 20;
 
    //rect r;
    //r.w = 10;
    //r.h = 20;
 
    //rect _r;
    //_r.w = 10;
    //_r.h = 20;
 
    //rect __r;
    //__r.w = 10;
    //__r.h = 30;
 
    std::list<GeneralShape *> list;
 
    list.push_back(&c);
    //list.push_back(&r);
 
    std::cout << "c == _c /true/: " << (c == _c) << std::endl;
    std::cout << "c == __c /false/: " << (c == __c) << std::endl;
    //std::cout << "r == _r /true/: " << (r == _r) << std::endl;
    //std::cout << "r == __r /false/: " << (r == __r) << std::endl;
    std::cout << "*list.front() == _c /true/: " << (*list.front() == _c) << std::endl;
    std::cout << "*list.front() == __c /false/: " << (*list.front() == __c) << std::endl;
    //std::cout << "*list.back() == _r /true/: " << (*list.back() == _r) << std::endl;
    //std::cout << "*list.back() == __r /false/: " << (*list.back() == __r) << std::endl;
 
    system("pause");
 
    return 0;
}
