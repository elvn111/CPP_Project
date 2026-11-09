#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    while (1)
    {

        //system("echo 0 > /sys/class/leds/red/brightness");//      #关闭red灯
        //system("echo 0 > /sys/class/leds/blue/brightness");//     #关闭blue灯
        //system("echo 0 > /sys/class/leds/green/brightness");//    #关闭green灯
        //system("echo 255 > /sys/class/leds/red/brightness");//    #点亮red灯
        //system("echo 255 > /sys/class/leds/blue/brightness");//   #点亮blue灯
        system("sudo echo ELVN");//  #点亮green灯
    }
}