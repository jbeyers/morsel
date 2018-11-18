// Basic Morsel code
float ll;
int rr;
float avg;
unsigned long prev;
unsigned long current;
float alpha = 0.01;
float alpha2 = 0.0001;
float envtop = 0.0;
float envbot = 0.0;
float mid = 0.0;
int pulse = 200;

void setup() { 
  Serial.begin(9600);
  pinMode(A0, INPUT);
  rr = 0;
  avg = 0;
  prev = millis();
} 

void loop() { 
//  digitalWrite(A0, 1);
  current = millis();
  if (prev < current) {
    ll = float(analogRead(A0));
    rr += 1;
    if (rr > 99) {
      rr = 0;
      Serial.print(int(avg));
      Serial.print(" ");
      Serial.print(int(envtop));
      Serial.print(" ");
      Serial.print(pulse);
      Serial.print(" ");
      Serial.println(int(envbot));
    }
    avg += alpha * (ll - avg);
    envtop += alpha2 * (ll - envtop);
    envbot += alpha2 * (ll - envbot);
    envtop = max(envtop, avg);
    envbot = min(envbot, avg);
    mid = (envtop+envbot)/2;
    if (avg > mid) {
      pulse = 200;
    } else {
      pulse = 160;
    }
    if (envtop < envbot + 20.0) {
      pulse = 180;
    }
  }
  prev = current;
}
