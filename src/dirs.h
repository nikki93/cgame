#ifndef DIRS_H
#define DIRS_H

#ifndef DATA_DIR
#define DATA_DIR "./data/"
#endif

#ifndef USR_DIR
#define USR_DIR "./usr/"
#endif

#define data_path(path) (DATA_DIR path)
#define usr_path(path) (USR_DIR path)

#endif

