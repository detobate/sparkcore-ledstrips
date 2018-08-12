/*
    DEVICEID: [YOUR DEVICE ID]
    ACCESS TOKEN: [YOUR ACCESS TOKEN]

    //turnLedOn
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/On -d access_token=[YOUR ACCESS TOKEN]

    //turnLedOff
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/Off -d access_token=[YOUR ACCESS TOKEN]

    //pass the values for the LED strip as the arg value at the end
    curl https://api.spark.io/v1/devices/[YOUR DEVICE ID]/setColour -d access_token=[YOUR ACCESS TOKEN] -d "args=[WEBCOLOR]"

*/


int ledStripRedPin = D1;
int ledStripGreenPin = D0;
int ledStripBluePin = D2;

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
    Particle.function("control", control);
    Particle.function("toggle", toggle);

    Particle.variable("status", "off");


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

    int idx = 0;
    if (webcolor.startsWith("#")){
        idx = 1;
    }
    String R = webcolor.substring(0+idx, 2+idx);
    String G = webcolor.substring(2+idx, 4+idx);
    String B = webcolor.substring(4+idx, 6+idx);

    analogWrite(ledStripRedPin, strtoul(R, 0, 16));
    analogWrite(ledStripGreenPin, strtoul(G, 0, 16));
    analogWrite(ledStripBluePin, strtoul(B, 0, 16));
    Particle.publish("status", webcolor);
    Particle.publish("R", R);
    Particle.publish("G", G);
    Particle.publish("B", B);
    char foo[50];
    webcolor.toCharArray(foo,50);
    Particle.variable("status", foo);
    return(1);
}


int turnOn(String foo){
    analogWrite(ledStripRedPin, 200);
    analogWrite(ledStripGreenPin, 200);
    analogWrite(ledStripBluePin, 200);
    Particle.publish("status", "on");
    Particle.publish("R", "200");
    Particle.publish("G", "200");
    Particle.publish("B", "200");
    Particle.variable("status", "on");
    return(1);
}

int turnOnDim(String foo){
    analogWrite(ledStripRedPin, 50);
    analogWrite(ledStripGreenPin, 50);
    analogWrite(ledStripBluePin, 50);
    Particle.publish("status", "dim");
    Particle.publish("R", "50");
    Particle.publish("G", "50");
    Particle.publish("B", "50");
    Particle.variable("status", "dim");
    return(1);
}

int turnOnFull(String foo){
    analogWrite(ledStripRedPin, 255);
    analogWrite(ledStripGreenPin, 255);
    analogWrite(ledStripBluePin, 255);
    Particle.publish("status", "full");
    Particle.publish("R", "255");
    Particle.publish("G", "255");
    Particle.publish("B", "255");
    Particle.variable("status", "full");
    return(1);
}

int turnOff(String foo){
    resetAll();
    Particle.publish("status", "off");
    Particle.variable("status", "off");
    return(0);
}

int control(String command){
    if (command == "on") {
        return(turnOn("foo"));
    }
    else if (command == "off") {
        return(turnOff("foo"));
    }
    else if (command == "toggle") {
        return(toggle("foo"));
    }
    else {
        return(setColour(command));
    }
}

int toggle(String foo){
}

//private
void resetAll(){
    analogWrite(ledStripRedPin, 0);
    analogWrite(ledStripGreenPin, 0);
    analogWrite(ledStripBluePin, 0);
}
