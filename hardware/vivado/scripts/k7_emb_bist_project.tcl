start_gui
set version "2013.2"
set project_name "k7_emb_bist_${version}"
set project_dir "project"
set ip_package_dir "../../../sources/ip_package"
set hdl_dir "../../../sources/hdl"
set ui_dir "../../../sources/ui"
set constrs_dir "../../../sources/constraints"
set scripts_dir "../../scripts"
set bd_name "k7_emb_bist"
set part "xc7k325tffg900-2"
set board "xilinx.com:kintex7:kc705:1.0"

# set up project
create_project $project_name $project_dir -part $part -force
set_property board $board [current_project]

# set up IP repo
set_property ip_repo_paths $ip_package_dir [current_fileset]
update_ip_catalog -rebuild

# set up bd design
create_bd_design $bd_name
source $scripts_dir/${bd_name}_bd.tcl
validate_bd_design
save_bd_design
close_bd_design $bd_name

# add hdl sources and xdc constraints to project
add_files -fileset sources_1 -norecurse $hdl_dir/${bd_name}_top.v
add_files -fileset constrs_1 -norecurse $constrs_dir/${bd_name}_top.xdc
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

# apply UI file
file copy -force $ui_dir/bd_99380920.ui $project_dir/$project_name.srcs/sources_1/bd/$bd_name/ui/bd_99380920.ui

# re-open bd design with layout and comments
open_bd_design $project_dir/$project_name.srcs/sources_1/bd/$bd_name/$bd_name.bd

### For user reference ####
##reset_run impl_1
##launch_runs synth_1
##wait_on_run synth_1
##launch_runs impl_1 -to_step write_bitstream
##wait_on_run impl_1
##puts "Implementation process is complete !"
##open_run impl_1
##open_bd_design {project/k7_emb_bist_2013.2.srcs/sources_1/bd/k7_emb_bist/k7_emb_bist.bd}
##export_hardware [get_files project/k7_emb_bist_2013.2.srcs/sources_1/bd/k7_emb_bist/k7_emb_bist.bd] [get_runs impl_1] -bitstream
##close_project
##exit

