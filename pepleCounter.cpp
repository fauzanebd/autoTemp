
int ppl = 0;

bool pi = 0;
bool po = 0;

void setupCounter(int in, int out) {
  pinMode(in, INPUT);
  pinMode(out, INPUT);
}


int loopCounter(int in, int out) {
  pi = digitalRead(in);
  po = digitalRead(out);
  if (pi == 0) {
    // Serial.println("Reading people in");
    while (true) {
      po = digitalRead(out);
      if (po == 0) {
        ppl++;
        // Serial.print("People in room: ");
        // Serial.println(ppl);
        delay(100);
        break;
      }
    }
  }
  else if (po == 0) {
    // Serial.println("Reading people out");
    while (true) {
      pi = digitalRead(in);
      if (pi == 0) {
        if (ppl > 0) {
           ppl--; 
        }
        // Serial.print("People in room: ");
        // Serial.println(ppl);
        delay(100);
        break;
      }
    }
  }
  return ppl;
  
}