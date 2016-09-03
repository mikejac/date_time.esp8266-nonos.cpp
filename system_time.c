/* 
 * The MIT License (MIT)
 * 
 * ESP8266 Non-OS Firmware
 * Copyright (c) 2015 Michael Jacobsen (github.com/mikejac)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "system_time.h"
#include <github.com/mikejac/misc.esp8266-nonos.cpp/espmissingincludes.h>
#include "os_type.h"
#include "osapi.h"

static volatile esp_time_t _system_time;
static volatile esp_time_t _uptime;

#if defined(USE_TIMER_INTERRUPTS)
    #define US_TO_RTC_TIMER_TICKS(t)          \
        ((t) ?                                   \
         (((t) > 0x35A) ?                   \
          (((t)>>2) * ((APB_CLK_FREQ>>4)/250000) + ((t)&0x3) * ((APB_CLK_FREQ>>4)/1000000))  :    \
          (((t) *(APB_CLK_FREQ>>4)) / 1000000)) :    \
         0)

    //FRC1
    #define FRC1_ENABLE_TIMER  BIT7

    typedef enum {
        DIVDED_BY_1   = 0,
        DIVDED_BY_16  = 4,
        DIVDED_BY_256 = 8,
    } TIMER_PREDIVED_MODE;

    typedef enum {
        TM_LEVEL_INT = 1,
        TM_EDGE_INT  = 0,
    } TIMER_INT_MODE;
#else
    static ETSTimer esp_timer;
#endif

#if defined(USE_TIMER_INTERRUPTS)
/**
 * 
 */
static void frc1_interrupt_handler(void)
{
    _system_time++;
    _uptime++;
}
#else
void ICACHE_FLASH_ATTR frc1_handler(void *arg)
{
    _system_time++;
    _uptime++;
}
#endif
/**
 * 
 * @return 
 */
int esp_start_system_time()
{
    _system_time  = 0;
    _uptime       = 0;
    
#if defined(USE_TIMER_INTERRUPTS)
    ETS_FRC_TIMER1_INTR_ATTACH(frc1_interrupt_handler, NULL);
    TM1_EDGE_INT_ENABLE();
    ETS_FRC1_INTR_ENABLE();
    
    RTC_CLR_REG_MASK(FRC1_INT_ADDRESS, FRC1_INT_CLR_MASK);
    
    RTC_REG_WRITE(FRC1_CTRL_ADDRESS,
                  DIVDED_BY_16
                  | FRC1_ENABLE_TIMER
                  | TM_EDGE_INT);
    
    RTC_REG_WRITE(FRC1_LOAD_ADDRESS, US_TO_RTC_TIMER_TICKS(1000000));
#else
    os_timer_disarm(&esp_timer);
    os_timer_setfn(&esp_timer, (os_timer_func_t *)frc1_handler, NULL);
    os_timer_arm(&esp_timer, 1000, 1);
#endif
        
    return 0;
}
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_time(esp_time_t* timer)
{
    if(timer != 0) {
        *timer = _system_time;
    }
    
    return _system_time;
}
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_uptime(esp_time_t* timer)
{
    if(timer != 0) {
        *timer = _uptime;
    }
    
    return _uptime;
}
/**
 * 
 * @param t
 * @return 
 */
int esp_stime(esp_time_t *t)
{
    if(t != 0) {
        _system_time = *t;
        return 0;
    }
    
    return -1;
}

