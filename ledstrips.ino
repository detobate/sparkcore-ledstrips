/*
    DEVICEID: [YOUR DEVICE ID]
    ACCESS TOKEN: [YOUR ACCESS TOKEN]

    //Functions:
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/On -d access_token=[YOUR ACCESS TOKEN]
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/Off -d access_token=[YOUR ACCESS TOKEN]
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/dim -d access_token=[YOUR ACCESS TOKEN]
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/full -d access_token=[YOUR ACCESS TOKEN]

    // or pass the values for the LED strip as a webcolor in an arg value at the end
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/setColour -d access_token=[YOUR ACCESS TOKEN] -d "args=[WEBCOLOR]"

*/


int ledStripBluePin = A4;
int ledStripRedPin = A1;
int ledStripGreenPin = A0;

void setup() {

    //set the PWM analog pins as outputs
    pinMode(ledStripRedPin, OUTPUT);  // sets the Red LED strip pin as output
    pinMode(ledStripGreenPin, OUTPUT);  // sets the Green LED strip pin as output
    pinMode(ledStripBluePin, OUTPUT);  // sets the Blue LED strip pin as output

    //set the default output to 0 (off)
    analogWrite(ledStripRedPin, 0);
    analogWrite(ledStripGreenPin, 0);
    analogWrite(ledStripBluePin, 0);

    // register the cloud function calls
    Particle.function("setColour", setColour);
    Particle.function("setColor", setColour);
    Particle.function("on", turnOn);
    Particle.function("full", turnOnFull);
    Particle.function("dim", turnOnDim);
    Particle.function("off", turnOff);

    //wait to show the cyan indication of web access
    delay(5000);

    //take control of the RGB LED
    RGB.control(true);
}


void loop() {
    //set the onboard rgb LED to indicated cloud connection status
    if(Particle.connected()){
        // Sets the LED to blue
        RGB.color(0, 0, 255);
    }else{
        // Set the LED to red
        RGB.color(255, 0, 0);
    }

    delay(200);
}


int setColour(String webcolor)
{
    unsigned long RGB[4];
    parseHex(webcolor, RGB);
    analogWrite(ledStripRedPin, RGB[0]);
    analogWrite(ledStripGreenPin, RGB[1]);
    analogWrite(ledStripBluePin, RGB[2]);
    Particle.publish("status", webcolor);
    Particle.publish("R", RGB[0]);
    Particle.publish("G", RGB[1]);
    Particle.publish("B", RGB[2]);
  return(1);
}


int turnOn(String foo){
    analogWrite(ledStripRedPin, 200);
    analogWrite(ledStripGreenPin, 200);
    analogWrite(ledStripBluePin, 200);
    Particle.publish("status", "on");
    Particle.publish("R", 200);
    Particle.publish("G", 200);
    Particle.publish("B", 200);
    return(1);
}

int turnOnDim(String foo){
    analogWrite(ledStripRedPin, 50);
    analogWrite(ledStripGreenPin, 50);
    analogWrite(ledStripBluePin, 50);
    Particle.publish("status", "dim");
    Particle.publish("R", 50);
    Particle.publish("G", 50);
    Particle.publish("B", 50);

    return(1);
}

int turnOnFull(String foo){
    analogWrite(ledStripRedPin, 255);
    analogWrite(ledStripGreenPin, 255);
    analogWrite(ledStripBluePin, 255);
    Particle.publish("status", "full");
    Particle.publish("R", 255);
    Particle.publish("G", 255);
    Particle.publish("B", 255);
    return(1);
}

int turnOff(String foo){
    resetAll();
    Particle.publish("status", "off");
    return(0);
}

void parseHex(String webcolor, unsigned long RGB[]){
    int idx = 0;
    if (webcolor.startsWith("#")){
        idx = 1;
    }
    String R = webcolor.substring(0+idx, 1+idx);
    String G = webcolor.substring(2+idx, 3+idx);
    String B = webcolor.substring(4+idx, 5+idx);

    RGB[0] = strtoul(R, 0, 16);
    RGB[1] = strtoul(G, 0, 16);
    RGB[2] = strtoul(B, 0, 16);
}

//private
void resetAll(){
    analogWrite(ledStripRedPin, 0);
    analogWrite(ledStripGreenPin, 0);
    analogWrite(ledStripBluePin, 0);
}
