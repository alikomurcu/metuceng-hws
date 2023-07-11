#include "main.h"
#include "parser.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>

#define ls_folder 1
#define ls_file 0
#define wrong_path -1
#define cno_root 2
#define FAT_ENTRY_SIZE 4
#define mode_is_file 1
#define mode_is_folder 0
#define NUM_CLST 102400


using namespace std;

void concate_names();
unsigned int find_first_free_block(int cno);
unsigned int find_first_free_cluster();

typedef struct struct_ls{
    string name;
    uint16_t modifiedTime;         // Modification time with H:M:S format
    uint16_t modifiedDate;         // Modification date with Y:M:D format
    uint32_t fileSize;             // Filesize in bytes
    uint16_t type;
} struct_ls;


FILE *fptr;
//FatFileLFN lfn_file;
FatFileEntry file;
int lCt;
int cCt;
BPB_struct fat_bpb;

string filename;
char cat_char[32];

unsigned int free_clst_ptr;
unsigned int cur_cno;
unsigned int ls_cluster_no;
unsigned int cat_cluster_no;
unsigned int touch_cluster_no;
unsigned int mkdir_cluster_no;
unsigned int mv_cluster_no_src;
unsigned int mv_cluster_no_dst;

vector<string> find_dir_list;       // temporary global list for cd
vector<string> cur_dir_list;
parsed_input *pI;
bool isLFN;
bool unused;

vector<string> crdr_contents;
string tmp_names;

tm *calculate_time()
{
    time_t cur = time(nullptr);
    tm * loc_time = localtime(&cur);
    return loc_time;
}

unsigned char lfn_checksum(unsigned char *pFCBName)
{
    int i;
    unsigned char sum = 0;

    for (i = 11; i; i--)
        sum = ((sum & 1) << 7) + (sum >> 1) + *pFCBName++;

    return sum;
}

unsigned int find_dir_no(vector<string> path);

void str_toker(vector<string> *parsed_dir, string path)
{
    string tmp = "";
    string name = path;
    name += '/';

    for(auto e: name)
    {
        if(e == '/')
        {
            if(tmp != "")
                parsed_dir->push_back(tmp);
            tmp = "";
            continue;
        }
        else
        {
            tmp += e;
        }
    }
}

unsigned int get_cluster_size()
{
    return fat_bpb.BytesPerSector * fat_bpb.SectorsPerCluster;
}

// Returns the address of the first cluster of the data sector
unsigned int first_data_sec()
{
    return (fat_bpb.ReservedSectorCount + fat_bpb.NumFATs * fat_bpb.extended.FATSize) * fat_bpb.BytesPerSector;
}

unsigned int fat_sec()
{
    return fat_bpb.ReservedSectorCount * fat_bpb.BytesPerSector;
}

unsigned int go_cluster(unsigned int cno)       // subtracting should not cause an overflow !
{
    return first_data_sec() + (cno-2) * fat_bpb.SectorsPerCluster * fat_bpb.BytesPerSector;     // TODO: check cno-2
}

unsigned int go_fat_sec(unsigned int cno)
{
    return fat_sec() + cno * FAT_ENTRY_SIZE;       // FAT entry is 4 bytes
}

void write_to_fat(unsigned int cno)
{
    fseek(fptr, go_fat_sec(cno) ,  SEEK_SET);
    fwrite(&free_clst_ptr, 4, 1, fptr);     // next of cno is freeclstptr

    unsigned int minus_one = 0xFFFFFFFF;
    fseek(fptr, go_fat_sec(free_clst_ptr) ,  SEEK_SET);     // the next of the next is 0xFFFFFFFF
    fwrite(&minus_one, 4, 1, fptr);     // next of cno is freeclstptr
}

unsigned int find_next_from_FAT(unsigned int cno)
{
    unsigned int next_cluster;
    fseek(fptr, go_fat_sec(cno),  SEEK_SET);
    fread(&next_cluster, FAT_ENTRY_SIZE, 1, fptr);

    if(next_cluster >=  0x0FFFFFF8 || next_cluster == 0)       // EOC
        next_cluster = -1;      // 0xFFFFFFFF
    return next_cluster;
}

unsigned int file_reader_char(unsigned int cno)
{
    fseek(fptr, go_cluster(cno) + cCt++ * sizeof(cat_char),  SEEK_SET);
    fread(&cat_char, sizeof(cat_char), 1, fptr);
    if(file.lfn.attributes == 15)       // ??
        isLFN = true;
    else isLFN = false;
    unused = false;     // ??
    if(file.lfn.sequence_number == 0xE5)
        unused = true;
}


// Reads the lfns and the file in cluster: cno
unsigned int file_reader(unsigned int cno)
{
    fseek(fptr, go_cluster(cno) + lCt++ *sizeof(file),  SEEK_SET);
    fread(&file, sizeof(file), 1, fptr);
    if(file.lfn.attributes == 15)
        isLFN = true;
    else isLFN = false;
    unused = false;
    if(file.lfn.sequence_number == 0xE5)
        unused = true;
}

