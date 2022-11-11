
int ppl = 0;

bool pi = 0;
bool po = 0;

long int pi_read_at;
long int po_read_at;

void setupCounter(int in, int out) {
  pinMode(in, INPUT);
  pinMode(out, INPUT);
}


int loopCounter(int in, int out) {    
  bool pi = digitalRead(in);
  bool po = digitalRead(out);
    if (pi == 0 && po == 1){
      Serial.println("Reading people in");
      while (true) {
          po = digitalRead(out);
          if (po == 0){
            ppl++;
            Serial.print("one people in. remaining ppl: ");
            Serial.println(ppl);
            break;
          }
      }
    }
    else if (pi == 1 && po == 0){
      Serial.println("Reading people out");
      while (true) {
        pi = digitalRead(in);
        if (pi == 0){
          ppl--;
          Serial.print("one people out. remaining ppl: ");
          Serial.println(ppl);
          break;
        }
      }
    }

    return ppl;
  
}