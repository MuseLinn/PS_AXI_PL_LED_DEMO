# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: D:\My_Proj\ZYNQ\Demo\PS_AXI_PL_LED_Demo\PS_AXI_PL_LED_app_system\_ide\scripts\debugger_ps_axi_pl_led_app-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source D:\My_Proj\ZYNQ\Demo\PS_AXI_PL_LED_Demo\PS_AXI_PL_LED_app_system\_ide\scripts\debugger_ps_axi_pl_led_app-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Xilinx PYNQ-Z1 003017AD42CBA" && level==0 && jtag_device_ctx=="jsn-Xilinx PYNQ-Z1-003017AD42CBA-23727093-0"}
fpga -file D:/My_Proj/ZYNQ/Demo/PS_AXI_PL_LED_Demo/PS_AXI_PL_LED_app/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw D:/My_Proj/ZYNQ/Demo/PS_AXI_PL_LED_Demo/PS_AXI_PL_LED_platform/export/PS_AXI_PL_LED_platform/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source D:/My_Proj/ZYNQ/Demo/PS_AXI_PL_LED_Demo/PS_AXI_PL_LED_app/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow D:/My_Proj/ZYNQ/Demo/PS_AXI_PL_LED_Demo/PS_AXI_PL_LED_app/Debug/PS_AXI_PL_LED_app.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