bool is_absolute(string name)
{
    if(name[0] == '/') return true;
    return false;
}

void one_dir_up(string *name, vector<string> *cur_list)
{
    vector<string> parsed_dir;

    str_toker(&parsed_dir, *name);

    for(auto & i : parsed_dir)
    {
        if(i == "..")
        {
            if(!cur_list->empty())
            cur_list->pop_back();
        }
        else if(i == ".")
        {
            ;
        }
        else
        {
            cur_list->push_back(i);
        }
    }
}

void crt_cur_dir(string name, vector<string> *crt_list)
{
    string tmp = "";
    name += '/';
    for(auto e: name)
    {
        if(e == '/')
        {
            if(tmp != "")
                crt_list->push_back(tmp);
            tmp = "";
            continue;
        }
        else
        {
            tmp += e;
        }
    }
}


// Truns given path to a path vector
// handles absolute path, .. , . , and relative paths
void go_dir(string name, vector<string> *go_list)       // TODO: ls . ls ./ must be handled
{
    if(is_absolute(name))
    {
        go_list->clear();
        crt_cur_dir(name, go_list);
    }
    else if(name.find("..") != -1 || name.find('.') != -1)       // edit cd ../.. later TODO recursive approach
        one_dir_up(&name, go_list);  // TODO: correct here
    else
    {
        crt_cur_dir(name, go_list);
    }
}

void cur_dir_print()
{
    if(cur_dir_list.empty())
        cout << "/";
    else
    {
        for(auto e: cur_dir_list)
            cout << "/" << e;
    }
    cout << "> ";
}

//Finds the directory in path and returns the cno of that directory, if can not returns -1
unsigned int find_dir_no(vector<string> path)
{
    unsigned int next_cluster;
    stack<string> tmp;
    string curname;     // TODO empty string?
    int it = 0;
    unsigned int cluster = cno_root;
    lCt = 0;
    int counted_cluster_rows = 0;

    while(true)
    {
        // Check if a cluster ends
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {   // TODO: encapsulate this if block to a function
            next_cluster = find_next_from_FAT(cluster);

            if(next_cluster == 0xFFFFFFFF)
            {
                return wrong_path;
            }
            cluster = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }

        // READ THE FILE
        file_reader(cluster);     // TODO: edit here curdir cno
        counted_cluster_rows++;
        bool is_root = false;
        if(path.empty())        // if path is = root
        {
//            if(file.lfn.sequence_number == 0x00)    // if root is empty
//                last_msdos.filename[1] = 0x20;

            is_root = true;
        }
        if(unused) continue;
        if(file.lfn.sequence_number == 0 && !is_root) { return wrong_path;}  // EOC TODO ??
        if(isLFN)
        {   // Store names for each lfn
            concate_names();
            tmp.push(tmp_names);
        }
        else        // Not lfn
        {
//            last_msdos = file.msdos;
            if(is_root)
                return cno_root;
            while(!tmp.empty())
            {
                curname += tmp.top();
                tmp.pop();
            }
            if(curname == path[it])     // if it is equals to entry in path
            {
                lCt = 0;
                cluster = file.msdos.firstCluster;
                it++;
                if(file.msdos.attributes != 16) return wrong_path;
                if(it == path.size())   // means that iteration on path has finished
                {
                    return cluster;
                }
            }
            curname = "";
        }
    }
}



void cd_task()
{
    lCt = 0;
    vector<string> tmp_cdlist;
    for(int i=0; i<cur_dir_list.size(); i++) tmp_cdlist.push_back(cur_dir_list[i]);     // Copy the cur dir list into temporary list

    if(pI->arg1 == nullptr)
    {
        cur_cno = cno_root;
        cur_dir_list.clear();
    }
    else
    {
        go_dir(pI->arg1, &cur_dir_list);
        int dir_no = find_dir_no(cur_dir_list);
        if(dir_no == wrong_path)
        {
            cur_dir_list.clear();
            for(int i=0; i<tmp_cdlist.size(); i++) cur_dir_list.push_back(tmp_cdlist[i]);       // copy back tmp to curdirlist if argument not found
        }
        else
        {
            cur_cno = dir_no;   // do nothing? TODO
        }
    }

}

void concate_names()
{
    char names123[13];
    for(int i=0; i<5; i++)  //name1
    {
        names123[i] = file.lfn.name1[i];
    }
    for(int i=0; i<6; i++)  //name1
    {
        names123[i+5] = file.lfn.name2[i];
    }
    for(int i=0; i<2; i++)  //name1
    {
        names123[i+11] = file.lfn.name3[i];
    }
    tmp_names = names123;
}

