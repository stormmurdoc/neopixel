// LED Effects
void Schalke(int sleep){
    for(int x=0;x<NUM_LEDS;x++){
        if ( (x % 2) == 0) {
            strip.setPixelColor(x,0,0,255);
            strip.show();
            delay(sleep);
        }
        else {
            strip.setPixelColor(x,255,255,255);
            strip.show();
            delay(sleep);
        }
    }
}
void Schalke2(int sleep){
    for(int x=NUM_LEDS;x>0;x--){
        if ( (x % 2) == 0) {
            strip.setPixelColor(x,255,255,255);
            strip.show();
            delay(sleep);
        }
        else {
            strip.setPixelColor(x,0,0,255);
            strip.show();
            delay(sleep);
        }
    }
}
// set all LEDs
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}


void xmas() {
    int j=0;
    int run=0;
    int remaining=0;
    int helper=0;

        for (i=0;i<NUM_LEDS;i++){
            if (j < 8) {
            strip.setPixelColor(run,125, 0, 0);
            strip.setPixelColor(run+1, 80, 45, 0);
            strip.setPixelColor(run+2, 64, 64, 0);
            strip.setPixelColor(run+3, 0, 125, 0);
            strip.setPixelColor(run+4, 0, 64, 64);
            strip.setPixelColor(run+5, 0, 0, 125);
            strip.setPixelColor(run+6, 50, 0, 75);
            strip.setPixelColor(run+7, 40, 40, 40);
            j++;
        }
        else {
            j=0;
            run=run + 8;
        }
    }
    remaining=round(NUM_LEDS / 8);
    helper=remaining*8;
    remaining=NUM_LEDS - helper;
    for (i=0;i<=remaining;i++){
            j=helper + i;
            strip.setPixelColor(j,0, 255, 0);
    }
    strip.show();
}
