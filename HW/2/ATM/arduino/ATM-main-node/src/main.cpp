#include <Arduino.h>
#include <Dictionary.h>


struct
{
  String id ;
  String pass;
  int wallet_amount;
  String history[2]={"",""};
}custumer1,custumer2;

void  add_history(String id,String message){

  if(id=="1"){
    if(custumer1.history[0]==""){custumer1.history[0]=message;}
    else if (custumer1.history[1]==""){custumer1.history[1]=message;}
    else{custumer1.history[0]=custumer1.history[1];custumer1.history[1]=message;}
  } 
    if(id=="2"){
    if(custumer2.history[0]==""){custumer2.history[0]=message;}
    else if (custumer2.history[1]==""){custumer2.history[1]=message;}
    else{custumer2.history[0]=custumer2.history[1];custumer2.history[1]=message;}
  }
}
bool login(String id, String pass){
  if(id.equals("1")){if(pass.equals(custumer1.pass)){return true;}return false;}
  if(id.equals("2")){if(pass.equals(custumer2.pass)){return true;}return false;}
  return false;
}

void setup(){
  Serial.begin(9600);
  custumer1.id="1";custumer1.wallet_amount=500;custumer1.pass="1234";
  custumer2.id="2";custumer2.wallet_amount=500;custumer2.pass="4321";


}

void login_handler(){
  String id="",pass="";
      while (true)
      {
        if (Serial.available() && id==""){id=Serial.readStringUntil('\n');}
        else if(Serial.available() && id!=""){pass=Serial.readStringUntil('\n');break;}
      }
    id.trim();pass.trim();
    bool response=login(id,pass);
    if (response){Serial.println("1");}
    else{Serial.println("0");}

}

void wallet_handler(){
  String id="";
      while (true)
      {
        if (Serial.available() && id==""){id=Serial.readStringUntil('\n');break;}
        
      }
    id.trim();
     if(id=="1"){Serial.println(custumer1.wallet_amount);}
     if(id=="2"){Serial.println(custumer2.wallet_amount);}
     else{Serial.println("wrong id.");}
}

void transfer_handler(){
    String id="",des_id="",amount="";
      while (true)
      {
        if (Serial.available() && id==""){id=Serial.readStringUntil('\n');}
        if(Serial.available() && id!="" && des_id==""){des_id=Serial.readStringUntil('\n');}
        if(Serial.available() && id!="" && des_id!="" && amount==""){amount=Serial.readStringUntil('\n');break;}
        
      }
      id.trim();des_id.trim();amount.trim();
      if(id=="1"){
        if(custumer1.wallet_amount< amount.toInt()){Serial.println("no enough money");}
        else{
          if(des_id=="2"){
            custumer1.wallet_amount =custumer1.wallet_amount- amount.toInt();
            custumer2.wallet_amount =custumer2.wallet_amount+ amount.toInt();
            String history_log="1->2:"+amount; 
            add_history("1",history_log);
            add_history("2",history_log);
            Serial.println("done");

          }}}
      if(id=="2"){
        
        if(custumer2.wallet_amount< amount.toInt()){Serial.println("no enough money");}
        else{
          if(des_id=="1"){
            custumer2.wallet_amount =custumer2.wallet_amount- amount.toInt();
            custumer1.wallet_amount =custumer1.wallet_amount+ amount.toInt();
            String history_log="2 -> 1 :"+amount; 
            add_history("1",history_log);
            add_history("2",history_log);
            Serial.println("done");
          }    

        }}
}

void history_handler(){
   String id="";
      while (true)
      {
        if (Serial.available() && id==""){id=Serial.readStringUntil('\n');break;}
        
      }
    id.trim();
    if(id=="1"){Serial.println(custumer1.history[0]);Serial.println(custumer1.history[1]);}
    if(id=="2"){Serial.println(custumer2.history[0]);Serial.println(custumer2.history[1]);}

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    
    String message=Serial.readStringUntil('\n');
    message.trim();
    if(message=="login"){login_handler();}
    else if (message=="wallet"){wallet_handler();}
    else if (message=="transfer"){transfer_handler();}
    else if (message=="history"){history_handler();}

  }

  
}


