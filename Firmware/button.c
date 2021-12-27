#include <iostream> // Include all needed libraries here
#include <wiringPi.h>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std; // No need to keep using “std”

string exec(const char *cmd)
{
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
int g = 0;
void myInterrupt(void)
{
    // cout << "pressed" << endl;
    if (digitalRead(1) == 1)
    {
        cout << "pressed" << endl;
        if (g == 0)
        {
            g = 1;
            cout << exec("raspivid -t 5000");
            cout << "done";
            cout << exec("aplay bell.wav");
            g = 0;
        }
        delay(1000);
    }
}
int main()
{
    wiringPiSetup(); // Setup the library

    pinMode(1, INPUT); // Configure GPIO1 as an input
    if (wiringPiISR(1, INT_EDGE_FALLING, myInterrupt) < 0)
    {
        printf("unable to setup ISR\n");
    }
    cout << exec("ls -lah");
    // Main program loop
    while (1)
    {
        // Button is pressed if digitalRead returns 0
        // if (digitalRead(1) == 0)
        // {
        //     cout << "pressed" << endl;
        //     delay(1000);
        // }
        // else
        // {
        //     cout << "not pressed" << endl;
        //     delay(1000);
        // }
    }
    return 0;
}