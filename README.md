# stm32f4-timer-interrupt-with-register
In this project, the number that is maximum value will be declared to be counted upper. The maximum value is attanded in TIMx_ARR register (auto reloaded register). So, the counter cant count from 0 to max value (TIMx_ARR stores). TIMx_PSC scales clock frequncy for adjusting  proper frequency in each counter reaching max value. Briefly, when the PA0 bit is pressed, external interrupt is trigerred. The led is lighted up. After that, the timer 2 starts to count up to 2000 in each one second. When counter reachs to 2000, the timer 2 interrupt generates a false statement to light off the led.

The formula of the calculation is **(PSC+1)*(ARR+1) = TIMclk/Updatefrequency.** PSC and ARR can be declared by using proper registers. Also the the the timer clock frequency is known. It is 84000000 Hz. The delay is not needed for this project so, **Updatefrequency** is one. The formula becomes **(PSC+1)*(ARR+1) = 84000000/1. (PSC+1)*(ARR+1)** is equal to 84000000. Generally, **PSC+1 is 42000.** So, **ARR+1 must be 2000.** Due to update frequency is equal to 1, the timer counts 0 from to 2000 each one second.

The counter timing diagram shows how timer work with registers  The timer clock starts and counter counts untill TIMx_ARR is matched maximum value which is 0x36. In every rising Timerclock edge, counter register increase count value (it is valid for internal clock divided by 1). When reached maximum value, update event (UEV) rises. If the update generation (UG) is set, the counter repeated own duty according to indicated frequency. In this project, the counter count 0 from 2000 in one second.

![111](https://github.com/yasinsulhan/stm32f4-timer-interrupt-with-register/assets/109728194/44286d37-285f-4aa0-ba21-706cd782fba1)
![222](https://github.com/yasinsulhan/stm32f4-timer-interrupt-with-register/assets/109728194/80a522ad-4dfa-4e2f-8b06-a597667b738b)

As a summary, The counter is clocked by the prescaler output CK_CNT, which is enabled only when the counter enable bit (CEN) in TIMx_CR1 register. In upcounting mode, the counter counts from 0 to the auto-reload value (content of the TIMx_ARR register), then restarts from 0 and generates a counter overflow event. An Update event can be generated at each counter overflow or by setting the UG bit in the TIMx_EGR register. In addition, if the URS bit (update request selection) in TIMx_CR1 register is set, setting the UG bit generates an update event UEV.

When an update event occurs, all the registers are updated and the update flag (UIF bit in TIMx_SR register) is set (depending on the URS bit): 

• The buffer of the prescaler is reloaded with the preload value (content of the TIMx_PSC register)

• The auto-reload shadow register is updated with the preload value (TIMx_ARR)


