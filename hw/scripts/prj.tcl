open_project layth/layth.xpr

export_simulation \
    -directory "./"    \
    -simulator xsim \
    -ip_user_files_dir "./layth/layth.ip_user_files/"

close_project
