#pragma once

enum class LTC_REGISTER {
    ERROR_INDEX = 1000,
    ERROR_NOTEBOOK_QUEUE_0 = 1001,
    ERROR_NOTEBOOK_QUEUE_1 = 1002,
    ERROR_NOTEBOOK_QUEUE_15 = 1016,
    TABLE_HEIGHT = 2000,
    LAST_KEYPRESS_DATA = 2001,
    KEYPRESS_CONTROL = 2002,
    UPTIME_AND_RESET_CONTROLLER = 3000,
    TEST = 3001,
    SAVE_CONFIGURATION_TO_FLASH = 3002,
    LOAD_CONFIGURATION_FROM_FLASH = 3003,
    SYSTEM_MODE = 3004,
    RESET_POSITION = 3005,
    CLEAR_ERRORS_FROM_LOG = 3011,
    CONVERT_REAL_UNITS_TO_RAW_DATA = 3012,
    MODIFY_POSITION_TARGET_WHEN_MOVING = 3021,
    MOTOR1_STOP_CURRENT_THRESHOLD = 11000,
    MOTOR1_ACTUAL_DIRECTION = 11003,
    MOTOR1_ACTUAL_POSITION = 11004,
    MOTOR1_ACTUAL_SPEED = 11005,
    MOTOR1_ACTUAL_PWM = 11006,
    MOTOR1_ACTUAL_MOTOR_POWER = 11007,
    MOTOR1_ACTUAL_MOTOR_SIGNAL_STATE = 11008,
    MOTOR1_CURRENT_REFERENCE = 11009,
    MOTOR1_CURRENT_FEEDBACK = 11010,
    MOTOR1_MAXIMUM_CURRENT = 11011,
    MOTOR1_I2T = 11015,
    MOTOR2_STOP_CURRENT_THRESHOLD = 11022,
    MOTOR2_ACTUAL_DIRECTION = 11025,
    MOTOR2_ACTUAL_POSITION = 11026,
    MOTOR2_ACTUAL_SPEED = 11027,
    MOTOR2_ACTUAL_PWM = 11028,
    MOTOR2_ACTUAL_MOTOR_POWER = 11029,
    MOTOR2_ACTUAL_MOTOR_SIGNAL_STATE = 11030,
    MOTOR2_CURRENT_REFERENCE = 11031,
    MOTOR2_CURRENT_FEEDBACK = 11032,
    MOTOR2_MAXIMUM_CURRENT = 11033,
    MOTOR2_I2T = 11037,
    MOTOR3_STOP_CURRENT_THRESHOLD = 11044,
    MOTOR3_ACTUAL_DIRECTION = 11047,
    MOTOR3_ACTUAL_POSITION = 11048,
    MOTOR3_ACTUAL_SPEED = 11049,
    MOTOR3_ACTUAL_PWM = 11050,
    MOTOR3_ACTUAL_MOTOR_POWER = 11051,
    MOTOR3_ACTUAL_MOTOR_SIGNAL_STATE = 11052,
    MOTOR3_CURRENT_REFERENCE = 11053,
    MOTOR3_CURRENT_FEEDBACK = 11054,
    MOTOR3_MAXIMUM_CURRENT = 11055,
    MOTOR3_I2T = 11059,
    MOTOR4_STOP_CURRENT_THRESHOLD = 11066,
    MOTOR4_ACTUAL_DIRECTION = 11069,
    MOTOR4_ACTUAL_POSITION = 11070,
    MOTOR4_ACTUAL_SPEED = 11071,
    MOTOR4_ACTUAL_PWM = 11072,
    MOTOR4_ACTUAL_MOTOR_POWER = 11073,
    MOTOR4_ACTUAL_MOTOR_SIGNAL_STATE = 11074,
    MOTOR4_CURRENT_REFERENCE = 11075,
    MOTOR4_CURRENT_FEEDBACK = 11076,
    MOTOR4_MAXIMUM_CURRENT = 11077,
    MOTOR4_I2T = 11078,
    MOVEMENT_IN_PROGRESS = 12000,
    ACTUAL_DIRECTION = 12001,
    ACTUAL_POSITION = 12002,
    POSITION_TARGET = 12003,
    DISTANCE_FROM_TARGET = 12004,
    POSITION_ERROR = 12005,
    IN_POSITION = 12006,
    MOTOR1_SPEED_FEEDBACK = 12015,
    MOTOR2_SPEED_FEEDBACK = 12021,
    MOTOR3_SPEED_FEEDBACK = 12027,
    MOTOR4_SPEED_FEEDBACK = 12033,
    RESET_COUNT = 14000,
    CYCLE_COUNT = 14002,
    LOGIC_STATE = 14005,
    ERROR_STATE = 14006,
    ERROR_STATE_TIMER = 14007,
    HARDWARE_OVERCURRENT_ERROR = 14008,
    VELOCITY_LP_X = 15006,
    VELOCITY_LP_Y = 15007,
    VELOCITY_LP_Z = 15008,
    VELOCITY_HP_X = 15009,
    VELOCITY_HP_Y = 15010,
    VELOCITY_HP_Z = 15011,
    TOP_MECHANICAL_LIMIT = 20003,
    HIGH_POSITION_MARGIN = 20004,
    LOW_POSITION_MARGIN = 20005,
    SLOWDOWN_DISTANCE = 20006,
    COLLISION_MOVE_BACK_DISTANCE = 20007,
    TARGET_REACHED_DISTANCE = 20008,
    HEIGHT_CORRECTION = 20011,
    DESIRED_SPEED = 20012,
    ACCELERATION_TIME = 20014,
    SPEED_RAMP_DOWN_TIME = 20015,
    REFERENCE_RUN_TARGET_SPEED = 20017,
    REFERENCE_RUN_ACCELERATION_TIME = 20020,
    TOP_LIMIT_SWITCH_OVERRUN = 20022,
    BOTTOM_LIMIT_SWITCH_OVERRUN = 20023,
    SET_REFERENCE_MODE_AT_POWER_UP = 21013,
    ENABLE_BRAKE = 21015,
    COLLISION_DETECTION_HIGH_PASS_FILTER_MOTOR1 = 25000,
    COLLISION_DETECTION_LOW_PASS_FILTER_MOTOR1 = 25001,
    COLLISION_DETECTION_LIMIT_MOTOR1 = 25002,
    COLLISION_DETECTION_HIGH_PASS_FILTER_MOTOR2 = 25005,
    COLLISION_DETECTION_LOW_PASS_FILTER_MOTOR2 = 25006,
    COLLISION_DETECTION_LIMIT_MOTOR2 = 25007,
    COLLISION_DETECTION_HIGH_PASS_FILTER_MOTOR3 = 25010,
    COLLISION_DETECTION_LOW_PASS_FILTER_MOTOR3 = 25011,
    COLLISION_DETECTION_LIMIT_MOTOR3 = 25012,
    COLLISION_DETECTION_HIGH_PASS_FILTER_MOTOR4 = 25015,
    COLLISION_DETECTION_LOW_PASS_FILTER_MOTOR4 = 25016,
    COLLISION_DETECTION_LIMIT_MOTOR4 = 25017,
    VELOCITY_LP_FILTER = 25024,
    VELOCITY_HP_FILTER = 25025,
    VELOCITY_LIMIT_X = 25026,
    VELOCITY_LIMIT_Y = 25027,
    VELOCITY_LIMIT_Z = 25028,
    USER_LOW_LIMIT_POSITION = 26000,
    USER_HIGH_LIMIT_POSITION = 26002,
    USER_POSITION_1 = 26004,
    USER_POSITION_2 = 26006,
    USER_POSITION_3 = 26008,
    USER_POSITION_4 = 26010,
    COLLISION_LEVEL = 26012,
    COLLISION_LEVEL_STEP = 26013,
    METRIC_IMPERIAL = 26014,
    DISPLAY_INDICATION = 26015,
    BUTTON_MODE = 26017,
    STOP_CURRENT_THRESHOLD_AT_BOTTOM_MOTOR1 = 29000,
    STOP_CURRENT_THRESHOLD_AT_BOTTOM_MOTOR2 = 29001,
    STOP_CURRENT_THRESHOLD_AT_BOTTOM_MOTOR3 = 29002,
    STOP_CURRENT_THRESHOLD_AT_BOTTOM_MOTOR4 = 29003,
    USE_LIMIT_SWITCH = 29012,
    CHECK_MOTOR_PRESENCE = 29015,
    TWO_WAY_REFERENCE = 29016,
    STOP_CURRENT_THRESHOLD_AT_TOP_MOTOR1 = 29017,
    STOP_CURRENT_THRESHOLD_AT_TOP_MOTOR2 = 29018,
    STOP_CURRENT_THRESHOLD_AT_TOP_MOTOR3 = 29019,
    STOP_CURRENT_THRESHOLD_AT_TOP_MOTOR4 = 29020,
    PATH_LENGTH = 30000,
    PATH_RAW = 30001,
    HEIGHT_OFFSET = 30003,
    SPEED_CONTROL_PROPORTIONAL_GAIN = 30018,
    SPEED_CONTROL_INTEGRAL_GAIN = 30019,
    SPEED_CONTROL_DIFFERENTIAL_GAIN = 30020,
    SPEED_CONTROL_TIME_BASE_PRESCALER = 30021,
    SPEED_CONTROL_OUTPUT_SATURATION_RANGE = 30022,
    STOP_CURRENT_THRESHOLD_M1 = 30028,
    STOP_CURRENT_THRESHOLD_M2 = 30029,
    STOP_CURRENT_THRESHOLD_M3 = 30030,
    STOP_CURRENT_THRESHOLD_M4 = 30031,
};
