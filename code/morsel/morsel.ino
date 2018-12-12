// Basic Morsel code
const int env_length = 80;
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
int envelope[env_length] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int env_index = 0;
int avg_int;
int env_max;
int env_min;
int i;
int updown;
int mode;
int a;
int agg; // how many samples get aggregated
int acc; // Accumulator of samples
int tot; // How many samples we have
int avg_acc; // Averaged accumulator value

void setup() { 
    Serial.begin(9600);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(A0, INPUT);
    rr = 0;
    avg = 0;
    prev = millis();
    env_max = 0;
    env_min = 0;
    updown = 0;
    agg = 2;
    acc = 0;
    tot = 0;
    avg_acc = 0;
    mode = 0;
} 

void loop() { 
    //  digitalWrite(A0, 1);
    current = millis();
    if (prev < current) {
        pinMode(A0, INPUT);
        delayMicroseconds(200);
        if (digitalRead(4) == LOW) {
            updown += 1;
        }
        if (digitalRead(5) == LOW) {
            updown -= 1;
        }
        if (digitalRead(4) == HIGH && digitalRead(5) == HIGH) {
            updown = 0;
        }
        if (updown == 400 && mode < 6) {
            mode += 1;
        }
        if (updown == -400 && mode > 0) {
            mode -= 1;
        }
        a = analogRead(A0);
        ll = float(a);
        avg += alpha * (ll - avg);
        avg_int = int(avg);
        acc += a;
        tot += 1;
        if (tot >= agg) {
            avg_acc = acc / tot;
            acc = 0;
            tot = 0;
        }
        
        // Showing just the raw value
        if (mode == 0) {
            agg = 1;
        }
        if (mode == 1) {
            agg = 3;
        }
        if (mode == 2) {
            agg = 10;
        }
        // Average over 10 samples
        if (mode == 3) {
            agg = 19;
        }
        // Average over 20 samples. Filters out 50Hz.
        if (mode == 4) {
            agg = 20;
        }
        // Average over 40 samples
        if (mode == 5) {
            agg = 50;
        }
        if (mode == 6) {
            agg = 20;
        }
            

        // Write to analog about 40 times a second
        rr += 1;
        if (rr > 25) {
            rr = 0;
            envelope[env_index] = avg_acc;
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
            //Serial.print(a);
            //Serial.print(" ");
            Serial.print(avg_int);
            Serial.print(" ");
            Serial.print(avg_acc);
            Serial.print(" ");
            Serial.print(env_min);
            Serial.print(" ");
            if (mode == 6) {
                Serial.print(env_max);
                Serial.print(" ");
                Serial.println(pulse);
            } else{
                Serial.println(env_max);
            }
        }
        mid =(env_max + env_min)/2;
        if (avg_acc > mid) {
            pulse = 140;
        } else {
            pulse = 100;
        }
        if (env_max - env_min < 20.0) {
            pulse = 120;
        }
        //if (mode == 1||mode == 3) {
        //    pinMode(A0, OUTPUT);
        //    digitalWrite(A0, HIGH);
        //    delayMicroseconds(200);
        //    digitalWrite(A0, LOW);
        //}
        prev += 1;
    }
}
