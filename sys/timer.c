#include <io.h>
#include <stdio.h>
#include <sys/timer.h>
#include <sys/idt.h>

uint32_t tick = 0;

void timer_callback()
{
    tick++;
    if((tick % 18) == 0)
    {
        uint32_t secs, mins, hours;
        secs = tick/100;
        if (secs >= 3600)		//hour section
        {
            hours = secs / 3600;
            mins =  secs % 3600;
            if(mins >= 60)
            {
                mins = mins/60;
            }
            secs = mins%60;

            if(hours < 9)
            {
		  if(mins < 10)
		  {
		      if(secs < 10)
                	   print_cx_cy(57, 24, "0%d:0%d:0%d system uptime", hours, mins, secs);
		      else
			   print_cx_cy(57, 24, "0%d:0%d:%d system uptime", hours, mins, secs);
		  }
		  else
                {
     		      if(secs < 10)
                        print_cx_cy(57, 24, "0%d:%d:0%d system uptime", hours, mins, secs);
		      else
			   print_cx_cy(57, 24, "0%d:%d:%d system uptime", hours, mins, secs);
		  }
            }
            else
            {
		  if(mins < 10)
                {
		      if(secs < 10)
               	   print_cx_cy(57, 24, "%d:0%d:0%d system uptime", hours, mins, secs);
		      else
               	   print_cx_cy(57, 24, "%d:0%d:%d system uptime", hours, mins, secs);
        	  }
                else
                {
		      if(secs < 10)
	                 print_cx_cy(57, 24, "%d:%d:0%d system uptime", hours, mins, secs);
		      else
		          print_cx_cy(57, 24, "%d:%d:%d system uptime", hours, mins, secs);
		  }
            }
        }
        else if (secs <3600  && secs >= 60)	//min section
        {
            mins = secs / 60;
            secs = secs % 60;
            if (mins < 10)
	     {
		  if(secs < 10)
                    print_cx_cy(57, 24, "00:0%d:0%d system uptime", mins, secs);
		  else
                    print_cx_cy(57, 24, "00:0%d:%d system uptime", mins, secs);
		  }
            else
            {
	         if(secs < 10)
                    print_cx_cy(57, 24, "00:%d:0%d system uptime", mins, secs);
		  else
                    print_cx_cy(57, 24, "00:%d:%d system uptime", mins, secs);
	     }
        }
        else if (secs < 60)		//second section
        {
            if(secs < 10)
	         print_cx_cy(57, 24, "00:00:0%d system uptime", secs);
	     else
		  print_cx_cy(57, 24, "00:00:%d system uptime", secs);
        }
    }
}

void set_timer_phase()
{
   int frequency = 100;
   uint16_t divisor = 1193180/frequency;

    // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)(divisor >> 8);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}