string date_calcor(uint16_t date)
{
    int day = date%32;
    date = date>>5;
    int month = date%16;
    string sday = to_string(day);
    if(sday.size() == 1)
        sday = "0" + sday;
    if(month == 0)
        return "January " + sday;
    if(month == 1)
        return "February " + sday;
    if(month == 2)
        return "March " + sday;
    if(month == 3)
        return "May " + sday;
    if(month == 4)
        return "April " + sday;
    if(month == 5)
        return "June " + sday;
    if(month == 6)
        return "July " + sday;
    if(month == 7)
        return "August " + sday;
    if(month == 8)
        return "September " + sday;
    if(month == 9)
        return "October " + sday;
    if(month == 10)
        return "November " + sday;
    if(month == 11)
        return "December " + sday;
}

string time_calcor(uint16_t time)
{
    int second = time%32;
    time = time >> 5;
    int minute = time%64;
    time = time >> 6;
    int hour = time%32;
    string shour = to_string(hour);
    string sminute = to_string(minute);
    if(shour.size() == 1)
        shour = "0" + shour;
    if(sminute.size() == 1)
        sminute = "0" + sminute;
    return shour + ":" + sminute;
}

bool ls_path()
{
    string path;
    // Parse the input path to the ls
    if(pI->arg1 == nullptr) return true;
    else if(pI->arg2 == nullptr && pI->arg1 && strcmp(pI->arg1, "-l") != 0)       // if arg1 not "-l" and arg2 is NULL
        path = pI->arg1;
    else if(pI->arg2 && strcmp(pI->arg1, "-l") == 0)
        path = pI->arg2;
    else if(strcmp(pI->arg1, "-l") == 0) return true;

    vector<string> parsed_dir = cur_dir_list;
//    str_toker(&parsed_dir, path);

    go_dir(path, &parsed_dir);


    int cluster = find_dir_no(parsed_dir);
    if(cluster != wrong_path)
    {
        ls_cluster_no = cluster;
        return true;
    }
    else return false;
}

void ls_task()
{
    vector<struct_ls> ffvec;
    vector<string> ls_names;
    stack<string> tmp;
    string curname;     // TODO empty string?
    ls_cluster_no = cur_cno;
    lCt = 0;

    unsigned int next_cluster;
    int counted_cluster_rows = 0;

    if(!ls_path())
        return;
    lCt = 0;
    if(pI->arg1 && !strcmp(pI->arg1, "-l"))     // ls -l
    {
        while(true)
        {
            if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
            {
                next_cluster = find_next_from_FAT(ls_cluster_no);
                if(next_cluster == 0xFFFFFFFF)
                {
                    break;
                }
                ls_cluster_no = next_cluster;
                next_cluster = find_next_from_FAT(next_cluster);
                counted_cluster_rows = 0;
                lCt = 0;
            }
            file_reader(ls_cluster_no);     // TODO: edit here curdir cno
            counted_cluster_rows++;

            if(unused) continue;
            if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
            if(isLFN)
            {   // Store names for each lfn
                concate_names();
                tmp.push(tmp_names);
            }
            else        // Not lfn
            {
                while(!tmp.empty())
                {
                    curname += tmp.top();
                    tmp.pop();
                }
                struct_ls tmp_folder;
                tmp_folder.fileSize = file.msdos.fileSize;
                tmp_folder.name = curname;
                tmp_folder.modifiedTime = file.msdos.modifiedTime;
                tmp_folder.modifiedDate = file.msdos.modifiedDate;

                if(file.msdos.attributes == 16)     // it is a directory
                {
                    tmp_folder.type = ls_folder;
                }
                else
                {
                    tmp_folder.type = ls_file;
                }
                if(file.msdos.filename[0] != '.')
                    ffvec.push_back(tmp_folder);
                curname = "";
            }
        }
        string att;
        unsigned int sz = ffvec.size();
        for(int i=0; i<sz; i++)
        {
            if(ffvec[i].type == ls_folder)
            {
                cout << "drwx------ 1 root root 0 " << date_calcor(ffvec[i].modifiedDate) << " " << time_calcor(ffvec[i].modifiedTime) << " " << ffvec[i].name;
            }

            else if(ffvec[i].type == ls_file)
            {
                cout << "-rwx------ 1 root root " << ffvec[i].fileSize;
                cout << " " << date_calcor(ffvec[i].modifiedDate) << " ";
                cout << time_calcor(ffvec[i].modifiedTime) << " ";
                cout << ffvec[i].name;
            }
            cout << "\n";
        }

    }
    else
    {
        while(true)
        {
            if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
            {
                next_cluster = find_next_from_FAT(ls_cluster_no);

                if(next_cluster == 0xFFFFFFFF)
                {
                    break;
                }
                ls_cluster_no = next_cluster;
                next_cluster = find_next_from_FAT(next_cluster);
                counted_cluster_rows = 0;
                lCt = 0;
            }
            file_reader(ls_cluster_no);     // TODO: edit here curdir cno
            counted_cluster_rows++;
            if(unused) continue;
            if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
            if(isLFN)
            {   // Store names for each lfn
                concate_names();
                tmp.push(tmp_names);
            }
            else        // Not lfn
            {
                while(!tmp.empty())
                {
                    curname += tmp.top();
                    tmp.pop();
                }
                if(file.msdos.filename[0] != '.')
                    ls_names.push_back(curname);
                curname = "";
            }
        }
        unsigned int sz = ls_names.size();
        for(int i=0; i<sz; i++)
        {

            cout << ls_names[i];
            if(i != sz-1 && ls_names[i] != "")
                cout << " ";
        }
        cout << "\n";
    }
}

