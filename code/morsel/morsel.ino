// Basic Morsel code
const int env_length = 20;
float ll;
int rr;
float avg;
unsigned long prev;
unsigned long current;
float alpha = 0.01;
float envtop = 0.0;
float envbot = 0.0;
float mid = 0.0;
int pulse = 200;
int envelope[env_length] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int env_index = 0;
int avg_int;
int env_max;
int env_min;
int i;

void setup() { 
  Serial.begin(9600);
  pinMode(A0, INPUT);
  rr = 0;
  avg = 0;
  prev = millis();
  env_max = 0;
  env_min = 0;
} 

void loop() { 
//  digitalWrite(A0, 1);
  current = millis();
  if (prev < current) {
    ll = float(analogRead(A0));
    rr += 1;
    if (rr > 99) {
      rr = 0;
      avg_int = int(avg);
      envelope[env_index] = avg_int;
      env_max = 0;
      env_min = 2000;
      for (i = 0; i < env_length; i++) {
        env_max = max(env_max, envelope[i]);
        env_min = min(env_min, envelope[i]);
      }
      env_index += 1;
      if (env_index == env_length) {
        env_index = 0;
      }
      Serial.print(int(ll));
      Serial.print(" ");
      Serial.print(avg_int);
      Serial.print(" ");
      Serial.print(pulse);
      Serial.print(" ");
      Serial.println(mid);
    }
    avg += alpha * (ll - avg);
    mid =(env_max + env_min)/2;
    if (avg > mid) {
      pulse = env_max;
    } else {
      pulse = env_min;
    }
    if (env_max - env_min < 20.0) {
      pulse = mid;
    }
  }
  prev = current;
}
