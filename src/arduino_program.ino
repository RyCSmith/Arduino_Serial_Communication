
unsigned long time;

// This method is called once
void setup(){
  // sets the transfer rate to 9600 baud
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

// This method is called repeatedly
void loop(){
  Serial.print("Time: ");
  time = millis();
  //prints time since program started
  Serial.println(time);
  // wait a second so as not to send massive amounts of data
  delay(1000);
  int message = Serial.read();
  if(message == 111){
    digitalWrite(13,HIGH);
  }
  if(message == 102){
    digitalWrite(13,LOW);
  }  
}
