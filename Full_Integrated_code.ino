#include <Encoder.h>
int xleft, xlefttemp;
int xright, xrighttemp;//clockwise as seen from the right is positive(encoder reading) for both the tyres and the flippers
//int xleftmotor, xrightmotor;
Encoder myEncleft(43,42);
Encoder myEncright(40,41);
/*
Encoder riityre(36,37);
Encoder letyre(39,38);
*/
int pwm3=10; // ASSUMING ITS LEFT FLIPPER AS SEEN FROM BACK
int m3d1=45;
int m3d2=44;

int pwm4=11; // ASSUMING ITS LEFT FLIPPER AS SEEN FROM BACK
int m4d1=46;
int m4d2=47;

int pwm1=2;
int m1d1=25;
int m1d2=23;
int pwm2=3;
int m2d1=27;
int m2d2=29;

double rpmleftflipglo = 0;
double rpmrightflipglo = 0;
/*
double rpmleftmotglo = 0;
double rpmrightmotglo = 0;
*/
double right_velocity = 0;
double left_velocity = 0;

void setup() {
  Serial.begin(9600);// put your setup code here, to run once:
  pinMode(m3d1, OUTPUT);
  pinMode(m3d2, OUTPUT);
  pinMode(pwm3, OUTPUT);

  pinMode(m4d1, OUTPUT);
  pinMode(m4d2, OUTPUT);
  pinMode(pwm4, OUTPUT);

  pinMode(m1d1, OUTPUT);
  pinMode(m1d2, OUTPUT);
  pinMode(m2d1, OUTPUT);
  pinMode(m2d2, OUTPUT);

  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);

  }
unsigned long previousMillisr = 0; 
unsigned long previousMillisl = 0; 

double vleft = 0;
double vright = 0;
int angle = 0;

unsigned long currentMillis;
void loop() {
  currentMillis=millis();
  xlefttemp = myEncleft.read();
  char c;
  if( xlefttemp >= 0){
   xleft = xlefttemp % 3600;
  }
  else xleft = 3600 + xlefttemp % 3600;

  xrighttemp = myEncright.read();
  if( xrighttemp >= 0){
   xright = xrighttemp % 2700;
  }
  else xright = 2700 + xrighttemp % 2700;
 if(Serial.available() > 0 ){
  c = Serial.read();
  if(c == 'g'){
    angle = 90;
    Fliper_Hold_Angle(angle);
  }
  else if(c == 'w'){
    generate_values_from_position(0, 255, 0.5, 0.5);
  }
  else if(c == 's'){
    generate_values_from_position(0, 0, 0.5, 0.5);
  }
  else if(c == 'h'){
    angle = 0;
    Fliper_Hold_Angle(angle);
  }
 }
 Fliper_Hold_Angle(angle);
  /*Serial.print( "right " );
  Serial.print( xright );
  Serial.print( "left" );
  Serial.println( xleft );
  Fliper_Hold_Angle(90);*/
}

double getting_rpm_motor_flipper_left(){
 double xlnew;
 xlnew = xleft;
 if (currentMillis - previousMillisl >= 15) {
   double s1l = xlnew;
   xleft = myEncleft.read();
   double s2l = xleft;
   double rpml;
   rpml = 1.111 * ( s2l - s1l ) ;
   rpmleftflipglo = rpml;
   previousMillisl = currentMillis;
   return rpml;
 }
 else return rpmleftflipglo;
}

double getting_rpm_motor_flipper_right(){
 double xrnew = xright;
 if (currentMillis - previousMillisr >= 15) {
 double s1r = xrnew;
 xright = myEncright.read();
 double s2r = xright;
 double rpmr; 
 rpmr = 1.111*(s2r-s1r);
 rpmrightflipglo = rpmr;
 previousMillisr = currentMillis;
 return rpmr;
 }
 else return rpmrightflipglo;
}  