void cat_file_contents()
{
    cCt = 0;
    int counted_cluster_rows = 0;
    unsigned int next_cluster;

    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cat_cluster_no);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            cat_cluster_no = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            cCt = 0;
        }


        file_reader_char(cat_cluster_no);     // TODO: edit here curdir cno
        counted_cluster_rows++;
        if(unused) continue;
        if(cat_char[0] == 0) { break;}  // EOC TODO ??
        for(int i=0; i<32; i++)
            if(cat_char[i] != 0)
                cout << cat_char[i];        // TODO: delete newlines at the end
    }
    cout << endl;
}

bool cat_path()
{
    string path;
    if(pI->arg1)
        path = pI->arg1;
    else return false;

    vector<string> parsed_dir = cur_dir_list;

    go_dir(path, &parsed_dir);


    int cluster = find_dir_no(parsed_dir);
    if(cluster != wrong_path)
    {
        cat_cluster_no = cluster;
        return true;
    }
    else return false;


    return true;
}

void cat_task()
{
    stack<string> tmp;
    string curname;     // TODO empty string?

    cat_cluster_no = cur_cno;
    int counted_cluster_rows = 0;
    unsigned int next_cluster;

    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cat_cluster_no);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            cat_cluster_no = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }
        file_reader(cat_cluster_no);     // TODO: edit here curdir cno
        counted_cluster_rows++;
        if(unused) continue;
        if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
        if(isLFN)
        {   // Store names for each lfn
            concate_names();
            tmp.push(tmp_names);
        }
        else        // Not lfn
        {
            while(!tmp.empty())
            {
                curname += tmp.top();
                tmp.pop();
            }
            if(strcmp(pI->arg1, curname.c_str()) == 0)      // file is found, print out its contents
            {
                cat_cluster_no = file.msdos.firstCluster;
                cat_file_contents();
            }
            curname = "";
        }
    }

}

bool is_file_in_directory(int cno, string file_name)
{
    unsigned int next_cluster;
    lCt = 0;
    int counted_cluster_rows = 0;
    stack<string> tmp;
    string curname = "";
    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(ls_cluster_no);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            ls_cluster_no = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }
        file_reader(cno);     // TODO: edit here curdir cno
        counted_cluster_rows++;
        if(unused) continue;
        if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
        if(isLFN)
        {   // Store names for each lfn
            concate_names();
            tmp.push(tmp_names);
        }
        else        // Not lfn
        {
            while(!tmp.empty())
            {
                curname += tmp.top();
                tmp.pop();
            }
            if(file_name == curname){
                return true;
            }
            curname = "";
        }
    }
    return false;

}

string touch_path()
{
    string path;
    if(pI->arg1)
        path = pI->arg1;
    else return "";

    vector<string> parsed_dir = cur_dir_list;

    go_dir(path, &parsed_dir);
//    int cluster = find_dir_no(parsed_dir);
//    if(cluster == wrong_path)
//        return filename;

    filename = parsed_dir[parsed_dir.size()-1];     // TODO
    parsed_dir.pop_back();      // delete the filename from file path

    int cluster = find_dir_no(parsed_dir);
    if(is_file_in_directory(cluster, filename))      // do not generate duplicates
    {
        touch_cluster_no = wrong_path;
        return filename;
    }
    if(cluster != wrong_path)
    {
        touch_cluster_no = cluster;
    }

    return filename;
}

