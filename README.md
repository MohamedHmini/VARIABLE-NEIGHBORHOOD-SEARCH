# VNS-SOLVER : [IMPLEMENTED IN C LANGUAGE]

### SOLVING A 'P||Cmax' SCHEDULING PROBLEM
to compile the scheduler program you should run the following bash code :
```bash
gcc -o main ./HLCio/dataframe.c ./HLCio/advio.c ./VNS.c ./scheduler.c
```
to execute :
```bash
./main <name of the dataset> <GVNS stopping condition (nbr of allowed failure iterations)>
```
e.g :
```bash
./main ./datasets/c500t3.txt 10
```
