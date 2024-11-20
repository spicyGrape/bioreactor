// Try for 4, got 3.65 to 3.80
// Try for 10, got 9.80 to 9.90
// Try for 7, got 6.85 to 6.89
// So added 0.15 to parsePH

// Declarations for pH subsystem
int phAnalogPin = A1;
int phRawRead = 0;
double phRead = 7.0;
int pH7Read = 240;
int pH4Read = 160;

double parsePh(int);
double pHPid(double, double);

// Declarations for pid for pH
#define PH_INT_ERR_MAX 0
#define PH_INT_ERR_MIN 0
double pHPrevVal;
double pHPrevErr;
double pHPrevTime;
double pHPrev;
double pHDt;
double pHIntErr;
int pHT;
double pHCalcP(double, double);
double pHCalcI(double, double, int);
double phCalcD(double, double);

void setup()
{
  Serial.begin(9600);
  pHIntErr = 0;
  pHPrevErr = 0;
  pHDt = 0;
}

double parsePh(int phRawRead)
{
  return -0.04 * phRawRead + 15.763;
}

void loop()
{
  phRawRead = analogRead(phAnalogPin);
  // phRawRead = 280;
  // Serial.print("pH Value: ");
  //Serial.println(phRawRead);
  phRead = parsePh(phRawRead);
  Serial.println(phRead);
  delay(100); 
  if ((int))
}

double pHPid(double targetVal, double currentVal, double P, double I, double D)
{

  return P * pHCalcP(targetVal, currentVal) + I * pHCalcI(targetVal, currentVal, T) + D * pHCalcD(targetVal, currentVal);
}

double pHCalcP(double targetVal, double currentVal)
{
  return targetVal - currentVal;
}

double pHCalcI(double targetVal, double currentVal, int T)
{
  double currentTime = (double)(T) * (1e-6);
  pHDt = currentTime - pHPrevTime;
  pHPrevTime = currentTime;
  pHIntErr += (targetVal - currentVal) * pHDt;
  if (pHIntErr > PH_INT_ERR_MAX)
  {
    pHIntErr = PH_INT_ERR_MAX;
  }
  else if (pHIntErr < PH_INT_ERR_MIN)
  {
    pHIntErr = PH_INT_ERR_MIN;
  }
  return pHIntErr;
}

double pHCalcD(double targetVal, double currentVal)
{
  return 0;
}