void make_lfn_name(string name, unsigned char *lfn_name)
{
    for(int i=0; i < name.size(); i++)
        lfn_name[i] = name[i];
    lfn_name[name.size()] = '\0';
}
vector<FatFileLFN> create_lfn(string name)
{
    vector<string> name_vec_13;
    vector<FatFileLFN> LFN_vec;
    string tmp_name;
    unsigned char *lfn_name = new unsigned char[name.size()+1];
    make_lfn_name(name, lfn_name);
    uint8_t checksum = lfn_checksum(lfn_name);
    int sz = name.size();
    for(int i = 0; i<sz; i++) // traverse in reverse
    {
        tmp_name += name[i];
        if(tmp_name.size() == 13)
        {
            name_vec_13.push_back(tmp_name);
            tmp_name.clear();
        }
    }
    if(tmp_name.size() != 13) name_vec_13.push_back(tmp_name);
    // Now name_vec_13 includes the name in reverse direction 13 by 13

    int seq = name_vec_13.size();
    for(int i=0 ;i < name_vec_13.size(); i++)
    {
        string filename = name_vec_13[i];
        bool first_time = true;
        FatFileLFN lfn;
        lfn.sequence_number = seq--;
        if(i==0)    lfn.sequence_number |= 0b01000000;      // set bit 6

        lfn.attributes = 0x0F;
        lfn.firstCluster = 0;
        lfn.checksum = checksum;
        lfn.reserved = 0x00;

        int common_ct = 0;
        for(int k = 0; k<5; k++)
        {
            if(common_ct < filename.size())
                lfn.name1[k] = filename[common_ct++];
            else if(first_time)
            {
                lfn.name1[k] = 0;
                first_time = false;
            }
            else
            {
                lfn.name1[k] = -1;
            }
        }

        for(int k = 0; k<6; k++)
        {
            if(common_ct < filename.size())
                lfn.name2[k] = filename[common_ct++];
            else if(first_time)
            {
                lfn.name2[k] = 0;
                first_time = false;
            }
            else
            {
                lfn.name2[k] = -1;
            }
        }

        for(int k = 0; k<2; k++)
        {
            if(common_ct < filename.size())
                lfn.name3[k] = filename[common_ct++];
            else if(first_time)
            {
                lfn.name3[k] = 0;
                first_time = false;
            }
            else
            {
                lfn.name3[k] = -1;
            }
        }

        LFN_vec.push_back(lfn);
    }
    return LFN_vec;
}

string ms_dos_index_finder(int cno)
{
    FatFile83 last_msdos;
    last_msdos.filename[1] = 0x20;
    lCt = 0;
    unsigned int counted_cluster_rows = 0;
    unsigned int next_cluster;
    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cno);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            ls_cluster_no = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }
        file_reader(cno);     // TODO: edit here curdir cno
        if(file.lfn.sequence_number == 0x00) break;
        if(file.lfn.attributes != 15)   last_msdos = file.msdos;
        counted_cluster_rows++;
        if(unused) continue;
        if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
    }



    string cur_msdos_index = "";

    if(last_msdos.filename[0] == 0x00) return "0";
    for(int i=1; i<8; i++)      // the first entry is ~
    {
        if(last_msdos.filename[i] == 0x20 || (last_msdos.filename[0] == 0x2E && last_msdos.filename[1] == 0x2E))        // if the last msdos is root or an empty-allocated cluster
        {
            return cur_msdos_index;
        }
        cur_msdos_index += last_msdos.filename[i];
    }
    for(int i=0; i<3; i++)
    {
        if(last_msdos.filename[i] == 0x20) {
            return cur_msdos_index;
        }
        cur_msdos_index += last_msdos.filename[i];
    }
}

void add_dot_double_dot_to_folder(int cno)
{
    /////////////   DOT   /////////////
    FatFile83 dot;// TODO
    dot.filename[0] = '.';
    dot.attributes = 0x10;
    dot.reserved = 0x00;
    tm * time = calculate_time();
    dot.creationTimeMs = 0;
    dot.creationTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    dot.creationDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;
    dot.lastAccessTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    dot.fileSize = 0;

    dot.modifiedTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    dot.modifiedDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;

    dot.firstCluster = free_clst_ptr % 65536;
    dot.eaIndex = free_clst_ptr >> 16;

    for(int i=1; i<11; i++) dot.filename[i] = 0x20;
    fseek(fptr, go_cluster(free_clst_ptr),  SEEK_SET);
    fwrite(&dot, sizeof(FatFileLFN), 1, fptr);

    /////////////   DOUBLE DOT   /////////////
    FatFile83 ddot;// TODO
    ddot.filename[0] = '..';
    ddot.attributes = 0x10;
    ddot.reserved = 0x00;
    time = calculate_time();
    ddot.creationTimeMs = 0;
    ddot.creationTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    ddot.creationDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;
    ddot.lastAccessTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    ddot.fileSize = 0;

    ddot.modifiedTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    ddot.modifiedDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;

    ddot.firstCluster = cno % 65536;
    ddot.eaIndex = cno >> 16;

    for(int i=1; i<11; i++) ddot.filename[i] = 0x20;
    fseek(fptr, go_cluster(free_clst_ptr) + sizeof(struct_FatFile83),  SEEK_SET);
    fwrite(&ddot, sizeof(FatFileLFN), 1, fptr);
}

