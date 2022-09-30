import xlrd
#please use xlrd==1.2.0,in 2.0 version,xlsx is not supported
import os

curr_dir_path=os.path.dirname(__file__)
x86_32_path=os.path.join(curr_dir_path,"x86-32ins.xlsx")
x86_64_path=os.path.join(curr_dir_path,"x86-64ins.xlsx")
x86_32_ins_raw_c_path=os.path.join(curr_dir_path,"32_ins_raw.c")
x86_64_ins_raw_c_path=os.path.join(curr_dir_path,"64_ins_raw.c")
def main():
    x86_32_file=xlrd.open_workbook(x86_32_path)
    x86_64_file=xlrd.open_workbook(x86_64_path)
    x86_32_sheet_names=x86_32_file.sheet_names()[0]
    x86_64_sheet_names=x86_64_file.sheet_names()[0]
    x86_32_table=x86_32_file.sheet_by_name(x86_32_sheet_names)
    x86_64_table=x86_64_file.sheet_by_name(x86_64_sheet_names)
    x86_32_ins_raw_c=open(x86_32_ins_raw_c_path,"w")
    x86_64_ins_raw_c=open(x86_64_ins_raw_c_path,"w")

    rownum=x86_32_table.nrows
    columnnum=x86_32_table.ncols
    x86_32_ins_raw_c.write("#include \"x86_backend.h\"\n")
    x86_32_ins_raw_c.write("struct x86_ins_raw x86_32_ins_raw[MAX_INS_RAW_NUM]=")
    x86_32_ins_raw_c.write("{\n")
    for i in range(1,rownum):
        x86_32_ins_raw_c.write("{")
        for j in range(columnnum):
            x86_32_ins_raw_c.write("\"")
            s=x86_32_table.cell_value(i,j)
            stype=x86_32_table.cell(i,j).ctype
            if stype==2:
                s=int(s)
                s=str(s)
            x86_32_ins_raw_c.write(str(s))
            x86_32_ins_raw_c.write("\",")
        x86_32_ins_raw_c.write("},\n")
    x86_32_ins_raw_c.write("};\n")
    
    rownum=x86_64_table.nrows
    columnnum=x86_64_table.ncols
    x86_64_ins_raw_c.write("#include \"x86_backend.h\"\n")
    x86_64_ins_raw_c.write("struct x86_ins_raw x86_64_ins_raw[MAX_INS_RAW_NUM]=")
    x86_64_ins_raw_c.write("{\n")
    for i in range(1,rownum):
        x86_64_ins_raw_c.write("{")
        for j in range(columnnum):
            x86_64_ins_raw_c.write("\"")
            s=x86_64_table.cell_value(i,j)
            stype=x86_64_table.cell(i,j).ctype
            if stype==2:
                s=int(s)
                s=str(s)
            x86_64_ins_raw_c.write(str(s))
            x86_64_ins_raw_c.write("\",")
        x86_64_ins_raw_c.write("},\n")
    x86_64_ins_raw_c.write("};\n")
    
    x86_32_ins_raw_c.close()
    x86_64_ins_raw_c.close()
if __name__ =='__main__':
    main()