#include "MCDriver.h"

MCDriver::MCDriver(const gpio_num_t pin, uint32_t time_res, uint32_t time_per){
    _pin = pin;
    
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = time_res,
        .period_ticks = time_per,
    };
    timer_config.count_mode = MCPWM_TIMER_COUNT_MODE_UP;
    mcpwm_new_timer(&timer_config, &timer);

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    
    mcpwm_new_operator(&operator_config, &oper);
    mcpwm_operator_connect_timer(oper, timer);

    mcpwm_cmpr_handle_t _comparator = NULL;
    mcpwm_comparator_config_t comparator_config;
    comparator_config.flags.update_cmp_on_tez = true;
    mcpwm_new_comparator(oper, &comparator_config, &_comparator);

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = _pin,
    };

    mcpwm_new_generator(oper, &generator_config, &generator);


    // go high on counter empty
    mcpwm_generator_set_action_on_timer_event(generator,MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    // go low on compare threshold
    mcpwm_generator_set_action_on_compare_event(generator,MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, _comparator, MCPWM_GEN_ACTION_LOW));

    mcpwm_timer_enable(timer);
    mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);
}
void MCDriver::send(uint32_t pulse){
    mcpwm_comparator_set_compare_value(_comparator,pulse);
}