void make_msdos(FatFile83 *msdos, int cno)
{
    string msdos_str = ms_dos_index_finder(cno);
    if(msdos_str == "") msdos_str = '0';
    int num = stoi(msdos_str);
    num ++;

    msdos_str = to_string(num);
    msdos->filename[0] = 0x7E;
    int i=1;
    for(i=1; i<8; i++)
    {
        if(i-1<msdos_str.size())
            msdos->filename[i] = msdos_str[i-1];      // TODO
        else msdos->filename[i] = 0x20;
    }
    for(int j=0; j<3; j++)
    {
        if(i-1<msdos_str.size())
            msdos->extension[j] = msdos_str[i++-1];
        else msdos->extension[j] = 0x20;
    }
    tm * time = calculate_time();
    msdos->lastAccessTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;

    msdos->modifiedTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    msdos->modifiedDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;
}

FatFile83 create_msdos(int mode, int cno)
{
    string msdos_str = ms_dos_index_finder(cno);
    if(msdos_str == "") msdos_str = '0';
    int num = stoi(msdos_str);
    num ++;

    msdos_str = to_string(num);
    FatFile83 msdos;
    msdos.filename[0] = 0x7E;
    int i=1;
    for(i=1; i<8; i++)
    {
        if(i-1<msdos_str.size())
            msdos.filename[i] = msdos_str[i-1];      // TODO
        else msdos.filename[i] = 0x20;
    }
    for(int j=0; j<3; j++)
    {
        if(i-1<msdos_str.size())
            msdos.extension[j] = msdos_str[i++-1];
        else msdos.extension[j] = 0x20;
    }
    if(mode == mode_is_file)
    {
        msdos.attributes = 0x20;        // TODO CHECK
        msdos.firstCluster = 0;        // TODO
        msdos.fileSize = 0x00;
        msdos.eaIndex = -1;
    }
    if(mode == mode_is_folder)
    {
        msdos.attributes = 0x10;
        find_first_free_cluster();      // TODO add . and .. entries.
        add_dot_double_dot_to_folder(cno);
        msdos.fileSize = 0x00;      // TODO
        msdos.eaIndex = free_clst_ptr >> 16;     // TODO freecluster
        msdos.firstCluster = free_clst_ptr % 65536;

    }

    msdos.reserved = 0x00;
    tm * time = calculate_time();
    msdos.creationTimeMs = 0;
    msdos.creationTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    msdos.creationDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;
    msdos.lastAccessTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;

    msdos.modifiedTime = (time->tm_hour<<11) + (time->tm_min<<5) + time->tm_sec;
    msdos.modifiedDate = (time->tm_year<<9) + (time->tm_mon<<5) + time->tm_mday;
    return msdos;
}

unsigned int find_first_free_block(unsigned int *cno)
{
    FatFile83 zero_byte;
    unsigned int counted_cluster_rows = 0;
    int next_cluster;
    int i=0;
    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(*cno);

            if(next_cluster == 0xFFFFFFFF) // nest cluster is -1 so allocate a new one
            {
                find_first_free_cluster();
                write_to_fat(*cno);     // edit the next of *cno as free_cluster_ptr
                *cno = free_clst_ptr;
                break;
            }

            *cno = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            i = 0;
        }
        fseek(fptr, go_cluster(*cno) + (counted_cluster_rows * (get_cluster_size() / sizeof(file))),  SEEK_SET);     // This part was erroneous TODO
        fread(&zero_byte, sizeof(FatFile83), 1, fptr);

        if(zero_byte.filename[0] == 0x00)
        {
            return counted_cluster_rows;
        }
        counted_cluster_rows++;
    }
}

unsigned int find_first_free_cluster()        // TODO: edit here
{
    int zero_byte;
    int total_loop_ct = 0;
    while(total_loop_ct<NUM_CLST)       // iterate at most # data region clusters
    {
        fseek(fptr, go_cluster(free_clst_ptr),  SEEK_SET);
        fread(&zero_byte, 4, 1, fptr);
        if(zero_byte == 0x00)
        {
            return free_clst_ptr;
        }
        else
        {
            free_clst_ptr = (free_clst_ptr + 1) % NUM_CLST;
            if(free_clst_ptr == 0) free_clst_ptr = cno_root;
        }
        total_loop_ct++;
    }
}

