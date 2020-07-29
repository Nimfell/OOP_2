#include "stdafx.h"
#include "iostream"
using namespace std;
//#ifndef __PORTS_H
//#include <MdR32F9x.h>
//#include <stdio.h>
//#include <SoftTimer.h>

class com_port // абстрактный класс вверху иерархии 
{    
protected:
    int has_error;   //единица, если была ошибка приема данных
    void error (void) { has_error = 1; }; 
    virtual void receive () = 0; // функция вызывается по наличую данных

public:
    unsigned char num_port;  //номер порта
    com_port(unsigned char com) {num_port = com;} 
    ~com_port (void){}
};

//---Класс протокола  Modbus---
class Modbus: public com_port 
{   
protected:      
    int IsTransfer;  // наличие 1, отсутствие 0 передачи по каналу

    unsigned char data_in[50];  //рабочий буфер приема
    unsigned char data_out[50]; //рабочий буфер передачи

    int j;   // счетчик событий (подсчет принятых сообщениё или времени простоя без них)
    int F_answ; //ответ получен 
    int F_ack;  //пакет принят (контрольная сумма совпала), запрос
    //TSoftTimer Time_out;  // тайм аут после приёма сообщения (5c по протоколу на скорости 9600)
    //-----------------
    int  *Uart; // аппаратный буфер для ожидания отправки следущего байта данных
    unsigned int MsgCount; // количество отправленных байтов
    unsigned int MsgSize;  // количество байтов на отправление

    //--перем.для расчета КС--
    unsigned int ks_msg;    // контрольная сумма в сообщении
    unsigned int ks_calc;   // контрольная сумма расчитанная по приему
    unsigned char *puchMsg; //сообщение
    unsigned int DataLen;   // длинна     
    unsigned int CRC16(unsigned char *puchMsg, unsigned int DataLen); //расчет кс с использованием табл.значений
    bool ks_are_equal(); // сравнение принятого сообщения и заявленной суммы в сообщении
    //-----------------------
    unsigned char query(unsigned char*, unsigned char, unsigned char, unsigned int, unsigned int); //составить запрос в data_out
    unsigned char answer(unsigned char*, unsigned char, unsigned char, unsigned int, unsigned int); //составить ответ в data_out     

public:
    void send_msg (){}; // отправить сообщение при наличии запроса
    unsigned char* get_msg (){}; // выдать последнее полученное сообщение 
    //---------
    virtual void receive () =0;//{cout << "receive Modbus" << endl;}; // функция вызывается по прерыванию по приему  
    virtual void disable_transfer (void) =0; // разрешить прием сообщений
    virtual void enable_transfer (void) =0;  // разрешить отправку сообщений 

    //конструктор   
    Modbus(unsigned char port, int *Uart_adr): com_port(port)
    { Uart = Uart_adr;
      has_error = 0;
      IsTransfer = j = 0;
    } 
};


class Modbus_Master: public Modbus 
{   
    int master_id; //адрес мастера в сообщении
    int* slaves;   // подчинненные устройства

public:
    void cycle_Master(void){}; //опрос подчиненных устройств в цикле с ожиданием ответа

    virtual void receive ()              {cout << "receive Modbus_Master" << endl;}; // функция вызывается по прерыванию по приему  
    virtual void disable_transfer (void) {cout << "disable Modbus_Master" << endl;}; // разрешить прием сообщений
    virtual void enable_transfer (void)  {cout << "enable  Modbus_Master" << endl;};  // разрешить отправку сообщений  

    //конструктор     
    Modbus_Master(unsigned char port, int* Uart_adr, int index, int* slaves_table): Modbus(port, Uart_adr)
    { slaves = slaves_table;  
      master_id = index;
      F_ack = 1; // начать опрос 
    } 
};

class Modbus_Slave: public Modbus 
{
    int master_id;  // адрес мастера в сообщении
    int slave_id;   // адресс подчиненного 
    //-----------------
public:
    void cycle_Slave(void){}; //отправление ответа при наличии запроса
    //---------
    virtual void receive (void)          {cout << "receive Modbus_Slave" << endl;}; // функция вызывается по прерыванию по приему  
    virtual void disable_transfer (void) {cout << "disable Modbus_Slave" << endl;}; // разрешить прием сообщений
    virtual void enable_transfer (void)  {cout << "enable  Modbus_Slave" << endl;};  // разрешить отправку сообщений  

    //конструктор 
    Modbus_Slave(unsigned char port, int *Uart_adr, int sl_index, int m_index): 
          Modbus(port, Uart_adr)
    { slave_id = sl_index;  
      master_id = m_index;
      F_ack = 0; // ожидать запрос
    } 
};

/*#define __PORTS_H
#endif*/ 

//==============================
//#include <MODBUS.h>
//#include <MdR32F9x.h>
#include <stdio.h>
//#include <SoftTimer.h>

int main(void)
{
   
   unsigned char port = 1;
   int Uart           = 2;
   int* Uart_adr      = &Uart;
   int index          = 0x12;
   int slaves[2]      = {0x01, 0x02};
   int* slaves_table  = &slaves[0];

   //-------------------------------------------------------
   Modbus_Master UART1(port, Uart_adr, index, slaves_table);
   Modbus_Slave  UART2(port, Uart_adr, slaves[1], index);

   Modbus *Port[2];

   Port[0] = &UART1;
   Port[1] = &UART2;

   Port[0]->receive(); // вызываются методы дочерних классов
   Port[1]->receive();      

	
}


