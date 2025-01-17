set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

set(CMAKE_CXX_FLAGS "-mcmse -mcpu=cortex-m55 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS   "-mcmse -mcpu=cortex-m55 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,--gc-sections")

set(CMAKE_SIZEOF_VOID_P 4)