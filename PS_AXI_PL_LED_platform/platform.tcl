# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct D:\My_Proj\ZYNQ\Demo\PS_AXI_PL_LED_Demo\PS_AXI_PL_LED_platform\platform.tcl
# 
# OR launch xsct and run below command.
# source D:\My_Proj\ZYNQ\Demo\PS_AXI_PL_LED_Demo\PS_AXI_PL_LED_platform\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {PS_AXI_PL_LED_platform}\
-hw {D:\My_Proj\ZYNQ\Demo\PS_AXI_PL_LED_Demo\design_1_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -fsbl-target {psu_cortexa53_0} -out {D:/My_Proj/ZYNQ/Demo/PS_AXI_PL_LED_Demo}

platform write
platform generate -domains 
platform active {PS_AXI_PL_LED_platform}
bsp reload
platform generate