void Fliper_Hold_Angle(int theta){
 
  double eleft, eright;
//for the right flipper as seen from back
  double kpleft = 0.147;
  double kdleft = 4.27;

  double kpright = 0.147;
  double kdright = 6.5;
  int fperleft = 10 * theta;
  int fperright = 7.5 * theta;
  vleft = getting_rpm_motor_flipper_left();
  vright = getting_rpm_motor_flipper_right();
  
  if(xleft <= ( fperleft - 20 ) && (fperleft-20 - xleft) <= 1800 ){
    digitalWrite(m3d1, LOW);// its clockwise and is the right flipper
    digitalWrite(m3d2, HIGH);
    eleft = (( fperleft-20 ) - xleft) * kpleft - kdleft * vleft;
    if( eleft >= 570){
      eleft = 570;
    }
    else if( eleft <= 0){
      eleft = -1;
    }
    else eleft = eleft;
    eleft = map(eleft, 0, 570, 160, 0);
    analogWrite(pwm3, eleft);
  }

  else if(xleft <= ( fperleft - 20 ) && (fperleft-20 - xleft) > 1800 ){
    digitalWrite(m3d1, HIGH);// its clockwise and is the right flipper
    digitalWrite(m3d2, LOW);
    eleft = (( fperleft-20 ) - xleft) * kpleft - kdleft * vleft;
    if( eleft >= 570){
      eleft = 570;
    }
    else if( eleft <= 0){
      eleft = -1;
    }
    else eleft = eleft;
    eleft = map(eleft, 0, 570, 160, 0);
    analogWrite(pwm3, eleft);
 }
  else if(xleft >= (fperleft+20) && (xleft - fperleft +20) <= 1800 ){
    digitalWrite(m3d1, HIGH);// its anti clockwise and is the right flipper
    digitalWrite(m3d2, LOW);
    eleft = (( 20 - fperleft ) + xleft ) * kpleft - kdleft * vleft;
    if( eleft >= 570){
      eleft = 570;
    }
    else if( eleft <= 0){
      eleft = -1;
    }
    else eleft = eleft;
    eleft = map(eleft, 0, 570, 140, 0);
    analogWrite(pwm3, eleft);
  }
 else if(xleft >= (fperleft+20) && (xleft - fperleft +20) > 1800 ){
    digitalWrite(m3d1, LOW);// its anti clockwise and is the right flipper
    digitalWrite(m3d2, HIGH);
    eleft = (( 20 - fperleft ) + xleft) * kpleft - kdleft * vleft;
    if( eleft >= 570){
      eleft = 570;
    }
    else if( eleft <= 0){
      eleft = -1;
    }
    else eleft = eleft;
    eleft = map(eleft, 0, 570, 140, 0);
    analogWrite(pwm3, eleft);   
 }

  else{
    digitalWrite(m3d1, LOW);//assuming its anti clockwise and is the right flipper
    digitalWrite(m3d2, LOW);
    analogWrite(pwm3,-1);    
  }

/************right flipper code****************/
    if(xright <= ( fperright - 15 ) && (fperright-15 - xright) <= 1350 ){
    digitalWrite(m4d1, LOW);// its clockwise and is the right flipper
    digitalWrite(m4d2, HIGH);
    eright = (( fperright-15 ) - xright) * kpright - ( kdright * vright );
    if( eright >= 570){
      eright = 570;
    }
    else if( eright <= 0){
      eright = -1;
    }
    else eright = eright;
    eright = map(eright, 0, 570, 160, 0);
    analogWrite(pwm4, eright);
  }

  else if(xright <= ( fperright - 15 ) && (fperright-15 - xright) > 1350 ){
    digitalWrite(m4d1, HIGH);// its clockwise and is the right flipper
    digitalWrite(m4d2, LOW);
    eright = (( fperright-15 ) - xright) * kpright - kdright * vright;
   if( eright >= 570){
      eright = 570;
    }
    else if( eright <= 0){
      eright = -1;
    }
    else eright = eright;
    eright = map(eright, 0, 570, 160, 0);
    analogWrite(pwm4, eright);
 }
  else if(xright >= (fperright+15) && (xright - fperright +15) <= 1350 ){
    digitalWrite(m4d1, HIGH);// its anti clockwise and is the right flipper
    digitalWrite(m4d2, LOW);
    eright = (( 15 - fperright ) + xright ) * kpright - kdright * vright;
    if( eright >= 570){
      eright = 570;
    }
    else if( eright <= 0){
      eright = -1;
    }
    else eright = eright;
    eright = map(eright, 0, 570, 140, 0);
    analogWrite(pwm4, eright);
  }
 else if(xright >= (fperright+15) && (xright - fperright +15) > 1350 ){
    digitalWrite(m4d1, LOW);// its anti clockwise and is the right flipper
    digitalWrite(m4d2, HIGH);
    eright = (( 15 - fperright ) + xright) * kpright - kdright * vright;
    if( eright >= 570){
      eright = 570;
    }
    else if( eright <= 0){
      eright = -1;
    }
    else eright = eright;
    eright = map(eright, 0, 570, 140, 0);
    analogWrite(pwm4, eright);   
 }

  else{
    digitalWrite( m4d1, LOW );//assuming its anti clockwise and is the right flipper
    digitalWrite( m4d2, LOW );
    analogWrite( pwm4,-1 );    
  }
   

}

