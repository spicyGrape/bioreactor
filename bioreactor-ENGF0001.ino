// Try for 4, got 3.65 to 3.80
// Try for 10, got 9.80 to 9.90
// Try for 7, got 6.85 to 6.89
// So added 0.15 to parsePH

// Declarations for pH subsystem
int ph_ana_in = A1;
int ph_ana_out_1 = 5;
int ph_ana_out_2 = 6;
double ph_reading = 7.0;
double ph_buffer[10];
unsigned long sys_time = 0;
int ph_pump_1_state;
int ph_pump_2_state;
int ph_next_time;
void ph_init();
int prev_sys_time = 0;

void ph_pump_start(int);
void ph_pump_stop(int);

double ph_measure();

// Declarations for pid for pH
//double pHPrevVal;
//double pHPrevErr;
//double pHPrevTime;
//double pHPrev;
//double pHDt;
//double pHIntErr;
//int pHT;
//double pHCalcP(double, double);
//double pHCalcI(double, double, int);
//double phCalcD(double, double);
void ph_init()
{
  ph_pump_1_state = 0;
  ph_pump_2_state = 0;
  ph_next_time = 0;
}

void ph_pump_start(int pump_number)
{
  if (pump_number == 1)
  {
    if (!(ph_pump_1_state))
    {
        ph_pump_stop(2);
        analogWrite(ph_ana_out_1, 255);
        ph_pump_1_state = 1;
    }
  }
  else
  {
    if (!(ph_pump_2_state))
    {
        ph_pump_stop(1);
        analogWrite(ph_ana_out_2, 255);
        ph_pump_2_state = 1;
    }
  }
}

void ph_pump_stop(int pump_number)
{
  if (pump_number == 1)
  {
    if (ph_pump_1_state)
    {
        analogWrite(ph_ana_out_1, 0);
        ph_pump_1_state = 0;
    }
  }
  else
  {
    if (ph_pump_2_state)
    {
        analogWrite(ph_ana_out_2, 0);
        ph_pump_2_state = 0;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  sys_time = micros();
  ph_init();
  //pHIntErr = 0;
  //pHPrevErr = 0;
  //pHDt = 0;
}

double ph_measure()
{
  int ph_raw = analogRead(ph_ana_in);
  int ph_parsed = -0.0395 * ph_raw + 15.5;
  for (int i = 0; i < 9; i++)
  {
    ph_buffer[i] = ph_buffer[i+1];
  }
  ph_buffer[9] = ph_parsed;
  double ph_average = 0.0;
  for (int i = 0; i < 10; i++)
  {
    ph_average += ph_buffer[i];
  }
  ph_average *= 0.1;
  return ph_average;
}

double ph_control(double ph_target, double ph_tolerance)
{
  //pump 1 raise ph
  //pump 2 reduce ph
  double ph_reading = ph_measure();
  if (ph_reading < ph_target - ph_tolerance)
  {
    ph_pump_start(1);
  }
  else if (ph_reading < ph_target)
  {
    ph_pump_stop(2);
  }
  else if (ph_reading < ph_target + ph_tolerance)
  {
    ph_pump_stop(1);
  }
  else if (ph_reading > ph_target + ph_tolerance)
  {
    ph_pump_start(2);
  }

}

void loop()
{
  // phRawRead = 280;
  // Serial.print("pH Value: ");
  // Serial.println(phRawRead);
  sys_time = millis();
  if (sys_time < prev_sys_time)
  {
    ph_next_time = 0;
  }
  if (sys_time > ph_next_time)
  {
    ph_next_time += 1e3;
    Serial.println(ph_measure());
    ph_control(1.0, 1.0);
  }
  prev_sys_time = sys_time;
}