void add_lfn_msdos_to_cluster(string name, int mode)
{
    vector<FatFileLFN> lfn_vec = create_lfn(name);
    reverse(lfn_vec.begin(), lfn_vec.end());
    int next_cluster;
    unsigned int cno;
    if(mode == mode_is_file) cno = touch_cluster_no;
    else cno = mkdir_cluster_no;
    unsigned int counted_cluster_rows = find_first_free_block(&cno);     // Handle here it does not returns the first free block
    int i = 0;
    while(i<lfn_vec.size()+1)     // size + 1  because it writes msdos at the end
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cno);

            if(next_cluster == 0xFFFFFFFF)      // i.e., -1
            {
                if(i== lfn_vec.size()+1) return;        // if msdos has also passed
                next_cluster = find_first_free_cluster();       // the first free cluster ptr
                write_to_fat(cno);
                // New cluster allocated!
            }
            counted_cluster_rows = 0;
            cno = next_cluster;
            //next_cluster = find_next_from_FAT(next_cluster);
        }
        if(i<lfn_vec.size())
        {
            fseek(fptr, go_cluster(cno) + counted_cluster_rows *sizeof(FatFileLFN),  SEEK_SET);
            fwrite(&lfn_vec[i], sizeof(FatFileLFN), 1, fptr);
        }
        else        // only iterate once -- in the end i = lfn_vec.size()
        {
            FatFile83 msdos = create_msdos(mode, cno);
            fseek(fptr, go_cluster(cno) + counted_cluster_rows *sizeof(FatFile83),  SEEK_SET);
            fwrite(&msdos, sizeof(FatFile83), 1, fptr);
        }
        counted_cluster_rows++;
        i++;
    }
}

void touch_task()
{
    string filename = touch_path();
    if(touch_cluster_no == wrong_path) return;
    add_lfn_msdos_to_cluster(filename, mode_is_file);
}

string mkdir_path()
{
    string path;
    if(pI->arg1)
        path = pI->arg1;
    else return "";

    vector<string> parsed_dir = cur_dir_list;

    go_dir(path, &parsed_dir);

    filename = parsed_dir[parsed_dir.size()-1];     // TODO
    parsed_dir.pop_back();      // delete the filename from file path

    int cluster = find_dir_no(parsed_dir);
    if(is_file_in_directory(cluster, filename))      // do not generate duplicates
    {
        mkdir_cluster_no = wrong_path;
        return filename;
    }
    if(cluster != wrong_path)
    {
        mkdir_cluster_no = cluster;
    }

    return filename;
}

void mkdir_task()
{
    string filename = mkdir_path();
    if(mkdir_cluster_no == wrong_path) return;
    add_lfn_msdos_to_cluster(filename, mode_is_folder);
}

vector<FatFileEntry> find_lfn_msdos_from_src(string filename, unsigned int cno)
{
    vector<struct_ls> ffvec;
    vector<string> ls_names;
    stack<string> tmp;
    string curname;     // TODO empty string?
    ls_cluster_no = cur_cno;
    lCt = 0;
    unsigned int next_cluster;
    int counted_cluster_rows = 0;
    vector<FatFileEntry> tmp_ffes;      // last entry is msdos, others are lfns
    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cno);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            cno = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }
        file_reader(cno);     // TODO: edit here curdir cno
        counted_cluster_rows++;
        if(unused) continue;
        if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
        if(isLFN)
        {   // Store names for each lfn
            concate_names();
            tmp.push(tmp_names);
            tmp_ffes.push_back(file);       // push lfns to tmp vector
        }
        else        // Not lfn
        {
            while(!tmp.empty())
            {
                curname += tmp.top();
                tmp.pop();
            }

            if(filename == curname)     // if found filename correctly, store lfns and msdos in vector 
            {
                tmp_ffes.push_back(file);       // this one is msdos
                return tmp_ffes;
            }
            tmp_ffes.clear();
            curname = "";
        }
    }
}

void clear_src_lfns_msdos(vector<FatFileEntry> to_be_deleted, unsigned int cno)
{       // TODO: update fat table while deleting
    lCt = 0;
    unsigned int next_cluster;
    int counted_cluster_rows = 0;
    int target_chcksum = to_be_deleted[0].lfn.checksum;
    bool found = false;
    while(true)
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cno);

            if(next_cluster == 0xFFFFFFFF)
            {
                break;
            }
            cno = next_cluster;
            next_cluster = find_next_from_FAT(next_cluster);
            counted_cluster_rows = 0;
            lCt = 0;
        }
        file_reader(cno);     // TODO: edit here curdir cno
        if(unused) continue;
        if(file.lfn.sequence_number == 0) { break;}  // EOC TODO ??
        if(isLFN)
        {   // Check if cheksum is equal
            if(file.lfn.checksum == target_chcksum)
            {
                file.lfn.sequence_number = 0xE5;        // i.e., delete this lfn    must write to it
                fseek(fptr, go_cluster(cno) + counted_cluster_rows * sizeof(FatFileLFN),  SEEK_SET);
                fwrite(&file.lfn.sequence_number, sizeof(file.lfn.sequence_number), 1, fptr);
                found = true;
            }
        }
        else        // Not lfn
        {
            if(found)
            {
                file.msdos.filename[0] = 0xE5;          //i.e., delete this msdos
                fseek(fptr, go_cluster(cno) + counted_cluster_rows * sizeof(FatFileLFN),  SEEK_SET);
                fwrite(&file.msdos.filename[0], sizeof(file.msdos.filename[0]), 1, fptr);
            }
        }
        counted_cluster_rows++;
    }

}