int motor_correction(int pwm_value_original) {
  return 150 - (30.0 * pwm_value_original) / 51; // to map [0, 255] to [150, 0]
}

void write_values(int pwm1_value, int m1d1_value, int m1d2_value, int pwm2_value, int m2d1_value, int m2d2_value) {
  analogWrite(pwm1, motor_correction(pwm1_value));
  digitalWrite(m1d1, m1d1_value);
  digitalWrite(m1d2, m1d2_value);

  analogWrite(pwm2, motor_correction(pwm2_value));
  digitalWrite(m2d1, m2d1_value);
  digitalWrite(m2d2, m2d2_value);
}


void generate_values_from_position(int x_position, int y_position, int k_y, int k_x) {
  
  if(abs(x_position) >= 30 || abs(y_position) >= 30){
  /*double rstar = (sqrt( x_position*x_position + y_position*y_position)) * 0.018;
  double k = (1.46 - atan(-5 * (rstar - 2)))/7.5225;*/
  /* int left_velocity =  k_x * y_position +  x_position);
   int right_velocity =  k * (x_position -  y_position);
*/
 int left_velocity = k_y * y_position + k_x * x_position,
      right_velocity = k_y * y_position - k_x * x_position;
  if(left_velocity >= 0) {
    if(right_velocity >= 0) {
      write_values(left_velocity, LOW, HIGH, right_velocity, LOW, HIGH);
    }
    else if(right_velocity < 0) {
      write_values(left_velocity, LOW, HIGH, -right_velocity, HIGH, LOW);
    }
  }
  else if(left_velocity < 0) {
    if(right_velocity >= 0) {
      write_values(-left_velocity, HIGH, LOW, right_velocity, LOW, HIGH);
    }
    else if(right_velocity < 0) {
      write_values(-left_velocity, HIGH, LOW, -right_velocity, HIGH, LOW);
    }
  }
}
else{
  analogWrite(pwm1,-1);
  digitalWrite(m1d1, HIGH);
  digitalWrite(m1d2, HIGH);

  analogWrite(pwm2, -1);
  digitalWrite(m2d1, HIGH);
  digitalWrite(m2d2, HIGH);
}
}

/*double getting_rpm_motor_left(){
 double xlnew;
 xlnew = xleftmotor;
 if (currentMillis - previousMillisl >= 15) {
   double s1l = xlnew;
   xleft = letyre.read();
   double s2l = xleftmotor;
   double rpml;
   rpml = 300 / 18 * ( s2l - s1l ) / ( 15 );
   rpmleftmotglo = rpml;
   previousMillisl = currentMillis;
   return rpml;
 }
 else return rpmleftmotglo;
}

double getting_rpm_motor_right(){
 double xrnew = xrightmotor;
 if (currentMillis - previousMillisr >= 15) {
 double s1r = xrnew;
 xright = riityre.read();
 double s2r = xrightmotor;
 double rpmr; 
 rpmr = 300/18*(s2r-s1r)/(15);
 rpmrightmotglo = rpmr;
 previousMillisr = currentMillis;
 return rpmr;
 }
 else return rpmrightmotglo;
}

void rpmpilotright(double r){
  double rpmright = getting_rpm_motor_right();
  double pr = 1.6125;
  double ir = 0.01625;
  digitalWrite(m2d1, LOW);
  digitalWrite(m2d2, HIGH);
  error_right=r-rpmright+error_right;
  double rirpm = pr*r + ir*(error_right);
  rirpm = map(rirpm,0,255,255,0);
  analogWrite(pwm2,rirpm);
}
void rpmpilotleft(double r){
  double rpmleft = getting_rpm_motor_left();
  double pl = 1.35;
  double il = 0.0135;
  digitalWrite(m1d1, LOW);
  digitalWrite(m1d2, HIGH);
  error_left=r-rpmleft+error_left;
  double lerpm = pl*r + il*(error_left);
  lerpm = map(lerpm,0,255,255,0);
  analogWrite(pwm1,lerpm);
}
*/



