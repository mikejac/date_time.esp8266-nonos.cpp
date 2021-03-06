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

#ifndef DATE_TIME_H
#define	DATE_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <c_types.h>

// 'time_t' is already typedef'ed in sys/types.h
typedef uint64_t esp_time_t;
/**
 * 
 * @return 
 */
int esp_start_system_time();
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_time(esp_time_t* timer);
/**
 * 
 * @param timer
 * @return 
 */
esp_time_t esp_uptime(esp_time_t* timer);
/**
 * 
 * @param t
 * @return 
 */
int esp_stime(esp_time_t *t);

#ifdef __cplusplus
}
#endif

#endif	/* DATE_TIME_H */

