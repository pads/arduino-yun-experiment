# Arduino Yun Experiment

This project allows temperature and light sensor readings to be output as a web app.

## Board Setup

![Breadboard](https://raw.githubusercontent.com/pads/arduino-yun-experiment/master/breadboard.jpg)

## Sketch Setup

Upload `api/api.ino` to the Yun.  You should see `Waiting...` on the first row of the LCD display.  Test the API in a browser by going to:

    http://arduino.local/arduino/temperature
    http://arduino.local/arduino/light
  
You should see output such as the examples below:

```javascript
{
  "temperature": 20.58,
  "measure": "C"
}
```

```javascript
{
  "light": 10,
  "measure": "lx"
}
```
  
The LCD display will also output the reading requsted as well as the current value.

## Web App Setup

Run the deploy script as follows to copy the files to the public `www` folder on the Yun web server:

    ./deploy
    
This script will prompt for a password unless you have setup SSH public key access.

Navigate to `http://arduino.local/yun.html` and you should see the web app running.