void write_to_dst(vector<FatFileEntry> to_be_added, unsigned int cno)
{
    unsigned int next_cluster;
    unsigned int counted_cluster_rows = find_first_free_block(&cno);     // Handle here it does not returns the first free block
    int i = 0;
    while(i<to_be_added.size()+1)     // size + 1  because it writes msdos at the end
    {
        if(counted_cluster_rows == get_cluster_size() / sizeof(file))       // 1024 / 32 = 32
        {
            next_cluster = find_next_from_FAT(cno);

            if(next_cluster == 0xFFFFFFFF)      // i.e., -1
            {
                if(i== to_be_added.size()+1) return;        // if msdos has also passed
                next_cluster = find_first_free_cluster();       // the first free cluster ptr
                write_to_fat(cno);
                // New cluster allocated!
            }
            counted_cluster_rows = 0;
            cno = next_cluster;
            //next_cluster = find_next_from_FAT(next_cluster);
        }
        if(i<to_be_added.size())
        {
            fseek(fptr, go_cluster(cno) + counted_cluster_rows *sizeof(FatFileLFN),  SEEK_SET);
            fwrite(&to_be_added[i].lfn, sizeof(FatFileLFN), 1, fptr);
        }
        else        // only iterate once -- in the end i = to_be_added.size()
        {
            make_msdos(&to_be_added[i].msdos, cno);
            fseek(fptr, go_cluster(cno) + counted_cluster_rows *sizeof(FatFile83),  SEEK_SET);
            fwrite(&to_be_added[i].msdos, sizeof(FatFile83), 1, fptr);
        }
        counted_cluster_rows++;
        i++;
    }

}

string mv_path()
{
    string src_path, dst_path;
    if(pI->arg1)
        src_path = pI->arg1;
    if(pI->arg2)
        dst_path = pI->arg2;
    if(pI->arg1 == nullptr || pI->arg2 == nullptr)
    return "";

    vector<string> parsed_dir_src = cur_dir_list;
    vector<string> parsed_dir_dst = cur_dir_list;

////////////////////////////////////////////////////// SRC //////////////////////////////////////////////////////
    go_dir(src_path, &parsed_dir_src);   // append src to current path

    filename = parsed_dir_src[parsed_dir_src.size()-1];     // TODO src filename
    parsed_dir_src.pop_back();

    int cluster_src = find_dir_no(parsed_dir_src);

    if(cluster_src != wrong_path)
    {
        mv_cluster_no_src = cluster_src;
    }
    if(cluster_src == wrong_path) return "";
////////////////////////////////////////////////////// DST //////////////////////////////////////////////////////

    go_dir(dst_path, &parsed_dir_dst);   // append src to current path

    int cluster_dst = find_dir_no(parsed_dir_dst);
    if(is_file_in_directory(cluster_dst, filename))      // do not generate duplicates
    {
        mkdir_cluster_no = wrong_path;
        return filename;
    }

    if(cluster_dst != wrong_path)
    {
        mv_cluster_no_dst = cluster_dst;
    }

    if(cluster_dst == wrong_path)   return "";
    return filename;
}

void mv_task()
{
    string filename = mv_path();
    if(filename == "") return;
    vector<FatFileEntry> lfns_msdos = find_lfn_msdos_from_src(filename, mv_cluster_no_src);
    clear_src_lfns_msdos(lfns_msdos, mv_cluster_no_src);
    write_to_dst(lfns_msdos, mv_cluster_no_dst);
}

void read_img()
{
    // Read img file into bpb
    fptr = fopen("/home/witcha/example.img", "r+"); //TODO: edit here later
    fread(&fat_bpb, sizeof(fat_bpb), 1, fptr);     // TODO 3,1
}

void take_input()
{
    string line;
    std::getline (std::cin, line);
    //cout << "\n" << line;
    char chrLine[1024];     // TODO: check this after,
    strcpy(chrLine, line.c_str());
    pI = new parsed_input;
    if(chrLine[0] == 0x00) return;
    parse(pI, chrLine);
}

int main()
{
    unused = false;
    isLFN = true;
    lCt = 0;
    cCt = 0;
    read_img();
    cur_cno = cno_root;
    free_clst_ptr = cno_root;
    while(true)
    {
        lCt = 0;
        cur_dir_print();
        // input task
        take_input();
        if(pI->type == QUIT)
        {
            //delete pI;
            break;
        }
        else if(pI->type == CD)
        {
            cd_task();
        }
        else if(pI->type == LS)
        {
            ls_task();
        }
        else if(pI->type == MKDIR)
        {
            mkdir_task();
        }
        else if(pI->type == TOUCH)
        {
            touch_task();
        }
        else if(pI->type == MV)
        {
            mv_task();
        }
        else if(pI->type == CAT)
        {
            cat_task();
        }
        //delete pI;
    }

    return 0;
}